#include "../include/Overloads.h"
#include "../include/Nodes.h"
#include <any>
#include <string>

bool operator==(const std::any& A, const std::any& B)
{
    if (A.type() != B.type()) return false;

    #define double(value) std::any_cast<double>(value)
    #define bool(value) std::any_cast<bool>(value)
    #define string(value) std::any_cast<std::string>(value)

    if (A.type() == typeid(double))
        return (double(A) == double(B));
    else if (A.type() == typeid(bool))
        return (bool(A) == bool(B));
    else if (A.type() == typeid(std::string))
        return (string(A) == string(B));
    
    return false; // Random return value.
}

bool operator==(const vpE& A, const vpE& B)
{
    if (A.size() != B.size()) return false;

    for (size_t i = 0; i < A.size(); i++)
    {
        if (*(A[i]) != *(B[i])) return false;
    }

    return true;
}

bool operator==(const vpS& A, const vpS& B)
{
    if (A.size() != B.size()) return false;

    for (size_t i = 0; i < A.size(); i++)
    {
        if (*(A[i]) != *(B[i])) return false;
    }

    return true;
}

bool operator==(const vT& A, const vT& B)
{
    if (A.size() != B.size()) return false;
    for (size_t i = 0; i < A.size(); i++)
    {
        if (A[i] != B[i]) return false;
    }

    return true;
}