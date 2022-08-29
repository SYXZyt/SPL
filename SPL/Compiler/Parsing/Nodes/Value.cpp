#include "Value.h"

int SPL::Compiler::Parser::Nodes::Value::Size()
{
	int size;

	if (type == ValueType::FLOAT)
	{
		size = sizeof(float);
	}
	else if (type == ValueType::INT)
	{
		size = sizeof(int);
	}
	else if (type == ValueType::IDENTIFIER)
	{
		size = sizeof(int);
	}
	else
	{
		if (token.GetValueString().size() == 0) return 1;

		//In this case, we need to add the null terminator onto the string, as that is included in the rom as well
		std::string s = token.GetValueString();
		Terminate(s);

		token.SetValueString(s);

		size = static_cast<int>(token.GetValueString().length());
	}

	return size;
}

SPL::Compiler::Parser::Nodes::Value::Value()
{
	type = ValueType::INT;
}