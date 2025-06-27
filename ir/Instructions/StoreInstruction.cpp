#include "StoreInstruction.h"
#include "CastInstruction.h"
#include "Value.h"
#include <cstdint>
#include <sstream> // std::ostringstream 定义在此头文件中
#include <iomanip> // std::hexfloat 等格式操控器定义在这里

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

    // switch (addr->getType()->getTypeID()) {
    //     case Type::IntegerTyID:
    //         str = "store i32 " + val->getIRName() + ", i32* " + addr->getIRName() + ", align 4";
    //         break;
    //     case Type::FloatTyID:
    //         str = "store float " + val->getIRName() + ", float* " + addr->getIRName() + ", align 4";
    //         break;
    //     case Type::PointerTyID:
    //         str = "store " + val->getType()->toString() + " " + val->getIRName() + ", " + addr->getType()->toString()
    //         +
    //               " " + addr->getIRName() + ", align 4";
    //         break;
    //     case Type::ArrayTyID:
    //         str = "store " + val->getType()->toString() + " " + val->getIRName() + ", " + addr->getType()->toString()
    //         +
    //               "* " + addr->getIRName() + ", align 4";
    //         break;
    //     default:
    //         str = "store  " + val->getIRName() + ",  " + addr->getIRName() + ", align 4";

    //         break;
    // }
    // 根据地址类型生成对应的存储指令字符串
    if (addr->getType()->getTypeID() == Type::IntegerTyID) {
        str = "store i32 " + val->getIRName() + ", i32* " + addr->getIRName() + ", align 4";
    } else if (addr->getType()->getTypeID() == Type::FloatTyID) {
        if (val->getValueCategory() == ValueCategory::IMMEDIATE) {
            double             floatVal = val->getFloatInitVal();
            float              cut = (float) floatVal;
            double             extend = (double) cut;
            uint64_t           floatBits = bitcast<double, uint64_t>(extend);
            std::ostringstream oss;
            oss << "0x" << std::hex << std::uppercase << floatBits;
            std::string floatStr = oss.str();
            str = "store float " + floatStr + ", float* " + addr->getIRName() + ", align 4";
        } else {
            str = "store float " + val->getIRName() + ", float* " + addr->getIRName() + ", align 4";
        }

    }
    ///对应两者都是指针
    else if (
        (val->getType()->getTypeID() == Type::PointerTyID) && (addr->getType()->getTypeID() == Type::PointerTyID)) {
        str = "store " + val->getType()->toString() + " " + val->getIRName() + ", " + addr->getType()->toString() +
              "* " + addr->getIRName() + ", align 8";
    } else if (addr->getType()->getTypeID() == Type::PointerTyID) {
        str = "store " + val->getType()->toString() + " " + val->getIRName() + ", " + addr->getType()->toString() +
              " " + addr->getIRName() + ", align 8";
    } else if (addr->getType()->getTypeID() == Type::ArrayTyID) {
        str = "store " + val->getType()->toString() + " " + val->getIRName() + ", " + addr->getType()->toString() +
              "* " + addr->getIRName() + ", align 4";
    } else {
        str = "store  " + val->getIRName() + ",  " + addr->getIRName() + ", align 4";
    }
}
