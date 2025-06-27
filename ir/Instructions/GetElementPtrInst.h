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
private:
    //用于计算数组的基址和偏移
    int baseRegId_ = -1; ///< 存放基址寄存器编号（如 a0 → 10）
    int offset_ = 0;     ///< 存放偏移量（单位：字节）

public:
    /// 设置用于访问该 GEP 结果的基址寄存器
    void setAddressingInfo(int32_t baseRegId, int32_t offset)
    {
        baseRegId_ = baseRegId;
        offset_ = offset;
    }

    /// 获取基址寄存器编号
    int getBaseRegId() const
    {
        return baseRegId_;
    }

    /// 获取偏移量
    int getOffset() const
    {
        return offset_;
    }
};
