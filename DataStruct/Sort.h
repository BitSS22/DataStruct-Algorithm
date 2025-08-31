#pragma once
#include "Utility.h"
#include <assert.h>

typedef bool(*Compare)(const int&, const int&);
typedef int Type;

namespace Sort
{
#pragma region Sort Info
// Agument로 배열, 정렬 규칙 함수를 받는다.
// 배열의 길이는 템플릿 타입 추론으로 구한다.
// 함수 템플릿 오버로드로 기본 함수는 DefaultCompare(내림차순)를 사용한다.
// (람다, 펑터, 함수 포인터 등을 전부 받을 수 있도록 템플릿 오버로드)
// 함수 포인터가 nullptr이면 assert.

#pragma endregion
#pragma region Bubble
	template <typename Type, size_t Size, typename Compare>
	void BubbleSort(Type(&_Arr)[Size], Compare _Comp) noexcept
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
					Utility::Swap(_Arr[i + 1], _Arr[i]);
			}
			// 마지막 원소는 정렬 된 원소이므로 제외
			--_Size;
		}
	}
	template <typename Type, size_t Size>
	void BubbleSort(Type(&_Arr)[Size]) noexcept
	{
		BubbleSort(_Arr, Utility::DefaultCompare<Type>);
	}
#pragma endregion
#pragma region Insert
	template <typename Type, size_t Size, typename Compare>
	void InsertSort(Type (&_Arr)[Size], Compare _Comp) noexcept
	{
		assert(_Comp);

		// 첫번째 원소는 정렬 된 것으로 취급
		for (size_t i = 1; i < Size; ++i)
		{
			size_t CurIndex = i;

			// 현재 인덱스에서 0번 인덱스까지 순차적으로 값 비교
			while (CurIndex > 0)
			{
				// 현재 인덱스와 이전 인덱스의 대소 비교. true라면 둘을 바꾼다.
				// 바뀐 자리의 이전 인덱스와 계속 비교.
				if (_Comp(_Arr[CurIndex], _Arr[CurIndex - 1]))
				{
					Utility::Swap(_Arr[CurIndex], _Arr[CurIndex - 1]);
					--CurIndex;
				}
				else
				{
					// false라면 자기 자리를 찾았으므로 break
					break;
				}
			}
		}
	}
	template <typename Type, size_t Size>
	void InsertSort(Type(&_Arr)[Size]) noexcept
	{
		InsertSort(_Arr, Utility::DefaultCompare<Type>);
	}
#pragma endregion
#pragma region Selection
	template <typename Type, size_t Size, typename Compare>
	void SelectionSort(Type (&_Arr)[Size], Compare _Comp) noexcept
	{
		assert(_Comp);

		// 모든 원소 순회
		for (size_t i = 0; i < Size; ++i)
		{
			// 가장 _Comp에 맞는 원소를 찾는다.
			size_t FindIndex = i;
			for (size_t j = i; j < Size; ++j)
			{
				if (_Comp(_Arr[j], _Arr[FindIndex]))
					FindIndex = j;
			}
			// 찾은 원소를 i번째 원소와 교환한다.
			Utility::Swap(_Arr[i], _Arr[FindIndex]);
		}
	}
	template <typename Type, size_t Size>
	void SelectionSort(Type(&_Arr)[Size]) noexcept
	{
		SelectionSort(_Arr, Utility::DefaultCompare<Type>);
	}
#pragma endregion
#pragma region Merge
	void MergeSort(Type _Arr[], size_t Size, Compare _Comp)
	{
		// assert(Size == 0 || _Comp == nullptr)
		assert(Size || _Comp);

		// 같은 사이즈의 배열을 하나 만든다.
		Type* NewArr = reinterpret_cast<Type*>(malloc(sizeof(Type) * Size));

		// nullptr Check
		if (!NewArr)
			return;

		while (true)
		{

		}

		// 할당한 메모리 해제
		free(NewArr);
	}
#pragma endregion
#pragma region Shell
	void ShellSort(Type _Arr[], size_t Size);
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
