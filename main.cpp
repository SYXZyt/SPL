#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "SPLErrors.h"

#include "rom.h"
#include "SPL\Compiler\Parsing\Parser.h"
#include "SPL\VirtualMachine\Processor.h"
#include "SPL\Disassembling\Disassembler.h"
#include "SPL\Compiler\Parsing\Nodes\Node.h"
#include "SPL\Compiler\Tokenisation\Token.h"
#include "SPL\Compiler\Tokenisation\Lexer.h"
#include "SPL\Compiler\Assembling\Assembler.h"
#include "SPL\Compiler\PreProcessing\PreProcessor.h"

//These are used for development. Enable them if you want. They mostly just dump pre-ran code
//May be interesting if you wanna see how the language handles code under the hood
//#define SKIPVC		//Skip the version control check
//#define DUMPLEXER		//Print the tokens generated by the Lexer to the screen
//#define DUMPPRE		//Print the tokens generated by the PreProcessor to the screen
//#define DUMPPARSER	//Print the nodes generated by the Parser to the screen
//#define ALWAYSCOMPILE	//Skip checking for the hash and always compile the code, even if a binary matching it has been found
//#define DISASSEMBLE

const char* hashName = "_spl_cache_\\checksum.hash"; //Stores a hash unique to each script, to check if the program needs to compile again
const char* binaryName = "_spl_cache_\\out.bin"; //The name of the binary spat out by the compiler

//https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
/// <summary>
/// Check to see if a file exists
/// </summary>
/// <param name="filename">The filename to check for</param>
/// <returns>True if the file exists</returns>
inline bool FileExists(const char* filename)
{
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

//https://stackoverflow.com/questions/34153765/generate-checksum-for-string
/// <summary>
/// Generate a checksum for a provided string
/// </summary>
/// <param name="s">The string to generate a checksum for</param>
/// <returns></returns>
long long int GenerateCheckSum(std::string s)
{
	long long k = 7;
	
	for (int i = 0; i < s.length(); i++)
	{
		k *= 23;
		k += s[i];
		k *= 13;
		k %= 1000000009;
	}

	return k;
}

/// <summary>
/// Generate a checksum and write it to a cache file
/// </summary>
/// <param name="data">The data to generate a sum for</param>
void WriteCheckSum(std::string data)
{
	long long int sum = GenerateCheckSum(data);

	std::filesystem::create_directory("_spl_cache_");

	std::ofstream out(hashName);
	out << sum;
	out.close();
}

/// <summary>
/// Load a rom from a binary file
/// </summary>
/// <param name="filename">The file to load from</param>
/// <returns>The rom that has been loaded</returns>
rom LoadRom(const char* filename)
{
	if (!FileExists(filename))
	{
		std::cerr << "File '" << filename << "' does not exist" << std::endl;
		exit(0);
	}

	std::ifstream in(filename, std::ios::binary);
	in.unsetf(std::ios::skipws);
	std::streampos filesize;

	in.seekg(0, std::ios::end);
	filesize = in.tellg();
	in.seekg(0, std::ios::beg);

	std::vector<byte> v;
	v.reserve(filesize);

	v.insert(v.begin(), std::istream_iterator<byte>(in), std::istream_iterator<byte>());
	rom r = {new byte[v.size()], v.size()};
	byte* bytes = v.data();

	//If we did r.bytes = v.data() then all the data in the rom would be lost when v is deleted, so we must create new memory and handle the deletion ourselves-
	memcpy(r.bytes, bytes, r.size);

	return r;
}

/// <summary>
/// Check if the current data is the same as the saved checksum. This is to determine if the code should be recompiled
/// </summary>
/// <param name="data">The data to generate a sum for</param>
/// <returns>If the checksums are identical</returns>
bool CheckSumIdentical(std::string data)
{
	//If the checksum file does not exist, return false as we need to compile the program
	if (!FileExists(hashName)) return false;

	long long int fileToCompile = GenerateCheckSum(data);
	long long int hash;

	std::ifstream file(hashName);
	file >> hash;

	return fileToCompile == hash;
}

/// <summary>
/// Ask the user to enter code using the terminal
/// </summary>
/// <returns>The code the user entered</returns>
std::string GetStdioInput()
{
	std::string line;
	std::string output = "";

	std::cout << "SPL v1.0.0 (13 August 2022) [X64]\nEnter a blank line to execute" << std::endl;

	while (true)
	{
		std::cout << ">>>";
		std::getline(std::cin, line);
		line += "\n";
		output += line;

		if (line == "\n")
		{
			break;
		}
	}

	return output;
}

/// <summary>
/// Read SPL from a file
/// </summary>
/// <param name="filename">The file to load code from</param>
/// <returns>The SPL code that the file contained</returns>
std::string GetFileInput(std::string filename)
{
	std::ifstream in(filename);
	std::stringstream buffer;
	buffer << in.rdbuf();
	return buffer.str();
}

int main(int argc, char** argv)
{
#ifdef _DEBUG
	std::cout << "DEBUG BUILD. CHANGE FLAG TO RELEASE BEFORE DISTRIBUTING" << std::endl;
#endif

	bool skipOptimiser = false;

	using namespace SPL::Disassembling;
	using namespace SPL::VirtualMachine;
	using namespace SPL::Compiler::Parser;
	using namespace SPL::Compiler::Assembler;
	using namespace SPL::Compiler::Tokenisation;
	using namespace SPL::Compiler::Parser::Nodes;
	using namespace SPL::Compiler::PreProcessing;

	std::string inputName = "<stdio>";
	std::string input;

	//If no file argument was provided, just get the code off of the console
	if (argc <= 1)
	{
		input = GetStdioInput();
	}
	else
	{
		const char* file = argv[1];

		if (!FileExists(file))
		{
			std::cerr << file << " does not exist" << std::endl;
		}

		inputName = file;
		input = GetFileInput(inputName);
	}

	bool reCompile = !CheckSumIdentical(input);

	//Another check we can do is by compiling anywhere if no rom was found
	if (!FileExists(binaryName)) reCompile = true;

#if defined ALWAYSCOMPILE || _DEBUG
	reCompile = true;
#endif

	//If the saved binary is the same as the code we have here, then we don't need to recompile the program
	if (reCompile)
	{
		Lexer lexer = Lexer(input, inputName);
		std::vector<Token> tokens = lexer.GenerateTokens();

#ifdef DUMPLEXER
		for (Token t : tokens)
		{
			std::cout << t << std::endl;
		}
#endif

		PreProcessor preProcessor = PreProcessor(tokens);
		tokens = preProcessor.Process();

#ifdef DUMPPRE
		for (Token t : tokens)
		{
			std::cout << t << std::endl;
		}
#endif

		Parser parser = Parser(tokens);
		std::vector<Node*> nodes = parser.Parse();

#ifdef DUMPPARSER
		int size = 0;
		for (Node* n : nodes)
		{
			std::cout << n->_ToString() << std::endl;
			size += n->Size();
		}
		std::cout << "Total Size: " << size << " bytes" << std::endl;
#endif
		
		Assembler assembler = Assembler(nodes, binaryName);
		assembler.Assemble();

		if (nodes.size() == 0) Error("No SPL code provided");

		//Now the binary has been written, we can write the correct checksum
		WriteCheckSum(input);

		//Now that we have finished with the nodes, we can clean them up
		for (Node* n : nodes)
		{
			delete n;
			n = nullptr;
		}
	}

	//Now that we know we have an assembled file, we can load it, and start up the virtual machine
	//Important to note that the heap memory in the rom object is destroyed in the destructor of the processor
	//If theres is no processor object, the memory will remain
	//To fix this, use rom::clear();
	rom _rom = LoadRom(binaryName);

#ifdef DISASSEMBLE
	Disassembler d = Disassembler(_rom);
	d.Disassemble();
#else
	Processor cpu = Processor(_rom);
	cpu.Run();
	std::cout << "Program exited with code " << cpu.GetExitCode() << std::endl;
#endif
}