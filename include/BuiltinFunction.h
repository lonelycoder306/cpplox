#pragma once
#include "Environment.h"
#include "Expr.h"
#include "LoxCallable.h"
#include "Object.h"
#include <string>
#include <vector>

Environment builtinSetup();

class Interpreter;

class BuiltinFunction : public LoxCallable
{
    public:
        BuiltinFunction(std::string mode);
        Object call(Interpreter interpreter, Expr* expr, std::vector<Object> arguments);
        int arity();
        std::string toString();
    
    private:
        std::string mode;

        Object b_clock(); // time_t
        Object b_type(Interpreter interpreter, Object object); // std::string
        Object b_string(Interpreter interpreter, Object object); // std::string
        Object b_number(Call* expr, Object object); // double
        Object b_length(Call* expr, Object object); // double
};