#include "../include/Resolver.h"
#include "../include/Error.h"
#include "../include/Expr.h"
#include "../include/Interpreter.h"
#include "../include/Nodes.h"
#include "../include/Object.h"
#include "../include/Stmt.h"
#include "../include/Token.h"
#include <string>

// Constructor.

Resolver::Resolver(Interpreter interpreter)
{
    this->interpreter = interpreter;
}

// General methods.

void Resolver::beginScope()
{
    scopes.push_back({});
}

void Resolver::endScope()
{
    scopes.pop_back();
}

void Resolver::declare(Token name)
{
    if (scopes.size() == 0) return;

    std::map<std::string, bool> scope = scopes[scopes.size() - 1];
    if (scope.contains(name.lexeme))
        throw ResolveError(name, "Already a variable with this name in this scope.");
    scopes[scopes.size() - 1][name.lexeme] = false;
}

void Resolver::define(Token name)
{
    if (scopes.size() == 0) return;
    scopes[scopes.size() - 1][name.lexeme] = true;
}

void Resolver::resolve(vpS statements)
{
    for (Stmt* stmt: statements)
        resolve(stmt);
}

void Resolver::resolve(Stmt* stmt)
{
    stmt->accept(*this);
}

void Resolver::resolve(Expr* expr)
{
    expr->accept(*this); // Unused return value.
}

void Resolver::resolveLocal(Expr* expr, Token name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        if (scopes[i].contains(name.lexeme))
        {
            interpreter.resolve(expr, scopes.size() - 1 - i);
        }
    }
}

void Resolver::resolveFunction(Function* function, FunctionType type)
{
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;
    beginScope();
    if (function->params != nullptr)
    {
        for (Token param : *(function->params))
        {
            declare(param);
            define(param);
        }
    }
    resolve(function->body);
    endScope();
    currentFunction = enclosingFunction;
}

void Resolver::resolveLambda(Lambda* lambda, FunctionType type)
{
    FunctionType enclosingLambda = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token param : lambda->params)
    {
        declare(param);
        define(param);
    }
    resolve(lambda->body);
    endScope();
    currentFunction = enclosingLambda;
}

// Statement methods.

void Resolver::visitBreakStmt(Break* stmt)
{
    (void) stmt;
}

void Resolver::visitBlockStmt(Block* stmt)
{
    beginScope();
    resolve(stmt->statements);
    endScope();
}

void Resolver::visitContinueStmt(Continue* stmt)
{
    (void) stmt;
}

void Resolver::visitExpressionStmt(Expression* stmt)
{
    resolve(stmt->expression);
}

void Resolver::visitFunctionStmt(Function* stmt)
{
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FUNCTION);
}

void Resolver::visitIfStmt(If* stmt)
{
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr)
        resolve(stmt->elseBranch);
}

void Resolver::visitPrintStmt(Print* stmt)
{
    resolve(stmt->expression);
}

void Resolver::visitReturnStmt(Return* stmt)
{
    if (currentFunction == NONE)
        throw ResolveError(stmt->keyword, "Can't return from top-level code.");

    if (stmt->value != nullptr)
    {
        if (currentFunction == INITIALIZER)
            throw ResolveError(stmt->keyword, "Can't return a value from an initializer.");
        resolve(stmt->value);
    }
}

void Resolver::visitVarStmt(Var* stmt)
{
    declare(stmt->name);
    if (stmt->initializer != nullptr)
        resolve(stmt->initializer);
    define(stmt->name);
}

void Resolver::visitWhileStmt(While* stmt)
{
    resolve(stmt->condition);
    resolve(stmt->body);
}

// Expression methods.

Object Resolver::visitAssignExpr(Assign* expr)
{
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return Object(nullptr);
}

Object Resolver::visitBinaryExpr(Binary* expr)
{
    resolve(expr->left);
    resolve(expr->right);
    return Object(nullptr);
}

Object Resolver::visitCallExpr(Call* expr)
{
    resolve(expr->callee);

    for (Expr* argument : expr->arguments)
        resolve(argument);
    
    return Object(nullptr);
}

Object Resolver::visitCommaExpr(Comma* expr)
{
    for (Expr* expr : expr->expressions)
        resolve(expr);
    
    return Object(nullptr);
}

Object Resolver::visitGroupingExpr(Grouping* expr)
{
    resolve(expr->expression);
    return Object(nullptr);
}

Object Resolver::visitLambdaExpr(Lambda* expr)
{
    resolveLambda(expr, LAMBDA);
    return Object(nullptr);
}

Object Resolver::visitLiteralExpr(Literal* expr)
{
    (void) expr;
    return Object(nullptr);
}

Object Resolver::visitLogicalExpr(Logical* expr)
{
    resolve(expr->left);
    resolve(expr->right);
    return Object(nullptr);
}

Object Resolver::visitTernaryExpr(Ternary* expr)
{
    resolve(expr->condition);
    resolve(expr->trueBranch);
    resolve(expr->falseBranch);
    return Object(nullptr);
}

Object Resolver::visitUnaryExpr(Unary* expr)
{
    resolve(expr->right);
    return Object(nullptr);
}

Object Resolver::visitVariableExpr(Variable* expr)
{
    std::string name = expr->name.lexeme;
    int size = (int) scopes.size();
    
    if (!(size == 0) && (scopes[size - 1][name] == false))
        throw ResolveError(expr->name, "Can't read local variable in its own initializer.");

    resolveLocal(expr, expr->name);
    return Object(nullptr);
}