#pragma once

#include <string>
#include <cstdint>
#include "Type.h"

class FloatType final : public Type {

public:
    ///
    /// @brief 获取类型，全局只有一份
    /// @return FloatType*
    ///
    static FloatType * getType();

    ///
    /// @brief 获取类型的IR标识符
    /// @return std::string IR标识符float
    ///
    [[nodiscard]] std::string toString() const override;


    ///
    /// @brief 获得类型所占内存空间大小（单位：字节）
    /// @return int32_t
    ///
    [[nodiscard]] int32_t getSize() const override;

private:
    ///
    /// @brief 构造函数
    ///
    explicit FloatType() : Type(Type::FloatTyID) {}

    ///
    /// @brief 唯一的 float 类型实例
    ///
    static FloatType * oneInstance;
};
