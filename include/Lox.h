#pragma once
#include "Error.h"
#include "Interpreter.h"
#include "Token.h"
#include <string>

class Lox
{
	public:
		Lox() = default;
		static void run(std::string& source);
		static void runFile(char *path);
		static void runPrompt();
		static void error(int line, std::string message);
		static void error(Token token, std::string message);
		static void runtimeError(RuntimeError& error);

	private:
		static bool hadError;
		static bool hadRuntimeError;
        static Interpreter interpreter;
		static void report(int line, std::string where, std::string message);
};