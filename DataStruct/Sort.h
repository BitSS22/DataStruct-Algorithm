#pragma once
#include "Utility.h"
#include <assert.h>

typedef int Type;

namespace Sort
{
#pragma region Sort Info
// Agument로 배열, 정렬 규칙 함수를 받는다.
// 배열의 길이는 템플릿 타입 추론으로 구한다.
// 함수 미 지정시 디폴트 인자로 미리 정의한 DefaultCompare를 사용한다. (내림차순)
#pragma endregion
#pragma region Bubble
	template <typename Type, size_t Size>
	void BubbleSort(Type (&_Arr)[Size], bool(*_Comp)(const Type& _Left, const Type& Right) = Utility::DefaultCompare) noexcept
	{
		assert(_Comp);
		size_t _Size = Size;

		// 남은 원소 1 이하 == return
		while (_Size > 1)
		{
			for (size_t i = 0; i < _Size - 1; ++i)
			{
				// 다음 원소와 현재 원소 비교, true == Swap
				if (_Comp(_Arr[i + 1], _Arr[i]))
					Utility::Swap(_Arr[i], _Arr[i + 1]);
			}
			// 마지막 원소는 정렬 된 원소이므로 제외
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
