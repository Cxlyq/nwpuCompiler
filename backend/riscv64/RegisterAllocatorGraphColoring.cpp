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
    // std::cout << "=== Register Allocation Result ===\n";
    // for (const auto & entry: colorMap) {
    //     Value * val = entry.first;
    //     int     regIndex = entry.second;
    //     std::cout << val->getIRName() << " -> r" << regIndex << "\n";
    // }

    // if (!spilledInt.empty()) {
    //     std::cout << "=== Spilled Int Variables ===\n";
    //     for (auto * val: spilledInt) {
    //         std::cout << val->getIRName() << "\n";
    //     }
    // }

    // if (!spilledFloat.empty()) {
    //     std::cout << "=== Spilled Float Variables ===\n";
    //     for (auto * val: spilledFloat) {
    //         std::cout << val->getIRName() << "\n";
    //     }
    // }
    return spilledInt.empty() && spilledFloat.empty();
}

/// @brief 简化干涉图，构建简化栈
/// @return 是否成功简化（若为 false，说明需溢出）
bool GraphColoringRegisterAllocator::simplify(
    const std::unordered_map<Value *, std::unordered_set<Value *>> & graph, std::stack<Value *> & stack,
    std::vector<Value *> & spilled)
{
    // 从变量名提取数字的lambda
    auto extractNumber = [](const std::string & name) -> int {
        size_t i = 0;
        while (i < name.size() && !isdigit(name[i]))
            i++;
        if (i == name.size())
            return -1;
        return std::stoi(name.substr(i));
    };

    std::unordered_map<Value *, std::unordered_set<Value *>> tempGraph = graph;
    std::unordered_set<Value *>                              removed;

    bool progress = false;

    while (true) {
        bool                 found = false;
        std::vector<Value *> candidates;

        // 找所有度数小于寄存器数量的节点
        for (auto & entry: tempGraph) {
            Value * node = entry.first;
            if (removed.count(node))
                continue;

            int degree = 0;
            for (Value * n: entry.second) {
                if (!removed.count(n))
                    degree++;
            }
            if (degree < regCount) {
                candidates.push_back(node);
            }
        }

        if (candidates.empty())
            break;

        // 按数字降序排序，数字大的先压栈，数字小的后压栈
        std::sort(candidates.begin(), candidates.end(), [&](Value * a, Value * b) {
            return extractNumber(a->getIRName()) > extractNumber(b->getIRName());
        });

        for (Value * node: candidates) {
            if (!removed.count(node)) {
                stack.push(node);
                removed.insert(node);
                found = true;
                progress = true;
            }
        }

        if (!found)
            break;
    }

    // 剩下没删的节点就是溢出节点
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
        // std::cout << "\n>>> BasicBlock: " << block->label << "\n";

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

            if (inst->hasResultValue()) {
                Value * def = inst;
                if (!stackVars.count(def)) { // 过滤栈变量
                    bool inserted = false;
                    for (Value * val: live) {
                        if (val != def &&
                            val->getType()->isFloatType() == def->getType()->isFloatType()) { // 仅相同类型变量干涉
                            addInterference(def, val);
                            inserted = true;
                        }
                    }
                    // 如果没有和任何变量干涉，也要把 def 加进干涉图（空邻居集）
                    if (!inserted) {
                        if (def->getType()->isFloatType()) {
                            if (!floatInterferenceGraph.count(def)) {
                                floatInterferenceGraph[def] = {};
                            }
                        } else {
                            if (!intInterferenceGraph.count(def)) {
                                intInterferenceGraph[def] = {};
                            }
                        }
                    }
                }
                live.erase(def); // 依然要 erase（不管是不是栈变量）
            }

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
int GraphColoringRegisterAllocator::Allocate(Value * var)
{

    if (var && (var->getRegId() != -1)) {
        return var->getRegId();
    }
    auto iter = colorMap.find(var);
    if (iter != colorMap.end()) {
        int regIndex = iter->second;

        if (var->getType()->isFloatType()) { // 浮点变量
            if (floatRegBitmap.test(regIndex)) {
                int32_t regno = -1;

                // 查询空闲的浮点寄存器
                for (int k = 0; k < PlatformRiscV64::maxUsableFloatRegNum; ++k) {
                    if (!floatRegBitmap.test(k)) { // 如果该浮点寄存器未被占用
                        regIndex = k;
                        break;
                    }
                }

                if (regIndex != -1) {
                    // 占用该寄存器
                    floatBitmapSet(regIndex);
                } else {
                    // 没有空闲寄存器，选择溢出最旧的变量
                    Value * oldestVar = floatRegValues.front();
                    regno = oldestVar->getRegId();
                    oldestVar->setRegId(-1);
                    floatRegValues.erase(floatRegValues.begin());
                }
                if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableFloatRegNum) {
                    regno = PlatformRiscV64::RISCV64_FLOAT_REGS[regIndex];
                }
                // 加入新的变量
                var->setRegId(regno);
                floatRegValues.push_back(var);
                return regno; // 返回物理浮点寄存器编号
            } else if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableFloatRegNum) {
                int regno = PlatformRiscV64::RISCV64_FLOAT_REGS[regIndex];
                var->setRegId(regno);
                floatRegValues.push_back(var);
                floatBitmapSet(regIndex);
                return regno;
            }
        } else { // 整数变量
            if (intRegBitmap.test(regIndex)) {
                int32_t regno = -1;
                // 查询空闲的整数寄存器
                for (int k = 0; k < PlatformRiscV64::maxUsableIntRegNum; ++k) {
                    if (!intRegBitmap.test(k)) { // 如果该寄存器未被占用
                        regIndex = k;
                        break;
                    }
                }

                if (regIndex != -1) {
                    // 占用该寄存器
                    intBitmapSet(regIndex);
                } else {
                    // 没有空闲寄存器，选择溢出最旧的变量
                    Value * oldestVar = intRegValues.front();
                    regno = oldestVar->getRegId();
                    oldestVar->setRegId(-1);
                    intRegValues.erase(intRegValues.begin());
                }
                if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableIntRegNum) {
                    regno = PlatformRiscV64::RISCV64_INT_REGS[regIndex];
                }
                // 加入新的变量
                var->setRegId(regno);
                intRegValues.push_back(var);
                return regno; // 返回物理寄存器编号
            } else if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableIntRegNum) {
                int regno = PlatformRiscV64::RISCV64_INT_REGS[regIndex];
                var->setRegId(regno);
                intRegValues.push_back(var);
                intBitmapSet(regIndex);
                return regno;
            }
        }
    } else if (var->getType()->isIntegerType()) {
        int32_t regno = -1;
        int     regIndex = -1;
        // 查询空闲的整数寄存器
        for (int k = 0; k < PlatformRiscV64::maxUsableIntRegNum; ++k) {
            if (!intRegBitmap.test(k)) { // 如果该寄存器未被占用
                regIndex = k;
                break;
            }
        }

        if (regIndex != -1) {
            // 占用该寄存器
            intBitmapSet(regIndex);
        } else {
            // 没有空闲寄存器，选择溢出最旧的变量
            Value * oldestVar = intRegValues.front();
            regno = oldestVar->getRegId();
            oldestVar->setRegId(-1);
            intRegValues.erase(intRegValues.begin());
        }
        if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableIntRegNum) {
            regno = PlatformRiscV64::RISCV64_INT_REGS[regIndex];
        }
        // 加入新的变量
        var->setRegId(regno);
        intRegValues.push_back(var);
        return regno; // 返回物理寄存器编号
    } else if (var->getType()->isFloatType()) {
        int32_t regno = -1;
        int     regIndex = -1;

        // 查询空闲的浮点寄存器
        for (int k = 0; k < PlatformRiscV64::maxUsableFloatRegNum; ++k) {
            if (!floatRegBitmap.test(k)) { // 如果该浮点寄存器未被占用
                regIndex = k;
                break;
            }
        }

        if (regIndex != -1) {
            // 占用该寄存器
            floatBitmapSet(regIndex);
        } else {
            // 没有空闲寄存器，选择溢出最旧的变量
            Value * oldestVar = floatRegValues.front();
            regno = oldestVar->getRegId();
            oldestVar->setRegId(-1);
            floatRegValues.erase(floatRegValues.begin());
        }
        if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableFloatRegNum) {
            regno = PlatformRiscV64::RISCV64_FLOAT_REGS[regIndex];
        }
        // 加入新的变量
        var->setRegId(regno);
        floatRegValues.push_back(var);
        return regno; // 返回物理浮点寄存器编号
    }

    return -1;
}

/// @brief 临时分配一个int型寄存器
/// @return int 寄存器编号
///
int GraphColoringRegisterAllocator::AllocateTempInt()
{
    int32_t regno = -1;
    int     regIndex = -1;
    // 查询空闲的整数寄存器
    for (int k = 0; k < PlatformRiscV64::maxUsableIntRegNum; ++k) {
        if (!intRegBitmap.test(k)) { // 如果该寄存器未被占用
            regIndex = k;
            break;
        }
    }

    if (regIndex != -1) {
        // 占用该寄存器
        intBitmapSet(regIndex);
    } else {
        // 没有空闲寄存器，选择溢出最旧的变量
        Value * oldestVar = intRegValues.front();
        regno = oldestVar->getRegId();
        oldestVar->setRegId(-1);
        intRegValues.erase(intRegValues.begin());
    }
    if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableIntRegNum) {
        regno = PlatformRiscV64::RISCV64_INT_REGS[regIndex];
    }
    return regno; // 返回物理寄存器编号
}

/// @brief 临时分配一个float型寄存器
/// @return int 寄存器编号
///
int GraphColoringRegisterAllocator::AllocateTempFloat()
{
    int32_t regno = -1;
    int     regIndex = -1;

    // 查询空闲的浮点寄存器
    for (int k = 0; k < PlatformRiscV64::maxUsableFloatRegNum; ++k) {
        if (!floatRegBitmap.test(k)) { // 如果该浮点寄存器未被占用
            regIndex = k;
            break;
        }
    }

    if (regIndex != -1) {
        // 占用该寄存器
        floatBitmapSet(regIndex);
    } else {
        // 没有空闲寄存器，选择溢出最旧的变量
        Value * oldestVar = floatRegValues.front();
        regno = oldestVar->getRegId();
        oldestVar->setRegId(-1);
        floatRegValues.erase(floatRegValues.begin());
    }
    if (regIndex >= 0 && regIndex < PlatformRiscV64::maxUsableFloatRegNum) {
        regno = PlatformRiscV64::RISCV64_FLOAT_REGS[regIndex];
    }
    return regno; // 返回物理浮点寄存器编号
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
            if (var->getType()->isFloatType()) {

                auto it = std::find(floatRegValues.begin(), floatRegValues.end(), var);
                if (it != floatRegValues.end()) {
                    floatRegValues.erase(it);
                }
                // 浮点寄存器，计算浮点寄存器对应的bitmap索引
                floatRegBitmap.reset(regIndex);
                // 整数寄存器，直接操作 intRegBitmap

            } else {
                intRegBitmap.reset(regIndex);
                auto it = std::find(intRegValues.begin(), intRegValues.end(), var);
                if (it != intRegValues.end()) {
                    intRegValues.erase(it);
                }
            }
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
        // std::cout << "[free] 忽略无效寄存器号 -1\n";
        return;
    }

    int regIndex = regNoToIndex(no);
    if (regIndex == -1) {
        std::cout << "[free] 非法寄存器号: " << no << "，无法找到对应索引\n";
        return;
    }

    if (no >= 32) {
        // 浮点寄存器
        floatRegBitmap.reset(regIndex);
        // std::cout << "[free] 释放浮点寄存器 f" << no << " (index " << regIndex << ")\n";

        auto pIter = std::find_if(floatRegValues.begin(), floatRegValues.end(), [=](Value * val) {
            return val->getRegId() == no;
        });

        if (pIter != floatRegValues.end()) {
            // std::cout << "[free] -> 移除绑定变量: " << (*pIter)->getName() << "\n";
            (*pIter)->setRegId(-1);
            floatRegValues.erase(pIter);
        } else {
            // std::cout << "[free] -> 未找到绑定变量，可能是立即数或未追踪变量\n";
        }
    } else {
        // 整数寄存器
        intRegBitmap.reset(regIndex);
        // std::cout << "[free] 释放整数寄存器 r" << no << " (index " << regIndex << ")\n";

        auto pIter =
            std::find_if(intRegValues.begin(), intRegValues.end(), [=](Value * val) { return val->getRegId() == no; });

        if (pIter != intRegValues.end()) {
            // std::cout << "[free] -> 移除绑定变量: " << (*pIter)->getName() << "\n";
            (*pIter)->setRegId(-1);
            intRegValues.erase(pIter);
        } else {
            // std::cout << "[free] -> 未找到绑定变量，可能是立即数或未追踪变量\n";
        }
    }
    // ✅ 输出完整 intRegBitmap 状态
    // std::cout << "[free] 当前整数寄存器使用状态：\n";
    // for (int i = 0; i < PlatformRiscV64::maxUsableIntRegNum; ++i) {
    //     int  regNo = PlatformRiscV64::RISCV64_INT_REGS[i];
    //     bool used = intRegBitmap.test(i);
    //     std::cout << "  r" << regNo << ": " << (used ? "占用" : "空闲") << "\n";
    // }
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
// 检查整型寄存器是否被占用
bool GraphColoringRegisterAllocator::isRegOccupied(int regNum)
{
    int regIndex = regNoToIndex(regNum);
    if (regIndex == -1) {
        std::cout << "[free] 非法寄存器号: " << regNum << "，无法找到对应索引\n";
        return false;
    }
    if (regNum >= 32) {
        return regIndex < PlatformRiscV64::maxUsableFloatRegNum && floatRegBitmap.test(regIndex);
    } else {
        return regIndex < PlatformRiscV64::maxUsableIntRegNum && intRegBitmap.test(regIndex);
    }
}