///
/// @file Antlr4CSTVisitor.h
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
#pragma once

#include "AST.h"
#include "MiniCBaseVisitor.h"
#include "MiniCParser.h"

/// @brief 遍历具体语法树产生抽象语法树
class MiniCCSTVisitor : public MiniCBaseVisitor {

public:
    /// @brief 构造函数
    MiniCCSTVisitor();

    /// @brief 析构函数
    virtual ~MiniCCSTVisitor();

    /// @brief 遍历CST产生AST
    /// @param root CST语法树的根结点
    /// @return AST的根节点
    ast_node * run(MiniCParser::CompileUnitContext * root);

protected:
    /* 下面的函数都是从MiniCBaseVisitor继承下来的虚拟函数，需要重载实现 */
    /* 少数几个并非虚拟函数，不带override */
    /// @brief 非终结运算符compileUnit的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitCompileUnit(MiniCParser::CompileUnitContext * ctx) override;

    /// @brief 非终结运算符funcDef的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitFuncDef(MiniCParser::FuncDefContext * ctx) override;

    /// @brief 非终结运算符funcType的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitFuncType(MiniCParser::FuncTypeContext * ctx) override;

    /// @brief 非终结符FuncFParams的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitFuncFParams(MiniCParser::FuncFParamsContext * ctx) override;

    /// @brief 非终结符FuncRParam的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitFuncFParam(MiniCParser::FuncFParamContext * ctx) override;

    /// @brief 非终结运算符block的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitBlock(MiniCParser::BlockContext * ctx) override;

    /// @brief 非终结运算符blockItemList的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitBlockItemList(MiniCParser::BlockItemListContext * ctx) override;

    /// @brief 非终结运算符blockItem的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitBlockItem(MiniCParser::BlockItemContext * ctx) override;

    /// @brief 非终结符decl的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitDecl(MiniCParser::DeclContext * ctx) override;

    ///
    /// @brief 非终结符basicType的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    ///
    std::any visitBasicType(MiniCParser::BasicTypeContext * ctx) override;

    /// @brief 非终结运算符constDecl中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitConstDecl(MiniCParser::ConstDeclContext * ctx) override;

    /// @brief 非终结运算符constDef中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitConstDef(MiniCParser::ConstDefContext * ctx) override;

    /// @brief 非终结运算符varDecl中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitVarDecl(MiniCParser::VarDeclContext * ctx) override;

    /// @brief 非终结运算符varDef中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitVarDef(MiniCParser::VarDefContext * ctx) override;

    /// @brief 非终结运算符initVal中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitInitVal(MiniCParser::InitValContext * ctx);

    /// @brief 非终结运算符singleVal中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitSingleVal(MiniCParser::SingleValContext * ctx) override;

    /// @brief 非终结运算符multiVal中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitMultiVal(MiniCParser::MultiValContext * ctx) override;

    /// @brief 非终结运算符statement中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitStatement(MiniCParser::StatementContext * ctx);

    /// @brief 非终结运算符statement中的returnStatement的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitReturnStatement(MiniCParser::ReturnStatementContext * ctx) override;

    /// @brief 内部产生的非终结符assignStatement的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitAssignStatement(MiniCParser::AssignStatementContext * ctx) override;

    /// @brief 非终结运算符blockStatement中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitBlockStatement(MiniCParser::BlockStatementContext * ctx) override;

    /// @brief 非终结符ExpressionStatement的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitExpressionStatement(MiniCParser::ExpressionStatementContext * ctx) override;

    /// @brief 非终结运算符ifelseStatement中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitIfelseStatement(MiniCParser::IfelseStatementContext * ctx) override;

    /// @brief 非终结运算符whileStatement中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitWhileStatement(MiniCParser::WhileStatementContext * ctx) override;

    /// @brief 非终结运算符breakStatement中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitBreakStatement(MiniCParser::BreakStatementContext * ctx) override;

    /// @brief 非终结运算符continueStatement中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitContinueStatement(MiniCParser::ContinueStatementContext * ctx) override;

    /// @brief 非终结运算符expr的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitExpr(MiniCParser::ExprContext * ctx) override;

    /// @brief 非终结运算符cond中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitCond(MiniCParser::CondContext * ctx) override;

    /// @brief 非终结运算符lOrExp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitLOrExp(MiniCParser::LOrExpContext * ctx) override;

    /// @brief 非终结运算符andExp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitLAndExp(MiniCParser::LAndExpContext * ctx) override;

    /// @brief 非终结运算符eqExp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitEqExp(MiniCParser::EqExpContext * ctx) override;

    /// @brief 非终结运算符eqOp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitEqOp(MiniCParser::EqOpContext * ctx) override;

    /// @brief 非终结运算符relExp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitRelExp(MiniCParser::RelExpContext * ctx) override;

    /// @brief 非终结运算符relOp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitRelOp(MiniCParser::RelOpContext * ctx) override;

    /// @brief 非终结运算符addExp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitAddExp(MiniCParser::AddExpContext * ctx) override;

    /// @brief 非终结运算符addOp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitAddOp(MiniCParser::AddOpContext * ctx) override;

    /// @brief 非终结运算符mulExp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitMulExp(MiniCParser::MulExpContext * ctx) override;

    /// @brief 非终结运算符mulOp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitMulOp(MiniCParser::MulOpContext * ctx) override;

    /// @brief 非终结运算符unaryExp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitUnaryExp(MiniCParser::UnaryExpContext * ctx) override;

    /// @brief 非终结运算符unaryOp中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitUnaryOp(MiniCParser::UnaryOpContext * ctx) override;

    /// @brief 非终结符PrimaryExp的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitPrimaryExp(MiniCParser::PrimaryExpContext * ctx);

    /// @brief 非终结符parenExpr的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitParenExpr(MiniCParser::ParenExprContext * ctx) override;

    /// @brief 非终结符leftValue的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitLeftValue(MiniCParser::LeftValueContext * ctx) override;

    /// @brief 非终结符basicNum的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitBasicNum(MiniCParser::BasicNumContext * ctx) override;

    /// @brief 非终结符funcCall的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitFuncCall(MiniCParser::FuncCallContext * ctx) override;

    /// @brief 非终结符FuncRParams的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitFuncRParams(MiniCParser::FuncRParamsContext * ctx) override;

    /// @brief 非终结符lVal的分析
    /// @param ctx CST上下文
    /// @return std::any AST的节点
    std::any visitLVal(MiniCParser::LValContext * ctx) override;

    /// @brief 非终结运算符number中的遍历
    /// @param ctx CST上下文
    /// @return AST的节点
    std::any visitNumber(MiniCParser::NumberContext * ctx) override;
};