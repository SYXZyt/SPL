#pragma once
#include <map>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <Windows.h>

#include "../../RNG.h"
#include "../../Int.h"
#include "../../rom.h"
#include "../../Stack.h"
#include "../../Float.h"
#include "Accumulator.h"
#include "VariableData.h"
#include "../../Keyword.h"
#include "../../SPLErrors.h"
#include "../../ErrorStream.h"
#include "../Disassembling/Disassembler.h"

namespace SPL
{
	namespace VirtualMachine
	{

		class Processor final
		{
		private:
			rom _rom;
			int ptr;
			int code;
			bool terminate;
			bool breakpoint;

			std::vector<std::string> identifiers;

			RNG rng;
			Accumulator accumulator;

			void LoadConstants();
			void LoadIdentifiers();

			const HANDLE ch = GetStdHandle(STD_OUTPUT_HANDLE);

#pragma region Console_Stuff
			void MoveCursor();
			void ClearConsole();
			void SetColour();
			void CursorMode();
#pragma endregion

			/// <summary>
			/// Read a string from rom
			/// </summary>
			/// <returns>The read string</returns>
			std::string ReadString();

			/// <summary>
			/// Read an int from rom
			/// </summary>
			/// <returns>The read int</returns>
			int ReadInt();

			/// <summary>
			/// Read a float from rom
			/// </summary>
			/// <returns>The read float</returns>
			float ReadFloat();

			/// <summary>
			/// Move the current pointer to the next value
			/// </summary>
			void Advance();

			void Breakpoint();

			std::map<std::string, VariableData*> vstack;
			Stack<int> cstack;
			Stack<VariableData*> stack;

		public:
			/// <summary>
			/// Begin execution of the loaded rom
			/// </summary>
			void Run();

			/// <summary>
			/// Get the current exit code of the processor
			/// </summary>
			/// <returns></returns>
			const int GetExitCode() const;

			Processor() = delete;
			Processor(const Processor&) = delete;

			/// <summary>
			/// Create a new processor with a given rom
			/// </summary>
			/// <param name="_rom">The rom to load</param>
			Processor(rom _rom, bool breakpoint = false);
			~Processor();
		};
	}
}