///
/// @file UnaryInstruction.cpp
/// @brief 一元操作指令
///
/// @author   Kevin-wjq13777(w137776w@gmail.com)
/// @version 1.0
/// @date 2025/5/1
///
/// @copyright Copyright (c) 2025
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2025/5/1 <td>1.0     <td>Kevin-wjq13777  <td>新建
/// </table>
///
#include "UnaryInstruction.h"

#include "Function.h"
#include "IntegerType.h"
#include "FloatType.h"
#include "VoidType.h"

///@brief 选择一个一元操作指令
/// @param func 函数对象
/// @param hs 操作数
/// @param intOp 整数操作符
/// @param floatOp 浮点操作符
/// @return UnaryInstruction* 一元操作指令对象
/// @note 该函数会自动判断操作数的类型，选择合适的操作符
UnaryInstruction *
UnaryInstruction::createAutoTyped(Function * func, Value * hs, IRInstOperator intOp, IRInstOperator floatOp)
{
    // bool isFloat = hs->getType()->isFloatType();
    bool isFloat =
        hs->getType()->isArrayType() ? hs->getType()->getElementType()->isFloatType() : hs->getType()->isFloatType();
    IRInstOperator op = isFloat ? floatOp : intOp;
    Type *         floatTy = FloatType::getType();
    Type *         intTy = IntegerType::getTypeInt();
    Type *         type = isFloat ? floatTy : intTy;
    return new UnaryInstruction(func, op, hs, type);
}

/// @brief 构造函数
/// @param _op 操作符
/// @param _result 结果操作数
/// @param _srcVal 源操作数
UnaryInstruction::UnaryInstruction(Function * _func, IRInstOperator _op, Value * _srcVal, Type * _type)
    : Instruction(_func, _op, _type)
{
    addOperand(_srcVal);
}

/// @brief 转换成字符串
/// @param str 转换后的字符串
void UnaryInstruction::toString(std::string & str)
{

    Value * src = getOperand(0);

    switch (op) {
        case IRInstOperator::IRINST_OP_POS_I:
            // 整数单目正指令，一元运算
            str = getIRName() + " = pos_i " + src->getIRName();
            break;

        case IRInstOperator::IRINST_OP_POS_F:
            // 浮点数数单目正指令，一元运算
            str = getIRName() + " = pos_f " + src->getIRName();
            break;

        case IRInstOperator::IRINST_OP_NEG_I:
            // 整数单目负指令，一元运算
            str = getIRName() + " = neg_i " + src->getIRName();
            break;

        case IRInstOperator::IRINST_OP_NEG_F:
            // 浮点数单目负指令，一元运算
            str = getIRName() + " = neg_f " + src->getIRName();
            break;

        case IRInstOperator::IRINST_OP_NOT_I:
            // 整数逻辑非指令，一元运算
            str = getIRName() + " = not_i " + src->getIRName();
            break;

        case IRInstOperator::IRINST_OP_NOT_F:
            // 浮点逻辑非指令，一元运算
            str = getIRName() + " = not_f " + src->getIRName();
            break;

        default:
            // 未知指令
            Instruction::toString(str);
            break;
    }
}
