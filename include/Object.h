#pragma once
#include <any>
#include <string>

struct Object
{
	std::any value;

	Object() {}
	Object(std::any v) : value(v) {}
	std::string printVal();
    std::string printType();
};