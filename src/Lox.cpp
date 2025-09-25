#include "../include/Lox.h"
// #include "../include/Cleaner.h"
#include "../include/Error.h"
#include "../include/Interpreter.h"
#include "../include/Nodes.h"
#include "../include/Parser.h"
#include "../include/Resolver.h"
#include "../include/Scanner.h"
#include "../include/Token.h"
#include <cctype>
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
	run(source, path);

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

        prepString(line);
		run(line);
		hadError = false;
	}
}

void Lox::run(std::string& source, std::string fileName)
{	
	Scanner scanner(source, fileName);
	std::vector<Token> tokens = scanner.scanTokens();

    if (hadError) return;

    Parser parser(tokens);
    vpS statements = parser.parse();

	if (hadError) return;

    Resolver resolver(interpreter);
    resolver.resolve(statements);

    if (hadError) return;

    interpreter.interpret(statements);
}

void Lox::strip(std::string& string, char c)
{
    if (c != '\0')
    {
        while (string.back() == c)
            string = string.substr(0, string.size() - 1);
    }
    else
    {
        while (isspace(string.back()))
            string = string.substr(0, string.size() - 1);
    }
}

void Lox::prepString(std::string& string)
{
    // 1. Remove trailing whitespace.
    // 2. Replace tabs with four spaces.
    // 3. Remove any possible trailing slashes (\).

    // 1,3.
    while (isspace(string.back()) || (string.back() == '\\'))
        string = string.substr(0, string.size() - 1);

    // 2.
    size_t start = string.find('\t', 0);
    while (start != std::string::npos)
    {
        string.replace(start, 1, "    ");
        start = string.find('\t', start + 1);
    }
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
    int column = error.type == SCAN ? error.column : error.token.column;
    std::string message = error.message;
    std::string lexerFile = error.type == SCAN ? error.fileName : "";

    std::cerr << error.name;

    int lexemeLen = 1;
    if (lexerFile == "")
        lexemeLen = error.token.lexeme.size();
    
    std::string file;
    if (lexerFile != "") file = lexerFile;
    else if (error.token.fileName != "") file = error.token.fileName;
    else
        file = "_REPL_";
    
    std::string fileText = file == "_REPL_" ? "" : "\"" + file + "\", ";

    if (lexemeLen == 0)
        std::cerr << " error" + where + " [" + fileText + "line " << line << "]: " + message + "\n";
    else if (lexemeLen == 1)
        std::cerr << " error" + where + " [" + fileText + "line " 
        << line << ", " << column << "]: " + message + "\n";
    else
        std::cerr << " error" + where + " [" + fileText + "line " 
        << line << ", " << column << "-" << column + lexemeLen - 1 << "]: " + message + "\n";

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