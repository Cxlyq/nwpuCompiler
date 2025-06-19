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
#include <iostream>

/// @brief 构造函数
/// @param _op 操作符
/// @param _result 结果操作数
/// @param _srcVal 源操作数
CastInstruction::CastInstruction(Function * _func, Value * _srcVal, Type * _type)
    : Instruction(_func, IRInstOperator::IRINST_OP_CAST, _type)
{
    addOperand(_srcVal);
    if (_type->getTypeID() == Type::PointerTyID && _srcVal->getType()->getTypeID() != Type::PointerTyID) {
        // 如果目标类型是指针类型，但源操作数不是指针类型，则需要进行强制转换
        Type::TypeID id = _type->getPointeeType()->getTypeID();
        Type *       dstType = nullptr;

        switch (id) {
            case Type::IntegerTyID:
                dstType = IntegerType::getTypeInt();
                break;
            case Type::FloatTyID:
                dstType = FloatType::getType();
                break;

            // 其他类型……
            default:
                // std::cout(false && "Unknown type ID");
                ;
        }
        this->type = dstType;
    } else {
        this->type = _type;
    }
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
    } else if (
        (srcType->getTypeID() == Type::IntegerTyID && dstType->getTypeID() == Type::FloatTyID) ||
        (srcType->getTypeID() == Type::IntegerTyID && dstType->getPointeeType()->getTypeID() == Type::FloatTyID)) {
        // i32 -> float
        str = getIRName() + " = sitofp i32 " + src->getIRName() + " to float";
    } else if (
        (srcType->getTypeID() == Type::FloatTyID && dstType->getTypeID() == Type::IntegerTyID) ||
        (srcType->getTypeID() == Type::FloatTyID && dstType->getPointeeType()->getTypeID() == Type::IntegerTyID)) {
        // float -> i32
        str = getIRName() + " = fptosi float " + src->getIRName() + " to i32";
    } else if (srcType->isPointerType()) {
        str = getIRName() + " = bitcast " + srcType->toString() + " " + src->getIRName() + " to " + dstType->toString();
    } else if (srcType->isInt32Type() && dstType->isInt1Byte()) {
        // i32 -> i1
        str = getIRName() + " = trunc i32 " + src->getIRName() + " to i1";
    }

    else {
        // fallback
        Instruction::toString(str);
    }
}
