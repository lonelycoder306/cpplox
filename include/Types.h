#pragma once
#include "Object.h"
#include <any>
#include <string>
#include <typeinfo>

enum Type {
	NUM,
	STR,
	BOOL,
	LOXFUNC,
	NONE,
	INVALID
};

Type type(std::any value);
Type type(Object object);