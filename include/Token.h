#pragma once
#include "Object.h"
#include "TokenType.h"
#include <string>

class Token
{
	public:
		TokenType type;
		std::string lexeme;
		Object literal;
		int line;

		Token() : line(0) {};
		Token(TokenType type, std::string lexeme, Object literal, int line);
		std::string toString();
};