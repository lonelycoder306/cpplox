#pragma once
#include "Nodes.h"
#include "Token.h"
#include <vector>

class Stmt
{
    public:
        Stmt();
        virtual ~Stmt();
};

class Break : public Stmt
{
    public:
        Token breakCMD;

        Break(Token breakCMD);
};

class Block : public Stmt
{
    public:
        vpS statements;

        Block(vpS statements);
};

class Class : public Stmt
{
    public:
        Token name;
        Expr* superclass;
        vpS methods;
        vpS classMethods;

        Class(Token name, Expr* superclass, vpS methods, vpS classMethods);
};

class Continue : public Stmt
{
    public:
        Token continueCMD;

        Continue(Token continueCMD);
};

class Function : public Stmt
{
    public:
        Token name;
        vT params;
        vpS body;

        Function(Token name, vT params, vpS body);
};

class If : public Stmt
{
    public:
        Expr* condition;
        Stmt* thenBranch;
        Stmt* elseBranch;

        If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch);
};

class Expression : public Stmt
{
    public:
        Expr* expression;

        Expression(Expr* expression);
};

class Print : public Stmt
{
    public:
        Expr* expression;

        Print(Expr* expression);
};

class Return : public Stmt
{
    public:
        Token keyword;
        Expr* value;

        Return(Token keyword, Expr* value);
};

class Var : public Stmt
{
    public:
        Token name;
        Expr* initializer;

        Var(Token name, Expr* initializer);
};

class While : public Stmt
{
    public:
        Expr* condition;
        Stmt* body;

        While(Expr* condition, Stmt* body);
};