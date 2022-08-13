#include "VariableData.h"

std::string SPL::VirtualMachine::VariableData::GetString() const
{
	if (type != VariableType::STRING) return "SPL_FAILED_TO_CONVERT_TO_STRING";
	return *static_cast<std::string*>(raw);
}

int SPL::VirtualMachine::VariableData::GetInt() const
{
	if (type != VariableType::INT) return 0;
	return *static_cast<int*>(raw);
}

float SPL::VirtualMachine::VariableData::GetFloat() const
{
	if (type != VariableType::FLOAT) return 0.0f;
	return *static_cast<float*>(raw);
}

SPL::VirtualMachine::VariableType SPL::VirtualMachine::VariableData::GetType() const
{
	return type;
}

SPL::VirtualMachine::VariableData::VariableData()
{
	raw = nullptr;
	type = VariableType::INT;
}

SPL::VirtualMachine::VariableData::VariableData(const VariableData& v)
{
	type = v.type;

	if (type == VariableType::FLOAT) raw = new float(*static_cast<float*>(v.raw));
	else if (type == VariableType::INT) raw = new int(*static_cast<int*>(v.raw));
	else raw = new std::string(*static_cast<std::string*>(v.raw));
}

SPL::VirtualMachine::VariableData::VariableData(int i)
{
	raw = new int(i);
	type = VariableType::INT;
}

SPL::VirtualMachine::VariableData::VariableData(float f)
{
	raw = new float(f);
	type = VariableType::FLOAT;
}

SPL::VirtualMachine::VariableData::VariableData(std::string s)
{
	raw = new std::string(s);
	type = VariableType::STRING;
}

SPL::VirtualMachine::VariableData::~VariableData()
{
	delete raw;
	raw = nullptr;
}