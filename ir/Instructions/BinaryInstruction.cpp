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
BinaryInstruction * BinaryInstruction::createAutoTyped(Function * func,
    Value * lhs,
    Value * rhs,
    IRInstOperator intOp,
    IRInstOperator floatOp) {
bool isFloat = lhs->getType()->isFloatType() || rhs->getType()->isFloatType();
IRInstOperator op = isFloat ? floatOp : intOp;
Type * type = isFloat ? FloatType::getInstance() : IntegerType::getInstance();
return new BinaryInstruction(func, op, lhs, rhs, type);
}




/// @brief 构造函数
/// @param _op 操作符
/// @param _result 结果操作数
/// @param _srcVal1 源操作数1
/// @param _srcVal2 源操作数2
BinaryInstruction::BinaryInstruction(Function * _func,
                                     IRInstOperator _op,
                                     Value * _srcVal1,
                                     Value * _srcVal2,
                                     Type * _type)
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

            // 加法指令，二元运算
            str = getIRName() + " = add " + src1->getIRName() + "," + src2->getIRName();
            break;
        case IRInstOperator::IRINST_OP_SUB_I:

            // 减法指令，二元运算
            str = getIRName() + " = sub " + src1->getIRName() + "," + src2->getIRName();
            break;

        default:
            // 未知指令
            Instruction::toString(str);
            break;
    }
}
