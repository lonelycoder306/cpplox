#pragma once
#include "Object.h"
#include "Token.h"
#include <stdexcept>
#include <string>

enum ErrorType
{
	SCAN,
	PARSE,
	STATIC,
	RUNTIME
};

class BaseError : public std::exception
{
	public:
        int line;
        Token token;
        int column;
        std::string fileName;
		std::string message;
        std::string name;
        ErrorType type;

		BaseError(int line, std::string message);
		BaseError(Token token, std::string message);
		void show();
};

class ScanError : public BaseError
{
	public:
		ScanError(int line, int column, std::string file, std::string message);
};

class ParseError : public BaseError
{
	public:
		ParseError(Token token, std::string message);
};

class StaticError : public BaseError
{
	public:
		StaticError(Token token, std::string message);
};

class RuntimeError : public BaseError
{
	public:
		RuntimeError(Token token, std::string message);
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