#include "../include/Stmt.h"
#include "../include/Expr.h"
#include "../include/Nodes.h"
#include "../include/Token.h"

// Constructor.
Stmt::Stmt() = default;
Stmt::~Stmt() = default;

// Break.
Break::Break(Token breakCMD)
{
    this->breakCMD = breakCMD;
}

// Block.
Block::Block(vpS statements)
{
    this->statements = statements;
}

// Class.
Class::Class(Token name, Expr* superclass, vpS methods, vpS classMethods)
{
    this->name = name;
    this->superclass = superclass;
    this->methods = methods;
    this->classMethods = classMethods;
}

// Continue.
Continue::Continue(Token continueCMD)
{
    this->continueCMD = continueCMD;
}

// Function.
Function::Function(Token name, vT params, vpS body)
{
    this->name = name;
    this->params = params;
    this->body = body;
}

// If.
If::If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch)
{
    this->condition = condition;
    this->thenBranch = thenBranch;
    this->elseBranch = elseBranch;
}

// Expression.
Expression::Expression(Expr* expression)
{
    this->expression = expression;
}

// Print.
Print::Print(Expr* expression)
{
    this->expression = expression;
}

// Return.
Return::Return(Token keyword, Expr* value)
{
    this->keyword = keyword;
    this->value = value;
}

// Var.
Var::Var(Token name, Expr* initializer)
{
    this->name = name;
    this->initializer = initializer;
}

// While.
While::While(Expr* condition, Stmt* body)
{
    this->condition = condition;
    this->body = body;
}