///
/// @file Value.h
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
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "Use.h"
#include "Type.h"
#include <cstring>

///
/// @brief 值类，每个值都要有一个类型，全局变量和局部变量可以有名字，
/// 但通过运算得到的指令类值没有名字，只有在需要输出时给定名字即可
///
/// Value表示所有可计算的值的基类，例如常量、指令、参数等。
/// 每个Value都有一个类型(Type)和一个名字(Name)。Value是IR中所有可计算实体的抽象。
///

enum class ValueCategory {
    UNKNOWN,
    CONSTANT, // 常量
    VARIABLE, // 变量
    IMMEDIATE // 立即数
};
struct FloatNum {
    double   val;        // 浮点数值
    uint64_t float_bits; // 高精度float二进制格式
};
class Value {

protected:
    /// @brief 变量名，函数名等原始的名字，可能为空串
    std::string name;

    ///
    /// @brief IR名字，用于文本IR的输出
    ///
    std::string IRName;

    /// @brief 类型
    Type * type;

    ///
    /// @brief define-use链，这个定值被使用的所有边，即所有的User
    ///
    std::vector<Use *> uses;

    enum ValueType { NONE, INT, FLOAT, ARRAY_INT, ARRAY_FLOAT };

    union InitVal {
        int                   intVal;
        FloatNum              floatVal;
        std::vector<int> *    array_int_init_list;
        std::vector<double> * array_float_init_list;
    };

    InitVal       initVal;
    ValueType     valueType = NONE;
    ValueCategory valueCategory = ValueCategory::UNKNOWN;

public:
    bool isInited;

    /// @brief 构造函数
    /// @param _type
    explicit Value(Type * _type);

    /// @brief 析构函数
    virtual ~Value();

    /// @brief 获取名字
    /// @return 变量名
    [[nodiscard]] virtual std::string getName() const;

    ///
    /// @brief 设置名字
    /// @param _name 名字
    ///
    void setName(std::string _name);

    /// @brief 获取名字
    /// @return 变量名
    [[nodiscard]] virtual std::string getIRName() const;

    ///
    /// @brief 设置名字
    /// @param _name 名字
    ///
    void setIRName(std::string _name);

    /// @brief 获取类型
    /// @return 变量名
    virtual Type * getType() const;

    void setType(Type * _type)
    {
        this->type = _type;
    }
    ///
    /// @brief 增加一条边，增加Value被使用次数
    /// @param use
    ///
    void addUse(Use * use);

    ///
    /// @brief 消除一条边，减少Value被使用次数
    /// @param use
    ///
    void removeUse(Use * use);

    /// @brief 清点使用该Value的use
    /// @return 变量尚存的被使用个数
    int32_t getUserNum();

    ///
    /// @brief 取得变量所在的作用域层级
    /// @return int32_t 层级
    ///
    virtual int32_t getScopeLevel();

    ///
    /// @brief 获得分配的寄存器编号或ID
    /// @return int32_t 寄存器编号
    ///
    virtual int32_t getRegId();

    ///
    /// @brief @brief 如是内存变量型Value，则获取基址寄存器和偏移
    /// @param regId 寄存器编号
    /// @param offset 相对偏移
    /// @return true 是内存型变量
    /// @return false 不是内存型变量
    ///
    virtual bool getMemoryAddr(int32_t * regId = nullptr, int64_t * offset = nullptr);

    ///
    /// @brief 对该Value进行Load用的寄存器编号
    /// @return int32_t 寄存器编号
    ///
    virtual void setRegId(int32_t regId);

    // 为value赋初值

    double                getFloatInitVal();
    int                   getIntInitVal();
    ValueType             getValueType();
    ValueCategory         getValueCategory();             //获取值的类别（常量或变量）
    void                  setCategory(ValueCategory cat); //设置值的类别（常量或变量）
    bool                  setInitVal(FloatNum val);
    bool                  setInitVal(int val);
    bool                  setInitVal(std::vector<int> * arrayVal);
    bool                  setInitVal(std::vector<double> * arrayVal);
    std::vector<int> *    getInitIntVal();
    std::vector<double> * getInitFloatVal();
    std::string           getInitValStr();
    bool                  getArrayValByIndex(std::vector<int> & indexs, double * val);

    template <typename T, typename U>
    static U bitcast(T value)
    {
        static_assert(sizeof(T) == sizeof(U), "Size mismatch");
        U result;
        std::memcpy(&result, &value, sizeof(T));
        return result;
    }
};
