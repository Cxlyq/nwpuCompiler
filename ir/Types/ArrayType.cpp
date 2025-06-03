#include "ArrayType.h"

// // 构造函数
// ArrayType::ArrayType(Type * elemType, const std::vector<int> & dims) : elementType(elemType), dimensions(dims)
// {
//     // 验证至少有一个维度
//     assert(dims.size() > 0);
//     // 计算总大小
//     sizeInBytes = calculateSize();

//     ID = ArrayTyID; // 设置类型ID
// }
ArrayType::ArrayType(Type * elemType, const std::vector<int> & dims)
{
    assert(dims.size() > 0);
    ID = ArrayTyID;
    dimensions = dims;
    if (dims.size() == 1) {
        elementType = elemType;
        thisdimensionSize = dims[0];
    } else {
        std::vector<int> sub_dims(dims.begin() + 1, dims.end());
        elementType = new ArrayType(elemType, sub_dims);
        thisdimensionSize = dims[0];
    }

    sizeInBytes = calculateSize();
}

// 获取数组的元素类型
Type * ArrayType::getElementType() const
{
    return elementType;
}

// 获取数组的维度
const std::vector<int> & ArrayType::getDimensions() const
{
    return dimensions;
}

// 获取数组的大小（字节数）
int ArrayType::getSize() const
{
    return sizeInBytes;
}

// 获取数组元素的大小（字节数）
int ArrayType::getSizeInBytes() const
{
    return sizeInBytes;
}

// 获取数组的维度数量
size_t ArrayType::getNumDimensions() const
{
    return dimensions.size();
}

// 判断两个数组类型是否相等
bool ArrayType::operator==(const ArrayType & other) const
{
    return (elementType == other.elementType) && (dimensions == other.dimensions);
}

// 静态方法，获取数组类型
ArrayType * ArrayType::getArrayType(Type * elemType, const std::vector<int> & dims)
{
    return new ArrayType(elemType, dims);
}

// 计算数组的总字节数
int ArrayType::calculateSize() const
{
    int totalSize = 1;
    for (int dim: dimensions) {
        totalSize *= dim; // 累乘所有维度的大小
    }
    return totalSize * elementType->getSizeInBytes(); // 乘以元素类型的字节大小
}
