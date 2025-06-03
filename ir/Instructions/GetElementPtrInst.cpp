#include "GetElementPtrInst.h"
#include <sstream>
#include "ArrayType.h"
#include "PointerType.h"
#include "Type.h"

/// 构造函数实现
GetElementPtrInst::GetElementPtrInst(
    Function * func, Value * base, Type * gepType, const std::vector<Value *> & indices)
    : Instruction(func, IRInstOperator::IRINST_OP_GEP, gepType)
{
    // 设置操作数：第一个是 base，后面是 indices
    addOperand(base);
    //: 这里应该继续添加一个value，不过输出的结果仍然是一个类型，所以就偷懒了
    addOperand(base);
    for (auto * idx: indices) {
        addOperand(idx);
    }

    // 设置最终的结果类型为指向最后一个元素的指针
    Type * current = gepType;
    for (size_t i = 1; i < indices.size(); ++i) { // 跳过第一个 i64 0（数组对象指针偏移）
        if (current->isArrayType()) {
            current = static_cast<ArrayType *>(current)->getElementType();
        } else {
            break;
        }
    }
    this->type = const_cast<PointerType *>(PointerType::get(current));
}

/// toString 实现
void GetElementPtrInst::toString(std::string & str)
{
    std::ostringstream oss;
    oss << getIRName() << " = getelementptr inbounds ";

    oss << getOperand(0)->getType()->toString() << ", " << getOperand(0)->getType()->toString() << "* "
        << getOperand(0)->getIRName();

    for (size_t i = 1; i < getOperands().size(); ++i) {
        oss << ", i64 " << getOperand(i)->getIRName();
    }

    str = oss.str();
}
