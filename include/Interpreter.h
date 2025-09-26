#pragma once
// #include "Cleaner.h"
#include "Environment.h"
#include "Expr.h"
#include "Object.h"
#include "Overloads.h"
#include "Stmt.h"
#include "Token.h"
#include "Visitor.h"
#include <string>
#include <vector>
#include <map>

/*
// Custom struct type to avoid using map or unordered_map.
// The form requires a < operator, while the latter requires
// a hash function and an == operator.
struct distArray
{
    std::vector<Expr> keys;
    std::vector<int> distances;

    bool contains(const Expr& key)
    {
        auto it = std::find(keys.begin(), keys.end(), key);
        if (it == keys.end()) return false;
        return true;
    }
    
    int operator[](Expr& key)
    {
        auto it = std::find(keys.begin(), keys.end(), key);
        int index = std::distance(keys.begin(), it);
        return distances[index];
    }
};
*/

class Interpreter : public Visitor
{
    public:
        Environment globals;
        Environment builtins;

        Interpreter();
        void interpret(vpS);
        void execute(Stmt*);
        Object evaluate(Expr* expr);
        void executeBlock(vpS statements, Environment& environment);
        void resolve(Expr* expr, int depth);

        // Statement methods.

        void visitBreakStmt(Break* stmt) override;
        void visitBlockStmt(Block* stmt) override;
        void visitClassStmt(Class* stmt) override;
        void visitContinueStmt(Continue* stmt) override;
        void visitExpressionStmt(Expression* stmt) override;
        void visitFunctionStmt(Function* stmt) override;
        void visitIfStmt(If* stmt) override;
        void visitPrintStmt(Print* stmt) override;
        void visitReturnStmt(Return* stmt) override;
        void visitVarStmt(Var* stmt) override;
        void visitWhileStmt(While* stmt) override;

        // Expression methods.

        Object visitAssignExpr(Assign* expr) override;
        Object visitBinaryExpr(Binary* expr) override;
        Object visitCallExpr(Call* expr) override;
        Object visitCommaExpr(Comma* expr) override;
        Object visitGetExpr(Get* expr) override;
        Object visitGroupingExpr(Grouping* expr) override;
        Object visitLambdaExpr(Lambda* expr) override;
        Object visitLiteralExpr(Literal* expr) override;
        Object visitLogicalExpr(Logical* expr) override;
        Object visitSetExpr(Set* expr) override;
        Object visitSuperExpr(Super* expr) override;
        Object visitTernaryExpr(Ternary* expr) override;
        Object visitThisExpr(This* expr) override;
        Object visitUnaryExpr(Unary* expr) override;
        Object visitVariableExpr(Variable* expr) override;

        // Helper methods.
        std::string stringify(Object object); // Public to use in built-in function string().

    private:
        Environment* environment = &globals;
        std::map<Expr*, int> locals;
        int loopLevel = 0;
        // Cleaner cleaner;

        // Helper methods.
        Object lookUpVariable(Token name, Expr *expr);
        void checkNumberOperand(Token bOperator, Object operand);
        void checkNumberOperands(Token bOperator, Object left, Object right);
        bool isTruthy(Object object);
        bool isEqual(Object a, Object b);
        Object plus(Binary* expr, Object left, Object right);
        Object callFunc(Object callee, std::vector<Object> arguments, Call* expr);
        Object callNative(Object callee, std::vector<Object> arguments, Call* expr);
        Object callClass(Object callee, std::vector<Object> arguments, Call* expr);
};