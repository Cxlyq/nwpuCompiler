#include "AddInstruction.h"
AddInstruction::AddInstruction(Value * res, Value * left, Value * right, Function * func)
    : Instruction(func, IRInstOperator::IRINST_OP_ADD_I, VoidType::getType()), result(res), lhs(left), rhs(right)
{

    // TODO 设置 use-def 链
    // result->setDef(this);
    // lhs->addUse(this);
    // rhs->addUse(this);

    // 添加到操作数列表（用于统一管理）
    addOperand(lhs);
    addOperand(rhs);

    // TODO 如果是编译期可计算常量，加上初始值
    // if (lhs->getValueType() == Value::ValueType::INT && rhs->getValueType() == Value::ValueType::INT) {
    //     result->setInitVal(lhs->getIntVal() + rhs->getIntVal());
    // } else if (lhs->getValueType() == Value::ValueType::FLOAT && rhs->getValueType() == Value::ValueType::FLOAT) {
    //     result->setInitVal(lhs->getFloatVal() + rhs->getFloatVal());
    // } else {
    //     // 可选处理 INT + FLOAT 或 FLOAT + INT（根据语言规范）
    //     // 或者干脆忽略，交给后续阶段处理
    // }
}

void AddInstruction::toString(std::string & str)
{
    str += result->getIRName() + " = add " + lhs->getIRName() + ", " + rhs->getIRName();
}
