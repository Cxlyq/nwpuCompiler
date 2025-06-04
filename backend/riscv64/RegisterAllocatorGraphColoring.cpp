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

void GraphColoringRegisterAllocator::addInterference(Value * a, Value * b)
{
    if (a == b)
        return; // 不与自己冲突
    interferenceGraph[a].insert(b);
    interferenceGraph[b].insert(a);
}

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

void GraphColoringRegisterAllocator::select()
{
    // 什么都不做，等 assignColors 实际分配寄存器
    // 或者你可以预处理溢出变量在 assignColors 中跳过它们
}

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

int GraphColoringRegisterAllocator::getRegister(Value * val) const
{
    auto it = colorMap.find(val);
    if (it != colorMap.end()) {
        return it->second;
    }
    return -1; // -1 表示没有分配寄存器（可能溢出）
}

const std::vector<Value *> & GraphColoringRegisterAllocator::getSpilled() const
{
    return spilled;
}

void GraphColoringRegisterAllocator::buildGraph(const LiveVariableAnalysis & lva)
{
    for (auto * block: lva.getBasicBlocks()) {
        std::unordered_set<Value *> live = lva.getLiveOut(block->label); // 拷贝 liveOut

        // 反向遍历指令更贴近算法语义（自底向上）
        for (auto it = block->instructions.rbegin(); it != block->instructions.rend(); ++it) {
            Instruction * inst = *it;

            if (inst->hasResultValue()) {
                Value * def = inst;

                for (Value * val: live) {
                    if (val != def) {
                        addInterference(def, val);
                    }
                }

                live.erase(def); // 删除定义的变量，防止后续干扰
            }

            // 将当前指令使用的变量加入 live 集合
            for (Use * use: inst->getOperands()) {
                Value * operand = use->getUsee();
                live.insert(operand);
            }
        }
    }
}
