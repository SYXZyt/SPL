#include "Assembler.h"

//C++ doesn't have a simple add range function like C#, so I just recreated it here
template <typename T>
static void AddRange(std::vector<T>& a, std::vector<T> b)
{
	a.insert(a.end(), b.begin(), b.end());
}

static std::vector<unsigned char> AssembleValue(Value* v)
{
	std::vector<unsigned char> assembled;
	switch (v->Type())
	{
		case ValueType::IDENTIFIER:
		case ValueType::STRING:
		{
			std::vector<unsigned char> str = GetAscii(v->Token().GetValueString());
			AddRange(assembled, str);
		}
		break;
		case ValueType::INT:
		{
			std::vector<unsigned char> bytes = IntToBytes(v->Token().GetValueInt());
			AddRange(assembled, bytes);
		}
		break;
		case ValueType::FLOAT:
		{
			std::vector<unsigned char> bytes = FloatToBytes(v->Token().GetValueFloat());
			AddRange(assembled, bytes);
		}
		break;
	}

	return assembled;
}

static void VerifyVariables(SPL::Compiler::Assembler::FinalNodes& nodes)
{
	//Run a quick pass and store any constants
	std::list<std::string> consts;
	for (Constant* c : nodes.constants)
	{
		if (std::count(consts.begin(), consts.end(), c->GetName().GetLexeme()))
		{
			Error(SPL_CONST_OVERWRITE, c->Token(), ErrorMessages[SPL_CONST_OVERWRITE], "Assembler.cpp");
		} consts.push_back(c->GetName().GetLexeme());
	}

	//Now run through normal nodes and check if and mutable variables share a name with a const
	for (Node* n : nodes.nodes)
	{
		if (Let* l = dynamic_cast<Let*>(n))
		{
			if (std::count(consts.begin(), consts.end(), l->Name().GetLexeme()))
			{
				Error(SPL_CONST_OVERWRITE, l->Token(), ErrorMessages[SPL_CONST_OVERWRITE], "Assembler.cpp");
			}
		}
	}
}

template <typename T>
static int GetIndex(const std::vector<T>& vec, const T& item)
{
	auto i = std::find(vec.begin(), vec.end(), item);

	//If we find the element, we can do a quick calculation to get its index
	if (i != vec.end()) return static_cast<int>(i - vec.begin());

	//If we don't find it, return -1
	return -1;
}

int SPL::Compiler::Assembler::Assembler::GetLabelOffset(int line)
{
	//Loop over every node, until we find the first node to match the line number
	//If the current node is over the line and the last wasn't, then it means this node is the closest, so we can accept the current node
	//Simply, we can check if the current node line is greater or equal than the line we are looking for
	int index;
	bool found = false;
	for (index = 0; index < nodes.nodes.size(); index++)
	{
		if (nodes[index]->Token().GetPosition().Y() >= line)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		std::string params[]{ std::to_string(line) };

		Error(SPL_NO_CODE, *nodes[static_cast<int>(nodes.nodes.size()) - 1], GetMessageWithParams(ErrorMessages[SPL_NO_CODE], 1, params), "Assembler.cpp");
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
	VerifyVariables(nodes);

	std::vector<unsigned char> assembled;

	//Step one, write the total count of constants
	std::vector<unsigned char> constSizeBytes = IntToBytes(static_cast<int>(nodes.constants.size()));
	AddRange(assembled, constSizeBytes);

	//Now write the constants
	for (Constant* c : nodes.constants)
	{
		//Write the name
		AddRange(assembled, GetAscii(c->GetName().GetValueString()));

		//Write the type
		//00 -> INT
		//01 -> FLOAT
		//02 -> STRING
		switch (c->GetValue()->Type())
		{
			case ValueType::INT:
				assembled.push_back(0x00);
				break;
			case ValueType::FLOAT:
				assembled.push_back(0x01);
				break;
			case ValueType::STRING:
				assembled.push_back(0x02);
				break;
		}

		AddRange(assembled, AssembleValue(c->GetValue()));
	}

	//Before we assemble the nodes, we need to inform the vm of how many identifiers to read
	std::vector<unsigned char> idenSizeBytes = IntToBytes(static_cast<int>(identifiers.size()));
	AddRange(assembled, idenSizeBytes);

	for (std::string iden : identifiers)
	{
		AddRange(assembled, GetAscii(iden));
	}

	for (Node* n : nodes.nodes)
	{
		//Now we need to check what type of node this is, to properly assemble it
		if (Let* let = dynamic_cast<Let*>(n))
		{
			int nameOff = GetIndex(identifiers, let->Name().GetLexeme());
			if (nameOff == -1)
			{
				std::string param[]{let->Name().GetLexeme()};
				Error(SPL_IDENTIFIER_NOT_FOUND, *n, GetMessageWithParams(ErrorMessages[SPL_IDENTIFIER_NOT_FOUND], 1, param), "Assembler.cpp");
			}

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
			AddRange(assembled, IntToBytes(nameOff));

			//Now all the previous data about the variable has been written, we can write the main data
			std::vector<unsigned char> data;
			switch (opcode)
			{
				case 0x05:
				{
					int nameOff = GetIndex(identifiers, let->GetValue()->Token().GetLexeme());
					if (nameOff == -1)
					{
						std::string param[]{ let->GetValue()->Token().GetLexeme() };
						Error(SPL_IDENTIFIER_NOT_FOUND, *n, GetMessageWithParams(ErrorMessages[SPL_IDENTIFIER_NOT_FOUND], 1, param), "Assembler.cpp");
					}
					AddRange(data, IntToBytes(nameOff));
				}
				break;
				case 0x02:
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
			assembled.push_back(0x01);

			int nameOff = GetIndex(identifiers, setPop->Name()->Token().GetLexeme());
			if (nameOff == -1)
			{
				std::string param[]{ setPop->Name()->Token().GetLexeme() };
				Error(SPL_IDENTIFIER_NOT_FOUND, *n, GetMessageWithParams(ErrorMessages[SPL_IDENTIFIER_NOT_FOUND], 1, param), "Assembler.cpp");
			}

			AddRange(assembled, IntToBytes(nameOff));
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
				case 0x09:
				{
					int nameOff = GetIndex(identifiers, print->ToPrint()->Token().GetLexeme());
					if (nameOff == -1)
					{
						std::string param[]{ print->ToPrint()->Token().GetLexeme()};
						Error(SPL_IDENTIFIER_NOT_FOUND, *n, GetMessageWithParams(ErrorMessages[SPL_IDENTIFIER_NOT_FOUND], 1, param), "Assembler.cpp");
					}
					AddRange(data, IntToBytes(nameOff));
				}
				break;
				case 0x06:
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
			int nameOff = GetIndex(identifiers, free->VariableName().GetLexeme());
			if (nameOff == -1)
			{
				std::string param[]{ free->VariableName().GetLexeme() };
				Error(SPL_IDENTIFIER_NOT_FOUND, *n, GetMessageWithParams(ErrorMessages[SPL_IDENTIFIER_NOT_FOUND], 1, param), "Assembler.cpp");
			}

			assembled.push_back(0x0a);
			AddRange(assembled, IntToBytes(nameOff));
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
				case 0x12:
				{
					int nameOff = GetIndex(identifiers, push->GetValue()->Token().GetLexeme());
					if (nameOff == -1)
					{
						std::string param[]{ push->GetValue()->Token().GetLexeme()};
						Error(SPL_IDENTIFIER_NOT_FOUND, *n, GetMessageWithParams(ErrorMessages[SPL_IDENTIFIER_NOT_FOUND], 1, param), "Assembler.cpp");
					}
					AddRange(data, IntToBytes(nameOff));
				}
				break;
				case 0x0f:
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
		else if (Equ* equ = dynamic_cast<Equ*>(n))
		{
			assembled.push_back(0x1D);

			AddRange(assembled, IntToBytes(GetLabelOffset(equ->Line()->Token().GetValueInt())));
		}
		else if (Neq* neq = dynamic_cast<Neq*>(n))
		{
			assembled.push_back(0x1E);

			AddRange(assembled, IntToBytes(GetLabelOffset(neq->Line()->Token().GetValueInt())));
		}
		else if (Grt* grt = dynamic_cast<Grt*>(n))
		{
			assembled.push_back(0x1F);

			AddRange(assembled, IntToBytes(GetLabelOffset(grt->Line()->Token().GetValueInt())));
		}
		else if (GrtEqu* grtEqu = dynamic_cast<GrtEqu*>(n))
		{
			assembled.push_back(0x20);

			AddRange(assembled, IntToBytes(GetLabelOffset(grtEqu->Line()->Token().GetValueInt())));
		}
		else if (Lwr* lwr = dynamic_cast<Lwr*>(n))
		{
			assembled.push_back(0x21);

			AddRange(assembled, IntToBytes(GetLabelOffset(lwr->Line()->Token().GetValueInt())));
		}
		else if (LwrEqu* lwrEqu = dynamic_cast<LwrEqu*>(n))
		{
			assembled.push_back(0x22);

			AddRange(assembled, IntToBytes(GetLabelOffset(lwrEqu->Line()->Token().GetValueInt())));
		}
		else if (Inc* inc = dynamic_cast<Inc*>(n))
		{
			assembled.push_back(0x23);
		}
		else if (Dec* dec = dynamic_cast<Dec*>(n))
		{
			assembled.push_back(0x24);
		}
		else if (Input* input = dynamic_cast<Input*>(n))
		{
			assembled.push_back(0x25);
		}
		else if (Mod* mod = dynamic_cast<Mod*>(n))
		{
			assembled.push_back(0x26);
		}
		else if (Console* console = dynamic_cast<Console*>(n))
		{
			const std::string subop = console->GetConsoleOp()->Token().GetLexeme();

			//Check we have a valid subop
			if (!ConsoleSubOps.count(subop))
			{
				std::string params[]{subop};
				Error(SPL_UNKNOWN_CONSOLE_OP, *n, GetMessageWithParams(ErrorMessages[SPL_UNKNOWN_CONSOLE_OP], 1, params), "Assembler.cpp");
			}

			assembled.push_back(ConsoleSubOps[subop]);
		}
		else if (SPL::Compiler::Parser::Nodes::Sleep* sleep = dynamic_cast<SPL::Compiler::Parser::Nodes::Sleep*>(n))
		{
			assembled.push_back(0x2a);
			AddRange(assembled, AssembleValue(sleep->GetDelay()));
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
	FinalNodes finalNodes{};
	std::vector<Constant*> constants;
	std::vector<Node*> _nodes;

	for (Node* n : nodes)
	{
		if (Let* l = dynamic_cast<Let*>(n))
		{
			if (!std::count(identifiers.begin(), identifiers.end(), l->Name().GetLexeme())) identifiers.push_back(l->Name().GetLexeme());
		}

		if (Constant* c = dynamic_cast<Constant*>(n))
		{
			constants.push_back(c);

			if (!std::count(identifiers.begin(), identifiers.end(), c->GetName().GetLexeme())) identifiers.push_back(c->GetName().GetLexeme());
		}
		else _nodes.push_back(n);
	}

	finalNodes.constants = constants;
	finalNodes.nodes = _nodes;
	this->nodes = finalNodes;
	this->outputfile = outputfile;
}