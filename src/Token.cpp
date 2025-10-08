#include "../include/Object.h"
#include "../include/Token.h"
#include "../include/TokenType.h"
#include <string>

Token::Token(TokenType type, std::string lexeme, Object literal, int line,
             int column, std::string fileName)
{
	this->type = type;
	this->lexeme = lexeme;
	this->literal = literal;
	this->line = line;
    this->column = column;
    this->fileName = fileName;
}

bool Token::operator==(Token& other)
{
    return ((this->type == other.type) &&
            (this->lexeme == other.lexeme) &&
            (this->line == other.line) &&
            (this->column == other.column) &&
            (this->fileName == other.fileName));
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