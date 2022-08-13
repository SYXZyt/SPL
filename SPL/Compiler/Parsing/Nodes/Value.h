#pragma once
#include "Node.h"
#include "../../../../String.h"
#include "../../Tokenisation/Token.h"
#include "../../Tokenisation/TokenType.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				enum class ValueType
				{
					IDENTIFIER,
					STRING,
					INT,
					FLOAT,
				};

				static ValueType TokenTypeToValueType(Tokenisation::TokenType type)
				{
					using SPL::Compiler::Tokenisation::TokenType;

					switch (type)
					{
						case TokenType::STRING:
							return ValueType::STRING;
						case TokenType::FLOAT:
							return ValueType::FLOAT;
						case TokenType::INT:
							return ValueType::INT;
						default:
							return ValueType::IDENTIFIER;
					}
				}

				static std::ostream& operator<<(std::ostream& os, ValueType type)
				{
					switch (type)
					{
						case ValueType::FLOAT:
							os << "FLOAT";
							break;
						case ValueType::IDENTIFIER:
							os << "IDEN";
							break;
						case ValueType::INT:
							os << "INTEGER";
							break;
						case ValueType::STRING:
							os << "STRING";
							break;
					}

					return os;
				}

				class Value : public Node
				{
				private:
					ValueType type;

				public:
					inline ValueType Type()
					{
						return type;
					}

					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[VALUE] - " << Size() << " " << type;
						ss << " RAW: ";
						switch (type)
						{
							case ValueType::FLOAT:
								ss << token.GetValueFloat();
								break;
							case ValueType::INT:
								ss << token.GetValueInt();
								break;
							case ValueType::STRING:
							case ValueType::IDENTIFIER:
								ss << Escape(token.GetValueString());
								break;
						}

						return ss.str();
					}

					Value();
					Value(const Value& v)
					{
						type = v.type;
						token = v.token;
					}
					Value(Tokenisation::Token token, ValueType type) : Node(token)
					{
						this->type = type;
					}
				};
			}
		}
	}
}