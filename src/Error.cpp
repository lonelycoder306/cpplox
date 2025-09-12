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