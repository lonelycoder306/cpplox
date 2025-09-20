#include "../include/Cleaner.h"
#include "../include/Expr.h"
#include "../include/Nodes.h"
#include "../include/Object.h"
#include "../include/Stmt.h"

// General methods.

void Cleaner::clean(vpS statements)
{
    for (Stmt* stmt : statements)
    {
        if (stmt != nullptr) clean(stmt);
    }
}

void Cleaner::clean(vpE expressions)
{
    for (Expr* expr : expressions)
    {
        if (expr != nullptr) clean(expr);
    }
}

void Cleaner::clean(Stmt* stmt)
{
    if (stmt != nullptr)
        stmt->accept(*this);
}

void Cleaner::clean(Expr* expr)
{
    (void) expr->accept(*this);
}

bool Cleaner::cleanable(Stmt* stmt)
{
    auto check = dynamic_cast<Declaration *>(stmt);
    if (!check) return false;
    return true;
}

// Statement methods.

void Cleaner::visitBreakStmt(Break* stmt)
{
    delete stmt;
}

// Revise.
void Cleaner::visitBlockStmt(Block* stmt)
{
    for (Stmt* stmt : stmt->statements)
    {
        if (cleanable(stmt)) clean(stmt);
    }

    delete stmt;
}

void Cleaner::visitContinueStmt(Continue* stmt)
{
    delete stmt;
}

void Cleaner::visitFunctionStmt(Function* stmt)
{
    delete stmt->params;
    clean(stmt->body);
    delete stmt;
}

void Cleaner::visitIfStmt(If* stmt)
{
    clean(stmt->condition);
    clean(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) clean(stmt->elseBranch);
    delete stmt;
}

void Cleaner::visitExpressionStmt(Expression* stmt)
{
    clean(stmt->expression);
    delete stmt;
}

void Cleaner::visitPrintStmt(Print* stmt)
{
    clean(stmt->expression);
    delete stmt;
}

void Cleaner::visitReturnStmt(Return* stmt)
{
    clean(stmt->value);
    delete stmt;
}

void Cleaner::visitVarStmt(Var* stmt)
{
    if (stmt->initializer != nullptr) clean(stmt->initializer);
    delete stmt;
}

void Cleaner::visitWhileStmt(While* stmt)
{
    clean(stmt->condition);
    clean(stmt->body);
    delete stmt;
}

Object Cleaner::visitAssignExpr(Assign* expr)
{
    clean(expr->value);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitBinaryExpr(Binary* expr)
{
    clean(expr->left);
    clean(expr->right);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitCallExpr(Call* expr)
{
    clean(expr->callee);
    clean(expr->arguments);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitCommaExpr(Comma* expr)
{
    clean(expr->expressions);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitGroupingExpr(Grouping* expr)
{
    clean(expr->expression);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitLambdaExpr(Lambda* expr)
{
    clean(expr->body);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitLiteralExpr(Literal* expr)
{
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitLogicalExpr(Logical* expr)
{
    clean(expr->left);
    clean(expr->right);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitTernaryExpr(Ternary* expr)
{
    clean(expr->condition);
    clean(expr->trueBranch);
    clean(expr->falseBranch);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitUnaryExpr(Unary* expr)
{
    clean(expr->right);
    delete expr;
    return Object(nullptr);
}

Object Cleaner::visitVariableExpr(Variable* expr)
{
    delete expr;
    return Object(nullptr);
}