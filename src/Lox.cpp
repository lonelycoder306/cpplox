#include "../include/Lox.h"
#include "../include/Cleaner.h"
#include "../include/Error.h"
#include "../include/Interpreter.h"
#include "../include/Nodes.h"
#include "../include/Parser.h"
#include "../include/Resolver.h"
#include "../include/Scanner.h"
#include "../include/Token.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;
Interpreter Lox::interpreter = Interpreter();

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
        std::string tempString;

        do
        {
            if (line.size() != 0)
                std::cout << "... ";
            getline(std::cin, tempString);
			if ((tempString.size() != 0) && (tempString[tempString.size() - 1] == '\\'))
			{
				// Replace the \ with a space.
				// Not only removing the \ since that will combine separate lines.
				// This would cause problems if we don't indent (add a \t separator)
				// between consequent lines.
				line += tempString.substr(0, tempString.size() - 1) + '\n';
			}
            else
                line += tempString;
        } while ((tempString.size() != 0) && (tempString[tempString.size() - 1] == '\\'));
		if (line == "") break;

		run(line);
		hadError = false;
	}
}

void Lox::run(std::string& source)
{	
	Scanner scanner(source);
	std::vector<Token> tokens = scanner.scanTokens();

    Parser parser(tokens);
    vpS statements = parser.parse();

	if (hadError) return;

    Resolver resolver(interpreter);
    resolver.resolve(statements);

    if (hadError) return;

    interpreter.interpret(statements);
}

void Lox::error(BaseError& exception)
{
    if (exception.type == SCAN)
        report(exception, "");
    else if (exception.token.type == eof)
        report(exception, " at end");
    else
        report(exception, " at '" + exception.token.lexeme + "'");
}

void Lox::report(BaseError& error, std::string where)
{
    int line = error.type == SCAN ? error.line : error.token.line;
    std::string message = error.message;

    std::cerr << error.name + " error"  +  where << " [line " << line << "]: " <<
        message << '\n';

	if (error.type == RUNTIME)
		hadRuntimeError = true;
	else
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