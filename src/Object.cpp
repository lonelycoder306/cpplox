#include "../include/Object.h"
#include "../include/LoxClass.h"
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
    if (type(*this) == LOX_FUNC)
        return "function";
    if (type(*this) == LOX_CLASS)
        return "class";
    if (type(*this) == LOX_INST)
        return "class instance";
    if (type(*this) == NONE)
        return "nil";
    
    return "unknown type";
}