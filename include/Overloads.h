#pragma once
#include "Expr.h"
#include "Nodes.h"
#include "Stmt.h"
#include <any>
#include <string>
#include <typeinfo>
#include <iterator>

bool operator==(const std::any& A, const std::any& B);

bool operator==(const vpE& A, const vpE& B);

bool operator==(const vpS& A, const vpS& B);

bool operator==(const vT& A, const vT& B);