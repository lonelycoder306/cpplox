#include "../include/LoxFunction.h"
#include "../include/Environment.h"
#include "../include/Error.h"
#include "../include/Interpreter.h"
#include "../include/LoxInstance.h"
#include "../include/Object.h"
#include "../include/Stmt.h"
#include <string>
#include <vector>

LoxFunction::LoxFunction(Function declaration, Environment closure, bool isInitializer) :
    declaration(declaration)
{
    this->closure = closure;
    this->isInitializer = isInitializer;
}

LoxFunction LoxFunction::bind(LoxInstance* instance)
{
    Environment environment = new Environment(closure);
    environment.define("this", Object(instance));
    return LoxFunction(declaration, environment, isInitializer);
}

Object LoxFunction::call(Interpreter interpreter, std::vector<Object> arguments)
{
    Environment environment = new Environment(closure);
    if (declaration.params != nullptr)
    {
        vT params = *declaration.params;
        for (int i = 0; i < (int) declaration.params->size(); i++)
            environment.define(params[i].lexeme, arguments[i]);
    }

    //Edit to match my getAt code.
    Token dummyToken = Token(THIS, "this", Object(nullptr), 0);

    try
    {
        interpreter.executeBlock(declaration.body, environment);
    }
    catch (ReturnValue& returnValue)
    {
        if (isInitializer) return closure.getAt(0, dummyToken);

        return returnValue.value;
    }

    if (isInitializer) return closure.getAt(0, dummyToken);
    return Object(nullptr);
}

bool LoxFunction::isGetter()
{
    return (declaration.params == nullptr);
}

int LoxFunction::arity()
{
    if (declaration.params == nullptr) return 0;
    return (int) declaration.params->size();
}

std::string LoxFunction::toString()
{
    return "<fn " + declaration.name.lexeme + ">";
}