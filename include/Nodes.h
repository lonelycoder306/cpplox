#pragma once
#include "Token.h"
#include <memory>
#include <vector>

class Expr;
class Assign;
class Binary;
class Call; 
class Comma; 
class Get;
class Grouping;
class Lambda;
class Literal;
class Logical;
class Set; 
class Super; 
class Ternary; 
class This; 
class Unary; 
class Variable;

class Stmt;
class Break;
class Block;
class Class;
class Continue;
class Function;
class If;
class Expression;
class Print;
class Return;
class Var;
class While;

using vT = std::vector<Token>;

using vpE = std::vector<Expr*>;
using vpS = std::vector<Stmt*>;