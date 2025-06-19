#include "StoreInstruction.h"
#include "CastInstruction.h"

/// @brief 构造函数，初始化目标地址和存储的值
/// @param _func 所属的函数
/// @param addr 存储目标地址
/// @param val 存储的值
StoreInstruction::StoreInstruction(Function * _func, Value * addr, Value * val)
    : Instruction(_func, IRInstOperator::IRINST_OP_STORE, VoidType::getType()) // 假设STORE操作
{
    addOperand(addr); // 地址操作数
    addOperand(val);  // 值操作数
}

/// @brief 转换成字符串显示
/// @param str 转换后的字符串
void StoreInstruction::toString(std::string & str)
{
    Value * addr = getOperand(0); // 获取地址
    Value * val = getOperand(1);  // 获取存储的值

    switch (addr->getType()->getTypeID()) {
        case Type::IntegerTyID:
            str = "store i32 " + val->getIRName() + ", i32* " + addr->getIRName() + ", align 4";
            break;
        case Type::FloatTyID:
            str = "store float " + val->getIRName() + ", float* " + addr->getIRName() + ", align 4";
            break;
        case Type::PointerTyID:
            str = "store " + val->getType()->toString() + " " + val->getIRName() + ", " + addr->getType()->toString() +
                  " " + addr->getIRName() + ", align 4";
            break;
        case Type::ArrayTyID:
            str = "store " + val->getType()->toString() + " " + val->getIRName() + ", " + addr->getType()->toString() +
                  "* " + addr->getIRName() + ", align 4";
            break;
        default:
            str = "store  " + val->getIRName() + ",  " + addr->getIRName() + ", align 4";

            break;
    }
}
