#include "LoadInstruction.h"
#include "CastInstruction.h"
#include "Type.h"
#include "IntegerType.h"
#include "FloatType.h"
#include "PointerType.h"

/// @brief 构造函数
/// @param _func 所属函数
/// @param result 加载结果保存的目标变量
/// @param addr 源地址
LoadInstruction::LoadInstruction(Function * _func, Value * addr)
    : Instruction(_func, IRInstOperator::IRINST_OP_LOAD, addr->getType()) // LOAD 操作，返回类型取决于目标变量类型
{
    addOperand(addr); // 加载的地址（类似于右值）

    if (addr->getType()->isPointerType()) {
        Type::TypeID id = addr->getType()->getPointeeType()->getTypeID();
        if (id == Type::IntegerTyID) {
            this->setType(IntegerType::getTypeInt()); // 如果是指向整数的指针，则结果类型为整数
        } else if (id == Type::FloatTyID) {
            this->setType(FloatType::getType()); // 如果是指向浮点数的指针，则结果类型为浮点数
        } else if (id == Type::PointerTyID) {
            PointerType * pointerType = new PointerType(addr->getType()->getPointeeType()->getPointeeType());
            this->type = pointerType;
            // this->setType(addr->getType()->getPointeeType()); // 如果是指向指针的指针，则结果类型为指针
        } // 其他情况，直接使用指向的类型
    }
}
/// @brief 转换为字符串形式
/// @param str 输出的字符串
void LoadInstruction::toString(std::string & str)
{
    // Value * result = getOperand(0); // 被赋值的变量
    Value * addr = getOperand(0); // 从哪个地址加载
    switch (addr->getType()->getTypeID()) {
        case Type::IntegerTyID:
            str = getIRName() + " = load i32, i32* " + addr->getIRName() + ", align 4";
            break;
        case Type::FloatTyID:
            str = getIRName() + " = load float, float* " + addr->getIRName() + ", align 4";
            break;
        case Type::PointerTyID:
            str = getIRName() + " = load " + this->getType()->toString() + ", " + addr->getType()->toString() + " " +
                  addr->getIRName() + ", align 4";
            break;
        default:
            str = getIRName() + " = load from " + addr->getIRName();

            break;
    }
}
