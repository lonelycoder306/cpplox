#pragma once
#include "Nodes.h"
#include "Object.h"
#include "Token.h"
#include "Visitor.h"
#include <vector>

class Expr
{
    public:
        Expr();
        virtual ~Expr();
        virtual Object accept(Visitor& visitor) = 0;
        virtual bool operator==(Expr& other) = 0;
};

class Assign : public Expr
{
    public:
        Token name;
        Expr* value;

        Assign(Token name, Expr* value);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Binary : public Expr
{
    public:
        Expr* left;
        Token bOperator;
        Expr* right;

        Binary(Expr* left, Token bOperator, Expr* right);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Call : public Expr
{
    public:
        Expr* callee;
        Token paren;
        vpE arguments;

        Call(Expr* callee, Token paren, vpE arguments);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Comma : public Expr
{
    public:
        vpE expressions;

        Comma(vpE expressions);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Get : public Expr
{
    public:
        Expr* object;
        Token name;

        Get(Expr* object, Token name);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Grouping : public Expr
{
    public:
        Expr* expression;

        Grouping(Expr* expression);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Lambda : public Expr
{
    public:
        vT params;
        vpS body;

        Lambda(vT params, vpS body);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Literal : public Expr
{
    public:
        Object value;

        Literal(Object value);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Logical : public Expr
{
    public:
        Expr* left;
        Token lOperator;
        Expr* right;

        Logical(Expr* left, Token lOperator, Expr* right);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Set : public Expr
{
    public:
        Expr* object;
        Token name;
        Expr* value;

        Set(Expr* object, Token name, Expr* value);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Super : public Expr
{
    public:
        Token keyword;
        Token method;

        Super(Token keyword, Token method);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Ternary : public Expr
{
    public:
        Expr* condition;
        Expr* trueBranch;
        Expr* falseBranch;

        Ternary(Expr* condition, Expr* trueBranch, Expr* falseBranch);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class This : public Expr
{
    public:
        Token keyword;

        This(Token keyword);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Unary : public Expr
{
    public:
        Token uOperator;
        Expr* right;

        Unary(Token uOperator, Expr* right);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};

class Variable : public Expr
{
    public:
        Token name;

        Variable(Token name);
        Object accept(Visitor& visitor) override;
        bool operator==(Expr& other) override;
};