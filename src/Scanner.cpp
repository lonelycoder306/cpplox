#include "../include/Scanner.h"
#include "../include/Error.h"
#include "../include/Lox.h"
#include "../include/Object.h"
#include "../include/TokenType.h"
#include <cctype>
#include <string>

Scanner::Scanner(std::string& source, std::string& fileName)
{
	this->source = source;
    this->fileName = fileName;
	keywords["and"] = AND;
    keywords["break"] = BREAK;
	keywords["class"] = CLASS;
    keywords["continue"] = CONTINUE;
	keywords["else"] = ELSE;
	keywords["false"] = FALSE;
	keywords["fix"] = FIX;
	keywords["for"] = FOR;
	keywords["fun"] = FUN;
	keywords["if"] = IF;
	keywords["nil"] = NIL;
	keywords["or"] = OR;
	keywords["print"] = PRINT;
	keywords["return"] = RETURN;
	keywords["super"] = SUPER;
	keywords["this"] = THIS;
	keywords["true"] = TRUE;
	keywords["var"] = VAR;
	keywords["while"] = WHILE;
}

std::vector<Token> Scanner::scanTokens()
{
	while (!isAtEnd())
	{
		start = current;
        try
        {
            scanToken();
        }
        catch(ScanError& error)
        {
            error.show();
        }
	}

	tokens.push_back(Token(eof, "", Object(NULL), line, column, fileName));
	return tokens;
}

bool Scanner::match(char expected)
{
	if (isAtEnd()) return false;
	if (source[current] != expected) return false;
	current++;
	return true;
}

char Scanner::peek()
{
	if (isAtEnd()) return '\0';
	return source[current];
}

char Scanner::peekNext()
{
	if ((current + 1) >= source.length()) return '\0';
	return source[current + 1];
}

bool Scanner::isAtEnd()
{
	return (current >= source.length());
}

char Scanner::advance()
{
	current++;
	return source[current - 1];
}

void Scanner::addToken(TokenType type)
{
	addToken(type, Object(NULL));
}

void Scanner::addToken(TokenType type, Object literal)
{
	std::string text = source.substr(start, current - start);
	tokens.push_back(Token(type, text, literal, line, column, fileName));
}

void Scanner::scanToken()
{
	char c = advance();
    column++;

	switch (c)
	{
		case '(': addToken(LEFT_PAREN); break;
		case ')': addToken(RIGHT_PAREN); break;
		case '{': addToken(LEFT_BRACE); break;
		case '}': addToken(RIGHT_BRACE); break;
		case ',': addToken(COMMA); break;
		case '.': addToken(DOT); break;
		case '-': addToken(MINUS); break;
		case '+': addToken(PLUS); break;
		case ';': addToken(SEMICOLON); break;
		case '*': addToken(STAR); break;
		case '?': addToken(Q_MARK); break;
		case ':': addToken(COLON); break;
        case '^': addToken(POWER); break;
        case '%': addToken(MOD); break;

		case '!':
			addToken(match('=') ? BANG_EQUAL : BANG);
			break;
		case '=':
			addToken(match('=') ? EQUAL_EQUAL : EQUAL);
			break;
		case '<':
			addToken(match('=') ? LESS_EQUAL : LESS);
			break;
		case '>':
			addToken(match('=') ? GREATER_EQUAL : GREATER);
			break;

		case '/':
			if (match('/'))
			{
				// A comment goes until the end of the line.
				while ((peek() != '\n') && !isAtEnd()) advance();
			}

			else if (match('*'))
			{
				int count = 1;
				while ((count != 0) && !isAtEnd())
				{
					// Newlines: increment line and skip a character.
					if (peek() == '\n') line++;
					else if ((peek() == '/') && (peekNext() == '*'))
					{
						count++;
						// Skip two characters (combined with advance outside if-else-if block).
						advance();
					}
					else if ((peek() == '*') && (peekNext() == '/'))
					{
						count--;
						advance();
					}
					advance(); // Avoids adding an extra advance() in each block.
				}

				if (count != 0)
				{
					throw ScanError(line, column, fileName, "Unterminated comment block.");
				}
			}

			else
				addToken(SLASH);
			break;

		case ' ':
		case '\r':
		case '\t':
			// Ignore whitespace.
			break;

		case '\n':
			line++;
            column = 0;
			break;

		case '"': string(); break;

		default:
			if (isdigit(c))
                number();
			else if (isalpha(c) || c == '_')
                identifier();
			else
				throw ScanError(line, column, fileName, "Unexpected character.");
			break;
	}
}

void Scanner::identifier()
{
	while (isalnum(peek()) || (peek() == '_')) advance();

	std::string text = source.substr(start, current - start);
	TokenType type = IDENTIFIER;
	if (keywords.find(text) != keywords.end())
		type = keywords[text];
	addToken(type);
    column += tokens.back().lexeme.size() - 1;
}

void Scanner::number()
{
	while (isdigit(peek())) advance();

	// Look for a fractional part.
	if ((peek() == '.') && isdigit(peekNext()))
	{
		// Consume the "."
		advance();
		while (isdigit(peek())) advance();
	}
	Object num(std::stod(source.substr(start, current - start + 1)));
	addToken(NUMBER, num);
    column += tokens.back().lexeme.size() - 1;
}

void Scanner::string()
{
	while ((peek() != '"') && !isAtEnd())
	{
		if (peek() == '\n')
        {
            line++;
            column = 0;
        }
		advance();
	}
	if (isAtEnd())
		throw ScanError(line, column, fileName, "Unterminated string.");
	// The closing ".
	advance();

	// Trim the surrounding quotes.
	std::string value = source.substr(start + 1, (current - 1) - (start + 1));
	addToken(STRING, Object(value));
    column += tokens.back().lexeme.size() - 1;
}