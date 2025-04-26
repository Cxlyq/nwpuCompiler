///
/// @file Antlr4CSTVisitor.cpp
/// @brief Antlr4的具体语法树的遍历产生AST
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
// TODO: [完整性检查] 默认出口(nullptr?)
// TODO: [完整性检查] 每个函数都要加入对应标签
// TODO: [语义检查] 每个Visitor函数都需要进行语义分析，throw error.
#include <any>
#include <cmath>
#include <cstddef>
#include <string>
#include "Antlr4CSTVisitor.h"
#include "AST.h"
#include "AttrType.h"
#include "MiniCParser.h"
#include "iostream"

#define Instanceof(res, type, var) auto res = dynamic_cast<type>(var)

/// @brief 构造函数
MiniCCSTVisitor::MiniCCSTVisitor()
{}

/// @brief 析构函数
MiniCCSTVisitor::~MiniCCSTVisitor()
{}

/// @brief 遍历CST产生AST
/// @param root CST语法树的根结点
/// @return AST的根节点
ast_node * MiniCCSTVisitor::run(MiniCParser::CompileUnitContext * root)
{
    return std::any_cast<ast_node *>(visitCompileUnit(root));
}

/// @brief 非终结运算符compileUnit的遍历
/// @param ctx CST上下文
/// @return 根结点
std::any MiniCCSTVisitor::visitCompileUnit(MiniCParser::CompileUnitContext * ctx)
{
    // compileUnit: (funcDef | decl)* EOF
    // VarDecl改为Decl
    // 请注意这里必须先遍历全局变量后遍历函数。肯定可以确保全局变量先声明后使用的规则，但有些情况却不能检查出。
    // 事实上可能函数A后全局变量B后函数C，这时在函数A中是不能使用变量B的，需要报语义错误，但目前的处理不会。
    // 因此在进行语义检查时，可能追加检查行号和列号，如果函数的行号/列号在全局变量的行号/列号的前面则需要报语义错误
    // TODO: [语义检查] 追加实现行列号的变量检查

    ast_node * temp_node;
    ast_node * compileUnitNode = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT);

    // 可能多个变量，因此必须循环遍历
    for (auto varCtx: ctx->decl()) {

        // 变量函数定义
        temp_node = std::any_cast<ast_node *>(visitDecl(varCtx));
        (void) compileUnitNode->insert_son_node(temp_node);
    }

    // 可能有多个函数，因此必须循环遍历
    for (auto funcCtx: ctx->funcDef()) {

        // 变量函数定义
        temp_node = std::any_cast<ast_node *>(visitFuncDef(funcCtx));
        (void) compileUnitNode->insert_son_node(temp_node);
    }

    return compileUnitNode;
}

/// @brief 非终结运算符funcDef的遍历
/// @param ctx CST上下文
/// @return 函数结点
std::any MiniCCSTVisitor::visitFuncDef(MiniCParser::FuncDefContext * ctx)
{
    // 识别的文法产生式：funcDef : T_INT T_ID T_L_PAREN T_R_PAREN block;
    // 函数返回类型，终结符

    type_attr funcReturnType = std::any_cast<type_attr>(visitFuncType(ctx->funcType()));
    // 创建函数名的标识符终结符节点，终结符
    char * id = strdup(ctx->T_ID()->getText().c_str());

    var_id_attr funcId{id, (int64_t) ctx->T_ID()->getSymbol()->getLine()};

    // 形参结点目前没有，设置为空指针
    ast_node * formalParamsNode = nullptr;
    // TODO: [函数] 设置形参结点入口
    if (ctx->funcFParams()) {
        formalParamsNode = std::any_cast<ast_node *>(visitFuncFParams(ctx->funcFParams()));
    }

    // 遍历block结点创建函数体节点，非终结符
    auto blockNode = std::any_cast<ast_node *>(visitBlock(ctx->block()));

    // 创建函数定义的节点，孩子有类型，函数名，语句块和形参(实际上无)
    // create_func_def函数内会释放funcId中指向的标识符空间，切记，之后不要再释放，之前一定要是通过strdup函数或者malloc分配的空间
    return create_func_def(funcReturnType, funcId, blockNode, formalParamsNode);
}
/// @brief 非终结运算符funcType的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitFuncType(MiniCParser::FuncTypeContext * ctx)
{
    type_attr attr{.type = BasicType::TYPE_VOID, .lineno = -1};
    if (ctx->T_INT()) {
        attr.type = BasicType::TYPE_INT;
        attr.lineno = (int64_t) ctx->T_INT()->getSymbol()->getLine();
    } else if (ctx->T_FLOAT()) {
        attr.type = BasicType::TYPE_FLOAT;
        attr.lineno = (int64_t) ctx->T_FLOAT()->getSymbol()->getLine();
    }
    return attr;
}

/// @brief 非终结运算符funcFParams的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitFuncFParams(MiniCParser::FuncFParamsContext * ctx)
{
    // TODO: [函数] 形参列表结点
    std::vector<ast_node *> params;

    for (auto paramCtx: ctx->funcFParam()) {
        auto paramNode = std::any_cast<ast_node *>(visitFuncFParam(paramCtx));
        params.push_back(paramNode);
    }

    return create_param_list(params); // 创建形参列表的AST节点
}

/// @brief 非终结运算符funcFParam的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitFuncFParam(MiniCParser::FuncFParamContext * ctx)
{
    // TODO: [函数] 形参结点
    // 获取参数类型
    type_attr paramType = std::any_cast<type_attr>(visitBasicType(ctx->basicType()));

    // 获取参数名称
    char * id = strdup(ctx->T_ID()->getText().c_str());
    var_id_attr paramId{id, (int64_t) ctx->T_ID()->getSymbol()->getLine()};

    // 判断是否是数组参数
    bool isArray = ctx->T_L_SQBRA().size() > 0;

    std::vector<ast_node *> dimensions;
    if (ctx->expr().size() > 0) {
        for (auto dimExpr: ctx->expr()) {
            dimensions.push_back(std::any_cast<ast_node *>(visit(dimExpr)));
        }
    }

    if (isArray) {
        return create_array_param(paramType, paramId, dimensions);
    } else {
        return create_var_param(paramType, paramId);
    }
    return nullptr;
}

/// @brief 非终结运算符block的遍历
/// @param ctx CST上下文
/// @return 下级结点/blocknode
std::any MiniCCSTVisitor::visitBlock(MiniCParser::BlockContext * ctx)
{
    // 识别的文法产生式：block : T_L_BRACE blockItemList? T_R_BRACE';
    if (ctx->blockItemList()) {
        // 语句块含有语句
        // 内部创建Block节点，并把语句加入，这里不需要创建Block节点
        return visitBlockItemList(ctx->blockItemList());
    } else {
        // 语句块没有语句
        // 为了方便创建一个空的Block节点
        return create_contain_node(ast_operator_type::AST_OP_BLOCK);
    }
}

/// @brief 非终结运算符blockItemList的遍历
/// @param ctx CST上下文
/// @return blocknode
std::any MiniCCSTVisitor::visitBlockItemList(MiniCParser::BlockItemListContext * ctx)
{
    // 识别的文法产生式：blockItemList : blockItem +;
    // 正闭包 循环 至少一个blockItem
    auto block_node = create_contain_node(ast_operator_type::AST_OP_BLOCK);

    for (auto blockItemCtx: ctx->blockItem()) {

        // 非终结符，需遍历
        auto blockItem = std::any_cast<ast_node *>(visitBlockItem(blockItemCtx));

        // 插入到块节点中
        (void) block_node->insert_son_node(blockItem);
    }

    return block_node;
}

///
/// @brief 非终结运算符blockItem的遍历
/// @param ctx CST上下文
///	@return 下级结点
std::any MiniCCSTVisitor::visitBlockItem(MiniCParser::BlockItemContext * ctx)
{
    // 识别的文法产生式：blockItem : statement | decl
    if (ctx->statement()) {
        // 语句识别
        return visitStatement(ctx->statement());
    } else if (ctx->decl()) {
        return visitDecl(ctx->decl());
    } else {
        return nullptr;
    }
}

/// @brief 非终结运算符decl的遍历
/// @param ctx CST上下文
///	@return 下级结点
std::any MiniCCSTVisitor::visitDecl(MiniCParser::DeclContext * ctx)
{
    if (ctx->constDecl()) {
        return visitConstDecl(ctx->constDecl());
    } else if (ctx->varDecl()) {
        return visitVarDecl(ctx->varDecl());
    }
    return nullptr;
}

/// @brief 非终结运算符basicType的遍历
/// @param ctx CST上下文
///	@return [非结点]形参类型
std::any MiniCCSTVisitor::visitBasicType(MiniCParser::BasicTypeContext * ctx)
{
    // basicType: T_INT,T_FLOAT;
    type_attr attr{BasicType::TYPE_VOID, -1};
    if (ctx->T_INT()) {
        attr.type = BasicType::TYPE_INT;
        attr.lineno = (int64_t) ctx->T_INT()->getSymbol()->getLine();
    } else if (ctx->T_FLOAT()) {
        attr.type = BasicType::TYPE_FLOAT;
        attr.lineno = (int64_t) ctx->T_FLOAT()->getSymbol()->getLine();
    }
    return attr;
}
/// @brief 非终结运算符constDecl的遍历
/// @param ctx CST上下文
///	@return 常量声明结点（含子结点）
std::any MiniCCSTVisitor::visitConstDecl(MiniCParser::ConstDeclContext * ctx)
{
    // constDecl: T_CONST basicType constDef (T_COMMA constDef)* T_SEMICOLON;

    // 声明语句节点
    ast_node * const_stmt_node = create_contain_node(ast_operator_type::AST_OP_CONST_DECL_STMT);

    // 类型节点
    type_attr typeAttr = std::any_cast<type_attr>(visitBasicType(ctx->basicType()));

    for (auto & constCtx: ctx->constDef()) {

        // 常量名节点
        ast_node * id_node = std::any_cast<ast_node *>(visitConstDef(constCtx));

        // 创建类型节点
        ast_node * type_node = create_type_node(typeAttr);

        // 创建常量定义节点
        ast_node * decl_node = ast_node::New(ast_operator_type::AST_OP_CONST_DECL, type_node, id_node, nullptr);

        // 插入到变量声明语句
        (void) const_stmt_node->insert_son_node(decl_node);
    }
    return const_stmt_node;
}

/// @brief 非终结运算符constDef的遍历
/// @param ctx CST上下文
///	@return 常量定义结点
std::any MiniCCSTVisitor::visitConstDef(MiniCParser::ConstDefContext * ctx)
{
    // T_ID (T_L_SQBRA expr T_R_SQBRA)* T_ASSIGN initVal;

    // 声明语句节点
    ast_node * const_def_node = create_contain_node(ast_operator_type::AST_OP_CONST_DEF);

    auto constId = ctx->T_ID()->getText();
    int64_t lineNo = (int64_t) ctx->T_ID()->getSymbol()->getLine();
    auto constIdNode = ast_node::New(constId, lineNo);
    (void) const_def_node->insert_son_node(constIdNode);
    // TODO: [交流:array] 统一数组结点格式
    for (auto & exprCtx: ctx->expr()) {
        // 多维数组节点
        ast_node * const_val_node = std::any_cast<ast_node *>(visitExpr(exprCtx));
        (void) const_def_node->insert_son_node(const_val_node);
    }
    if (!ctx->initVal()) {
        // TODO: [语义检查] 处理throw问题
        printf("const without initialization.");
        return const_def_node;
    } else {
        auto initValNode = std::any_cast<ast_node *>(visitInitVal(ctx->initVal()));
        (void) const_def_node->insert_son_node(initValNode);
        return const_def_node;
    }
}

/// @brief 非终结运算符varDecl的遍历
/// @param ctx CST上下文
///	@return 变量声明结点（含子结点）
std::any MiniCCSTVisitor::visitVarDecl(MiniCParser::VarDeclContext * ctx)
{
    // varDecl: basicType varDef (T_COMMA varDef)* T_SEMICOLON;

    // 声明语句节点
    ast_node * stmt_node = create_contain_node(ast_operator_type::AST_OP_VAR_DECL_STMT);

    // 类型节点
    type_attr typeAttr = std::any_cast<type_attr>(visitBasicType(ctx->basicType()));

    for (auto & varCtx: ctx->varDef()) {
        // 变量名节点
        ast_node * id_node = std::any_cast<ast_node *>(visitVarDef(varCtx));

        // 创建类型节点
        ast_node * type_node = create_type_node(typeAttr);

        // 创建变量定义节点
        ast_node * decl_node = ast_node::New(ast_operator_type::AST_OP_VAR_DECL, type_node, id_node, nullptr);

        // 插入到变量声明语句
        (void) stmt_node->insert_son_node(decl_node);
    }

    return stmt_node;
}

/// @brief 非终结运算符varDef的遍历
/// @param ctx CST上下文
///	@return 变量定义结点
std::any MiniCCSTVisitor::visitVarDef(MiniCParser::VarDefContext * ctx)
{
    // varDef: T_ID (T_L_SQBRA expr T_R_SQBRA)* (T_ASSIGN initVal)?;
    ast_node * var_def_node = create_contain_node(ast_operator_type::AST_OP_VAR_DEF);
    auto varId = ctx->T_ID()->getText();
    // 获取行号
    int64_t lineNo = (int64_t) ctx->T_ID()->getSymbol()->getLine();
    auto varIdNode = ast_node::New(varId, lineNo);
    (void) var_def_node->insert_son_node(varIdNode);
    // TODO: [交流] 统一数组相关结点的表示方式
    for (auto & exprCtx: ctx->expr()) {
        // 多维数组节点
        ast_node * temp = std::any_cast<ast_node *>(visitExpr(exprCtx));
        (void) var_def_node->insert_son_node(temp);
    }
    if (ctx->initVal()) {
        auto initValNode = std::any_cast<ast_node *>(visitInitVal(ctx->initVal()));
        (void) var_def_node->insert_son_node(initValNode);
    }
    return var_def_node;
}
/// @brief 非终结运算符InitVal的遍历
/// @param ctx CST上下文
///	@return 下级结点
std::any MiniCCSTVisitor::visitInitVal(MiniCParser::InitValContext * ctx)
{
    if (Instanceof(singleValCtx, MiniCParser::SingleValContext *, ctx)) {
        return visitSingleVal(singleValCtx);
    } else if (Instanceof(multiValCtx, MiniCParser::MultiValContext *, ctx)) {
        return visitMultiVal(multiValCtx);
    }
    return nullptr;
}
/// @brief 非终结运算符singleVal的遍历
/// @param ctx CST上下文
///	@return 下级结点
std::any MiniCCSTVisitor::visitSingleVal(MiniCParser::SingleValContext * ctx)
{
    return visitExpr(ctx->expr());
}
/// @brief 非终结运算符multiVal的遍历
/// @param ctx CST上下文
///	@return 向量结点
std::any MiniCCSTVisitor::visitMultiVal(MiniCParser::MultiValContext * ctx)
{
    if (!ctx->initVal().empty()) {
        ast_node * initVal_node = create_contain_node(ast_operator_type::AST_OP_INIT_VAL);
        for (auto & initValCtx: ctx->initVal()) {
            auto valNode = std::any_cast<ast_node *>(visitInitVal(initValCtx));
            (void) initVal_node->insert_son_node(valNode);
        }
        return initVal_node;
    }
    return nullptr;
}

/// @brief 非终结运算符statement中的遍历
/// @param ctx CST上下文
/// @return 下级结点
std::any MiniCCSTVisitor::visitStatement(MiniCParser::StatementContext * ctx)
{
    // 识别的文法产生式：
    // statement:
    // 	T_RETURN expr? T_SEMICOLON										# returnStatement
    // 	| lVal T_ASSIGN expr T_SEMICOLON								# assignStatement
    // 	| block															# blockStatement
    // 	| expr? T_SEMICOLON												# expressionStatement
    // 	| T_IF T_L_PAREN cond T_R_PAREN statement (T_ELSE statement)?	# ifelseStatement
    // 	| T_WHILE T_L_PAREN cond T_R_PAREN statement					# whileStatement
    // 	| T_BREAK T_SEMICOLON											# breakStatement
    // 	| T_CONTINUE T_SEMICOLON										# continueStatement;
    if (Instanceof(assignCtx, MiniCParser::AssignStatementContext *, ctx)) {
        return visitAssignStatement(assignCtx);
    } else if (Instanceof(returnCtx, MiniCParser::ReturnStatementContext *, ctx)) {
        return visitReturnStatement(returnCtx);
    } else if (Instanceof(blockCtx, MiniCParser::BlockStatementContext *, ctx)) {
        return visitBlockStatement(blockCtx);
    } else if (Instanceof(exprCtx, MiniCParser::ExpressionStatementContext *, ctx)) {
        return visitExpressionStatement(exprCtx);
    } else if (Instanceof(ifelseCtx, MiniCParser::IfelseStatementContext *, ctx)) {
        return visitIfelseStatement(ifelseCtx);
    } else if (Instanceof(whileCtx, MiniCParser::WhileStatementContext *, ctx)) {
        return visitWhileStatement(whileCtx);
    } else if (Instanceof(breakCtx, MiniCParser::BreakStatementContext *, ctx)) {
        return visitBreakStatement(breakCtx);
    } else if (Instanceof(continueCtx, MiniCParser::ContinueStatementContext *, ctx)) {
        return visitContinueStatement(continueCtx);
    } else {
        return nullptr;
    }
}

/// @brief 非终结运算符statement中的returnStatement的遍历
/// @param ctx CST上下文
/// @return RETURNexprNode
std::any MiniCCSTVisitor::visitReturnStatement(MiniCParser::ReturnStatementContext * ctx)
{
    // 识别的文法产生式：returnStatement -> T_RETURN expr T_SEMICOLON

    // 非终结符，表达式expr遍历
    auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 创建返回节点，其孩子为Expr
    return create_contain_node(ast_operator_type::AST_OP_RETURN, exprNode);
}

/// @brief 非终结运算符statement中的assignStatement的遍历
/// @param ctx CST上下文
/// @return ASSIGNexprNode
std::any MiniCCSTVisitor::visitAssignStatement(MiniCParser::AssignStatementContext * ctx)
{
    // 识别文法产生式：assignStatement: lVal T_ASSIGN expr T_SEMICOLON

    // 赋值左侧左值Lval遍历产生节点
    auto lvalNode = std::any_cast<ast_node *>(visitLVal(ctx->lVal()));

    // 赋值右侧expr遍历
    auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 创建一个AST_OP_ASSIGN类型的中间节点，孩子为Lval和Expr
    return ast_node::New(ast_operator_type::AST_OP_ASSIGN, lvalNode, exprNode, nullptr);
}

/// @brief 非终结运算符statement中的blockStatement的遍历
/// @param ctx CST上下文
/// @return 下级结点 （block）
std::any MiniCCSTVisitor::visitBlockStatement(MiniCParser::BlockStatementContext * ctx)
{
    // 识别文法产生式 blockStatement: block

    return visitBlock(ctx->block());
}

/// @brief 非终结运算符statement中的returnStatement的遍历
/// @param ctx CST上下文
/// @return 下级结点（或空）
std::any MiniCCSTVisitor::visitExpressionStatement(MiniCParser::ExpressionStatementContext * ctx)
{
    // 识别文法产生式  expr ? T_SEMICOLON #expressionStatement;
    if (ctx->expr()) {
        // 表达式语句ff
        // 遍历expr非终结符，创建表达式节点后返回
        return visitExpr(ctx->expr());
    } else {
        // 空语句
        // 直接返回空指针，需要再把语句加入到语句块时要注意判断，空语句不要加入
        return nullptr;
    }
}

std::any MiniCCSTVisitor::visitIfelseStatement(MiniCParser::IfelseStatementContext * ctx)
{
    // TODO: [选择：非线性] 是否需要加一层结点表明各块功能？
    auto condNode = std::any_cast<ast_node *>(visitCond(ctx->cond()));
    auto ifstmtNode = std::any_cast<ast_node *>(visitStatement(ctx->statement()[0]));
    ast_node * elsestmtNode = nullptr;
    if (ctx->T_ELSE()) {
        elsestmtNode = std::any_cast<ast_node *>(visitStatement(ctx->statement()[1]));
    }
    return ast_node::New(ast_operator_type::AST_OP_IFELSE, condNode, ifstmtNode, elsestmtNode, nullptr);
}
std::any MiniCCSTVisitor::visitWhileStatement(MiniCParser::WhileStatementContext * ctx)
{
    auto condNode = std::any_cast<ast_node *>(visitCond(ctx->cond()));
	auto stmtNode = std::any_cast<ast_node *>(visitStatement(ctx->statement()));
    return ast_node::New(ast_operator_type::AST_OP_WHILE, condNode, stmtNode, nullptr);
}

std::any MiniCCSTVisitor::visitBreakStatement(MiniCParser::BreakStatementContext * ctx)
{
    // TODO: [非线性] Break操作（结点？）
    return ast_node::New(ast_operator_type::AST_OP_BREAK, nullptr);
}

std::any MiniCCSTVisitor::visitContinueStatement(MiniCParser::ContinueStatementContext * ctx)
{
    // TODO: [非线性] Continue操作（结点？）
    return ast_node::New(ast_operator_type::AST_OP_CONTINUE, nullptr);
}

/// @brief 非终结运算符expr的遍历
/// @param ctx CST上下文
/// @return 下级结点
std::any MiniCCSTVisitor::visitExpr(MiniCParser::ExprContext * ctx)
{
    // 识别产生式：expr: addExp;
    // TODO: [选择][交流] 是否考虑逻辑算术混合运算问题
    // return visitAddExp(ctx->addExp());
    return visitCond(ctx->cond()); //把addExp修改为cond
}

/// @brief 非终结运算符cond的遍历
/// @param ctx CST上下文
/// @return 下级结点
std::any MiniCCSTVisitor::visitCond(MiniCParser::CondContext * ctx)
{
    // TODO: [选择] 是否弃掉这层壳？
    return visitLOrExp(ctx->lOrExp());
}
/// @brief 非终结运算符lOrExp的遍历
/// @param ctx CST上下文
/// @return 或运算（左结合后）的根结点
std::any MiniCCSTVisitor::visitLOrExp(MiniCParser::LOrExpContext * ctx)
{
    // 识别的文法产生式：lOrExp : lAndExp (T_OR lAndExp)*;
    if (ctx->T_OR().empty()) {
        // 没有'||'运算符，则说明闭包识别为0，只识别了第一个非终结符lAndExp
        return visitLAndExp(ctx->lAndExp()[0]);
    }

    ast_node *left, *right;

    // 存在'||'运算符，记录方便按个数进行遍历
    auto opsCtxVec = ctx->T_OR();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int) opsCtxVec.size(); k++) {

        // 获取运算符
        ast_operator_type op = ast_operator_type::AST_OP_OR;

        if (k == 0) {

            // 左操作数
            left = std::any_cast<ast_node *>(visitLAndExp(ctx->lAndExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitLAndExp(ctx->lAndExp()[k + 1]));

        // 新建结点作为下一个运算符的左操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}
/// @brief 非终结运算符lAndExp的遍历
/// @param ctx CST上下文
/// @return 与运算（左结合后）的根结点
std::any MiniCCSTVisitor::visitLAndExp(MiniCParser::LAndExpContext * ctx)
{
    // 识别的文法产生式：lOrExp : eqExp (T_AND eqExp)*;
    if (ctx->T_AND().empty()) {
        // 没有'&&'运算符，则说明闭包识别为0，只识别了第一个非终结符eqExp
        return visitEqExp(ctx->eqExp()[0]);
    }
    ast_node *left, *right;
    // 存在'&&'运算符，记录方便按个数进行遍历
    auto opsCtxVec = ctx->T_AND();
    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int) opsCtxVec.size(); k++) {
        // 获取运算符
        ast_operator_type op = ast_operator_type::AST_OP_AND;
        if (k == 0) {
            // 左操作数
            left = std::any_cast<ast_node *>(visitEqExp(ctx->eqExp()[k]));
        }
        // 右操作数
        right = std::any_cast<ast_node *>(visitEqExp(ctx->eqExp()[k + 1]));
        // 新建结点作为下一个运算符的左操作符
        left = ast_node::New(op, left, right, nullptr);
    }
    return left;
}
/// @brief 非终结运算符eqExp的遍历
/// @param ctx CST上下文
/// @return 判等运算（左结合后）的根结点
std::any MiniCCSTVisitor::visitEqExp(MiniCParser::EqExpContext * ctx)
{
    // 识别的文法产生式：eqExp : relExp (eqOp relExp)*;
    if (ctx->eqOp().empty()) {
        // 没有eqOp运算符，则说明闭包识别为0，只识别了第一个非终结符relExp
        return visitRelExp(ctx->relExp()[0]);
    }

    ast_node *left, *right;

    // 存在eqOp运算符，记录方便按个数进行遍历
    auto opsCtxVec = ctx->eqOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int) opsCtxVec.size(); k++) {

        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitEqOp(opsCtxVec[k]));

        if (k == 0) {

            // 左操作数
            left = std::any_cast<ast_node *>(visitRelExp(ctx->relExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitRelExp(ctx->relExp()[k + 1]));

        // 新建结点作为下一个运算符的左操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}
/// @brief 非终结运算符eqOp的遍历
/// @param ctx CST上下文
/// @return [非结点] ast_operator_type 操作类型
std::any MiniCCSTVisitor::visitEqOp(MiniCParser::EqOpContext * ctx)
{
    if (ctx->T_EQ()) {
        return ast_operator_type::AST_OP_EQ;
    } else if (ctx->T_NEQ()) {
        return ast_operator_type::AST_OP_NEQ;
    } else {
        return ast_operator_type::AST_OP_MAX;
    }
}
/// @brief 非终结运算符relExp的遍历
/// @param ctx CST上下文
/// @return 比较运算（左结合后）的根结点
std::any MiniCCSTVisitor::visitRelExp(MiniCParser::RelExpContext * ctx)
{
    // 识别的文法产生式：relExp : addExp (relOp addExp)*;
    if (ctx->relOp().empty()) {
        // 没有relOp运算符，则说明闭包识别为0，只识别了第一个非终结符addExp
        return visitAddExp(ctx->addExp()[0]);
    }

    ast_node *left, *right;

    // 存在relOp运算符，记录方便按个数进行遍历
    auto opsCtxVec = ctx->relOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int) opsCtxVec.size(); k++) {
        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitRelOp(opsCtxVec[k]));

        if (k == 0) {

            // 左操作数
            left = std::any_cast<ast_node *>(visitAddExp(ctx->addExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitAddExp(ctx->addExp()[k + 1]));

        // 新建结点作为下一个运算符的左操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}
/// @brief 非终结运算符relOp的遍历
/// @param ctx CST上下文
/// @return [非结点] ast_operator_type 操作类型
std::any MiniCCSTVisitor::visitRelOp(MiniCParser::RelOpContext * ctx)
{
    if (ctx->T_GE()) {
        return ast_operator_type::AST_OP_GE;
    } else if (ctx->T_LE()) {
        return ast_operator_type::AST_OP_LE;
    } else if (ctx->T_GREATER()) {
        return ast_operator_type::AST_OP_GNE;
    } else if (ctx->T_LESS()) {
        return ast_operator_type::AST_OP_LNE;
    } else {
        return ast_operator_type::AST_OP_MAX;
    }
}
/// @brief 非终结运算符addExp的遍历
/// @param ctx CST上下文
/// @return 加减运算（左结合后）的根结点
std::any MiniCCSTVisitor::visitAddExp(MiniCParser::AddExpContext * ctx)
{
    // 识别的文法产生式：addExp : mulExp (addOp mulExp)*;
    if (ctx->addOp().empty()) {
        // 没有addOp运算符，则说明闭包识别为0，只识别了第一个非终结符mulExp
        return visitMulExp(ctx->mulExp()[0]);
    }

    ast_node *left, *right;

    // 存在addOp运算符，记录方便按个数进行遍历
    auto opsCtxVec = ctx->addOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int) opsCtxVec.size(); k++) {
        // TODO: [参考]需要区分位置的同类子结点可以直接使用forautoin，否则确定下标

        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitAddOp(opsCtxVec[k]));

        if (k == 0) {

            // 左操作数
            left = std::any_cast<ast_node *>(visitMulExp(ctx->mulExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitMulExp(ctx->mulExp()[k + 1]));

        // 新建结点作为下一个运算符的左操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}
/// @brief 非终结运算符addOp的遍历
/// @param ctx CST上下文
/// @return [非结点] ast_operator_type 操作类型
// TODO: [!!参考]注意上面的return,形成标准
std::any MiniCCSTVisitor::visitAddOp(MiniCParser::AddOpContext * ctx)
{
    // 识别的文法产生式：addOp : T_ADD | T_SUB
    // TODO: [参考] 运算结点如何返回
    if (ctx->T_ADD()) {
        return ast_operator_type::AST_OP_ADD;
    } else if (ctx->T_SUB()) {
        return ast_operator_type::AST_OP_SUB;
    } else {
        return ast_operator_type::AST_OP_MAX;
    }
}
/// @brief 非终结运算符mulExp的遍历
/// @param ctx CST上下文
/// @return 乘除余运算（左结合后）的根结点
std::any MiniCCSTVisitor::visitMulExp(MiniCParser::MulExpContext * ctx)
{
    // 识别的文法产生式：mulExp : unaryExp (mulOp unaryExp)*;
    if (ctx->mulOp().empty()) {
        // 没有mulOp运算符，则说明闭包识别为0，只识别了第一个非终结符unaryExp
        return visitUnaryExp(ctx->unaryExp()[0]);
    }

    ast_node *left, *right;

    // 存在mulOp运算符，记录方便按个数进行遍历
    auto opsCtxVec = ctx->mulOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int) opsCtxVec.size(); k++) {

        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitMulOp(opsCtxVec[k]));

        if (k == 0) {

            // 左操作数
            left = std::any_cast<ast_node *>(visitUnaryExp(ctx->unaryExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitUnaryExp(ctx->unaryExp()[k + 1]));

        // 新建结点作为下一个运算符的左操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}
/// @brief 非终结运算符mulOp的遍历
/// @param ctx CST上下文
/// @return [非结点] ast_operator_type 操作类型
std::any MiniCCSTVisitor::visitMulOp(MiniCParser::MulOpContext * ctx)
{
    if (ctx->T_MUL()) {
        return ast_operator_type::AST_OP_MUL;
    } else if (ctx->T_DIV()) {
        return ast_operator_type::AST_OP_DIV;
    } else if (ctx->T_MOD()) {
        return ast_operator_type::AST_OP_MOD;
    } else {
        return ast_operator_type::AST_OP_MAX;
    }
}

/// @brief 非终结运算符unaryExp的遍历
/// @param ctx CST上下文
/// @return 下级结点
std::any MiniCCSTVisitor::visitUnaryExp(MiniCParser::UnaryExpContext * ctx)
{
    // 识别文法产生式：unaryExp: (unaryOp)* primaryExp
    // TODO: [单目] 单目运算表达式结点
    if (ctx->unaryOp().empty()) {
        // 没有unaryOp运算符，则说明闭包识别为0，只识别了唯一的primaryExp
        return visitPrimaryExp(ctx->primaryExp());
    }
    ast_node * right;
    // 存在unaryOp运算符，记录方便按个数进行遍历
    auto opsCtxVec = ctx->unaryOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = (int) opsCtxVec.size() - 1; k >= 0; k--) {

        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitUnaryOp(opsCtxVec[k]));

        if (k == (int) opsCtxVec.size() - 1) {

            // 右操作数
            right = std::any_cast<ast_node *>(visitPrimaryExp(ctx->primaryExp()));
        }
        // 新建结点作为下一个运算符的右操作符
        right = ast_node::New(op, right, nullptr);
    }
    return right;
}
std::any MiniCCSTVisitor::visitUnaryOp(MiniCParser::UnaryOpContext * ctx)
{
    if (ctx->T_ADD()) {
        return ast_operator_type::AST_OP_POS;
    } else if (ctx->T_SUB()) {
        return ast_operator_type::AST_OP_NEG;
    } else if (ctx->T_NOT()) {
        return ast_operator_type::AST_OP_NOT;
    } else {
        return ast_operator_type::AST_OP_MAX;
    }
}

/// @brief 非终结运算符PrimaryExp的遍历
/// @param ctx CST上下文
/// @return 下级结点
std::any MiniCCSTVisitor::visitPrimaryExp(MiniCParser::PrimaryExpContext * ctx)
{
    // 识别文法产生式 primaryExp: T_L_PAREN expr T_R_PAREN | number | lVal | T_ID T_L_PAREN funcRParams? T_R_PAREN;
    if (Instanceof(pexprCtx, MiniCParser::ParenExprContext *, ctx)) {
        return visitParenExpr(pexprCtx);
    } else if (Instanceof(lvalCtx, MiniCParser::LeftValueContext *, ctx)) {
        return visitLeftValue(lvalCtx);
    } else if (Instanceof(bnumCtx, MiniCParser::BasicNumContext *, ctx)) {
        return visitBasicNum(bnumCtx);
    } else if (Instanceof(funcCtx, MiniCParser::FuncCallContext *, ctx)) {
        return visitFuncCall(funcCtx);
    } else {
        return nullptr;
    }
    // TODO: 完成基本表达式匹配
    ast_node * node = nullptr;
    // if (ctx->number()) {
    //     // 无符号整型字面量
    //     // 识别 primaryExp: number
    //     node = std::any_cast<ast_node *>(visitNumber(ctx->number()));
    // } else if (ctx->lVal()) {
    //     // 具有左值的表达式
    //     // 识别 primaryExp: lVal
    //     node = std::any_cast<ast_node *>(visitLVal(ctx->lVal()));
    // } else if (ctx->expr()) {
    //     // 带有括号的表达式
    //     // primaryExp: T_L_PAREN expr T_R_PAREN
    //     node = std::any_cast<ast_node *>(visitExpr(ctx->expr()));
    // }

    return node;
}
/// @brief 非终结运算符parenExpr的遍历
/// @param ctx CST上下文
/// @return 括号结点
std::any MiniCCSTVisitor::visitParenExpr(MiniCParser::ParenExprContext * ctx)
{
    // TODO:完成括号结点（？还是说就这？）
    return visitExpr(ctx->expr());
}
/// @brief 非终结运算符LeftValue的遍历
/// @param ctx CST上下文
/// @return 下级结点
std::any MiniCCSTVisitor::visitLeftValue(MiniCParser::LeftValueContext * ctx)
{
    return visitLVal(ctx->lVal());
}
/// @brief 非终结运算符lVal的遍历
/// @param ctx CST上下文
/// @return LValnode
std::any MiniCCSTVisitor::visitLVal(MiniCParser::LValContext * ctx)
{
    // 识别文法产生式：lVal: T_ID;
    // 获取ID的名字
    auto varId = ctx->T_ID()->getText();

    // 获取行号
    int64_t lineNo = (int64_t) ctx->T_ID()->getSymbol()->getLine();
    // TODO: [数组] 完成数组匹配
    // for (auto ctxarr: ctx->expr()) {

    // }
    return ast_node::New(varId, lineNo);
}

/// @brief 非终结运算符BasicNum的遍历
/// @param ctx CST上下文
/// @return 下级结点
std::any MiniCCSTVisitor::visitBasicNum(MiniCParser::BasicNumContext * ctx)
{
    return visitNumber(ctx->number());
}

/// @brief 非终结运算符number的遍历
/// @param ctx CST上下文
/// @return numbernode 数结点
std::any MiniCCSTVisitor::visitNumber(MiniCParser::NumberContext * ctx)
{
    ast_node * numberNode = nullptr;
    if (ctx->T_INT_DIGIT()) {
        uint32_t val = (uint32_t) stoull(ctx->T_INT_DIGIT()->getText());
        int64_t lineNo = (int64_t) ctx->T_INT_DIGIT()->getSymbol()->getLine();
        numberNode = ast_node::New(digit_int_attr{val, lineNo});
    }
    // else if (ctx->T_FLOAT_DIGIT()) {
    // 	   //TODO: [交流:fp]了解sysY的float标准
    //     float_t val = (float_t) stoull(ctx->T_FLOAT_DIGIT()->getText());
    //     int64_t lineNo = (int64_t) ctx->T_FLOAT_DIGIT()->getSymbol()->getLine();
    //     //TODO: [fp]ast_node::New 扩展digit_float_attr
    //     numberNode = ast_node::New(digit_float_attr{val, lineNo});
    // }
    return numberNode;
}
std::any MiniCCSTVisitor::visitFuncCall(MiniCParser::FuncCallContext * ctx)
{
    // 识别的文法产生式：T_ID T_L_PAREN funcRParams? T_R_PAREN
    // TODO: [函数] 函数调用结点（->实参）
    char * id = strdup(ctx->T_ID()->getText().c_str());
    ast_node * funcname_node = ast_node::New(id, (int64_t) ctx->T_ID()->getSymbol()->getLine());
    ast_node * formalParamsNode = nullptr;
    if (ctx->funcRParams()) {
        formalParamsNode = std::any_cast<ast_node *>(visitFuncRParams(ctx->funcRParams()));
    }
    return create_func_call(funcname_node,formalParamsNode);
}
std::any MiniCCSTVisitor::visitFuncRParams(MiniCParser::FuncRParamsContext * ctx)
{
    // 识别的文法产生式：realParamList : expr (T_COMMA expr)*;
    // TODO: [函数] 实参列表结点
    // TODO: [数组] 确认数组支持
    auto paramListNode = create_contain_node(ast_operator_type::AST_OP_FUNC_REAL_PARAMS);

    for (auto paramCtx: ctx->expr()) {

        auto paramNode = std::any_cast<ast_node *>(visitExpr(paramCtx));

        paramListNode->insert_son_node(paramNode);
    }

    return paramListNode;
}