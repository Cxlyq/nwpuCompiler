///
/// @file Value.cpp
/// @brief 值操作类型，所有的变量、函数、常量都是Value
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

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sys/types.h>

#include "Value.h"
#include "Use.h"

/// @brief 构造函数
/// @param _type
Value::Value(Type * _type) : type(_type)
{
    // 不需要增加代码
    valueType = NONE;
    valueCategory = ValueCategory::UNKNOWN;
    isInited = false;
}

/// @brief 析构函数
Value::~Value()
{
    // 如有资源清理，请这里追加代码
}

/// @brief 获取名字
/// @return 变量名
std::string Value::getName() const
{
    return name;
}

///
/// @brief 设置名字
/// @param _name 名字
///
void Value::setName(std::string _name)
{
    this->name = _name;
}

/// @brief 获取名字
/// @return 变量名
std::string Value::getIRName() const
{
    return IRName;
}

///
/// @brief 设置名字
/// @param _name 名字
///
void Value::setIRName(std::string _name)
{
    this->IRName = _name;
}

/// @brief 获取类型
/// @return 变量名
Type * Value::getType() const
{
    return type;
}

///
/// @brief 增加一条边，增加Value被使用次数
/// @param use
///
void Value::addUse(Use * use)
{
    uses.push_back(use);
}

///
/// @brief 消除一条边，减少Value被使用次数
/// @param use
///
void Value::removeUse(Use * use)
{
    auto pIter = std::find(uses.begin(), uses.end(), use);
    if (pIter != uses.end()) {
        uses.erase(pIter);
    }
}

/// @brief 清点使用该Value的use
/// @return 变量尚存的被使用个数
int32_t Value::getUserNum()
{
	return (int32_t) uses.size();
}

///
/// @brief 取得变量所在的作用域层级
/// @return int32_t 层级
///
int32_t Value::getScopeLevel()
{
    std::cout << "MethodCall warning: use base class(Value)method instead of extend methods.";
    return -1;
}

///
/// @brief 获得分配的寄存器编号或ID
/// @return int32_t 寄存器编号 -1代表无效的寄存器编号
///
int32_t Value::getRegId()
{
    std::cout << "MethodCall warning: use base class(Value)method instead of extend methods.";
    return -1;
}

///
/// @brief @brief 如是内存变量型Value，则获取基址寄存器和偏移
/// @param regId 寄存器编号
/// @param offset 相对偏移
/// @return true 是内存型变量
/// @return false 不是内存型变量
///
bool Value::getMemoryAddr(int32_t * regId, int64_t * offset)
{
    (void) regId;
    (void) offset;
    std::cout << "MethodCall warning: use base class(Value)method instead of extend methods.";
    return false;
}


///
/// @brief 对该Value进行Load用的寄存器编号
/// @return int32_t 寄存器编号
///
void Value::setRegId(int32_t regId)
{
    (void) regId;
    std::cout << "MethodCall warning: use base class(Value)method instead of extend methods.";
}

// 为value赋初值
// TODO:[重构] 将下面方法抽象为较少的父方法，在每一类value的对应文件中override Value对应子类的继承方法。
float Value::getFloatInitVal()
{
    return initVal.floatVal;
}
uint32_t Value::getIntInitVal()
{
    return initVal.intVal;
}
Value::ValueType Value::getValueType()
{
    return valueType;
}

ValueCategory Value::getValueCategory()
{
    return valueCategory;
}

void Value::setCategory(ValueCategory cat)
{
    valueCategory = cat;
}

bool Value::setInitVal(float val)
{
    // if (valueCategory != ValueCategory::CONSTANT) {
    //     std::cerr << "Error: setInitVal to a not-constant variable." << std::endl;
    //     return false;
    // }
    initVal.floatVal = val;
    this->valueType = ValueType::FLOAT;
    isInited = true;
    this->val.floatVal = val;
    return true;
}
bool Value::setInitVal(uint32_t val)
{
    // if (valueCategory != ValueCategory::CONSTANT) {
    //     std::cerr << "Error: setInitVal to a not-constant variable." << std::endl;
    //     return false;
    // }
    initVal.intVal = val;
    this->valueType = ValueType::INT;
    isInited = true;
    this->val.intVal = val;
    return true;
}

std::string Value::getInitValStr()
{
    if (valueType == ValueType::FLOAT) {
        return std::to_string(initVal.floatVal);
    } else {
        return std::to_string(initVal.intVal);
    }
}

uint32_t Value::getIntVal()
{
    return val.intVal;
}
float Value::getFloatVal()
{
    return val.floatVal;
}
void Value::setVal(uint32_t val)
{
    this->val.intVal = val;
}
void Value::setVal(float val)
{
    this->val.floatVal = val;
}
bool Value::setInitVal(std::vector<int> * arrayVal)
{

    valueType = ARRAY_INT;
    initVal.array_int_init_list = arrayVal;
    isInited = true;
    return true;
}
bool Value::setInitVal(std::vector<float> * arrayVal)
{
    valueType = ARRAY_FLOAT;
    initVal.array_float_init_list = arrayVal;
    isInited = true;
    return true;
}
// TODO:完成访问数组初值的功能
bool Value::getArrayValByIndex(std::vector<int> & indexs, float * val)
{
    const std::vector<int> origin_dims = this->getType()->getDimensions();
    // 检查索引的大小是否超过数组维度
    if (indexs.empty()) {
        std::cerr << "Error: indexs cannot be empty." << std::endl;
        return false;
    }
    if (origin_dims.empty()) {
        std::cerr << "Error: origin_dims cannot be empty." << std::endl;
        return false;
    }
    // 检查索引是否在有效范围内
    if (indexs.size() != origin_dims.size()) {
        std::cerr << "Error: index size exceeds array dimensions." << std::endl;
        return false;
    }
    for (size_t i = 0; i < indexs.size(); ++i) {
        if (indexs[i] < 0 || indexs[i] >= origin_dims[i]) {
            std::cerr << "Error: index out of bounds for dimension " << i << "." << std::endl;
            return false;
        }
    }
    // 计算偏移
    int linear_index = 0;
    int stride = 1;
    // 从后向前累乘
    for (int i = origin_dims.size() - 1; i >= 0; --i) {
        linear_index += indexs[i] * stride;
        stride *= origin_dims[i];
    }

    if (this->valueCategory == ValueCategory::CONSTANT && this->isInited) {
        if (this->valueType == ValueType::ARRAY_INT) {
            *val = (float) (*(this->initVal.array_int_init_list))[linear_index];
            return true;
        } else if (this->valueType == ValueType::ARRAY_FLOAT) {
            *val = (*(this->initVal.array_float_init_list))[linear_index];
            return true;
        } else {
            std::cerr << "Error: Value is not an array type." << std::endl;
            return false;
        }
    } else {
        std::cerr << "Error: Value is not initialized or not a constant." << std::endl;
        return false;
    }
    return false;
}

std::vector<int> * Value::getInitIntVal()
{
    return initVal.array_int_init_list;
}
std::vector<float> * Value::getInitFloatVal()
{
    return initVal.array_float_init_list;
}