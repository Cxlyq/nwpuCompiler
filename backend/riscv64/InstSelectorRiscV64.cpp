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
#include <iostream>
#include <string>
#include <typeinfo>
#include <cstdint>
#include "Common.h"
#include "ConstFloat.h"
#include "ConstInt.h"
#include "GlobalVariable.h"
#include "ILocRiscV64.h"
#include "InstSelectorRiscV64.h"
#include "Instruction.h"
#include "LocalVariable.h"
#include "PlatformRiscV64.h"
#include "LoadInstruction.h"
#include "PointerType.h"
#include "RegVariable.h"
#include "Function.h"
#include "UnaryInstruction.h"
#include "LabelInstruction.h"
#include "GotoInstruction.h"
#include "FuncCallInstruction.h"
#include "MoveInstruction.h"
#include "ConditionalBranchInstruction.h" /// @brief 构造函数
#include "BinaryInstruction.h"
#include "GetElementPtrInst.h"
#include "Value.h"
#include "math.h"
/// @param _irCode 指令
/// @param _iloc ILoc
/// @param _func 函数
// TODO: @JEV055 [指令指派1]完善指令翻译表及处理函数
InstSelectorRiscV64::InstSelectorRiscV64(
    vector<Instruction *> & _irCode, ILocRiscV64 & _iloc, Function * _func, GraphColoringRegisterAllocator & allocator)
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
    for (auto inst: ir) {
        if (inst->getUserNum() == 0) {
            simpleRegisterAllocator.free(inst);
        }
    }
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
    // 获取被保护寄存器
    auto & protectedRegNo = func->getProtectedReg();

    func->getParams();
    int tmp = simpleRegisterAllocator.AllocateTempInt();
    // 分配栈帧空间
    iloc.allocStack(func, tmp);
    simpleRegisterAllocator.free(tmp);
    // 保存被保护寄存器到栈
    int offset = func->getMaxDep() - 8;
    for (auto regno: protectedRegNo) {
        std::string regName = PlatformRiscV64::regName[regno];
        int         size = (regno < 32 && regno > 0) ? 8 : 4;

        if (offset >= -2048 && offset <= 2047) {
            std::string op = (regno < 32) ? "sd" : "fsw";
            iloc.inst(op, regName, std::to_string(offset) + "(sp)");
        } else {
            int tmpReg = simpleRegisterAllocator.AllocateTempInt(); // 假设 t0 可用
            iloc.inst("li", PlatformRiscV64::regName[tmpReg], std::to_string(offset));
            iloc.inst("add", PlatformRiscV64::regName[tmpReg], "sp", PlatformRiscV64::regName[tmpReg]);
            std::string op = (regno < 32) ? "sd" : "fsw";
            iloc.inst(op, regName, "0(" + PlatformRiscV64::regName[tmpReg] + ")");
            simpleRegisterAllocator.free(tmpReg); // 假设 t0 可用
        }

        offset -= size;
    }

    // 设置帧指针 s0(fp) = sp + frame_size
    if (func->getMaxDep() >= -2048 && func->getMaxDep() <= 2047) {
        iloc.inst(
            "addi",
            PlatformRiscV64::regName[RISCV64_FP_REG_NO],
            PlatformRiscV64::regName[RISCV64_SP_REG_NO],
            std::to_string(func->getMaxDep())); // fp = sp + frame_size
    } else {
        int tmpReg = simpleRegisterAllocator.AllocateTempInt();                               // 假设 t0 可用
        iloc.inst("li", PlatformRiscV64::regName[tmpReg], std::to_string(func->getMaxDep())); // 确保 t0 可用
        iloc.inst(
            "add",
            PlatformRiscV64::regName[RISCV64_FP_REG_NO],
            PlatformRiscV64::regName[RISCV64_SP_REG_NO],
            PlatformRiscV64::regName[tmpReg]); // fp = sp + frame_size
        simpleRegisterAllocator.free(tmpReg);
    }

    //
    //
    // 将传入的参数从寄存器保存到对应的栈位置（直接 sw / fsw）
    int intRegIndex = 0;
    int floatRegIndex = 0;
    for (int i = 0; i < func->getParams().size(); i++) {
        auto param = func->getParams()[i];
        auto formalValue = func->getVarValues()[i];

        int64_t offset = -1;
        int     regId = -1;
        formalValue->getMemoryAddr(&regId, &offset); // 相对于 fp 的偏移

        if (param->getType()->isFloatType()) {
            if (floatRegIndex < 8) {
                int freg = 42 + floatRegIndex; // fa0 = 42
                param->setRegId(freg);
                floatRegIndex++;
            }
        } else if (param->getType()->isInt32Type() || param->getType()->isPointerType()) {
            if (intRegIndex < 8) {
                int reg = 10 + intRegIndex; // a0 = 10
                param->setRegId(reg);
                intRegIndex++;
            }
        }
    }
}

/// @brief 函数出口指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_exit(Instruction * inst)
{
    if (inst->getOperandsNum()) {
        // 存在返回值
        Value * retVal = inst->getOperand(0);

        // 赋值给寄存器a0
        int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
        iloc.load_var(10, retVal, tmp_reg_no);
        simpleRegisterAllocator.free(tmp_reg_no);
    }
    auto & protectedRegNo = func->getProtectedReg();

    // 保护寄存器的恢复
    // 保存被保护寄存器到栈
    int offset = func->getMaxDep() - 8; // 栈偏移起点
    for (auto regno: protectedRegNo) {
        int         size = (regno >= 0 && regno < 32) ? 8 : 4;
        std::string regName = PlatformRiscV64::regName[regno];

        if (offset >= -2048 && offset <= 2047) {
            std::string op = (regno < 32) ? "ld" : "flw";
            iloc.inst(op, regName, std::to_string(offset) + "(sp)");
        } else {
            int tmpReg = simpleRegisterAllocator.AllocateTempInt(); // 确保 t0 可用或用寄存器分配器分配一个
            iloc.inst("li", PlatformRiscV64::regName[tmpReg], std::to_string(offset));
            iloc.inst("add", PlatformRiscV64::regName[tmpReg], "sp", PlatformRiscV64::regName[tmpReg]);
            std::string op = (regno < 32) ? "ld" : "flw";
            iloc.inst(op, regName, "0(" + PlatformRiscV64::regName[tmpReg] + ")");
        }

        offset -= size;
    }

    // 恢复栈空间
    if (func->getMaxDep() >= -2048 && func->getMaxDep() <= 2047) {
        iloc.inst(
            "addi",
            PlatformRiscV64::regName[RISCV64_SP_REG_NO],
            PlatformRiscV64::regName[RISCV64_SP_REG_NO],
            std::to_string(func->getMaxDep())); // sp = sp + frame_size
    } else {
        int tmpReg = simpleRegisterAllocator.AllocateTempInt();                               // 假设 t0 可用
        iloc.inst("li", PlatformRiscV64::regName[tmpReg], std::to_string(func->getMaxDep())); // 确保 t0 可用
        iloc.inst(
            "add",
            PlatformRiscV64::regName[RISCV64_SP_REG_NO],
            PlatformRiscV64::regName[RISCV64_SP_REG_NO],
            PlatformRiscV64::regName[tmpReg]); // fp = sp + frame_size
        simpleRegisterAllocator.free(tmpReg);
    }

    // 返回
    iloc.inst("jr", "ra");
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
    Instanceof(brCondInst, ConditionalInstruction *, inst);
    Value * cond = brCondInst->getOperand(0);
    Instanceof(trueTarget, LabelInstruction *, brCondInst->getOperand(1));
    Instanceof(falseTarget, LabelInstruction *, brCondInst->getOperand(2));

    int32_t cond_reg_no = cond->getRegId();
    int32_t load_cond_reg_no, tmp_reg_no;
    // 看条件变量是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (cond_reg_no == -1) {
        // 分配一个寄存器r8
        load_cond_reg_no = simpleRegisterAllocator.Allocate(cond);
        tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
        // cond -> r8，这里可能由于偏移不满足指令的要求，需要额外分配寄存器
        iloc.load_var(load_cond_reg_no, cond, tmp_reg_no);
        simpleRegisterAllocator.free(tmp_reg_no);
    } else {
        load_cond_reg_no = cond_reg_no;
    }
    // 条件寄存器r8的值为0，则跳转到假分支，否则跳转到真分支
    iloc.inst("beqz", PlatformRiscV64::regName[load_cond_reg_no], falseTarget->getName());
    iloc.inst("j", trueTarget->getName());
    // 释放寄存器
}

/// @brief 二元操作指令翻译成RISCV64汇编
/// @param inst IR指令
/// @param operator_name 操作码
/// @param rs_reg_no 结果寄存器号
/// @param op1_reg_no 源操作数1寄存器号
/// @param op2_reg_no 源操作数2寄存器号
void InstSelectorRiscV64::translate_two_operator(Instruction * inst, string operator_name)
{
    // IR: result = op nsw(?) arg1,arg2;
    // result: Instruction(必reg)
    // arg1/arg2: Instruction(必reg) / LocalVariable(可能reg) / ConstInt/Float(必非reg) /
    int32_t       arg1_reg_no = -1;
    int32_t       arg2_reg_no = -1;
    Instruction * result = inst;
    Value *       arg1 = inst->getOperand(0);
    Value *       arg2 = inst->getOperand(1);
    std::cout << "[InstSelectorRiscV64::translate_two_operator] result: " + result->getIRName() << std::endl;
    std::cout << "[InstSelectorRiscV64::translate_two_operator] arg1: " + arg1->getIRName() << std::endl;

    std::cout << "[InstSelectorRiscV64::translate_two_operator] arg2: " + arg2->getIRName() << std::endl;

    if (Instanceof(instArg1, Instruction *, arg1)) {
        arg1_reg_no = instArg1->getRegId();
    } else if (Instanceof(LVArg1, LocalVariable *, arg1)) {
        arg1_reg_no = LVArg1->getRegId();
    } else if (Instanceof(immIntArg1, ConstInt *, arg1)) {
        arg1_reg_no = immIntArg1->getRegId();
    } else if (Instanceof(immFloatArg1, ConstFloat *, arg1)) {
        arg1_reg_no = immFloatArg1->getRegId();
    } else {
        std::cout
            << "[InstSelectorRiscV64::translate_two_operator]:arg1 is not Inst / LocalVariable / ConstInt/Float\n";
    }
    if (Instanceof(instArg2, Instruction *, arg2)) {
        arg2_reg_no = instArg2->getRegId();
    } else if (Instanceof(LVArg2, LocalVariable *, arg2)) {
        arg2_reg_no = LVArg2->getRegId();
    } else if (Instanceof(immIntArg2, ConstInt *, arg2)) {
        arg2_reg_no = immIntArg2->getRegId();
    } else if (Instanceof(immFloatArg2, ConstFloat *, arg2)) {
        arg2_reg_no = immFloatArg2->getRegId();
    } else {
        std::cout
            << "[InstSelectorRiscV64::translate_two_operator]:arg2 is not Inst / LocalVariable / ConstInt/Float\n";
    }

    int32_t result_reg_no = simpleRegisterAllocator.Allocate(result);

    int32_t load_result_reg_no, load_arg1_reg_no, load_arg2_reg_no, tmp_reg_no = -1;
    // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg1_reg_no == -1) {
        tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
        // 分配一个寄存器r8
        if (arg1->getType()->isIntegerType()) {
            load_arg1_reg_no = simpleRegisterAllocator.AllocateTempInt();
        } else if (arg1->getType()->isFloatType()) {
            load_arg1_reg_no = simpleRegisterAllocator.AllocateTempFloat();
        } else {
            load_arg1_reg_no = -1;
        }
        // arg1 -> r8，这里可能由于偏移不满足指令的要求，需要额外分配寄存器
        iloc.load_var(load_arg1_reg_no, arg1, tmp_reg_no);
        simpleRegisterAllocator.free(tmp_reg_no);
    } else {
        std::cout << "arg1_reg_no:" << arg1_reg_no << endl;

        load_arg1_reg_no = arg1_reg_no;
    }

    // 看arg2是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg2_reg_no == -1) {
        std::cout << "arg2_reg_no:" << arg2_reg_no << endl;
        tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
        if (arg2->getType()->isIntegerType()) {
            load_arg2_reg_no = simpleRegisterAllocator.AllocateTempInt();
            std::cout << "load_arg2_reg_no:" << load_arg2_reg_no << endl;

        } else if (arg2->getType()->isFloatType()) {
            load_arg2_reg_no = simpleRegisterAllocator.AllocateTempFloat();
        } else {
            load_arg2_reg_no = -1;
        }
        std::cout << "load_arg2_reg_no:" << load_arg2_reg_no << endl;

        iloc.load_var(load_arg2_reg_no, arg2, tmp_reg_no);
        std::cout << "load_arg2_reg_no:" << load_arg2_reg_no << endl;

        simpleRegisterAllocator.free(tmp_reg_no);

    } else {
        load_arg2_reg_no = arg2_reg_no;
    }

    // // 看结果变量是否是寄存器，若不是则需要分配一个新的寄存器来保存运算的结果
    // if (result_reg_no == -1) {
    //     load_result_reg_no = simpleRegisterAllocator.Allocate(result);
    // } else {
    //     load_result_reg_no = result_reg_no;
    // }
    load_result_reg_no = result_reg_no;
    // r8 + r9 -> r10

    iloc.inst(
        operator_name,
        PlatformRiscV64::regName[load_result_reg_no],
        PlatformRiscV64::regName[load_arg1_reg_no],
        PlatformRiscV64::regName[load_arg2_reg_no]);

    // // 结果不是寄存器，则需要把rs_reg_name保存到结果变量中
    // if (result_reg_no == -1) {

    //     // 这里使用预留的临时寄存器，因为立即数可能过大，必须借助寄存器才可操作。
    //     // TODO:[寄存器分配]无地可放运算结果则压栈，建议给予寄存器（因为通常紧接着就store了）
    //     // r10 -> result
    //     iloc.store_var(load_result_reg_no, result, RISCV64_TMP_REG_NO);
    // }

    // 释放寄存器
    simpleRegisterAllocator.free(load_arg1_reg_no);
    simpleRegisterAllocator.free(load_arg2_reg_no);
    inst->removeOperand(0);
    inst->removeOperand(1);
    // simpleRegisterAllocator.free(result);
    std::cout << "[InstSelectorRiscV64::translate_two_operator]:end" << endl;
}

/// @brief 一元操作指令翻译成RISCV64汇编
/// @param inst IR指令
/// @param operator_name 操作码
/// @param rs_reg_no 结果寄存器号
/// @param op1_reg_no 源操作数寄存器号
void InstSelectorRiscV64::translate_one_operator(Instruction * inst, string operator_name)
{
    Instruction * result = inst;
    Value *       arg1 = inst->getOperand(0);
    int32_t       arg1_reg_no = -1;
    int32_t       result_reg_no = simpleRegisterAllocator.Allocate(result);
    std::cout << "[InstSelectorRiscV64::translate_one_operator] result: " + result->getIRName() << std::endl;
    std::cout << "[InstSelectorRiscV64::translate_one_operator] arg1: " + arg1->getIRName() << std::endl;

    if (Instanceof(instArg1, Instruction *, arg1)) {
        arg1_reg_no = instArg1->getRegId();
    } else if (Instanceof(LVArg1, LocalVariable *, arg1)) {
        arg1_reg_no = LVArg1->getRegId();
    } else if (Instanceof(immIntArg1, ConstInt *, arg1)) {
        arg1_reg_no = immIntArg1->getRegId();
    } else if (Instanceof(immFloatArg1, ConstFloat *, arg1)) {
        arg1_reg_no = immFloatArg1->getRegId();
    } else {
        std::cout
            << "[InstSelectorRiscV64::translate_two_operator]:arg1 is not Inst / LocalVariable / ConstInt/Float\n";
    }
    int32_t load_arg1_reg_no, load_result_reg_no, tmp_reg_no = -1;
    // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg1_reg_no == -1) {
        if (arg1->getType()->isIntegerType()) {
            load_arg1_reg_no = simpleRegisterAllocator.AllocateTempInt();
        } else if (arg1->getType()->isFloatType()) {
            load_arg1_reg_no = simpleRegisterAllocator.AllocateTempFloat();
            if (Instanceof(immFloatArg1, ConstFloat *, arg1)) {
                tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
                std::cout << immFloatArg1->getIRName() << "is a Float immediate, which takes one more int register.\n";
            }
        } else {
            load_arg1_reg_no = -1;
        }
        // arg1 -> r8，这里可能由于偏移不满足指令的要求，需要额外分配寄存器
        iloc.load_var(load_arg1_reg_no, arg1, tmp_reg_no);
        simpleRegisterAllocator.free(tmp_reg_no);
    } else {
        load_arg1_reg_no = arg1_reg_no;
    }
    load_result_reg_no = result_reg_no;
    // // 看结果变量是否是寄存器，若不是则需要分配一个新的寄存器来保存运算的结果
    // if (result_reg_no == -1) {
    //     // 分配一个寄存器r10，用于暂存结果
    //     load_result_reg_no = simpleRegisterAllocator.Allocate(result);
    // } else {
    //     load_result_reg_no = result_reg_no;
    // }

    // 根据操作名称生成相应的一元运算指令
    iloc.inst(operator_name, PlatformRiscV64::regName[load_result_reg_no], PlatformRiscV64::regName[load_arg1_reg_no]);

    // // 结果不是寄存器，则需要把rs_reg_name保存到结果变量中
    // if (result_reg_no == -1) {
    //     // 这里使用预留的临时寄存器，因为立即数可能过大，必须借助寄存器才可操作。
    // 	//TODO:[寄存器分配]无地可放运算结果则压栈，建议给予寄存器（因为通常紧接着就store了）
    //     // r10 -> result
    //     iloc.store_var(load_result_reg_no, result, RISCV64_TMP_REG_NO);
    // }

    // 释放寄存器
    simpleRegisterAllocator.free(load_arg1_reg_no);
    // simpleRegisterAllocator.free(result);
    inst->removeOperand(0);
}

/// @brief 整数加法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_add_int32(Instruction * inst)
{
    translate_two_operator(inst, "addw");
}

/// @brief 整数减法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_sub_int32(Instruction * inst)
{
    translate_two_operator(inst, "subw");
}

/// @brief 整数乘法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_mul_int32(Instruction * inst)
{
    translate_two_operator(inst, "mulw");
}

/// @brief 整数除法指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_div_int32(Instruction * inst)
{
    translate_two_operator(inst, "divw");
}

/// @brief 整数取模指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_mod_int32(Instruction * inst)
{
    translate_two_operator(inst, "remw");
}

/// @brief 整数相等指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_eq_int32(Instruction * inst)
{
    // 获取两个操作数
    Value * lhs = inst->getOperand(0);
    Value * rhs = inst->getOperand(1);
    auto    subInst = BinaryInstruction::createAutoTyped(
        func,
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_SUB_I,
        IRInstOperator::IRINST_OP_SUB_F);
    // sub temp, lhs, rhs
    translate_sub_int32(subInst);

    // delete subInst;
    // seqz result, temp
    inst->clearOperands();
    inst->addOperand(subInst);
    translate_one_operator(inst, "seqz");
}

/// @brief 整数不等指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_neq_int32(Instruction * inst)
{
    // 获取两个操作数
    std::cout << "[InstSelectorRiscV64::translate_neq_int32]1" << endl;
    Value * lhs = inst->getOperand(0);
    std::cout << "[InstSelectorRiscV64::translate_neq_int32]lhs: " << lhs << endl;

    Value * rhs = inst->getOperand(1);
    std::cout << "[InstSelectorRiscV64::translate_neq_int32]rhs: " << rhs << endl;

    auto subInst = BinaryInstruction::createAutoTyped(
        func,
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_SUB_I,
        IRInstOperator::IRINST_OP_SUB_F);
    // sub temp, lhs, rhs

    translate_sub_int32(subInst);

    // delete subInst;
    // seqz result, temp
    inst->clearOperands();
    inst->addOperand(subInst);
    translate_one_operator(inst, "snez");
}

/// @brief 整数小于等于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_le_int32(Instruction * inst)
{
    // 获取两个操作数
    Value * lhs = inst->getOperand(0); // a
    Value * rhs = inst->getOperand(1); // b

    // 创建 slt 临时指令：t = b < a
    auto sltInst = BinaryInstruction::createAutoTyped(
        func,
        rhs,
        lhs,
        IRInstOperator::IRINST_OP_LNE_I,
        IRInstOperator::IRINST_OP_LNE_F); // 注意顺序是 b < a

    // 执行 slt
    translate_lne_int32(sltInst); // 你需要已有 translate_lt_int32()

    // 替换 inst 的操作数为 sltInst 的结果
    inst->clearOperands();
    inst->addOperand(sltInst);

    // 执行 seqz：result = !(b < a) ==> a <= b
    translate_one_operator(inst, "seqz");
}

/// @brief 整数大于等于指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_ge_int32(Instruction * inst)
{
    // 获取两个操作数
    Value * lhs = inst->getOperand(0); // a
    Value * rhs = inst->getOperand(1); // b

    // 创建 slt 临时指令：t = b < a
    auto sltInst = BinaryInstruction::createAutoTyped(
        func,
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_LNE_I,
        IRInstOperator::IRINST_OP_LNE_F); // 注意顺序是 b < a

    // 执行 slt
    translate_lne_int32(sltInst); // 你需要已有 translate_lt_int32()

    // 替换 inst 的操作数为 sltInst 的结果
    inst->clearOperands();
    inst->addOperand(sltInst);

    // 执行 seqz：result = !(b < a) ==> a <= b
    translate_one_operator(inst, "seqz");
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
    Value * arg1 = inst->getOperand(0);
    Value * arg2 = inst->getOperand(1);
    inst->clearOperands();
    inst->addOperand(arg2);
    inst->addOperand(arg1);
    translate_two_operator(inst, "slt");
}

/// @brief 整数取正指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_pos_int32(Instruction * inst)
{
    // 可以优化不需要操作
    translate_nop(inst);
}

/// @brief 整数取负指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_neg_int32(Instruction * inst)
{
    // FIXME: [指令指派补充] 需要处理立即数溢出问题
    translate_two_operator(inst, "subw");
}

/// @brief 整数逻辑非指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_not_int32(Instruction * inst)
{
    // 逻辑非操作，直接翻译成seqz指令
    // 即如果结果为0，则返回1，否则返回0
    translate_one_operator(inst, "seqz");
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
    translate_nop(inst);
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
        iloc.store_var(arg1_regId, result, 5);
    } else if (result_regId != -1) {
        // 内存变量 => 寄存器
        int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
        iloc.load_var(result_regId, arg1, tmp_reg_no);
        simpleRegisterAllocator.free(tmp_reg_no);
    } else {
        // 内存变量 => 内存变量

        int32_t temp_regno = simpleRegisterAllocator.AllocateTempInt();
        int     tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
        // arg1 -> r8
        iloc.load_var(temp_regno, arg1, tmp_reg_no);

        // r8 -> rs 可能用到r9
        iloc.store_var(temp_regno, result, tmp_reg_no);
        simpleRegisterAllocator.free(tmp_reg_no);
        simpleRegisterAllocator.free(temp_regno);
    }
}

/// @brief 函数调用指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_call(Instruction * inst)
{
    FuncCallInstruction * callInst = dynamic_cast<FuncCallInstruction *>(inst);
    std::cout << "[InstSelectorRiscV64::translate_call]: call " << callInst->getName() << "() translate begins\n";
    int32_t operandNum = callInst->getOperandsNum();
    std::cout << "[InstSelectorRiscV64::translate_call]: argument NUM: " << operandNum << "\n";
    // if (operandNum != realArgCount) {
    //     // 两者不一致 也可能没有ARG指令，正常
    //     if (realArgCount != 0) {

    //         minic_log(LOG_ERROR, "ARG指令的个数与调用函数个数不一致");
    //     }
    // }
    int intIndex = 0;   // 对应 a0–a7（x10–x17）
    int floatIndex = 0; // 对应 fa0–fa7（f10–f17）
    if (operandNum) {
        for (uint32_t k = 0; k < operandNum; k++) {
            auto arg = callInst->getOperand(k);

            int value_reg_id = arg->getRegId();
            if (Instanceof(GEP, GetElementPtrInst *, arg)) {
                GEP->getBaseRegId();
                int tmp_reg_no = simpleRegisterAllocator.AllocateTempIntToTempReg();
                iloc.mov_reg(tmp_reg_no, GEP->getBaseRegId());
                GEP->setAddressingInfo(tmp_reg_no, GEP->getOffset());
            } else if (value_reg_id != -1) {
                std::cout << "operandNum:" << arg->getIRName() << "\t" << value_reg_id << endl;
                int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
                simpleRegisterAllocator.free(arg);
                iloc.store_var(value_reg_id, arg, tmp_reg_no);
                simpleRegisterAllocator.free(tmp_reg_no);
            }
        }

        for (Value * value: simpleRegisterAllocator.getIntRegValues()) {
            int value_reg_id = value->getRegId();
            if (value_reg_id >= 10 && value_reg_id <= 17) {
                std::cout << "IntRegValues():" << value->getIRName() << "\t" << value_reg_id << endl;
                if (Instanceof(GEP, GetElementPtrInst *, value)) {
                    GEP->getBaseRegId();
                } else {
                    int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
                    simpleRegisterAllocator.free(value);
                    iloc.store_var(value_reg_id, value, tmp_reg_no);
                    simpleRegisterAllocator.free(tmp_reg_no);
                }
            }
        }
        for (Value * value: simpleRegisterAllocator.getFloatRegValues()) {
            int value_reg_id = value->getRegId();
            if (value_reg_id >= 42 && value_reg_id <= 49) {
                if (Instanceof(GEP, GetElementPtrInst *, value)) {
                    GEP->getBaseRegId();
                } else {
                    int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
                    simpleRegisterAllocator.free(value);
                    iloc.store_var(value_reg_id, value, tmp_reg_no);
                    simpleRegisterAllocator.free(tmp_reg_no);
                }
            }
        }
        // 前八个的后面参数采用栈传递
        int esp = 0;
        // for (uint32_t k = 0; k < operandNum; k++) {
        //     auto arg = callInst->getOperand(k);

        //     int value_reg_id = arg->getRegId();

        //     if (value_reg_id != -1) {
        //         std::cout << "operandNum:" << arg->getIRName() << "\t" << value_reg_id << endl;
        //         int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
        //         simpleRegisterAllocator.free(arg);
        //         iloc.store_var(value_reg_id, arg, tmp_reg_no);
        //         simpleRegisterAllocator.free(tmp_reg_no);
        //     }
        // }
        for (uint32_t k = 0; k < operandNum; k++) {
            auto arg = callInst->getOperand(k);

            if (arg->getType()->isFloatType()) {
                if (floatIndex < 8) {
                    int float_reg_no = 42 + floatIndex; // 42号为fa0
                                                        // TODO:[优化]在非全栈模式时需要对被占用的栈保护寄存器压栈
                    // if(PlatformRiscV64::floatRegVal[float_reg_no]->)
                    simpleRegisterAllocator.Allocate(float_reg_no);
                    int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
                    std::cout << "[InstSelectorRiscV64::translate_call]: call " << callInst->getName()
                              << "(): float arg " << floatIndex << " allocating, arg name:" << arg->getName() << "\n";
                    iloc.load_var(float_reg_no, arg, tmp_reg_no);
                    simpleRegisterAllocator.free(tmp_reg_no);
                    floatIndex++;
                } else {
                    // 栈上传递浮点数参数
                    MemVariable * newVal = func->newMemVariable((Type *) PointerType::get(arg->getType()));
                    newVal->setMemoryAddr(RISCV64_SP_REG_NO, esp);
                    esp += 4; // 浮点数 4 字节
                    Instruction * storeInst = new StoreInstruction(func, newVal, arg);
                    translate_store(storeInst);
                    delete storeInst;
                    // floatIndex++;
                }
            } else if (arg->getType()->isInt32Type()) {
                if (intIndex < 8) {
                    int int_reg_no = 10 + intIndex; // 10号为a0
                    std::cout << arg->getRegId() << endl;

                    simpleRegisterAllocator.Allocate(int_reg_no);
                    int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
                    std::cout << "[InstSelectorRiscV64::translate_call]: call " << callInst->getName() << "(): int arg "
                              << intIndex << " allocating, arg name:" << arg->getName() << "\n";
                    iloc.load_var(int_reg_no, arg, tmp_reg_no);
                    simpleRegisterAllocator.free(tmp_reg_no);
                    intIndex++;
                } else {
                    // 栈上传递整数参数
                    MemVariable * newVal = func->newMemVariable((Type *) PointerType::get(arg->getType()));
                    newVal->setMemoryAddr(RISCV64_SP_REG_NO, esp);
                    esp += 4;
                    Instruction * storeInst = new StoreInstruction(func, newVal, arg);
                    translate_store(storeInst);
                    delete storeInst;
                    // intIndex++;
                }
            } else if (arg->getType()->isPointerType()) {
                // TODO:[数组实参]完善这种情况
                std::cout << "尚未完成数组作为实参的函数调用\n";
                if (intIndex < 8) {
                    int int_reg_no = 10 + intIndex;
                    simpleRegisterAllocator.Allocate(int_reg_no);
                    std::cout << "尚未完成数组作为实参的函数调用1\n";
                    if (Instanceof(GEParg, GetElementPtrInst *, arg)) {
                        iloc.lea_var(int_reg_no, GEParg);
                    } else if (Instanceof(LOADarg, LoadInstruction *, arg)) {
                        int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
                        iloc.load_var(int_reg_no, LOADarg, tmp_reg_no);
                        simpleRegisterAllocator.free(tmp_reg_no);
                    }
                    // 关键：加载指针的地址

                    intIndex++;
                } else {
                    // 栈上传递指针
                    const Type *  ptrType = PointerType::get(arg->getType());
                    MemVariable * newVal = func->newMemVariable(const_cast<Type *>(ptrType));
                    newVal->setMemoryAddr(RISCV64_SP_REG_NO, esp);
                    esp += 8; // 指针默认按 8 字节处理
                    Instruction * storeInst = new StoreInstruction(func, newVal, arg);
                    translate_store(storeInst);
                    delete storeInst;
                }
            } else {
                std::cout << "函数调用参数非Array/Int/Float";
            }
        }
    }
    std::cout << "[InstSelectorRiscV64::translate_call]: finish alloc arguments"
              << "\n";
    iloc.call_fun(callInst->getName());
    std::cout << "[InstSelectorRiscV64::translate_call]: call translated"
              << "\n";
    for (int i = 1; i < intIndex; ++i) {
        simpleRegisterAllocator.free(10 + i); //
    }
    for (int i = 1; i < floatIndex; ++i) {
        simpleRegisterAllocator.free(42 + i); //
    }
    // 赋值指令
    if (callInst->hasResultValue()) {
        if (callInst->getType()->isFloatType()) {
            int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
            iloc.store_var(42, callInst, tmp_reg_no);
            simpleRegisterAllocator.free(tmp_reg_no);
        } else if (callInst->getType()->isInt32Type()) {
            int tmp_reg_no = simpleRegisterAllocator.AllocateTempInt();
            iloc.store_var(10, callInst, tmp_reg_no);
            simpleRegisterAllocator.free(tmp_reg_no);
            int     regid = -1;
            int64_t offset = -1;
            callInst->getMemoryAddr(&regid, &offset);
            std::cout << "[InstSelectorRiscV64::translate_call]: call " << callInst->getName() << "(): return int "
                      << callInst->getIRName() << " storing, target mem regid+offset: " << regid << "+" << offset
                      << "\n";

        } else {
            std::cout << "调用函数实际返回值类型不是int32/float\n";
        }
    }
    simpleRegisterAllocator.free(10);
    simpleRegisterAllocator.free(42);

    callInst->clearOperands();
    std::cout << "[InstSelectorRiscV64::translate_call]: call " << callInst->getName() << " end.\n";
    // 函数调用后清零，使得下次可正常统计
    // realArgCount = 0;
}

/// @brief 实参指令翻译成RISCV64汇编
/// @param inst
// TODO:考虑是否弃用
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
        std::cout << "[InstSelectorRiscV64::translate_store] src is ConstInt, value= " << ConstIntSrc->getVal() << "\n";
        if (Instanceof(LVDst, LocalVariable *, dst)) {
            dst_regId = LVDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is LocalVariable, regid=" << dst_regId << "\t"
                      << LVDst->getIRName() << "\n";
        } else if (Instanceof(GLDst, GlobalVariable *, dst)) {
            dst_regId = GLDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is GlobalVariable, regid=" << dst_regId << "\t"
                      << GLDst->getIRName() << "\n";
        } // 这里dst_regId是寄存器号，dst是内存变量
        else if (Instanceof(GEPDst, GetElementPtrInst *, dst)) {
            dst_regId = GEPDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is GetElementPtrInst, regid=" << dst_regId << "\t"
                      << GEPDst->getIRName() << "\n";
        } else {
            std::cout << "[InstSelectorRiscV64::translate_store] dst is not a "
                         "GetElementPtrInst/GlobalVariable/LocalVariable\n";
            return;
        }
        if (dst_regId != -1) {
            if (Instanceof(GEPDst, GetElementPtrInst *, dst)) {
                int32_t data_regno = simpleRegisterAllocator.AllocateTempInt();
                iloc.load_imm(data_regno, ConstIntSrc->getVal());

                iloc.store_var(data_regno, GEPDst, dst_regId);

                simpleRegisterAllocator.free(data_regno);
            } else {
                iloc.load_imm(dst_regId, ConstIntSrc->getVal());
            }
        } else {
            int32_t addr_regno = simpleRegisterAllocator.AllocateTempInt();
            int32_t data_regno = simpleRegisterAllocator.AllocateTempInt();
            std::cout << data_regno << endl;
            std::cout << addr_regno << endl;

            iloc.load_imm(data_regno, ConstIntSrc->getVal());

            iloc.store_var(data_regno, dst, addr_regno);

            simpleRegisterAllocator.free(data_regno);

            simpleRegisterAllocator.free(addr_regno);
        }
    } else if (Instanceof(ConstFloatSrc, ConstFloat *, src)) {
        // 源操作数是立即数
        // FIXME: 目前只支持整数
        std::cout << "[InstSelectorRiscV64::translate_store] src is ConstFloat, originValue=" << ConstFloatSrc->getVal()
                  << ", unionIntValue=" << float2int(ConstFloatSrc->getVal()) << "\n";
        if (Instanceof(LVDst, LocalVariable *, dst)) {
            dst_regId = LVDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is LocalVariable, regid=" << dst_regId << "\t"
                      << LVDst->getIRName() << "\n";
        } else if (Instanceof(GLDst, GlobalVariable *, dst)) {
            dst_regId = GLDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is GlobalVariable, regid=" << dst_regId << "\t"
                      << LVDst->getIRName() << "\n";
        } // 这里dst_regId是寄存器号，dst是内存变量
        else if (Instanceof(GEPDst, GetElementPtrInst *, dst)) {
            dst_regId = GEPDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is GetElementPtrInst, regid=" << dst_regId << "\t"
                      << GEPDst->getIRName() << "\n";
        } else {
            std::cout << "[InstSelectorRiscV64::translate_store] dst is not a "
                         "GetElementPtrInst/GlobalVariable/LocalVariable\n";
            return;
        }
        if (dst_regId != -1) {
            if (Instanceof(GEPDst, GetElementPtrInst *, dst)) {
                int32_t data_regno = simpleRegisterAllocator.AllocateTempFloat();

                iloc.load_imm(data_regno, ConstIntSrc->getVal());

                iloc.store_var(data_regno, GEPDst, dst_regId);

                simpleRegisterAllocator.free(data_regno);
            } else {
                int32_t tmp_regno = simpleRegisterAllocator.AllocateTempInt();
                iloc.load_imm(dst_regId, ConstFloatSrc->getVal(), tmp_regno);
                simpleRegisterAllocator.free(tmp_regno);
            }
        } else {
            int32_t tmp_regno = simpleRegisterAllocator.AllocateTempInt();
            int32_t data_regno = simpleRegisterAllocator.AllocateTempFloat();
            iloc.load_imm(data_regno, ConstFloatSrc->getVal(), tmp_regno);
            iloc.store_var(data_regno, dst, tmp_regno);
            simpleRegisterAllocator.free(data_regno);
            simpleRegisterAllocator.free(tmp_regno);
        }
    } else if (Instanceof(ParamSrc, FormalParam *, src)) {
        src_regId = ParamSrc->getRegId();
        std::cout << "[InstSelectorRiscV64::translate_store] src is FormalParam, regid=" << src_regId << "\n";
        if (Instanceof(LVDst, LocalVariable *, dst)) {
            if (src_regId != -1) {
                iloc.store_var(src_regId, LVDst);
            } else {
                std::cout << "[InstSelectorRiscV64::translate_store]:尚未实现栈传参\n";
            }
        } else {
            std::cout << "[InstSelectorRiscV64::translate_store]:形参并未store进变量\n";
        }

    } else {
        // 源变量是Instruction临时变量或Local局部变量的情况
        if (Instanceof(InstSrc, Instruction *, src)) {
            src_regId = InstSrc->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] src is Instruction, regid=" << src_regId << "\t"
                      << InstSrc->getIRName() << "\n";
        } else if (Instanceof(LVSrc, LocalVariable *, src)) {
            src_regId = LVSrc->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] src is LocalVariable, regid=" << src_regId << "\t"
                      << InstSrc->getIRName() << "\n";
        } else {
            std::cout << "[InstSelectorRiscV64::translate_store] src is not a Instruction/Local variable\n";
            return;
        }
        if (Instanceof(LVDst, LocalVariable *, dst)) {
            // 目的变量是局部变量，不需要额外分配指针寄存器
            dst_regId = LVDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is LocalVariable, regid=" << dst_regId << "\t"
                      << LVDst->getIRName() << "\n";
        } else if (Instanceof(GLDst, GlobalVariable *, dst)) {
            // 目的变量是全局变量，需要分配指针寄存器
            dst_regId = GLDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is GlobalVariable, regid=" << dst_regId << "\t"
                      << GLDst->getIRName() << "\n";
        } else if (Instanceof(GEPDst, GetElementPtrInst *, dst)) {
            dst_regId = GEPDst->getRegId();
            std::cout << "[InstSelectorRiscV64::translate_store] dst is GetElementPtrInst, regid=" << dst_regId << "\t"
                      << GEPDst->getIRName() << "\n";
        } else {
            std::cout << "[InstSelectorRiscV64::translate_store] dst is not a "
                         "GetElementPtrInst/GlobalVariable/LocalVariable\n";
            return;
        }
        int32_t addr_regno = simpleRegisterAllocator.AllocateTempInt();
        if (src_regId != -1) {
            // 源操作数是寄存器，则直接存储到寄存器中
            iloc.store_var(src_regId, dst, addr_regno); // XXX: 考虑修改函数，看是否需要额外指派地址寄存器
        } else {
            // 源操作数是内存变量，则需要先load到寄存器中
            int32_t data_regno = -1;
            if (src->getType()->isIntegerType()) {
                std::cout << 1 << endl;
                data_regno = simpleRegisterAllocator.AllocateTempInt(); // FIXME:考虑溢出情况

            } else if (src->getType()->isFloatType()) {
                data_regno = simpleRegisterAllocator.AllocateTempFloat();
            }
            // t0 <- src
            iloc.load_var(data_regno, src, addr_regno);
            // t0 -> dst
            iloc.store_var(data_regno, dst, addr_regno);
            simpleRegisterAllocator.free(data_regno);
        }
        simpleRegisterAllocator.free(addr_regno);
    }
    inst->removeOperand(0);
    inst->removeOperand(1);
}

/// @brief 加载指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_load(Instruction * inst)
{
    // IR dst:result=load (type) src:operand[0]
    // 可能的情况分析：dst:Instruction(必定reg)src:GlobalVariable(必定mem),LocalVariable(可能reg)
    int32_t       src_regId = -1;
    int32_t       dst_regId = -1;
    Value *       src = inst->getOperand(0);
    Instruction * dst = inst;
    std::cout << "[InstSelectorRiscV64::translate_load] srctype:" << typeid(inst->getOperand(1)).name() << "\n"
              << "[InstSelectorRiscV64::translate_load] dsttype:" << typeid(inst->getOperand(0)).name() << "\n";

    // 源变量是Instruction临时变量或Local局部变量的情况
    if (Instanceof(InstSrc, Instruction *, src)) {
        src_regId = InstSrc->getRegId();
        std::cout << "[InstSelectorRiscV64::translate_load] src is Instruction, regid=" << src_regId << "\t"
                  << InstSrc->getIRName() << "\n";
    } else if (Instanceof(LVSrc, LocalVariable *, src)) {
        src_regId = LVSrc->getRegId();
        std::cout << "[InstSelectorRiscV64::translate_load] src is LocalVariable, regid=" << src_regId << "\t"
                  << LVSrc->getIRName() << "\n";
    } else if (Instanceof(GLSrc, GlobalVariable *, src)) {
        src_regId = GLSrc->getRegId();
        std::cout << "[InstSelectorRiscV64::translate_load] src is GlobalVariable, regid=" << src_regId << "\t"
                  << GLSrc->getIRName() << "\n";
    } else {
        std::cout << "[InstSelectorRiscV64::translate_load] src is not a Instruction/Local variable\n";
        return;
    }

    // 目的变量是局部变量，不需要额外分配指针寄存器
    dst_regId = simpleRegisterAllocator.Allocate(dst);
    std::cout << "[InstSelectorRiscV64::translate_load] dst is Instruction, regid=" << dst_regId << "\t"
              << dst->getIRName() << "\n";
    if (src_regId != -1) {
        if (Instanceof(GEPDst, GetElementPtrInst *, src)) {
            int32_t addr_regno = simpleRegisterAllocator.AllocateTempInt();
            //  data_reg<- src
            iloc.load_var(dst_regId, GEPDst, addr_regno);
            iloc.store_var(dst_regId, dst, addr_regno);
            simpleRegisterAllocator.free(addr_regno);

        } else {
            // 源操作数是寄存器，则直接存储到寄存器中
            // iloc.mov_reg(dst_regId, src_regId); // XXX: 考虑修改函数，看是否需要额外指派地址寄存器
            iloc.store_var(src_regId, dst, -1);
        }
    } else {
        // 源操作数是内存变量，则需要先load到寄存器中 // FIXME:考虑溢出情况
        int32_t addr_regno = simpleRegisterAllocator.AllocateTempInt();
        //  data_reg<- src
        iloc.load_var(dst_regId, src, addr_regno);
        simpleRegisterAllocator.free(addr_regno);
    }
    inst->removeOperand(0);
}

/// @brief Cast指令翻译成RISCV64汇编
/// @param inst IR指令
void InstSelectorRiscV64::translate_cast(Instruction * inst)
{
    // TODO: @JEV055 [指令指派] 需要实现类型转换
    Value * src = inst->getOperand(0);
    Type *  srcType = src->getType();
    Type *  dstType = inst->getType();
    if (srcType->isInt1Byte() && dstType->isInt32Type()) {
        // zext i1 → i32，委托给已有的 zext 处理逻辑
        int dstReg = simpleRegisterAllocator.Allocate(inst);
        int srcReg = simpleRegisterAllocator.Allocate(src);
        iloc.inst("andi", PlatformRiscV64::regName[dstReg], PlatformRiscV64::regName[srcReg], "1");
    } else if (
        (srcType->getTypeID() == Type::IntegerTyID && dstType->getTypeID() == Type::FloatTyID) ||
        (srcType->getTypeID() == Type::IntegerTyID && dstType->getPointeeType()->getTypeID() == Type::FloatTyID)) {
        translate_one_operator(inst, "fcvt.s.w");
    } else if (
        (srcType->getTypeID() == Type::FloatTyID && dstType->getTypeID() == Type::IntegerTyID) ||
        (srcType->getTypeID() == Type::FloatTyID && dstType->getPointeeType()->getTypeID() == Type::IntegerTyID)) {
        // float → i32
        translate_one_operator(inst, "fcvt.w.s");
    } else if (srcType->isInt32Type() && dstType->isInt1Byte()) {
        // i32 → i1（截断）
        int dstFReg = simpleRegisterAllocator.Allocate(inst);
        int srcReg = simpleRegisterAllocator.Allocate(src);
        iloc.inst(
            "sltu",
            PlatformRiscV64::regName[dstFReg],
            PlatformRiscV64::regName[0],
            PlatformRiscV64::regName[srcReg]);
    } else if (srcType->isInt32Type() && dstType->isInt64Type()) {
        // // i32->i64
        // int dstReg = simpleRegisterAllocator.Allocate(inst);
        // int srcReg = simpleRegisterAllocator.Allocate(src);
        // iloc.inst("addi", PlatformRiscV64::regName[dstReg], PlatformRiscV64::regName[srcReg], "0");
        // FIXME: 对地址的处理是否要显示
    } else {
        std::cerr << "[ERROR] Unsupported cast: " << srcType->toString() << " → " << dstType->toString() << std::endl;
    }
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
    // std::cout << "translate_gep:Start" << endl;
    Instanceof(gepInst, GetElementPtrInst *, inst);
    Value * base = gepInst->getOperand(0);
    int     index = 0; // 数组偏移
    bool    isConst = true;
    int     OperandNum = gepInst->getOperandsNum();
    // Value * index0 = inst->getOperand(1); // 通常是常量 0
    if (Instanceof(index1, ConstInt *, gepInst->getOperand(OperandNum - 1))) {
        index = index1->getVal();
    } else if (Instanceof(index1, Instruction *, gepInst->getOperand(OperandNum - 1))) {
        if (Instanceof(index1_Operand, ConstInt *, index1->getOperand(0))) {
            index = index1_Operand->getVal();
        } else {
            isConst = false;
        }
    } else {
        std::cout << "[InstSelectorRiscV64::translate_gep] src is error\n";
    }

    // ---------- Step 1: 处理 base 地址 ----------
    // std::cout << "translate_gep:处理base地址" << endl;

    if (Instanceof(base_s0, LocalVariable *, base)) {
        // std::cout << "translate_gep:base is LocalVariable" << endl;

        int32_t baseRegId = -1;
        int64_t baseOffset = -1;
        bool    result = base_s0->getMemoryAddr(&baseRegId, &baseOffset);
        if (!result) {
            minic_log(LOG_ERROR, "BUG");
        }
        // ---------- Step 2: 处理偏移量 index1 ----------
        int elementSize = base->getType()->getElementType()->getSize(); // 比如 i32 -> 4
        std::cout << " elementSize: " << elementSize << endl;
        int offset = 0;
        offset = (index * elementSize) + baseOffset;
        // std::cout << "index:" << index << endl;
        // std::cout << "baseOffset:" << baseOffset << endl;
        // std::cout << "offset:" << offset << endl;
        if (!isConst) {
            Instanceof(index1, Instruction *, gepInst->getOperand(OperandNum - 1));
            int index1OperandRegId = simpleRegisterAllocator.Allocate(index1->getOperand(0));
            int resultRegId = simpleRegisterAllocator.Allocate(gepInst); // 假设是 a0，编号为 10
            iloc.inst("li", PlatformRiscV64::regName[resultRegId], to_string(elementSize));
            iloc.inst(
                "mul",
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[resultRegId]);
            iloc.inst(
                "add",
                PlatformRiscV64::regName[resultRegId],
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[baseRegId]);
            gepInst->setAddressingInfo(resultRegId, offset);
            simpleRegisterAllocator.free(index1OperandRegId);
        } else {
            gepInst->setAddressingInfo(baseRegId, offset);
        }
    } else if (Instanceof(base_gv, GlobalVariable *, base)) {
        // std::cout << "translate_gep:base is GlobalVariable" << endl;

        // 目标寄存器：假设你用 getRegId 获取目标寄存器编号
        int         resultRegId = simpleRegisterAllocator.Allocate(gepInst);
        int         tmpRegId = simpleRegisterAllocator.AllocateTempInt(); // 假设是 a0，编号为 10
        std::string label = base_gv->getName();
        int         elementSize = base->getType()->getElementType()->getSize(); // 比如 i32 -> 4
        // 比如 "a"
        // 输出 lui a0, %hi(a)
        iloc.inst("lui", PlatformRiscV64::regName[tmpRegId], "%hi(" + label + ")", "");

        // 输出 addi a0, a0, %lo(a)
        iloc.inst(
            "addi",
            PlatformRiscV64::regName[resultRegId],
            PlatformRiscV64::regName[tmpRegId],
            "%lo(" + label + ")");
        if (!isConst) {
            Instanceof(index1, Instruction *, gepInst->getOperand(OperandNum - 1));
            int index1OperandRegId = simpleRegisterAllocator.Allocate(index1->getOperand(0));
            iloc.inst("li", PlatformRiscV64::regName[tmpRegId], to_string(elementSize));

            iloc.inst(
                "mul",
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[tmpRegId]);
            iloc.inst(
                "add",
                PlatformRiscV64::regName[resultRegId],
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[resultRegId]);
            simpleRegisterAllocator.free(index1OperandRegId);
        }
        int offset = 0;
        offset = (index * elementSize);
        gepInst->setAddressingInfo(resultRegId, offset);
        simpleRegisterAllocator.free(tmpRegId);
    } else if (Instanceof(base_gep, GetElementPtrInst *, base)) {
        // std::cout << "translate_gep:base is GetElementPtrInst" << endl;

        int32_t baseRegId = base_gep->getBaseRegId();
        // std::cout << "translate_gep:baseRegId is " << baseRegId << endl;

        int64_t baseOffset = base_gep->getOffset();
        // std::cout << "translate_gep:baseOffset is " << baseOffset << endl;

        // ---------- Step 2: 处理偏移量 index1 ----------
        int elementSize = base->getType()->getPointeeType()->getElementType()->getSize(); // 比如 i32 -> 4
        // std::cout << "translate_gep:elementSize is " << elementSize << endl;

        int offset = 0;

        offset = (index * elementSize) + baseOffset;
        if (!isConst) {
            Instanceof(index1, Instruction *, gepInst->getOperand(OperandNum - 1));
            int index1OperandRegId = simpleRegisterAllocator.Allocate(index1->getOperand(0));
            int resultRegId = simpleRegisterAllocator.Allocate(gepInst); // 假设是 a0，编号为 10
            iloc.inst("li", PlatformRiscV64::regName[resultRegId], to_string(elementSize));
            iloc.inst(
                "mul",
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[resultRegId]);
            iloc.inst(
                "add",
                PlatformRiscV64::regName[resultRegId],
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[baseRegId]);
            gepInst->setAddressingInfo(resultRegId, offset);
            simpleRegisterAllocator.free(index1OperandRegId);
        } else {
            gepInst->setAddressingInfo(baseRegId, offset);
        }
        // std::cout << "index:" << index << endl;
        // std::cout << "baseOffset:" << baseOffset << endl;
        // std::cout << "offset:" << offset << endl;
    } else if (Instanceof(base_load, LoadInstruction *, base)) {
        // std::cout << "translate_gep:base is LoadInstruction" << endl;

        int32_t baseRegId = simpleRegisterAllocator.Allocate(base_load);

        // ---------- Step 2: 处理偏移量 index1 ----------

        int elementSize = base_load->getType()->getPointeeType()->getSize(); // 比如 i32 -> 4
        // std::cout << "elementSize:" << elementSize << endl;

        int offset = 0;
        offset = (index * elementSize);
        // std::cout << "index:" << index << endl;
        // std::cout << "offset:" << offset << endl;
        if (!isConst) {
            Instanceof(index1, Instruction *, gepInst->getOperand(OperandNum - 1));
            int index1OperandRegId = simpleRegisterAllocator.Allocate(index1->getOperand(0));
            int resultRegId = simpleRegisterAllocator.Allocate(gepInst); // 假设是 a0，编号为 10
            iloc.inst("li", PlatformRiscV64::regName[resultRegId], to_string(elementSize));
            iloc.inst(
                "mul",
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[resultRegId]);
            iloc.inst(
                "add",
                PlatformRiscV64::regName[resultRegId],
                PlatformRiscV64::regName[index1OperandRegId],
                PlatformRiscV64::regName[baseRegId]);
            gepInst->setAddressingInfo(resultRegId, offset);
            simpleRegisterAllocator.free(index1OperandRegId);
        } else {
            gepInst->setAddressingInfo(baseRegId, offset);
        }
    } else {
        std::cout << "[InstSelectorRiscV64::translate_gep] src is not a Global/Local variable\n";
    }
    // std::cout << "translate_gep:end" << endl;
}
