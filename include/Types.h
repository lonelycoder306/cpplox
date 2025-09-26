#pragma once
#include "Object.h"
#include <any>
#include <string>
#include <typeinfo>

enum Type {
	NUM,
	STR,
	BOOL,
	LOX_FUNC,
    LOX_NATIVE,
    LOX_CLASS,
    LOX_INST,
    CLASS_INST,
    TIME,
	NONE,
	INVALID
};

Type type(std::any value);
Type type(Object object);