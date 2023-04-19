#pragma once
#include <vector>
#include <iostream>

#include "VariableData.h"

namespace SPL
{
	namespace VirtualMachine
	{
		class ProcessorStack final
		{
		private:
			std::vector<VariableData*> s;

		public:
			inline size_t Size()
			{
				return s.size();
			}

			inline bool IsEmpty()
			{
				return s.size() == 0;
			}

			inline void Push(VariableData* value)
			{
				s.push_back(value);
			}

			inline VariableData* Peek()
			{
				if (IsEmpty()) return nullptr;

				return s[s.size() - 1];
			}

			inline VariableData* Pop()
			{
				if (IsEmpty()) return nullptr;
				VariableData* v = s.back();
				s.pop_back();
				return v;
			}

			auto begin() const
			{
				return s.begin();
			}

			auto end() const
			{
				return s.end();
			}
		};
	}
}