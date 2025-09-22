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
        int column;
        std::string fileName;

		Token() : line(0) {};
		Token(TokenType type, std::string lexeme, Object literal, int line,
              int column, std::string fileName);
        bool operator==(Token& other);
		std::string toString();
};