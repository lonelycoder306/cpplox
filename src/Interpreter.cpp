#include "../include/Interpreter.h"
#include "../include/Error.h"
#include "../include/Expr.h"
#include "../include/Lox.h"
#include "../include/LoxFunction.h"
#include "../include/Nodes.h"
#include "../include/Object.h"
#include "../include/Overloads.h"
#include "../include/Stmt.h"
#include <any>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

// General methods.

void Interpreter::interpret(vpS statements)
{
    try
    {
        for (Stmt* statement: statements)
            execute(statement);
    }
    catch (RuntimeError& error)
    {
        Lox::runtimeError(error);
    }
}

void Interpreter::execute(Stmt* stmt)
{
    stmt->accept(*this);
}

Object Interpreter::evaluate(Expr* expr)
{
    return expr->accept(*this);
}

void Interpreter::executeBlock(vpS statements, Environment environment)
{
    Environment previous = this->environment;
    try {
        this->environment = environment;

        for (Stmt* statement: statements)
            execute(statement);
    }
    catch (RuntimeError& error)
    {
        // ...
    }
    this->environment = previous;
}

// Statement methods.

void Interpreter::visitBreakStmt(Break* stmt)
{
    if (loopLevel == 0) throw BreakError(stmt->breakCMD, stmt->loopType);

    // Will only be thrown if break statement is reached.
    // Statement will not be reached after a false if-condition (for example).
    throw RuntimeError(stmt->breakCMD,
            "Cannot have 'break' outside loop.");
}

void Interpreter::visitBlockStmt(Block* stmt)
{
    executeBlock(stmt->statements, Environment(environment));
}

void Interpreter::visitContinueStmt(Continue* stmt)
{
    if (loopLevel == 0) throw ContinueError(stmt->continueCMD, stmt->loopType);

    // Will only be thrown if continue statement is reached.
    // Statement will not be reached after a false if-condition (for example).
    throw RuntimeError(stmt->continueCMD,
            "Cannot have 'continue' outside loop.");
}

/*
void Interpreter::visitClassStmt(Class* stmt)
{
    // ...
}
*/

void Interpreter::visitExpressionStmt(Expression* stmt)
{
    // Print out the return value of any expression statement (except assignments and function calls).
    if (!(dynamic_cast<Assign *>(stmt->expression)) &&
        !(dynamic_cast<Call *>(stmt->expression)) /*&&
        !(dynamic_cast<Set *>(stmt->expression))*/)
    {
        visitPrintStmt(new Print(stmt->expression));
    }
    else
        evaluate(stmt->expression); // Only evaluate for assignment statements.
}

void Interpreter::visitFunctionStmt(Function* stmt)
{
    // Check that function is not an unassigned lambda (do nothing if it is).
    // Line #0 signals uninitialized (i.e., empty) Token.
    if (stmt->name.line != 0)
    {
        LoxFunction function(*stmt, environment, false);
        environment.define(stmt->name.lexeme, Object(function));
    }
}

void Interpreter::visitIfStmt(If* stmt)
{
    if (isTruthy(evaluate(stmt->condition)))
        execute(stmt->thenBranch);
    else if (stmt->elseBranch != nullptr)
        execute(stmt->elseBranch);
}

void Interpreter::visitPrintStmt(Print* stmt)
{
    Object value = evaluate(stmt->expression);
    std::cout << stringify(value);
}

void Interpreter::visitReturnStmt(Return* stmt)
{
    Object value(nullptr);

    if (stmt->value != nullptr) value = evaluate(stmt->value);

    throw ReturnValue(value);
}

void Interpreter::visitVarStmt(Var* stmt)
{
    // Initialize to an array if no initializer provided.
    // Easy to check for later to determine if the variable is uninitialized in the user's code.
    Object value(std::vector<int>(0));
    if (stmt->initializer != nullptr)
        value = evaluate(stmt->initializer);

    environment.define(stmt->name.lexeme, value);
}

void Interpreter::visitWhileStmt(While* stmt)
{
    loopLevel++;
    while(isTruthy(evaluate(stmt->condition)))
    {
        try {
            execute(stmt->body);
        }
        catch (BreakError& error) {
            break;
        }
        catch (ContinueError& error) {
            // ...
        }
    }
    loopLevel--;
}

// Expression methods.

Object Interpreter::visitAssignExpr(Assign* expr)
{
    Object value = evaluate(expr->value);

    if (locals.contains(expr))
    {
        int distance = locals[expr];
        environment.assignAt(distance, expr->name, value);
    }
    else
        globals.assign(expr->name, value);

    return value;
}

Object Interpreter::visitBinaryExpr(Binary* expr)
{
    #define double(value) std::any_cast<double>(value)
    
    Object left = evaluate(expr->left);
    Object right = evaluate(expr->right);

    switch (expr->bOperator.type)
    {
        case GREATER:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left.value) > double(right.value));
        case GREATER_EQUAL:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left.value) >= double(right.value));
        case LESS:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left.value) < double(right.value));
        case LESS_EQUAL:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left.value) <= double(right.value));
        case BANG_EQUAL: return Object(!isEqual(left, right));
        case EQUAL_EQUAL: return Object(isEqual(left, right));
        case MINUS:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left.value) - double(right.value));
        case PLUS:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left.value) - double(right.value));
        case SLASH:
            checkNumberOperands(expr->bOperator, left, right);
            if (double(right) == 0)
                throw RuntimeError(expr->bOperator, "Division by zero not allowed.");
            return Object(double(left) / double(right));
        case STAR:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left.value) * double(right.value));
        default:
            return Object(nullptr);
    }
}

Object Interpreter::visitCallExpr(Call* expr)
{
    Object callee = evaluate(expr->callee);

    std::vector<Object> arguments;
    for (Expr* argument: expr->arguments)
        arguments.push_back(evaluate(argument));

    // Change to match any child-class of LoxCallable.
    if (!(callee.value.type() == typeid(LoxFunction)))
        throw new RuntimeError(expr->paren, "Can only call functions and classes.");

    // LoxFunction instead of LoxCallable (temporarily).
    LoxFunction function = std::any_cast<LoxFunction>(callee.value);
    if ((int) arguments.size() != function.arity())
        throw new RuntimeError(expr->paren, "Expected " +
                std::to_string(function.arity()) + " arguments but got " +
                std::to_string(arguments.size()) + ".");

    return function.call(*this, arguments);
}

Object Interpreter::visitCommaExpr(Comma* expr)
{
    vpE expressions = expr->expressions;
    int expressionNumber = expressions.size();
    for (int i = 0; i < expressionNumber - 1; i++)
        evaluate(expressions[i]);
    return evaluate(expressions[expressionNumber - 1]);
}

/*
Object Interpreter::visitGetExpr(Get* expr)
{
    Object object = evaluate(expr->object);
    if (object.value.type() == typeid(LoxInstance)) {
        Object result = std::any_cast<LoxInstance>(object.value).get(expr->name);
        if ((result.value.type() == typeid(LoxFunction)) && 
            (std::any_cast<LoxFunction>(result)).isGetter())
            result = std::any_cast<LoxFunction>(result).call(*this, {});

        return result;
    }

    throw RuntimeError(expr->name, "Only instances have properties.");
}
*/

Object Interpreter::visitGroupingExpr(Grouping* expr)
{
    return evaluate(expr->expression);
}

Object Interpreter::visitLambdaExpr(Lambda* expr)
{
    Function lambdaDeclaration(Token(), &(expr->params), expr->body);
    return Object(LoxFunction(lambdaDeclaration, environment, false));
}

Object Interpreter::visitLiteralExpr(Literal* expr)
{
    return expr->value;
}

Object Interpreter::visitLogicalExpr(Logical* expr)
{
    Object left = evaluate(expr->left);

    if (expr->lOperator.type == OR)
    {
        if (isTruthy(left)) return left;
    }
    else
    {
        if (!isTruthy(left)) return left;
    }

    return evaluate(expr->right);
}

/*
Object Interpreter::visitSetExpr(Set* expr)
{
    Object object = evaluate(expr->object);

    if (!(object.value.type() == typeid(LoxInstance)))
        throw RuntimeError(expr->name, "Only instances have fields.");

    Object value = evaluate(expr->value);
    std::any_cast<LoxInstance>(object).set(expr->name, value);
    return value;
}
*/

/*
Object Interpreter::visitSuperExpr(Super* expr)
{
    int distance = locals[expr];
    LoxClass superclass = environment.getAt(distance, expr.keyword);
    Token dummyToken = new Token(TokenType.THIS, "this", "this", 0);

    LoxInstance object = (LoxInstance)environment.getAt(
            distance - 1, dummyToken);

    LoxFunction method = superclass.findMethod(expr.method.lexeme);

    if (method == null) {
        throw new RuntimeError(expr.method,
                "Undefined property '" + expr.method.lexeme + "'.");
    }

    return method.bind(object);
}
*/

Object Interpreter::visitTernaryExpr(Ternary* expr)
{
    if (isTruthy(evaluate(expr->condition)))
        return evaluate(expr->trueBranch);
    return evaluate(expr->falseBranch);
}

/*
Object Interpreter::visitThisExpr(This* expr)
{
    return lookUpVariable(expr->keyword, expr);
}
*/

Object Interpreter::visitUnaryExpr(Unary* expr)
{
    #define double(value) std::any_cast<double>(value)
    
    Object right = evaluate(expr->right);

    switch(expr->uOperator.type)
    {
        case BANG:
            return Object(!isTruthy(right));
        case MINUS:
            checkNumberOperand(expr->uOperator, right);
            return Object(-double(right));
    }

    return Object(nullptr); // Random return value.
}

Object Interpreter::visitVariableExpr(Variable* expr)
{
    return lookUpVariable(expr->name, expr);
}

// Helper methods.

Object Interpreter::lookUpVariable(Token name, Expr *expr)
{
    if (locals.contains(expr))
    {
        int distance = locals[expr];
        return environment.getAt(distance, name);
    }
    else
        return globals.get(name);
}

void Interpreter::checkNumberOperand(Token bOperator, Object operand)
{
    if (typeid(operand) == typeid(double)) return;

    throw RuntimeError(bOperator, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token bOperator, Object left, Object right)
{
    if ((typeid(left) == typeid(double)) 
        && (typeid(right) == typeid(double))) 
        return;

    throw RuntimeError(bOperator, "Operands must be numbers.");
}

bool Interpreter::isTruthy(Object object)
{
    #define bool(value) std::any_cast<bool>(value)
    
    if (object.value.type() == typeid(nullptr)) return true;
    if (object.value.type() == typeid(bool)) return bool(object.value);
    return true;
}

bool Interpreter::isEqual(Object a, Object b)
{
    if ((a.value.type() == typeid(nullptr)) &&
        (b.value.type() == typeid(nullptr)))
        return true;
    
    if (a.value.type() == typeid(nullptr)) return false;

    return (a.value == b.value);
}

std::string Interpreter::stringify(Object object)
{
    if (object.value.type() == typeid(nullptr)) return "nil";

    #define double(value) std::any_cast<double>(value)
    #define bool(value) std::any_cast<bool>(value)
    #define string(value) std::any_cast<std::string>(value)

    if (object.value.type() == typeid(double)) return std::to_string(double(object.value));
    if (object.value.type() == typeid(bool))
    {
        if (bool(object.value)) return "true";
        else
            return "false";
    }
    if (object.value.type() == typeid(std::string)) return string(object.value);

    return ""; // Random return value.
}