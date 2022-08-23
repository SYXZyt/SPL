#include "Accumulator.h"

#define SCAST_INT static_cast<int>
#define SCAST_FLT static_cast<float> 

#define COMPARISON(op) \
VariableData* lhs = stack->Pop();\
VariableData* rhs = stack->Pop();\
if (lhs->GetType() != rhs->GetType()) return false;\
if (lhs->GetType() == VariableType::INT)\
{\
	return lhs->GetInt() op rhs->GetInt();\
}\
else if (lhs->GetType() == VariableType::FLOAT)\
{\
	return lhs->GetFloat() op rhs->GetFloat();\
}\
else\
{\
	return lhs->GetString() op rhs->GetString();\
}

using namespace SPL::VirtualMachine;

static bool CanBeNumber(const std::string s)
{
	int decCount = 0;

	for (int i = 0; i < s.length(); i++)
	{
		//Check if we have a minus, that it is the first letter
		//If it isn't, it's invalid
		if (s[i] == '-' && i > 0)
		{
			return false;
		}
		else if (s[i] == '.') decCount++;
		else if (!isdigit(s[i])) return false;
	}

	if (decCount > 1) return false;

	return true;
}

SPL::VirtualMachine::Accumulator::Accumulator()
{
	stack = nullptr;
}

SPL::VirtualMachine::Accumulator::Accumulator(Stack<VariableData*>& stack)
{
	this->stack = &stack;
}

void SPL::VirtualMachine::Accumulator::CalculateAddition()
{
	//Addition like most calculations will return a different value based on the types provided.
	//The processor has already handled strings being provided, so we just need to worry about if we are returning an int or float
	VariableData* lhs = stack->Pop();
	VariableData* rhs = stack->Pop();

	if (lhs->GetType() == VariableType::FLOAT || rhs->GetType() == VariableType::FLOAT)
	{
		float result;

		if (lhs->GetType() == VariableType::FLOAT) result = lhs->GetFloat();
		else result = SCAST_FLT(lhs->GetInt());

		if (rhs->GetType() == VariableType::FLOAT) result += rhs->GetFloat();
		else result += SCAST_FLT(rhs->GetInt());

		stack->Push(new VariableData(result));
	}
	else
	{
		int result;

		if (lhs->GetType() == VariableType::FLOAT) result = SCAST_INT(lhs->GetFloat());
		else result = lhs->GetInt();

		if (rhs->GetType() == VariableType::FLOAT) result += SCAST_INT(rhs->GetFloat());
		else result += rhs->GetInt();

		stack->Push(new VariableData(result));
	}
}

void SPL::VirtualMachine::Accumulator::CalculateSubtraction()
{
	VariableData* lhs = stack->Pop();
	VariableData* rhs = stack->Pop();

	if (lhs->GetType() == VariableType::FLOAT || rhs->GetType() == VariableType::FLOAT)
	{
		float result;

		if (lhs->GetType() == VariableType::FLOAT) result = lhs->GetFloat();
		else result = SCAST_FLT(lhs->GetInt());

		if (rhs->GetType() == VariableType::FLOAT) result -= rhs->GetFloat();
		else result -= SCAST_FLT(rhs->GetInt());

		stack->Push(new VariableData(result));
	}
	else
	{
		int result;

		if (lhs->GetType() == VariableType::FLOAT) result = SCAST_INT(lhs->GetFloat());
		else result = lhs->GetInt();

		if (rhs->GetType() == VariableType::FLOAT) result -= SCAST_INT(rhs->GetFloat());
		else result -= rhs->GetInt();

		stack->Push(new VariableData(result));
	}
}

void SPL::VirtualMachine::Accumulator::CalculateMultiplication()
{
	VariableData* lhs = stack->Pop();
	VariableData* rhs = stack->Pop();

	if (lhs->GetType() == VariableType::FLOAT || rhs->GetType() == VariableType::FLOAT)
	{
		float result;

		if (lhs->GetType() == VariableType::FLOAT) result = lhs->GetFloat();
		else result = SCAST_FLT(lhs->GetInt());

		if (rhs->GetType() == VariableType::FLOAT) result *= rhs->GetFloat();
		else result *= SCAST_FLT(rhs->GetInt());

		stack->Push(new VariableData(result));
	}
	else
	{
		int result;

		if (lhs->GetType() == VariableType::FLOAT) result = SCAST_INT(lhs->GetFloat());
		else result = lhs->GetInt();

		if (rhs->GetType() == VariableType::FLOAT) result *= SCAST_INT(rhs->GetFloat());
		else result *= rhs->GetInt();

		stack->Push(new VariableData(result));
	}
}

bool SPL::VirtualMachine::Accumulator::CalculateDivision()
{
	//We also need to add an addition check here, to handle div by 0 errors
	VariableData* lhs = stack->Pop();
	VariableData* rhs = stack->Pop();

	if (lhs->GetType() == VariableType::FLOAT || rhs->GetType() == VariableType::FLOAT)
	{
		float result;

		if (lhs->GetType() == VariableType::FLOAT) result = lhs->GetFloat();
		else result = SCAST_FLT(lhs->GetInt());

		if (rhs->GetType() == VariableType::FLOAT)
		{
			if (lhs->GetFloat() == 0.0) return false;
			result /= rhs->GetFloat();
		}
		else
		{
			if (rhs->GetInt() == 0) return false;
			result /= SCAST_FLT(rhs->GetInt());
		}

		stack->Push(new VariableData(result));
	}
	else
	{
		int result;

		if (lhs->GetType() == VariableType::FLOAT) result = SCAST_INT(lhs->GetFloat());
		else result = lhs->GetInt();

		if (rhs->GetType() == VariableType::FLOAT)
		{
			if (rhs->GetFloat() == 0.0) return false;
			result /= SCAST_INT(rhs->GetFloat());
		}
		else
		{
			if (rhs->GetInt() == 0) return false;
			result /= rhs->GetInt();
		}

		stack->Push(new VariableData(result));
	}

	return true;
}

void SPL::VirtualMachine::Accumulator::CalculatePower()
{
	VariableData* lhs = stack->Pop();
	VariableData* rhs = stack->Pop();

	if (lhs->GetType() == VariableType::FLOAT || rhs->GetType() == VariableType::FLOAT)
	{
		float base;
		float power;

		if (lhs->GetType() == VariableType::FLOAT) base = lhs->GetFloat();
		else base = SCAST_FLT(lhs->GetInt());

		if (rhs->GetType() == VariableType::FLOAT) power = rhs->GetFloat();
		else power = SCAST_FLT(rhs->GetInt());

		//pow returns a double, so we need to trim that extra precision, and convert it to a float
		stack->Push(new VariableData(SCAST_FLT(pow(base, power))));
	}
	else
	{
		int base;
		int power;

		if (lhs->GetType() == VariableType::FLOAT) base = SCAST_INT(lhs->GetFloat());
		else base = lhs->GetInt();

		if (rhs->GetType() == VariableType::FLOAT) power = SCAST_INT(rhs->GetFloat());
		else power = rhs->GetInt();

		stack->Push(new VariableData(SCAST_INT(pow(base, power))));
	}
}

void SPL::VirtualMachine::Accumulator::CalculateConcatenation()
{
	VariableData* lhs = stack->Pop();
	VariableData* rhs = stack->Pop();

	std::string result;

	switch (lhs->GetType())
	{
		case VariableType::FLOAT:
			result = std::to_string(lhs->GetFloat());
			break;
		case VariableType::INT:
			result = std::to_string(lhs->GetInt());
			break;
		default:
			result = lhs->GetString();
			break;
	}

	switch (rhs->GetType())
	{
		case VariableType::FLOAT:
			result += std::to_string(rhs->GetFloat());
			break;
		case VariableType::INT:
			result += std::to_string(rhs->GetInt());
			break;
		default:
			result += rhs->GetString();
			break;
	}

	stack->Push(new VariableData(result));
}

void SPL::VirtualMachine::Accumulator::CastToString()
{
	VariableData* toCast = stack->Pop();

	std::string result;

	if (toCast->GetType() == VariableType::FLOAT)
	{
		result = std::to_string(toCast->GetFloat());
	}
	else if (toCast->GetType() == VariableType::INT)
	{
		result = std::to_string(toCast->GetInt());
	}
	else
	{
		stack->Push(toCast);
		return;
	}

	stack->Push(new VariableData(result));
}

bool SPL::VirtualMachine::Accumulator::CastToFloat()
{
	VariableData* toCast = stack->Pop();

	if (toCast->GetType() == VariableType::FLOAT)
	{
		//If the user is trying to convert a type to itself, we can just add it back onto the stack and exit
		stack->Push(toCast);
	}
	else if (toCast->GetType() == VariableType::INT)
	{
		float value = SCAST_FLT(toCast->GetInt());
		stack->Push(new VariableData(value));
	}
	else
	{
		//Strings a little more tricky, as it isn't guaranteed to be able to be converted, so we need to check if the format is valid
		if (!CanBeNumber(toCast->GetString())) return false;

		float value = std::stof(toCast->GetString());
		stack->Push(new VariableData(value));
	}

	return true;
}

bool SPL::VirtualMachine::Accumulator::CastToInt()
{
	VariableData* toCast = stack->Pop();

	if (toCast->GetType() == VariableType::FLOAT)
	{
		int value = SCAST_INT(toCast->GetFloat());
		stack->Push(new VariableData(value));
	}
	else if (toCast->GetType() == VariableType::INT)
	{
		stack->Push(toCast);
	}
	else
	{
		//Strings a little more tricky, as it isn't guaranteed to be able to be converted, so we need to check if the format is valid
		if (!CanBeNumber(toCast->GetString())) return false;

		int value = std::stoi(toCast->GetString());
		stack->Push(new VariableData(value));
	}

	return true;
}

bool SPL::VirtualMachine::Accumulator::EqualComparison()
{
	COMPARISON(==);
}

bool SPL::VirtualMachine::Accumulator::NotEqualComparison()
{
	COMPARISON(!=);
}

bool SPL::VirtualMachine::Accumulator::GreaterComparison()
{
	COMPARISON(>);
}

bool SPL::VirtualMachine::Accumulator::GreaterEqualComparison()
{
	COMPARISON(>=);
}

bool SPL::VirtualMachine::Accumulator::LessComparison()
{
	COMPARISON(<);
}

bool SPL::VirtualMachine::Accumulator::LessEqualComparison()
{
	COMPARISON(<=);
}
