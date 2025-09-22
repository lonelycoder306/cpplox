#pragma once
#include "Classes.h"
#include "LoxClass.h"
#include "Object.h"
#include "Token.h"
#include <map>
#include <string>

class LoxInstance
{
    public:
        LoxInstance(const LoxClass& klass);
        Object get(Token name);
        void set(Token name, Object value);
        std::string toString();
    
    private:
        LoxClass klass;
        std::map<std::string, Object> fields;
};