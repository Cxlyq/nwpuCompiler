///
/// @file InstSelectorRiscV64.cpp
/// @brief 指令选择器-RISCV64的实现
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-11-21 <td>1.0     <td>zenglj  <td>新做
/// </table>
///
#include <cstdio>
#include <typeinfo>
#include "Common.h"
#include "GlobalVariable.h"
#include "ILocRiscV64.h"
#include "InstSelectorRiscV64.h"
#include "Instruction.h"
#include "LocalVariable.h"
#include "PlatformRiscV64.h"

#include "PointerType.h"
#include "RegVariable.h"
#include "Function.h"

#include "LabelInstruction.h"
#include "GotoInstruction.h"
#include "FuncCallInstruction.h"
#include "MoveInstruction.h"

/// @brief 构造函数
/// @param _irCode 指令
/// @param _iloc ILoc
/// @param _func 函数
// TODO: @JEV055 [指令指派1]完善指令翻译表及处理函数
InstSelectorRiscV64::InstSelectorRiscV64(
    vector<Instruction *> & _irCode, ILocRiscV64 & _iloc, Function * _func, SimpleRegisterAllocatorRiscV64 & allocator)
    : ir(_irCode), iloc(_iloc), func(_func), simpleRegisterAllocator(allocator)
// TODO: @jev055 [指令指派] 在修改之后将simpleRegisterAllocator改为对应的寄存器分配算法文件
{
    translator_handlers[IRInstOperator::IRINST_OP_ENTRY] = &InstSelectorRiscV64::translate_entry;
    translator_handlers[IRInstOperator::IRINST_OP_EXIT] = &InstSelectorRiscV64::translate_exit;

    translator_handlers[IRInstOperator::IRINST_OP_LABEL] = &InstSelectorRiscV64::translate_label;
    translator_handlers[IRInstOperator::IRINST_OP_GOTO] = &InstSelectorRiscV64::translate_goto;
    translator_handlers[IRInstOperator::IRINST_OP_BR_COND] =
        &InstSelectorRiscV64::translate_br_cond; // FIXME: 需要实现条件跳转

    translator_handlers[IRInstOperator::IRINST_OP_ADD_I] = &InstSelectorRiscV64::translate_add_int32;
    translator_handlers[IRInstOperator::IRINST_OP_SUB_I] = &InstSelectorRiscV64::translate_sub_int32;
    translator_handlers[IRInstOperator::IRINST_OP_MUL_I] = &InstSelectorRiscV64::translate_mul_int32;
    translator_handlers[IRInstOperator::IRINST_OP_DIV_I] = &InstSelectorRiscV64::translate_div_int32;
    translator_handlers[IRInstOperator::IRINST_OP_MOD_I] =
        &InstSelectorRiscV64::translate_mod_int32; // FIXME: 需要实现取模
    // translator_handlers[IRInstOperator::IRINST_OP_AND] =
    //     &InstSelectorRiscV64::translate_and_int32; // FIXME: 需要实现逻辑与
    // translator_handlers[IRInstOperator::IRINST_OP_OR] =
    //     &InstSelectorRiscV64::translate_or_int32; // FIXME: 需要实现逻辑或
    translator_handlers[IRInstOperator::IRINST_OP_EQ_I] =
        &InstSelectorRiscV64::translate_eq_int32; // FIXME: 需要实现整数相等
    translator_handlers[IRInstOperator::IRINST_OP_NEQ_I] =
        &InstSelectorRiscV64::translate_neq_int32; // FIXME: 需要实现整数不等
    translator_handlers[IRInstOperator::IRINST_OP_LE_I] =
        &InstSelectorRiscV64::translate_le_int32; // FIXME: 需要实现整数小于等于
    translator_handlers[IRInstOperator::IRINST_OP_GE_I] =
        &InstSelectorRiscV64::translate_ge_int32; // FIXME: 需要实现整数大于等于
    translator_handlers[IRInstOperator::IRINST_OP_LNE_I] =
        &InstSelectorRiscV64::translate_lne_int32; // FIXME: 需要实现整数小于
    translator_handlers[IRInstOperator::IRINST_OP_GNE_I] =
        &InstSelectorRiscV64::translate_gne_int32; // FIXME: 需要实现整数大于
    translator_handlers[IRInstOperator::IRINST_OP_POS_I] =
        &InstSelectorRiscV64::translate_pos_int32; // FIXME: 需要实现整数取正
    translator_handlers[IRInstOperator::IRINST_OP_NEG_I] =
        &InstSelectorRiscV64::translate_neg_int32; // FIXME: 需要实现整数取负
    translator_handlers[IRInstOperator::IRINST_OP_NOT_I] =
        &InstSelectorRiscV64::translate_not_int32; // FIXME: 需要实现整数逻辑非
    translator_handlers[IRInstOperator::IRINST_OP_ADD_F] = &InstSelectorRiscV64::translate_add_float32;
    translator_handlers[IRInstOperator::IRINST_OP_SUB_F] = &InstSelectorRiscV64::translate_sub_float32;
    translator_handlers[IRInstOperator::IRINST_OP_MUL_F] = &InstSelectorRiscV64::translate_mul_float32;
    translator_handlers[IRInstOperator::IRINST_OP_DIV_F] = &InstSelectorRiscV64::translate_div_float32;
    translator_handlers[IRInstOperator::IRINST_OP_EQ_F] =
        &InstSelectorRiscV64::translate_eq_float32; // FIXME: 需要实现浮点数相等
    translator_handlers[IRInstOperator::IRINST_OP_NEQ_F] =
        &InstSelectorRiscV64::translate_neq_float32; // FIXME: 需要实现浮点数不等
    translator_handlers[IRInstOperator::IRINST_OP_GE_F] =
        &InstSelectorRiscV64::translate_ge_float32; // FIXME: 需要实现浮点数大于等于
    translator_handlers[IRInstOperator::IRINST_OP_LE_F] =
        &InstSelectorRiscV64::translate_le_float32; // FIXME: 需要实现浮点数小于等于
    translator_handlers[IRInstOperator::IRINST_OP_LNE_F] =
        &InstSelectorRiscV64::translate_lne_float32; // FIXME: 需要实现浮点数小于
    translator_handlers[IRInstOperator::IRINST_OP_GNE_F] =
        &InstSelectorRiscV64::translate_gne_float32; // FIXME: 需要实现浮点数大于
    translator_handlers[IRInstOperator::IRINST_OP_POS_F] =
        &InstSelectorRiscV64::translate_pos_float32; // FIXME: 需要实现浮点数取正
    translator_handlers[IRInstOperator::IRINST_OP_NEG_F] =
        &InstSelectorRiscV64::translate_neg_float32; // FIXME: 需要实现浮点数取负
    translator_handlers[IRInstOperator::IRINST_OP_NOT_F] =
        &InstSelectorRiscV64::translate_not_float32; // FIXME: 需要实现浮点数逻辑非

    // translator_handlers[IRInstOperator::IRINST_OP_ASSIGN] = &InstSelectorRiscV64::translate_assign;

    translator_handlers[IRInstOperator::IRINST_OP_FUNC_CALL] = &InstSelectorRiscV64::translate_call;
    translator_handlers[IRInstOperator::IRINST_OP_ARG] = &InstSelectorRiscV64::translate_arg;

    translator_handlers[IRInstOperator::IRINST_OP_STORE] = &InstSelectorRiscV64::translate_store; // FIXME: 需要实现存值
    translator_handlers[IRInstOperator::IRINST_OP_LOAD] = &InstSelectorRiscV64::translate_load; // FIXME: 需要实现加载
    translator_handlers[IRInstOperator::IRINST_OP_CAST] =
        &InstSelectorRiscV64::translate_cast; // FIXME: 需要实现类型转换
    translator_handlers[IRInstOperator::IRINST_OP_GEP] = &InstSelectorRiscV64::translate_gep; // FIXME: 需要实现指针获取
}

///
/// @brief 析构函数
///
InstSelectorRiscV64::~InstSelectorRiscV64()
{}

/// @brief 指令选择执行
void InstSelectorRiscV64::run()
{
    for (auto inst: ir) {

        // 逐个指令进行翻译
        if (!inst->isDead()) {
            translate(inst);
        }
    }
}

/// @brief 指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate(Instruction * inst)
{
    // 操作符
    IRInstOperator op = inst->getOp();

    map<IRInstOperator, translate_handler>::const_iterator pIter;
    pIter = translator_handlers.find(op);
    if (pIter == translator_handlers.end()) {
        // 没有找到，则说明当前不支持
        printf("Translate: Operator(%d) not support\n", (int) op);
        return;
    }

    // 开启时输出IR指令作为注释
    if (showLinearIR) {
        outputIRInstruction(inst);
    }

    (this->*(pIter->second))(inst);
}

///
/// @brief 输出IR指令
///
void InstSelectorRiscV64::outputIRInstruction(Instruction * inst)
{
    std::string irStr;
    inst->toString(irStr);
    if (!irStr.empty()) {
        iloc.comment(irStr);
    }
}

/// @brief NOP翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_nop(Instruction * inst)
{
    (void) inst;
    iloc.nop();
}

/// @brief 函数入口指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_entry(Instruction * inst)
{
    // 查看保护的寄存器
    auto & protectedRegNo = func->getProtectedReg();
    auto & protectedRegStr = func->getProtectedRegStr();

    bool first = true;
    for (auto regno: protectedRegNo) {
        if (first) {
            protectedRegStr = PlatformRiscV64::regName[regno];
            first = false;
        } else {
            protectedRegStr += "," + PlatformRiscV64::regName[regno];
        }
    }

    if (!protectedRegStr.empty()) {
        iloc.inst("push", "{" + protectedRegStr + "}");
    }

    // 为fun分配栈帧，含局部变量、函数调用值传递的空间等
    iloc.allocStack(func);
}

/// @brief 函数出口指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_exit(Instruction * inst)
{
    if (inst->getOperandsNum()) {
        // 存在返回值
        Value * retVal = inst->getOperand(0);

        // 赋值给寄存器R0
        iloc.load_var(RISCV64_RA_REG_NO, retVal);
    }

    // 恢复栈空间
    iloc.inst("mov", "sp", "fp");

    // 保护寄存器的恢复
    auto & protectedRegStr = func->getProtectedRegStr();
    if (!protectedRegStr.empty()) {
        iloc.inst("pop", "{" + protectedRegStr + "}");
    }

    iloc.inst("bx", "lr");
}

/// @brief Label指令指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_label(Instruction * inst)
{
    Instanceof(labelInst, LabelInstruction *, inst);

    iloc.label(labelInst->getName());
}

/// @brief goto指令指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_goto(Instruction * inst)
{
    Instanceof(gotoInst, GotoInstruction *, inst);

    // 无条件跳转
    iloc.jump(gotoInst->getTarget()->getName());
}

/// @brief 有条件分支指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_br_cond(Instruction * inst)
{
    // TODO: @JEV055 这里是AI编写，需要修改实现条件跳转
    // Instanceof(brCondInst, BranchCondInstruction *, inst);
    // Value * cond = brCondInst->getCondition();
    // Value * trueTarget = brCondInst->getTrueTarget();
    // Value * falseTarget = brCondInst->getFalseTarget();
    // int32_t cond_reg_no = cond->getRegId();
    // int32_t true_target_reg_no = trueTarget->getRegId();
    // int32_t false_target_reg_no = falseTarget->getRegId();
    // int32_t load_cond_reg_no, load_true_target_reg_no, load_false_target_reg_no;
    // // 看条件变量是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    // if (cond_reg_no == -1) {
    //     // 分配一个寄存器r8
    //     load_cond_reg_no = simpleRegisterAllocator.Allocate(cond);

    // 	// cond -> r8，这里可能由于偏移不满足指令的要求，需要额外分配寄存器
    // 	iloc.load_var(load_cond_reg_no, cond);
    // } else {
    // 	load_cond_reg_no = cond_reg_no;
    // }
    // // 看真分支目标是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    // if (true_target_reg_no == -1) {
    //     // 分配一个寄存器r9
    //     load_true_target_reg_no = simpleRegisterAllocator.Allocate(trueTarget);

    // 	// trueTarget -> r9
    // 	iloc.load_var(load_true_target_reg_no, trueTarget);
    // } else {
    // 	load_true_target_reg_no = true_target_reg_no;
    // }
    // // 看假分支目标是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    // if (false_target_reg_no == -1) {
    //     // 分配一个寄存器r10
    //     load_false_target_reg_no = simpleRegisterAllocator.Allocate(falseTarget);
    //     // falseTarget -> r10
    //     iloc.load_var(load_false_target_reg_no, falseTarget);
    // } else {
    //     load_false_target_reg_no = false_target_reg_no;
    // }
    // // 条件寄存器r8的值为0，则跳转到假分支，否则跳转到真分支
    // iloc.inst(
    //     "beqz",
    //     PlatformRiscV64::regName[load cond_reg_no],
    //     PlatformRiscV64::regName[load_false_target_reg_no],
    //     PlatformRiscV64::regName[load_true_target_reg_no]);
    // // 释放寄存器
    // simpleRegisterAllocator.free(cond);
    // simpleRegisterAllocator.free(trueTarget);
    // simpleRegisterAllocator.free(falseTarget);
}

/// @brief 二元操作指令翻译成RISCV64汇编
/// @param inst IR指令
/// @param operator_name 操作码
/// @param rs_reg_no 结果寄存器号
/// @param op1_reg_no 源操作数1寄存器号
/// @param op2_reg_no 源操作数2寄存器号
void InstSelectorRiscV64::translate_two_operator(Instruction * inst, string operator_name)
{
    Value * result = inst;
    Value * arg1 = inst->getOperand(0);
    Value * arg2 = inst->getOperand(1);

    int32_t arg1_reg_no = arg1->getRegId();
    int32_t arg2_reg_no = arg2->getRegId();
    int32_t result_reg_no = inst->getRegId();
    int32_t load_result_reg_no, load_arg1_reg_no, load_arg2_reg_no;

    // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg1_reg_no == -1) {

        // 分配一个寄存器r8
        load_arg1_reg_no = simpleRegisterAllocator.Allocate(arg1);

        // arg1 -> r8，这里可能由于偏移不满足指令的要求，需要额外分配寄存器
        iloc.load_var(load_arg1_reg_no, arg1);
    } else {
        load_arg1_reg_no = arg1_reg_no;
    }

    // 看arg2是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg2_reg_no == -1) {

        // 分配一个寄存器r9
        load_arg2_reg_no = simpleRegisterAllocator.Allocate(arg2);

        // arg2 -> r9
        iloc.load_var(load_arg2_reg_no, arg2);
    } else {
        load_arg2_reg_no = arg2_reg_no;
    }

    // 看结果变量是否是寄存器，若不是则需要分配一个新的寄存器来保存运算的结果
    if (result_reg_no == -1) {
        // 分配一个寄存器r10，用于暂存结果
        load_result_reg_no = simpleRegisterAllocator.Allocate(result);
    } else {
        load_result_reg_no = result_reg_no;
    }

    // r8 + r9 -> r10
    iloc.inst(
        operator_name,
        PlatformRiscV64::regName[load_result_reg_no],
        PlatformRiscV64::regName[load_arg1_reg_no],
        PlatformRiscV64::regName[load_arg2_reg_no]);

    // 结果不是寄存器，则需要把rs_reg_name保存到结果变量中
    if (result_reg_no == -1) {

        // 这里使用预留的临时寄存器，因为立即数可能过大，必须借助寄存器才可操作。
		// TODO:[寄存器分配]无地可放运算结果则压栈，建议给予寄存器（因为通常紧接着就store了）
        // r10 -> result
        iloc.store_var(load_result_reg_no, result, RISCV64_TMP_REG_NO);
    }

    // 释放寄存器
    simpleRegisterAllocator.free(arg1);
    simpleRegisterAllocator.free(arg2);
    simpleRegisterAllocator.free(result);
}

// /// @brief 一元操作指令翻译成RISCV64汇编
// /// @param inst IR指令
// /// @param operator_name 操作码
// /// @param rs_reg_no 结果寄存器号
// /// @param op1_reg_no 源操作数寄存器号
// void InstSelectorRiscV64::translate_one_operator(Instruction * inst, string operator_name)
// {
//     Value * result = inst;
//     Value * arg1 = inst->getOperand(0);

//     int32_t arg1_reg_no = arg1->getRegId();
//     int32_t result_reg_no = inst->getRegId();
//     int32_t load_arg1_reg_no, load_result_reg_no;

//     // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
//     if (arg1_reg_no == -1) {
//         // 分配一个寄存器r8
//         load_arg1_reg_no = simpleRegisterAllocator.Allocate(arg1);

//         // arg1 -> r8，这里可能由于偏移不满足指令的要求，需要额外分配寄存器
//         iloc.load_var(load_arg1_reg_no, arg1);
//     } else {
//         load_arg1_reg_no = arg1_reg_no;
//     }

//     // 看结果变量是否是寄存器，若不是则需要分配一个新的寄存器来保存运算的结果
//     if (result_reg_no == -1) {
//         // 分配一个寄存器r10，用于暂存结果
//         load_result_reg_no = simpleRegisterAllocator.Allocate(result);
//     } else {
//         load_result_reg_no = result_reg_no;
//     }

//     // 根据操作名称生成相应的一元运算指令
//     iloc.inst(operator_name, PlatformRiscV64::regName[load_result_reg_no],
//     PlatformRiscV64::regName[load_arg1_reg_no]);

//     // 结果不是寄存器，则需要把rs_reg_name保存到结果变量中
//     if (result_reg_no == -1) {
//         // 这里使用预留的临时寄存器，因为立即数可能过大，必须借助寄存器才可操作。
// 		TODO:[寄存器分配]无地可放运算结果则压栈，建议给予寄存器（因为通常紧接着就store了）
//         // r10 -> result
//         iloc.store_var(load_result_reg_no, result, RISCV64_TMP_REG_NO);
//     }

//     // 释放寄存器
//     simpleRegisterAllocator.free(arg1);
//     simpleRegisterAllocator.free(result);
// }

/// @brief 整数加法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_add_int32(Instruction * inst)
{
    translate_two_operator(inst, "add");
}

/// @brief 整数减法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_sub_int32(Instruction * inst)
{
    translate_two_operator(inst, "sub");
}

/// @brief 整数乘法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_mul_int32(Instruction * inst)
{
    translate_two_operator(inst, "mul");
}

/// @brief 整数除法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_div_int32(Instruction * inst)
{
    translate_two_operator(inst, "divu");
}

/// @brief 整数取模指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_mod_int32(Instruction * inst)
{
    translate_two_operator(inst, "remu");
}

/// @brief 整数相等指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_eq_int32(Instruction * inst)
{
    translate_two_operator(inst, "seqz");
}

/// @brief 整数不等指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_neq_int32(Instruction * inst)
{
    translate_two_operator(inst, "snez");
}

/// @brief 整数小于等于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_le_int32(Instruction * inst)
{
    translate_two_operator(inst, "sle");
}

/// @brief 整数大于等于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_ge_int32(Instruction * inst)
{
    translate_two_operator(inst, "sge");
}

/// @brief 整数小于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_lne_int32(Instruction * inst)
{
    translate_two_operator(inst, "slt");
}

/// @brief 整数大于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_gne_int32(Instruction * inst)
{
    translate_two_operator(inst, "sgt");
}

/// @brief 整数取正指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_pos_int32(Instruction * inst)
{
    //可以优化不需要操作
}

/// @brief 整数取负指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_neg_int32(Instruction * inst)
{
    // FIXME: [指令指派补充] 需要处理立即数溢出问题
    translate_two_operator(inst, "neg");
}

/// @brief 整数逻辑非指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_not_int32(Instruction * inst)
{
    // 逻辑非操作，直接翻译成seqz指令
    // 即如果结果为0，则返回1，否则返回0
    translate_two_operator(inst, "seqz");
}

/// @brief 浮点数加法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_add_float32(Instruction * inst)
{
    translate_two_operator(inst, "fadd.s");
}

/// @brief 浮点数减法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_sub_float32(Instruction * inst)
{
    translate_two_operator(inst, "fsub.s");
}

/// @brief 浮点数乘法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_mul_float32(Instruction * inst)
{
    translate_two_operator(inst, "fmul.s");
}

/// @brief 浮点数除法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_div_float32(Instruction * inst)
{
    translate_two_operator(inst, "fdiv.s");
}

/// @brief 浮点数相等指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_eq_float32(Instruction * inst)
{
    translate_two_operator(inst, "feq.s");
}

/// @brief 浮点数不等指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_neq_float32(Instruction * inst)
{
    translate_two_operator(inst, "fne.s");
}

/// @brief 浮点数大于等于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_ge_float32(Instruction * inst)
{
    translate_two_operator(inst, "fge.s");
}

/// @brief 浮点数小于等于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_le_float32(Instruction * inst)
{
    translate_two_operator(inst, "fle.s");
}

/// @brief 浮点数小于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_lne_float32(Instruction * inst)
{
    translate_two_operator(inst, "flt.s");
}

/// @brief 浮点数大于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_gne_float32(Instruction * inst)
{
    translate_two_operator(inst, "fgt.s");
}

/// @brief 浮点数取正指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_pos_float32(Instruction * inst)
{
    // 置空 translate_nop()
}

/// @brief 浮点数取负指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_neg_float32(Instruction * inst)
{
    // 直接翻译成neg指令
    translate_two_operator(inst, "fneg.s");
}

/// @brief 浮点数逻辑非指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_not_float32(Instruction * inst)
{
    // 逻辑非操作，直接翻译成feqz指令
    // 即如果结果为0，则返回1，否则返回0
    translate_two_operator(inst, "feqz.s");
}

/// @brief 赋值指令翻译成RISCV64汇编
/// @param inst IR指令
// TODO:[决断]疑似弃用
void InstSelectorRiscV64::translate_assign(Instruction * inst)
{
    Value * result = inst->getOperand(0);
    Value * arg1 = inst->getOperand(1);

    int32_t arg1_regId = arg1->getRegId();
    int32_t result_regId = result->getRegId();

    if (arg1_regId != -1) {
        // 寄存器 => 内存
        // 寄存器 => 寄存器

        // r8 -> rs 可能用到r9
        iloc.store_var(arg1_regId, result, RISCV64_TMP_REG_NO);
    } else if (result_regId != -1) {
        // 内存变量 => 寄存器

        iloc.load_var(result_regId, arg1);
    } else {
        // 内存变量 => 内存变量

        int32_t temp_regno = simpleRegisterAllocator.Allocate();

        // arg1 -> r8
        iloc.load_var(temp_regno, arg1);

        // r8 -> rs 可能用到r9
        iloc.store_var(temp_regno, result, RISCV64_TMP_REG_NO);

        simpleRegisterAllocator.free(temp_regno);
    }
}

/// @brief 函数调用指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_call(Instruction * inst)
{
    FuncCallInstruction * callInst = dynamic_cast<FuncCallInstruction *>(inst);

    int32_t operandNum = callInst->getOperandsNum();

    if (operandNum != realArgCount) {

        // 两者不一致 也可能没有ARG指令，正常
        if (realArgCount != 0) {

            minic_log(LOG_ERROR, "ARG指令的个数与调用函数个数不一致");
        }
    }

    if (operandNum) {

        // 强制占用这几个寄存器参数传递的寄存器
        simpleRegisterAllocator.Allocate(10);
        simpleRegisterAllocator.Allocate(11);
        simpleRegisterAllocator.Allocate(12);
        simpleRegisterAllocator.Allocate(13);
        simpleRegisterAllocator.Allocate(14);
        simpleRegisterAllocator.Allocate(15);
        simpleRegisterAllocator.Allocate(16);
        simpleRegisterAllocator.Allocate(17);

        // 前四个的后面参数采用栈传递
        int esp = 0;
        for (int32_t k = 7; k < operandNum; k++) {

            auto arg = callInst->getOperand(k);

            // 新建一个内存变量，用于栈传值到形参变量中
            MemVariable * newVal = func->newMemVariable((Type *) PointerType::get(arg->getType()));
            newVal->setMemoryAddr(RISCV64_SP_REG_NO, esp);
            esp += 4;

            Instruction * assignInst = new MoveInstruction(func, newVal, arg);

            // 翻译赋值指令
            translate_assign(assignInst);

            delete assignInst;
        }

        for (int32_t k = 0; k < operandNum && k < 4; k++) {

            auto arg = callInst->getOperand(k);

            // 检查实参的类型是否是临时变量。
            // 如果是临时变量，该变量可更改为寄存器变量即可，或者设置寄存器号
            // 如果不是，则必须开辟一个寄存器变量，然后赋值即可

            Instruction * assignInst = new MoveInstruction(func, PlatformRiscV64::intRegVal[k], arg);

            // 翻译赋值指令
            translate_assign(assignInst);

            delete assignInst;
        }
    }

    iloc.call_fun(callInst->getName());

    if (operandNum) {
        simpleRegisterAllocator.free(10);
        simpleRegisterAllocator.free(11);
        simpleRegisterAllocator.free(12);
        simpleRegisterAllocator.free(13);
        simpleRegisterAllocator.free(14);
        simpleRegisterAllocator.free(15);
        simpleRegisterAllocator.free(16);
        simpleRegisterAllocator.free(17);
    }

    // 赋值指令
    if (callInst->hasResultValue()) {

        // 新建一个赋值操作
        Instruction * assignInst = new MoveInstruction(func, callInst, PlatformRiscV64::intRegVal[0]);

        // 翻译赋值指令
        translate_assign(assignInst);

        delete assignInst;
    }

    // 函数调用后清零，使得下次可正常统计
    realArgCount = 0;
}

/// @brief 实参指令翻译成RISCV64汇编
/// @param inst
void InstSelectorRiscV64::translate_arg(Instruction * inst)
{
    // 翻译之前必须确保源操作数要么是寄存器，要么是内存，否则出错。
    Value * src = inst->getOperand(0);

    // 当前统计的ARG指令个数
    int32_t regId = src->getRegId();

    if (realArgCount < 8) {
        // 前四个参数
        if (regId != -1) {
            if (regId != realArgCount) {
                // 肯定寄存器分配有误
                minic_log(LOG_ERROR, "第%d个ARG指令对象寄存器分配有误: %d", argCount + 1, regId);
            }
        } else {
            minic_log(LOG_ERROR, "第%d个ARG指令对象不是寄存器", argCount + 1);
        }
    } else {
        // 必须是内存分配，若不是则出错
        int32_t baseRegId;
        bool    result = src->getMemoryAddr(&baseRegId);
        if ((!result) || (baseRegId != RISCV64_SP_REG_NO)) {

            minic_log(LOG_ERROR, "第%d个ARG指令对象不是SP寄存器寻址", argCount + 1);
        }
    }

    realArgCount++;
}

/// @brief 存储指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_store(Instruction * inst)
{
    // HACK: @JEV055 临时更改了getOperand(pos)的参数，后续统一标准之后会更改回来
    // 存储指令，必须是内存变量
    // IR: store src, dst
    // RISCV64: 根据变量类型判断

    // 在这里判断源类型是啥，目标变量类型是啥，以及源是否是寄存器变量
    // 目前推测的可能情况：src:Instruction(可能reg),LocalVariable(可能reg),ConstInt(啥都不是)
    // dst:GlobalVariable(必非reg),LocalVariable(可能reg)
    int32_t src_regId = -1;
    int32_t dst_regId = -1;
    Value * src = inst->getOperand(1);
    Value * dst = inst->getOperand(0);
    std::cout << "[InstSelectorRiscV64::translate_store] srctype:" << typeid(inst->getOperand(1)).name() << "\n"
              << "[InstSelectorRiscV64::translate_store] dsttype:" << typeid(inst->getOperand(0)).name() << "\n";
	if (Instanceof(ConstIntSrc, ConstInt *, src)) {
        // 源操作数是立即数
        // FIXME: 目前只支持整数
        std::cout << "[InstSelectorRiscV64::translate_store] src is ConstInt\n";
        if (Instanceof(LVDst, LocalVariable *, dst)) {
            dst_regId = LVDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is LocalVariable, regid=" << dst_regId << "\n";
        } else if (Instanceof(GLDst, GlobalVariable *, dst)) {
            dst_regId = GLDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is GlobalVariable, regid=" << dst_regId << "\n";
        } // 这里dst_regId是寄存器号，dst是内存变量
        else {
            std::cout << "[InstSelectorRiscV64::translate_store] dst is not a GlobalVariable/LocalVariable\n";
            return;
        }
        if (dst_regId != -1) {
            iloc.load_imm(dst_regId, ConstIntSrc->getVal());
        } else {
            int32_t addr_regno = simpleRegisterAllocator.Allocate();
            int32_t data_regno = simpleRegisterAllocator.Allocate();
            iloc.load_imm(data_regno, ConstIntSrc->getVal());
            iloc.store_var(data_regno, dst, addr_regno);
            simpleRegisterAllocator.free(data_regno);
            simpleRegisterAllocator.free(addr_regno);
        }
    } else {
        // 源变量是Instruction临时变量或Local局部变量的情况
        if (Instanceof(InstSrc, Instruction *, src)) {
            src_regId = InstSrc->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] src is Instruction, regid=" << src_regId << "\n";
        } else if (Instanceof(LVSrc, LocalVariable *, src)) {
            src_regId = LVSrc->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] src is LocalVariable, regid=" << src_regId << "\n";
        } else {
            std::cout << "[InstSelectorRiscV64::translate_store] src is not a Instruction/Local variable\n";
            return;
        }
        if (Instanceof(LVDst, LocalVariable *, dst)) {
            // 目的变量是局部变量，不需要额外分配指针寄存器
            dst_regId = LVDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is LocalVariable, regid=" << dst_regId << "\n";
        } else if (Instanceof(GLDst, GlobalVariable *, dst)) {
            // 目的变量是全局变量，需要分配指针寄存器
            dst_regId = GLDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is GlobalVariable, regid=" << dst_regId << "\n";
        } else {
            std::cout << "[InstSelectorRiscV64::translate_store] dst is not a GlobalVariable/LocalVariable\n";
            return;
        }
        int32_t addr_regno = simpleRegisterAllocator.Allocate();
        if (src_regId != -1) {
            // 源操作数是寄存器，则直接存储到寄存器中
            iloc.store_var(src_regId, dst, addr_regno); // XXX: 考虑修改函数，看是否需要额外指派地址寄存器
        } else {
            // 源操作数是内存变量，则需要先load到寄存器中
            int32_t data_regno = simpleRegisterAllocator.Allocate(); // FIXME:考虑溢出情况
            // t0 <- src
            iloc.load_var(data_regno, src, RISCV64_SP_REG_NO);
            // t0 -> dst
            iloc.store_var(data_regno, dst, addr_regno);
            simpleRegisterAllocator.free(data_regno);
        }
        simpleRegisterAllocator.free(addr_regno);
    }
}

/// @brief 加载指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_load(Instruction * inst)
{
    // IR dst:result=load (type) src:operand[0]
    // 可能的情况分析：dst:Instruction(可能reg?)src:GlobalVariable(必定mem),LocalVariable(可能reg)
    int32_t src_regId = -1;
    int32_t dst_regId = -1;
    Value * src = inst->getOperand(0);
    Value * dst = inst;
    std::cout << "[InstSelectorRiscV64::translate_store] srctype:" << typeid(inst->getOperand(1)).name() << "\n"
              << "[InstSelectorRiscV64::translate_store] dsttype:" << typeid(inst->getOperand(0)).name() << "\n";

	// 源变量是Instruction临时变量或Local局部变量的情况
	if (Instanceof(InstSrc, Instruction *, src)) {
		src_regId = InstSrc->getRegId();
		std::cout << "[InstSelectorRiscV64::translate_store] src is Instruction, regid=" << src_regId << "\n";
	} else if (Instanceof(LVSrc, LocalVariable *, src)) {
		src_regId = LVSrc->getRegId();
		std::cout << "[InstSelectorRiscV64::translate_store] src is LocalVariable, regid=" << src_regId << "\n";
	} else if (Instanceof(GLSrc, GlobalVariable *, src)) {
		src_regId = GLSrc->getRegId();
		std::cout << "[InstSelectorRiscV64::translate_store] src is LocalVariable, regid=" << src_regId << "\n";
	} else {
		std::cout << "[InstSelectorRiscV64::translate_store] src is not a Instruction/Local variable\n";
		return;
	}

	// 目的变量是局部变量，不需要额外分配指针寄存器
	dst_regId = inst->getRegId();
	std::cout << "[InstSelectorRiscV64::translate_store] dst is LocalVariable, regid=" << dst_regId << "\n";
	if (src_regId != -1) {
		// 源操作数是寄存器，则直接存储到寄存器中
		iloc.store_var(src_regId, dst); // XXX: 考虑修改函数，看是否需要额外指派地址寄存器
	} else {
		// 源操作数是内存变量，则需要先load到寄存器中

		int32_t data_regno = simpleRegisterAllocator.Allocate(); // FIXME:考虑溢出情况
		int32_t addr_regno = simpleRegisterAllocator.Allocate();
		//  data_reg<- src
		iloc.load_var(data_regno, src, RISCV64_SP_REG_NO);
		// data_reg -> dst
		iloc.store_var(data_regno, dst);
		simpleRegisterAllocator.free(addr_regno);
		simpleRegisterAllocator.free(data_regno);
	}
    
}

/// @brief Cast指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_cast(Instruction * inst)
{
    // TODO: @JEV055 [指令指派] 需要实现类型转换
}

/// @brief GEP指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_gep(Instruction * inst)
{
    // TODO: @JEV055 [指令指派] 需要实现指针获取
    // GEP指令用于获取指针的地址，通常用于数组或结构体
    // 这里需要根据指针的类型和偏移量来计算地址
    // 例如，对于数组指针，可以通过基地址加上偏移量来计算
    // 对于结构体指针，可以通过基地址加上字段偏移量来
    // 计算地址
    // 需要注意的是，GEP指令的结果是一个指针类型的
    // 变量，因此需要将结果存储到一个寄存器或内存变量
    // 中
}
