///
/// @file UnaryInstruction.h
/// @brief 一元操作指令（如 +x, -x, !x）
///
/// @author ...
/// @version 1.0
/// @date 2024-09-30
///
#pragma once

#include <string>

#include "Value.h"
#include "Instruction.h"

class Function;

///
/// @brief 一元操作指令
///
class UnaryInstruction : public Instruction {
public:
    ///
    /// @brief 构造函数
    /// @param _op 一元操作类型（POS, NEG, NOT）
    /// @param _srcVal 源操作数
    /// @param _func 所属函数
    ///
    UnaryInstruction(Function * _func, IRInstOperator _op,Value * srcVal,Type * _type);

    static  UnaryInstruction* createAutoTyped(Function * func,
        Value * hs,
        IRInstOperator intOp,
        IRInstOperator floatOp);
    /// @brief 转换成字符串
    void toString(std::string & str) override;

};
