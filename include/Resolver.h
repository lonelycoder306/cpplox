#pragma once
#include "Expr.h"
#include "Interpreter.h"
#include "Nodes.h"
#include "Stmt.h"
#include "Token.h"
#include "Visitor.h"
#include <map>
#include <string>
#include <vector>

class Resolver : public Visitor
{
    private:
        Interpreter* interpreter;
        std::vector<std::map<std::string, int>> scopes;
        enum FunctionType { NOFUNC, FUNCTION, LAMBDA, INITIALIZER, METHOD };
        enum ClassType { NOCLASS, CLASS, SUBCLASS };
        FunctionType currentFunction = NOFUNC;
        ClassType currentClass = NOCLASS;
    
    public:
        Resolver(Interpreter& interpreter);

        // General methods.

        void beginScope();
        void endScope();
        void declare(Token name);
        void define(Token name);
        void resolve(vpS statements);
        void resolve(Stmt* stmt);
        void resolve(Expr* expr);
        void resolveLocal(Expr* expr, Token name);
        void resolveFunction(Function* function, FunctionType type);
        void resolveLambda(Lambda* lambda, FunctionType type);

        // Statement methods.

        void visitBreakStmt(Break* stmt) override;
        void visitBlockStmt(Block* stmt) override;
        void visitClassStmt(Class* stmt) override;
        void visitContinueStmt(Continue* stmt) override;
        void visitExpressionStmt(Expression* stmt) override;
        void visitFetchStmt(Fetch* stmt) override;
        void visitFunctionStmt(Function* stmt) override;
        void visitIfStmt(If* stmt) override;
        void visitPrintStmt(Print* stmt) override;
        void visitReturnStmt(Return* stmt) override;
        void visitVarStmt(Var* stmt) override;
        void visitWhileStmt(While* stmt) override;

        // Expression methods.
        // Return value can be Object(nullptr).
        // Will be discarded anyway.

        Object visitAssignExpr(Assign* expr) override;
        Object visitBinaryExpr(Binary* expr) override;
        Object visitCallExpr(Call* expr) override;
        Object visitCommaExpr(Comma* expr) override;
        Object visitGetExpr(Get* expr) override;
        Object visitGroupingExpr(Grouping* expr) override;
        Object visitLambdaExpr(Lambda* expr) override;
        Object visitLiteralExpr(Literal* expr) override;
        Object visitLogicalExpr(Logical* expr) override;
        Object visitSetExpr(Set* expr) override;
        Object visitSuperExpr(Super* expr) override;
        Object visitTernaryExpr(Ternary* expr) override;
        Object visitThisExpr(This* expr) override;
        Object visitUnaryExpr(Unary* expr) override;
        Object visitVariableExpr(Variable* expr) override;
};