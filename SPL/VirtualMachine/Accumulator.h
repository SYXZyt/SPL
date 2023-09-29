#pragma once
#include <cmath>

#include "VariableData.h"
#include "ProcessorStack.h"

namespace SPL
{
	namespace VirtualMachine
	{
		/// <summary>
		/// Class storing methods to assist the virtual processor with maths operations
		/// </summary>
		class Accumulator final
		{
		private:
			ProcessorStack* stack;

		public:
			/// <summary>
			/// Calculate addition and push the result
			/// </summary>
			void CalculateAddition();

			/// <summary>
			/// Calculate subtraction and push the result
			/// </summary>
			void CalculateSubtraction();

			/// <summary>
			/// Calculate multiplication and push the result
			/// </summary>
			void CalculateMultiplication();

			/// <summary>
			/// Calculate  division and push the result
			/// </summary>
			/// <returns>False if the division could not be calculated</returns>
			bool CalculateDivision();

			/// <summary>
			/// Calculate power and push the result
			/// </summary>
			void CalculatePower();

			/// <summary>
			/// Calculate a concatenation and push the result
			/// </summary>
			void CalculateConcatenation();

			void Increment();
			void Decrement();

			void Modulo();

			/// <summary>
			/// Cast a value to string
			/// </summary>
			void CastToString();

			/// <summary>
			/// Cast a value to floating point, and push the result
			/// </summary>
			/// <returns>False if the cast failed</returns>
			bool CastToFloat();

			/// <summary>
			/// Cast a value to an integer, and push the result
			/// </summary>
			/// <returns>False if the cast failed</returns>
			bool CastToInt();

			/// <summary>
			/// Check if a value can be cast to an int
			/// </summary>
			void IsInt();
			
			/// <summary>
			/// Check if a value can be cast to a float
			/// </summary>
			void IsFloat();

			bool EqualComparison();
			bool NotEqualComparison();
			bool GreaterComparison();
			bool GreaterEqualComparison();
			bool LessComparison();
			bool LessEqualComparison();

			/// <summary>
			/// Create a new accumulator
			/// </summary>
			Accumulator();
			Accumulator(const Accumulator&) = delete;

			/// <summary>
			/// Create a new accumulator
			/// </summary>
			/// <param name="stack">The stack to push and pop from</param>
			Accumulator(ProcessorStack& stack);
		};
	}
}