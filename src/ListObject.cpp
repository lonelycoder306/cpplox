#include "../include/ListObject.h"
#include "../include/Error.h"
#include "../include/Expr.h"
#include "../include/Interpreter.h"
#include "../include/Object.h"
#include "../include/Token.h"
#include "../include/Types.h"
#include <algorithm>
#include <any>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

static std::string_view functions[] = {
    "add", "insert", "pop", "remove", "delete",
    "join", "unique", "forEach", "transform", "filter", "flat",
    "contains", "duplicate", "index", "indexLast", "any", "all", "collect",
    "reverse", "sort", "sorted", "pair", "separate",
    "sum", "min", "max", "average"
};

// class ListObject

ListObject::ListObject()
{
    this->array = {};
    for (std::string_view function : functions)
        this->methods[function] = ListFunction(function);
}

ListObject::ListObject(std::vector<Object> array)
{
    this->array = array;
    for (std::string_view function : functions)
        this->methods[function] = ListFunction(function);
}

Object ListObject::get(Token name)
{
    auto it = methods.find(name.lexeme);
    if (it != methods.end())
    {
        ListFunction method = it->second;
        method.bind(*this);
        return Object(method);
    }

    throw RuntimeError(name, "Undefined property or method '" + name.lexeme + "'.");
}

Object& ListObject::operator[](int index)
{
    if (index > 0)
        return array[index];
    else
        return array[(int) array.size() + index];
}

bool ListObject::checkIndices(int start, int *end)
{
    int length = (int) this->array.size();
    if ((end == nullptr) && (start < -1*length))
        throw std::out_of_range("Index out of bounds.");
    else if ((end == nullptr) && (start >= length))
        throw std::out_of_range("Index out of bounds.");
    else if ((end != nullptr) && (start < 0))
        throw std::out_of_range("Start index out of bounds.");
    else if (end != nullptr)
    {
        if (((*end > 0) && (*end < start)) || (*end < -1*length))
            throw std::out_of_range("End index out of bounds.");
    }

    return true;
}

ListObject ListObject::partitionList(int start, int end)
{
    // Starting basic implementation.
    // Assuming end is not null.
    std::vector<Object> partition(end - start + 1);
    std::copy(this->array.begin() + start, this->array.begin() + end, 
            partition.begin());
    return ListObject(partition);
}

std::string ListObject::toString()
{
    Interpreter dummyInterpreter;
    std::string string = "[";

    for (int i = 0; i < (int) this->array.size(); i++)
    {
        Object element = this->array[i];
        if (i != 0) string += ", ";
        if (type(element) == STR)
            string += "\"";
        string += dummyInterpreter.stringify(element);
        if (type(element) == STR)
            string += "\"";
    }

    string += "]";
    return string;
}

// class ListFunction

#define double(obj) std::any_cast<double>(obj.value)

//static std::unordered_map<std::string_view, std::any> functions = {
//    {"add", ListFunction::add},
//    {"insert", ListFunction::insert},
//    {"pop", ListFunction::pop},
//    {"remove", ListFunction::remove}
//};

ListFunction::ListFunction(std::string_view mode)
{
    this->mode = mode;
    this->instance = nullptr;
}

void ListFunction::bind(ListObject& instance)
{
    this->instance = &instance;
}

Object ListFunction::call(Interpreter interpreter, Expr* expr, std::vector<Object> arguments)
{
    // Temporary implementation.
    Call* call = (Call *) expr;
    std::string_view calleeName = ((Variable *) call->callee)->name.lexeme;

    if (calleeName == "add")
    {
        add(*call, arguments[0]);
        return Object(nullptr);
    }
    else if (calleeName == "insert")
    {
        insert(*call, double(arguments[0]), arguments[1]);
        return Object(nullptr);
    }
    else if (calleeName == "pop")
        return pop(*call);
    else if (calleeName == "remove")
        return remove(*call, double(arguments[0]));
    else
        return Object(nullptr); // Temporary.
}

void ListFunction::add(Call expr, Object element)
{
    this->instance->array.push_back(element);
}

void ListFunction::insert(Call expr, double index, Object element) {}

Object ListFunction::pop(Call expr)
{
    Object last = this->instance->array.back();
    this->instance->array.pop_back();
    return last;
}

Object ListFunction::remove(Call expr, double index)
{
    // Temporarily to suppress errors.
    return Object(nullptr);
}

int ListFunction::arity()
{
    // Temporarily to suppress errors.
    return 1;
}

std::string ListFunction::toString()
{
    // Temporarily to suppress errors.
    return "";
}

bool ListFunction::check(Call expr, std::vector<Object> arguments)
{
    // Temporarily to suppress errors.
    return true;
}