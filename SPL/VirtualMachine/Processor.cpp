#include "Processor.h"

#pragma region Defines
#define KILL terminate = true;

#define HANDLENULLVAR(varName) \
if (!vstack.count(varName)) \
{ \
	std::string params[] {std::string(varName)};\
	ErrorNoExit(SPL_UNKNOWN_VAR, GetMessageWithParams(ErrorMessages[SPL_UNKNOWN_VAR], 1, params)); \
	KILL; \
	break; \
}

#define LOCKSTRINGFROMCALC(calcName) \
std::string params[] {std::string(calcName)}; \
ErrorNoExit(SPL_STRING_UNEXPECTED, GetMessageWithParams(ErrorMessages[SPL_STRING_UNEXPECTED], 1, params)); \
KILL; \
break;

#define PREPARE_TYPE_FOR_ACCU(calcName) \
SPL::VirtualMachine::VariableData* b = stack.Pop(); \
SPL::VirtualMachine::VariableData* a = stack.Pop(); \
if (b->GetType() == SPL::VirtualMachine::VariableType::STRING || a->GetType() == SPL::VirtualMachine::VariableType::STRING) {LOCKSTRINGFROMCALC(calcName)} \
stack.Push(a); \
stack.Push(b);

#pragma endregion

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

				std::string varName = ReadString();
				VariableData* v = stack.Pop();
				VariableData* data = new VariableData(*v);
				vstack[varName] = data;
			}
			break;
			case 0x02: //let string
			{
				std::string varName = ReadString();
				VariableData* v = new VariableData(ReadString());
				vstack[varName] = v;
			}
			break;
			case 0x03: //let float
			{
				std::string varName = ReadString();
				VariableData* v = new VariableData(ReadFloat());
				vstack[varName] = v;
			}
			break;
			case 0x04: //let int
			{
				std::string varName = ReadString();
				VariableData* v = new VariableData(ReadInt());
				vstack[varName] = v;
			}
			break;
			case 0x05: //copy
			{
				std::string copyName = ReadString();
				std::string origName = ReadString();
				HANDLENULLVAR(origName);
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
				std::string name = ReadString();
				HANDLENULLVAR(name);
				VariableData* v = vstack[name];

				if (v->GetType() == VariableType::STRING) std::cout << v->GetString();
				else if (v->GetType() == VariableType::FLOAT) std::cout << v->GetFloat();
				else std::cout << v->GetInt();
			}
			break;
			case 0x0a: //free
			{
				std::string name = ReadString();
				HANDLENULLVAR(name);
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
				cstack.Push(ptr + 1);
				ptr = ReadInt();
			}
			break;
			case 0x0e: //ret
			{
				//We need to check that we aren't returning when the callstack is empty. The analyser should've caught it, but we want to make sure
				if (cstack.Size() == 0)
				{
					ErrorNoExit(SPL_RET_EMPTY, ErrorMessages[SPL_RET_EMPTY]);
					KILL;
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
				std::string name = ReadString();
				HANDLENULLVAR(name);
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

				stack.Pop();
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
				accumulator.CalculateConcatenation();
			}
			break;
			case 0x1a: //to_string
			{
				accumulator.CastToString();
			}
			break;
			case 0x1b: //to_float
			{
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

	for (auto var : vstack)
	{
		VariableData* v = var.second;
		delete v;
		v = nullptr;
	}
}