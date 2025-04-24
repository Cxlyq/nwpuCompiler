#include "Types/FloatType.h"

FloatType * FloatType::oneInstance = nullptr;

FloatType * FloatType::getType()
{
    if (!oneInstance) {
        oneInstance = new FloatType();
    }
    return oneInstance;
}
std::string FloatType::toString() const {
    return "float";
}

int32_t FloatType::getSize() const {
    return 4;
}

