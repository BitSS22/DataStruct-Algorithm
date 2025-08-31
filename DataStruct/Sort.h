#pragma once
#include "Utility.h"
#include <assert.h>

typedef int Type;

namespace Sort
{
#pragma region Utility
	template<typename Type>
	inline bool DefaultCompare(const Type& _Left, const Type& _Right) noexcept
	{
		return _Left < _Right;
	}

	// Size < 2 Check
	constexpr bool LessTwo(size_t _Size) noexcept
	{
		return _Size < 2;
	}
#pragma endregion
#pragma region Bubble
	void BubbleSort(Type _Arr[], size_t _Size, bool(*_Comp)(const Type& _Left, const Type& Right) = DefaultCompare) noexcept
	{
		assert(_Comp);

		// ���� ���� 1 ���� == return
		while (_Size > 1)
		{
			for (size_t i = 0; i < _Size - 1; ++i)
			{
				// ���� ���ҿ� ���� ���� ��, true == Swap
				if (_Comp(_Arr[i + 1], _Arr[i]))
					Swap(_Arr[i], _Arr[i + 1]);
			}
			// ������ ���Ҵ� ���� �� �����̹Ƿ� ����
			--_Size;
		}
	}
#pragma endregion
#pragma region Insert
	void InsertSort(Type _Arr[], size_t Size)
	{

	}
#pragma endregion
#pragma region Selection
	void SelectionSort(Type _Arr[], size_t Size);
#pragma endregion
#pragma region Shell
	void ShellSort(Type _Arr[], size_t Size);
#pragma endregion
#pragma region Merge
	void MergeSort(Type _Arr[], size_t Size);
#pragma endregion
#pragma region Heap
	void HeapSort(Type _Arr[], size_t Size);
#pragma endregion
#pragma region Quick
	void QuickSort(Type _Arr[], size_t Size);
#pragma endregion
#pragma region Radix
	void RadixSort(Type _Arr[], size_t Size);
#pragma endregion
};
