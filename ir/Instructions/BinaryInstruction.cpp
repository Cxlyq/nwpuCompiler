///
/// @file BinaryInstruction.cpp
/// @brief 二元操作指令
///
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-29
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// </table>
///
#include "BinaryInstruction.h"

#include "Function.h"
#include "IntegerType.h"
#include "FloatType.h"
#include "VoidType.h"

///@brief 选择一个二元操作指令
/// @param func 函数对象
/// @param lhs 左操作数
/// @param rhs 右操作数
/// @param intOp 整数操作符
/// @param floatOp 浮点操作符
/// @return BinaryInstruction* 二元操作指令对象
/// @note 该函数会自动判断操作数的类型，选择合适的操作符
BinaryInstruction * BinaryInstruction::createAutoTyped(
    Function * func, Value * lhs, Value * rhs, IRInstOperator intOp, IRInstOperator floatOp)
{
    bool isFloat = (lhs->getType()->isArrayType() ? lhs->getType()->getElementType()->isFloatType()
                                                  : lhs->getType()->isFloatType()) ||
                   (rhs->getType()->isArrayType() ? rhs->getType()->getElementType()->isFloatType()
                                                  : rhs->getType()->isFloatType());
    IRInstOperator op = isFloat ? floatOp : intOp;
    Type *         floatTy = FloatType::getType();
    Type *         intTy = IntegerType::getTypeInt();
    Type *         type = isFloat ? floatTy : intTy;
    return new BinaryInstruction(func, op, lhs, rhs, type);
}

/// @brief 构造函数
/// @param _op 操作符
/// @param _result 结果操作数
/// @param _srcVal1 源操作数1
/// @param _srcVal2 源操作数2
BinaryInstruction::BinaryInstruction(
    Function * _func, IRInstOperator _op, Value * _srcVal1, Value * _srcVal2, Type * _type)
    : Instruction(_func, _op, _type)
{
    addOperand(_srcVal1);
    addOperand(_srcVal2);
}

/// @brief 转换成字符串
/// @param str 转换后的字符串
void BinaryInstruction::toString(std::string & str)
{

    Value *src1 = getOperand(0), *src2 = getOperand(1);

    switch (op) {
        case IRInstOperator::IRINST_OP_ADD_I:
            //整数加法指令，二元运算
            str = getIRName() + " = add nsw i32 " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_ADD_F:
            //浮点数加法指令，二元运算
            str = getIRName() + " = fadd float " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_SUB_I:
            // 整数减法指令，二元运算
            str = getIRName() + " = sub nsw i32 " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_SUB_F:
            // 浮点数减法指令，二元运算
            str = getIRName() + " = fsub float " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_MUL_I:
            // 整数乘法指令，二元运算
            str = getIRName() + " = mul nsw i32 " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_MUL_F:
            // 浮点数乘法指令，二元运算
            str = getIRName() + " = fmul float " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_DIV_I:
            // 整数除法指令，二元运算
            str = getIRName() + " = sdiv i32 " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_DIV_F:
            // 浮点数除法指令，二元运算
            str = getIRName() + " = fdiv float " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_MOD_I:
            // 整数取余指令，二元运算
            str = getIRName() + " = srem i32 " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_AND:
            // 逻辑与指令，二元运算
            str = getIRName() + " = and " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_OR:
            // 逻辑或指令，二元运算
            str = getIRName() + " = or " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_EQ_I:
            // 整数相等指令，二元运算
            str = getIRName() + " = eq_i " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_EQ_F:
            // 浮点数相等指令，二元运算
            str = getIRName() + " = eq_f " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_NEQ_I:
            // 整数不等指令，二元运算
            str = getIRName() + " = neq_i " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_NEQ_F:
            // 浮点数不等指令，二元运算
            str = getIRName() + " = neg_f " + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_GE_I:
            // 整数大于等于指令，二元运算
            str = getIRName() + " =  ge_i" + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_GE_F:
            // 浮点数大于等于指令，二元运算
            str = getIRName() + " =  ge_f" + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_LE_I:
            // 整数小于等于指令，二元运算
            str = getIRName() + " =  le_i" + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_LE_F:
            // 浮点数小于等于指令，二元运算
            str = getIRName() + " =  le_f" + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_GNE_I:
            // 整数大于指令，二元运算
            str = getIRName() + " =  gne_i" + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_GNE_F:
            // 浮点数大于指令，二元运算
            str = getIRName() + " =  gne_f" + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_LNE_I:
            // 整数小于指令，二元运算
            str = getIRName() + " =  lne_i" + src1->getIRName() + ", " + src2->getIRName();
            break;

        case IRInstOperator::IRINST_OP_LNE_F:
            // 浮点数小于指令，二元运算
            str = getIRName() + " =  lne_f" + src1->getIRName() + ", " + src2->getIRName();
            break;

        default:
            // 未知指令
            Instruction::toString(str);
            break;
    }
}
