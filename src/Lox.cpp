#include "../include/Error.h"
#include "../include/Lox.h"
#include "../include/Scanner.h"
#include "../include/Token.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;

void Lox::runFile(char* path)
{
	std::ifstream fileIn(path);
    if (fileIn.fail())
    {
        std::cerr << "File could not be opened.";
        exit(66);
    }

	std::string source((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
	run(source);

	// Indicate an error in the exit code.
	if (hadError) exit(65);
	if (hadRuntimeError) exit(70);
}

void Lox::runPrompt()
{
	while (true)
	{
		std::cout << ">>> ";
		std::string line;
		getline(std::cin, line);

		if (line == "") break;

		run(line);
		hadError = false;
	}
}

void Lox::run(std::string& source)
{
	Scanner scanner(source);
	std::vector<Token> tokens = scanner.scanTokens();

	for (Token token : tokens)
		std::cout << token.toString() << '\n';
}

void Lox::error(int line, std::string message)
{
	report(line, "", message);
}

void Lox::error(Token token, std::string message)
{
	if (token.type = eof)
		report(token.line, " at end", message);
	else
		report(token.line, "at '" + token.lexeme + "'", message);
}

void Lox::runtimeError(RuntimeError& error)
{
	std::cerr << error.message << "\n[line " << error.token.line << "]" << '\n';
	hadRuntimeError = true;
}

void Lox::report(int line, std::string where, std::string message)
{
	std::cerr << "[line " << line << "] Error" << where << ": " << message << '\n';
	hadError = true;
}

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "Usage: cpplox [script]";
		exit(64);
	}
	else if (argc == 2)
		Lox::runFile(argv[1]);
	else
		Lox::runPrompt();
}