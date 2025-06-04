#pragma once

#include "Instruction.h"
#include "Value.h"
#include "Function.h"
#include <vector>
#include "Type.h"
#include "PointerType.h"

/// @brief GEP（GetElementPtr）指令，从数组指针中计算元素地址
class GetElementPtrInst : public Instruction {
public:
    /// @brief 构造函数
    /// @param func 所属函数
    /// @param base 基础指针（数组变量）
    /// @param gepType 要访问的原始数组类型（如 [5 x [6 x i32]]）
    /// @param indices 多维索引值（通常是 {0, idx1, idx2, ...}）
    GetElementPtrInst(Function * func, Value * base, Type * gepType, const std::vector<Value *> & indices);

    /// @brief 转换为 LLVM IR 字符串
    /// @param str 输出字符串
    void toString(std::string & str) override;

    // private:
    //     /// 存储原始类型，用于输出 GEP 的类型描述
    //     Type * rawType;
};
