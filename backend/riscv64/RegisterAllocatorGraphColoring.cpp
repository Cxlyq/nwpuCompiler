///
/// @file RegisterAllocatorGraphColoring.cpp
/// @brief 基于图着色的寄存器分配器
/// @author wangjq
/// @version 1.0
/// @date 2025-06-03
///
/// @copyright Copyright (c) 2025
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2025-06-03 <td>1.0     <td>wangjq  <td>新建
/// </table>
///

#include "RegisterAllocatorGraphColoring.h"
#include <algorithm>
#include <iostream>

GraphColoringRegisterAllocator::GraphColoringRegisterAllocator(int regCount) : regCount(regCount)
{}

/// @brief 为干涉图添加一条无向边（两个变量互相干涉）
/// @param a 第一个变量
/// @param b 第二个变量
void GraphColoringRegisterAllocator::addInterference(Value * a, Value * b)
{
    if (a == b)
        return; // 不与自己冲突
    interferenceGraph[a].insert(b);
    interferenceGraph[b].insert(a);
}

/// @brief 执行图着色寄存器分配流程
/// @return 分配是否成功，true 表示无溢出，false 表示有变量溢出寄存器
bool GraphColoringRegisterAllocator::allocate()
{
    simplifyStack = std::stack<Value *>();
    spilled.clear();
    colorMap.clear();

    if (!simplify()) {
        // 所有点都无法简化，强制溢出一个点
        auto it = interferenceGraph.begin();
        if (it != interferenceGraph.end()) {
            spilled.push_back(it->first);
            interferenceGraph.erase(it);
        }
    }

    select();
    assignColors();

    return spilled.empty();
}

/// @brief 简化干涉图，构建简化栈
/// @return 是否成功简化（若为 false，说明需溢出）
bool GraphColoringRegisterAllocator::simplify()
{
    std::unordered_map<Value *, std::unordered_set<Value *>> tempGraph = interferenceGraph;
    std::unordered_set<Value *>                              removed;

    bool progress = false;

    while (true) {
        bool found = false;

        for (auto & entry: tempGraph) {
            Value * node = entry.first;
            if (removed.count(node))
                continue;

            const auto & neighbors = entry.second;

            int degree = 0;
            for (Value * n: neighbors) {
                if (!removed.count(n))
                    degree++;
            }

            if (degree < regCount) {
                simplifyStack.push(node);
                removed.insert(node);
                found = true;
                progress = true;
            }
        }

        if (!found)
            break;
    }

    // 如果还有剩余节点没简化（可能全都度数 >= regCount），记录为溢出
    for (const auto & entry: tempGraph) {
        if (!removed.count(entry.first)) {
            spilled.push_back(entry.first);
        }
    }

    return progress;
}

/// @brief 预处理选择阶段（此实现中为空）
///        可用于未来溢出处理或寄存器预分配
void GraphColoringRegisterAllocator::select()
{
    // 什么都不做，等 assignColors 实际分配寄存器
    // 或者你可以预处理溢出变量在 assignColors 中跳过它们
}

/// @brief 给简化栈中的变量分配实际寄存器编号（图着色）
void GraphColoringRegisterAllocator::assignColors()
{
    while (!simplifyStack.empty()) {
        Value * node = simplifyStack.top();
        simplifyStack.pop();

        std::unordered_set<int> usedColors;
        for (Value * neighbor: interferenceGraph[node]) {
            if (colorMap.count(neighbor)) {
                usedColors.insert(colorMap[neighbor]);
            }
        }

        // 找一个未使用的颜色（寄存器）
        int color = -1;
        for (int i = 0; i < regCount; ++i) {
            if (!usedColors.count(i)) {
                color = i;
                break;
            }
        }

        if (color == -1) {
            spilled.push_back(node);
        } else {
            colorMap[node] = color;
        }
    }
}

/// @brief 获取指定变量分配到的寄存器编号
/// @param val 变量指针
/// @return 寄存器编号，若为 -1 则表示未分配（可能已溢出）
int GraphColoringRegisterAllocator::getRegister(Value * val) const
{
    auto it = colorMap.find(val);
    if (it != colorMap.end()) {
        return it->second;
    }
    return -1; // -1 表示没有分配寄存器（可能溢出）
}

/// @brief 获取所有溢出（未分配成功）变量
/// @return 包含溢出变量指针的向量引用
const std::vector<Value *> & GraphColoringRegisterAllocator::getSpilled() const
{
    return spilled;
}

/// @brief 根据活跃变量分析结果构建干涉图
/// @param lva 活跃变量分析的结果对象
void GraphColoringRegisterAllocator::buildGraph(const LiveVariableAnalysis & lva)
{
    for (auto * block: lva.getBasicBlocks()) {
        std::unordered_set<Value *> live = lva.getLiveOut(block->label); // 初始化 live 集

        std::cout << ">>> BasicBlock: " << block->label << "\n";
        std::cout << "Initial LiveOut: ";
        for (auto * v: live)
            std::cout << v->getIRName() << " ";
        std::cout << "\n";

        for (auto it = block->instructions.rbegin(); it != block->instructions.rend(); ++it) {
            Instruction * inst = *it;

            if (inst->hasResultValue()) {
                Value * def = inst;
                std::cout << "  DEF: " << def->getIRName() << "\n";

                for (Value * val: live) {
                    if (val != def) {
                        addInterference(def, val);
                        std::cout << "    Interfere: " << def->getIRName() << " <--> " << val->getIRName() << "\n";
                    }
                }

                live.erase(def);
            }

            // 添加使用的变量到 live
            for (Use * use: inst->getOperands()) {
                Value * operand = use->getUsee();
                if (operand->getValueCategory() == ValueCategory::IMMEDIATE)
                    continue;
                if (operand->getType()->isVoidType())
                    continue;
                live.insert(operand);
                std::cout << "  USE: " << operand->getIRName() << "\n";
            }

            std::cout << "  Updated live set: ";
            for (auto * v: live)
                std::cout << v->getIRName() << " ";
            std::cout << "\n";
        }

        std::cout << "Final Live for block " << block->label << ": ";
        for (auto * v: live)
            std::cout << v->getIRName() << " ";
        std::cout << "\n\n";
    }

    // 输出最终干涉图
    std::cout << "=== Interference Graph ===\n";
    for (auto & [v, neighbors]: interferenceGraph) {
        std::cout << v->getIRName() << " : ";
        for (auto * n: neighbors)
            std::cout << n->getIRName() << " ";
        std::cout << "\n";
    }
}

/// @brief 获取所有变量的寄存器分配映射
/// @return 变量到寄存器编号的映射
std::unordered_map<Value *, int> GraphColoringRegisterAllocator::getColorMap() const
{
    return colorMap;
}
