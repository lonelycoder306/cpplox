#include "../include/Parser.h"
#include "../include/Error.h"
#include "../include/Expr.h"
#include "../include/Lox.h"
#include "../include/Nodes.h"
#include "../include/Scanner.h"
#include "../include/Stmt.h"
#include "../include/Token.h"
#include "../include/TokenType.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define VAR_DEC true
#define FIX_DEC false

// Public methods.
Parser::Parser(vT tokens)
{
    this->tokens = tokens;
}

vpS Parser::parse()
{
    vpS statements;
    while (!isAtEnd())
        statements.push_back(declaration());
    
    return statements;
}

// Private methods.

// Statement methods.
Stmt* Parser::declaration()
{
    try
    {
        if (match(CLASS)) return classDeclaration();
        if (match(FUN))
        {
            if (check(IDENTIFIER)) return function("function");
            // Signal that the function is an unassigned lambda (so interpreter does nothing with it).
            return function("");
        }
        if (match(VAR)) return varDeclaration(VAR_DEC);
        if (match(FIX)) return varDeclaration(FIX_DEC);

        return statement();
    }
    catch (ParseError& error)
    {
        synchronize();
        error.show();
        return nullptr;
    }
}

Stmt* Parser::statement()
{
    if (match(BREAK)) return breakStatement();
    if (match(CONTINUE)) return continueStatement();
    if (match(FOR)) return forStatement();
    if (match(GET)) return fetchStatement();
    if (match(IF)) return ifStatement();
    if (match(PRINT)) return printStatement();
    if (match(RETURN)) return returnStatement();
    if (match(WHILE)) return whileStatement();
    if (match(LEFT_BRACE))
    {
        Stmt* stmtBlock = new Block(block());
        return stmtBlock;
    }

    return expressionStatement();
}

Stmt* Parser::breakStatement()
{
    Token breakToken = previous();
    consume(SEMICOLON, "Expect ';' after 'break'.");
    return new Break(breakToken, loopType);
}

Stmt* Parser::classDeclaration()
{
    Token name = consume(IDENTIFIER, "Expect class name.");

    Expr* superclass = nullptr;
    if (match(LESS))
    {
        consume(IDENTIFIER, "Expect superclass name.");
        superclass = new Variable(previous());
    }

    consume(LEFT_BRACE, "Expect '{' before class body.");

    vpS methods;
    vpS classMethods;
    while (!check(RIGHT_BRACE) && !isAtEnd())
        (match(CLASS) ? classMethods : methods).push_back(function("method"));

    consume(RIGHT_BRACE, "Expect '}' after class body.");

    return new Class(name, superclass, methods, classMethods);
}

Stmt* Parser::continueStatement()
{
    Token continueToken = previous();
    consume(SEMICOLON, "Expect ';' after 'continue'.");
    return new Continue(continueToken, loopType);
}

Stmt* Parser::fetchStatement()
{
    Token modeToken = previous();
    std::string mode = modeToken.lexeme;
    mode = mode.substr(3, mode.length() - 3);
    Token nameToken = consume(STRING, "Expect name of import.");
    std::string name = nameToken.lexeme;
    // Cut off the "".
    name = name.substr(1, name.length() - 2);
    consume(SEMICOLON, "Expect ';' after import statement.");

    if (mode == "Lib")
    {
        name = "Libraries\\" + name + ".lox";
        std::ifstream libIn(name);
        if (libIn.fail())
            throw ParseError(nameToken, "No such library file.");
        std::string libFile((std::istreambuf_iterator<char>(libIn)), 
                                    std::istreambuf_iterator<char>());
        Scanner tempScanner(libFile, name);
        vT newTokens = tempScanner.scanTokens();
        this->tokens.insert(this->tokens.begin() + current, 
                                newTokens.begin(), newTokens.end() - 1);
    }
    else if (mode == "File")
    {
        std::ifstream fileIn(name);
        if (fileIn.fail())
            throw ParseError(nameToken, "No such Lox file.");
        std::string loxFile((std::istreambuf_iterator<char>(fileIn)), 
                                    std::istreambuf_iterator<char>());
        Scanner tempScanner(loxFile, name);
        vT newTokens = tempScanner.scanTokens();
        this->tokens.insert(this->tokens.begin() + current, 
                                newTokens.begin(), newTokens.end() - 1);
    }

    return new Fetch(mode, name);
}

Stmt* Parser::forStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    Stmt* initializer;
    if (match(SEMICOLON))
        initializer = nullptr;
    else if (match(VAR))
        initializer = varDeclaration(VAR_DEC);
    else
        initializer = expressionStatement();

    Expr* condition = nullptr;
    if (!check(SEMICOLON)) condition = expression();
    consume(SEMICOLON, "Expect ';' after loop condition.");

    Expr* increment = nullptr;
    if (!check(RIGHT_PAREN)) increment = expression();
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");

    std::string currentLoop = this->loopType;
    this->loopType = "forLoop";
    Stmt* body = statement();
    this->loopType = currentLoop;

    if (increment != nullptr)
    {
        Stmt* tempStmt = new Expression(increment);
        body = new Block({body, tempStmt});
    }

    if (condition == nullptr) 
        condition = new Literal(Object(true));
    body = new While(condition, body);

    if (initializer != nullptr)
        body = new Block({initializer, body});

    return body;
}

Stmt* Parser::ifStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    Stmt* thenBranch = statement();
    Stmt* elseBranch = nullptr;
    if (match(ELSE)) elseBranch = statement();

    return new If(condition, thenBranch, elseBranch);
}

Stmt* Parser::printStatement()
{
    Expr* value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new Print(value);
}

Stmt* Parser::returnStatement()
{
    Token keyword = previous();

    Expr* value = nullptr;
    if (!check(SEMICOLON))
        value = expression();

    consume(SEMICOLON, "Expect ';' after return value.");

    // Report a warning if any code follows a return statement in the same scope.
    if (!check(RIGHT_BRACE))
    {
        std::cout << "Warning (line " << peek().line <<
                "): code found after return statement (will be ignored).\n";
    }

    return new Return(keyword, value);
}

Stmt* Parser::varDeclaration(bool access)
{
    Token name = consume(IDENTIFIER, "Expect variable name.");

    Expr* initializer = nullptr;
    if (match(EQUAL))
        initializer = expression();
    else if (access == FIX_DEC)
        throw ParseError(peek(), "Must provide initializer to fixed variable.");

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return new Var(name, initializer, access);
}

Stmt* Parser::whileStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    
    std::string currentLoop = this->loopType;
    this->loopType = "whileLoop";
    Stmt* body = statement();
    this->loopType = currentLoop;

    return new While(condition, body);
}

Stmt* Parser::expressionStatement()
{
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new Expression(expr);
}

Stmt* Parser::function(std::string kind)
{
    Token name;
    // Check that function is not an unassigned lambda.
    if (kind != "")
        name = consume(IDENTIFIER, "Expect " + kind + " name.");

    vT* parameters = nullptr; // Add std::optional to make null list for getter methods.

    // Allow omitting the parameter list entirely in method getters.
    if (!(kind == "method") || check(LEFT_PAREN))
    {
        consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
        parameters = new vT;
        if (!check(RIGHT_PAREN))
        {
            do
            {
                if (parameters->size() >= 255)
                    throw ParseError(peek(), "Can't have more than 255 parameters.");

                parameters->push_back(consume(IDENTIFIER, "Expect parameter name."));
            } while (match(COMMA));
        }

        consume(RIGHT_PAREN, "Expect ')' after parameters.");
    }

    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    vpS body = block();
    return new Function(name, parameters, body);
}

vpS Parser::block()
{
    vpS statements;

    while (!check(RIGHT_BRACE) && !isAtEnd())
        statements.push_back(declaration());

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

// Expression methods.
Expr* Parser::expression()
{
    return comma();
}

Expr* Parser::comma()
{
    vpE expressions;

    Expr* expr = lambda();
    expressions.push_back(expr);

    while (match(COMMA))
    {
        expr = lambda();
        expressions.push_back(expr);
    }

    if (expressions.size() > 1)
        return new Comma(expressions);

    return expr;
}

Expr* Parser::lambda()
{
    if (match(FUN))
    {
        consume(LEFT_PAREN, "Expect '(' after 'fun' keyword.");
        vT parameters;
        if (!check(RIGHT_PAREN))
        {
            do
            {
                if (parameters.size() >= 255)
                    throw ParseError(peek(), "Can't have more than 255 parameters.");

                parameters.push_back(consume(IDENTIFIER,  "Expect parameter name."));
            } while (match(COMMA));
        }

        consume(RIGHT_PAREN, "Expect ')' after parameters.");

        consume(LEFT_BRACE, "Expect '{' before lambda body.");
        vpS body = block();

        return new Lambda(parameters, body);
    }

    return assignment();
}

Expr* Parser::assignment()
{
    Expr* expr = orExpr();

    if (match(EQUAL))
    {
        Token equals = previous();
        Expr* value = assignment();

        if (dynamic_cast<Variable*>(expr))
        {
            Token name = ((Variable *)expr)->name;
            return new Assign(name, value);
        }

        else if (dynamic_cast<Get*>(expr))
        {
            Get* get = (Get*) expr;
            return new Set(get->object, get->name, value);
        }

        throw ParseError(equals, "Invalid assignment target.");
    }

    return expr;
}

Expr* Parser::orExpr()
{
    Expr* expr = andExpr();

    while (match(OR))
    {
        Token lOperator = previous();
        Expr* right = andExpr();
        expr = new Logical(expr, lOperator, right);
    }

    return expr;
}

Expr* Parser::andExpr()
{
    Expr* expr = ternary();

    while (match(AND))
    {
        Token lOperator = previous();
        Expr* right = equality();
        expr = new Logical(expr, lOperator, right);
    }

    return expr;
}

Expr* Parser::ternary()
{
    Expr* expr = equality();

    while (match(Q_MARK))
    {
        Expr* left = equality();
        consume(COLON, "Expect colon separator between ternary operator branches.");
        Expr* right = ternary();
        expr = new Ternary(expr, left, right);
    }

    return expr;
}

Expr* Parser::equality()
{
    Expr* expr = comparison();

    while(match(BANG_EQUAL, EQUAL_EQUAL))
    {
        Token bOperator = previous();
        Expr* right = comparison();
        expr = new Binary(expr, bOperator, right);
    }

    return expr;
}

Expr* Parser::comparison()
{
    Expr* expr = term();

    while(match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
    {
        Token bOperator = previous();
        Expr* right = term();
        expr = new Binary(expr, bOperator, right);
    }

    return expr;
}

Expr* Parser::term()
{
    Expr* expr = factor();

    while(match(MINUS, PLUS))
    {
        Token bOperator = previous();
        Expr* right = factor();
        expr = new Binary(expr, bOperator, right);
    }

    return expr;
}

Expr* Parser::factor()
{
    Expr* expr = unary();

    while(match(SLASH, STAR, MOD))
    {
        Token bOperator = previous();
        Expr* right = unary();
        expr = new Binary(expr, bOperator, right);
    }

    return expr;
}

Expr* Parser::unary()
{
    if(match(BANG, MINUS))
    {
        Token uOperator = previous();
        Expr* right = unary();
        return new Unary(uOperator, right);
    }

    return exponent();
}

Expr* Parser::exponent()
{
    Expr* expr = call();

    while (match(POWER))
    {
        Token bOperator = previous();
        Expr* right = exponent();
        expr = new Binary(expr, bOperator, right);
    }

    return expr;
}

Expr* Parser::finishCall(Expr* callee)
{
    vpE arguments;
    Token paren = previous();

    if (!check(RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() > 255)
                throw ParseError(peek(), "Can't have more than 255 arguments.");
            // Personal edit/change.
            // lambda() instead of expression() to ignore comma expressions in function arguments.
            arguments.push_back(lambda());
        } while (match(COMMA));
    }

    consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return new Call(callee, paren, arguments);
}

Expr* Parser::call()
{
    Expr* expr = primary();

    while (true)
    {
        if (match(LEFT_PAREN))
            expr = finishCall(expr);
        else if (match(DOT))
        {
            Token name = consume(IDENTIFIER, "Expect property name after '.'.");
            expr = new Get(expr, name);
        }
        else
            break;
    }

    return expr;
}

Expr* Parser::primary()
{
    if(match(FALSE)) return new Literal(Object(false));
    if(match(TRUE)) return new Literal(Object(true));
    if(match(NIL)) return new Literal(Object(nullptr)); // Temporary.

    if(match(NUMBER, STRING))
        return new Literal(previous().literal);

    if (match(SUPER))
    {
        Token keyword = previous();
        consume(DOT, "Expect '.' after 'super'.");
        Token method = consume(IDENTIFIER, "Expect superclass method name.");
        return new Super(keyword, method);
    }

    if (match(THIS)) return new This(previous());

    if(match(IDENTIFIER))
        return new Variable(previous());

    if(match(LEFT_PAREN))
    {
        Expr* expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping(expr);
    }

    throw ParseError(peek(), "Expect expression.");
}

// Helper methods.
template<typename... Type>
bool Parser::match(Type... types)
{
	for (TokenType type : {types ...})
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}

	return false;
}

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type)) return advance();

    throw ParseError(peek(), message);
}

bool Parser::check(TokenType type)
{
    if (isAtEnd()) return false;
    return (peek().type == type);
}

Token Parser::advance()
{
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd()
{
    return (peek().type == eof);
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::previous()
{
    return tokens[current - 1];
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd())
    {
        if (previous().type == SEMICOLON) return;

        switch (peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }

        advance();
    }
}