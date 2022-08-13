#pragma once

/// <summary>
/// Class representing a 2D point
/// </summary>
class Vector2 final
{
private:
	int x;
	int y;

public:
	/// <summary>
	/// Set the x position
	/// </summary>
	/// <param name="x">The new x position</param>
	void SetX(int x);

	/// <summary>
	/// Set the y position
	/// </summary>
	/// <param name="y">The new y position</param>
	void SetY(int y);

	/// <summary>
	/// Get the x position of the vector2
	/// </summary>
	/// <returns>The x position</returns>
	inline int X() const
	{
		return x;
	}

	/// <summary>
	/// Get the y position of the vector2
	/// </summary>
	/// <returns>The y position</returns>
	inline int Y() const
	{
		return y;
	}

	/// <summary>
	/// Create a blank vector2
	/// </summary>
	Vector2();

	/// <summary>
	/// Create a deep copy of another vector2
	/// </summary>
	/// <param name="v">The other vector</param>
	Vector2(const Vector2& v);

	/// <summary>
	/// Create a vector2 with x and y being the same value
	/// </summary>
	/// <param name="i"></param>
	Vector2(int i);

	/// <summary>
	/// Create a vector2 at a given point
	/// </summary>
	/// <param name="x">The x position</param>
	/// <param name="y">The y position</param>
	Vector2(int x, int y);
};