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
		delete[] bytes;
		bytes = nullptr;
	}
};

static rom TrimRom(int newStart, rom& _rom)
{
	//If the start pos is 0, no changes are needed
	if (newStart == 0) return _rom;

	rom newRom{};
	newRom.bytes = new byte[_rom.size - newStart];
	newRom.size = _rom.size - newStart;

	//The reason I manually move the memory here, is because memcpy can't copy from a given index
	for (int i = newStart; i < _rom.size; i++)
	{
		newRom.bytes[i - newStart] = _rom[i];
	}

	_rom = newRom;

	return newRom;
}