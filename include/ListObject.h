#pragma once
#include "LoxCallable.h"
#include "Nodes.h"
#include "Object.h"
#include "Token.h"
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

class Interpreter;
class ListObject;
class ListFunction;

class ListObject
{
    private:
        std::map<std::string_view, ListFunction> methods;
    
    public:
        std::vector<Object> array;
        
        ListObject();
        ListObject(std::vector<Object> array);
        Object get(Token name);
        void set() {}
        Object& operator[](int index);
        // End is a pointer to allow it to be a nullptr.
        bool checkIndices(int start, int *end);
        ListObject partitionList(int start, int end);
        std::string toString();
};

class ListFunction final : public LoxCallable
{
    private:
        std::string_view mode;
        ListObject *instance;

        bool check(Call expr, std::vector<Object> arguments);
    
    public:
        ListFunction() = default;
        ListFunction(std::string_view mode);
        void bind(ListObject& instance);
        Object call(Interpreter interpreter, Expr* expr, std::vector<Object> arguments) override;

        void add(Call expr, Object element);
        void insert(Call expr, double index, Object element);
        Object pop(Call expr);
        Object remove(Call expr, double index);

        int arity() override;
        std::string toString();
};

class ListInit : public LoxCallable
{
    private:
        bool check(Call expr, std::vector<Object> arguments);
        List arrayList(List list);
        List stringList(std::string string);
        List floatList(double size);
    
    public:
        ListInit() = default;
        Object call(Interpreter interpreter, Expr* expr, std::vector<Object> arguments) override;
        int arity() override;
        std::string toString();
};