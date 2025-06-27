#pragma once

#include "Constant.h"
#include "IRConstant.h"
#include "FloatType.h"
#include "Value.h"
#include "sstream"
///
/// @brief 浮点型常量类
///
class ConstFloat : public Constant {

public:
    ///
    /// @brief 指定值的常量
    /// \param val 浮点数值
    explicit ConstFloat(float val) : Constant(FloatType::getType())
    {
        floatVal = val;
        double   extend = static_cast<double>(val);
        uint64_t float_bits = bitcast<double, uint64_t>(extend);
        setInitVal({val, float_bits});
        // name = std::to_string(val);
        std::ostringstream oss;
        oss << "0x" << std::hex << std::uppercase << float_bits;
        name = oss.str();
    }

    /// @brief 获取名字
    /// @return 变量名
    [[nodiscard]] std::string getIRName() const override
    {
        return name;
    }

    ///
    /// @brief 获取值
    /// @return float
    ///
    float getVal() const
    {
        return floatVal;
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
    /// @brief 设置该Value加载时的寄存器编号
    /// @param regId 寄存器编号
    ///
    void setRegId(int32_t regId) override
    {
        this->regId = regId;
    }

    ///
    /// @brief 判断是否为零值（用于判断是否属于.bss段）
    /// @return true 零值
    [[nodiscard]] bool isZeroValue() const override
    {
        return floatVal == 0.0f;
    }

private:
    /// @brief 浮点数值
    float floatVal;

    /// @brief 变量加载到寄存器中时对应的寄存器编号
    int32_t regId = -1;
};
