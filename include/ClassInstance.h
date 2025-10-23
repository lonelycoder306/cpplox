#pragma once
#include "Object.h"
#include "Token.h"
#include <map>
#include <string>

class ClassInstance
{
	public:
		ClassInstance() = default;
		ClassInstance(Object klassObj);
		Object get(Token name);
		void set(Token name, Object value);

	private:
		// Save the metaclass as an object to avoid circularity.
		Object klassObj;
		std::map<std::string, Object> fields;
};