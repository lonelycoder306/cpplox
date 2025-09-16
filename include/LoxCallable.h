#pragma once
#include "Object.h"
#include <vector>

class Interpreter;

class LoxCallable
{
    public:
        // virtual ~LoxCallable() = 0;
        virtual int arity() = 0;
        virtual Object call(Interpreter interpreter, std::vector<Object> arguments) = 0;
};