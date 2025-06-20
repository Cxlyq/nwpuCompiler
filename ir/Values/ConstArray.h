#pragma once

#include "Constant.h"
#include <vector>
#include <iostream>
#include <cassert>
#include "ArrayType.h"

/// @brief 多维数组常量类
class ConstArray : public Constant {
public:
    /// @param elemType 数组元素的类型（如 FloatType）
    /// @param elements 常量元素列表（如 ConstFloat）
    /// @param dims 各维度的大小
    ConstArray(Type * elemType, const std::vector<Constant *> & elements, const std::vector<int> & dims)
        : Constant(ArrayType::getArrayType(elemType, dims)), dimensions(dims)
    {
        assert(dims.size() > 0); // 必须至少有一个维度

        // 创建多维数组
        values = createMultiDimArray(elements, dims, 0);
        name = "array";
    }

    [[nodiscard]] std::string getIRName() const override
    {
        return name;
    }

    [[nodiscard]] const std::vector<Constant *> & getValues() const
    {
        return values;
    }

    int32_t getRegId() override
    {
        return regId;
    }

    void setRegId(int32_t regId) override
    {
        this->regId = regId;
    }

    /// @brief 获取指定维度的元素
    /// @param indices 各维度的索引（例如二维数组时提供 {row, col}）
    /// @return 指定位置的元素
    Constant * getElementAt(const std::vector<int> & indices) const
    {
        assert(indices.size() == dimensions.size()); // 确保索引维度与数组维度一致
        size_t index = 0;
        size_t multiplier = 1;

        for (size_t i = dimensions.size(); i-- > 0;) {
            assert(indices[i] < dimensions[i]); // 确保索引在范围内
            index += indices[i] * multiplier;
            multiplier *= dimensions[i];
        }

        return values[index]; // 返回指定位置的元素
    }

    /// @brief 判断是否为零值数组
    [[nodiscard]] bool isZeroValue() const override
    {
        for (const auto & val: values) {
            if (!val->isZeroValue())
                return false;
        }
        return true;
    }

private:
    std::vector<Constant *> values;     // 用于存储多维数组的元素
    std::vector<int>        dimensions; // 存储各个维度的大小
    int32_t                 regId = -1; // 寄存器编号

    // TODO // 正确性检查
    //  递归创建多维数组结构
    std::vector<Constant *>
    createMultiDimArray(const std::vector<Constant *> & elements, const std::vector<int> & dims, size_t dim)
    {
        if (dim == dims.size() - 1) {
            return elements; // 最后一个维度，返回元素列表
        }

        size_t                  elementsPerDimension = dims[dim];
        std::vector<Constant *> subArrays;
        // size_t idx = 0;

        for (size_t i = 0; i < elementsPerDimension; ++i) {
            std::vector<Constant *> subElements = createMultiDimArray(elements, dims, dim + 1);
            subArrays.push_back(new ConstArray(
                ArrayType::getArrayType(elements[0]->getType(), {dims[dim + 1]}),
                subElements,
                {dims[dim + 1]}));
        }

        return subArrays;
    }
};
