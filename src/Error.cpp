#include "../include/Error.h"
#include "../include/Lox.h"
#include "../include/Object.h"
#include "../include/Token.h"
#include <string>

// BaseError.
BaseError::BaseError(int line, std::string message)
{
	this->line = line;
	this->message = message;
}

BaseError::BaseError(Token token, std::string message)
{
	this->token = token;
	this->message = message;
}

void BaseError::show()
{
    Lox::error(*this);
}

// ScanError.
ScanError::ScanError(int line, int column, std::string file, std::string message) :
    BaseError(line, message)
{
    this->name = "Scan";
    this->type = SCAN;
    this->column = column;
    this->fileName = file;
}

// ParseError.
ParseError::ParseError(Token token, std::string message) :
    BaseError(token, message)
{
    this->name = "Parse";
    this->type = PARSE;
}

// StaticError.
StaticError::StaticError(Token token, std::string message) :
    BaseError(token, message)
{
    this->name = "Static";
    this->type = STATIC;
}

// RuntimeError.
RuntimeError::RuntimeError(Token token, std::string message) :
    BaseError(token, message)
{
    this->name = "Runtime";
    this->type = RUNTIME;
}

// Break.
BreakError::BreakError(Token token, std::string loopType)
{
	this->token = token;
	this->loopType = loopType;
}

// Continue.
ContinueError::ContinueError(Token token, std::string loopType)
{
	this->token = token;
	this->loopType = loopType;
}

// Return.
ReturnValue::ReturnValue(Object value)
{
	this->value = value;
}