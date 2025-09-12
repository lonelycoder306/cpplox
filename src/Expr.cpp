#include "../include/Expr.h"
#include "../include/Nodes.h"
#include "../include/Object.h"
#include "../include/Stmt.h"
#include "../include/Token.h"

// Constructor.
Expr::Expr() = default;

// Destructor.
Expr::~Expr() = default;

// Assign.
Assign::Assign(Token name, Expr* value)
{
    this->name = name;
    this->value = value;
}

// Binary.
Binary::Binary(Expr* left, Token bOperator, Expr* right)
{
    this->left = left;
    this->bOperator = bOperator;
    this->right = right;
}

// Call.
Call::Call(Expr* callee, Token paren, vpE arguments)
{
    this->callee = callee;
    this->paren = paren;
    this->arguments = arguments;
}

// Comma.
Comma::Comma(vpE expressions)
{
    this->expressions = expressions;
}

// Get.
Get::Get(Expr* object, Token name)
{
    this->object = object;
    this->name = name;
}

// Grouping.
Grouping::Grouping(Expr* expression)
{
    this->expression = expression;
}

// Lambda.
Lambda::Lambda(vT params, vpS body)
{
    this->params = params;
    this->body = body;
}

// Literal.
Literal::Literal(Object value)
{
    this->value = value;
}

// Logical.
Logical::Logical(Expr* left, Token lOperator, Expr* right)
{
    this->left = left;
    this->lOperator = lOperator;
    this->right = right;
}

// Set.
Set::Set(Expr* object, Token name, Expr* value)
{
    this->object = object;
    this->name = name;
    this->value = value;
}

// SExpr*r.
Super::Super(Token keyword, Token method)
{
    this->keyword = keyword;
    this->method = method;
}

// Ternary.
Ternary::Ternary(Expr* condition, Expr* trueBranch, Expr* falseBranch)
{
    this->condition = condition;
    this->trueBranch = trueBranch;
    this->falseBranch = falseBranch;
}

// This.
This::This(Token keyword)
{
    this->keyword = keyword;
}

// Unary.
Unary::Unary(Token uOperator, Expr* right)
{
    this->uOperator = uOperator;
    this->right = right;
}

// Variable.
Variable::Variable(Token name)
{
    this->name = name;
}