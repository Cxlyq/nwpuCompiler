///
/// @file IRGenerator.cpp
/// @brief AST遍历产生线性IR的源文件
/// @author zenglj (zenglj@live.com)
/// @version 1.1
/// @date 2024-11-23
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// <tr><td>2024-11-23 <td>1.1     <td>zenglj  <td>表达式版增强
/// </table>
///
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cmath>

#include "AST.h"
#include "ArrayType.h"
#include "AttrType.h"
#include "Common.h"
#include "ConstFloat.h"
#include "ConstInt.h"
#include "FloatType.h"
#include "Function.h"
#include "GlobalVariable.h"
#include "IRCode.h"
#include "IRGenerator.h"
#include "Instruction.h"
#include "IntegerType.h"
#include "LocalVariable.h"
#include "Module.h"
#include "EntryInstruction.h"
#include "LabelInstruction.h"
#include "ExitInstruction.h"
#include "FuncCallInstruction.h"
#include "BinaryInstruction.h"
#include "MoveInstruction.h"
#include "GotoInstruction.h"
#include "PointerType.h"
#include "StoreInstruction.h"
#include "Type.h"
#include "UnaryInstruction.h"
#include "ConditionalBranchInstruction.h"
#include "Value.h"
#include "LoadInstruction.h"
#include "CastInstruction.h"
#include "IcmpInstruction.h"
#include "FcmpInstruction.h"
#include "GetElementPtrInst.h"

/// @brief 构造函数
/// @param _root AST的根
/// @param _module 符号表
IRGenerator::IRGenerator(ast_node * _root, Module * _module) : root(_root), module(_module)
{
    /* 叶子节点 */

    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_LITERAL_UINT] = &IRGenerator::ir_leaf_node_uint;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_VAR_ID] = &IRGenerator::ir_leaf_node_var_id;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_TYPE] = &IRGenerator::ir_leaf_node_type;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT] = &IRGenerator::ir_leaf_node_float;
    ast2ir_handlers[ast_operator_type::AST_OP_ARRAY_ACCESS] = &IRGenerator::ir_array_access;

    /* 表达式运算， 加减 */
    ast2ir_handlers[ast_operator_type::AST_OP_SUB] = &IRGenerator::ir_sub;
    ast2ir_handlers[ast_operator_type::AST_OP_ADD] = &IRGenerator::ir_add;
    ast2ir_handlers[ast_operator_type::AST_OP_MUL] = &IRGenerator::ir_mul;
    ast2ir_handlers[ast_operator_type::AST_OP_DIV] = &IRGenerator::ir_div;
    ast2ir_handlers[ast_operator_type::AST_OP_MOD] = &IRGenerator::ir_mod;
    ast2ir_handlers[ast_operator_type::AST_OP_AND] = &IRGenerator::ir_and;
    ast2ir_handlers[ast_operator_type::AST_OP_OR] = &IRGenerator::ir_or;
    ast2ir_handlers[ast_operator_type::AST_OP_EQ] = &IRGenerator::ir_eq;
    ast2ir_handlers[ast_operator_type::AST_OP_NEQ] = &IRGenerator::ir_neq;
    ast2ir_handlers[ast_operator_type::AST_OP_GE] = &IRGenerator::ir_ge;
    ast2ir_handlers[ast_operator_type::AST_OP_LE] = &IRGenerator::ir_le;
    ast2ir_handlers[ast_operator_type::AST_OP_GNE] = &IRGenerator::ir_gne;
    ast2ir_handlers[ast_operator_type::AST_OP_LNE] = &IRGenerator::ir_lne;
    ast2ir_handlers[ast_operator_type::AST_OP_POS] = &IRGenerator::ir_pos;
    ast2ir_handlers[ast_operator_type::AST_OP_NEG] = &IRGenerator::ir_neg;
    ast2ir_handlers[ast_operator_type::AST_OP_NOT] = &IRGenerator::ir_not;

    /* 语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_ASSIGN] = &IRGenerator::ir_assign;
    ast2ir_handlers[ast_operator_type::AST_OP_RETURN] = &IRGenerator::ir_return;
    ast2ir_handlers[ast_operator_type::AST_OP_IFELSE] = &IRGenerator::ir_ifelse;
    ast2ir_handlers[ast_operator_type::AST_OP_WHILE] = &IRGenerator::ir_while;
    ast2ir_handlers[ast_operator_type::AST_OP_BREAK] = &IRGenerator::ir_break;
    ast2ir_handlers[ast_operator_type::AST_OP_CONTINUE] = &IRGenerator::ir_continue;

    /* 函数调用 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_CALL] = &IRGenerator::ir_function_call;

    /* 函数定义 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_DEF] = &IRGenerator::ir_function_define;
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS] = &IRGenerator::ir_function_formal_params;

    /* 变量定义语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_VAR_DECL_STMT] = &IRGenerator::ir_declare_statment;
    ast2ir_handlers[ast_operator_type::AST_OP_VAR_DECL] = &IRGenerator::ir_variable_declare;

    ast2ir_handlers[ast_operator_type::AST_OP_CONST_DECL_STMT] = &IRGenerator::ir_const_declare_statment;
    ast2ir_handlers[ast_operator_type::AST_OP_CONST_DECL] = &IRGenerator::ir_const_declare;

    /* 语句块 */
    ast2ir_handlers[ast_operator_type::AST_OP_BLOCK] = &IRGenerator::ir_block;

    /* 编译单元 */
    ast2ir_handlers[ast_operator_type::AST_OP_COMPILE_UNIT] = &IRGenerator::ir_compile_unit;
}

/// @brief 遍历抽象语法树产生线性IR，保存到IRCode中
/// @param root 抽象语法树
/// @param IRCode 线性IR
/// @return true: 成功 false: 失败
bool IRGenerator::run()
{
    ast_node * node;

    // 从根节点进行遍历
    node = ir_visit_ast_node(root);

    return node != nullptr;
}

/// @brief 根据AST的节点运算符查找对应的翻译函数并执行翻译动作
/// @param node AST节点
/// @return 成功返回node节点，否则返回nullptr
ast_node * IRGenerator::ir_visit_ast_node(ast_node * node)
{
    // 空节点
    if (nullptr == node) {
        return nullptr;
    }

    bool result;

    std::unordered_map<ast_operator_type, ast2ir_handler_t>::const_iterator pIter;
    pIter = ast2ir_handlers.find(node->node_type);
    if (pIter == ast2ir_handlers.end()) {
        // 没有找到，则说明当前不支持

        result = (this->ir_default)(node);
    } else {
        result = (this->*(pIter->second))(node);
    }

    if (!result) {
        // 语义解析错误，则出错返回
        node = nullptr;
    }

    return node;
}

/// @brief 遍历数组节点，提取数组名和维度信息
/// @param array_node 数组节点
/// @param name 数组名
/// @param dims 数组维度
/// @note 该函数会遍历数组节点，提取数组名和维度信息
void extract_array_info(ast_node * array_node, std::string & name, std::vector<ast_node *> & dims)
{
    while (array_node->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
        // 使用 if/else 替代 assert
        if (array_node->sons.size() != 2) {
            std::fprintf(
                stderr,
                "Assertion failed: array_node->sons.size() == 2, file %s, line %d\n",
                __FILE__,
                __LINE__);
            std::abort();
        }

        dims.insert(dims.begin(), array_node->sons[1]); // 从右向左插入维度
        array_node = array_node->sons[0];               // 向左深入
    }
    name = array_node->name; // 最左侧是变量标识符
}

/// @brief 计算偏移量
/// @param ori_dims 原始维度
/// @param dims 当前维度
/// @return 偏移量
int calcOffset(const std::vector<int> & ori_dims, const std::vector<int> & dims)
{
    int offset = 0;
    int stride = 1;
    for (int i = ori_dims.size() - 1; i >= 0; --i) {
        offset += dims[i] * stride;
        stride *= ori_dims[i];
    }
    return offset;
}

/// @brief 未知节点类型的节点处理
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_default(ast_node * node)
{
    // 未知的节点
    printf("Unkown node(%d)\n", (int) node->node_type);
    return true;
}

/// @brief 编译单元AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_compile_unit(ast_node * node)
{
    module->setCurrentFunction(nullptr);
    for (auto son: node->sons) {

        // 遍历编译单元，要么是函数定义，要么是语句
        ast_node * son_node = ir_visit_ast_node(son);
        if (!son_node) {
            std::cerr << "Compile unit: Failed to compile unit!" << std::endl;
            return false;
        }
    }

    return true;
}

/// @brief 函数定义AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_define(ast_node * node)
{
    bool result;
    // 创建一个函数，用于当前函数处理
    if (module->getCurrentFunction()) {
        // 函数中嵌套定义函数，这是不允许的，错误退出
        std::cerr << "Function define: Not allow to define a new function in the function!" << std::endl;
        return false;
    }

    // 函数定义的AST包含四个孩子
    // 第一个孩子：函数返回类型
    // 第二个孩子：函数名字
    // 第三个孩子：形参列表
    // 第四个孩子：函数体即block
    ast_node * type_node = node->sons[0];
    ast_node * name_node = node->sons[1];
    ast_node * param_node = node->sons[2];
    ast_node * block_node = node->sons[3];

    // 创建一个新的函数定义
    Function * newFunc = module->newFunction(name_node->name, type_node->type);
    if (!newFunc) {
        // 新定义的函数已经存在，则失败返回。
        printf("Function define: multiple define function %s\n", name_node->name.c_str());
        return false;
    }

    // 当前函数设置有效，变更为当前的函数
    module->setCurrentFunction(newFunc);

    // 进入函数的作用域
    module->enterScope();

    // 获取函数的IR代码列表，用于后面追加指令用，注意这里用的是引用传值
    InterCode & irCode = newFunc->getInterCode();
    /// NOTE:不需要entry指令，多了反而报错
    // // 这里也可增加一个函数入口Label指令，便于后续基本块划分
    // LabelInstruction * entryLabelInst = new LabelInstruction(newFunc);
    // irCode.addInst(entryLabelInst);

    // // 创建并加入Entry入口指令
     irCode.addInst(new EntryInstruction(newFunc));

    // 创建出口指令并不加入出口指令，等函数内的指令处理完毕后加入出口指令
    LabelInstruction * exitLabelInst = new LabelInstruction(newFunc);

    // 函数出口指令保存到函数信息中，因为在语义分析函数体时return语句需要跳转到函数尾部，需要这个label指令
    newFunc->setExitLabel(exitLabelInst);

    // 遍历形参，没有IR指令，不需要追加
    result = ir_function_formal_params(param_node);
    if (!result) {
        // 形参解析失败
        printf("Function define: function(%s) formals error\n", name_node->name.c_str());
        return false;
    }
    node->blockInsts.addInst(param_node->blockInsts);

    // 新建一个Value，用于保存函数的返回值，如果没有返回值可不用申请
    Value * retValue = nullptr;

    if (!type_node->type->isVoidType()) {
        // 保存函数返回值变量到函数信息中，在return语句翻译时需要设置值到这个变量中
        retValue = static_cast<LocalVariable *>(module->newVarValue(type_node->type, "ret"));
        // XXX: 初步完成：这里最好设置返回值变量的初值为0，以便在没有返回值时能够返回0
        node->blockInsts.addInst(new StoreInstruction(newFunc, retValue, module->newConstInt(0)));
    }
    newFunc->setReturnValue(retValue);

    // 函数内已经进入作用域，内部不再需要做变量的作用域管理
    block_node->needScope = false;

    // 遍历block
    result = ir_block(block_node);
    if (!result) {
        // block解析失败
        printf("Function define: function(%s) block error\n", name_node->name.c_str());
        return false;
    }

    // IR指令追加到当前的节点中
    node->blockInsts.addInst(block_node->blockInsts);

    // 此时，所有指令都加入到当前函数中，也就是node->blockInsts

    // node节点的指令移动到函数的IR指令列表中
    irCode.addInst(node->blockInsts);

    // XXX:取消了出口指令，但上述似乎有一处添加了
    //  添加函数出口Label指令，主要用于return语句跳转到这里进行函数的退出
    // irCode.addInst(exitLabelInst);

    // 函数出口指令
    auto * loadExit = new LoadInstruction(newFunc, newFunc->getReturnValue());
    irCode.addInst(loadExit);
    irCode.addInst(new ExitInstruction(newFunc, loadExit));

    // 恢复成外部函数
    module->setCurrentFunction(nullptr);

    // 退出函数的作用域
    module->leaveScope();

    return true;
}

/// @brief 形式参数AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_formal_params(ast_node * node)
{

    // 每个形参变量都创建对应的临时变量，用于表达实参转递的值
    // 而真实的形参则创建函数内的局部变量。
    // 然后产生赋值指令，用于把表达实参值的临时变量拷贝到形参局部变量上。
    // 请注意这些指令要放在Entry指令后面，因此处理的先后上要注意。
    // node 节点的 sons 列表包含每个形参的AST节点 (例如: int a, int arr[], int arr[][10])

    Function * currentFunc = module->getCurrentFunction();
    if (!currentFunc) {
        // Should not happen if called from ir_function_define correctly
        std::cerr << "Function formal params: called outside function context." << std::endl;
        return false;
    }

    // 遍历形参列表的每一个形参节点 (AST_TYPE_PARAM_DECL)
    // 假设每个 param_decl_node 的结构是 [TypeNode, Identifier/ArrayNameNode]
    for (ast_node * param_decl_node: node->sons) {
        if (param_decl_node->sons.size() < 2) {
            std::cerr << "Function formal params: Invalid AST structure for parameter declaration." << std::endl;
            return false;
        } else if (param_decl_node->sons.size() > 2) { //数组型参
            std::vector<ast_node *> array_def_nodes;
            for (auto node: param_decl_node->sons) {
                array_def_nodes.push_back(node); // 维度节点
            }
            ast_node *       type_node = array_def_nodes[0];       // 第一个节点是类型
            ast_node *       array_name_node = array_def_nodes[1]; // 第二个节点是数组名
            std::vector<int> dims;                                 // 数组维度
            // 提取数组维度信息
            for (size_t i = 2; i < array_def_nodes.size(); ++i) {
                ast_node * dim_node = array_def_nodes[i];
                dims.push_back(dim_node->integer_val);
            }

            dims[0] = 0; // 形参数组的第一个维度为0，表示形参数组的大小不确定

            std::string array_name = array_name_node->name;
            Type *      param_type_ir = type_node->type;

            if (!param_type_ir) {
                std::cerr << "Function formal params: Failed to determine IR type for parameter '" << array_name
                          << "' in function '" << currentFunc->getName() << "'" << std::endl;
                return false;
            }

            ArrayType * arrayType = new ArrayType(param_type_ir, dims);
            if (!arrayType) {
                std::cerr << "Function formal params: Failer to generate Array Type!" << std::endl;
            }
            Type * eletype = arrayType->getElementType();
            std::cout << "Function formal params: Array type is " << arrayType->toString() << std::endl;
            PointerType * pointerType = new PointerType(eletype);
            std::cout << "Function formal params: Pointer type is " << pointerType->toString() << std::endl;
            // PointerType * pointee = new PointerType(pointerType);
            //  创建一个数组局部变量
            Value * param_value = module->newVarValue(pointerType, array_name);
            if (!param_value) {
                std::cerr << "Function formal params: Failed to create IR Value for parameter '" << array_name
                          << "' in function '" << currentFunc->getName() << "'" << std::endl;
                return false;
            }
            param_decl_node->val = param_value;

            auto fParam = new FormalParam(pointerType, array_name);
            currentFunc->addParams(fParam);

            // 生成 MoveInstruction 将传入实参值复制到局部形参变量
            // Instruction * move_inst = new MoveInstruction(currentFunc, param_value, fParam);
            StoreInstruction * store_inst = new StoreInstruction(currentFunc, param_value, fParam);
            node->blockInsts.addInst(store_inst);
        } else {
            ast_node *  type_node = param_decl_node->sons[0];
            ast_node *  param_node = param_decl_node->sons[1];
            std::string param_name;
            param_name = param_node->name;
            Type * param_type_ir = type_node->type;
            if (!param_type_ir) {
                std::cerr << "Function formal params: Failed to determine IR type for parameter '"
                          << "' in function '" << currentFunc->getName() << "'" << param_name << "' in function '"
                          << currentFunc->getName() << "'"
                          << "'" << std::endl;
                return false;
            }
            Value * param_value = module->newVarValue(param_type_ir, param_name);
            param_decl_node->val = param_value;
            if (!param_value) {
                std::cerr << "Function formal params: Failed to create IR Value for parameter '"
                          << "' in function '" << currentFunc->getName() << "'" << param_name << "'"
                          << "' in function '" << currentFunc->getName() << "'" << std::endl;
                return false;
            }
            auto fParam = new FormalParam(param_type_ir, param_name);
            currentFunc->addParams(fParam);

            // 生成 MoveInstruction 将传入实参值复制到局部形参变量
            // 这条指令确保了传入的值被存储在作用域中的 LocalVariable 中，供函数体使用。
            // MoveInstruction(Function* func, Value* dest, Value* src)
            // Instruction *      move_inst = new MoveInstruction(currentFunc, param_value, fParam);
            StoreInstruction * store_inst = new StoreInstruction(currentFunc, param_value, fParam);
            node->blockInsts.addInst(store_inst);
        }
    }
    // 所有形参处理成功
    return true;
}

/// @brief 函数调用AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_call(ast_node * node)
{
    std::vector<Value *> realParams;

    // 获取当前正在处理的函数
    Function * currentFunc = module->getCurrentFunction();

    // 函数调用的节点包含两个节点：
    // 第一个节点：函数名节点
    // 第二个节点：实参列表节点

    std::string funcName = node->sons[0]->name;
    int64_t     lineno = node->sons[0]->line_no;

    ast_node * paramsNode = node->sons[1];

    // 根据函数名查找函数，看是否存在。若不存在则出错
    // 这里约定函数必须先定义后使用
    auto calledFunction = module->findFunction(funcName);
    if (nullptr == calledFunction) {
        minic_log(LOG_ERROR, "函数(%s)未定义或声明", funcName.c_str());
        return false;
    }

    // 当前函数存在函数调用
    currentFunc->setExistFuncCall(true);

    // 如果没有孩子，也认为是没有参数

    // 有参数：
    if (!paramsNode->sons.empty()) {

        int32_t argsCount = (int32_t) paramsNode->sons.size();

        // 当前函数中调用函数实参个数最大值统计，实际上是统计实参传参需在栈中分配的大小
        // 因为目前的语言支持的int和float都是四字节的，只统计个数即可
        if (argsCount > currentFunc->getMaxFuncCallArgCnt()) {
            currentFunc->setMaxFuncCallArgCnt(argsCount);
        }

        // 遍历参数列表，孩子是表达式
        // 这里自左往右计算表达式
        for (auto son: paramsNode->sons) {
            ast_node * son_node;
            Type *     son_type = son->type;
            // std::cout << "Function call(Real Param): son type is " << son_type->toString() << std::endl;
            ///因为如果是数组访问，走专门的函数，所以不能visit，否则会额外生成ir
            if (!(son->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS)) {
                son_node = ir_visit_ast_node(son);
            }

            if (son->node_type == ast_operator_type::AST_OP_LEAF_VAR_ID) {
                // 叶子节点是变量ID
                // std::cout << "Function call(Real Param): son_node is " << son_node->name << std::endl;
                son_type = module->findVarValue(son_node->name)->getType();
            } // else {
            //     // std::cout << "Function call(Real Param): son_node is not leaf node!" << std::endl;
            // }
            // std::cout << "type " << son_type->toString() << std::endl;

            ///如果是数组，就要走专门的函数
            if (son->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS || son_type->isArrayType()) {
                // std::cout << "Function call(Real Param): arrayRParam is " << std::endl;
                Value * arrayRParam = funcall_array_access(son);
                // std::cout << "Function call(Real Param): arrayRParam is " << arrayRParam << std::endl;
                //  if (!arrayRParam) {
                //      std::cerr << "Function call(Real Param):Failed to array access!" << std::endl;
                //      return false;
                //  }
                if (arrayRParam == nullptr) {
                    std::cerr << "Function call(Real Param):Failed to array access, arrayRParam is nullptr!"
                              << std::endl;
                    return false;
                }
                realParams.push_back(arrayRParam);
                node->blockInsts.addInst(son->blockInsts);
            } else {
                // 遍历Block的每个语句，进行显示或者运算
                // ast_node * temp = ir_visit_ast_node(son);
                if (!son_node) {
                    return false;
                }
                realParams.push_back(son_node->val);
                node->blockInsts.addInst(son_node->blockInsts);
            }
        }
    }

    // TODO 这里请追加函数调用的语义错误检查，这里只进行了函数参数的个数检查等，其它请自行追加。
    if (realParams.size() != calledFunction->getParams().size()) {
        // 函数参数的个数不一致，语义错误
        std::cout << realParams.size() << " " << calledFunction->getParams().size() << std::endl;
        minic_log(LOG_ERROR, "第%lld行的被调用函数(%s)未定义或声明", (long long) lineno, funcName.c_str());
        return false;
    } else {
        // for (int paramNo = 0; paramNo < realParams.size(); paramNo++) {
        //     if (realParams[paramNo]->getType() != calledFunction->getParams()[paramNo]->getType()) {
        //         // 参数类型不匹配
        //         minic_log(LOG_ERROR, "函数(%s)的第%d个参数类型不匹配", funcName.c_str(), paramNo + 1);
        //         return false;
        //     }
        // }
    }
    calledFunction->realParams = realParams;
    // 返回调用有返回值，则需要分配临时变量，用于保存函数调用的返回值
    Type * type = calledFunction->getReturnType();

    FuncCallInstruction * funcCallInst = new FuncCallInstruction(currentFunc, calledFunction, realParams, type);

    // 创建函数调用指令
    node->blockInsts.addInst(funcCallInst);

    // 函数调用结果Value保存到node中，可能为空，上层节点可利用这个值
    node->val = funcCallInst;

    return true;
}

/// @brief 语句块（含函数体）AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_block(ast_node * node)
{
    // 进入作用域
    if (node->needScope) {
        module->enterScope();
    }

    std::vector<ast_node *>::iterator pIter;
    for (pIter = node->sons.begin(); pIter != node->sons.end(); ++pIter) {

        // 遍历Block的每个语句，进行显示或者运算
        ast_node * temp = ir_visit_ast_node(*pIter);
        if (!temp) {
            return false;
        }

        node->blockInsts.addInst(temp->blockInsts);
    }

    // 离开作用域
    if (node->needScope) {
        module->leaveScope();
    }

    return true;
}

/// @brief 整数和float加法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_add(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    auto addInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_ADD_I,
        IRInstOperator::IRINST_OP_ADD_F);

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(addInst);

    node->val = addInst;

    return true;
}

/// @brief 整数和float减法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_sub(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 减法节点，左结合，先计算左节点，后计算右节点

    // 减法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 减法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    auto subInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_SUB_I,
        IRInstOperator::IRINST_OP_SUB_F);

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(subInst);

    node->val = subInst;

    return true;
}

/// @brief 整数和float乘法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mul(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 乘法节点，左结合，先计算左节点，后计算右节点

    // 乘法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 乘法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    auto mulInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_MUL_I,
        IRInstOperator::IRINST_OP_MUL_F);
    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(mulInst);

    node->val = mulInst;

    return true;
}

/// @brief 整数和float除法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_div(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 除法节点，左结合，先计算左节点，后计算右节点

    // 除法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 除法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    if (((int) right->node_type) == 0 && !right->integer_val) {
        //为整数0时报除数为0错误
        return false;
    }
    if (((int) right->node_type) == 1 && !right->integer_val) {

        //为浮点数0时报除数为0.0错误
        return false;
    }

    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    /// 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    auto divInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_DIV_I,
        IRInstOperator::IRINST_OP_DIV_F);

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(divInst);

    node->val = divInst;

    return true;
}

/// @brief 整数取余AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mod(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 取余节点，左结合，先计算左节点，后计算右节点

    // 取余的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 取余的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }
    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    //取余运算不支持float类型
    bool isFloat = (lhs->getType()->isArrayType() ? lhs->getType()->getElementType()->isFloatType()
                                                  : lhs->getType()->isFloatType()) ||
                   (rhs->getType()->isArrayType() ? rhs->getType()->getElementType()->isFloatType()
                                                  : rhs->getType()->isFloatType());
    if (isFloat) {
        return false;
    }

    if ((right->node_type) == ast_operator_type::AST_OP_LEAF_LITERAL_UINT && !right->integer_val) {
        //为整数0时报mod 0错误
        return false;
    }

    BinaryInstruction * modInst = new BinaryInstruction(
        module->getCurrentFunction(),
        IRInstOperator::IRINST_OP_MOD_I,
        lhs,
        rhs,
        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(modInst);

    node->val = modInst;

    return true;
}

/// @brief 逻辑与AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_and(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];
    Value *    zero = module->newConstInt(0);
    Value *    one = module->newConstInt(1);

    // 逻辑与节点，左结合，先计算左节点，后计算右节点
    // 2. 创建表示结构不同基本块入口的标签
    // 这些标签将在后续指令中被引用（作为跳转目标）
    // 同时，它们本身也是指令，会被添加到线性指令列表中，代表基本块的开始。
    Function * currentFunc = module->getCurrentFunction();

    // 第一个变量为真，检查第二个变量的入口标签
    LabelInstruction * check_nonzero = new LabelInstruction(currentFunc);
    // 假块的入口标签 (如果存在)。如果在 else 块之前创建，可以作为假分支的目标。
    LabelInstruction * store_true = new LabelInstruction(currentFunc);
    LabelInstruction * store_false = new LabelInstruction(currentFunc);

    // 汇合点标签
    LabelInstruction * merge_label = new LabelInstruction(currentFunc);
    // 逻辑与的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    Value * lhs = left->val;
    node->blockInsts.addInst(left->blockInsts);
    ///检查操作数是否是数组，若是需要load
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

        // printf("yes,left\n");
        LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
        lhs = LoadInst;
        lhs->setType(module->findVarValue(left->name)->getType());
        node->blockInsts.addInst(LoadInst);
    }

    auto neqInst1 = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        zero,
        IRInstOperator::IRINST_OP_NEQ_I,
        IRInstOperator::IRINST_OP_NEQ_F,
        true);
    node->blockInsts.addInst(neqInst1);
    ConditionalInstruction * cond_branch_inst1 =
        new ConditionalInstruction(currentFunc, neqInst1, check_nonzero, store_false);
    node->blockInsts.addInst(cond_branch_inst1);
    // 添加标签
    node->blockInsts.addInst(check_nonzero);
    // 逻辑与的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
        // printf("yes,right\n");
        LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
        rhs = LoadInst;
        rhs->setType(module->findVarValue(right->name)->getType());
        node->blockInsts.addInst(LoadInst);
    }

    auto neqInst2 = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        rhs,
        zero,
        IRInstOperator::IRINST_OP_NEQ_I,
        IRInstOperator::IRINST_OP_NEQ_F,
        true);
    node->blockInsts.addInst(neqInst2);

    ConditionalInstruction * cond_branch_inst2 =
        new ConditionalInstruction(currentFunc, neqInst2, store_true, store_false);
    node->blockInsts.addInst(cond_branch_inst2);
    // 添加标签
    node->blockInsts.addInst(store_true);

    // // 2. 对左值做 icmp ne，判断是否为真
    // Value * zero = module->newConstInt(0);
    // Value * cmp1 = new ICmpInstruction(currentFunc, ICmpInstruction::ICMP_NE, lhs_val, zero);
    // node->blockInsts.addInst(cmp1);

    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);

    std::string tmpName = generateTempName("ValueOfLogic");
    Value * ValueOfLogic = module->newVarValueWithInt(IntegerType::getTypeInt(), tmpName, 0, ValueCategory::VARIABLE);
    // ValueOfLogic->setType(left->val->getType());
    StoreInstruction * storeInst1 = new StoreInstruction(module->getCurrentFunction(), ValueOfLogic, one);
    node->blockInsts.addInst(storeInst1);
    // 在 then 块的末尾添加一个无条件跳转到 merge 块的指令。
    // 即使 then 块的最后一条指令本身是一个终止指令（如 return 或 goto），
    // 为了简化生成逻辑，通常还是会添加一个额外的跳转指令。优化阶段可以移除死代码。
    // 使用你提供的 GotoInstruction 类 (它是无条件跳转)。
    node->blockInsts.addInst(new GotoInstruction(currentFunc, merge_label));
    node->blockInsts.addInst(store_false);
    StoreInstruction * storeInst2 = new StoreInstruction(module->getCurrentFunction(), ValueOfLogic, zero);
    node->blockInsts.addInst(storeInst2);

    node->blockInsts.addInst(new GotoInstruction(currentFunc, merge_label));
    node->blockInsts.addInst(merge_label);
    LoadInstruction * LoadInst3 = new LoadInstruction(module->getCurrentFunction(), ValueOfLogic);
    LoadInst3->setType(module->findVarValue(ValueOfLogic->getName())->getType());
    node->blockInsts.addInst(LoadInst3);
    // TODO 逻辑运算是否需要区别int和float型,对float是否作检查，检查代码参考取余部分，

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);

    node->val = ValueOfLogic;

    return true;
}

/// @brief 逻辑或AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_or(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];
    Value *    zero = module->newConstInt(0);
    Value *    one = module->newConstInt(1);

    // 逻辑与节点，左结合，先计算左节点，后计算右节点
    // 2. 创建表示结构不同基本块入口的标签
    // 这些标签将在后续指令中被引用（作为跳转目标）
    // 同时，它们本身也是指令，会被添加到线性指令列表中，代表基本块的开始。
    Function * currentFunc = module->getCurrentFunction();

    // 第一个变量为真，检查第二个变量的入口标签
    LabelInstruction * check_nonzero = new LabelInstruction(currentFunc);
    // 假块的入口标签 (如果存在)。如果在 else 块之前创建，可以作为假分支的目标。
    LabelInstruction * store_true = new LabelInstruction(currentFunc);
    LabelInstruction * store_false = new LabelInstruction(currentFunc);

    // 汇合点标签
    LabelInstruction * merge_label = new LabelInstruction(currentFunc);
    // 逻辑与的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    Value * lhs = left->val;
    node->blockInsts.addInst(left->blockInsts);
    ///检查操作数是否是数组，若是需要load
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

        // printf("yes,left\n");
        LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
        lhs = LoadInst;
        lhs->setType(module->findVarValue(left->name)->getType());
        node->blockInsts.addInst(LoadInst);
    }

    auto neqInst1 = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        zero,
        IRInstOperator::IRINST_OP_NEQ_I,
        IRInstOperator::IRINST_OP_NEQ_F,
        true);
    node->blockInsts.addInst(neqInst1);
    ConditionalInstruction * cond_branch_inst1 =
        new ConditionalInstruction(currentFunc, neqInst1, store_true, check_nonzero);
    node->blockInsts.addInst(cond_branch_inst1);
    // 添加标签
    node->blockInsts.addInst(check_nonzero);
    // 逻辑与的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
        // printf("yes,right\n");
        LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
        rhs = LoadInst;
        rhs->setType(module->findVarValue(right->name)->getType());
        node->blockInsts.addInst(LoadInst);
    }

    auto neqInst2 = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        rhs,
        zero,
        IRInstOperator::IRINST_OP_NEQ_I,
        IRInstOperator::IRINST_OP_NEQ_F,
        true);
    node->blockInsts.addInst(neqInst2);

    ConditionalInstruction * cond_branch_inst2 =
        new ConditionalInstruction(currentFunc, neqInst2, store_true, store_false);
    node->blockInsts.addInst(cond_branch_inst2);
    // 添加标签
    node->blockInsts.addInst(store_true);

    // // 2. 对左值做 icmp ne，判断是否为真
    // Value * zero = module->newConstInt(0);
    // Value * cmp1 = new ICmpInstruction(currentFunc, ICmpInstruction::ICMP_NE, lhs_val, zero);
    // node->blockInsts.addInst(cmp1);

    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);

    std::string tmpName = generateTempName("ValueOfLogic");
    Value * ValueOfLogic = module->newVarValueWithInt(IntegerType::getTypeInt(), tmpName, 0, ValueCategory::VARIABLE);
    // ValueOfLogic->setType(left->val->getType());
    StoreInstruction * storeInst1 = new StoreInstruction(module->getCurrentFunction(), ValueOfLogic, one);
    node->blockInsts.addInst(storeInst1);
    // 在 then 块的末尾添加一个无条件跳转到 merge 块的指令。
    // 即使 then 块的最后一条指令本身是一个终止指令（如 return 或 goto），
    // 为了简化生成逻辑，通常还是会添加一个额外的跳转指令。优化阶段可以移除死代码。
    // 使用你提供的 GotoInstruction 类 (它是无条件跳转)。
    node->blockInsts.addInst(new GotoInstruction(currentFunc, merge_label));
    node->blockInsts.addInst(store_false);
    StoreInstruction * storeInst2 = new StoreInstruction(module->getCurrentFunction(), ValueOfLogic, zero);
    node->blockInsts.addInst(storeInst2);

    node->blockInsts.addInst(new GotoInstruction(currentFunc, merge_label));
    node->blockInsts.addInst(merge_label);
    LoadInstruction * LoadInst3 = new LoadInstruction(module->getCurrentFunction(), ValueOfLogic);
    LoadInst3->setType(module->findVarValue(ValueOfLogic->getName())->getType());
    node->blockInsts.addInst(LoadInst3);
    // TODO 逻辑运算是否需要区别int和float型,对float是否作检查，检查代码参考取余部分，

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);

    node->val = ValueOfLogic;

    return true;
}
/// @brief 逻辑表达式生成的临时变量
/// @param 变量名
/// @return 序列号+变量名
std::string IRGenerator::generateTempName(const std::string & base)
{
    static std::unordered_map<std::string, int> nameCounters;
    int                                         count = nameCounters[base]++;
    return base + std::to_string(count);
}

/// @brief 逻辑相等AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_eq(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 逻辑相等节点，左结合，先计算左节点，后计算右节点

    // 左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    // 2. 创建表示结构不同基本块入口的标签
    auto eqInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_EQ_I,
        IRInstOperator::IRINST_OP_EQ_F,
        true);

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(eqInst);
    node->val = eqInst;
    return true;
}

/// @brief 逻辑不等AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_neq(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 节点，左结合，先计算左节点，后计算右节点

    // 左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }
    // 2. 创建表示结构不同基本块入口的标签
    // 这些标签将在后续指令中被引用（作为跳转目标）
    // 同时，它们本身也是指令，会被添加到线性指令列表中，代表基本块的开始。
    Function * currentFunc = module->getCurrentFunction();

    // 真块的入口标签
    LabelInstruction * true_branch_label = new LabelInstruction(currentFunc);
    // 假块的入口标签 (如果存在)。如果在 else 块之前创建，可以作为假分支的目标。
    LabelInstruction * false_branch_target = new LabelInstruction(currentFunc);
    // 汇合点标签
    LabelInstruction * merge_label = new LabelInstruction(currentFunc);

    auto neqInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_NEQ_I,
        IRInstOperator::IRINST_OP_NEQ_F,
        true);

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(neqInst);

    ConditionalInstruction * cond_branch_inst =
        new ConditionalInstruction(currentFunc, neqInst, true_branch_label, false_branch_target);
    node->blockInsts.addInst(cond_branch_inst);
    // 添加标签
    node->blockInsts.addInst(true_branch_label);
    Value * zero = module->newConstInt(0);

    Value *     one = module->newConstInt(1);
    std::string tmpName = generateTempName("ValueOfLogic");
    Value * ValueOfLogic = module->newVarValueWithInt(IntegerType::getTypeInt(), tmpName, 0, ValueCategory::VARIABLE);
    ValueOfLogic->setType(IntegerType::getTypeInt());
    StoreInstruction * storeInst1 = new StoreInstruction(module->getCurrentFunction(), ValueOfLogic, one);
    node->blockInsts.addInst(storeInst1);
    // 在 then 块的末尾添加一个无条件跳转到 merge 块的指令。
    // 即使 then 块的最后一条指令本身是一个终止指令（如 return 或 goto），
    // 为了简化生成逻辑，通常还是会添加一个额外的跳转指令。优化阶段可以移除死代码。
    // 使用你提供的 GotoInstruction 类 (它是无条件跳转)。
    node->blockInsts.addInst(new GotoInstruction(currentFunc, merge_label));
    node->blockInsts.addInst(false_branch_target);
    StoreInstruction * storeInst2 = new StoreInstruction(module->getCurrentFunction(), ValueOfLogic, zero);
    node->blockInsts.addInst(storeInst2);

    node->blockInsts.addInst(new GotoInstruction(currentFunc, merge_label));
    node->blockInsts.addInst(merge_label);
    LoadInstruction * LoadInst3 = new LoadInstruction(module->getCurrentFunction(), ValueOfLogic);
    LoadInst3->setType(module->findVarValue(ValueOfLogic->getName())->getType());
    node->blockInsts.addInst(LoadInst3);
    node->val = neqInst;

    return true;
}

/// @brief 大于等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_ge(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 左结合，先计算左节点，后计算右节点

    // 左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    auto geInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_GE_I,
        IRInstOperator::IRINST_OP_GE_F,
        true);
    node->blockInsts.addInst(geInst);
    node->val = geInst;
    return true;
}

/// @brief 小于等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_le(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 左结合，先计算左节点，后计算右节点

    // 左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }
    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    auto leInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_LE_I,
        IRInstOperator::IRINST_OP_LE_F,
        true);

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(leInst);

    node->val = leInst;

    return true;
}

/// @brief 大于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_gne(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 左结合，先计算左节点，后计算右节点

    // 左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    auto gneInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_GNE_I,
        IRInstOperator::IRINST_OP_GNE_F,
        true);

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    // node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(gneInst);
    node->val = gneInst;

    return true;
}

/// @brief 小于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_lne(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 左结合，先计算左节点，后计算右节点

    // 左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }
    ///检查操作数是否是数组，若是需要load
    node->blockInsts.addInst(left->blockInsts);
    Value * lhs = left->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (left->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {

    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), left->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(left->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    node->blockInsts.addInst(right->blockInsts);
    Value * rhs = right->val;
    // std::cout << "right type: " << rhs->getType()->toString() << std::endl;
    // std::cout << "right string: " << rhs->getIRName() << std::endl;
    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     rhs = LoadInst;
    //     rhs->setType(module->findVarValue(right->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }

    // 操作数不同时进行类型转换
    if (left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) {
        if ((left->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (right->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), lhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            lhs = castInst;
        }
        if ((right->val->getType()->getTypeID() == Type::IntegerTyID) &&
            (left->val->getType()->getTypeID() == Type::FloatTyID)) {
            CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), rhs, FloatType::getType());
            node->blockInsts.addInst(castInst);
            rhs = castInst;
        }
    }

    auto lneInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        rhs,
        IRInstOperator::IRINST_OP_LNE_I,
        IRInstOperator::IRINST_OP_LNE_F,
        true);
    node->blockInsts.addInst(lneInst);
    node->val = lneInst;
    return true;
}

/// @brief 单目正号AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_pos(ast_node * node)
{
    ast_node * son_node = node->sons[0]; // +x 的 x

    // 访问子表达式
    ast_node * expr = ir_visit_ast_node(son_node);
    if (!expr) {
        return false;
    }

    node->blockInsts.addInst(expr->blockInsts);
    Value * lhs = expr->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    if (expr->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
        // printf("yes,left\n");
        LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), expr->val);
        lhs = LoadInst;
        lhs->setType(module->findVarValue(expr->name)->getType());
        node->blockInsts.addInst(LoadInst);
    }

    // 设置当前节点的计算结果
    node->val = lhs;

    return true;
}

/// @brief 单目负号AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_neg(ast_node * node)
{
    ast_node * son_node = node->sons[0]; // -x 的 x

    // 访问子表达式
    ast_node * expr = ir_visit_ast_node(son_node);
    if (!expr) {
        return false;
    }

    node->blockInsts.addInst(expr->blockInsts);
    Value * rhs = expr->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (expr->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), expr->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(expr->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }
    //
    Value * zero;

    if (rhs->getType()->isIntegerType()) {
        zero = module->newConstInt(0);
    } else {
        zero = module->newConstFloat(0.0);
    }
    // 生成IR指令：result = -expr->val
    auto negInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        zero,
        rhs,
        IRInstOperator::IRINST_OP_NEG_I,
        IRInstOperator::IRINST_OP_NEG_F);

    // 合并子表达式的IR并加入当前指令
    // node->blockInsts.addInst(expr->blockInsts);
    node->blockInsts.addInst(negInst);

    // 设置当前节点的计算结果
    node->val = negInst;

    return true;
}

/// @brief 逻辑非AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_not(ast_node * node)
{
    ast_node * son_node = node->sons[0];

    // 访问子表达式
    ast_node * expr = ir_visit_ast_node(son_node);
    if (!expr) {
        return false;
    }
    node->blockInsts.addInst(expr->blockInsts);
    Value * lhs = expr->val;
    // std::cout << "left type: " << lhs->getType()->toString() << std::endl;
    // if (expr->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,left\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), expr->val);
    //     lhs = LoadInst;
    //     lhs->setType(module->findVarValue(expr->name)->getType());
    //     node->blockInsts.addInst(LoadInst);
    // }
    Value * zero;

    if (lhs->getType()->isIntegerType()) {
        zero = module->newConstInt(0);
    } else {
        zero = module->newConstFloat(0.0);
    }

    auto cmpInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        lhs,
        zero,
        IRInstOperator::IRINST_OP_NEQ_I,
        IRInstOperator::IRINST_OP_NEQ_F,
        true);
    node->blockInsts.addInst(cmpInst);

    Value * one = module->newConstInt(1);
    // 生成IR指令：result = -expr->val
    auto notInst = BinaryInstruction::createAutoTyped(
        module->getCurrentFunction(),
        cmpInst,
        one,
        IRInstOperator::IRINST_OP_NOT_I,
        IRInstOperator::IRINST_OP_NOT_F,
        true);

    // 合并子表达式的IR并加入当前指令
    // node->blockInsts.addInst(expr->blockInsts);
    node->blockInsts.addInst(notInst);
    notInst->setType(IntegerType::getTypeBool());
    CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), notInst, IntegerType::getTypeInt());
    node->blockInsts.addInst(castInst);
    // 设置当前节点的计算结果
    node->val = castInst;

    return true;
}

/// @brief 赋值AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_assign(ast_node * node)
{
    ast_node * son1_node = node->sons[0];
    ast_node * son2_node = node->sons[1];

    // 赋值节点，自右往左运算

    // 赋值运算符的左侧操作数
    ast_node * left = ir_visit_ast_node(son1_node);
    if (!left) {
        // 某个变量没有定值
        // 这里缺省设置变量不存在则创建，因此这里不会错误
        return false;
    }

    // 赋值运算符的右侧操作数
    ast_node * right = ir_visit_ast_node(son2_node);
    if (!right) {
        // 某个变量没有定值
        printf("Assign: some variables have no values.\n");
        return false;
    }

    Value * temp = module->findVarValue(left->name);

    if (temp->getValueCategory() != ValueCategory::VARIABLE) {
        minic_log(LOG_ERROR, "第%lld行的(%s)为常量，不允许赋值", (long long) node->line_no, left->name.c_str());
        return false;
    }

    node->blockInsts.addInst(right->blockInsts);

    ///检查右值是否是数组，若是需要load
    Value * Roperand = right->val;

    /// 检查类型是否匹配，若不匹配，插入类型转换指令,
    /// 指针和它的指向类型不匹配时，进行强制转换
    if ((left->val->getType()->getTypeID() != right->val->getType()->getTypeID()) &&
        (left->val->getType()->getPointeeType()->getTypeID() != right->val->getType()->getTypeID())) {
        // int -> float 强制转换
        CastInstruction * castInst = new CastInstruction(module->getCurrentFunction(), Roperand, left->val->getType());
        node->blockInsts.addInst(castInst);
        Roperand = castInst;
    }

    // if (right->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
    //     // printf("yes,right\n");
    //     LoadInstruction * LoadInst = new LoadInstruction(module->getCurrentFunction(), right->val);
    //     Roperand = LoadInst;
    //     node->blockInsts.addInst(LoadInst);
    // }

    ///检查右值
    node->blockInsts.addInst(left->blockInsts);
    StoreInstruction * storeInst = new StoreInstruction(module->getCurrentFunction(), left->val, Roperand);
    node->blockInsts.addInst(storeInst);
    node->val = storeInst;

    return true;
}

/// @brief return节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_return(ast_node * node)
{
    ast_node * right = nullptr;

    // return语句可能没有没有表达式，也可能有，因此这里必须进行区分判断
    if (!node->sons.empty()) {

        ast_node * son_node = node->sons[0];

        // 返回的表达式的指令保存在right节点中
        right = ir_visit_ast_node(son_node);
        if (!right) {

            // 某个变量没有定值
            printf("Return: some variables have no values.\n");
            return false;
        }
    }

    Function * currentFunc = module->getCurrentFunction();

    // 返回值存在时则移动指令到node中
    if (right) {

        // 创建临时变量保存IR的值，以及线性IR指令
        node->blockInsts.addInst(right->blockInsts);

        // 返回值赋值到函数返回值变量上，然后跳转到函数的尾部
        // node->blockInsts.addInst(new MoveInstruction(currentFunc, currentFunc->getReturnValue(), right->val));
        node->blockInsts.addInst(new StoreInstruction(
            currentFunc,
            currentFunc->getReturnValue(),
            right->val)); // 将返回值存储到函数的返回值变量中

        // auto * loadInst = new LoadInstruction(currentFunc,
        //                                       right->val); // 加载返回值变量的值到当前节点
        // node->blockInsts.addInst(loadInst);                // 加载返回值变量的值到当前节点

        // auto * returnvar = new LoadInstruction(currentFunc, currentFunc->getReturnValue());
        node->val = right->val;
        // node->val = right->val;                  // 设置当前节点的值为函数返回值变量
        // currentFunc->setReturnValue(right->val); // 更新函数的返回值为加载后的值

        // TODO:返回值类型检查

    } else {
        // 没有返回值
        node->val = nullptr;
        // node->blockInsts.addInst(new ExitInstruction(currentFunc, nullptr)); // 添加返回指令
    }

    // 跳转到函数的尾部出口指令上
    // node->blockInsts.addInst(new GotoInstruction(currentFunc, currentFunc->getExitLabel()));

    return true;
}

bool IRGenerator::ir_ifelse(ast_node * node)
{
    // AST 节点结构:
    // node->sons[0] 是条件表达式
    // node->sons[1] 是 if 语句块
    // node->sons[2] 是 else 语句块 (可选)
    ast_node * cond_node = node->sons[0];
    ast_node * if_node = node->sons[1];
    ast_node * else_node = (node->sons.size() > 2) ? node->sons[2] : nullptr;
    bool       hasBreakContinueInIf = false;   // 防止if break continue标签与merge重复
    bool       hasBreakContinueInElse = false; // 防止else break continue标签与merge重复

    // 检测if子结点有没有break
    for (auto node_in_if: if_node->sons) {
        if (node_in_if->node_type == ast_operator_type::AST_OP_BREAK ||
            node_in_if->node_type == ast_operator_type::AST_OP_CONTINUE) {
            hasBreakContinueInIf = true;
            break;
        }
    }

    // 获取当前函数，if块必须位于函数内
    Function * currentFunc = module->getCurrentFunction();
    if (!currentFunc) {
        std::cerr << "Error: If-else outside function." << std::endl;
        return false;
    }

    // 1. 生成条件表达式的IR
    // ir_visit_ast_node 会递归访问子节点并生成其IR。
    // 生成的指令存储在 cond->blockInsts，结果值存储在 cond->val 中。
    // ast_node * cond = ir_visit_ast_node(cond_node);
    // if (!cond) {
    //     printf("Ifelse: no condition block\n");
    //     return false;
    // }

    // 将条件表达式生成的指令添加到当前节点的指令列表中。
    // 这些指令将构成 if-else 结构前导基本块的一部分。
    // node->blockInsts.addInst(cond->blockInsts);

    // 获取条件表达式的值 (应为一个布尔值，如 IR 中的 i1 类型)
    // Value * cond_val = cond->val;
    // if (!cond_val) {
    //     printf("Ifelse: condition has no value.\n");
    //     return false;
    // }

    // 2. 创建表示 if-else 结构不同基本块入口的标签
    // 这些标签将在后续指令中被引用（作为跳转目标）
    // 同时，它们本身也是指令，会被添加到线性指令列表中，代表基本块的开始。

    // if 块的入口标签
    LabelInstruction * true_branch_label = new LabelInstruction(currentFunc);
    // else 块的入口标签 (如果存在)。如果在 else 块之前创建，可以作为假分支的目标。
    LabelInstruction * else_label = nullptr;
    // if-else 结构结束后的汇合点标签
    LabelInstruction * merge_label = new LabelInstruction(currentFunc);

    // 确定条件分支的假分支目标
    // 如果有 else 块，假分支跳到 else 块的标签
    // 如果没有 else 块，假分支跳到 merge 块的标签
    LabelInstruction * false_branch_target = nullptr;
    if (else_node) {
        else_label = new LabelInstruction(currentFunc); // 创建 else 块的实际标签
        false_branch_target = else_label;
        // 检测else中是否有break标签
        for (auto node_in_else: else_node->sons) {
            if (node_in_else->node_type == ast_operator_type::AST_OP_BREAK ||
                node_in_else->node_type == ast_operator_type::AST_OP_CONTINUE) {
                hasBreakContinueInElse = true;
                break;
            }
        }
    } else {
        false_branch_target = merge_label;
    }

    // 3. 添加条件分支指令 (br i1)
    // 这个指令紧跟在条件表达式指令之后，根据 cond_val 的布尔值决定跳转。
    // if (!cond_val->getType()->isInt1Byte()) {
    //     auto castToI1 = new CastInstruction(currentFunc, cond_val, IntegerType::getTypeBool());
    //     node->blockInsts.addInst(castToI1);
    //     cond_val = castToI1;
    // }
    // ConditionalInstruction * cond_branch_inst =
    //     new ConditionalInstruction(currentFunc, cond_val, true_branch_label, false_branch_target);
    // node->blockInsts.addInst(cond_branch_inst);

    // 支持短路，添加条件分支指令。区别于遍历ir_and/or，因为其会产生额外的一个Value ValueOfLogic
    if (!gen_condition_branch(cond_node, true_branch_label, false_branch_target, node->blockInsts)) {
        // Error occurred during condition branching generation
        std::cerr << "Error generating condition branch for if-else." << std::endl;
        // TODO: Add location info
        return false;
    }
    // 前导基本块（包含条件求值和条件分支）的指令已生成并添加到 node->blockInsts。
    // 接下来生成 then 块、else 块和 merge 块的指令，并按顺序添加到 node->blockInsts。

    // 4. 生成 if块 的IR
    // 添加 if 块的标签
    node->blockInsts.addInst(true_branch_label);

    // 访问 if 语句块 AST 节点。生成其内部指令
    ast_node * ifBlock = ir_visit_ast_node(if_node);
    if (!ifBlock) {
        // if 块生成失败
        // 注意：即使 if 块为空（例如 `{}`），ir_visit_ast_node 也应该成功，返回一个 blockInsts 为空的节点。
        printf("if block generate failed.\n");
        return false;
    }
    // 将 if 块生成的指令添加到当前节点的指令列表中。
    node->blockInsts.addInst(ifBlock->blockInsts);

    // 在 then 块的末尾添加一个无条件跳转到 merge 块的指令。
    // 即使 then 块的最后一条指令本身是一个终止指令（如 return 或 goto），
    // 为了简化生成逻辑，通常还是会添加一个额外的跳转指令。优化阶段可以移除死代码。
    // 使用你提供的 GotoInstruction 类 (它是无条件跳转)。
    if (!hasBreakContinueInIf) {
        node->blockInsts.addInst(new GotoInstruction(currentFunc, merge_label));
    }

    // 5. 生成 else 块的IR (如果存在)
    if (else_node) {
        // 添加 else 块的标签。这标志着 else 基本块的开始。
        // 注意：else_label 就是之前为 false_branch_target 创建的标签。
        node->blockInsts.addInst(else_label);

        // 访问 else 语句块 AST 节点。生成其内部指令。
        ast_node * elseBlock = ir_visit_ast_node(else_node);
        if (!elseBlock) {
            // else 块生成失败
            printf("else block generate failed.\n");
            return false;
        }
        // 将 else 块生成的指令添加到当前节点的指令列表中。
        node->blockInsts.addInst(elseBlock->blockInsts);

        // 在 else 块的末尾添加一个无条件跳转到 merge 块的指令。
        // 同 then 块，即使 else 块的最后一条指令本身是终止指令，也添加一个跳转。
        if (!hasBreakContinueInElse) {
            node->blockInsts.addInst(new GotoInstruction(currentFunc, merge_label));
        }
    }

    // 6. 添加 merge 块的标签
    // 这是 if-else 结构之后所有代码开始的地方。then 块和 else 块（如果存在）都会跳转到这里。
    node->blockInsts.addInst(merge_label);

    // if-else 语句本身不产生值，所以 node->val 保持 nullptr。

    return true;
}

bool IRGenerator::ir_while(ast_node * node)
{
    // AST 节点结构 (SysY while 通常是两个子节点):
    // node->sons[0] 是条件表达式AST节点
    // node->sons[1] 是循环体AST节点

    ast_node * cond_node = node->sons[0]; // 条件表达式AST节点
    ast_node * body_node = node->sons[1]; // 循环体AST节点
    bool       hasBreakContinue = false;
    // 防止break标签和while merge重复
    for (auto nodeInWhile: body_node->sons) {
        if (nodeInWhile->node_type == ast_operator_type::AST_OP_BREAK ||
            nodeInWhile->node_type == ast_operator_type::AST_OP_CONTINUE) {
            hasBreakContinue = true; // 检测循环体中是否有break语句
            break;
        }
    }
    Function * currentFunc = module->getCurrentFunction(); // 获取当前函数

    // 1. 创建表示循环不同部分的标签
    // 循环头部标签 (用于条件求值和分支，也是从循环体跳回的目标)
    LabelInstruction * loop_header_label = new LabelInstruction(currentFunc);
    // 循环体标签 (条件为真时跳转到的目标)
    LabelInstruction * loop_body_label = new LabelInstruction(currentFunc);
    // 循环出口标签 (条件为假时跳转到的目标，循环结束后的代码入口)
    LabelInstruction * loop_exit_label = new LabelInstruction(currentFunc);

    enterLabels.push(loop_header_label); // 记录循环头部标签
    exitLabels.push(loop_exit_label);    // 记录循环出口标签
    // 2. 添加一个无条件跳转到循环头部标签的指令
    // 这是为了确保在执行 while 循环逻辑之前，先进入循环头部块。
    // 如果 while 语句是基本块的第一个语句，这个跳转可能是多余的（优化时会移除），
    // 但为了生成正确的控制流，通常会添加。
    // 或者，另一种结构是直接把 loop_header_label 作为当前 blockInsts 的第一个指令，
    // 表示当前的基本块就直接是循环头部块。这里采用先添加跳转，再添加标签的方式，
    // 这样 while 语句之前的指令和 while 语句的IR生成是分离的。

    node->blockInsts.addInst(new GotoInstruction(currentFunc, loop_header_label));

    // 3. 生成循环头部块 (条件求值和条件分支)
    // 添加循环头部标签，标记这个基本块的开始
    node->blockInsts.addInst(loop_header_label);

    // // 访问条件表达式AST节点，生成其IR
    // ast_node * cond = ir_visit_ast_node(cond_node);
    // if (!cond) {
    //     // 条件表达式生成失败
    //     enterLabels.pop();
    //     exitLabels.pop();
    //     printf("While: Condition express generate failed.\n");
    //     return false;
    // }
    // // 将条件表达式生成的指令添加到当前节点的指令列表中 (属于循环头部块)
    // node->blockInsts.addInst(cond->blockInsts);

    // // 获取条件表达式的值 (应为一个布尔值，i1 类型)
    // Value * cond_val = cond->val;
    // if (!cond_val) {
    //     // 条件表达式必须产生一个值
    //     enterLabels.pop();
    //     exitLabels.pop();
    //     printf("While: no value for condition expression\n");
    //     return false; // 或者更详细的错误处理
    // }

    // // 添加条件分支指令 (br i1)
    // // 如果条件为真 (cond_val)，跳转到 loop_body_label
    // // 如果条件为假 (!cond_val)，跳转到 loop_exit_label
    // ConditionalInstruction * cond_branch_inst =
    //     new ConditionalInstruction(currentFunc, cond_val, loop_body_label, loop_exit_label);
    // node->blockInsts.addInst(cond_branch_inst);

    // 支持短路
    if (!gen_condition_branch(cond_node, loop_body_label, loop_exit_label, node->blockInsts)) {
        // Error occurred during condition branching generation
        std::cerr << "Error generating condition branch for while." << std::endl;
        return false;
    }

    // 4. 生成循环体块
    // 添加循环体标签，标记这个基本块的开始
    node->blockInsts.addInst(loop_body_label);

    // 访问循环体AST节点，生成其IR
    ast_node * body = ir_visit_ast_node(body_node);
    if (!body) {
        // 循环体生成失败
        // 注意：即使循环体为空（例如 `{}`），ir_visit_ast_node 也应该成功，返回一个 blockInsts 为空的节点。
        enterLabels.pop();
        exitLabels.pop();
        printf("While: Body expression error\n");
        return false;
    }
    // 将循环体生成的指令添加到当前节点的指令列表中
    node->blockInsts.addInst(body->blockInsts);

    // 在循环体块的末尾添加一个无条件跳转回循环头部标签的指令
    // 这是循环的关键，完成一次迭代后回到头部检查条件。
    // 使用你提供的 GotoInstruction 类。
    if (!hasBreakContinue) {
        node->blockInsts.addInst(new GotoInstruction(currentFunc, loop_header_label));
    }

    // 5. 生成循环出口块
    // 添加循环出口标签。这标志着循环结束后的基本块的开始。
    node->blockInsts.addInst(loop_exit_label);

    // === 从栈中弹出循环头部and出口标签，表示离开当前循环的作用域 ===
    enterLabels.pop();
    exitLabels.pop();
    // while 语句本身不产生值，所以 node->val 保持 nullptr。

    return true;
}

bool IRGenerator::ir_break(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction(); // 获取当前函数

    // 1. 检查是否在循环内部
    if (exitLabels.empty()) {
        // break 语句在循环外部，这是一个编译错误
        // TODO: 更好的错误报告机制，包括位置信息
        printf("Break: break statement outside of loop.");
        return false; // 返回 false 表示生成失败
    }

    // 2. 获取当前最内层循环的出口标签 (栈顶元素)
    LabelInstruction * exit_label = exitLabels.top(); // vector 的 back() 获取栈顶

    // 3. 创建无条件跳转指令，跳转到循环出口标签
    Instruction * break_inst = new GotoInstruction(currentFunc, exit_label);

    // 4. 将跳转指令添加到当前节点的指令列表中
    node->blockInsts.addInst(break_inst);

    // break 语句本身不产生值
    node->val = nullptr; // 确保 val 为 nullptr

    // break 指令终止了当前的基本块，后续指令理论上不可达。
    // 即使后续有指令，优化器会清理。在线性 IR 生成时，只添加 break 跳转指令即可。

    return true;
}

bool IRGenerator::ir_continue(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction(); // 获取当前函数

    // 1. 检查是否在循环内部
    // continue 语句只在循环内部有效，检查 loop_continue_labels_ 栈
    if (enterLabels.empty()) {
        // continue 语句在循环外部，这是一个编译错误
        printf("Continue: continue statement outside of loop.");
        return false; // 返回 false 表示生成失败
    }

    // 2. 获取当前最内层循环的头部标签 (loop_continue_labels_ 栈顶元素)
    LabelInstruction * header_label = enterLabels.top(); // vector 的 back() 获取栈顶

    // 3. 创建无条件跳转指令，跳转到循环头部标签
    Instruction * continue_inst = new GotoInstruction(currentFunc, header_label);

    // 4. 将跳转指令添加到当前节点的指令列表中
    node->blockInsts.addInst(continue_inst);

    // continue 语句本身不产生值
    node->val = nullptr; // 确保 val 为 nullptr

    // continue 指令也终止了当前的基本块

    return true;
}

/// @brief 类型叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_type(ast_node * node)
{
    // 不需要做什么，直接从节点中获取即可。

    return true;
}

/// @brief 标识符叶子节点翻译成线性中间IR，变量声明的不走这个语句
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_var_id(ast_node * node)
{
    Value * val;

    // 查找ID型Value
    // 变量，则需要在符号表中查找对应的值

    val = module->findVarValue(node->name);
    Type * type = val->getType();
    // std::cout << "the type in this var is " << type->toString() << std::endl;
    //  std::cout << " " << node->is_lvar << type->isArrayType() << 'and ' << type->getTypeID() << std::endl;
    if (node->is_lvar || type->isArrayType()) {
        node->val = val;
    } else {
        if (type->isFloatType() || type->isIntegerType()) {

            auto LoadInst = new LoadInstruction(module->getCurrentFunction(), val);
            node->val = LoadInst;
            // std::cout << node->getNodeName() << std::endl;
            // std::cout << "var name " << node->getNodeName() << std::endl;
            node->val->setName(node->name); // 设置名称
                                            // node->val->setIRName(std::string _name)
            node->blockInsts.addInst(LoadInst);
        } else {
            std::cout << 'here' << std::endl;
            PointerType * pointerType = new PointerType(type);

            val->setType(pointerType);
            std::cout << "the point type" << pointerType->toString() << std::endl;
            std::cout << node->getNodeName() << std::endl;
            auto loadInst = new LoadInstruction(module->getCurrentFunction(), val);
            node->val = loadInst;
            node->blockInsts.addInst(loadInst);

            val->setType(type); // 还原
        }
    }

    return true;
}

/// @brief 无符号整数字面量叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_uint(ast_node * node)
{
    ConstInt * val;

    // 新建一个整数常量Value
    val = module->newConstInt((int32_t) node->integer_val);

    node->val = val;

    return true;
}

/// @brief float数字面量叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_float(ast_node * node)
{
    ConstFloat * val;

    // 新建一个浮点数常量Value
    val = module->newConstFloat(node->float_val);

    node->val = val;

    return true;
}

// bool IRGenerator::ir_array_access(ast_node * node)
// {

//     // 是数组变量，提取数组名和维度表达式
//     std::string             array_name;
//     std::vector<ast_node *> array_dims;
//     extract_array_info(node, array_name, array_dims);

//     ///设置name，否则作为左值会报错
//     node->name = array_name;
//     // 解析维度表达式为实际的常数
//     // std::vector<int> dims;
//     // for (auto * expr_node: array_dims) {
//     //     int dim_size = evaluateConstExpr(expr_node); // 假设此函数返回维度大小
//     //     dims.push_back(dim_size);
//     // }

//     ///使用tempVal获取之前生成的节点
//     Value * tempVal = module->findVarValue(array_name);
//     ///获取定义的时候，声明数组各维度
//     Type * type = tempVal->getType();
//     if (type->isArrayType()) {
//         auto *           arrayType = static_cast<ArrayType *>(type);
//         std::vector<int> ori_dims = arrayType->getDimensions();
//         // int              offset_size = calcOffset(ori_dims, dims);
//         //  int              offset = offset_size * 4;
//         int d = ori_dims.size();
//         int m = array_dims.size();

//         // 从后往前构造偏移表达式
//         Value * offset = nullptr;
//         Value * stride = module->newConstInt(1); // 初始stride=1

//         for (int i = d - 1; i >= d - m; --i) {
//             ast_node * expr_node = ir_visit_ast_node(array_dims[i - (d - m)]);

//             // 生成子表达式的 IR
//             Value * indexVal = expr_node->val;
//             node->blockInsts.addInst(expr_node->blockInsts);
//             //  tmp = indexVal * stride
//             auto term = new BinaryInstruction(
//                 module->getCurrentFunction(),
//                 IRInstOperator::IRINST_OP_MUL_I,
//                 indexVal,
//                 stride,
//                 IntegerType::getTypeInt());
//             node->blockInsts.addInst(term);

//             // offset = offset + term
//             if (offset == nullptr) {
//                 offset = term;
//             } else {
//                 auto sum = new BinaryInstruction(
//                     module->getCurrentFunction(),
//                     IRInstOperator::IRINST_OP_ADD_I,
//                     offset,
//                     term,
//                     IntegerType::getTypeInt());
//                 node->blockInsts.addInst(sum);
//                 offset = sum;
//             }
//             if (i - 1 >= d - m) {
//                 // 更新stride *= ori_dims[i]
//                 auto new_stride = new BinaryInstruction(
//                     module->getCurrentFunction(),
//                     IRInstOperator::IRINST_OP_MUL_I,
//                     stride,
//                     module->newConstInt(ori_dims[i]),
//                     IntegerType::getTypeInt());
//                 stride = new_stride;
//                 node->blockInsts.addInst(new_stride);
//             }
//         }
//         auto offest_size = new BinaryInstruction(
//             module->getCurrentFunction(),
//             IRInstOperator::IRINST_OP_MUL_I,
//             offset,
//             module->newConstInt(4),
//             IntegerType::getTypeInt());
//         node->blockInsts.addInst(offest_size);

//         auto addr = new BinaryInstruction(
//             module->getCurrentFunction(),
//             IRInstOperator::IRINST_OP_ADD_I,
//             tempVal,
//             offest_size,
//             IntegerType::getTypeInt());
//         node->val = addr;
//         // ///需要手动设置Type，否则addr默认是int类型的value
//         // node->val->setType(type);
//         // std::cout << "addr type: " << addr->getType()->toString() << std::endl;
//         node->blockInsts.addInst(addr);

//     } else {
//         // 处理错误情况
//         std::cerr << "Array access: Error: Expected an array type." << std::endl;
//         return false;
//     }

//     return true;
// }

///@brief 这个版本为zjl版本的LLVm
// bool IRGenerator::ir_array_access(ast_node * node)
// {
//     std::string             array_name;
//     std::vector<ast_node *> array_dims;
//     extract_array_info(node, array_name, array_dims);

//     node->name = array_name;

//     Value * tempVal = module->findVarValue(array_name);
//     Type *  type = tempVal->getType();
//     if (!type->isArrayType()) {
//         std::cerr << "Array access: Error: Expected an array type." << std::endl;
//         return false;
//     }

//     // auto * arrayType = static_cast<ArrayType *>(type);
//     // int    totalDims = arrayType->getDimensions().size();
//     int accessDims = array_dims.size();

//     // 构造 gep 索引：{i64 0, i64 idx1, i64 idx2, ...}
//     std::vector<Value *> indices;
//     indices.push_back(module->newConstInt(0)); // 第一个是0，表示起始

//     Type * currType = type;
//     for (int i = 0; i < accessDims; ++i) {
//         ast_node * idxNode = ir_visit_ast_node(array_dims[i]);
//         Value *    indexVal = idxNode->val;

//         node->blockInsts.addInst(idxNode->blockInsts);
//         indices.push_back(indexVal);

//         // 更新类型为当前维度的元素类型
//         if (currType->isArrayType())
//             currType = static_cast<ArrayType *>(currType)->getElementType();
//     }

//     // 构造 getelementptr 指令
//     auto gepInst = new GetElementPtrInst(
//         module->getCurrentFunction(),
//         tempVal,  // 数组变量
//         type,     // 原始数组类型（如 [5 x [6 x i32]]）
//         indices); // 多级索引

//     node->blockInsts.addInst(gepInst);
//     node->val = gepInst;
//     node->val->setType(currType); // 设置为最终指向类型（如 i32*）

//     return true;
// }

///@brief 这个版本为标准版本的LLVm
bool IRGenerator::ir_array_access(ast_node * node)
{
    std::string             array_name;
    std::vector<ast_node *> array_dims;
    extract_array_info(node, array_name, array_dims);

    node->name = array_name;

    Value * tempVal = module->findVarValue(array_name);
    Type *  type = tempVal->getType();
    std::cout << " type  " << type->toString() << std::endl;
    if ((!type->isArrayType()) && (!type->isPointerType())) {
        std::cerr << "Array access: Error: Expected an array type." << std::endl;
        return false;
    }

    int accessDims = array_dims.size();

    // 起始指针
    Value *           gepPtr = tempVal;
    Type *            gepType = type;
    LoadInstruction * loadInst = nullptr;
    if (type->isPointerType()) {
        //如果是指针类型，直接获取指向的类型
        PointerType * pointerType = new PointerType(type);

        gepPtr->setType(pointerType);
        std::cout << "the point type" << pointerType->toString() << std::endl;
        loadInst = new LoadInstruction(module->getCurrentFunction(), gepPtr);
        node->blockInsts.addInst(loadInst);

        tempVal->setType(type); // 还原
        gepPtr = loadInst;      // 更新 gepPtr 为加载后的值
                                ///
        // gepPtr->setType(loadInst->getType()->getPointeeType())                   //
        gepType = gepPtr->getType(); // 更新 gepType 为加载后的类型
                                     // 获取指向的类型
        std::cout << "gep type in point: " << gepType->toString() << std::endl;
    }
    // 逐层调用getelementptr
    for (int i = 0; i < accessDims; ++i) {
        // 先处理索引表达式，转换成Value*
        ast_node * idxNode = ir_visit_ast_node(array_dims[i]);
        Value *    indexVal = idxNode->val;
        node->blockInsts.addInst(idxNode->blockInsts);

        // getelementptr第一个索引固定为0
        Value * zero = module->newConstInt(0);

        // 生成getelementptr指令：
        // 类型：gepType是当前的数组类型，如 [5 x [6 x i32]] 或 [6 x i32]
        // 返回的类型是当前维度元素的指针类型，比如 [6 x i32]* 的元素是 i32
        if (type->isPointerType()) {
            // 如果是指针类型，只取一维
            auto gepInst =
                new GetElementPtrInst(module->getCurrentFunction(), gepPtr, gepType, std::vector<Value *>{indexVal});
            node->blockInsts.addInst(gepInst);

            gepPtr = gepInst;
        } else {
            auto gepInst = new GetElementPtrInst(
                module->getCurrentFunction(),
                gepPtr,
                gepType,
                std::vector<Value *>{zero, indexVal});

            node->blockInsts.addInst(gepInst);

            gepPtr = gepInst;
        }

        // 更新类型为下一维
        if (gepType->isArrayType()) {
            auto * arrTy = static_cast<ArrayType *>(gepType);
            gepType = arrTy->getElementType();
        } else if (gepType->isPointerType()) {
            // PointerType * pointerType = new PointerType(gepType->getPointeeType()->getPointeeType());
            PointerType * pointeeType =
                const_cast<PointerType *>(static_cast<const PointerType *>(gepType->getPointeeType()));
            gepType = pointeeType;
        } else {
            // 否则，不能进一步推进，退出
            break;
        }
    }
    std::cout << "gep type: " << gepType->toString() << std::endl;
    std::cout << "gepPTR type: " << gepPtr->getType()->toString() << std::endl;
    if (node->is_lvar) {
        node->val = gepPtr;
        if (type->isPointerType()) {
            node->val->setType(gepType); // 设置最后的类型，应该是元素指针类型
        }
    } else {
        gepPtr->setType(gepType); // 设置 gepPtr 的类型为最终的元素指针类型
        auto loadInst = new LoadInstruction(module->getCurrentFunction(), gepPtr);
        node->blockInsts.addInst(loadInst);
        node->val = loadInst; // 设置为加载后的值
    }

    std::cout << "final type: " << node->val->getType()->toString() << std::endl;
    // node->val->setType(gepType); // 设置最后的类型，应该是元素指针类型

    return true;
}

Value * IRGenerator::funcall_array_access(ast_node * node)
{

    // 是数组变量，提取数组名和维度表达式
    std::string             array_name;
    std::vector<ast_node *> array_dims;
    extract_array_info(node, array_name, array_dims);

    ///设置name，否则作为左值会报错
    node->name = array_name;

    /// 使用tempVal获取之前生成的节点
    Value * tempVal = module->findVarValue(array_name);
    if (!tempVal) {
        std::cerr << "Function call - array: Cannot find array!" << std::endl;
    }
    Type * type = tempVal->getType();
    std::cout << "array type  " << type->toString() << std::endl;
    if (type->isArrayType()) {
        int accessDims = array_dims.size();
        int ori_dims = static_cast<ArrayType *>(type)->getDimensions().size();
        std::cout << "array access dims: " << accessDims << std::endl;
        // 起始指针
        Value * gepPtr = tempVal;

        Type * gepType = type;
        if (accessDims == 0) {
            // getelementptr 0, 0
            Value * zero = module->newConstInt(0);

            // GEP 获取数组首地址，模仿数组 decay 成指针的行为
            auto gepInst = new GetElementPtrInst(
                module->getCurrentFunction(),
                tempVal,                           // 原始数组变量 Value*，类型如 [5 x i32]*
                gepType,                           // 类型是 [5 x i32]*
                std::vector<Value *>{zero, zero}); // GEP 0, 0 => 获取 a[0]

            node->blockInsts.addInst(gepInst);
            node->val = gepInst;
            std::cout << "array type: " << node->val->getType()->toString() << std::endl;
            return node->val; // 返回最终的 gep 指令 Value*
        } else if (accessDims == ori_dims) {
            // 逐层调用getelementptr
            // std::cout << "there is a array in function call  " << node->getNodeName() << std::endl;
            for (int i = 0; i < accessDims; ++i) {
                // 先处理索引表达式，转换成Value*
                ast_node * idxNode = ir_visit_ast_node(array_dims[i]);
                Value *    indexVal = idxNode->val;
                node->blockInsts.addInst(idxNode->blockInsts);
                // getelementptr第一个索引固定为0
                Value * zero = module->newConstInt(0);

                // 生成getelementptr指令：
                // 类型：gepType是当前的数组类型，如 [5 x [6 x i32]] 或 [6 x i32]
                // 返回的类型是当前维度元素的指针类型，比如 [6 x i32]* 的元素是 i32
                auto gepInst = new GetElementPtrInst(
                    module->getCurrentFunction(),
                    gepPtr,
                    gepType,
                    std::vector<Value *>{zero, indexVal});
                node->blockInsts.addInst(gepInst);
                // 更新类型为下一维
                if (gepType->isArrayType()) {

                    // auto * arrTy = static_cast<ArrayType *>(gepType);
                    gepPtr = gepInst;                    // 更新起始指针为当前的 gep 指令
                    gepType = gepType->getElementType(); // 更新 gepType 为当前的元素类型
                    gepPtr->setType(gepType);            // 设置 gepPtr 的类型为最终的元素指针类型
                }
                // arrayPRParam = gepInst;
            }
            PointerType * pointeeType =
                const_cast<PointerType *>(static_cast<const PointerType *>(gepType->getPointeeType()));
            gepType = pointeeType;
            gepPtr->setType(gepType); // 设置 gepPtr 的类型为最终的元素指针类型
            auto * loadInst = new LoadInstruction(module->getCurrentFunction(), gepPtr); // 加载最终的指针值
            node->blockInsts.addInst(loadInst);
            node->val = loadInst; // 设置最终的值为加载后的指针
            std::cout << "array type: " << node->val->getType()->toString() << std::endl;
            return node->val; // 返回最终的加载指令 Value*
        } else {
            //说明是多重指针
            // 逐层调用getelementptr
            // std::cout << "there is a array in function call  " << node->getNodeName() << std::endl;
            for (int i = 0; i < accessDims; ++i) {
                // 先处理索引表达式，转换成Value*
                ast_node * idxNode = ir_visit_ast_node(array_dims[i]);
                Value *    indexVal = idxNode->val;
                node->blockInsts.addInst(idxNode->blockInsts);
                // getelementptr第一个索引固定为0
                Value * zero = module->newConstInt(0);

                // 生成getelementptr指令：
                // 类型：gepType是当前的数组类型，如 [5 x [6 x i32]] 或 [6 x i32]
                // 返回的类型是当前维度元素的指针类型，比如 [6 x i32]* 的元素是 i32
                auto gepInst = new GetElementPtrInst(
                    module->getCurrentFunction(),
                    gepPtr,
                    gepType,
                    std::vector<Value *>{zero, indexVal});
                node->blockInsts.addInst(gepInst);
                // 更新类型为下一维
                if (gepType->isArrayType()) {

                    // auto * arrTy = static_cast<ArrayType *>(gepType);
                    gepPtr = gepInst;                    // 更新起始指针为当前的 gep 指令
                    gepType = gepType->getElementType(); // 更新 gepType 为当前的元素类型
                    gepPtr->setType(gepType);            // 设置 gepPtr 的类型为最终的元素指针类型
                }
            }
            // getelementptr 0, 0
            Value * zero = module->newConstInt(0);

            // GEP 获取数组首地址，模仿数组 decay 成指针的行为
            auto final_gepInst = new GetElementPtrInst(
                module->getCurrentFunction(),
                gepPtr,                            // 原始数组变量 Value*，类型如 [5 x i32]*
                gepType,                           // 类型是 [5 x i32]*
                std::vector<Value *>{zero, zero}); // GEP 0, 0 => 获取 a[0]
            std::cout << "final_gepinst type: " << final_gepInst->getType()->toString() << std::endl;
            std::cout << "final_gepPtr type: " << gepPtr->getType()->toString() << std::endl;
            std::cout << "final_gep type: " << gepType->toString() << std::endl;
            node->blockInsts.addInst(final_gepInst);

            node->val = final_gepInst; // 设置最终的 gep 指令 Value*

            // auto * castInst = new CastInstruction(
            //     module->getCurrentFunction(),
            //     final_gepInst,
            //     new PointerType(gepType)); // 指定转换类型为 GEP 到指针
            // node->blockInsts.addInst(castInst);
            // std::cout << "cast type: " << castInst->getType()->toString() << std::endl;
            // node->val = castInst;
            // std::cout << "array type: " << node->val->getType()->toString() << std::endl;
            // // node->val = castInst; // 最终的 gep 指令 Value*
            return node->val; // 返回最终的 gep 指令 Value*
        }
    } else {
        // 处理错误情况
        std::cerr << "Error: Expected an array type." << std::endl;
        return nullptr;
    }

    return node->val; // 返回最终的 gep 指令 Value*
}

// Value * IRGenerator::funcall_array_access(ast_node * node, std::vector<Instruction *> & insts)
// {

//     // 是数组变量，提取数组名和维度表达式
//     std::string             array_name;
//     std::vector<ast_node *> array_dims;
//     extract_array_info(node, array_name, array_dims);

//     ///设置name，否则作为左值会报错
//     node->name = array_name;
//     // 解析维度表达式为实际的常数
//     std::vector<int> dims;
//     for (auto * expr_node: array_dims) {
//         int temp_size = evaluateConstExpr(expr_node); // 尝试计算数组维度，如果失败则返回负数
//         int dim_size;
//         if (temp_size < 0) {
//             dim_size = -1;
//             dims.push_back(dim_size);
//         } else {
//             dim_size = temp_size;
//             dims.push_back(dim_size);
//         }
//     }

//     /// 使用tempVal获取之前生成的节点
//     Value * tempVal = module->findVarValue(array_name);
//     if (!tempVal) {
//         std::cerr << "Function call - array: Cannot find array!" << std::endl;
//     }
//     ArrayType * arrayType = new ArrayType(tempVal->getType()->getElementType(), dims);
//     std::cout << "arrayType type: " << arrayType->toString() << std::endl;
//     ArrayType * arrayType1 = new ArrayType(tempVal->getType()->getElementType(), dims);
//     Value *     arrayPRParam = new Value(arrayType); // 设置实参表
//     arrayPRParam->setName(array_name);               // a
//     // std::cout << "arrayPRParam type: " << arrayPRParam->getType()->toString() << std::endl;
//     Type * type = tempVal->getType();

//     if (type->isArrayType()) {
//         auto *           arrayType = static_cast<ArrayType *>(type);
//         std::vector<int> ori_dims = arrayType->getDimensions();
//         // int              offset_size = calcOffset(ori_dims, dims);
//         //  int              offset = offset_size * 4;
//         int d = ori_dims.size();
//         int m = array_dims.size();

//         // 从后往前构造偏移表达式
//         Value * offset = nullptr;
//         Value * stride = module->newConstInt(1); // 初始stride=1

//         for (int j = d - 1; j > m - 1; j--) {
//             // 更新stride *= ori_dims[i]
//             auto new_stride = new BinaryInstruction(
//                 module->getCurrentFunction(),
//                 IRInstOperator::IRINST_OP_MUL_I,
//                 stride,
//                 module->newConstInt(ori_dims[j]),
//                 IntegerType::getTypeInt());
//             stride = new_stride;
//             // insts.push_back(new_stride);
//             node->blockInsts.addInst(new_stride);
//         }

//         for (int i = m - 1; i >= 0; i--) {
//             // ast_node * expr_node = array_dims[i - (d - m)];

//             // // 生成子表达式的 IR
//             // ir_visit_ast_node(expr_node);
//             // Value * indexVal = expr_node->val;
//             ast_node * expr_node = ir_visit_ast_node(array_dims[i]);

//             // 生成子表达式的 IR
//             Value * indexVal = expr_node->val;
//             node->blockInsts.addInst(expr_node->blockInsts);
//             // insts.push_back(expr_node->blockInsts);/

//             // tmp = indexVal * stride
//             auto term = new BinaryInstruction(
//                 module->getCurrentFunction(),
//                 IRInstOperator::IRINST_OP_MUL_I,
//                 indexVal,
//                 stride,
//                 IntegerType::getTypeInt());
//             // insts.push_back(term);
//             node->blockInsts.addInst(term);

//             // offset = offset + term
//             if (offset == nullptr) {
//                 offset = term;
//             } else {
//                 auto sum = new BinaryInstruction(
//                     module->getCurrentFunction(),
//                     IRInstOperator::IRINST_OP_ADD_I,
//                     offset,
//                     term,
//                     IntegerType::getTypeInt());
//                 // insts.push_back(sum);
//                 node->blockInsts.addInst(sum);
//                 offset = sum;
//             }

//             if (i - 1 >= 0) {
//                 // 更新stride *= ori_dims[i]
//                 auto new_stride = new BinaryInstruction(
//                     module->getCurrentFunction(),
//                     IRInstOperator::IRINST_OP_MUL_I,
//                     stride,
//                     module->newConstInt(ori_dims[i]),
//                     IntegerType::getTypeInt());
//                 stride = new_stride;
//                 // insts.push_back(new_stride);
//                 node->blockInsts.addInst(new_stride);
//             }
//         }
//         auto offest_size = new BinaryInstruction(
//             module->getCurrentFunction(),
//             IRInstOperator::IRINST_OP_MUL_I,
//             offset,
//             module->newConstInt(4),
//             IntegerType::getTypeInt());
//         // insts.push_back(offest_size);
//         node->blockInsts.addInst(offest_size);

//         auto addr = new BinaryInstruction(
//             module->getCurrentFunction(),
//             IRInstOperator::IRINST_OP_ADD_I,
//             tempVal,
//             offest_size,
//             IntegerType::getTypeInt());
//         node->val = addr;
//         // ///需要手动设置Type，否则addr默认是int类型的value
//         // node->val->setType(type);
//         // std::cout << "addr type: " << addr->getType()->toString() << std::endl;
//         // insts.push_back(addr);
//         node->blockInsts.addInst(addr);
//         arrayPRParam = addr;
//         arrayPRParam->setType(arrayType1); // 设置类型

//     } else {
//         // 处理错误情况
//         std::cerr << "Error: Expected an array type." << std::endl;
//         return nullptr;
//     }

//     return arrayPRParam;
// }

/// @brief 变量声明语句节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_declare_statment(ast_node * node)
{
    bool result = false;

    for (auto & child: node->sons) {

        // 遍历每个变量声明
        result = ir_variable_declare(child);
        if (!result) {
            break;
        }
        node->blockInsts.addInst(child->blockInsts);
    }

    return result;
}

/// @brief 变量定声明节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_variable_declare(ast_node * node)
{
    // 第一个孩子：类型，第二个孩子：变量名（数组或普通变量），第三个孩子：初值（如果有）
    ast_node * type_node = node->sons[0]; // 类型节点
    ast_node * id_node = node->sons[1];   // 变量名节点（支持数组的情况）
    ast_node * init_val_node = (node->sons.size() > 2) ? node->sons[2] : nullptr; // 初始值节点（可选）

    Type * var_type = type_node->type;

    if (id_node->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
        // 是数组变量，提取数组名和维度表达式
        std::string             array_name;
        std::vector<ast_node *> array_dims;
        extract_array_info(id_node, array_name, array_dims);

        // 解析维度表达式为实际的常数
        std::vector<int> dims;
        for (auto * expr_node: array_dims) {
            float dim_size;
            if (!evaluateConstExpr(expr_node, &dim_size)) {
                std::cerr << "Const declare: Failed to evaluate constant expression for array dimension." << std::endl;
                return false;
            }
            dims.push_back((int) dim_size);
        }
        // 调用 module->newArrayVarValue 分配数组变量
        node->val = module->newArrayVarValue(var_type, array_name, dims, ValueCategory::VARIABLE);
        if (init_val_node) {
            std::vector<ast_node *>      init_list;
            std::vector<Instruction *> * insts = new std::vector<Instruction *>;
            if (!init_array_flattened(node->val, dims, init_val_node, *insts, init_list, var_type)) {
                printf("数组初始化失败\n");
                return false;
            }
            // 在这里使用 GlobalVariable* 临时变量来访问子类方法
            GlobalVariable * gv = static_cast<GlobalVariable *>(node->val);
            gv->setFasle_inBSSSection();
            // 数组初始化后，不属于ibss段，不论局部变量和全局变量
            // 存储初值
            if (type_node->type->isFloatType()) {
                // 浮点数类型
                auto float_init_list = new std::vector<float>;
                for (auto init_num: init_list) {
                    if (init_num->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT) {
                        float_init_list->push_back(init_num->float_val);
                    } else if (init_num->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_UINT) {
                        if (init_num->integer_val != 0) {
                            std::cerr << "Warning: Auto transform type \"int\" to \"float\" at \"" << array_name
                                      << "\"." << std::endl;
                        }
                        // TODO 增加类型转化指令
                        float_init_list->push_back((float) init_num->integer_val);
                    } else {
                        std::cerr << "ERROR(const declare): No match type for  float array " << array_name << "."
                                  << std::endl;
                        return false;
                    }
                }

                node->val->setInitVal(float_init_list);
            } else {
                // 整数类型
                auto int_init_list = new std::vector<int>;
                for (auto init_num: init_list) {
                    if (init_num->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT) {
                        int_init_list->push_back(init_num->float_val);
                        std::cerr << "Warning: Auto transform type \"float\" to \"int\" at \"" << array_name << "\"."
                                  << std::endl;
                        // TODO 增加类型转化指令
                    } else if (init_num->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_UINT) {
                        int_init_list->push_back((float) init_num->integer_val);
                    } else {
                        std::cerr << "ERROR(const declare): No matched type for const float array " << array_name << "."
                                  << std::endl;
                        return false;
                    }
                }
                node->val->setInitVal(int_init_list);
            }
            for (auto inst: *insts) {
                node->blockInsts.addInst(inst);
            }
        }

    } else {
        // 普通变量
        std::string var_name = id_node->name;

        if (init_val_node) {
            if (type_node->type->isFloatType()) {
                // 浮点数类型
                node->val =
                    module->newVarValueWithFloat(var_type, var_name, init_val_node->float_val, ValueCategory::VARIABLE);
            } else {
                // 整数类型
                node->val =
                    module->newVarValueWithInt(var_type, var_name, init_val_node->integer_val, ValueCategory::VARIABLE);
            }
            // 赋值运算符的左侧操作数
            ast_node * left = ir_visit_ast_node(id_node);
            if (!left) {
                // 某个变量没有定值
                // 这里缺省设置变量不存在则创建，因此这里不会错误
                printf(" no values.\n");
                return false;
            }
            // 赋值运算符的右侧操作数
            ast_node * right = ir_visit_ast_node(init_val_node);
            if (!right) {
                // 某个变量没有定值
                printf("Assign: some variables have no values.\n");
                return false;
            }

            StoreInstruction * storeInst = new StoreInstruction(module->getCurrentFunction(), left->val, right->val);
            // 创建临时变量保存IR的值，以及线性IR指令
            node->blockInsts.addInst(right->blockInsts);
            node->blockInsts.addInst(left->blockInsts);
            node->blockInsts.addInst(storeInst);

        } else {
            node->val = module->newVarValue(var_type, var_name);
        }
    }
    return true;
}

bool IRGenerator::ir_const_declare_statment(ast_node * node)
{
    bool result = false;
    for (auto & child: node->sons) {
        // 遍历每个常量声明
        result = ir_const_declare(child);
        if (!result) {
            break;
        }
        node->blockInsts.addInst(child->blockInsts);
    }
    return result;
}

bool IRGenerator::ir_const_declare(ast_node * node)
{
    // 第一个孩子：类型，第二个孩子：常量名，第三个孩子：初值
    ast_node * type_node = node->sons[0];                                         // 类型节点
    ast_node * id_node = node->sons[1];                                           // 常量名
    ast_node * init_val_node = (node->sons.size() > 2) ? node->sons[2] : nullptr; //初始值节点(常量必须有初始值)

    Type * var_type = type_node->type;
    if (id_node->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
        // 是数组变量，提取数组名和维度表达式
        std::string             array_name;
        std::vector<ast_node *> array_dims;
        extract_array_info(id_node, array_name, array_dims);

        // 解析维度表达式为实际的常数
        std::vector<int> dims;
        for (auto * expr_node: array_dims) {
            float dim_size;
            if (!evaluateConstExpr(expr_node, &dim_size)) {
                std::cerr << "Const declare: Failed to evaluate constant expression for array dimension." << std::endl;
                return false;
            }
            dims.push_back((int) dim_size);
        }

        // 调用 module->newArrayVarValue 分配数组变量
        node->val = module->newArrayVarValue(var_type, array_name, dims, ValueCategory::CONSTANT);
        if (init_val_node) {
            std::vector<ast_node *>      init_list;
            std::vector<Instruction *> * insts = new std::vector<Instruction *>;
            if (!init_array_flattened(node->val, dims, init_val_node, *insts, init_list, var_type)) {
                std::cerr << "Const declare: Failed to init const array!" << std::endl;
                return false;
            }
            // 在这里使用 GlobalVariable* 临时变量来访问子类方法
            GlobalVariable * gv = static_cast<GlobalVariable *>(node->val);
            gv->setFasle_inBSSSection();
            // 存储初值
            if (type_node->type->isFloatType()) {
                // 浮点数类型
                auto float_init_list = new std::vector<float>;
                for (auto init_num: init_list) {
                    if (init_num->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT) {
                        float_init_list->push_back(init_num->float_val);
                    } else if (init_num->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_UINT) {
                        if (init_num->integer_val != 0) {
                            std::cerr << "Warning: Auto transform type \"int\" to \"float\" at \"" << array_name
                                      << "\"." << std::endl;
                        }
                        // TODO 增加类型转化指令
                        float_init_list->push_back((float) init_num->integer_val);
                    } else {
                        std::cerr << "ERROR(const declare): No match type for const float array " << array_name << "."
                                  << std::endl;
                        return false;
                    }
                }
                node->val->setInitVal(float_init_list);
            } else {
                // 整数类型
                auto int_init_list = new std::vector<int>;
                for (auto init_num: init_list) {
                    if (init_num->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT) {
                        int_init_list->push_back(init_num->float_val);
                        std::cerr << "Warning: Auto transform type \"float\" to \"int\" at \"" << array_name << "\"."
                                  << std::endl;
                        // TODO 增加类型转化指令
                    } else if (init_num->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_UINT) {
                        int_init_list->push_back((float) init_num->integer_val);
                    } else {
                        std::cerr << "ERROR(const declare): No matched type for const int array " << array_name << "."
                                  << std::endl;
                        return false;
                    }
                }
                node->val->setInitVal(int_init_list);
            }
            for (auto inst: *insts) {
                node->blockInsts.addInst(inst);
            }
        } else {
            std::cerr << "Semantic error: constant variable must be initialized\n" << std::endl;
            return false;
        }
    } else {
        // 普通变量
        std::string var_name = id_node->name;
        // TODO 增加类型转化指令
        if (init_val_node) {
            if (type_node->type->isFloatType()) {
                // 浮点数类型
                if (init_val_node->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_UINT) {
                    node->val = module->newVarValueWithFloat(
                        var_type,
                        var_name,
                        (float) init_val_node->integer_val,
                        ValueCategory::CONSTANT);
                    if (init_val_node->integer_val != 0) {
                        std::cerr << "Warning: Auto transform type \"int\" to \"float\" at variable \"" << var_name
                                  << "\"." << std::endl;
                    }
                } else if (init_val_node->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT) {
                    node->val = module->newVarValueWithFloat(
                        var_type,
                        var_name,
                        init_val_node->float_val,
                        ValueCategory::CONSTANT);
                } else {
                    float init_num;
                    if (!evaluateConstExpr(init_val_node, &init_num)) {
                        std::cerr << "ERROR(const declare): Cannot evaluate a non-const expression!" << var_name << "."
                                  << std::endl;
                        return false;
                    } else {
                        std::cout << "Evaluate successful with return value: " << init_num << std::endl;
                        node->val = module->newVarValueWithFloat(var_type, var_name, init_num, ValueCategory::CONSTANT);
                    }
                }
                // 检查是否成功创建常量变量
                if (!node->val) {
                    std::cerr << "Error: Failed to create constant variable with float value." << std::endl;
                    return false;
                }
            } else {
                // 整数类型
                if (init_val_node->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_UINT) {
                    node->val = module->newVarValueWithInt(
                        var_type,
                        var_name,
                        init_val_node->integer_val,
                        ValueCategory::CONSTANT);
                } else if (init_val_node->node_type == ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT) {
                    node->val = module->newVarValueWithFloat(
                        var_type,
                        var_name,
                        (int) init_val_node->float_val,
                        ValueCategory::CONSTANT);
                    std::cerr << "Warning: Auto transform type \"float\" to \"int\" at variable \"" << var_name << "\"."
                              << std::endl;
                    // TODO 增加类型转化指令
                } else {
                    float init_num;
                    if (!evaluateConstExpr(init_val_node, &init_num)) {
                        std::cerr << "ERROR(const declare): Cannot evaluate a non-const expression!" << var_name << "."
                                  << std::endl;
                        return false;
                    } else {
                        std::cout << "Evaluate successful with return value: " << init_num << std::endl;
                        node->val =
                            module->newVarValueWithInt(var_type, var_name, (int) init_num, ValueCategory::CONSTANT);
                    }
                }
                // 检查是否成功创建常量变量
                if (!node->val) {
                    std::cerr << "Error: Failed to create constant variable with int value." << std::endl;
                    return false;
                }
            }
            // 赋值运算符的左侧操作数
            ast_node * left = ir_visit_ast_node(id_node);
            if (!left) {
                // 某个变量没有定值
                // 这里缺省设置变量不存在则创建，因此这里不会错误
                printf(" no values.\n");
                return false;
            }
            // 赋值运算符的右侧操作数
            // ast_node * right = ir_visit_ast_node(init_val_node);
            // if (!right) {
            //     // 某个变量没有定值
            //     printf("Assign: some variables have no values.\n");
            //     return false;
            // }
            StoreInstruction * storeInst;
            if (type_node->type->isFloatType()) {
                ConstFloat * constf = new ConstFloat(node->val->getFloatInitVal());
                storeInst = new StoreInstruction(module->getCurrentFunction(), left->val, constf);
            } else {
                ConstInt * consti = new ConstInt(node->val->getIntInitVal());
                storeInst = new StoreInstruction(module->getCurrentFunction(), left->val, consti);
            }

            // 创建临时变量保存IR的值，以及线性IR指令
            node->blockInsts.addInst(node->blockInsts);
            node->blockInsts.addInst(left->blockInsts);
            node->blockInsts.addInst(storeInst);

        } else {
            printf("Semantic error: constant variable must be initialized\n");
            return false;
        }
    }
    return true;
}
// TODO：验证计算功能（等待实现常量访问的方法）

bool IRGenerator::evaluateConstExpr(ast_node * root, float * result)
{
    if (!root || !result)
        return false;

    switch (root->node_type) {
        case ast_operator_type::AST_OP_LEAF_LITERAL_UINT:
            *result = static_cast<float>(root->integer_val);
            return true;

        case ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT:
            *result = root->float_val;
            return true;

        case ast_operator_type::AST_OP_LEAF_VAR_ID:
            return getConstVal(root->name, result);

        case ast_operator_type::AST_OP_ARRAY_ACCESS: {
            if (root->sons.empty())
                return false;
            ast_node * name_node = root->sons[0];
            if (name_node->node_type != ast_operator_type::AST_OP_LEAF_VAR_ID)
                return false;

            std::vector<int> dims;
            for (size_t i = 1; i < root->sons.size(); ++i) {
                float val;
                if (!evaluateConstExpr(root->sons[i], &val))
                    return false;
                dims.push_back(static_cast<int>(val));
            }
            return getConstVal(name_node->name, dims, result);
        }

        // 一元运算
        case ast_operator_type::AST_OP_POS:
        case ast_operator_type::AST_OP_NEG:
        case ast_operator_type::AST_OP_NOT: {
            if (root->sons.size() != 1)
                return false;
            float operand;
            if (!evaluateConstExpr(root->sons[0], &operand))
                return false;

            switch (root->node_type) {
                case ast_operator_type::AST_OP_POS:
                    *result = +operand;
                    break;
                case ast_operator_type::AST_OP_NEG:
                    *result = -operand;
                    break;
                case ast_operator_type::AST_OP_NOT:
                    *result = (!operand) ? 1.0f : 0.0f;
                    break;
                default:
                    return false;
            }
            return true;
        }

        // 二元运算
        default: {
            if (root->sons.size() != 2)
                return false;
            float lhs, rhs;
            if (!evaluateConstExpr(root->sons[0], &lhs))
                return false;
            if (!evaluateConstExpr(root->sons[1], &rhs))
                return false;

            switch (root->node_type) {
                case ast_operator_type::AST_OP_ADD:
                    *result = lhs + rhs;
                    break;
                case ast_operator_type::AST_OP_SUB:
                    *result = lhs - rhs;
                    break;
                case ast_operator_type::AST_OP_MUL:
                    *result = lhs * rhs;
                    break;
                case ast_operator_type::AST_OP_DIV:
                    if (rhs == 0.0f)
                        return false;
                    *result = lhs / rhs;
                    break;
                case ast_operator_type::AST_OP_MOD:
                    if (rhs == 0.0f)
                        return false;
                    *result = fmodf(lhs, rhs);
                    break;
                case ast_operator_type::AST_OP_AND:
                    *result = (lhs && rhs) ? 1.0f : 0.0f;
                    break;
                case ast_operator_type::AST_OP_OR:
                    *result = (lhs || rhs) ? 1.0f : 0.0f;
                    break;
                case ast_operator_type::AST_OP_EQ:
                    *result = (lhs == rhs) ? 1.0f : 0.0f;
                    break;
                case ast_operator_type::AST_OP_NEQ:
                    *result = (lhs != rhs) ? 1.0f : 0.0f;
                    break;
                case ast_operator_type::AST_OP_GE:
                    *result = (lhs >= rhs) ? 1.0f : 0.0f;
                    break;
                case ast_operator_type::AST_OP_LE:
                    *result = (lhs <= rhs) ? 1.0f : 0.0f;
                    break;
                case ast_operator_type::AST_OP_GNE:
                    *result = (lhs > rhs) ? 1.0f : 0.0f;
                    break;
                case ast_operator_type::AST_OP_LNE:
                    *result = (lhs < rhs) ? 1.0f : 0.0f;
                    break;
                default:
                    return false;
            }
            return true;
        }
    }
}

bool IRGenerator::init_array_flattened(
    Value * arrayVar, const std::vector<int> & dims, ast_node * initNode, std::vector<Instruction *> & Insts,
    std::vector<ast_node *> & init_list, Type * val_type)
{
    // 1. 计算总元素数
    int total_elems = 1;
    for (int d: dims)
        total_elems *= d;

    // 2. 拉平成一维值数组
    std::vector<ast_node *> * flat_list = new std::vector<ast_node *>;
    flatten_init_node(initNode, dims, 0, *flat_list);

    // 3. 填充 IR
    for (int i = 0; i < total_elems; ++i) {
        ast_node * val_node = (i < flat_list->size()) ? (*flat_list)[i] : nullptr;

        Value * val = nullptr;
        if (val_node) {
            if (!val_node->val) {
                float * val_float = new float;
                evaluateConstExpr(val_node, val_float);
                if (val_type->isFloatType()) {
                    float result = *val_float;
                    delete val_float;                              // 释放临时变量
                    val_node->val = module->newConstFloat(result); // 设置浮点值                  //
                    val_node->node_type = ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT; // 设置节点类型为浮点数

                } else if (val_type->isIntegerType()) {
                    int result = (int) *val_float;
                    delete val_float; // 释放临时变量
                    val_node->val = module->newConstInt(result);
                    val_node->node_type = ast_operator_type::AST_OP_LEAF_LITERAL_UINT; // 设置节点类型为整数
                }
            }
            val = val_node->val ? val_node->val : nullptr;
            if (!val) {
                std::cerr << "Error: val_node->val is null at index " << i << std::endl;
                return false;
            }
        } else {
            // TODO：有待检查此处补0在多维数组情况下是否逻辑正确
            auto tempNode = new ast_node(digit_int_attr{0, 0});
            flat_list->push_back(tempNode);
            val = module->newConstInt(0); // 默认补零
        }

        // 4. 计算地址偏移（假设 4 字节）
        auto offest = new BinaryInstruction(
            module->getCurrentFunction(),
            IRInstOperator::IRINST_OP_MUL_I,
            module->newConstInt(i),
            module->newConstInt(4),
            IntegerType::getTypeInt());
        Insts.push_back(offest);

        // Value * addr = module->createAdd(arrayVar, module->newConstInt(offset), Insts);
        auto addr = new BinaryInstruction(
            module->getCurrentFunction(),
            IRInstOperator::IRINST_OP_ADD_I,
            arrayVar,
            offest,
            IntegerType::getTypeInt());
        Insts.push_back(addr);
        // 5. 生成 store 指令
        StoreInstruction * storeInst = new StoreInstruction(module->getCurrentFunction(), addr, val);
        // addr->setIRName(std::to_string(addr->getIntVal()));
        Insts.push_back(storeInst);
    }
    init_list = *flat_list;
    return true;
}

// FIXME：@吴灿 调用evaluateConstExpr计算初始化节点的值
void IRGenerator::flatten_init_node(
    ast_node * node, const std::vector<int> & dims, int depth, std::vector<ast_node *> & flat_list)
{
    if (!node)
        return;

    if (node->node_type == ast_operator_type::AST_OP_INIT_VAL) {
        int i = 0;
        for (; i < (int) node->sons.size(); ++i) {

            flatten_init_node(node->sons[i], dims, depth + 1, flat_list);
        }
        // 补零（如果不足当前维度）
        for (; i < dims[depth]; ++i) {
            if (depth + 1 == (int) dims.size()) {
                // 到达标量层，补0
                ast_node * zero = new ast_node(ast_operator_type::AST_OP_LEAF_LITERAL_UINT);
                zero->integer_val = 0;
                zero->val = module->newConstInt(0);
                flat_list.push_back(zero);
            } else {
                flatten_init_node(nullptr, dims, depth + 1, flat_list);
            }
        }
    } else {
        // 是一个值节点，直接加入
        flat_list.push_back(node);
    }
}

bool IRGenerator::gen_condition_branch(
    ast_node * cond_node, LabelInstruction * true_target, LabelInstruction * false_target,
    InterCode & current_block_insts)
{
    if (!cond_node) {
        std::cerr << "Error: Null condition node for branching." << std::endl;
        return false;
    }
    Function * currentFunc = module->getCurrentFunction();
    if (!currentFunc) {
        std::cerr << "Error: gen_condition_branch called outside function context." << std::endl;
        return false;
    }

    ast_operator_type op = cond_node->node_type; // 获取操作符类型
    // --- 1. Check for short-circuiting operators (&&, ||) ---
    if (op == ast_operator_type::AST_OP_AND || op == ast_operator_type::AST_OP_OR) {
        // std::cout << "Handling short-circuiting!" << std::endl;
        ast_node * left_node = cond_node->sons[0];
        ast_node * right_node = cond_node->sons[1];

        if (op == ast_operator_type::AST_OP_AND) {
            // Short-circuit for && (a && b)
            // Logic: Evaluate a. If a is true, evaluate b. If a is false, jump to false_target.
            // a && b branches to true_target if (a is true AND b is true)
            // a && b branches to false_target if (a is false OR (a is true AND b is false))

            // Create a label to evaluate the right side (b) if the left side (a) is true
            LabelInstruction * eval_right_label = new LabelInstruction(currentFunc);

            // Recursively generate IR for the left operand (a)
            // If 'a' is true, jump to eval_right_label. If 'a' is false, jump directly to the overall false_target.
            if (!gen_condition_branch(left_node, eval_right_label, false_target, current_block_insts)) {
                std::cerr << "Error generating left operand for &&." << std::endl;
                return false;
            }

            // Add the label for the basic block that evaluates the right side
            current_block_insts.addInst(eval_right_label);

            // Recursively generate IR for the right operand (b)
            // If 'b' is true, jump to the overall true_target. If 'b' is false, jump to the overall false_target.
            // Note: This block is only reached if 'a' was true.
            if (!gen_condition_branch(right_node, true_target, false_target, current_block_insts)) {
                std::cerr << "Error generating right operand for &&." << std::endl;
                return false;
            }
            return true; // Successfully generated IR for && short-circuiting

        } else if (op == ast_operator_type::AST_OP_OR) {
            // Short-circuit for || (a || b)
            // Logic: Evaluate a. If a is false, evaluate b. If a is true, jump to true_target.
            // a || b branches to true_target if (a is true OR (a is false AND b is true))
            // a || b branches to false_target if (a is false AND b is false)

            // Create a label to evaluate the right side (b) if the left side (a) is false
            LabelInstruction * eval_right_label = new LabelInstruction(currentFunc);

            // Recursively generate IR for the left operand (a)
            // If 'a' is true, jump directly to the overall true_target. If 'a' is false, jump to eval_right_label.
            if (!gen_condition_branch(left_node, true_target, eval_right_label, current_block_insts)) {
                std::cerr << "Error generating left operand for ||." << std::endl;
                return false;
            }

            // Add the label for the basic block that evaluates the right side
            current_block_insts.addInst(eval_right_label);

            // Recursively generate IR for the right operand (b)
            // If 'b' is true, jump to the overall true_target. If 'b' is false, jump to the overall false_target.
            // Note: This block is only reached if 'a' was false.
            if (!gen_condition_branch(right_node, true_target, false_target, current_block_insts)) {
                std::cerr << "Error generating right operand for ||." << std::endl;
                return false;
            }

            return true; // Successfully generated IR for || short-circuiting
        }
        // Fall through if it's another binary op (like comparison)
    }
    // --- 2. Check for logical NOT (!) ---
    else if (op == ast_operator_type::AST_OP_NOT) {
        ast_node * operand_node = cond_node->sons[0];
        // expr is true when expr is false, and false when expr is true.
        // So, recursively generate IR for 'expr' but swap the true and false targets.
        return gen_condition_branch(operand_node, false_target, true_target, current_block_insts);
        // Fall through for other unary ops
    }

    // --- 3. Handle other condition types (comparisons, variables, literals, calls returning value) ---
    // For these, evaluate the expression to get a single Value, then branch based on that value.

    ast_node * cond_eval_result = ir_visit_ast_node(cond_node); // Generate IR for the condition expression
    if (!cond_eval_result || !cond_eval_result->val) {
        std::cerr << "Error: Condition expression failed to generate value." << std::endl;
        return false;
    }
    current_block_insts.addInst(cond_eval_result->blockInsts); // Add the evaluation instructions to the current block

    Value * cond_val = cond_eval_result->val;
    Type *  cond_type = cond_val->getType();

    // --- 4. Ensure the condition value is of type i1 (boolean) ---
    // SysY treats non-zero int/float as true, zero as false. Need to convert if necessary.
    Value * branch_cond_val = nullptr; // This will be the final i1 value used for branching

    if (cond_type->isInt1Byte()) {
        branch_cond_val = cond_val;
    } else if (cond_type->isIntegerType()) {
        Constant * zero_const = new ConstInt(0);
        if (!zero_const) {
            std::cerr << "Internal Error: Failed to get zero constant for integer type." << std::endl;
            return false;
        }
        // Create the icmp ne instruction: cond_val != 0
        // Use the IcmpInstruction constructor.
        Instruction * cmp_inst =
            new IcmpInstruction(currentFunc, IRInstOperator::IRINST_OP_NEQ_I, cond_val, zero_const);
        current_block_insts.addInst(cmp_inst);
        branch_cond_val = static_cast<Value *>(cmp_inst); // The IcmpInstruction itself is the i1 Value result
    } else if (cond_type->isFloatType()) {                // If it's a float type (like float/f32)
        // Convert non-zero float to i1 true, zero to i1 false (value != 0.0)
        // Use FcmpInstruction with 'une' predicate for float not equal.
        Constant * zero_const = new ConstFloat(0.0f); // Assuming ConstantFloat::get(0.0f) returns a Value*
        if (!zero_const) {
            std::cerr << "Internal Error: Failed to get zero constant for float type." << std::endl;
            return false;
        }
        // Create the fcmp une instruction: cond_val != 0.0
        Instruction * fcmp_inst = new FcmpInstruction(
            currentFunc,
            IRInstOperator::IRINST_OP_NEQ_F,
            cond_val,
            zero_const); // Using NEQ_F maps to 'une' in Fcmp toString
        current_block_insts.addInst(fcmp_inst);
        branch_cond_val = static_cast<Value *>(fcmp_inst); // The FcmpInstruction itself is the i1 Value result

    } else {
        // Unsupported type for a condition
        std::cerr << "Error: Invalid type for condition expression: " << cond_type->toString() << std::endl;
        return false;
    }

    if (!branch_cond_val) {
        std::cerr << "Internal Error: Branch condition value is null after type handling." << std::endl;
        return false;
    }

    // --- 5. Add the final conditional branch based on the resulting i1 value ---
    // ConditionalInstruction(Function* func, Value* condition, LabelInstruction* true_target, LabelInstruction*
    // false_target)
    current_block_insts.addInst(new ConditionalInstruction(currentFunc, branch_cond_val, true_target, false_target));

    return true; // Successfully generated IR for non-short-circuiting condition
}

// TODO:验证获取常量是否正常
bool IRGenerator::getConstVal(std::string name, float * val)
{
    Value * var = module->findVarValue(name);
    if (!var) {
        std::cerr << "Error: Constant variable '" << name << "' not found." << std::endl;
        return false;
    }
    if (var->getValueCategory() == ValueCategory::CONSTANT && var->isInited) {
        if (var->getType()->isFloatType()) {
            *val = var->getFloatInitVal();
            return true;
        } else if (var->getType()->isIntegerType()) {
            *val = (float) var->getIntInitVal();
            return true;
        } else {
            return false;
        }
    } else {
        std::cerr << "Error: Variable '" << name << "' is not a constant or not initialized." << std::endl;
        return false;
    }
}
bool IRGenerator::getConstVal(std::string name, std::vector<int> & dims, float * val)
{
    Value * var = module->findVarValue(name);
    if (!var) {
        std::cerr << "Error: Constant variable '" << name << "' not found." << std::endl;
        return false;
    }
    if (var->getValueCategory() == ValueCategory::CONSTANT && var->isInited) {
        if (var->getType()->getBaseElementType()) {
            if (!var->getArrayValByIndex(dims, val)) {
                std::cerr << "Error: Failed to get array value for variable '" << name << "' with dimensions [";
                for (size_t i = 0; i < dims.size(); ++i) {
                    std::cout << dims[i];
                    if (i < dims.size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << "]." << std::endl;
                return false;
            } else {
                return true;
            }
        } else {
            std::cerr << "Error: Variable '" << name << "' is not an array." << std::endl;
            return false;
        }
    } else {
        std::cerr << "Error: Variable '" << name << "' is not a constant or not initialized." << std::endl;
        return false;
    }
    return false;
}
