#include "../include/ClassInstance.h"
#include "../include/Error.h"
#include "../include/LoxClass.h"
#include "../include/Object.h"
#include "../include/Token.h"

#define class(obj) std::any_cast<LoxClass *>(obj.value)

ClassInstance::ClassInstance(Object klassObj)
{
    this->klassObj = klassObj;
}

Object ClassInstance::get(Token name)
{
    if (fields.contains(name.lexeme))
        return fields[name.lexeme];

    LoxClass* klass = class(klassObj);
    if (klass->hasMethod(name.lexeme))
    {
        LoxFunction method = klass->findMethod(name.lexeme);
        return Object(method.bind(this));
    }

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void ClassInstance::set(Token name, Object value)
{
    fields[name.lexeme] = value;
}