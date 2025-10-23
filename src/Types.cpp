#include "../include/Types.h"
#include "../include/BuiltinFunction.h"
#include "../include/ClassInstance.h"
#include "../include/ListObject.h"
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
    if (value.type() == typeid(BuiltinFunction))
        return LOX_NATIVE;
    if (value.type() == typeid(LoxClass))
        return LOX_CLASS;
    if (value.type() == typeid(LoxInstance *))
        return LOX_INST;
    if (value.type() == typeid(ClassInstance *))
        return CLASS_INST;
    if (value.type() == typeid(ListObject))
        return LIST;
    if (value.type() == typeid(ListFunction))
        return LIST_FUNC;
    if (value.type() == typeid(time_t))
        return TIME;
    if (value.type() == typeid(std::vector<int>))
		return NONE;
    if (value.type() == typeid(std::nullptr_t))
		return NONE;
	return INVALID;
}

Type type(Object object)
{
	return type(object.value);
}