///
/// @file RegisterAllocatorGraphColoring.h
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
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>

#include "BitMap.h"
#include "Value.h"
#include "PlatformArm32.h"

class GraphColoringRegisterAllocator {
public:
    GraphColoringRegisterAllocator(int regCount = PlatformArm32::maxUsableRegNum);

    ///
    /// @brief 添加变量之间的冲突（干涉）关系
    /// @param a 第一个变量
    /// @param b 第二个变量
    ///
    void addInterference(Value * a, Value * b);

    ///
    /// @brief 执行寄存器分配（图着色）
    /// @return 分配成功返回true，否则部分变量溢出
    ///
    bool allocate();

    ///
    /// @brief 获取变量分配到的寄存器编号
    ///
    int getRegister(Value * val) const;

    ///
    /// @brief 被溢出的变量列表
    ///
    const std::vector<Value *> & getSpilled() const;

private:
    void buildGraph();
    bool simplify();
    void select();
    void assignColors();

private:
    int regCount;

    std::unordered_map<Value *, std::unordered_set<Value *>> interferenceGraph;
    std::unordered_map<Value *, int>                         colorMap;
    std::vector<Value *>                                     spilled;

    std::stack<Value *> simplifyStack;
};
