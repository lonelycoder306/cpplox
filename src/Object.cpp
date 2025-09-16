#include "../include/Object.h"
#include <string>
#include <typeinfo>

std::string Object::printVal()
{
	if (this->value.type() == typeid(int))
		return std::to_string(any_cast<int>(this->value));
	if (this->value.type() == typeid(double))
		return std::to_string(any_cast<double>(this->value));
	if (this->value.type() == typeid(bool))
	{
		bool v = any_cast<bool>(this->value);
		if (v) return "true";
		else
			return "false";
	}
	if (this->value.type() == typeid(std::string))
		return any_cast<std::string>(this->value);
    
    return "NULL";
}

std::string Object::printType()
{
    if (this->value.type() == typeid(int))
		return "number";
	if (this->value.type() == typeid(double))
		return "number";
	if (this->value.type() == typeid(bool))
        return "boolean";
	if (this->value.type() == typeid(std::string))
		return "string";
    
    return "unknown";
}