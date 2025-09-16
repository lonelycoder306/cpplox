#include "../include/Environment.h"
#include "../include/Error.h"
#include "../include/Object.h"
#include "../include/Token.h"
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

Environment::Environment()
{
    this->enclosing = nullptr;
}

Environment::Environment(Environment* enclosing)
{
    this->enclosing = enclosing;
}

Object Environment::get(Token name)
{
    if (values.contains(name.lexeme))
    {
        Object obj = values[name.lexeme];
        // Check that value has been given a value.
        // Temporarily using int-type vectors to indicate no assigned value.
        if (!(obj.value.type() == typeid(std::vector<int>)))
            return obj;

        throw new RuntimeError(name,
                "Uninitialized variable '" + name.lexeme + "'.");
    }

    else if (enclosing != nullptr)
        return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, Object value)
{
    if (values.contains(name.lexeme))
    {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing != nullptr)
    {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::define(std::string name, Object value)
{
    values[name] = value;
}

Environment Environment::ancestor(int distance)
{
    Environment environment = this;
    for (int i = 0; i < distance; i++)
        environment = *(environment.enclosing);

    return environment;
}

Object Environment::getAt(int distance, Token name)
{
    return ancestor(distance).get(name);
}

void Environment::assignAt(int distance, Token name, Object value)
{
    ancestor(distance).assign(name, value);
}