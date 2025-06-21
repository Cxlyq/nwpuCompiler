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
#include "PlatformRiscV64.h"
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
    if (a->getType()->isFloatType()) {
        floatInterferenceGraph[a].insert(b);
        floatInterferenceGraph[b].insert(a);
    } else {
        intInterferenceGraph[a].insert(b);
        intInterferenceGraph[b].insert(a);
    }
}

/// @brief 执行图着色寄存器分配流程
/// @return 分配是否成功，true 表示无溢出，false 表示有变量溢出寄存器
bool GraphColoringRegisterAllocator::allocate()
{
    simplifyIntStack = std::stack<Value *>();
    simplifyFloatStack = std::stack<Value *>();

    spilledInt.clear();
    spilledFloat.clear();

    colorMap.clear();

    if (!simplify(intInterferenceGraph, simplifyIntStack, spilledInt)) {
        // 所有点都无法简化，强制溢出一个点
        auto it = intInterferenceGraph.begin();
        if (it != intInterferenceGraph.end()) {
            spilledInt.push_back(it->first);
            intInterferenceGraph.erase(it);
        }
    }
    if (!simplify(floatInterferenceGraph, simplifyFloatStack, spilledFloat)) {
        // 所有点都无法简化，强制溢出一个点
        auto it = floatInterferenceGraph.begin();
        if (it != floatInterferenceGraph.end()) {
            spilledFloat.push_back(it->first);
            floatInterferenceGraph.erase(it);
        }
    }
    select();
    assignColors(intInterferenceGraph, simplifyIntStack);
    assignColors(floatInterferenceGraph, simplifyFloatStack);

    // 输出寄存器分配映射
    std::cout << "=== Register Allocation Result ===\n";
    for (const auto & entry: colorMap) {
        Value * val = entry.first;
        int     regIndex = entry.second;
        std::cout << val->getIRName() << " -> r" << regIndex << "\n";
    }

    if (!spilledInt.empty()) {
        std::cout << "=== Spilled Int Variables ===\n";
        for (auto * val: spilledInt) {
            std::cout << val->getIRName() << "\n";
        }
    }

    if (!spilledFloat.empty()) {
        std::cout << "=== Spilled Float Variables ===\n";
        for (auto * val: spilledFloat) {
            std::cout << val->getIRName() << "\n";
        }
    }
    return spilledInt.empty() && spilledFloat.empty();
}

/// @brief 简化干涉图，构建简化栈
/// @return 是否成功简化（若为 false，说明需溢出）
bool GraphColoringRegisterAllocator::simplify(
    const std::unordered_map<Value *, std::unordered_set<Value *>> & graph, std::stack<Value *> & stack,
    std::vector<Value *> & spilled)
{
    std::unordered_map<Value *, std::unordered_set<Value *>> tempGraph = graph;
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
                stack.push(node);
                removed.insert(node);
                found = true;
                progress = true;
            }
        }

        if (!found)
            break;
    }

    // 溢出记录
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
void GraphColoringRegisterAllocator::assignColors(
    const std::unordered_map<Value *, std::unordered_set<Value *>> & graph, std::stack<Value *> & stack)
{
    while (!stack.empty()) {
        Value * node = stack.top();
        stack.pop();

        std::unordered_set<int> usedColors;
        for (Value * neighbor: graph.at(node)) { // <<< 改为当前图graph
            if (colorMap.count(neighbor)) {
                usedColors.insert(colorMap[neighbor]);
            }
        }

        int color = -1;
        for (int i = 0; i < regCount; ++i) {
            if (!usedColors.count(i)) {
                color = i;
                break;
            }
        }

        if (color == -1) {
            // spill应分类:
            if (node->getType()->isFloatType()) {
                spilledFloat.push_back(node);
            } else {
                spilledInt.push_back(node);
            }
        } else {
            colorMap[node] = color;
        }
    }
}

// /// @brief 获取指定变量分配到的寄存器编号
// /// @param val 变量指针
// /// @return 寄存器编号，若为 -1 则表示未分配（可能已溢出）
// int GraphColoringRegisterAllocator::getRegister(Value * val) const
// {
//     auto it = colorMap.find(val);
//     if (it != colorMap.end()) {
//         int regIndex = it->second;
//         if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableIntRegNum) {
//             return PlatformRiscV64::RISCV64_INT_REGS[regIndex];
//         }
//     }
//     return -1;
// }

/// @brief 获取Int型溢出（未分配成功）变量
/// @return 包含溢出变量指针的向量引用
const std::vector<Value *> & GraphColoringRegisterAllocator::getIntSpilled() const
{
    return spilledInt;
}

/// @brief 获取Float型溢出（未分配成功）变量
/// @return 包含溢出变量指针的向量引用
const std::vector<Value *> & GraphColoringRegisterAllocator::getFloatSpilled() const
{
    return spilledFloat;
}

/// @brief 根据活跃变量分析结果构建干涉图
/// @param lva 活跃变量分析的结果对象
void GraphColoringRegisterAllocator::buildGraph(const LiveVariableAnalysis & lva)
{
    const auto & stackVars = lva.getStackVars(); // <<< 获取栈变量集合
    for (auto * block: lva.getBasicBlocks()) {
        std::unordered_set<Value *> live;
        for (auto * v: lva.getLiveOut(block->label)) {
            if (!stackVars.count(v)) // <<< 过滤掉栈变量
                live.insert(v);
        }
        // 初始化 live 集

        // std::cout << ">>> BasicBlock: " << block->label << "\n";
        // std::cout << "Initial LiveOut: ";
        // for (auto * v: live)
        //     std::cout << v->getIRName() << " ";
        // std::cout << "\n";

        for (auto it = block->instructions.rbegin(); it != block->instructions.rend(); ++it) {
            Instruction * inst = *it;

            // 添加使用的变量到 live
            for (Use * use: inst->getOperands()) {
                Value * operand = use->getUsee();
                if (operand->getValueCategory() == ValueCategory::IMMEDIATE)
                    continue;
                if (operand->getType()->isVoidType())
                    continue;
                if (stackVars.count(operand)) // <<< 过滤掉栈变量
                    continue;
                live.insert(operand);
                // std::cout << "  USE: " << operand->getIRName() << "\n";
            }
            if (inst->hasResultValue()) {
                Value * def = inst;
                if (!stackVars.count(def)) { // 过滤栈变量
                    for (Value * val: live) {

                        if (val != def &&
                            val->getType()->isFloatType() == def->getType()->isFloatType()) { // 仅相同类型变量干涉
                            addInterference(def, val);
                        }
                    }
                }
                live.erase(def); // 依然要 erase（不管是不是栈变量）
            }

            // std::cout << "  Updated live set: ";
            // for (auto * v: live)
            //     std::cout << v->getIRName() << " ";
            // std::cout << "\n";
        }

        // std::cout << "Final Live for block " << block->label << ": ";
        // for (auto * v: live)
        //     std::cout << v->getIRName() << " ";
        // std::cout << "\n\n";
    }

    // //输出最终干涉图
    // std::cout << "=== intInterference Graph ===\n";
    // for (auto & [v, neighbors]: intInterferenceGraph) {
    //     std::cout << v->getIRName() << " : ";
    //     for (auto * n: neighbors)
    //         std::cout << n->getIRName() << " ";
    //     std::cout << "\n";
    // }
    // std::cout << "=== floatInterference Graph ===\n";

    // for (auto & [v, neighbors]: floatInterferenceGraph) {
    //     std::cout << v->getIRName() << " : ";
    //     for (auto * n: neighbors)
    //         std::cout << n->getIRName() << " ";
    //     std::cout << "\n";
    // }
}

/// @brief 获取所有变量的寄存器分配映射
/// @return 变量到寄存器编号的映射
std::unordered_map<Value *, int> GraphColoringRegisterAllocator::getColorMap() const
{
    return colorMap;
}

///
/// @brief 分配一个寄存器。如果没有，则选取寄存器中最晚使用的寄存器，同时溢出寄存器到变量中
/// @return int 寄存器编号
// 新增以适配SimpleRegisterAllocatorRiscV64接口
int GraphColoringRegisterAllocator::Allocate(Value * var, int32_t no)
{
    if (var && (var->getRegId() != -1)) {
        return var->getRegId();
    }

    auto iter = colorMap.find(var);
    if (iter != colorMap.end()) {
        int regIndex = iter->second;
        if (var->getType()->isFloatType()) { // 浮点变量
            if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableFloatRegNum) {
                int regno = PlatformRiscV64::RISCV64_FLOAT_REGS[regIndex];
                var->setRegId(regno);
                return regno;
            }
        } else { // 整数变量
            if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableIntRegNum) {
                int regno = PlatformRiscV64::RISCV64_INT_REGS[regIndex];
                var->setRegId(regno);
                return regno;
            }
        }
    }

    if (std::find(spilledInt.begin(), spilledInt.end(), var) != spilledInt.end() ||
        std::find(spilledFloat.begin(), spilledFloat.end(), var) != spilledFloat.end()) {
        return -1;
    }

    return -1;
}

/// @brief 由寄存器号查找编号（支持整数和浮点寄存器）
/// @param regno
int GraphColoringRegisterAllocator::regNoToIndex(int regNo)
{
    if (regNo >= 32 && regNo <= 63) {
        for (int i = 0; i < PlatformRiscV64::maxUsableFloatRegNum; ++i) {
            if (PlatformRiscV64::RISCV64_FLOAT_REGS[i] == regNo) {
                return i;
            }
        }
    } else {
        for (int i = 0; i < PlatformRiscV64::maxUsableIntRegNum; ++i) {
            if (PlatformRiscV64::RISCV64_INT_REGS[i] == regNo) {
                return i;
            }
        }
    }
    return -1; // 未找到
}

///
/// @brief 强制占用一个指定的寄存器。如果寄存器被占用，则强制寄存器关联的变量溢出
/// @param no 要分配的寄存器编号
///
void GraphColoringRegisterAllocator::Allocate(int32_t no)
{
    int regIndex = regNoToIndex(no); // 全局index: 0~63

    if (regIndex == -1)
        return; // 非法寄存器

    if (no < 32) { // int寄存器
        if (intRegBitmap.test(regIndex)) {
            free(no);
        }
        intBitmapSet(regIndex);
    } else { // float寄存器
        if (floatRegBitmap.test(regIndex)) {
            free(no);
        }
        floatBitmapSet(regIndex);
    }
}

///
/// @brief 将变量对应的load寄存器标记为空闲状态
/// @param var 变量
///
void GraphColoringRegisterAllocator::free(Value * var)
{
    if (var && var->getRegId() != -1) {
        int regIndex = regNoToIndex(var->getRegId());
        if (regIndex != -1) {
            if (var->getType()->isIntegerType()) {
                // 整数寄存器，直接操作 intRegBitmap
                intRegBitmap.reset(regIndex);
            } else {
                // 浮点寄存器，计算浮点寄存器对应的bitmap索引
                floatRegBitmap.reset(regIndex);
            }
        }
        auto it = std::find(regValues.begin(), regValues.end(), var);
        if (it != regValues.end()) {
            regValues.erase(it);
        }
        var->setRegId(-1);
    }
}

///
/// @brief 将寄存器no标记为空闲状态
/// @param no 寄存器编号
///
void GraphColoringRegisterAllocator::free(int32_t no)
{
    if (no == -1) {
        return;
    }

    int regIndex = regNoToIndex(no);
    if (regIndex == -1) {
        return; // 非法寄存器号
    }

    if (no >= 32) {
        floatRegBitmap.reset(regIndex);

    } else {
        intRegBitmap.reset(regIndex);
    }

    auto pIter = std::find_if(regValues.begin(), regValues.end(), [=](auto val) { return val->getRegId() == no; });

    if (pIter != regValues.end()) {
        (*pIter)->setRegId(-1);
        regValues.erase(pIter);
    }
}

void GraphColoringRegisterAllocator::intBitmapSet(int32_t no)
{
    intRegBitmap.set(no);
    usedIntBitmap.set(no);
}

void GraphColoringRegisterAllocator::floatBitmapSet(int32_t no)
{
    floatRegBitmap.set(no);
    usedFloatBitmap.set(no);
}
