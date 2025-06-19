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
#include "PlatformRiscV64.h"
#include "LiveVariableAnalysis.h"

class GraphColoringRegisterAllocator {
public:
    GraphColoringRegisterAllocator(int regCount = PlatformRiscV64::maxUsableRegNum);

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
    void                         buildGraph(const LiveVariableAnalysis & lva);

    /// @brief 获取所有变量的寄存器分配映射
    std::unordered_map<Value *, int> getColorMap() const;
    ///
    /// @brief 尝试按指定的寄存器编号进行分配，若能分配，则直接分配，否则按图着色分配
    /// @param var 分配寄存器的变量
    /// @param no 指定的寄存器编号
    /// @return int 寄存器编号
    ///
    int Allocate(Value * var = nullptr, int32_t no = -1);

    ///
    /// @brief 强制占用一个指定的寄存器。如果寄存器被占用，则强制寄存器关联的变量溢出
    /// @param no 要分配的寄存器编号
    ///
    void Allocate(int32_t no);

    ///
    /// @brief 将变量对应的load寄存器标记为空闲状态
    /// @param var 变量
    ///
    void free(Value * var);

    ///
    /// @brief 将寄存器no标记为空闲状态
    /// @param no 寄存器编号
    ///
    void free(int32_t no);

private:
    bool simplify();
    void select();
    void assignColors();

private:
    int regCount;

    std::unordered_map<Value *, std::unordered_set<Value *>> interferenceGraph; //干涉图
    std::unordered_map<Value *, int>                         colorMap;          //寄存器分配表
    std::vector<Value *>                                     spilled;           //溢出处理

    std::stack<Value *> simplifyStack;

protected:
    ///
    /// @brief 寄存器位图：1已被占用，0未被使用
    ///
    BitMap<PlatformRiscV64::maxUsableRegNum> regBitmap;

    ///
    /// @brief 寄存器被那个Value占用。按照时间次序加入
    ///
    std::vector<Value *> regValues;

    ///
    /// @brief 使用过的所有寄存器编号
    ///
    BitMap<PlatformRiscV64::maxUsableRegNum> usedBitmap;

protected:
    ///
    /// @brief 寄存器被置位，使用过的寄存器被置位
    /// @param no
    ///
    void bitmapSet(int32_t no);

    ///
    /// @brief 由寄存器号查找编号
    /// @param regno
    ///
    int regNoToIndex(int regNo);
};
