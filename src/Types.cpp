#include "../include/Types.h"
#include "../include/ClassInstance.h"
#include "../include/LoxClass.h"
#include "../include/LoxFunction.h"
#include "../include/LoxInstance.h"
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
		return LOX_FUNC;
    if (value.type() == typeid(LoxClass))
        return LOX_CLASS;
    if (value.type() == typeid(LoxInstance *))
        return LOX_INST;
    if (value.type() == typeid(ClassInstance *))
        return CLASS_INST;
    if (value.type() == typeid(std::nullptr_t))
		return NONE;
	return INVALID;
}

Type type(Object object)
{
	return type(object.value);
}