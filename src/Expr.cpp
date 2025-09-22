#include "../include/Expr.h"
#include "../include/Nodes.h"
#include "../include/Object.h"
#include "../include/Overloads.h"
#include "../include/Stmt.h"
#include "../include/Token.h"
#include "../include/Visitor.h"

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

Object Assign::accept(Visitor& visitor)
{
    return visitor.visitAssignExpr(this);
}

bool Assign::operator==(Expr& other)
{
    auto check = dynamic_cast<Assign *>(&other);
    if (!check) return false;
    return ((this->name == check->name) &&
            (*(this->value) == *(check->value)));

}

// Binary.
Binary::Binary(Expr* left, Token bOperator, Expr* right)
{
    this->left = left;
    this->bOperator = bOperator;
    this->right = right;
}

Object Binary::accept(Visitor& visitor)
{
    return visitor.visitBinaryExpr(this);
}

bool Binary::operator==(Expr& other)
{
    auto check = dynamic_cast<Binary *>(&other);
    if (!check) return false;
    return ((*(this->left) == *(check->left)) &&
            (this->bOperator == check->bOperator) &&
            (*(this->right) == *(check->right)));

}

// Call.
Call::Call(Expr* callee, Token paren, vpE arguments)
{
    this->callee = callee;
    this->paren = paren;
    this->arguments = arguments;
}

Object Call::accept(Visitor& visitor)
{
    return visitor.visitCallExpr(this);
}

bool Call::operator==(Expr& other)
{
    auto check = dynamic_cast<Call *>(&other);
    if (!check) return false;
    return ((*(this->callee) == *(check->callee)) &&
            (this->paren == check->paren) &&
            (this->arguments == check->arguments)); // Overload for vpE.
}

// Comma.
Comma::Comma(vpE expressions)
{
    this->expressions = expressions;
}

Object Comma::accept(Visitor& visitor)
{
    return visitor.visitCommaExpr(this);
}

bool Comma::operator==(Expr& other)
{
    auto check = dynamic_cast<Comma *>(&other);
    if (!check) return false;
    return (this->expressions == check->expressions);
}

// Get.
Get::Get(Expr* object, Token name)
{
    this->object = object;
    this->name = name;
}

Object Get::accept(Visitor& visitor)
{
    return visitor.visitGetExpr(this);
}

bool Get::operator==(Expr& other)
{
    auto check = dynamic_cast<Get *>(&other);
    if (!check) return false;
    return ((*(this->object) == *(check->object)) &&
            (this->name == check->name));
}

// Grouping.
Grouping::Grouping(Expr* expression)
{
    this->expression = expression;
}

Object Grouping::accept(Visitor& visitor)
{
    return visitor.visitGroupingExpr(this);
}

bool Grouping::operator==(Expr& other)
{
    auto check = dynamic_cast<Grouping *>(&other);
    if (!check) return false;
    return (*(this->expression) == *(check->expression));
}

// Lambda.
Lambda::Lambda(vT params, vpS body)
{
    this->params = params;
    this->body = body;
}

Object Lambda::accept(Visitor& visitor)
{
    return visitor.visitLambdaExpr(this);
}

bool Lambda::operator==(Expr& other)
{
    auto check = dynamic_cast<Lambda *>(&other);
    if (!check) return false;
    return ((this->params == check->params) &&
            (this->body == check->body));
}

// Literal.
Literal::Literal(Object value)
{
    this->value = value;
}

Object Literal::accept(Visitor& visitor)
{
    return visitor.visitLiteralExpr(this);
}

bool Literal::operator==(Expr& other)
{
    auto check = dynamic_cast<Literal *>(&other);
    if (!check) return false;
    return (this->value.value == check->value.value);
}

// Logical.
Logical::Logical(Expr* left, Token lOperator, Expr* right)
{
    this->left = left;
    this->lOperator = lOperator;
    this->right = right;
}

Object Logical::accept(Visitor& visitor)
{
    return visitor.visitLogicalExpr(this);
}

bool Logical::operator==(Expr& other)
{
    auto check = dynamic_cast<Logical *>(&other);
    if (!check) return false;
    return ((*(this->left) == *(check->left)) &&
            (this->lOperator == check->lOperator) &&
            (*(this->right) == *(check->right)));
}

// Set.
Set::Set(Expr* object, Token name, Expr* value)
{
    this->object = object;
    this->name = name;
    this->value = value;
}


Object Set::accept(Visitor& visitor)
{
    return visitor.visitSetExpr(this);
}

bool Set::operator==(Expr& other)
{
    auto check = dynamic_cast<Set *>(&other);
    if (!check) return false;
    return ((*(this->object) == *(check->object)) &&
            (this->name == check->name) &&
            (*(this->value) == *(check->value)));
}

// Super.
Super::Super(Token keyword, Token method)
{
    this->keyword = keyword;
    this->method = method;
}


Object Super::accept(Visitor& visitor)
{
    return visitor.visitSuperExpr(this);
}

bool Super::operator==(Expr& other)
{
    auto check = dynamic_cast<Super *>(&other);
    if (!check) return false;
    return ((this->keyword == check->keyword) &&
            (this->method == check->method));
}

// Ternary.
Ternary::Ternary(Expr* condition, Expr* trueBranch, Expr* falseBranch)
{
    this->condition = condition;
    this->trueBranch = trueBranch;
    this->falseBranch = falseBranch;
}

Object Ternary::accept(Visitor& visitor)
{
    return visitor.visitTernaryExpr(this);
}

bool Ternary::operator==(Expr& other)
{
    auto check = dynamic_cast<Ternary *>(&other);
    if (!check) return false;
    return ((*(this->condition) == *(check->condition)) &&
            (*(this->trueBranch) == *(check->trueBranch)) &&
            (*(this->falseBranch) == *(check->falseBranch)));
}

// This.
This::This(Token keyword)
{
    this->keyword = keyword;
}

Object This::accept(Visitor& visitor)
{
    return visitor.visitThisExpr(this);
}

bool This::operator==(Expr& other)
{
    auto check = dynamic_cast<This *>(&other);
    if (!check) return false;
    return (this->keyword == check->keyword);
}

// Unary.
Unary::Unary(Token uOperator, Expr* right)
{
    this->uOperator = uOperator;
    this->right = right;
}

Object Unary::accept(Visitor& visitor)
{
    return visitor.visitUnaryExpr(this);
}

bool Unary::operator==(Expr& other)
{
    auto check = dynamic_cast<Unary *>(&other);
    if (!check) return false;
    return ((this->uOperator == check->uOperator) &&
            (*(this->right) == *(check->right)));
}

// Variable.
Variable::Variable(Token name)
{
    this->name = name;
}

Object Variable::accept(Visitor& visitor)
{
    return visitor.visitVariableExpr(this);
}

bool Variable::operator==(Expr& other)
{
    auto check = dynamic_cast<Variable *>(&other);
    if (!check) return false;
    return (this->name == check->name);
}