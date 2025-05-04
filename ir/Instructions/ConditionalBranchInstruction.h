///
/// @file ConditionalBranchInstruction.h
/// @brief 分支跳转指令
///
/// @author cx (cxlyq.g@gmail.com)
/// @version 1.0
/// @date 2025-05-03
///
/// @copyright Copyright (c) 2025
///
#pragma once

#include <string>

#include "Instruction.h"

#include "LabelInstruction.h"

#include "Value.h"
class Function;

///
/// @brief 跳转分支指令
///
class ConditionalInstruction : public Instruction {

public:
    ///
    /// @brief 构造函数
    /// @param _func 所属函数
    ///
    explicit ConditionalInstruction(Function * _func,
                                     Value * _cond_val,
                                     LabelInstruction * _true_branch_label,
                                     LabelInstruction * _false_branch_target);

    ///
    /// @brief 转换成字符串
    /// @param str 返回指令字符串
    ///
    void toString(std::string & str) override;
private:
    Value * cond_val;
    LabelInstruction * true_branch_label;
    LabelInstruction * false_branch_target;
};
