#pragma once
#include "Error.h"
#include "Interpreter.h"
#include "Nodes.h"
#include "Token.h"
#include <string>
#include <vector>

class Lox
{
	public:
		Lox() = default;
		static void run(std::string& source);
		static void runFile(char *path);
		static void runPrompt();
        static void error(BaseError& exception);

	private:
		static bool hadError;
		static bool hadRuntimeError;
        static Interpreter interpreter;
        static void report(BaseError& error, std::string where);
};