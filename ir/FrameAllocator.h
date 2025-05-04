#pragma once

#include <cstdint>

///
/// @brief 栈帧分配器：用于在函数栈中分配局部变量的空间
///
class FrameAllocator {
public:
    /// @brief 初始化偏移（通常从0开始，或设为负值表示相对FP向下增长）
    explicit FrameAllocator(int startOffset = 0) {
        currentOffset = startOffset;
    }

    /// @brief 分配 size 字节，并返回该变量的偏移量（一般是负值）
    /// @param size 所需字节数
    /// @param alignment 对齐（默认按4字节对齐）
    /// @return 栈帧中偏移（如 -4, -8, ...）
    int allocate(int size, int alignment = 4) {
        // 向下对齐
        currentOffset -= alignTo(size, alignment);
        return currentOffset;
    }

    /// @brief 获取当前栈帧总大小（用于函数开始时调整栈）
    int getTotalSize() const {
        return -currentOffset;  // 因为 offset 是负增长
    }

    /// @brief 重置分配器（进入新函数作用域可用）
    void reset(int startOffset = 0) {
        currentOffset = startOffset;
    }

private:
    int currentOffset = 0;

    int alignTo(int size, int alignment) {
        return ((size + alignment - 1) / alignment) * alignment;
    }
};
