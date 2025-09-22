#include "../include/LoxClass.h"
#include "../include/Interpreter.h"
#include "../include/LoxFunction.h"
#include "../include/LoxInstance.h"
#include "../include/Object.h"
#include <string>

LoxClass::LoxClass(std::string name, LoxClass* metaclass,
    LoxClass* superclass, std::map<std::string, LoxFunction> methods) :
        ClassInstance(Object(metaclass))
{
    this->name = name;
    this->superclass = superclass;
    this->methods = methods;
}

bool LoxClass::hasMethod(std::string name)
{
    if (methods.contains(name)) return true;

    if (superclass != nullptr)
        return superclass->hasMethod(name);
    
    return false;
}

LoxFunction LoxClass::findMethod(std::string name)
{
    if (methods.contains(name))
        return methods[name];

    return superclass->findMethod(name);
}

std::string LoxClass::toString()
{
    return "<class " + name + ">";
}

Object LoxClass::call(Interpreter interpreter, std::vector<Object> arguments)
{
    LoxInstance* ptr = new LoxInstance(*this);
    if (hasMethod("init"))
    {
        LoxFunction initializer = findMethod("init");
        initializer.bind(ptr).call(interpreter, arguments);
    }
    return Object(ptr);
}

int LoxClass::arity() {
    if (!hasMethod("init")) return 0;
    
    LoxFunction initializer = findMethod("init");
    return initializer.arity();
}