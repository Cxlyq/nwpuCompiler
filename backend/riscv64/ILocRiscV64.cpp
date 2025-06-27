///
/// @file ILocRiscV64.cpp
/// @brief 指令序列管理的实现，ILOC的全称为Intermediate Language for Optimizing Compilers
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
#include <string>

#include "ILocRiscV64.h"
#include "Common.h"
#include "Function.h"
#include "GlobalVariable.h"
#include "LocalVariable.h"
#include "PlatformRiscV64.h"
#include "Module.h"
#include "GetElementPtrInst.h"
RiscInst::RiscInst(
    std::string _opcode, std::string _result, std::string _arg1, std::string _arg2, std::string _cond,
    std::string _addition)
    : opcode(_opcode), cond(_cond), result(_result), arg1(_arg1), arg2(_arg2), addition(_addition), dead(false)
{}

/*
    指令内容替换
*/
void RiscInst::replace(
    std::string _opcode, std::string _result, std::string _arg1, std::string _arg2, std::string _cond,
    std::string _addition)
{
    opcode = _opcode;
    result = _result;
    arg1 = _arg1;
    arg2 = _arg2;
    cond = _cond;
    addition = _addition;

#if 0
    // 空操作，则设置为dead
    if (op == "") {
        dead = true;
    }
#endif
}

/*
    设置为无效指令
*/
void RiscInst::setDead()
{
    dead = true;
}

/*
    输出函数
*/
std::string RiscInst::outPut()
{
    // 无用代码，什么都不输出
    if (dead) {
        return "";
    }

    // 占位指令,可能需要输出一个空操作，看是否支持
    // FIXME
    if (opcode.empty()) {
        return "";
    }

    std::string ret = opcode;

    if (!cond.empty()) {
        ret += cond;
    }

    // 结果输出
    if (!result.empty()) {
        if (result == ":") {
            ret += result;
        } else {
            ret += " " + result;
        }
    }

    // 第一元参数输出
    if (!arg1.empty()) {
        ret += "," + arg1;
    }

    // 第二元参数输出
    if (!arg2.empty()) {
        ret += "," + arg2;
    }

    // 其他附加信息输出
    if (!addition.empty()) {
        ret += "," + addition;
    }

    return ret;
}

#define emit(...) code.push_back(new RiscInst(__VA_ARGS__))

/// @brief 构造函数
/// @param _module 符号表
ILocRiscV64::ILocRiscV64(Module * _module)
{
    this->module = _module;
}

/// @brief 析构函数
ILocRiscV64::~ILocRiscV64()
{
    std::list<RiscInst *>::iterator pIter;

    for (pIter = code.begin(); pIter != code.end(); ++pIter) {
        delete (*pIter);
    }
}

/// @brief 删除无用的Label指令
void ILocRiscV64::deleteUnusedLabel()
{
    std::list<RiscInst *> labelInsts;
    for (RiscInst * arm: code) {
        if ((!arm->dead) && (arm->opcode[0] == '.') && (arm->result == ":")) {
            labelInsts.push_back(arm);
        }
    }

    // 检测Label指令是否在被使用，也就是是否有跳转到该Label的指令
    // 如果没有使用，则设置为dead
    for (RiscInst * labelRisc: labelInsts) {
        bool labelUsed = false;

        for (RiscInst * arm: code) {
            // TODO 转移语句的指令标识符根据定义修改判断
            if ((!arm->dead) && (((arm->opcode[0] == 'b') && (arm->arg1 == labelRisc->opcode)) ||
                                 ((arm->opcode == "jal") && (arm->arg1 == labelRisc->opcode)) ||
                                 ((arm->opcode == "j") && (arm->result == labelRisc->opcode)))) {
                labelUsed = true;
                break;
            }
        }

        if (!labelUsed) {
            labelRisc->setDead();
        }
    }
}

/// @brief 输出汇编
/// @param file 输出的文件指针
/// @param outputEmpty 是否输出空语句
void ILocRiscV64::outPut(FILE * file, bool outputEmpty)
{
    for (auto arm: code) {

        std::string s = arm->outPut();

        if (arm->result == ":") {
            // Label指令，不需要Tab输出
            fprintf(file, "%s\n", s.c_str());
            continue;
        }

        if (!s.empty()) {
            fprintf(file, "\t%s\n", s.c_str());
        } else if ((outputEmpty)) {
            fprintf(file, "\n");
        }
    }
}

/// @brief 获取当前的代码序列
/// @return 代码序列
std::list<RiscInst *> & ILocRiscV64::getCode()
{
    return code;
}

/**
 * 数字变字符串，若flag为真，则变为立即数寻址（加#）
 */
// TODO:[立即数]修改立即数逻辑
std::string ILocRiscV64::toStr(int num, bool flag)
{
    std::string ret;

    // if (flag) {
    //     ret = "#";
    // }

    ret += std::to_string(num);

    return ret;
}

/*
    产生标签
*/
void ILocRiscV64::label(std::string name)
{
    // .L1:
    emit(name, ":");
}

/// @brief 0个源操作数指令
/// @param op 操作码
/// @param rs 操作数
void ILocRiscV64::inst(std::string op, std::string rs)
{
    emit(op, rs);
}

/// @brief 一个操作数指令
/// @param op 操作码
/// @param rs 操作数
/// @param arg1 源操作数
void ILocRiscV64::inst(std::string op, std::string rs, std::string arg1)
{
    emit(op, rs, arg1);
}

/// @brief 两个操作数指令
/// @param op 操作码
/// @param rs 操作数
/// @param arg1 源操作数
/// @param arg2 源操作数
void ILocRiscV64::inst(std::string op, std::string rs, std::string arg1, std::string arg2)
{
    emit(op, rs, arg1, arg2);
}

///
/// @brief 注释指令，不包含分号
///
void ILocRiscV64::comment(std::string str)
{
    emit("@", str);
}

/// @brief 加载立即数 li r0,100
/// @param rs_reg_no 结果寄存器号
/// @param constant 立即数
void ILocRiscV64::load_imm(int rs_reg_no, int32_t constant)
{
    if (rs_reg_no == -1) {
        minic_log(LOG_ERROR, "BUG: Invalid register number for result register: %d", rs_reg_no);
        return;
    }
    if (constant <= 2047 && constant >= -2048) {
        // 如果常量在 -2048 到 2047 之间，可以直接使用 addi 指令
        emit("addiw", PlatformRiscV64::regName[rs_reg_no], "zero", std::to_string(constant));
    } else if (constant && 0xFFFFF000 == 0) {
        // 如果常量是 0xFFF00000 的倍数，可以直接使用 lui 指令
        emit("lui", PlatformRiscV64::regName[rs_reg_no], std::to_string(constant >> 12));
        return;
    } else {
        uint32_t upper = (constant >> 12) & 0xFFFFF;
        uint32_t lower = constant & 0xFFF;

        emit("lui", PlatformRiscV64::regName[rs_reg_no], std::to_string(upper));
        if (lower != 0) {
            emit(
                "addiw",
                PlatformRiscV64::regName[rs_reg_no],
                PlatformRiscV64::regName[rs_reg_no],
                std::to_string(lower));
        }
    }

    // emit("li", PlatformRiscV64::regName[rs_reg_no], std::to_string(constant));
}

/// @brief 加载立即数 li r0,100.0
/// @param rs_reg_no 结果寄存器号
/// @param constant 立即数
void ILocRiscV64::load_imm(int rs_reg_no, float num, int32_t tmp_reg_no)
{
    uint32_t numofINT = float2int(num);
    // TODO:[浮点数] 加载浮点立即数到寄存器，需要分配中间寄存器
    if (rs_reg_no == -1) {
        minic_log(LOG_ERROR, "BUG: Invalid register number for result register: %d", rs_reg_no);
        return;
    }
    if (tmp_reg_no == -1) {
        minic_log(LOG_ERROR, "BUG: Invalid register number for tempInt register: %d", tmp_reg_no);
        return;
    }
    uint32_t upper = (numofINT >> 12) & 0xFFFFF;
    uint32_t lower = numofINT & 0xFFF;
    emit("lui", PlatformRiscV64::regName[tmp_reg_no], std::to_string(upper));
    if (lower != 0) {
        emit(
            "addiw",
            PlatformRiscV64::regName[tmp_reg_no],
            PlatformRiscV64::regName[tmp_reg_no],
            std::to_string(lower));
    }
    emit("fmv.w.x", PlatformRiscV64::regName[rs_reg_no], PlatformRiscV64::regName[tmp_reg_no]);
}
/// @brief 基址寻址 lw rd, offset(base)
/// @param rs_reg_no 结果寄存器编号
/// @param base_reg_no 基址寄存器编号
/// @param offset 偏移
void ILocRiscV64::load_base(int rs_reg_no, int base_reg_no, int offset)
{
    std::string rsReg = PlatformRiscV64::regName[rs_reg_no];
    std::string base = PlatformRiscV64::regName[base_reg_no];
    std::string offset_str = toStr(offset);
    if (rs_reg_no >= 0 && rs_reg_no < 32) {
        // RISC-V64 的加载指令 lw rd, offset(base)
        emit("lw", rsReg, offset_str + "(" + base + ")");
    } else if (rs_reg_no >= 32 && rs_reg_no < 64) {
        // 对于RISC-V64，使用 flw 指令加载浮点寄存器
        emit("flw", rsReg, offset_str + "(" + base + ")");
    } else {
        minic_log(LOG_ERROR, "BUG: Invalid register number for result register: %d", rs_reg_no);
    }
}
/// @brief 基址寻址
/// @param srcReg 源寄存器
/// @param base_reg_no 基址寄存器
/// @param disp 偏移
/// @param tmp_reg_no 可能需要临时寄存器编号
void ILocRiscV64::store_base(int src_reg_no, int base_reg_no, int offset)
{
    std::string base = PlatformRiscV64::regName[base_reg_no];
    std::string srcReg = PlatformRiscV64::regName[src_reg_no];
    std::string offset_str = toStr(offset);
    if (src_reg_no >= 0 && src_reg_no < 32) {
        // RISC-V64 的存储指令 sw rs, offset(base)
        emit("sw", srcReg, offset_str + "(" + base + ")");
    } else if (src_reg_no >= 32 && src_reg_no < 64) {
        // 对于RISC-V64，使用 fsw 指令存储浮点寄存器
        emit("fsw", srcReg, offset_str + "(" + base + ")");
    } else {
        minic_log(LOG_ERROR, "BUG: Invalid register number for source register: %d", src_reg_no);
    }
}
/// @brief 寄存器Mov操作
/// @param rs_reg_no 结果寄存器
/// @param src_reg_no 源寄存器
void ILocRiscV64::mov_reg(int rs_reg_no, int src_reg_no)
{
    if(rs_reg_no>=0&&rs_reg_no<32){
        emit("mv", PlatformRiscV64::regName[rs_reg_no], PlatformRiscV64::regName[src_reg_no]);
    } else if (rs_reg_no >= 32 && rs_reg_no < 64) {
        emit("fmv.s", PlatformRiscV64::regName[rs_reg_no], PlatformRiscV64::regName[src_reg_no]);
    }
}
/// @brief 保存寄存器到局部变量，
/// @param src_reg_no 源寄存器
/// @param dest_var  局部变量
void ILocRiscV64::store_var(int src_reg_no, LocalVariable * dest_var)
{
    //在这里解决目的操作数是否为寄存器变量的问题
    int dest_reg_id = dest_var->getRegId();
    if (dest_reg_id != -1) {
        if (src_reg_no != dest_reg_id) {
            mov_reg(dest_reg_id, src_reg_no);
        }
    } else {
        // 对于局部变量，则直接从栈基址+偏移寻址
        // 栈帧偏移
        int32_t dest_baseRegId = -1;
        int64_t dest_offset = -1;
        bool    result = dest_var->getMemoryAddr(&dest_baseRegId, &dest_offset);
        if (!result) {
            minic_log(LOG_ERROR, "BUG");
        }
        store_base(src_reg_no, dest_baseRegId, dest_offset);
    }
}
void ILocRiscV64::store_var(int src_reg_no, Instruction * dest_var)
{
    //在这里解决目的操作数是否为寄存器变量的问题
    int dest_reg_id = dest_var->getRegId();
    if (dest_reg_id != -1) {
        if (src_reg_no != dest_reg_id) {
            mov_reg(dest_reg_id, src_reg_no);
        }
    } else {
        // 对于局部变量，则直接从栈基址+偏移寻址
        // 栈帧偏移
        int32_t dest_baseRegId = -1;
        int64_t dest_offset = -1;
        bool    result = dest_var->getMemoryAddr(&dest_baseRegId, &dest_offset);
        if (!result) {
            minic_log(LOG_ERROR, "BUG");
        }
        store_base(src_reg_no, dest_baseRegId, dest_offset);
    }
}

/// @brief 保存寄存器到变量，
/// @param src_reg_no 源寄存器
/// @param dest_var  全局变量
/// @param tmp_reg_no 基址寄存器
void ILocRiscV64::store_var(int src_reg_no, GlobalVariable * dest_var, int addr_reg_no)
{
    if (addr_reg_no == -1) {
        std::cout << "BUG[ILocRiscV64::store_var]:addr_reg_no can't be -1 when dealing with globalvariable.\n";
    }
    std::string name = dest_var->getName();
    emit("lui", PlatformRiscV64::regName[addr_reg_no], std::string("%hi(" + name + ")"));
    // 再加载低位
    emit(
        "sw",
        PlatformRiscV64::regName[src_reg_no],
        std::string("%lo(" + name + ")(" + PlatformRiscV64::regName[addr_reg_no] + ")"));
}

/// @brief 保存寄存器到局部变量，
/// @param src_reg_no 源寄存器
/// @param dest_var  局部变量
void ILocRiscV64::store_var(int src_reg_no, GetElementPtrInst * dest_var)
{
    //在这里解决目的操作数是否为寄存器变量的问题
    int dest_reg_id = dest_var->getRegId();
    if (dest_reg_id != -1) {
        if (src_reg_no != dest_reg_id) {
            mov_reg(dest_reg_id, src_reg_no);
        }
    } else {
        // 对于局部变量，则直接从栈基址+偏移寻址
        // 栈帧偏移
        int32_t dest_baseRegId = -1;
        int64_t dest_offset = -1;
        bool    result = dest_var->getMemoryAddr(&dest_baseRegId, &dest_offset);
        if (!result) {
            minic_log(LOG_ERROR, "BUG");
        }
        store_base(src_reg_no, dest_baseRegId, dest_offset);
    }
}
/// @brief 保存寄存器到变量，
/// @param src_reg_no 源寄存器
/// @param dest_var  全局变量
/// @param tmp_reg_no 基址寄存器
void ILocRiscV64::store_var(int src_reg_no, GetElementPtrInst * dest_var, int addr_reg_no)
{
    if (addr_reg_no == -1) {
        std::cout << "BUG[ILocRiscV64::store_var]:addr_reg_no can't be -1 when dealing with GetElementPtrInst.\n";
    }
    std::string name = dest_var->getName();
    Value *     base = dest_var->getOperand(0);
    Instanceof(index1, Instruction *, dest_var->getOperand(2)); // 数组偏移量
    Instanceof(index2, ConstInt *, index1->getOperand(0));
    int elementSize = base->getType()->getBaseElementType()->getSize(); // 比如 i32 -> 4
    int offset = 0;
    offset = (index2->getVal() * elementSize);
    emit("lui", PlatformRiscV64::regName[addr_reg_no], std::string("%hi(" + base->getName() + ")"));
    // 再加载低位
    emit(
        "sw",
        PlatformRiscV64::regName[src_reg_no],
        std::string(
            "%lo(" + base->getName() + "+" + std::to_string(offset) + ")(" + PlatformRiscV64::regName[addr_reg_no] +
            ")"));
}

/// @brief 保存寄存器到变量，保证将计算结果（r8）保存到变量
/// @param src_reg_no 源寄存器
/// @param dest_var  变量
/// @param tmp_reg_no 基址寄存器
void ILocRiscV64::store_var(int src_reg_no, Value * dest_var, int tmp_reg_no)
{
    //被保存目标变量肯定不是常量
    if (Instanceof(GEP, GetElementPtrInst *, dest_var)) {
        Value * base = GEP->getOperand(0); // GEP 的 base 是数组或结构体指针
        if (Instanceof(localbase, LocalVariable *, base)) {
            localbase->getRegId();
            store_var(src_reg_no, GEP);
        } else if (Instanceof(globalbase, GlobalVariable *, base)) {
            std::cout << 31 << std::endl;
            globalbase->getRegId();
            std::cout << 32 << std::endl;
            store_var(src_reg_no, GEP, tmp_reg_no);
            std::cout << 33 << std::endl;
        }
    } else if (Instanceof(localVar, LocalVariable *, dest_var)) {
        // 寄存器变量
        store_var(src_reg_no, localVar);
    } else if (Instanceof(instVar, Instruction *, dest_var)) {
        store_var(src_reg_no, instVar);
    } else if (Instanceof(globalVar, GlobalVariable *, dest_var)) {
        store_var(src_reg_no, globalVar, tmp_reg_no);
    } else {
        // TODO: [寻址]目前只实现了局部变量和全局变量
        std::cout << "[ILocRiscV64::store_var]被保存目标变量不是局部变量或全局变量\n";
        emit("sw", "?", "?");
    }
}
/// @brief 加载变量到寄存器，保证将变量放到reg中
/// @param rs_reg_no 结果寄存器
/// @param src_var 源操作数
void ILocRiscV64::load_var(int rs_reg_no, Value * src_var, int tmp_reg_no)
{
    if (Instanceof(GEP, GetElementPtrInst *, src_var)) {
        Value * base = GEP->getOperand(0); // GEP 的 base 是数组或结构体指针
        if (Instanceof(localbase, LocalVariable *, base)) {
            localbase->getRegId();
            load_var(rs_reg_no, GEP);
        } else if (Instanceof(globalbase, GlobalVariable *, base)) {
            std::cout << 31 << std::endl;
            globalbase->getRegId();
            std::cout << 32 << std::endl;
            load_var(rs_reg_no, GEP, tmp_reg_no);
            std::cout << 33 << std::endl;
        }
    } else if (Instanceof(constVal, ConstInt *, src_var)) {
        // 整型常量
        load_imm(rs_reg_no, constVal->getVal());
    } else if (Instanceof(constVal, ConstFloat *, src_var)) {
        // 浮点型常量
        load_imm(rs_reg_no, constVal->getVal(), tmp_reg_no);
    } else if (Instanceof(instVar, Instruction *, src_var)) {
        load_var(rs_reg_no, instVar);
    } else if (Instanceof(localVar, LocalVariable *, src_var)) {
        load_var(rs_reg_no, localVar);
    } else if (Instanceof(globalVar, GlobalVariable *, src_var)) {
        load_var(rs_reg_no, globalVar, tmp_reg_no);
    } else {
        std::cout << "[ILocRiscV64::load_var]被保存目标变量不是局部变量或全局变量或临时变量\n";
        emit("lw", "?", "?");
    }
}
/// @brief 加载变量到寄存器，保证将变量放到reg中
/// @param rs_reg_no 结果寄存器
/// @param src_var 源操作数：指令临时变量
void ILocRiscV64::load_var(int rs_reg_no, Instruction * src_var)
{
    if (src_var->getRegId() != -1) {
        // 源操作数为寄存器变量
        int src_regId = src_var->getRegId();
        if (src_regId != rs_reg_no) {
            mov_reg(rs_reg_no, src_regId);
        }
    } else {
        // 栈+偏移的寻址方式
        int32_t var_baseRegId = -1;
        int64_t var_offset = -1;
        bool    result = src_var->getMemoryAddr(&var_baseRegId, &var_offset);
        if (!result) {
            minic_log(LOG_ERROR, "BUG");
        }
        load_base(rs_reg_no, var_baseRegId, var_offset);
    }
}
/// @brief 加载变量到寄存器，保证将变量放到reg中
/// @param rs_reg_no 结果寄存器
/// @param src_var 源操作数：指令临时变量
void ILocRiscV64::load_var(int rs_reg_no, LocalVariable * src_var)
{
    if (src_var->getRegId() != -1) {
        // 源操作数为寄存器变量
        int src_regId = src_var->getRegId();
        if (src_regId != rs_reg_no) {
            mov_reg(rs_reg_no, src_regId);
        }
    } else {
        // 栈+偏移的寻址方式
        int32_t var_baseRegId = -1;
        int64_t var_offset = -1;
        bool    result = src_var->getMemoryAddr(&var_baseRegId, &var_offset);
        if (!result) {
            minic_log(LOG_ERROR, "BUG");
        }
        load_base(rs_reg_no, var_baseRegId, var_offset);
    }
}

/// @brief 加载变量到寄存器，保证将变量放到reg中
/// @param rs_reg_no 结果寄存器
/// @param src_var 源操作数：指令临时变量
void ILocRiscV64::load_var(int rs_reg_no, GetElementPtrInst * src_var)
{
    if (src_var->getRegId() != -1) {
        // 源操作数为寄存器变量
        int src_regId = src_var->getRegId();
        if (src_regId != rs_reg_no) {
            mov_reg(rs_reg_no, src_regId);
        }
    } else {
        // 栈+偏移的寻址方式
        int32_t var_baseRegId = -1;
        int64_t var_offset = -1;
        bool    result = src_var->getMemoryAddr(&var_baseRegId, &var_offset);
        if (!result) {
            minic_log(LOG_ERROR, "BUG");
        }
        load_base(rs_reg_no, var_baseRegId, var_offset);
    }
}

/// @brief 加载变量到寄存器，保证将变量放到reg中
/// @param rs_reg_no 结果寄存器
/// @param src_var 源操作数：全局变量
void ILocRiscV64::load_var(int rs_reg_no, GlobalVariable * src_var, int addr_reg_no)
{
    // xxx:可以做局部改进，将addr_reg_no与rs_reg_no设为同一寄存器
    std::string name = src_var->getName();
    emit("lui", PlatformRiscV64::regName[addr_reg_no], std::string("%hi(" + name + ")"));
    // 再加载低位
    emit(
        "lw",
        PlatformRiscV64::regName[rs_reg_no],
        std::string("%lo(" + name + ")(" + PlatformRiscV64::regName[addr_reg_no] + ")"));
}

/// @brief 加载变量到寄存器，保证将变量放到reg中
/// @param rs_reg_no 结果寄存器
/// @param src_var 源操作数：全局变量
void ILocRiscV64::load_var(int rs_reg_no, GetElementPtrInst * src_var, int addr_reg_no)
{
    // xxx:可以做局部改进，将addr_reg_no与rs_reg_no设为同一寄存器
    std::string name = src_var->getName();
    Value *     base = src_var->getOperand(0);
    Instanceof(index1, Instruction *, src_var->getOperand(2)); // 数组偏移量
    Instanceof(index2, ConstInt *, index1->getOperand(0));
    int elementSize = base->getType()->getBaseElementType()->getSize(); // 比如 i32 -> 4
    int offset = 0;
    offset = (index2->getVal() * elementSize);
    emit("lui", PlatformRiscV64::regName[addr_reg_no], std::string("%hi(" + base->getName() + ")"));
    // 再加载低位
    emit(
        "lw",
        PlatformRiscV64::regName[rs_reg_no],
        std::string(
            "%lo(" + base->getName() + "+" + std::to_string(offset) + ")(" + PlatformRiscV64::regName[addr_reg_no] +
            ")"));
}
/// @brief 加载变量地址到寄存器
/// @param rs_reg_no
/// @param var
// TODO:@JEV055 论证是否弃用
void ILocRiscV64::lea_var(int rs_reg_no, Value * var)
{
    // 被加载的变量肯定不是常量！
    // 被加载的变量肯定不是寄存器变量！

    // 目前只考虑局部变量

    // 栈帧偏移
    int32_t var_baseRegId = -1;
    int64_t var_offset = -1;

    bool result = var->getMemoryAddr(&var_baseRegId, &var_offset);
    if (!result) {
        minic_log(LOG_ERROR, "BUG");
    }

    std::string rsReg = PlatformRiscV64::regName[rs_reg_no];
    std::string base = PlatformRiscV64::regName[var_baseRegId];
    std::string offset_str = std::to_string(var_offset);
    emit("addi", rsReg, base, offset_str);
}

/// @brief 加载符号值 ldr r0,=g ldr r0,=.L1
/// @param rs_reg_no 结果寄存器编号
/// @param name 符号名
// TODO:论证是否弃用
void ILocRiscV64::load_symbol(int rs_reg_no, std::string name)
{
    // 假设符号地址可以直接通过 lui 和 addi 来处理
    // 这里简单模拟加载符号地址到寄存器
    // 实际中可能需要根据链接器等情况处理
    // 先加载高位
    emit("lui", PlatformRiscV64::regName[rs_reg_no], std::string("%hi(" + name + ")"));
    // 再加载低位
    emit(
        "lw",
        PlatformRiscV64::regName[rs_reg_no],
        PlatformRiscV64::regName[rs_reg_no],
        std::string("%lo(" + name + ")(" + PlatformRiscV64::regName[rs_reg_no] + ")"));
}

/// @brief 加载栈内变量地址
/// @param rsReg 结果寄存器号
/// @param base_reg_no 基址寄存器
/// @param off 偏移
void ILocRiscV64::leaStack(int rs_reg_no, int base_reg_no, int off)
{
    std::string rs_reg_name = PlatformRiscV64::regName[rs_reg_no];
    std::string base_reg_name = PlatformRiscV64::regName[base_reg_no];
    std::string off_str = std::to_string(off);
    emit("addi", rs_reg_name, base_reg_name, off_str);
}

/// @brief 函数内栈内空间分配（局部变量、形参变量、函数参数传值，或不能寄存器分配的临时变量等）
/// @param func 函数
/// @param tmp_reg_No
void ILocRiscV64::allocStack(Function * func)
{
    // 计算栈帧大小
    int off = func->getMaxDep();

    // 不需要在栈内额外分配空间，则什么都不做
    if (0 == off) {
        return;
    }

    // 保存SP寄存器到FP寄存器中
    // mov_reg(RISCV64_FP_REG_NO, RISCV64_SP_REG_NO);

    std::string off_str = std::to_string(off);
    emit("addi", "sp", "sp", "-" + off_str);
}

/// @brief 调用函数fun
/// @param fun
void ILocRiscV64::call_fun(std::string name)
{
    emit("jal", "ra", name);
}

/// @brief NOP操作
void ILocRiscV64::nop()
{
    emit("addi", "x0", "x0", "0");
}

///
/// @brief 无条件跳转指令
/// @param label 目标Label名称
///
void ILocRiscV64::jump(std::string label)
{
    emit("jal", "x0", label);
}
