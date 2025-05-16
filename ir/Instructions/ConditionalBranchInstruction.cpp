#include "VoidType.h"

#include "ConditionalBranchInstruction.h"

///
/// @brief 构造函数
/// @param _func 所属函数
///
ConditionalInstruction::ConditionalInstruction(
    Function * _func, Value * _cond_val, LabelInstruction * _true_branch_label, LabelInstruction * _false_branch_target)
    : Instruction(_func, IRInstOperator::IRINST_OP_LABEL, VoidType::getType())
{
    cond_val = _cond_val;
    true_branch_label = _true_branch_label;
    false_branch_target = _false_branch_target;
}

/// @brief 转换成字符串
/// @param str 返回指令字符串
void ConditionalInstruction::toString(std::string & str)
{
    // Example: br i1 %t1, label %then_block, label %else_block

    // 获取条件值（Value* condition）的 IR 名称
    std::string cond_name = cond_val->getIRName();

    // 获取真分支目标标签（LabelInstruction* true_target）的 IR 名称
    // 假设 LabelInstruction 类有 getIRName() 方法
    std::string true_label_name = true_branch_label->getIRName();

    // 获取假分支目标标签（LabelInstruction* false_target）的 IR 名称
    std::string false_label_name = false_branch_target->getIRName();

    // 构建指令文本字符串
    // 注意：条件值在 LLVM IR 中必须是 i1 类型，这里的 toString 直接写死了 i1。
    // 这意味着 ir_ifelse 在创建 ConditionalBranchInstruction 之前，
    // 必须确保 condition->val 已经是 i1 类型的值 (例如通过 icmp 或其他逻辑操作)。
    str = "\tbr i1 " + cond_name + ", label " + true_label_name + ", label " + false_label_name;
}