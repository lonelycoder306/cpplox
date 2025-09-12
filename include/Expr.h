#pragma once
#include "Nodes.h"
#include "Object.h"
#include "Token.h"
#include <vector>

class Expr
{
    public:
        Expr();
        virtual ~Expr();
};

class Assign : public Expr
{
    public:
        Token name;
        Expr* value;

        Assign(Token name, Expr* value);
};

class Binary : public Expr
{
    public:
        Expr* left;
        Token bOperator;
        Expr* right;

        Binary(Expr* left, Token bOperator, Expr* right);
};

class Call : public Expr
{
    public:
        Expr* callee;
        Token paren;
        vpE arguments;

        Call(Expr* callee, Token paren, vpE arguments);
};

class Comma : public Expr
{
    public:
        vpE expressions;

        Comma(vpE expressions);
};

class Get : public Expr
{
    public:
        Expr* object;
        Token name;

        Get(Expr* object, Token name);
};

class Grouping : public Expr
{
    public:
        Expr* expression;

        Grouping(Expr* expression);
};

class Lambda : public Expr
{
    public:
        vT params;
        vpS body;

        Lambda(vT params, vpS body);
};

class Literal : public Expr
{
    public:
        Object value;

        Literal(Object value);
};

class Logical : public Expr
{
    public:
        Expr* left;
        Token lOperator;
        Expr* right;

        Logical(Expr* left, Token lOperator, Expr* right);
};

class Set : public Expr
{
    public:
        Expr* object;
        Token name;
        Expr* value;

        Set(Expr* object, Token name, Expr* value);
};

class Super : public Expr
{
    public:
        Token keyword;
        Token method;

        Super(Token keyword, Token method);
};

class Ternary : public Expr
{
    public:
        Expr* condition;
        Expr* trueBranch;
        Expr* falseBranch;

        Ternary(Expr* condition, Expr* trueBranch, Expr* falseBranch);
};

class This : public Expr
{
    public:
        Token keyword;

        This(Token keyword);
};

class Unary : public Expr
{
    public:
        Token uOperator;
        Expr* right;

        Unary(Token uOperator, Expr* right);
};

class Variable : public Expr
{
    public:
        Token name;

        Variable(Token name);
};