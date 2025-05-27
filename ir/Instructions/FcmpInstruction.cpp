///
/// @file IcmpInstruction.cpp
/// @brief 整数比较指令 (icmp) 实现
///
/// @author Your Name // 修改为你的名字
/// @version 1.0
/// @date 2024-10-26 // 修改为当前日期
///
/// @copyright Copyright (c) 2024 // 修改为你的信息
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-10-26 <td>1.0     <td>Your Name  <td>新建 IcmpInstruction 实现
/// </table>
///

#include "FcmpInstruction.h"
#include "Constant.h"
#include "Function.h" // Assuming Function class is needed/used by Instruction base
#include "Value.h"    // Needed for Value operations like getIRName()
#include "Type.h"     // Needed to get the result type (i1) and operand types
#include "IntegerType.h"

// Assuming Type::getIntegerType(1) returns the i1 type.
// If you have a dedicated BooleanType, use BooleanType::getType() instead.
// #include "BooleanType.h" // If you use a dedicated BooleanType

// Helper function to map internal predicate enum to LLVM IR string
// You can also put this logic directly in toString if preferred.
static std::string getFcmpPredicateString(IRInstOperator predicate)
{
    switch (predicate) {
        case IRInstOperator::IRINST_OP_EQ_F:
            return "oeq"; // Equal
        case IRInstOperator::IRINST_OP_NEQ_F:
            return "une"; // Not Equal
        case IRInstOperator::IRINST_OP_GE_F:
            return "oge"; // Signed Greater Than or Equal To
        case IRInstOperator::IRINST_OP_LE_F:
            return "ole"; // Signed Less Than or Equal To
        case IRInstOperator::IRINST_OP_GNE_F:
            return "ogt"; // Signed Greater Than
        case IRInstOperator::IRINST_OP_LNE_F:
            return "olt"; // Signed Less Than
        default:
            return "invalid_fcmp_predicate"; // Should not happen if used correctly
    }
}

///
/// @brief 构造函数
/// @param _func 所属函数
/// @param _predicate 比较谓词
/// @param _lhs 左操作数
/// @param _rhs 右操作数
///
FcmpInstruction::FcmpInstruction(Function * _func, IRInstOperator _predicate, Value * _lhs, Value * _rhs)
    // Call base class constructor: Instruction(function, opcode, result_type)
    // The opcode is the predicate itself. The result type of icmp is i1 (boolean).
    : Instruction(_func, _predicate, IntegerType::getTypeBool()), lhs(_lhs), rhs(_rhs)
{
    // Optional: Add assertions or checks here to ensure predicate is a valid ICMP op
    // and that lhs/rhs types are compatible integer types.
}

///
/// @brief 构造函数
/// @param _func 所属函数
/// @param _predicate 比较谓词
/// @param _lhs 左操作数
/// @param _rhs 右常数操作数
///
FcmpInstruction::FcmpInstruction(Function * _func, IRInstOperator _predicate, Value * _lhs, Constant * _rhsConst)
    // Call base class constructor: Instruction(function, opcode, result_type)
    // The opcode is the predicate itself. The result type of icmp is i1 (boolean).
    : Instruction(_func, _predicate, IntegerType::getTypeBool()), lhs(_lhs), rhsConst(_rhsConst)
{
    // Optional: Add assertions or checks here to ensure predicate is a valid ICMP op
    // and that lhs/rhs types are compatible integer types.
    isRhsConst = true; // 标记 rhs 是常数
}

///
/// @brief 转换成IR指令文本形式
/// @param str IR指令文本
///
void FcmpInstruction::toString(std::string & str)
{
    if (!isRhsConst) {
        // LLVM IR icmp format: %result = icmp predicate operand1_type operand1, operand2
        // Example: %t7 = icmp ne i32 %l3, 0

        // Get the comparison predicate string (e.g., "ne", "sge")
        std::string predicate_str = getFcmpPredicateString(op); // 'op' is inherited from Instruction

        // Get the type of the operands being compared (e.g., "i32").
        // Assumes lhs and rhs have the same type and getType() and toString() methods.
        std::string operand_type_str = lhs->getType()->toString();

        // Get the IR names of the operands (e.g., "%l3", "0")
        std::string lhs_name = lhs->getIRName(); // Assumes Value has getIRName()
        std::string rhs_name = rhs->getIRName(); // Assumes Value has getIRName()

        // Get the IR name of this instruction itself.
        // Instructions produce a result value, and their name is the name of that result (e.g., "%t7").
        // Assumes Instruction (which inherits from User/Value) has getIRName().
        std::string result_name = getIRName();

        // Build the final string
        str = result_name + " = fcmp " + predicate_str + " " + operand_type_str + " " + lhs_name + ", " + rhs_name;
    } else {
        // LLVM IR icmp format: %result = icmp predicate operand1_type operand1, operand2
        // Example: %t7 = icmp ne i32 %l3, 0

        // Get the comparison predicate string (e.g., "ne", "sge")
        std::string predicate_str = getFcmpPredicateString(op); // 'op' is inherited from Instruction

        // Get the type of the operands being compared (e.g., "i32").
        // Assumes lhs and rhs have the same type and getType() and toString() methods.
        std::string operand_type_str = lhs->getType()->toString();

        // Get the IR names of the operands (e.g., "%l3", "0")
        std::string lhs_name = lhs->getIRName();      // Assumes Value has getIRName()
        std::string rhs_name = rhsConst->getIRName(); // Assumes Value has getIRName()

        // Get the IR name of this instruction itself.
        // Instructions produce a result value, and their name is the name of that result (e.g., "%t7").
        // Assumes Instruction (which inherits from User/Value) has getIRName().
        std::string result_name = getIRName();

        // Build the final string
        str = result_name + " = fcmp " + predicate_str + " " + operand_type_str + " " + lhs_name + ", " + rhs_name;
    }
}