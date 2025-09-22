#include "../include/Interpreter.h"
// #include "../include/Cleaner.h"
#include "../include/Error.h"
#include "../include/Expr.h"
#include "../include/Lox.h"
#include "../include/LoxCallable.h"
#include "../include/LoxClass.h"
#include "../include/LoxFunction.h"
#include "../include/LoxInstance.h"
#include "../include/Nodes.h"
#include "../include/Object.h"
#include "../include/Overloads.h"
#include "../include/Stmt.h"
#include "../include/Types.h"
#include <any>
#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#define double(obj) std::any_cast<double>(obj.value)
#define bool(obj) std::any_cast<bool>(obj.value)
#define string(obj) std::any_cast<std::string>(obj.value)
#define func(obj) std::any_cast<LoxFunction>(obj.value)
#define class(obj) std::any_cast<LoxClass>(obj.value)
#define instance(obj) std::any_cast<LoxInstance *>(obj.value)

// General methods.

void Interpreter::interpret(vpS statements)
{    
    try
    {
        for (Stmt* stmt: statements)
        {
            execute(stmt);
            // if (Cleaner::cleanable(stmt))
            //     cleaner.clean(stmt);
        }
        // cleaner.clean(statements);
    }
    catch (RuntimeError& error)
    {
        error.show();
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

void Interpreter::executeBlock(vpS statements, Environment& environment)
{
    Environment* previous = this->environment;
    try
    {
        this->environment = &environment;

        for (Stmt* stmt: statements)
        {
            execute(stmt);
            // if (Cleaner::cleanable(stmt))
            //     cleaner.clean(stmt);
        }
    }
    catch (RuntimeError& error)
    {
        this->environment = previous;
        throw error;
    }
    // We need to catch it here (and rethrow it) so that
    // the environment "unwrapping" happens smoothly.
    // C++ doesn't have "finally" clauses, so this is the
    // closest thing we have.
    catch (ContinueError& error)
    {
        this->environment = previous;
        throw error;
    }
    this->environment = previous;
}

void Interpreter::resolve(Expr* expr, int depth)
{
    locals[expr] = depth;
}

// Statement methods.

void Interpreter::visitBreakStmt(Break* stmt)
{
    if (loopLevel != 0) throw BreakError(stmt->breakCMD, stmt->loopType);

    // Will only be thrown if break statement is reached.
    // Statement will not be reached after a false if-condition (for example).
    throw RuntimeError(stmt->breakCMD,
            "Cannot have 'break' outside loop.");
}

void Interpreter::visitBlockStmt(Block* stmt)
{
    Environment newEnv(environment);
    executeBlock(stmt->statements, newEnv);
}

void Interpreter::visitContinueStmt(Continue* stmt)
{
    if (loopLevel != 0) throw ContinueError(stmt->continueCMD, stmt->loopType);

    // Will only be thrown if continue statement is reached.
    // Statement will not be reached after a false if-condition (for example).
    throw RuntimeError(stmt->continueCMD,
            "Cannot have 'continue' outside loop.");
}

void Interpreter::visitClassStmt(Class* stmt)
{
    Object superclass(nullptr);
    auto superclassVar = dynamic_cast<Variable *>(stmt->superclass);
    if (stmt->superclass != nullptr)
    {
        superclass = evaluate(stmt->superclass);
        if (type(superclass) != LOXCLASS)
            throw RuntimeError(superclassVar->name,
                        "Superclass must be a class");
    }

    environment->define(stmt->name.lexeme, Object(nullptr));

    if (stmt->superclass != nullptr)
    {
        environment = new Environment(environment);
        environment->define("super", superclass);
    }

    LoxClass* superclassPtr = nullptr;
    if (stmt->superclass != nullptr)
    {
        LoxClass super = class(superclass);
        superclassPtr = new LoxClass(super);
    }

    std::map<std::string, LoxFunction> classMethods;
    for (Stmt* stmt : stmt->classMethods)
    {
        auto func = dynamic_cast<Function *>(stmt);
        LoxFunction function = LoxFunction(*func, environment, false);
        classMethods[func->name.lexeme] = function;
    }

    std::map<std::string, LoxFunction> methods;
    for (Stmt* stmt : stmt->methods)
    {
        auto func = dynamic_cast<Function *>(stmt);
        LoxFunction function = LoxFunction(*func, environment, (func->name.lexeme == "init"));
        methods[func->name.lexeme] = function;
    }

    LoxClass klass = LoxClass(stmt->name.lexeme, superclassPtr, methods);

    if (stmt->superclass != nullptr)
        environment = environment->enclosing;
    
    environment->assign(stmt->name, Object(klass));
}

void Interpreter::visitExpressionStmt(Expression* stmt)
{
    // Print out the return value of any expression statement (except assignments and function calls).
    if (!(dynamic_cast<Assign *>(stmt->expression)) &&
        !(dynamic_cast<Call *>(stmt->expression)) &&
        !(dynamic_cast<Set *>(stmt->expression)))
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
        environment->define(stmt->name.lexeme, Object(function));
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
    std::cout << stringify(value) << '\n';
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

    environment->define(stmt->name.lexeme, value);
}

void Interpreter::visitWhileStmt(While* stmt)
{
    loopLevel++;
    while(isTruthy(evaluate(stmt->condition)))
    {
        try
        {
            execute(stmt->body);
        }
        catch (BreakError& error)
        {
            (void) error;
            break;
        }
        catch (ContinueError& error)
        {
            if (error.loopType == "forLoop")
            {
                auto body = dynamic_cast<Block*>(stmt->body);
                vpS statements = body->statements;
                execute(statements[statements.size() - 1]);
            }
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
        environment->assignAt(distance, expr->name, value);
    }
    else
        globals.assign(expr->name, value);

    return value;
}

Object Interpreter::plus(Binary* expr, Object left, Object right)
{
    if ((type(left) == NUM) && (type(right) == NUM))
        return Object(double(left) + double(right));
    if ((type(left) == STR) && (type(right) == STR))
        return Object(string(left) + string(right));
    if (type(left) == STR)
        return Object(string(left) + stringify(right));
    if (type(right) == STR)
        return Object(stringify(left) + string(right));
    
    throw RuntimeError(expr->bOperator, "Cannot add given operands.");
}

Object Interpreter::visitBinaryExpr(Binary* expr)
{    
    Object left = evaluate(expr->left);
    Object right = evaluate(expr->right);

    switch (expr->bOperator.type)
    {
        case GREATER:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left) > double(right));
        case GREATER_EQUAL:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left) >= double(right));
        case LESS:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left) < double(right));
        case LESS_EQUAL:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left) <= double(right));
        case BANG_EQUAL: return Object(!isEqual(left, right));
        case EQUAL_EQUAL: return Object(isEqual(left, right));
        case MINUS:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left) - double(right));
        case PLUS:
            return plus(expr, left, right);
        case SLASH:
            checkNumberOperands(expr->bOperator, left, right);
            if (double(right) == 0)
                throw RuntimeError(expr->bOperator, "Division by zero not allowed.");
            return Object(double(left) / double(right));
        case STAR:
            checkNumberOperands(expr->bOperator, left, right);
            return Object(double(left) * double(right));
        default:
            return Object(nullptr);
    }
}

Object Interpreter::callFunc(Object callee, std::vector<Object> arguments, Call* expr)
{
    LoxFunction function = func(callee);
    //type function = std::any_cast<type>(callee);
    if ((int)arguments.size() != function.arity())
        throw RuntimeError(expr->paren, "Expected " +
            std::to_string(function.arity()) + " arguments but got " +
            std::to_string(arguments.size()) + ".");

    return function.call(*this, arguments);
}

Object Interpreter::callClass(Object callee, std::vector<Object> arguments, Call* expr)
{
    LoxClass klass = class(callee);
    //type function = std::any_cast<type>(callee);
    if ((int)arguments.size() != klass.arity())
        throw RuntimeError(expr->paren, "Expected " +
            std::to_string(klass.arity()) + " arguments but got " +
            std::to_string(arguments.size()) + ".");

    return klass.call(*this, arguments);
}

Object Interpreter::visitCallExpr(Call* expr)
{
    Object callee = evaluate(expr->callee);

    std::vector<Object> arguments;
    for (Expr* argument: expr->arguments)
        arguments.push_back(evaluate(argument));

    // Change to match any child-class of LoxCallable.
    if (!(type(callee) == LOXFUNC) && !(type(callee) == LOXCLASS))
        throw RuntimeError(expr->paren, "Can only call functions and classes.");

    //#define type LoxCallable<LoxFunction>

    // LoxFunction instead of LoxCallable (temporarily).
    if (type(callee) == LOXFUNC)
        return callFunc(callee, arguments, expr);
    if (type(callee) == LOXCLASS)
        return callClass(callee, arguments, expr);
    return Object(nullptr); // Unreachable.
}

Object Interpreter::visitCommaExpr(Comma* expr)
{
    vpE expressions = expr->expressions;
    int expressionNumber = (int) expressions.size();
    for (int i = 0; i < expressionNumber - 1; i++)
        evaluate(expressions[i]);
    return evaluate(expressions[expressionNumber - 1]);
}

Object Interpreter::visitGetExpr(Get* expr)
{
    Object object = evaluate(expr->object);
    if (type(object) == INSTANCE)
    {
        Object result = instance(object)->get(expr->name);
        if ((type(result) == LOXFUNC) && 
            (func(result)).isGetter())
                result = std::any_cast<LoxFunction>(result).call(*this, {});

        return result;
    }

    throw RuntimeError(expr->name, "Only instances have properties.");
}

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

Object Interpreter::visitSetExpr(Set* expr)
{
    Object object = evaluate(expr->object);

    if (type(object) != INSTANCE)
        throw RuntimeError(expr->name, "Only instances have fields.");

    Object value = evaluate(expr->value);
    instance(object)->set(expr->name, value);
    return value;
}

Object Interpreter::visitSuperExpr(Super* expr)
{
    int distance = locals[expr];
    LoxClass superclass = class(environment->getAt(distance, expr->keyword));
    Token dummyToken = Token(THIS, "this", Object("this"), 0);

    LoxInstance* object = instance(environment->getAt(distance - 1, dummyToken));

    if (!superclass.hasMethod(expr->method.lexeme))
        throw RuntimeError(expr->method,
                "Undefined property '" + expr->method.lexeme + "'.");

    LoxFunction method = superclass.findMethod(expr->method.lexeme);

    return Object(method.bind(object));
}

Object Interpreter::visitTernaryExpr(Ternary* expr)
{
    if (isTruthy(evaluate(expr->condition)))
        return evaluate(expr->trueBranch);
    return evaluate(expr->falseBranch);
}

Object Interpreter::visitThisExpr(This* expr)
{
    return lookUpVariable(expr->keyword, expr);
}

Object Interpreter::visitUnaryExpr(Unary* expr)
{    
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
        return environment->getAt(distance, name);
    }
    else
        return globals.get(name);
}

void Interpreter::checkNumberOperand(Token bOperator, Object operand)
{
    if (type(operand) == NUM) return;

    throw RuntimeError(bOperator, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token bOperator, Object left, Object right)
{
    if ((type(left) == NUM) && (type(right) == NUM))
        return;

    throw RuntimeError(bOperator, "Operands must be numbers.");
}

bool Interpreter::isTruthy(Object object)
{    
    if (type(object) == NONE) return false;
    if (type(object) == BOOL) return bool(object);
    return true;
}

bool Interpreter::isEqual(Object a, Object b)
{
    if ((type(a) == NONE) &&
        (type(b) == NONE))
        return true;
    
    if (type(a) == NONE) return false;

    return (a.value == b.value);
}

std::string Interpreter::stringify(Object object)
{
    if (type(object) == NONE) return "nil";

    if (type(object) == NUM)
    {
        std::string doubleString = std::to_string(double(object));
        size_t pos = doubleString.find(".0");
        if (pos != std::string::npos)
            return doubleString.substr(0, pos);
        return doubleString;
    }
    if (type(object) == BOOL)
    {
        if (bool(object)) return "true";
        else
            return "false";
    }
    if (type(object) == STR) return string(object);
    if (type(object) == LOXFUNC) return func(object).toString();
    if (type(object) == LOXCLASS) return class(object).toString();
    if (type(object) == INSTANCE) return instance(object)->toString();

    return ""; // Random return value.
}