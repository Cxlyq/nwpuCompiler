#include "LiveVariableAnalysis.h"
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <algorithm>

void LiveVariableAnalysis::run(Function * func)
{
    buildBasicBlocks(func);
    buildCFG();
    computeUseDef();
    computeLiveInOut();
}

void LiveVariableAnalysis::buildBasicBlocks(Function * func)
{
    const auto & instructions = func->getInterCode().getInsts();
    BasicBlock * currentBlock = nullptr;

    for (Instruction * inst: instructions) {
        if (inst->getOp() == IRInstOperator::IRINST_OP_LABEL) {
            auto * newBlock = new BasicBlock();
            newBlock->label = inst->getName();
            labelToBlock[newBlock->label] = newBlock;
            basicBlocks.push_back(newBlock);
            currentBlock = newBlock;
        }

        if (currentBlock == nullptr) {
            currentBlock = new BasicBlock();
            currentBlock->label = "entry";
            basicBlocks.push_back(currentBlock);
        }

        currentBlock->instructions.push_back(inst);
    }
}

void LiveVariableAnalysis::buildCFG()
{
    for (size_t i = 0; i < basicBlocks.size(); ++i) {
        auto *        block = basicBlocks[i];
        Instruction * last = block->instructions.back();

        if (last->getOp() == IRInstOperator::IRINST_OP_GOTO) {
            std::string  target = last->getOperand(0)->getName();
            BasicBlock * succ = labelToBlock[target];
            block->successors.push_back(succ);
            succ->predecessors.push_back(block);
        } else if (last->getOp() != IRInstOperator::IRINST_OP_EXIT && i + 1 < basicBlocks.size()) {
            block->successors.push_back(basicBlocks[i + 1]);
            basicBlocks[i + 1]->predecessors.push_back(block);
        }
    }
}

void LiveVariableAnalysis::computeUseDef()
{
    for (auto * block: basicBlocks) {
        std::unordered_set<Value *> & useSet = use[block];
        std::unordered_set<Value *> & defSet = def[block];

        for (Instruction * inst: block->instructions) {
            // 遍历指令的所有操作数
            for (Use * u: inst->getOperands()) {
                Value * operand = u->getUsee();

                // 如果该操作数还没在 def 集中出现，则放入 use 集
                if (defSet.find(operand) == defSet.end()) {
                    useSet.insert(operand);
                }
            }

            // 如果该指令定义了一个值（结果变量），加入 def 集合
            if (inst->hasResultValue()) {
                defSet.insert(inst);
            }
        }
    }
}

void LiveVariableAnalysis::computeLiveInOut()
{
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto * block: basicBlocks) {
            std::unordered_set<Value *> newOut;
            for (auto * succ: block->successors) {
                const auto & succIn = liveIn[succ];
                newOut.insert(succIn.begin(), succIn.end());
            }

            std::unordered_set<Value *> newIn = use[block];
            std::unordered_set<Value *> temp;
            std::set_difference(
                newOut.begin(),
                newOut.end(),
                def[block].begin(),
                def[block].end(),
                std::inserter(temp, temp.begin()));
            newIn.insert(temp.begin(), temp.end());

            if (newIn != liveIn[block] || newOut != liveOut[block]) {
                liveIn[block] = std::move(newIn);
                liveOut[block] = std::move(newOut);
                changed = true;
            }
        }
    }
}

const std::unordered_set<Value *> & LiveVariableAnalysis::getLiveIn(const std::string & label) const
{
    return liveIn.at(labelToBlock.at(label));
}

const std::unordered_set<Value *> & LiveVariableAnalysis::getLiveOut(const std::string & label) const
{
    return liveOut.at(labelToBlock.at(label));
}
std::vector<LiveVariableAnalysis::BasicBlock *> LiveVariableAnalysis::getBasicBlocks() const
{
    return basicBlocks;
}
