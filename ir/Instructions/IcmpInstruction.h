///
/// @file IcmpInstruction.h
/// @brief 整数比较指令
///
/// @author cx (cxlyq.g@gmail.com)
/// @version 1.0
/// @date 2025-05-27
///
/// @copyright Copyright (c) 2025
///
#pragma once

#include <string>

#include "Instruction.h" // 包含基类 Instruction 的定义，以及 IRInstOperator 枚举
#include "Value.h"       // 包含 Value 类的定义，比较的操作数是 Value*

class Function; // Forward declare Function

///
/// @brief 整数比较指令 (icmp)
///
/// 这是一个二元指令，比较两个整数值，结果是一个 i1 (布尔) 值。
///
class IcmpInstruction : public Instruction {
public:
    ///
    /// @brief 构造函数
    /// @param _func 所属函数
    /// @param _predicate 比较谓词 (例如 IRINST_OP_EQ_I, IRINST_OP_NEQ_I 等)
    /// @param _lhs 左操作数
    /// @param _rhs 右操作数
    ///
    explicit IcmpInstruction(
        Function *     _func,
        IRInstOperator _predicate, // 这里的 op 就是比较谓词
        Value * _lhs, Value * _rhs);

    ///
    /// @brief 析构函数
    ///
    ~IcmpInstruction() override = default; // Virtual destructor, use default

    ///
    /// @brief 获取左操作数
    /// @return Value* 左操作数
    ///
    [[nodiscard]] Value * getLHS() const
    {
        return lhs;
    }

    ///
    /// @brief 获取右操作数
    /// @return Value* 右操作数
    ///
    [[nodiscard]] Value * getRHS() const
    {
        return rhs;
    }

    ///
    /// @brief 转换成IR指令文本形式
    /// @param str IR指令文本
    ///
    void toString(std::string & str) override;

private:
    ///
    /// @brief 左操作数
    ///
    Value * lhs;

    ///
    /// @brief 右操作数
    ///
    Value * rhs;

    // The comparison predicate (eq, ne, slt, sgt, sle, sge) is stored in the base class 'op'.
};