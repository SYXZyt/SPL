#pragma once
#include <string>
#include <ostream>

namespace SPL
{
	namespace VirtualMachine
	{
		enum class VariableType
		{
			STRING,
			INT,
			FLOAT,
		};

		class VariableData final
		{
		private:
			VariableType type;
			void* raw;
		
		public:
			std::string GetString() const;
			int GetInt() const;
			float GetFloat() const;

			VariableType GetType() const;

			VariableData();
			VariableData(const VariableData& v);
			VariableData(int i);
			VariableData(float f);
			VariableData(std::string s);
			~VariableData();
		};

		static std::ostream& operator<<(std::ostream& os, const VariableData& v)
		{
			if (v.GetType() == VariableType::INT) os << v.GetInt();
			else if (v.GetType() == VariableType::FLOAT) os << v.GetFloat();
			else os << v.GetString();
			return os;
		}
	}
}

static std::ostream& operator<<(std::ostream& os, SPL::VirtualMachine::VariableType type)
{
	using SPL::VirtualMachine::VariableType;

	switch (type)
	{
		case VariableType::STRING:
			os << "string";
			break;
		case VariableType::FLOAT:
			os << "float";
			break;
		case VariableType::INT:
			os << "int";
			break;
		default:
			os << "void";
			break;
	}

	return os;
}