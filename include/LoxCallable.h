#pragma once
#include "Object.h"
#include <vector>

class Interpreter;

//template <typename FuncType>
class LoxCallable
{
    public:
        // virtual ~LoxCallable() = 0;
        virtual int arity() = 0;
        virtual Object call(Interpreter interpreter, std::vector<Object> arguments) = 0;
};

/*
template <typename FuncType>
int LoxCallable<FuncType>::arity()
{
	return static_cast<FuncType*>(this)->arity();
}

template <typename FuncType>
Object LoxCallable<FuncType>::call(Interpreter& interpreter, std::vector<Object> arguments)
{
	return static_cast<FuncType*>(this)->call(interpreter, arguments);
}
*/