#include "../include/Object.h"
#include "../include/Token.h"
#include "../include/TokenType.h"
#include <string>

Token::Token(TokenType type, std::string lexeme, Object literal, int line)
{
	this->type = type;
	this->lexeme = lexeme;
	this->literal = literal;
	this->line = line;
}

std::string Token::toString()
{
	if (type != eof)
	{
		if ((type == STRING) || (type == NUMBER))
			return types[type] + " " + lexeme + " " + literal.printVal();
		else
			return types[type] + " " + lexeme + " null";
	}
	else
		return "EOF null";
}