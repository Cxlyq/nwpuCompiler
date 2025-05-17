#pragma once

#include "Instruction.h"
#include "Value.h"
#include "Function.h"

/// @brief 从内存地址加载值的指令
class LoadInstruction : public Instruction {
public:
    /// @brief 构造函数
    /// @param _func 所属函数
    // / @param result 结果保存的变量（目标寄存器或变量）
    /// @param addr 内存地址（源地址）
    LoadInstruction(Function * _func,  Value * addr);

    /// @brief 转换为字符串形式
    /// @param str 输出的字符串
    void toString(std::string & str) override;
};
