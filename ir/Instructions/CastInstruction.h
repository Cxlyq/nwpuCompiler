///
/// @file CastInstruction.h
/// @brief 一元操作指令
///
/// @author   Kevin-wjq13777(w137776w@gmail.com)
/// @version 1.0
/// @date 2025/5/26
///
/// @copyright Copyright (c) 2025
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2025/5/26 <td>1.0     <td>Kevin-wjq13777  <td>新建
/// </table>
///
#pragma once

#include <string>

#include "Value.h"
#include "Instruction.h"

class Function;

///
/// @brief 一元操作指令
///
class CastInstruction : public Instruction {
public:
    ///
    /// @brief 构造函数
    /// @param _op 一元操作类型（POS, NEG, NOT）
    /// @param _srcVal 源操作数
    /// @param _func 所属函数
    ///
    CastInstruction(Function * _func, Value * srcVal, Type * _type);

    /// @brief 转换成字符串
    void toString(std::string & str) override;
};
