#pragma once

#include "Instruction.h"
#include "Function.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

/// @brief 活跃变量分析器
class LiveVariableAnalysis {
public:
    /// @brief 基本块结构
    struct BasicBlock {
        std::string                label;
        std::vector<Instruction *> instructions;
        std::vector<BasicBlock *>  successors;
        std::vector<BasicBlock *>  predecessors;
    };

    /// @brief 执行活跃变量分析
    void run(Function * func);

    /// @brief 获取某个基本块的活跃入口变量集合
    const std::unordered_set<Value *> & getLiveIn(const std::string & label) const;

    /// @brief 获取某个基本块的活跃出口变量集合
    const std::unordered_set<Value *> &             getLiveOut(const std::string & label) const;
    std::vector<LiveVariableAnalysis::BasicBlock *> getBasicBlocks() const;

private:
    /// 所有基本块
    std::vector<BasicBlock *> basicBlocks;

    /// Label 到基本块的映射
    std::unordered_map<std::string, BasicBlock *> labelToBlock;

    /// 每个基本块的 USE 集合（使用但未定义）
    std::unordered_map<BasicBlock *, std::unordered_set<Value *>> use;

    /// 每个基本块的 DEF 集合（本块内定义）
    std::unordered_map<BasicBlock *, std::unordered_set<Value *>> def;

    /// 每个基本块的 LIVE-IN 集合
    std::unordered_map<BasicBlock *, std::unordered_set<Value *>> liveIn;

    /// 每个基本块的 LIVE-OUT 集合
    std::unordered_map<BasicBlock *, std::unordered_set<Value *>> liveOut;

private:
    /// @brief 构建基本块
    void buildBasicBlocks(Function * func);

    /// @brief 构建控制流图（successor/predecessor）
    void buildCFG();

    /// @brief 计算每个块的 use / def 集合
    void computeUseDef();

    /// @brief 执行数据流迭代算法
    void computeLiveInOut();
};
