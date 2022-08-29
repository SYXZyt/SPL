#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "../../../Int.h"
#include "../../../Float.h"
#include "../../../String.h"
#include "../../../ErrorStream.h"
#include "../Parsing/Nodes/Let.h"
#include "../Parsing/Nodes/Free.h"
#include "../Parsing/Nodes/Node.h"
#include "../Parsing/Nodes/Block.h"
#include "../Parsing/Nodes/Input.h"
#include "../Parsing/Nodes/Constant.h"
#include "../Parsing/Nodes/Stack/Pop.h"
#include "../Parsing/Nodes/Maths/Add.h"
#include "../Parsing/Nodes/Maths/Sub.h"
#include "../Parsing/Nodes/Maths/Div.h"
#include "../Parsing/Nodes/Maths/Mul.h"
#include "../Parsing/Nodes/Maths/Pow.h"
#include "../Parsing/Nodes/Maths/Inc.h"
#include "../Parsing/Nodes/Maths/Dec.h"
#include "../Parsing/Nodes/Maths/Mod.h"
#include "../Parsing/Nodes/Stack/Push.h"
#include "../Parsing/Nodes/Maths/Concat.h"
#include "../Parsing/Nodes/Output/Print.h"
#include "../Parsing/Nodes/Stack/SetPop.h"
#include "../Parsing/Nodes/Casting/ToInt.h"
#include "../Parsing/Nodes/ControlFlow/Ret.h"
#include "../Parsing/Nodes/Casting/ToFloat.h"
#include "../Parsing/Nodes/Comparisons/Equ.h"
#include "../Parsing/Nodes/Comparisons/Neq.h"
#include "../Parsing/Nodes/Comparisons/Grt.h"
#include "../Parsing/Nodes/Comparisons/Lwr.h"
#include "../Parsing/Nodes/Casting/ToString.h"
#include "../Parsing/Nodes/ControlFlow/Call.h"
#include "../Parsing/Nodes/ControlFlow/Goto.h"
#include "../Parsing/Nodes/ControlFlow/Exit.h"
#include "../Parsing/Nodes/Comparisons/GrtEqu.h"
#include "../Parsing/Nodes/Comparisons/LwrEqu.h"

using namespace SPL::Compiler::Parser::Nodes;
using namespace SPL::Compiler::Parser::Nodes::Casting;

namespace SPL
{
	namespace Compiler
	{
		namespace Assembler
		{
			struct FinalNodes
			{
				std::vector<Constant*> constants;
				std::vector<Node*> nodes;

				Node* operator[](int index)
				{
					return nodes[index];
				}

				Constant* operator<<(int index)
				{
					return constants[index];
				}
			};

			/// <summary>
			/// Class which can convert nodes to SPL bytecode
			/// </summary>
			class Assembler final
			{
			private:
				FinalNodes nodes;
				const char* outputfile;

				std::vector<std::string> identifiers;

				/// <summary>
				/// Get the binary offset of an instruction
				/// </summary>
				/// <param name="line">The line of the label</param>
				/// <returns>The offset in the binary</returns>
				int GetLabelOffset(int line);

				/// <summary>
				/// Write the assembled data, into a file on disk
				/// </summary>
				/// <param name="rom">The rom to write</param>
				void WriteBinary(std::vector<unsigned char> rom);
			public:

				/// <summary>
				/// Assemble all of the loaded nodes
				/// </summary>
				void Assemble();

				/// <summary>
				/// Create a new assembler
				/// </summary>
				/// <param name="nodes">The nodes to assemble</param>
				/// <param name="outputfile">The file to write to</param>
				Assembler(std::vector<Node*> nodes, const char* outputfile);
			};
		}
	}
}