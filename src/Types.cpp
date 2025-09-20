#include "../include/Types.h"
#include "../include/LoxFunction.h"
#include "../include/Object.h"
#include <any>
#include <string>
#include <typeinfo>

Type type(std::any value)
{
	if (value.type() == typeid(double))
		return NUM;
	if (value.type() == typeid(std::string))
		return STR;
	if (value.type() == typeid(bool))
		return BOOL;
	if (value.type() == typeid(LoxFunction))
		return LOXFUNC;
	if (value.type() == typeid(std::nullptr_t))
		return NONE;
	return INVALID;
}

Type type(Object object)
{
	return type(object.value);
}