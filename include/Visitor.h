#pragma once
#include "Nodes.h"
#include "Object.h"

class Visitor
{
    public:
        virtual void visitBreakStmt(Break* stmt) = 0;
        virtual void visitBlockStmt(Block* stmt) = 0;
        // virtual void visitClassStmt(Class* stmt) = 0;
        virtual void visitContinueStmt(Continue* stmt) = 0;
        virtual void visitFunctionStmt(Function* stmt) = 0;
        virtual void visitIfStmt(If* stmt) = 0;
        virtual void visitExpressionStmt(Expression* stmt) = 0;
        virtual void visitPrintStmt(Print* stmt) = 0;
        virtual void visitReturnStmt(Return* stmt) = 0;
        virtual void visitVarStmt(Var* stmt) = 0;
        virtual void visitWhileStmt(While* stmt) = 0;

        virtual Object visitAssignExpr(Assign* expr) = 0;
        virtual Object visitBinaryExpr(Binary* expr) = 0;
        virtual Object visitCallExpr(Call* expr) = 0;
        virtual Object visitCommaExpr(Comma* expr) = 0;
        // virtual Object visitGetExpr(Get* expr) = 0;
        virtual Object visitGroupingExpr(Grouping* expr) = 0;
        virtual Object visitLambdaExpr(Lambda* expr) = 0;
        virtual Object visitLiteralExpr(Literal* expr) = 0;
        virtual Object visitLogicalExpr(Logical* expr) = 0;
        // virtual Object visitSetExpr(Set* expr) = 0;
        // virtual Object visitSuperExpr(Super* expr) = 0;
        virtual Object visitTernaryExpr(Ternary* expr) = 0;
        // virtual Object visitThisExpr(This* expr) = 0;
        virtual Object visitUnaryExpr(Unary* expr) = 0;
        virtual Object visitVariableExpr(Variable* expr) = 0;
};