#include "Processor.h"

#pragma region Defines
#define KILL terminate = true

#define LOCKSTRINGFROMCALC(calcName) \
std::string params[] {std::string(calcName)}; \
ErrorNoExit(SPL_STRING_UNEXPECTED, GetMessageWithParams(ErrorMessages[SPL_STRING_UNEXPECTED], 1, params)); \
KILL; \
break;

#define PREPARE_TYPE_FOR_ACCU(calcName) \
SPL::VirtualMachine::VariableData* b = stack.Pop(); \
SPL::VirtualMachine::VariableData* a = stack.Pop(); \
if (b->GetType() == SPL::VirtualMachine::VariableType::STRING || a->GetType() == SPL::VirtualMachine::VariableType::STRING) {LOCKSTRINGFROMCALC(calcName)} \
stack.Push(a); /*Push in reverse order that we popped, as we want b to be on the bottom*/\
stack.Push(b)

#pragma endregion

void SPL::VirtualMachine::Processor::LoadIdentifiers()
{
	const int iCount = ReadInt();

	if (iCount == 0)
	{
		ptr = 0;
		_rom = TrimRom(4, _rom);
		return;
	}

	for (int i = 0; i < iCount; i++)
	{
		std::string name = ReadString();
		identifiers.push_back(name);
	}

	_rom = TrimRom(ptr, _rom);
	ptr = 0;
}

void SPL::VirtualMachine::Processor::MoveCursor()
{
	//Check we have two values
	if (stack.Size() < 2)
	{
		ErrorNoExit(SPL_SETPOS_EXPECTED_INT, ErrorMessages[SPL_SETPOS_EXPECTED_INT]);
		KILL;
		return;
	}

	VariableData* y = stack.Pop();
	VariableData* x = stack.Pop();

	if (x->GetType() != VariableType::INT || y->GetType() != VariableType::INT)
	{
		ErrorNoExit(SPL_SETPOS_EXPECTED_INT, ErrorMessages[SPL_SETPOS_EXPECTED_INT]);
		KILL;
		return;
	}

	int _x = x->GetInt();
	int _y = y->GetInt();

	COORD pos = {_x, _y};
	SetConsoleCursorPosition(ch, pos);

	delete x;
	delete y;
}

void SPL::VirtualMachine::Processor::LoadConstants()
{
	//Step one, read how many constants there are
	const int cCount = ReadInt();

	//If there are no constants, we can trim the start int
	if (cCount == 0)
	{
		ptr = 0;
		_rom = TrimRom(4, _rom);
		return;
	}

	for (int i = 0; i < cCount; i++)
	{
		//Read the name
		std::string constName = ReadString();

		//Now we need to read a byte, which determines the data type
		switch (_rom[ptr++])
		{
			case 0x00:
				vstack[constName] = new VariableData(ReadInt());
				break;
			case 0x01:
				vstack[constName] = new VariableData(ReadFloat());
				break;
			default:
				vstack[constName] = new VariableData(ReadString());
			break;
		}
	}

	_rom = TrimRom(ptr, _rom);
	ptr = 0;
}

std::string SPL::VirtualMachine::Processor::ReadString()
{
	std::string s = "";
	while (_rom[ptr])
	{
		s += _rom[ptr];
		Advance();
	} ptr++;
	return s;
}

int SPL::VirtualMachine::Processor::ReadInt()
{
	byte by[4]{};
	for (int i = 0; i < 4; i++)
	{
		by[i] = _rom[i + ptr];
	} ptr += 4;
	int i = BytesToInt(by);
	return i;
}

float SPL::VirtualMachine::Processor::ReadFloat()
{
	byte by[4]{};
	for (int i = 0; i < 4; i++)
	{
		by[i] = _rom[i + ptr];
	} ptr += 4;
	float f = BytesToFloat(by);
	return f;
}

void SPL::VirtualMachine::Processor::Advance()
{
	ptr++;
}

void SPL::VirtualMachine::Processor::Run()
{
	while (true)
	{
		if (ptr >= _rom.size) KILL;

		byte opcode = _rom[ptr];

		if (cstack.Size() >= callstackMaxSize)
		{
			ErrorNoExit(SPL_STACKOVERFLOW, ErrorMessages[SPL_STACKOVERFLOW]);
			KILL;
		}

		Advance();
		if (terminate) break;

		switch (opcode)
		{
			case 0x00: //nop
				break;
			case 0x01: //setpop
			{
				if (stack.Size() == 0)
				{
					ErrorNoExit(SPL_POP_EMPTY, ErrorMessages[SPL_POP_EMPTY]);
					KILL;
					break;
				}

				std::string varName = identifiers[ReadInt()];
				VariableData* v = stack.Pop();
				VariableData* data = new VariableData(*v);
				delete v;

				if (vstack.count(varName))
				{
					delete vstack[varName];
				}

				vstack[varName] = data;
			}
			break;
			case 0x02: //let string
			{
				std::string varName = identifiers[ReadInt()];
				VariableData* v = new VariableData(ReadString());

				if (vstack.count(varName))
				{
					delete vstack[varName];
				}

				vstack[varName] = v;
			}
			break;
			case 0x03: //let float
			{
				std::string varName = identifiers[ReadInt()];
				VariableData* v = new VariableData(ReadFloat());

				if (vstack.count(varName))
				{
					delete vstack[varName];
				}

				vstack[varName] = v;
			}
			break;
			case 0x04: //let int
			{
				std::string varName = identifiers[ReadInt()];
				VariableData* v = new VariableData(ReadInt());

				if (vstack.count(varName))
				{
					delete vstack[varName];
				}

				vstack[varName] = v;
			}
			break;
			case 0x05: //copy
			{
				std::string copyName = identifiers[ReadInt()];
				std::string origName = identifiers[ReadInt()];

				if (vstack.count(copyName))
				{
					delete vstack[copyName];
				}

				vstack[copyName] = new VariableData(*vstack[origName]);
			}
			break;
			case 0x06: //print string
			{
				std::cout << ReadString();
			}
			break;
			case 0x07: //print float
			{
				std::cout << ReadFloat();
			}
			break;
			case 0x08: //print int
			{
				std::cout << ReadInt();
			}
			break;
			case 0x09: //print var
			{
				std::string name = identifiers[ReadInt()];
				VariableData* v = vstack[name];

				if (v->GetType() == VariableType::STRING) std::cout << v->GetString();
				else if (v->GetType() == VariableType::FLOAT) std::cout << v->GetFloat();
				else std::cout << v->GetInt();
			}
			break;
			case 0x0a: //free
			{
				std::string name = identifiers[ReadInt()];
				delete vstack[name];
				vstack[name] = nullptr;
				vstack.erase(name);
			}
			break;
			case 0x0b: //exit
			{
				int code = ReadInt();
				KILL;
				this->code = code;
			}
			break;
			case 0x0c: //goto
			{
				ptr = ReadInt();
			}
			break;
			case 0x0d: //call
			{
				int temp = ReadInt();
				cstack.Push(ptr);
				ptr = temp;
			}
			break;
			case 0x0e: //ret
			{
				//We need to check that we aren't returning when the callstack is empty. The analyser should've caught it, but we want to make sure
				if (cstack.Size() == 0)
				{
					ErrorNoExit(SPL_RET_EMPTY, ErrorMessages[SPL_RET_EMPTY]);
					KILL;
					break;
				}

				ptr = cstack.Pop();
			}
			break;
			case 0x0f: //push string
			{
				std::string value = ReadString();
				stack.Push(new VariableData(value));
			}
			break;
			case 0x10: //push float
			{
				float value = ReadFloat();
				stack.Push(new VariableData(value));
			}
			break;
			case 0x11: //push int
			{
				int value = ReadInt();
				stack.Push(new VariableData(value));
			}
			break;
			case 0x12: //push var
			{
				std::string name = identifiers[ReadInt()];
				stack.Push(new VariableData(*vstack[name]));
			}
			break;
			case 0x13: //pop
			{
				if (stack.Size() == 0)
				{
					ErrorNoExit(SPL_POP_EMPTY, ErrorMessages[SPL_POP_EMPTY]);
					KILL;
					break;
				}

				VariableData* v = stack.Pop();
				delete v;
				v = nullptr;
			}
			break;
			case 0x14: //add
			{
				PREPARE_TYPE_FOR_ACCU("add");
				accumulator.CalculateAddition();
			}
			break;
			case 0x15: //sub
			{
				PREPARE_TYPE_FOR_ACCU("sub");
				accumulator.CalculateSubtraction();
			}
			break;
			case 0x16: //mul
			{
				PREPARE_TYPE_FOR_ACCU("mul");
				accumulator.CalculateMultiplication();
			}
			break;
			case 0x17: //div
			{
				PREPARE_TYPE_FOR_ACCU("div");
				if (!accumulator.CalculateDivision())
				{
					ErrorNoExit(SPL_DIV_ZERO, ErrorMessages[SPL_DIV_ZERO]);
					KILL;
				}
			}
			break;
			case 0x18: //pow
			{
				PREPARE_TYPE_FOR_ACCU("pow");
				accumulator.CalculatePower();
			}
			break;
			case 0x19: //concat
			{
				if (stack.Size() < 2)
				{
					std::string params[]{ "concat" };
					ErrorNoExit(SPL_NOT_ENOUGH_ITEMS, GetMessageWithParams(ErrorMessages[SPL_NOT_ENOUGH_ITEMS], 1, params));
					KILL;
					break;
				}

				accumulator.CalculateConcatenation();
			}
			break;
			case 0x1a: //to_string
			{
				if (stack.Size() == 0)
				{
					std::string params[]{ "to_string" };
					ErrorNoExit(SPL_NOT_ENOUGH_ITEMS, GetMessageWithParams(ErrorMessages[SPL_NOT_ENOUGH_ITEMS], 1, params));
					KILL;
					break;
				}

				accumulator.CastToString();
			}
			break;
			case 0x1b: //to_float
			{
				if (stack.Size() == 0)
				{
					std::string params[]{ "to_float" };
					ErrorNoExit(SPL_NOT_ENOUGH_ITEMS, GetMessageWithParams(ErrorMessages[SPL_NOT_ENOUGH_ITEMS], 1, params));
					KILL;
					break;
				}

				if (!accumulator.CastToFloat())
				{
					std::string params[]{"float"};
					ErrorNoExit(SPL_CONVERSION_ERROR, GetMessageWithParams(ErrorMessages[SPL_CONVERSION_ERROR], 1, params));
					KILL;
				}
			}
			break;
			case 0x1c: //to_int
			{
				if (stack.Size() == 0)
				{
					std::string params[]{ "to_int" };
					ErrorNoExit(SPL_NOT_ENOUGH_ITEMS, GetMessageWithParams(ErrorMessages[SPL_NOT_ENOUGH_ITEMS], 1, params));
					KILL;
					break;
				}

				if (!accumulator.CastToInt())
				{
					std::string params[]{ "int" };
					ErrorNoExit(SPL_CONVERSION_ERROR, GetMessageWithParams(ErrorMessages[SPL_CONVERSION_ERROR], 1, params));
					KILL;
				}
			}
			break;
			case 0x1d:
			{
				//Read the int offset to jump to if the comparison fails
				int failOffset = ReadInt();

				if (!accumulator.EqualComparison())
				{
					ptr = failOffset;
				}
			}
			break;
			case 0x1e:
			{
				//Read the int offset to jump to if the comparison fails
				int failOffset = ReadInt();

				if (!accumulator.NotEqualComparison())
				{
					ptr = failOffset;
				}
			}
			break;
			case 0x1f:
			{
				//Read the int offset to jump to if the comparison fails
				int failOffset = ReadInt();

				if (!accumulator.GreaterComparison())
				{
					ptr = failOffset;
				}
			}
			break;
			case 0x20:
			{
				//Read the int offset to jump to if the comparison fails
				int failOffset = ReadInt();

				if (!accumulator.GreaterEqualComparison())
				{
					ptr = failOffset;
				}
			}
			break;
			case 0x21:
			{
				//Read the int offset to jump to if the comparison fails
				int failOffset = ReadInt();

				if (!accumulator.LessComparison())
				{
					ptr = failOffset;
				}
			}
			break;
			case 0x22:
			{
				//Read the int offset to jump to if the comparison fails
				int failOffset = ReadInt();

				if (!accumulator.LessEqualComparison())
				{
					ptr = failOffset;
				}
			}
			break;
			case 0x23:
			{
				if (stack.Size() == 0)
				{
					std::string params[]{ "inc" };
					ErrorNoExit(SPL_NOT_ENOUGH_ITEMS, GetMessageWithParams(ErrorMessages[SPL_NOT_ENOUGH_ITEMS], 1, params));
					KILL;
					break;
				}

				VariableData* t = stack.Pop();

				if (t->GetType() == VariableType::STRING)
				{
					LOCKSTRINGFROMCALC("inc");
				}
				stack.Push(t);

				accumulator.Increment();
			}
			break;
			case 0x24:
			{
				if (stack.Size() == 0)
				{
					std::string params[]{"dec"};
					ErrorNoExit(SPL_NOT_ENOUGH_ITEMS, GetMessageWithParams(ErrorMessages[SPL_NOT_ENOUGH_ITEMS], 1, params));
					KILL;
					break;
				}

				VariableData* t = stack.Pop();

				if (t->GetType() == VariableType::STRING)
				{
					LOCKSTRINGFROMCALC("dec");
				}
				stack.Push(t);

				accumulator.Decrement();
			}
			break;
			case 0x25:
			{
				std::string input;
				std::getline(std::cin, input);
				stack.Push(new VariableData(input));
			}
			break;
			case 0x26:
			{
				PREPARE_TYPE_FOR_ACCU("mod");
				accumulator.Modulo();
			}
			break;
			case 0x27:
				MoveCursor();
				break;
			default:
			{
				std::string params[1]{};
				std::stringstream ss;
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)opcode;
				params[0] = ss.str().c_str();
				ErrorNoExit(SPL_UNKNOWN_OPCODE, GetMessageWithParams(ErrorMessages[SPL_UNKNOWN_OPCODE], 1, params));
				KILL;
			}
			break;
		}
	}
}

const int SPL::VirtualMachine::Processor::GetExitCode() const
{
	return code;
}

SPL::VirtualMachine::Processor::Processor(rom _rom)
{
	this->_rom = _rom;
	terminate = false;
	ptr = -1;
	Advance();
	code = SPL_EXIT_SUCCESS;
	accumulator = Accumulator(stack);
	LoadConstants();
	LoadIdentifiers();
}

SPL::VirtualMachine::Processor::~Processor()
{
	_rom.Clear();
	while (stack.Size() > 0)
	{
		VariableData* v = stack.Pop();
		delete v;
		v = nullptr;
	}

	for (std::pair<const std::string, VariableData*> var : vstack)
	{
		VariableData* v = var.second;
		delete v;
		v = nullptr;
	}
}