#pragma once
#include <string>
#include <sstream>
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

			std::string ToString()
			{
				//Overriding << doesn't work for some reason, so create a method instead
				std::stringstream ss;
				if (GetType() == VariableType::INT) ss << GetInt() << " (INT)";
				else if (GetType() == VariableType::FLOAT) ss << GetFloat() << " (FLOAT)";
				else ss << GetString() << " (STRING)";
				return ss.str();
			}

			VariableData();
			VariableData(const VariableData& v);
			VariableData(int i);
			VariableData(float f);
			VariableData(std::string s);
			~VariableData();
		};
	}
}