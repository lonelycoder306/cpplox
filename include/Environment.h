#pragma once
#include "Object.h"
#include "Token.h"
#include <map>
#include <string>

class Environment
{
    public:
        Environment* enclosing;
        std::map<std::string, Object> values;

        Environment();
        Environment(Environment* enclosing);
        Object get(Token name);
        void assign(Token name, Object value);
        void define(std::string name, Object value, bool access);
        Environment* ancestor(int distance);
        Object getAt(int distance, Token name);
        void assignAt(int distance, Token name, Object value);

    private:
        std::map<std::string, bool> varAccess;
};