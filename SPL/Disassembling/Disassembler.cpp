#include "Disassembler.h"

#define EMPTY std::vector<byte>()

#define INIT(base) \
std::string spl = base;\
std::vector<byte> bytes

#define SINGLEOP(op) \
result.assembled = EMPTY;\
result.disassembled = op

#define SETRESULT \
result.assembled = bytes; \
result.disassembled = spl

#define MOVEBYTES(dst, src) \
dst[0] = src[0]; \
dst[1] = src[1]; \
dst[2] = src[2]; \
dst[3] = src[3]

static std::string ReadString(const byte* rom, std::vector<byte>& bytes, int& offset)
{
	std::string str = "";
	while (rom[offset])
	{
		bytes.push_back(rom[offset]);
		str += rom[offset++];
	} offset++;

	bytes.push_back(0x00);
	return str;
}

static std::string ReadString(const byte* rom, int& offset)
{
	std::vector<byte> _;
	return ReadString(rom, _, offset);
}

static int ReadInt(const byte* rom, std::vector<byte>& bytes, int& offset)
{
	std::vector<byte> intBytes;
	for (int i = 0; i < 4; i++)
	{
		intBytes.push_back(rom[offset + i]);
		bytes.push_back(rom[offset + i]);
	} offset += 4;
	byte by[4]{};
	MOVEBYTES(by, intBytes);
	return BytesToInt(by);
}

static int ReadInt(const byte* rom, int& offset)
{
	std::vector<byte> _;
	return ReadInt(rom, _, offset);
}

static float ReadFloat(const byte* rom, std::vector<byte>& bytes, int& offset)
{
	std::vector<byte> fltBytes;
	for (int i = 0; i < 4; i++)
	{
		fltBytes.push_back(rom[offset + i]);
		bytes.push_back(rom[offset + i]);
	} offset += 4;
	byte by[4]{};
	MOVEBYTES(by, fltBytes);
	return BytesToFloat(by);
}

static float ReadFloat(const byte* rom, int& offset)
{
	std::vector<byte> _;
	return ReadFloat(rom, _, offset);
}

static byte ReadByte(const byte* rom, std::vector<byte>& bytes, int& offset)
{
	bytes.push_back(rom[offset]);
	return rom[offset++];
}

static byte ReadByte(const byte* rom, int& offset)
{
	std::vector<byte> _;
	return ReadByte(rom, _, offset);
}

static void ReadIdentifiers(const byte* rom, int& addr)
{
	int iCount = ReadInt(rom, addr);
	std::cout << "===|Identifiers " << iCount << "|===" << std::endl;
	for (int i = 0; i < iCount; i++)
	{
		std::cout << i << " ~ " << ReadString(rom, addr) << std::endl;
	}
}

static void ReadConstants(const byte* rom, int& addr)
{
	//Print constant values
	int cCount = ReadInt(rom, addr);
	std::cout << "===|CONSTS " << cCount << "|===" << std::endl;
	for (int i = 0; i < cCount; i++)
	{
		std::string name = ReadString(rom, addr);

		//Read a byte which determines what type to read
		byte type = ReadByte(rom, addr);

		std::cout << name << " ~ ";
		switch (type)
		{
			case 0x00:
			{
				int _int = ReadInt(rom, addr);
				std::cout << _int << std::endl;
			}
			break;
			case 0x01:
			{
				float flt = ReadFloat(rom, addr);
				std::cout << flt << std::endl;
			}
			break;
			default:
			{
				std::string str = ReadString(rom, addr);
				std::cout << str << std::endl;
			}
		}
	}
}

static void DumpDisassembly(std::vector<SPL::Disassembling::Disassembled> results)
{
	using SPL::Disassembling::Disassembled;

	//Out of all lines of bytes, get the longest
	int longestLine = 0;
	int longestSPL = 0; //This is so the border goes to the edge

	for (Disassembled d : results)
	{
		//Since each opcode is two chars and a space, we need to multiply the length by three
		int length = static_cast<int>(d.assembled.size()) * 3;
		if (longestLine < length) longestLine = length;
		if (longestSPL < d.disassembled.size()) longestSPL = static_cast<int>(d.disassembled.size());
	}

	std::cout << "\n===|Disassembly|===" << std::endl;
	int offset = sizeof("ADDR|OP||ASM");

	std::cout << "ADDR|OP||ASM|" << std::string(longestLine-4, ' ') << "SPL" << std::endl;
	std::cout << std::string(longestLine + longestSPL + offset, '-') << std::endl;

	//Now we can print the disassembly
	for (Disassembled d : results)
	{
		std::stringstream ss;
		ss << std::hex << std::setw(4) << std::setfill('0');
		ss << d.addr << ' ' << std::setw(2) << std::setfill('0');
		ss << (unsigned)d.opcode << "  ";
		for (byte b : d.assembled) ss << std::setw(2) << std::setfill('0') << (unsigned)b << ' ';

		int len = longestLine - static_cast<int>(d.assembled.size()) * 3;
		ss << std::string(len, ' ');
		ss << d.disassembled;

		std::cout << ss.str() << std::endl;
	}
}

void SPL::Disassembling::Disassembler::Disassemble()
{
	std::vector<Disassembled> results;
	int addr = 0;

	//Put rom in another scope as we don't need it after reading constants and such
	{
		byte* rom = &_rom.bytes[0];
		ReadConstants(rom, addr);
		ReadIdentifiers(rom, addr);
	}

	//Remove constant and identifier data so we can display correct addresses
	_rom = TrimRom(addr, _rom);

	//We manually iterate addr, so we can skip `addr++` in this for loop
	for (addr = 0; addr < _rom.size;)
	{
		int bytesRead = 0;
		Disassembled result = DisassembleInstruction(_rom, addr, bytesRead);
		addr += bytesRead;
		results.push_back(result);
	}

	DumpDisassembly(results);
}

SPL::Disassembling::Disassembled SPL::Disassembling::Disassembler::DisassembleInstruction(rom _rom, const int& startAddr, int& bytesRead)
{
	int addr = startAddr;
	byte* rom = &_rom.bytes[0];

	Disassembled result{};
	result.addr = addr;

	byte opcode = ReadByte(rom, addr);
	result.opcode = opcode;

	switch (opcode)
	{
		case 0x00:	//NULL
			SINGLEOP("nop");
			break;
		case 0x01:	//SETPOP
		{
			INIT("setpop [");

			spl += std::to_string(ReadInt(rom, bytes, addr)) + ']';
			SETRESULT;
		}
		break;
		case 0x02: //LET STRING
		{
			INIT("str [");
			spl += std::to_string(ReadInt(rom, bytes, addr));
			spl += "] \"";
			spl += Escape(ReadString(rom, bytes, addr));
			spl += '"';

			SETRESULT;
		}
		break;
		case 0x03: //LET FLOAT
		{
			INIT("flt [");
			spl += std::to_string(ReadInt(rom, bytes, addr));
			spl += "] " + std::to_string(ReadFloat(rom, bytes, addr));
			SETRESULT;
		}
		break;
		case 0x04: //LET INT
		{
			INIT("int [");
			spl += std::to_string(ReadInt(rom, bytes, addr));
			spl += "] " + std::to_string(ReadInt(rom, bytes, addr));
			SETRESULT;
		}
		break;
		case 0x05: //LET VAR
		{
			INIT("cpy [");
			spl += std::to_string(ReadInt(rom, bytes, addr));
			spl += "] [";
			spl += std::to_string(ReadInt(rom, bytes, addr)) + ']';
			SETRESULT;
		}
		break;
		case 0x06: //PRINT STRING
		{
			INIT("strout \"");
			spl += Escape(ReadString(rom, bytes, addr) + '"');
			SETRESULT;
		}
		break;
		case 0x07: //PRINT FLOAT
		{
			INIT("fltout ");
			spl += ReadString(rom, bytes, addr);
			SETRESULT;
		}
		break;
		case 0x08: //PRINT INT
		{
			INIT("intout");
			spl += std::to_string(ReadInt(rom, bytes, addr));
			SETRESULT;
		}
		break;
		case 0x09:
		{
			INIT("varout [");
			spl += std::to_string(ReadInt(rom, bytes, addr)) + ']';
			SETRESULT;
		}
		break;
		case 0x0a:
		{
			INIT("free [");
			spl += std::to_string(ReadInt(rom, bytes, addr)) + ']';
			SETRESULT;
		}
		break;
		case 0x0b:
		{
			INIT("exit ");
			spl += std::to_string(ReadInt(rom, bytes, addr));
			SETRESULT;
		}
		break;
		case 0x0c:
		{
			INIT("goto ");
			int offset = ReadInt(rom, bytes, addr);
			spl += std::to_string(offset) + " (0x" + GetHex(offset) + ')';
			SETRESULT;
		}
		break;
		case 0x0d:
		{
			INIT("call ");
			int offset = ReadInt(rom, bytes, addr);
			spl += std::to_string(offset) + " (0x" + GetHex(offset) + ')';
			SETRESULT;
		}
		break;
		case 0x0e:
		{
			SINGLEOP("ret");
		}
		break;
		case 0x0f:
		{
			INIT("strpush \"");
			spl += Escape(ReadString(rom, bytes, addr) + '"');
			SETRESULT;
		}
		break;
		case 0x10:
		{
			INIT("fltpush ");
			spl += std::to_string(ReadFloat(rom, bytes, addr));
			SETRESULT;
		}
		break;
		case 0x11:
		{
			INIT("intpush ");
			spl += std::to_string(ReadInt(rom, bytes, addr));
			SETRESULT;
		}
		break;
		case 0x12:
		{
			INIT("varpush [");
			spl += std::to_string(ReadInt(rom, bytes, addr)) + ']';
			SETRESULT;
		}
		break;
		case 0x13:
		{
			SINGLEOP("pop");
		}
		break;
		case 0x14:
		{
			SINGLEOP("add");
		}
		break;
		case 0x15:
		{
			SINGLEOP("sub");
		}
		break;
		case 0x16:
		{
			SINGLEOP("mul");
		}
		break;
		case 0x17:
		{
			SINGLEOP("div");
		}
		break;
		case 0x18:
		{
			SINGLEOP("pow");
		}
		break;
		case 0x19:
		{
			SINGLEOP("concat");
		}
		break;
		case 0x1a:
		{
			SINGLEOP("strcast");
		}
		break;
		case 0x1b:
		{
			SINGLEOP("fltcast");
		}
		break;
		case 0x1c:
		{
			SINGLEOP("intcast");
		}
		break;
		case 0x1d:
		{
			INIT("equ ");
			int offset = ReadInt(rom, bytes, addr);
			spl += std::to_string(offset) + " (0x" + GetHex(offset) + ')';
			SETRESULT;
		}
		break;
		case 0x1e:
		{
			INIT("neq ");
			int offset = ReadInt(rom, bytes, addr);
			spl += std::to_string(offset) + " (0x" + GetHex(offset) + ')';
			SETRESULT;
		}
		break;
		case 0x1f:
		{
			INIT("grt ");
			int offset = ReadInt(rom, bytes, addr);
			spl += std::to_string(offset) + " (0x" + GetHex(offset) + ')';
			SETRESULT;
		}
		break;
		case 0x20:
		{
			INIT("grtequ ");
			int offset = ReadInt(rom, bytes, addr);
			spl += std::to_string(offset) + " (0x" + GetHex(offset) + ')';
			SETRESULT;
		}
		break;
		case 0x21:
		{
			INIT("lwr ");
			int offset = ReadInt(rom, bytes, addr);
			spl += std::to_string(offset) + " (0x" + GetHex(offset) + ')';
			SETRESULT;
		}
		break;
		case 0x22:
		{
			INIT("lwrequ ");
			int offset = ReadInt(rom, bytes, addr);
			spl += std::to_string(offset) + " (0x" + GetHex(offset) + ')';
			SETRESULT;
		}
		break;
		case 0x23:
		{
			SINGLEOP("inc");
		}
		break;
		case 0x24:
		{
			SINGLEOP("dec");
		}
		break;
		case 0x25:
		{
			SINGLEOP("input");
		}
		break;
		case 0x26:
		{
			SINGLEOP("mod");
		}
		break;
		case 0x27:
		{
			SINGLEOP("console setpos");
		}
		break;
		case 0x28:
		{
			SINGLEOP("console clear");
		}
		break;
		case 0x29:
		{
			SINGLEOP("console setcolor");
		}
		break;
		default:
			SINGLEOP("??");
			break;
	}

	bytesRead = addr - startAddr;
	return result;
}

SPL::Disassembling::Disassembled SPL::Disassembling::Disassembler::DisassembleInstruction(rom _rom, const int& startAddr)
{
	int _ = 0;
	return DisassembleInstruction(_rom, startAddr, _);
}

SPL::Disassembling::Disassembler::Disassembler(rom _rom)
{
	this->_rom = _rom;
}