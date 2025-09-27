#include "../include/BuiltinFunction.h"
#include "../include/Environment.h"
#include "../include/Error.h"
#include "../include/Expr.h"
#include "../include/Interpreter.h"
#include "../include/Object.h"
#include "../include/Token.h"
#include "../include/Types.h"
#include <cctype>
#include <string>
#include <vector>

Environment builtinSetup()
{
    std::vector<std::string> functions = {"clock", "type", "string", "number", "length"};
    Environment builtins;
    for (std::string function : functions)
        builtins.define(function, Object(BuiltinFunction(function)), "VAR");
    return builtins;
}

BuiltinFunction::BuiltinFunction(std::string mode)
{
    this->mode = mode;
}

Object BuiltinFunction::call(Interpreter interpreter, Expr* expr, std::vector<Object> arguments)
{
    (void) expr; // To silence error.
    
    if (mode == "clock")
        return b_clock();
    if (mode == "type")
        return b_type(arguments[0]);
    if (mode == "string")
        return b_string(interpreter, arguments[0]);
    if (mode == "number")
        return b_number(dynamic_cast<Call *>(expr), arguments[0]);
    if (mode == "length")
        return b_length(dynamic_cast<Call *>(expr), arguments[0]);
    return Object(nullptr); // Unreachable.
}

Object BuiltinFunction::b_clock()
{
    return Object(time(0));
}

Object BuiltinFunction::b_type(Object object)
{
    return Object(object.printType());
}

Object BuiltinFunction::b_string(Interpreter interpreter, Object object)
{
    return Object(interpreter.stringify(object));
}

Object BuiltinFunction::b_number(Call* expr, Object object)
{
    Token callee;
    Variable* check;
    if ((check = dynamic_cast<Variable *>(expr->callee)))
        callee = check->name;
    // Check for access expression.

    if (type(object) != STR)
        throw RuntimeError(callee, "Invalid input to number().");
    std::string text = std::any_cast<std::string>(object.value);
    for (char c : text)
    {
        if (!isdigit(c) && (c != '.') && (c != '+') && (c != '-'))
            throw RuntimeError(callee, "Invalid input to number().");
    }
    
    return Object(std::stod(text));
}

// Only for strings for the time being.
Object BuiltinFunction::b_length(Call* expr, Object object)
{
    Token callee;
    Variable* check;
    if ((check = dynamic_cast<Variable *>(expr->callee)))
        callee = check->name;
    // Check for access expression.

    if (type(object) != STR)
        throw RuntimeError(callee, "Invalid input to length().");
    std::string text = std::any_cast<std::string>(object.value);
    
    return Object((double) text.size());
}

int BuiltinFunction::arity()
{
    if (mode == "clock")
        return 0;
    if (mode == "type")
        return 1;
    if (mode == "string")
        return 1;
    if (mode == "number")
        return 1;
    if (mode == "length")
        return 1;
    return -1; // Unreachable.
}

std::string BuiltinFunction::toString()
{
    return "<native fn>";
}