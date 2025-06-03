#pragma once

#include "Type.h"
#include <vector>
#include <cassert>
#include <iostream>

class ArrayType : public Type {
public:
    /// @brief 构造函数
    /// @param elemType 数组元素类型
    /// @param dims 数组的维度和每一维的大小
    ArrayType(Type * elementType, const std::vector<int> & dims);

    /// @brief 获取数组的元素类型
    Type * getElementType() const override;

    /// @brief 获取数组的维度
    const std::vector<int> & getDimensions() const;

    /// @brief 获取数组的大小（字节数）
    int getSize() const override; // 声明覆盖

    /// @brief 获取数组的大小（字节数）
    int getSizeInBytes() const override;

    /// @brief 获取数组的维度数量
    size_t getNumDimensions() const;

    /// @brief 判断两个数组类型是否相等
    bool operator==(const ArrayType & other) const;

    /// @brief 静态方法，获取数组类型
    static ArrayType * getArrayType(Type * elemType, const std::vector<int> & dims);

    // std::string toString() const override
    // {
    //     std::string result = "ArrayType(";
    //     result += elementType->toString() + ", [";
    //     for (size_t i = 0; i < dimensions.size(); ++i) {
    //         result += std::to_string(dimensions[i]);
    //         if (i < dimensions.size() - 1) {
    //             result += ", ";
    //         }
    //     }
    //     result += "])";
    //     return result;
    // }

    /// @brief 转换为字符串形式，适用于zlj格式
    // std::string toString() const override
    // {
    //     std::string result = elementType->toString();
    //     // 从最后一维往前包裹
    //     for (auto it = dimensions.rbegin(); it != dimensions.rend(); ++it) {
    //         result = "[" + std::to_string(*it) + " x " + result + "]";
    //     }
    //     return result;
    // }

    /// @brief 转换为字符串形式，适用于标准llvm格式
    std::string toString() const override
    {
        std::string str = elementType->toString();
        for (int i = dimensions.size() - 1; i >= 0; --i) {
            str = "[" + std::to_string(dimensions[i]) + " x " + str + "]";
        }
        return str;
    }

private:
    Type *           elementType; // 数组元素的类型
    std::vector<int> dimensions;  // 数组的维度信息
    int              sizeInBytes; // 数组的大小（字节）

    /// @brief 计算数组的总字节数
    int calculateSize() const;
};
