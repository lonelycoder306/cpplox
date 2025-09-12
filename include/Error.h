#pragma once
#include "Object.h"
#include "Token.h"
#include <stdexcept>
#include <string>

class BaseError : public std::exception
{
	public:
		int line;
		Token token;
		std::string message;

		BaseError(int line, std::string message);
		BaseError(Token token, std::string message);
};

class LexError : public BaseError
{
	public:
		LexError(int line, std::string message) : BaseError(line, message) {};
};

class ParseError : public BaseError
{
	public:
		ParseError(Token token, std::string message) : BaseError(token, message) {};
};

class ResolveError : public BaseError
{
	public:
		ResolveError(Token token, std::string message) : BaseError(token, message) {};
};

class RuntimeError : public BaseError
{
	public:
		RuntimeError(Token token, std::string message) : BaseError(token, message) {};
};

class BreakError : public std::exception
{
	public:
		Token token;
		std::string loopType;

		BreakError(Token token, std::string loopType);
};

class ContinueError : public std::exception
{
	public:
		Token token;
		std::string loopType;

		ContinueError(Token token, std::string loopType);
};

class ReturnValue : public std::exception
{
	public:
		Object value;

		ReturnValue(Object value);
};