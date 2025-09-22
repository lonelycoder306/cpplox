#include "../include/LoxInstance.h"
#include "../include/Classes.h"
#include "../include/Error.h"
#include "../include/LoxClass.h"
#include "../include/LoxFunction.h"
#include "../include/Object.h"
#include "../include/Token.h"
#include <iostream>
#include <string>

LoxInstance::LoxInstance(LoxClass klass) :
    klass(klass) {}

Object LoxInstance::get(Token name)
{
    if (fields.contains(name.lexeme))
        return fields[name.lexeme];

    if (klass.hasMethod(name.lexeme))
    {
        LoxFunction method = klass.findMethod(name.lexeme);
        return Object(method.bind(this));
    }

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, Object value)
{
    fields[name.lexeme] = value;
}

std::string LoxInstance::toString()
{
    return "<" + klass.name + " instance>";
}