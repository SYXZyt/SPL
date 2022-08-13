#pragma once
#include <map>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "../../Int.h"
#include "../../rom.h"
#include "../../Stack.h"
#include "../../Float.h"
#include "Accumulator.h"
#include "VariableData.h"
#include "SPLExitCodes.h"
#include "../../ErrorStream.h"

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

			Accumulator accumulator;

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
			Processor(rom _rom);
			~Processor();
		};
	}
}