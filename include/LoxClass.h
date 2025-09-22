#pragma once
#include "Classes.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "Object.h"
#include <map>
#include <string>
#include <vector>

// class LoxFunction;

class LoxClass : public LoxCallable
{
    public:
        std::string name;
        LoxClass* superclass;
        std::map<std::string, LoxFunction> methods;

        LoxClass(std::string name, 
                LoxClass* superclass, std::map<std::string, LoxFunction> methods);
        bool hasMethod(std::string name);
        LoxFunction findMethod(std::string name);
        std::string toString();
        Object call(Interpreter interpreter, std::vector<Object> arguments) override;
        int arity() override;
};