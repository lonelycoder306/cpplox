#pragma once
#include "Nodes.h"
#include "Token.h"
#include "Visitor.h"
#include <string>
#include <vector>

class Stmt
{
    public:
        Stmt();
        virtual ~Stmt() = 0;
        virtual void accept(Visitor& visitor) = 0;
        virtual bool operator==(Stmt& other) = 0;
};

class Declaration {};

class Break : public Stmt
{
    public:
        Token breakCMD;
        std::string loopType;

        Break(Token breakCMD, std::string loopType);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

class Block : public Stmt
{
    public:
        vpS statements;

        Block(vpS statements);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

/*
class Class : public Stmt
{
    public:
        Token name;
        Expr* superclass;
        vpS methods;
        vpS classMethods;

        Class(Token name, Expr* superclass, vpS methods, vpS classMethods);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};
*/

class Continue : public Stmt
{
    public:
        Token continueCMD;
        std::string loopType;

        Continue(Token continueCMD, std::string loopType);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

class Function : public Stmt, public Declaration
{
    public:
        Token name;
        vT* params;
        vpS body;

        Function(Token name, vT* params, vpS body);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

class If : public Stmt
{
    public:
        Expr* condition;
        Stmt* thenBranch;
        Stmt* elseBranch;

        If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

class Expression : public Stmt
{
    public:
        Expr* expression;

        Expression(Expr* expression);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

class Print : public Stmt
{
    public:
        Expr* expression;

        Print(Expr* expression);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

class Return : public Stmt
{
    public:
        Token keyword;
        Expr* value;

        Return(Token keyword, Expr* value);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

class Var : public Stmt, public Declaration
{
    public:
        Token name;
        Expr* initializer;

        Var(Token name, Expr* initializer);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};

class While : public Stmt
{
    public:
        Expr* condition;
        Stmt* body;

        While(Expr* condition, Stmt* body);
        void accept(Visitor& visitor) override;
        bool operator==(Stmt& other) override;
};