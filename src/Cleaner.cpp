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

void Cleaner::clean(Stmt* &stmt)
{
    if (stmt != nullptr)
        stmt->remove(*this, stmt);
}

void Cleaner::clean(Expr* &expr)
{
    if (expr != nullptr)
        (void) expr->remove(*this, expr);
}

bool Cleaner::cleanable(Stmt* &stmt)
{
    auto check = dynamic_cast<Declaration *>(stmt);
    if (check) return false;
    return true;
}

// Statement methods.

void Cleaner::visitBreakStmt(Break* &stmt)
{
    delete stmt;
    stmt = nullptr;
}

// Revise.
void Cleaner::visitBlockStmt(Block* &stmt)
{
    for (Stmt* stmt : stmt->statements)
    {
        if (cleanable(stmt)) clean(stmt);
    }

    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitClassStmt(Class* &stmt)
{
    clean(stmt->superclass);
    clean(stmt->methods);
    clean(stmt->classMethods);

    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitContinueStmt(Continue* &stmt)
{
    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitExpressionStmt(Expression* &stmt)
{
    clean(stmt->expression);
    delete stmt;
}

void Cleaner::visitFetchStmt(Fetch* &stmt)
{
    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitFunctionStmt(Function* &stmt)
{
    delete stmt->params;
    clean(stmt->body);
    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitIfStmt(If* &stmt)
{
    clean(stmt->condition);
    clean(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) clean(stmt->elseBranch);
    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitPrintStmt(Print* &stmt)
{
    clean(stmt->expression);
    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitReturnStmt(Return* &stmt)
{
    clean(stmt->value);
    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitVarStmt(Var* &stmt)
{
    if (stmt->initializer != nullptr) clean(stmt->initializer);
    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitWhileStmt(While* &stmt)
{
    clean(stmt->condition);
    clean(stmt->body);
    delete stmt;
    stmt = nullptr;
}

void Cleaner::visitAssignExpr(Assign* &expr)
{
    clean(expr->value);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitBinaryExpr(Binary* &expr)
{
    clean(expr->left);
    clean(expr->right);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitCallExpr(Call* &expr)
{
    clean(expr->callee);
    clean(expr->arguments);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitCommaExpr(Comma* &expr)
{
    clean(expr->expressions);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitGetExpr(Get* &expr)
{
    clean(expr->object);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitGroupingExpr(Grouping* &expr)
{
    clean(expr->expression);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitLambdaExpr(Lambda* &expr)
{
    clean(expr->body);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitListExpr(List* &expr)
{
    for (Expr* element : expr->elements)
        clean(element);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitLiteralExpr(Literal* &expr)
{
    delete expr;
    expr = nullptr;
}

void Cleaner::visitLogicalExpr(Logical* &expr)
{
    clean(expr->left);
    clean(expr->right);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitSetExpr(Set* &expr)
{
    clean(expr->object);
    clean(expr->value);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitSuperExpr(Super* &expr)
{
    delete expr;
    expr = nullptr;
}

void Cleaner::visitTernaryExpr(Ternary* &expr)
{
    clean(expr->condition);
    clean(expr->trueBranch);
    clean(expr->falseBranch);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitThisExpr(This* &expr)
{
    delete expr;
    expr = nullptr;
}

void Cleaner::visitUnaryExpr(Unary* &expr)
{
    clean(expr->right);
    delete expr;
    expr = nullptr;
}

void Cleaner::visitVariableExpr(Variable* &expr)
{
    delete expr;
    expr = nullptr;
}