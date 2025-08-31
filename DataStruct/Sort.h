#pragma once
#include "Utility.h"
#include <assert.h>

typedef int Type;

namespace Sort
{
#pragma region Sort Info
// Agument�� �迭, ���� ��Ģ �Լ��� �޴´�.
// �迭�� ���̴� ���ø� Ÿ�� �߷����� ���Ѵ�.
// �Լ� �� ������ ����Ʈ ���ڷ� �̸� ������ DefaultCompare�� ����Ѵ�. (��������)
#pragma endregion
#pragma region Bubble
	template <typename Type, size_t Size>
	void BubbleSort(Type (&_Arr)[Size], bool(*_Comp)(const Type& _Left, const Type& Right) = Utility::DefaultCompare) noexcept
	{
		assert(_Comp);
		size_t _Size = Size;

		// ���� ���� 1 ���� == return
		while (_Size > 1)
		{
			for (size_t i = 0; i < _Size - 1; ++i)
			{
				// ���� ���ҿ� ���� ���� ��, true == Swap
				if (_Comp(_Arr[i + 1], _Arr[i]))
					Utility::Swap(_Arr[i], _Arr[i + 1]);
			}
			// ������ ���Ҵ� ���� �� �����̹Ƿ� ����
			--_Size;
		}
	}
#pragma endregion
#pragma region Insert
	void InsertSort(Type _Arr[], size_t Size, bool(*_Comp)(const Type& _Left, const Type& Right) = Utility::DefaultCompare) noexcept
	{
		assert(_Comp);


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
