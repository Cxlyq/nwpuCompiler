#pragma once

#include "Instruction.h"
#include "Value.h"    // 假设 Value 类包含地址和值信息
#include "Function.h" // 假设包含函数的头文件
#include "VoidType.h" // 假设 VoidType 定义了返回类型

/// @brief 存储操作指令，将值存储到内存地址
class StoreInstruction : public Instruction {
public:
    /// @brief 构造函数
    /// @param _func 所属的函数
    /// @param addr 存储目标地址
    /// @param val 存储的值
    StoreInstruction(Function * _func, Value * addr, Value * val);

    /// @brief 转换成字符串显示
    /// @param str 转换后的字符串
    void toString(std::string & str) override;
};
