#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "../../rom.h"
#include "../../Int.h"
#include "../../Float.h"
#include "../../String.h"

namespace SPL
{
	namespace Disassembling
	{
		/// <summary>
		/// Struct containing about each line of the disassembly
		/// </summary>
		struct Disassembled
		{
			int addr;
			byte opcode;
			std::vector<byte> assembled;
			std::string disassembled;
		};

		/// <summary>
		/// Class which can take a binary, and generate SPL code from it
		/// </summary>
		class Disassembler final
		{
		private:
			rom _rom;
		public:
			/// <summary>
			/// Disassemble the loaded rom
			/// </summary>
			void Disassemble();
			
			/// <summary>
			/// Create a new disassembler
			/// </summary>
			/// <param name="_rom">The rom to disassemble</param>
			Disassembler(rom _rom);
		};
	}
} 