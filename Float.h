#pragma once
#include <vector>

#include "rom.h"

/// <summary>
/// Convert a float to 4 individual bytes
/// </summary>
/// <param name="f">The float to convert</param>
/// <returns>Vector containing the generated bytes</returns>
static std::vector<unsigned char> FloatToBytes(float f)
{
	byte* bytes = reinterpret_cast<byte*>(&f);
	std::vector<byte> b;
	for (int i = 0; i < sizeof(float); i++) b.push_back(bytes[i]);

	return b;
}

/// <summary>
/// Convert bytes into a floating point number
/// </summary>
/// <param name="bytes">The bytes to convert</param>
/// <returns>The generated float</returns>
static float BytesToFloat(unsigned char* bytes)
{
	return *(float*)bytes;
}