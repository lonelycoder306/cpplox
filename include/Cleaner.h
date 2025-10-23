#pragma once
#include "Nodes.h"

class Cleaner
{
    public:
        Cleaner() = default;
        void clean(vpS statements);
        void clean(Stmt* &stmt);
        bool cleanable(Stmt* &stmt);

        void visitBreakStmt(Break* &stmt);
        void visitBlockStmt(Block* &stmt);
        void visitClassStmt(Class* &stmt);
        void visitContinueStmt(Continue* &stmt);
        void visitExpressionStmt(Expression* &stmt);
        void visitFetchStmt(Fetch* &stmt);
        void visitFunctionStmt(Function* &stmt);
        void visitIfStmt(If* &stmt);
        void visitPrintStmt(Print* &stmt);
        void visitReturnStmt(Return* &stmt);
        void visitVarStmt(Var* &stmt);
        void visitWhileStmt(While* &stmt);

        void visitAssignExpr(Assign* &expr);
        void visitBinaryExpr(Binary* &expr);
        void visitCallExpr(Call* &expr);
        void visitCommaExpr(Comma* &expr);
        void visitGetExpr(Get* &expr);
        void visitGroupingExpr(Grouping* &expr);
        void visitLambdaExpr(Lambda* &expr);
        void visitListExpr(List* &expr);
        void visitLiteralExpr(Literal* &expr);
        void visitLogicalExpr(Logical* &expr);
        void visitSetExpr(Set* &expr);
        void visitSuperExpr(Super* &expr);
        void visitTernaryExpr(Ternary* &expr);
        void visitThisExpr(This* &expr);
        void visitUnaryExpr(Unary* &expr);
        void visitVariableExpr(Variable* &expr);

    private:
        void clean(vpE expressions);
        void clean(Expr* &expr);
};