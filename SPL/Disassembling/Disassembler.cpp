#include "Disassembler.h"

//#define FASTDUMP

#define EMPTY std::vector<byte>()

#define SINGLEOP(opName) \
result.assembled = EMPTY; \
result.disassembled = opName;

#define SETRESULT \
result.assembled = bytes; \
result.disassembled = spl;

#define ADDNAME(str) \
str += ReadString(rom, bytes, i);

#define ADDNAMENOINC(str) \
ADDNAME(str) \
i--;

#define MOVEBYTES(dst, src) \
dst[0] = src[0]; \
dst[1] = src[1]; \
dst[2] = src[2]; \
dst[3] = src[3];

static std::string ReadString(byte* rom, std::vector<byte>& bytes, int& i)
{
	std::string chars = "";
	while (rom[i])
	{
		bytes.push_back(rom[i]);
		chars += rom[i++];
	}

	bytes.push_back(0);
	i++;
	return chars;
}

void SPL::Disassembling::Disassembler::Disassemble()
{
	std::vector<Disassembled> results = std::vector<Disassembled>();

	//We need to loop over every byte and make an attempt at generating SPL code from it
	//Some instructions may fail so we can use ?? to represent that the disassembler failed to generate an instruction
	byte* rom = &_rom.bytes[0];

	for (int i = 0; i < _rom.size; i++)
	{
		byte opcode = rom[i];

		Disassembled result{};
		result.opcode = opcode;

		//Check if this is a known opcode
		switch (opcode)
		{
			case 0x00:
				SINGLEOP("nop");
				break;
			case 0x01:
			{
				i++;
				std::string spl = "setpop ";
				std::vector<byte> bytes;
				ADDNAMENOINC(spl);

				SETRESULT;
			}
			break;
			case 0x02:
			{
				i++;
				std::string spl = "str ";
				std::vector<byte> bytes;
				ADDNAME(spl);

				spl += " \"";
				ADDNAMENOINC(spl);
				spl += '"';

				SETRESULT;
			}
			break;
			case 0x03:
			{
				i++;
				std::string spl = "flt ";
				std::vector<byte> bytes;
				ADDNAME(spl);

				std::vector<byte> flt;
				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[j + i]);
					flt.push_back(rom[j + i]);
				} i += 3;

				float v = BytesToFloat(flt.data());
				spl += ' ' + std::to_string(v);

				SETRESULT;
			}
			break;
			case 0x04:
			{
				i++;
				std::string spl = "int ";
				std::vector<byte> bytes;
				ADDNAME(spl);

				std::vector<byte> integer;
				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[j + i]);
					integer.push_back(rom[j + i]);
				} i += 3;
				byte by[4]{};
				MOVEBYTES(by, bytes);

				int _i = BytesToInt(by);
				spl += ' ' + std::to_string(_i);

				SETRESULT;
			}
			break;
			case 0x05:
			{
				i++;
				std::string spl = "cpy ";
				std::vector<byte> bytes;
				ADDNAME(spl);

				spl += ' ';
				ADDNAMENOINC(spl);

				SETRESULT;
			}
			break;
			case 0x06:
			{
				i++;
				std::string spl = "strout \"";
				std::vector<byte> bytes;
				ADDNAMENOINC(spl);
				spl += '"';

				SETRESULT;
			}
			break;
			case 0x07:
			{
				i++;
				std::string spl = "fltout ";
				std::vector<byte> bytes;
				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[i + j]);
				} i += 3;
				byte by[4]{};
				MOVEBYTES(by, bytes);

				float value = BytesToFloat(by);
				spl += std::to_string(value);

				SETRESULT;
			}
			break;
			case 0x08:
			{
				i++;
				std::string spl = "intout ";
				std::vector<byte> bytes;

				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[i + j]);
				} i += 3;
				byte by[4]{};
				MOVEBYTES(by, bytes);
				int value = BytesToInt(by);
				spl += std::to_string(value);

				SETRESULT;
			}
			break;
			case 0x09:
			{
				i++;
				std::string spl = "varout ";
				std::vector<byte> bytes;
				ADDNAMENOINC(spl);

				SETRESULT;
			}
			break;
			case 0x0a:
			{
				i++;
				std::string spl = "free ";
				std::vector<byte> bytes;

				ADDNAMENOINC(spl);

				SETRESULT;
			}
			break;
			case 0x0b:
			{
				i++;
				std::string spl = "exit ";
				std::vector<byte> bytes;
				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[i + j]);
				} i += 3;
				byte by[4]{};
				MOVEBYTES(by, bytes);
				int value = BytesToInt(by);
				spl += std::to_string(value);

				SETRESULT;
			}
			break;
			case 0x0c:
			{
				i++;
				std::string spl = "goto ";
				std::vector<byte> bytes;
				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[i + j]);
				} i += 3;
				byte by[4]{};
				MOVEBYTES(by, bytes);
				int value = BytesToInt(by);
				spl += std::to_string(value);

				SETRESULT;
			}
			break;
			case 0x0d:
			{
				i++;
				std::string spl = "call ";
				std::vector<byte> bytes;
				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[i + j]);
				} i += 3;
				byte by[4]{};
				MOVEBYTES(by, bytes);
				int value = BytesToInt(by);
				spl += std::to_string(value);

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
				i++;
				std::string spl = "strpush \"";
				std::vector<byte> bytes;
				ADDNAMENOINC(spl);
				spl += '"';

				SETRESULT;
			}
			break;
			case 0x10:
			{
				i++;
				std::string spl = "fltpush ";
				std::vector<byte> bytes;
				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[i + j]);
				} i += 3;
				byte by[4]{};
				MOVEBYTES(by, bytes);
				float value = BytesToFloat(by);
				spl += std::to_string(value);

				SETRESULT;
			}
			break;
			case 0x11:
			{
				i++;
				std::string spl = "intpush ";
				std::vector<byte> bytes;
				for (int j = 0; j < 4; j++)
				{
					bytes.push_back(rom[i + j]);
				} i += 3;
				byte by[4]{};
				MOVEBYTES(by, bytes);
				int value = BytesToInt(by);
				spl += std::to_string(value);

				SETRESULT;
			}
			break;
			case 0x12:
			{
				i++;
				std::string spl = "varout ";
				std::vector<byte> bytes;
				ADDNAMENOINC(spl);

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
			default:
				SINGLEOP("??");
				break;
			}

		results.push_back(result);
		}
#pragma region Dump
#ifdef FASTDUMP
	for (Disassembled result : results)
	{
		std::stringstream ss;
		ss << std::hex << std::setw(2) << std::setfill('0');
		ss << (unsigned)result.opcode;
		ss << "  ";
		if (result.assembled.size())
		{
			for (byte b : result.assembled)
			{
				ss << std::hex << std::setw(2) << std::setfill('0');
				ss << (int)b << ' ';
			}
		}
		else
		{
			ss << "## ";
		}
		std::cout << ss.str();
		std::cout << ' ' << Escape(result.disassembled) << std::endl;
	}
#else
	//The idea here is that want to find out the largest amount of bytes he have to display, so we can offset all the disassembly, to make it nice and neat
	int charactersToAdd = 0;
	int longestSPL = 0;
	for (Disassembled d : results)
	{
		//We can simply get the longest by updating the longest if the current length is longer
		//We have to multiply the length by 3, as each byte has three characters used to display. e.g. `45 `
		//Characters is simply how many characters are needed to reach the target length. It is used in a calculation a bit further down
		int len = static_cast<int>((d.assembled.size() * 3) + 1);
		if (len > charactersToAdd) charactersToAdd = len;
		if (d.disassembled.size() > longestSPL) longestSPL = static_cast<int>(d.disassembled.size());
	}

	//Draws the guide at the top of the dump, can be disabled by commenting these. It's only for aesthetics
	std::cout << "OP||BIN|";
	for (int i = 0; i < charactersToAdd - 5; i++) std::cout << ' ';
	std::cout << "|ASM|" << std::endl;

	for (int i = 0; i < charactersToAdd + 3 + longestSPL; i++) std::cout << '=';
	std::cout << std::endl;

	//Now that we have done all calculations and drawn the guide, we can begin to dump the rom
	for (Disassembled result : results)
	{
		std::stringstream ss;
		ss << std::hex << std::setw(2) << std::setfill('0');
		ss << (unsigned)result.opcode << "  ";

		if (result.assembled.size())
		{
			for (byte b : result.assembled)
			{
				ss << std::hex << std::setw(2) << std::setfill('0');
				ss << (int)b << ' ';
			}
		}
		std::cout << ss.str();

		int spacer = charactersToAdd - (static_cast<int>(result.assembled.size() * 3));

		for (int i = 0; i < spacer; i++) std::cout << ' ';
		std::cout << Escape(result.disassembled) << std::endl;
	}
#endif
#pragma endregion
	}

SPL::Disassembling::Disassembler::Disassembler(rom _rom)
{
	this->_rom = _rom;
}