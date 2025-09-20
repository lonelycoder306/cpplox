#pragma once
#include "Nodes.h"
#include "Object.h"
#include "Visitor.h"

class Cleaner : public Visitor
{
    public:
        Cleaner() = default;
        void clean(vpS statements);
        void clean(Stmt* stmt);
        static bool cleanable(Stmt* stmt);

    private:
        void clean(vpE expressions);
        void clean(Expr* expr);

        void visitBreakStmt(Break* stmt) override;
        void visitBlockStmt(Block* stmt) override;
        // void visitClassStmt(Class* stmt) override;
        void visitContinueStmt(Continue* stmt) override;
        void visitFunctionStmt(Function* stmt) override;
        void visitIfStmt(If* stmt) override;
        void visitExpressionStmt(Expression* stmt) override;
        void visitPrintStmt(Print* stmt) override;
        void visitReturnStmt(Return* stmt) override;
        void visitVarStmt(Var* stmt) override;
        void visitWhileStmt(While* stmt) override;

        Object visitAssignExpr(Assign* expr) override;
        Object visitBinaryExpr(Binary* expr) override;
        Object visitCallExpr(Call* expr) override;
        Object visitCommaExpr(Comma* expr) override;
        // Object visitGetExpr(Get* expr) override;
        Object visitGroupingExpr(Grouping* expr) override;
        Object visitLambdaExpr(Lambda* expr) override;
        Object visitLiteralExpr(Literal* expr) override;
        Object visitLogicalExpr(Logical* expr) override;
        // Object visitSetExpr(Set* expr) override;
        // Object visitSuperExpr(Super* expr) override;
        Object visitTernaryExpr(Ternary* expr) override;
        // Object visitThisExpr(This* expr) override;
        Object visitUnaryExpr(Unary* expr) override;
        Object visitVariableExpr(Variable* expr) override;
};