#include "../include/Object.h"
#include "../include/LoxFunction.h"
#include "../include/Types.h"
#include <string>
#include <typeinfo>

std::string Object::printVal()
{
	if (type(*this) == NUM)
		return std::to_string(any_cast<double>(this->value));
	if (type(*this) == BOOL)
	{
		bool v = any_cast<bool>(this->value);
		if (v) return "true";
		else
			return "false";
	}
	if (type(*this) == STR)
		return any_cast<std::string>(this->value);
    if (type(*this) == LOXFUNC)
    {
        LoxFunction function = std::any_cast<LoxFunction>(this->value);
        return function.declaration.name.lexeme;
    }
    if (type(*this) == NONE)
        return "nil";
    
    return "NULL";
}

std::string Object::printType()
{
	if (type(*this) == NUM)
		return "number";
	if (type(*this) == BOOL)
        return "boolean";
	if (type(*this) == STR)
		return "string";
    if (type(*this) == LOXFUNC)
        return "function";
    if (type(*this) == NONE)
        return "nil";
    
    return "unknown type";
}