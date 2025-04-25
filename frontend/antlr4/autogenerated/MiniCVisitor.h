
// Generated from /home/code/compilerSys/develop/frontend/antlr4/MiniC.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "MiniCParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by MiniCParser.
 */
class  MiniCVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by MiniCParser.
   */
    virtual std::any visitCompileUnit(MiniCParser::CompileUnitContext *context) = 0;

    virtual std::any visitFuncDef(MiniCParser::FuncDefContext *context) = 0;

    virtual std::any visitFuncType(MiniCParser::FuncTypeContext *context) = 0;

    virtual std::any visitFuncFParams(MiniCParser::FuncFParamsContext *context) = 0;

    virtual std::any visitFuncFParam(MiniCParser::FuncFParamContext *context) = 0;

    virtual std::any visitBlock(MiniCParser::BlockContext *context) = 0;

    virtual std::any visitBlockItemList(MiniCParser::BlockItemListContext *context) = 0;

    virtual std::any visitBlockItem(MiniCParser::BlockItemContext *context) = 0;

    virtual std::any visitDecl(MiniCParser::DeclContext *context) = 0;

    virtual std::any visitBasicType(MiniCParser::BasicTypeContext *context) = 0;

    virtual std::any visitConstDecl(MiniCParser::ConstDeclContext *context) = 0;

    virtual std::any visitConstDef(MiniCParser::ConstDefContext *context) = 0;

    virtual std::any visitVarDecl(MiniCParser::VarDeclContext *context) = 0;

    virtual std::any visitVarDef(MiniCParser::VarDefContext *context) = 0;

    virtual std::any visitSingleVal(MiniCParser::SingleValContext *context) = 0;

    virtual std::any visitMultiVal(MiniCParser::MultiValContext *context) = 0;

    virtual std::any visitReturnStatement(MiniCParser::ReturnStatementContext *context) = 0;

    virtual std::any visitAssignStatement(MiniCParser::AssignStatementContext *context) = 0;

    virtual std::any visitBlockStatement(MiniCParser::BlockStatementContext *context) = 0;

    virtual std::any visitExpressionStatement(MiniCParser::ExpressionStatementContext *context) = 0;

    virtual std::any visitIfelseStatement(MiniCParser::IfelseStatementContext *context) = 0;

    virtual std::any visitWhileStatement(MiniCParser::WhileStatementContext *context) = 0;

    virtual std::any visitBreakStatement(MiniCParser::BreakStatementContext *context) = 0;

    virtual std::any visitContinueStatement(MiniCParser::ContinueStatementContext *context) = 0;

    virtual std::any visitExpr(MiniCParser::ExprContext *context) = 0;

    virtual std::any visitCond(MiniCParser::CondContext *context) = 0;

    virtual std::any visitLOrExp(MiniCParser::LOrExpContext *context) = 0;

    virtual std::any visitLAndExp(MiniCParser::LAndExpContext *context) = 0;

    virtual std::any visitEqExp(MiniCParser::EqExpContext *context) = 0;

    virtual std::any visitEqOp(MiniCParser::EqOpContext *context) = 0;

    virtual std::any visitRelExp(MiniCParser::RelExpContext *context) = 0;

    virtual std::any visitRelOp(MiniCParser::RelOpContext *context) = 0;

    virtual std::any visitAddExp(MiniCParser::AddExpContext *context) = 0;

    virtual std::any visitAddOp(MiniCParser::AddOpContext *context) = 0;

    virtual std::any visitMulExp(MiniCParser::MulExpContext *context) = 0;

    virtual std::any visitMulOp(MiniCParser::MulOpContext *context) = 0;

    virtual std::any visitMono(MiniCParser::MonoContext *context) = 0;

    virtual std::any visitFuncCall(MiniCParser::FuncCallContext *context) = 0;

    virtual std::any visitUnaryOp(MiniCParser::UnaryOpContext *context) = 0;

    virtual std::any visitFuncRParams(MiniCParser::FuncRParamsContext *context) = 0;

    virtual std::any visitPrimaryExp(MiniCParser::PrimaryExpContext *context) = 0;

    virtual std::any visitLVal(MiniCParser::LValContext *context) = 0;

    virtual std::any visitNumber(MiniCParser::NumberContext *context) = 0;


};

