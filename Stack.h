#pragma once
#include <stack>
#include <iostream>

//The max amount of values the processors callstack can store
const int callstackMaxSize = static_cast<int>(pow(2, 16));

//I don't like the fact that pop doesn't return a value in the normal stack, so I just created my own wrapper class
/// <summary>
/// Class representing a stack collection
/// </summary>
/// <typeparam name="T">The type of object to store</typeparam>
template<typename T>
class Stack final
{
private:
	std::stack<T> s;
public:

	/// <summary>
	/// Get how many elements are on the stack
	/// </summary>
	/// <returns>The total count</returns>
	inline size_t Size();

	/// <summary>
	/// Check if there are zero values on the stack
	/// </summary>
	/// <returns>True if empty</returns>
	inline bool IsEmpty();

	/// <summary>
	/// Push a value onto the stack
	/// </summary>
	/// <param name="value">The value to push</param>
	void Push(T value);

	/// <summary>
	/// Get a const reference to the bottom value on the stack
	/// </summary>
	/// <returns>The bottom value</returns>
	const T& Peek();

	/// <summary>
	/// Pop a value off of the stack, and return it
	/// </summary>
	/// <returns>The bottom value on the stack</returns>
	T Pop();

	/// <summary>
	/// Create a new stack object
	/// </summary>
	Stack();
};

template<typename T>
inline size_t Stack<T>::Size()
{
	return s.size();
}

template<typename T>
inline bool Stack<T>::IsEmpty()
{
	return s.size() == 0;
}

template<typename T>
inline void Stack<T>::Push(T value)
{
	s.push(value);
}

template<typename T>
inline const T& Stack<T>::Peek()
{
	return s.top();
}

template<typename T>
inline T Stack<T>::Pop()
{
	T v = s.top();
	s.pop();
	return v;
}

template<typename T>
inline Stack<T>::Stack()
{
	s = std::stack<T>();
}