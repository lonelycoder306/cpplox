#pragma once
#include "Object.h"
#include "Token.h"
#include "TokenType.h"
#include <map>
#include <string>
#include <vector>

class Scanner
{
	public:
		Scanner(std::string& source);
		std::vector<Token> scanTokens();

	private:
		std::string source;
		std::vector<Token> tokens;
		int start = 0;
		int current = 0;
		int line = 1;
		std::map<std::string, TokenType> keywords;

		bool match(char expected);
		char peek();
		char peekNext();
		bool isAtEnd();
		char advance();
		void addToken(TokenType type);
		void addToken(TokenType type, Object literal);

		void scanToken();
		void identifier();
		void number();
		void string();
};