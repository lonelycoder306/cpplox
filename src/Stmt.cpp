#include "../include/Stmt.h"
#include "../include/Expr.h"
#include "../include/Nodes.h"
#include "../include/Overloads.h"
#include "../include/Token.h"
#include "../include/Visitor.h"
#include <string>

// Constructor.
Stmt::Stmt() = default;
Stmt::~Stmt() = default;

// Break.
Break::Break(Token breakCMD, std::string loopType)
{
    this->breakCMD = breakCMD;
    this->loopType = loopType;
}

void Break::accept(Visitor& visitor)
{
    visitor.visitBreakStmt(this);
}

void Break::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitBreakStmt(reinterpret_cast<Break *&>(stmt));
}

bool Break::operator==(Stmt& other)
{
    auto check = dynamic_cast<Break *>(&other);
    if (!check) return false;
    return (this->breakCMD == check->breakCMD);
}

// Block.
Block::Block(vpS statements)
{
    this->statements = statements;
}

void Block::accept(Visitor& visitor)
{
    visitor.visitBlockStmt(this);
}

void Block::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitBlockStmt(reinterpret_cast<Block *&>(stmt));
}

bool Block::operator==(Stmt& other)
{
    auto check = dynamic_cast<Block *>(&other);
    if (!check) return false;
    return (this->statements == check->statements);
}

// Class.
Class::Class(Token name, Expr* superclass, vpS methods, vpS classMethods)
{
    this->name = name;
    this->superclass = superclass;
    this->methods = methods;
    this->classMethods = classMethods;
}

void Class::accept(Visitor& visitor)
{
    visitor.visitClassStmt(this);
}

void Class::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitClassStmt(reinterpret_cast<Class *&>(stmt));
}

bool Class::operator==(Stmt& other)
{
    auto check = dynamic_cast<Class *>(&other);
    if (!check) return false;
    return ((this->name == check->name) &&
            (*(this->superclass) == *(check->superclass)) &&
            (this->methods == check->methods) &&
            (this->classMethods) == check->classMethods);
}

// Continue.
Continue::Continue(Token continueCMD, std::string loopType)
{
    this->continueCMD = continueCMD;
    this->loopType = loopType;
}

void Continue::accept(Visitor& visitor)
{
    visitor.visitContinueStmt(this);
}

void Continue::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitContinueStmt(reinterpret_cast<Continue *&>(stmt));
}

bool Continue::operator==(Stmt& other)
{
    auto check = dynamic_cast<Continue *>(&other);
    if (!check) return false;
    return (this->continueCMD == check->continueCMD);
}

// Function.
Function::Function(Token name, vT* params, vpS body)
{
    this->name = name;
    this->params = params;
    this->body = body;
}

void Function::accept(Visitor& visitor)
{
    visitor.visitFunctionStmt(this);
}

void Function::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitFunctionStmt(reinterpret_cast<Function *&>(stmt));
}

bool Function::operator==(Stmt& other)
{
    auto check = dynamic_cast<Function *>(&other);
    if (!check) return false;
    if (this->params == nullptr)
    {
        if (check->params != nullptr) return false;
        return ((this->name == check->name) &&
                (this->body == check->body));
    }
    if (check->params == nullptr) return false;
    return ((this->name == check->name) &&
            (*(this->params) == *(check->params)) &&
            (this->body == check->body));
}

// If.
If::If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch)
{
    this->condition = condition;
    this->thenBranch = thenBranch;
    this->elseBranch = elseBranch;
}

void If::accept(Visitor& visitor)
{
    visitor.visitIfStmt(this);
}

void If::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitIfStmt(reinterpret_cast<If *&>(stmt));
}

bool If::operator==(Stmt& other)
{
    auto check = dynamic_cast<If *>(&other);
    if (!check) return false;
    if (this->elseBranch == nullptr)
    {
        if (check->elseBranch != nullptr) return false;
        return ((*(this->condition) == *(check->condition)) &&
                (*(this->thenBranch) == *(check->thenBranch)));
    }
    if (check->elseBranch == nullptr) return false;
    return ((*(this->condition) == *(check->condition)) &&
            (*(this->thenBranch) == *(check->thenBranch)) &&
            (*(this->elseBranch) == *(check->elseBranch)));
}

// Expression.
Expression::Expression(Expr* expression)
{
    this->expression = expression;
}

void Expression::accept(Visitor& visitor)
{
    visitor.visitExpressionStmt(this);
}

void Expression::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitExpressionStmt(reinterpret_cast<Expression *&>(stmt));
}

bool Expression::operator==(Stmt& other)
{
    auto check = dynamic_cast<Expression *>(&other);
    if (!check) return false;
    return (*(this->expression) == *(check->expression));
}

// Print.
Print::Print(Expr* expression)
{
    this->expression = expression;
}

void Print::accept(Visitor& visitor)
{
    visitor.visitPrintStmt(this);
}

void Print::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitPrintStmt(reinterpret_cast<Print *&>(stmt));
}

bool Print::operator==(Stmt& other)
{
    auto check = dynamic_cast<Print *>(&other);
    if (!check) return false;
    return (*(this->expression) == *(check->expression));
}

// Return.
Return::Return(Token keyword, Expr* value)
{
    this->keyword = keyword;
    this->value = value;
}

void Return::accept(Visitor& visitor)
{
    visitor.visitReturnStmt(this);
}

void Return::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitReturnStmt(reinterpret_cast<Return *&>(stmt));
}

bool Return::operator==(Stmt& other)
{
    auto check = dynamic_cast<Return *>(&other);
    if (!check) return false;
    return ((this->keyword == check->keyword) &&
            (*(this->value) == *(check->value)));
}

// Var.
Var::Var(Token name, Expr* initializer, bool access)
{
    this->name = name;
    this->initializer = initializer;
    this->access = access;
}

void Var::accept(Visitor& visitor)
{
    visitor.visitVarStmt(this);
}

void Var::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitVarStmt(reinterpret_cast<Var *&>(stmt));
}

bool Var::operator==(Stmt& other)
{
    auto check = dynamic_cast<Var *>(&other);
    if (!check) return false;
    return ((this->name == check->name) &&
            (*(this->initializer) == *(check->initializer)) &&
            (this->access == check->access));
}

// While.
While::While(Expr* condition, Stmt* body)
{
    this->condition = condition;
    this->body = body;
}

void While::accept(Visitor& visitor)
{
    visitor.visitWhileStmt(this);
}

void While::remove(Cleaner& cleaner, Stmt* &stmt)
{
    cleaner.visitWhileStmt(reinterpret_cast<While *&>(stmt));
}

bool While::operator==(Stmt& other)
{
    auto check = dynamic_cast<While *>(&other);
    if (!check) return false;
    return ((*(this->condition) == *(check->condition)) &&
            (*(this->body) == *(check->body)));
}