#pragma once
#include "ClassInstance.h"
#include "Environment.h"
#include "LoxCallable.h"
#include "Object.h"
#include "Stmt.h"
#include <string>
#include <vector>

class Interpreter;
class LoxInstance;

// final: No sub-classes (destructor can be non-virtual).
class LoxFunction final : public LoxCallable//<LoxFunction>
{
    public:
        Function declaration;

        LoxFunction() = default;
        LoxFunction(Function declaration, Environment closure, bool isInitializer);
        ~LoxFunction() = default;
        LoxFunction bind(LoxInstance* instance);
        LoxFunction bind(ClassInstance* instance);
        Object call(Interpreter interpreter, Expr* expr, std::vector<Object> arguments);
        bool isGetter();
        int arity();
        std::string toString();

    private:
        Environment closure;
        bool isInitializer;
};