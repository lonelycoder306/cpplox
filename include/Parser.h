#pragma once
#include "Nodes.h"
#include "Token.h"
#include "TokenType.h"
#include <string>
#include <vector>

class Parser
{
    public:
        Parser(vT tokens);
        vpS parse();
    
    private:
        vT tokens;
        int current = 0;
        std::string loopType;

        // Statement methods.

        Stmt* declaration();
        Stmt* statement();
        Stmt* breakStatement();
        Stmt* classDeclaration();
        Stmt* continueStatement();
        Stmt* fetchStatement();
        Stmt* forStatement();
        Stmt* ifStatement();
        Stmt* printStatement();
        Stmt* returnStatement();
        Stmt* varDeclaration(bool access);
        Stmt* whileStatement();
        Stmt* expressionStatement();
        Stmt* function(std::string kind);
        vpS block();

        // Expression methods.

        Expr* expression();
        Expr* comma();
        Expr* lambda();
        Expr* assignment();
        Expr* orExpr();
        Expr* andExpr();
        Expr* ternary();
        Expr* equality();
        Expr* comparison();
        Expr* term();
        Expr* factor();
        Expr* unary();
        Expr* exponent();
        Expr* finishCall(Expr* callee);
        Expr* call();
        Expr* list();
        Expr* primary();

        // Helper methods.

        template<typename... Type>
        bool match(Type... types);
        Token consume(TokenType type, std::string message);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        void synchronize();
};