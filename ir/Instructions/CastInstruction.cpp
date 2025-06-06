///
/// @file CastInstruction.cpp
/// @brief 一元操作指令
///
/// @author   Kevin-wjq13777(w137776w@gmail.com)
/// @version 1.0
/// @date 2025/5/26
///
/// @copyright Copyright (c) 2025
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2025/5/26 <td>1.0     <td>Kevin-wjq13777  <td>新建
/// </table>
///
#include "CastInstruction.h"

#include "Function.h"
#include "IntegerType.h"
#include "FloatType.h"
#include "Type.h"
#include "VoidType.h"

/// @brief 构造函数
/// @param _op 操作符
/// @param _result 结果操作数
/// @param _srcVal 源操作数
CastInstruction::CastInstruction(Function * _func, Value * _srcVal, Type * _type)
    : Instruction(_func, IRInstOperator::IRINST_OP_CAST, _type)
{
    addOperand(_srcVal);
}

/// @brief 转换成字符串
/// @param str 转换后的字符串
void CastInstruction::toString(std::string & str)
{
    Value * src = getOperand(0);
    Type *  srcType = src->getType(); // 源类型
    Type *  dstType = this->type;     // 目标类型（当前指令的类型）

    if (srcType->isInt1Byte() && dstType->getTypeID() == Type::IntegerTyID) {
        // i1 -> i32
        str = getIRName() + " = zext i1 " + src->getIRName() + " to i32";
    } else if (srcType->getTypeID() == Type::IntegerTyID && dstType->getTypeID() == Type::FloatTyID) {
        // i32 -> float
        str = getIRName() + " = sitofp i32 " + src->getIRName() + " to float";
    } else if (srcType->getTypeID() == Type::FloatTyID && dstType->getTypeID() == Type::IntegerTyID) {
        // float -> i32
        str = getIRName() + " = fptosi float " + src->getIRName() + " to i32";
    } else if (srcType->isPointerType()) {
        str = getIRName() + " = bitcast " + srcType->toString() + " " + src->getIRName() + " to " + dstType->toString();
    }

    else {
        // fallback
        Instruction::toString(str);
    }
}
