#include "Assembler.h"

//C++ doesn't have a simple add range function like C#, so I just recreated it here
template <typename T>
static void AddRange(std::vector<T>& a, std::vector<T> b)
{
	a.insert(a.end(), b.begin(), b.end());
}

int SPL::Compiler::Assembler::Assembler::GetLabelOffset(int line)
{
	//Loop over every node, until we find the first node to match the line number
	//If the current node is over the line and the last wasn't, then it means this node is the closest, so we can accept the current node
	//Simply, we can check if the current node line is greater or equal than the line we are looking for
	int index;
	bool found = false;
	for (index = 0; index < nodes.size(); index++)
	{
		if (nodes[index]->Token().GetPosition().Y() >= line)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		std::string s = "Could not jump to line ";
		s += line;
		s += " as no code was found there";

		Error(*nodes[nodes.size() - 1], s, "Assembler.cpp");
	}

	//Now we need to go over every node until the current node, and combine their size, to get the correct offset in the binary file
	int offset = 0;
	for (int i = 0; i < index; i++)
	{
		offset += nodes[i]->Size();
	}

	return offset;
}

void SPL::Compiler::Assembler::Assembler::WriteBinary(std::vector<unsigned char> rom)
{
	std::filesystem::create_directory("_spl_cache_");

	std::ofstream os;
	os.open(outputfile, std::ofstream::binary);

	for (byte b : rom)
	{
		os << b;
	}
	os.close();
}

void SPL::Compiler::Assembler::Assembler::Assemble()
{
	std::vector<unsigned char> assembled;

	for (Node* n : nodes)
	{
		//Now we need to check what type of node this is, to properly assemble it
		if (Let* let = dynamic_cast<Let*>(n))
		{
			std::vector<unsigned char> name = GetAscii(let->Name().GetLexeme());

			unsigned char opcode = 0;
			switch (let->GetValue()->Type())
			{
				case ValueType::STRING:
					opcode = 0x02;
					break;
				case ValueType::FLOAT:
					opcode = 0x03;
					break;
				case ValueType::INT:
					opcode = 0x04;
					break;
				case ValueType::IDENTIFIER:
					opcode = 0x05;
					break;
			};

			assembled.push_back(opcode);
			AddRange(assembled, name);

			//Now all the previous data about the variable has been written, we can write the main data
			std::vector<unsigned char> data;
			switch (opcode)
			{
				case 0x02:
				case 0x05:
				{
					std::string value = let->GetValue()->Token().GetValueString();
					Terminate(value);
					data = GetAscii(value);
				}
				break;
				case 0x03:
				{
					float value = let->GetValue()->Token().GetValueFloat();
					data = FloatToBytes(value);
				}
				break;
				case 0x04:
				{
					int value = let->GetValue()->Token().GetValueInt();
					data = IntToBytes(value);
				}
				break;
			}

			AddRange(assembled, data);
		}
		else if (SetPop* setPop = dynamic_cast<SetPop*>(n))
		{
			std::vector<unsigned char> name = GetAscii(setPop->Name().GetLexeme());
			assembled.push_back(0x01);
			AddRange(assembled, name);
		}
		else if (Print* print = dynamic_cast<Print*>(n))
		{
			unsigned char opcode = 0;
			switch (print->ToPrint()->Type())
			{
				case ValueType::STRING:
					opcode = 0x06;
					break;
				case ValueType::FLOAT:
					opcode = 0x07;
					break;
				case ValueType::INT:
					opcode = 0x08;
					break;
				case ValueType::IDENTIFIER:
					opcode = 0x09;
					break;
			}

			std::vector<unsigned char> data;
			switch (opcode)
			{
				case 0x06:
				case 0x09:
				{
					std::string value = print->ToPrint()->Token().GetValueString();
					Terminate(value);
					data = GetAscii(value);
				}
				break;
				case 0x07:
				{
					float value = print->ToPrint()->Token().GetValueFloat();
					data = FloatToBytes(value);
				}
				break;
				case 0x08:
				{
					int value = print->ToPrint()->Token().GetValueInt();
					data = IntToBytes(value);
				}
				break;
			}

			assembled.push_back(opcode);
			AddRange(assembled, data);
		}
		else if (Free* free = dynamic_cast<Free*>(n))
		{
			//Get the name of the variable to free
			std::string name = free->VariableName().GetValueString();
			Terminate(name);

			assembled.push_back(0x0a);
			AddRange(assembled, GetAscii(name));
		}
		else if (Exit* exit = dynamic_cast<Exit*>(n))
		{
			if (exit->GetValue()->Type() == ValueType::INT)
			{
				int code = exit->GetValue()->Token().GetValueInt();
				assembled.push_back(0x0B);
				AddRange(assembled, IntToBytes(code));
			}
			else
			{
				Error(*exit, "The provided expression cannot be used to exit", "Assembler.cpp");
			}

		}
		else if (Goto* jmp = dynamic_cast<Goto*>(n))
		{
			int offset = GetLabelOffset(jmp->Line()->Token().GetValueInt());
			assembled.push_back(0x0C);
			AddRange(assembled, IntToBytes(offset));
		}
		else if (Call* call = dynamic_cast<Call*>(n))
		{
			int offset = GetLabelOffset(call->Line()->Token().GetValueInt());
			assembled.push_back(0x0D);
			AddRange(assembled, IntToBytes(offset));
		}
		else if (dynamic_cast<Ret*>(n))
		{
			assembled.push_back(0x0e);
		}
		else if (Push* push = dynamic_cast<Push*>(n))
		{
			byte opcode = 0;
			switch (push->GetValue()->Type())
			{
				case ValueType::STRING: opcode = 0x0f;
					break;
				case ValueType::FLOAT: opcode = 0x10;
					break;
				case ValueType::INT: opcode = 0x11;
					break;
				case ValueType::IDENTIFIER: opcode = 0x12;
					break;
			}

			std::vector<unsigned char> data;
			switch (opcode)
			{
				case 0x0f:
				case 0x12:
				{
					std::string value = push->GetValue()->Token().GetValueString();
					Terminate(value);
					data = GetAscii(value);
				}
				break;
				case 0x10:
				{
					float value = push->GetValue()->Token().GetValueFloat();
					data = FloatToBytes(value);
				}
				break;
				case 0x11:
				{
					int value = push->GetValue()->Token().GetValueInt();
					data = IntToBytes(value);
				}
				break;
			}

			assembled.push_back(opcode);
			AddRange(assembled, data);
		}
		else if (dynamic_cast<Pop*>(n))
		{
			assembled.push_back(0x13);
		}
		else if (dynamic_cast<Add*>(n))
		{
			assembled.push_back(0x14);
		}
		else if (dynamic_cast<Sub*>(n))
		{
			assembled.push_back(0x15);
		}
		else if (dynamic_cast<Mul*>(n))
		{
			assembled.push_back(0x16);
		}
		else if (dynamic_cast<Div*>(n))
		{
			assembled.push_back(0x17);
		}
		else if (dynamic_cast<Pow*>(n))
		{
			assembled.push_back(0x18);
		}
		else if (dynamic_cast<Concat*>(n))
		{
			assembled.push_back(0x19);
		}
		else if (dynamic_cast<ToString*>(n))
		{
			assembled.push_back(0x1A);
		}
		else if (dynamic_cast<ToFloat*>(n))
		{
			assembled.push_back(0x1B);
		}
		else if (dynamic_cast<ToInt*>(n))
		{
			assembled.push_back(0x1C);
		}

		else
		{
			//C28182 is a false positive in this scenario, so it can just be disabled
#pragma warning( push )
#pragma warning( disable : 28182 )
			Error(*n, "Node does not have an assemble function available. This is a dev error and cannot be resolved by changing SPL input", "Assembler.cpp");
#pragma warning( pop )
		}

	} //End of loop

	WriteBinary(assembled);
}

SPL::Compiler::Assembler::Assembler::Assembler(std::vector<Node*> nodes, const char* outputfile)
{
	this->nodes = nodes;
	this->outputfile = outputfile;
}