#pragma once
#include "Classes.h"
#include "ClassInstance.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "Object.h"
#include <map>
#include <string>
#include <vector>

class LoxClass final : public LoxCallable, public ClassInstance
{
    public:
        std::string name;
        LoxClass* superclass;
        std::map<std::string, LoxFunction> methods;

        LoxClass() = default;
        LoxClass(std::string name, LoxClass* metaclass,
                LoxClass* superclass, std::map<std::string, LoxFunction> methods);
        bool hasMethod(std::string name);
        LoxFunction findMethod(std::string name);
        std::string toString();
        Object call(Interpreter interpreter, Expr* expr, std::vector<Object> arguments) override;
        int arity() override;
};