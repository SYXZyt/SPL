#pragma once
typedef unsigned char byte;

/// <summary>
/// Struct representing a rom file
/// </summary>
struct rom
{
	byte* bytes;
	size_t size;

	/// <summary>
	/// Get a byte at a given offset
	/// </summary>
	/// <param name="i">Index of the byte</param>
	/// <returns>The byte at the index</returns>
	byte operator[](int i)
	{
		return bytes[i];
	}

	/// <summary>
	/// Delete all the bytes saved
	/// </summary>
	void Clear()
	{
		delete bytes;
		bytes = nullptr;
	}
};