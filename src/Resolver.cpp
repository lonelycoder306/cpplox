#include "../include/Resolver.h"
#include "../include/Error.h"
#include "../include/Expr.h"
#include "../include/Interpreter.h"
#include "../include/Nodes.h"
#include "../include/Object.h"
#include "../include/Stmt.h"
#include "../include/Token.h"
#include <string>

#define FALSE -1
#define TRUE 1

// Constructor.

Resolver::Resolver(Interpreter& interpreter)
{
    this->interpreter = &interpreter;
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

    std::map<std::string, int> scope = scopes[scopes.size() - 1];
    if (scope.contains(name.lexeme))
        throw StaticError(name, "Already a variable with this name in this scope.");
    scopes[scopes.size() - 1][name.lexeme] = FALSE;
}

void Resolver::define(Token name)
{
    if (scopes.size() == 0) return;
    scopes[scopes.size() - 1][name.lexeme] = TRUE;
}

void Resolver::resolve(vpS statements)
{
    try
    {
        for (Stmt* stmt : statements)
            resolve(stmt);
    }
    catch (StaticError& error)
    {
        error.show();
    }
}

void Resolver::resolve(Stmt* stmt)
{
    stmt->accept(*this);
}

void Resolver::resolve(Expr* expr)
{
    (void) expr->accept(*this); // Unused return value.
}

void Resolver::resolveLocal(Expr* expr, Token name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        if (scopes[i].contains(name.lexeme))
        {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
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

void Resolver::visitClassStmt(Class* stmt)
{
    ClassType enclosingClass = currentClass;
    currentClass = CLASS;

    declare(stmt->name);
    define(stmt->name);

    auto super = dynamic_cast<Variable *>(stmt->superclass);
    if ((stmt->superclass != nullptr) &&
        (stmt->name.lexeme == super->name.lexeme))
            throw StaticError(super->name, "A class can't inherit from itself.");
    
    if (stmt->superclass != nullptr)
    {
        currentClass = SUBCLASS;
        resolve(stmt->superclass);
    }

    if (stmt->superclass != nullptr)
    {
        beginScope();
        scopes[scopes.size() - 1]["super"] = TRUE;
    }

    beginScope();
    scopes[scopes.size() - 1]["this"] = TRUE;

    for (Stmt* method : stmt->methods)
    {
        FunctionType declaration = METHOD;
        auto func = dynamic_cast<Function *>(method);
        if (func->name == "init")
            declaration = INITIALIZER;
        
        resolveFunction(func, declaration);
    }

    endScope();

    if (stmt->superclass != nullptr) endScope();

    currentClass = enclosingClass;
}

void Resolver::visitContinueStmt(Continue* stmt)
{
    (void) stmt;
}

void Resolver::visitExpressionStmt(Expression* stmt)
{
    resolve(stmt->expression);
}

void Resolver::visitFetchStmt(Fetch* stmt)
{
    (void) stmt;
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
    if (currentFunction == NOFUNC)
        throw StaticError(stmt->keyword, "Can't return from top-level code.");

    if (stmt->value != nullptr)
    {
        if (currentFunction == INITIALIZER)
            throw StaticError(stmt->keyword, "Can't return a value from an initializer.");
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

Object Resolver::visitGetExpr(Get* expr)
{
    resolve(expr->object);
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

Object Resolver::visitListExpr(List* expr)
{
    for (Expr* element : expr->elements)
        resolve(element);
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

Object Resolver::visitSetExpr(Set* expr)
{
    resolve(expr->value);
    resolve(expr->object);
    return Object(nullptr);
}

Object Resolver::visitSuperExpr(Super* expr)
{
    if (currentClass == NOCLASS)
        throw StaticError(expr->keyword, "Can't use 'super' outside of a class.");
    else if (currentClass == CLASS)
        throw StaticError(expr->keyword, "Can't use 'super' outside of a subclass.");
    resolveLocal(expr, expr->keyword);
    return Object(nullptr);
}

Object Resolver::visitTernaryExpr(Ternary* expr)
{
    resolve(expr->condition);
    resolve(expr->trueBranch);
    resolve(expr->falseBranch);
    return Object(nullptr);
}

Object Resolver::visitThisExpr(This* expr)
{
    if (currentClass == NOCLASS)
        throw StaticError(expr->keyword, "Can't use 'this' outside of a class.");

    resolveLocal(expr, expr->keyword);
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
    
    if (!(scopes.size() == 0) && (scopes[scopes.size() - 1][name] == FALSE))
        throw StaticError(expr->name, "Can't read local variable in its own initializer.");

    resolveLocal(expr, expr->name);
    return Object(nullptr);
}