#pragma once
#include "Environment.h"
#include "LoxCallable.h"
// #include "LoxInstance.h"
#include "Object.h"
#include "Stmt.h"
#include <string>
#include <vector>

class Interpreter;

// final: No sub-classes (destructor can be non-virtual).
class LoxFunction final : public LoxCallable//<LoxFunction>
{
    public:
        Function declaration;

        LoxFunction(Function declaration, Environment closure, bool isInitializer);
        ~LoxFunction() = default;
        // LoxFunction bind(LoxInstance instance);
        Object call(Interpreter interpreter, std::vector<Object> arguments);
        bool isGetter();
        int arity();
        std::string toString();

    private:
        Environment closure;
        bool isInitializer;
};