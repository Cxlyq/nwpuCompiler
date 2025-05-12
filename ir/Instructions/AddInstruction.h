#ifndef ADD_INSTRUCTION_H
#define ADD_INSTRUCTION_H

#include "Instruction.h"
#include "Value.h"
#include "Function.h"
#include <VoidType.h>
class AddInstruction : public Instruction {
public:
    AddInstruction(Value * res, Value * left, Value * right, Function * func);

    void toString(std::string & str) override;

private:
    Value * result;
    Value * lhs;
    Value * rhs;
};

#endif // ADD_INSTRUCTION_H
