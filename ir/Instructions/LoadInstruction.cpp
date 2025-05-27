#include "LoadInstruction.h"

/// @brief 构造函数
/// @param _func 所属函数
/// @param result 加载结果保存的目标变量
/// @param addr 源地址
LoadInstruction::LoadInstruction(Function * _func, Value * addr)
    : Instruction(_func, IRInstOperator::IRINST_OP_LOAD, addr->getType()) // LOAD 操作，返回类型取决于目标变量类型
{
    // TODO暂时不要result，默认寄存器
    //  addOperand(result); // 保存加载的结果（类似于左值）
    addOperand(addr); // 加载的地址（类似于右值）
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
        default:
            str = getIRName() + " = load from " + addr->getIRName();

            break;
    }
}
