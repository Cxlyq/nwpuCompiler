///
/// @file GlobalVariable.h
/// @brief 全局变量描述类
///
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-29
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// </table>
///
#pragma once

#include "GlobalValue.h"
#include "IRConstant.h"
#include "iostream"
#include <string>
///
/// @brief 全局变量，寻址时通过符号名或变量名来寻址
///
class GlobalVariable : public GlobalValue {

public:
    ///
    /// @brief 构建全局变量，默认对齐为4字节
    /// @param _type 类型
    /// @param _name 名字
    ///
    explicit GlobalVariable(Type * _type, std::string _name) : GlobalValue(_type, _name)
    {
        // 设置对齐大小
        setAlignment(4);
    }

    ///
    /// @brief  检查是否是函数
    /// @return true 是函数
    /// @return false 不是函数
    ///
    [[nodiscard]] bool isGlobalVarible() const override
    {
        return true;
    }

    ///
    /// @brief 是否属于BSS段的变量，即未初始化过的变量，或者初值都为0的变量
    /// @return true
    /// @return false
    ///
    [[nodiscard]] bool isInBSSSection() const
    {
        return this->inBSSSection;
    }

    ///
    /// @brief 取得变量所在的作用域层级
    /// @return int32_t 层级
    ///
    int32_t getScopeLevel() override
    {
        return 0;
    }

    ///
    /// @brief 对该Value进行Load用的寄存器编号
    /// @return int32_t 寄存器编号
    ///
    int32_t getRegId() override
    {
        return this->regId;
    }

    ///
    /// @brief 对该Value进行Load用的寄存器编号
    /// @return int32_t 寄存器编号
    ///
    void setRegId(int32_t regId) override
    {
        this->regId = regId;
    }

    // TODO:对于全局变量，是否有初值
    bool isZeroValue() const override
    {
        // 简单返回 false，或根据需要自定义
        return false;
    }

    // FIXME: 全局变量的赋初值指令生成，注意区分常量和变量，常量直接做替换
    ///
    /// @brief Declare指令IR显示
    /// @param str
    ///
    void toDeclareString(std::string & str)
    {
        // 添加全局常数判断
        if (this->valueCategory == ValueCategory::CONSTANT) {
            // LLVM IR 中，全局变量必须以 `@` 开头
            str = getIRName() + " = dso_local constant " + getType()->toString() + " ";
        } else {
            // LLVM IR 中，全局变量必须以 `@` 开头
            str = getIRName() + " = global " + getType()->toString() + " ";
        }

        if (isInited) {
            if (getType()->isArrayType()) {
                // 数组类型的初始值需要特殊处理
                str += "[";
                if (getType()->getBaseElementType()->isFloatType()) {
                    std::vector<double> * floatList = initVal.array_float_init_list;

                    for (size_t i = 0; i < floatList->size(); ++i) {
                        double val = (*floatList)[i];
                        str += "float " + std::to_string(val);
                        if (i != floatList->size() - 1)
                            str += ", ";
                    }
                } else {
                    std::vector<int> * intList = initVal.array_int_init_list;

                    for (size_t i = 0; i < intList->size(); ++i) {
                        int val = (*intList)[i];
                        str += "i32 " + std::to_string(val);
                        if (i != intList->size() - 1)
                            str += ", ";
                    }
                }

                str += "]";
            } else {
                str += getInitValStr(); // 初始化值，如 "0", "1", ...
            }

        } else {
            if (getType()->isArrayType())
                str += "zeroinitializer";
            else {
                str += "0"; // 未初始化
            }
        }
        str += ", align " + std::to_string(getAlignment());
    }

    /// 设置初始值（支持 ConstantInt、ConstantFP、ConstantArray 等）
    void setInitializer(Constant * init)
    {
        this->initializer = init;
        if (init != nullptr && !init->isZeroValue()) {
            inBSSSection = false;
        }
    }

    void setFasle_inBSSSection()
    {
        inBSSSection = false;
    }

    /// 获取初始值
    [[nodiscard]] Constant * getInitializer() const
    {
        return initializer;
    }

    /// 判断是否是数组
    [[nodiscard]] bool isArray() const
    {
        return getType()->isArrayType();
    }

    /// 判断是否是浮点型
    [[nodiscard]] bool isFloat() const
    {
        return getType()->isFloatType();
    }

private:
    ///
    /// @brief 变量加载到寄存器中时对应的寄存器编号
    ///
    int32_t regId = -1;

    ///
    /// @brief 默认全局变量在BSS段，没有初始化，或者即使初始化过，但都值都为0
    ///
    bool inBSSSection = true;

    ///
    /// @brief 变量的初始值
    /// @note 该值在BSS段中是无效的
    /// @note 该值在数据段中是有效的
    Constant * initializer = nullptr; // 新增：记录初始值
};
