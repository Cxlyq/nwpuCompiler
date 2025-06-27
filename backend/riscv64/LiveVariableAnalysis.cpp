
///
/// @file LiveVariableAnalysis.cpp
/// @brief 活跃性分析
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

#include "LiveVariableAnalysis.h"
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "iostream"

/// @brief 运行活跃性分析入口，包括构建基本块、CFG图、use/def集合和liveIn/liveOut集合
/// @param func 待分析的函数指针
void LiveVariableAnalysis::run(Function * func)
{
    collectStackVars(func);
    buildBasicBlocks(func);
    buildCFG();
    computeUseDef();
    computeLiveInOut();
}

/// @brief 收集函数中所有栈变量（alloca产生的变量）
/// @param func 当前函数指针
void LiveVariableAnalysis::collectStackVars(Function * func)
{
    stackVars.clear();
    for (auto & var: func->getVarValues()) {
        stackVars.insert(var);
    }
    // std::cout << "Collected stack variables:\n";
    // for (auto * v: stackVars) {
    //     std::cout << "  " << v->getIRName() << "\n";
    // }
}

/// @brief 构建基本块（Basic Blocks），并将其加入 basicBlocks 列表
/// @param func 分析目标函数，包含中间代码指令
void LiveVariableAnalysis::buildBasicBlocks(Function * func)
{
    const auto & instructions = func->getInterCode().getInsts();
    BasicBlock * currentBlock = nullptr;

    for (auto inst: instructions) {
        if (inst->getOp() == IRInstOperator::IRINST_OP_LABEL) {
            // 新 label，开启新的 block
            currentBlock = new BasicBlock();
            currentBlock->label = inst->getIRName();
            labelToBlock[currentBlock->label] = currentBlock;
            basicBlocks.push_back(currentBlock);
        }

        // 如果还没有 basic block，创建初始 entry block
        if (currentBlock == nullptr) {
            currentBlock = new BasicBlock();
            currentBlock->label = "entry";
            labelToBlock[currentBlock->label] = currentBlock;
            basicBlocks.push_back(currentBlock);
        }

        // 添加指令到当前 block
        currentBlock->instructions.push_back(inst);
    }

    // // 安全性验证日志
    // std::cout << "basicBlocks size: " << basicBlocks.size() << std::endl;
    // for (size_t i = 0; i < basicBlocks.size(); ++i) {
    //     auto * blk = basicBlocks[i];
    //     if (!blk) {
    //         std::cerr << "basicBlocks[" << i << "] is nullptr!" << std::endl;
    //     } else {
    //         std::cout << "basicBlocks[" << i << "] addr: " << blk << ", label: " << blk->label
    //                   << ", instructions count: " << blk->instructions.size() << std::endl;
    //     }
    // }
}

/// @brief 构建控制流图（CFG），设置基本块的前驱和后继关系
void LiveVariableAnalysis::buildCFG()
{
    for (size_t i = 0; i < basicBlocks.size(); ++i) {
        auto *        block = basicBlocks[i];
        Instruction * last = block->instructions.back();

        if (last->getOp() == IRInstOperator::IRINST_OP_GOTO) {
            std::string  target = last->getOperand(0)->getIRName();
            BasicBlock * succ = labelToBlock[target];
            block->successors.push_back(succ);
            succ->predecessors.push_back(block);
        } else if (last->getOp() == IRInstOperator::IRINST_OP_BR_COND) {
            std::string  trueLabel = last->getOperand(1)->getIRName();
            std::string  falseLabel = last->getOperand(2)->getIRName();
            BasicBlock * trueBlk = labelToBlock[trueLabel];
            BasicBlock * falseBlk = labelToBlock[falseLabel];

            block->successors.push_back(trueBlk);
            block->successors.push_back(falseBlk);
            trueBlk->predecessors.push_back(block);
            falseBlk->predecessors.push_back(block);
        } else if (last->getOp() != IRInstOperator::IRINST_OP_EXIT && i + 1 < basicBlocks.size()) {
            block->successors.push_back(basicBlocks[i + 1]);
            basicBlocks[i + 1]->predecessors.push_back(block);
        }
    }
}

/// @brief 计算每个基本块的 use 和 def 集合
/// use: 未定义就使用的变量集合；def: 本块中定义的变量集合
void LiveVariableAnalysis::computeUseDef()
{
    for (auto * block: basicBlocks) {
        std::unordered_set<Value *> & useSet = use[block];
        std::unordered_set<Value *> & defSet = def[block];

        for (Instruction * inst: block->instructions) {
            // 遍历指令的所有操作数
            for (size_t i = 0; i < inst->getOperands().size(); ++i) {
                Value * operand = inst->getOperands()[i]->getUsee();

                // 跳过立即数
                if (operand->getValueCategory() == ValueCategory::IMMEDIATE)
                    continue;

                // 跳过void类型
                if (operand->getType()->isVoidType())
                    continue;

                // 跳过栈变量
                if (stackVars.count(operand) > 0)
                    continue;

                // 非DEF中的才加入USE
                if (defSet.find(operand) == defSet.end()) {
                    useSet.insert(operand);
                }
            }

            // 如果该指令定义了一个值（结果变量），加入 def 集合
            if (inst->hasResultValue()) {
                defSet.insert(inst);
            }
        }
        //     // 🔍 输出调试信息
        //     std::cout << "[BasicBlock] " << block->label << "\n";

        //     std::cout << "  USE: { ";
        //     for (auto * v: useSet) {
        //         std::cout << v->getIRName() << " ";
        //     }
        //     std::cout << "}\n";

        //     std::cout << "  DEF: { ";
        //     for (auto * v: defSet) {
        //         std::cout << v->getIRName() << " ";
        //     }
        //     std::cout << "}\n";
    }
}

/// @brief 迭代计算每个基本块的 liveIn 和 liveOut 集合，直到不再变化
void LiveVariableAnalysis::computeLiveInOut()
{
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto * block: basicBlocks) {
            std::unordered_set<Value *> newOut;
            for (auto * succ: block->successors) {
                const auto & succIn = liveIn[succ];
                for (auto * v: succIn) {
                    if (stackVars.count(v))
                        continue; // 栈变量，跳过
                    newOut.insert(v);
                }
            }

            std::unordered_set<Value *> newIn = use[block];
            std::set<Value *>           sortedOut(newOut.begin(), newOut.end());
            std::set<Value *>           sortedDef(def[block].begin(), def[block].end());

            std::set<Value *> temp;
            std::set_difference(
                sortedOut.begin(),
                sortedOut.end(),
                sortedDef.begin(),
                sortedDef.end(),
                std::inserter(temp, temp.begin()));

            for (auto * v: temp) {
                if (stackVars.count(v))
                    continue; // 栈变量，跳过
                newIn.insert(v);
            }

            if (newIn != liveIn[block] || newOut != liveOut[block]) {
                liveIn[block] = std::move(newIn);
                liveOut[block] = std::move(newOut);
                changed = true;
            }
        }
    }
}

/// @brief 获取指定基本块标签对应的 liveIn 集合
/// @param label 基本块标签
/// @return 该块的 liveIn 集合
const std::unordered_set<Value *> & LiveVariableAnalysis::getLiveIn(const std::string & label) const
{
    return liveIn.at(labelToBlock.at(label));
}

/// @brief 获取指定基本块标签对应的 liveOut 集合
/// @param label 基本块标签
/// @return 该块的 liveOut 集合
const std::unordered_set<Value *> & LiveVariableAnalysis::getLiveOut(const std::string & label) const
{
    return liveOut.at(labelToBlock.at(label));
}

/// @brief 获取函数中所有构建好的基本块列表
/// @return 包含所有基本块的向量
const std::vector<LiveVariableAnalysis::BasicBlock *> & LiveVariableAnalysis::getBasicBlocks() const
{
    return basicBlocks;
}

/// @brief 获取函数中所有构建好的基本块列表
/// @return 包含所有基本块的向量
const std::unordered_set<Value *> & LiveVariableAnalysis::getStackVars() const
{
    return stackVars;
}