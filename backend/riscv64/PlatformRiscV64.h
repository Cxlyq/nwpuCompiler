#pragma once

#include <string>

#include "RegVariable.h"

// 在操作过程中临时借助的寄存器为RISCV64_TMP_REG_NO
#define RISCV64_TMP_REG_NO 5

// 栈寄存器SP和FP
#define RISCV64_SP_REG_NO 2
#define RISCV64_FP_REG_NO 8

// 函数跳转寄存器
#define RISCV64_RA_REG_NO 1 // ra = x1

/// @brief RISCV64平台信息
class PlatformRiscV64 {

    /// @brief 循环左移两位
    /// @param num
    static void roundLeftShiftTwoBit(unsigned int & num);

    /// @brief 判断num是否是常数表达式，8位数字循环右移偶数位得到
    /// @param num
    /// @return
    static bool __constExpr(int num);

public:
    /// @brief 同时处理正数和负数
    /// @param num
    /// @return
    static bool constExpr(int num);

    /// @brief 判定是否是合法的偏移
    /// @param num
    /// @return
    static bool isDisp(int num);

    /// @brief 判断是否是合法的寄存器名
    /// @param name 寄存器名字
    /// @return 是否是
    static bool isReg(std::string name);

    /// @brief 最大寄存器数目
    static const int maxRegNum = 64;

    /// @brief 可使用的通用寄存器的个数
    // x5-x7, x10-x17, x28-x31
    //使用x10-x17需要调用前保存/恢复
    static const int maxUsableRegNum = 15;
    
    /// @brief 可使用的通用寄存器的对应编号
    static const int RISCV64_REGS[maxUsableRegNum];

    /// @brief 寄存器的名字，r0-r15
    static const std::string regName[maxRegNum];

    /// @brief 对寄存器R0分配Value，记录位置
    static RegVariable * intRegVal[PlatformRiscV64::maxRegNum / 2];

    /// @brief 对寄存器R0分配Value，记录位置(浮点型)
    static RegVariable * floatRegVal[PlatformRiscV64::maxRegNum / 2];
};
