#pragma once
#include "Utility.h"
#include <assert.h>

typedef bool(*Compare)(const int&, const int&);
typedef int Type;

namespace Sort
{
#pragma region Sort Info
// Agument�� �迭, ���� ��Ģ �Լ��� �޴´�.
// �迭�� ���̴� ���ø� Ÿ�� �߷����� ���Ѵ�.
// �Լ� ���ø� �����ε�� �⺻ �Լ��� DefaultCompare(��������)�� ����Ѵ�.
// (����, ����, �Լ� ������ ���� ���� ���� �� �ֵ��� ���ø� �����ε�)
// �Լ� �����Ͱ� nullptr�̸� assert.

#pragma endregion
#pragma region Bubble
	template <typename Type, size_t Size, typename Compare>
	void BubbleSort(Type(&_Arr)[Size], Compare _Comp) noexcept
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
					Utility::Swap(_Arr[i + 1], _Arr[i]);
			}
			// ������ ���Ҵ� ���� �� �����̹Ƿ� ����
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

		// ù��° ���Ҵ� ���� �� ������ ���
		for (size_t i = 1; i < Size; ++i)
		{
			size_t CurIndex = i;

			// ���� �ε������� 0�� �ε������� ���������� �� ��
			while (CurIndex > 0)
			{
				// ���� �ε����� ���� �ε����� ��� ��. true��� ���� �ٲ۴�.
				// �ٲ� �ڸ��� ���� �ε����� ��� ��.
				if (_Comp(_Arr[CurIndex], _Arr[CurIndex - 1]))
				{
					Utility::Swap(_Arr[CurIndex], _Arr[CurIndex - 1]);
					--CurIndex;
				}
				else
				{
					// false��� �ڱ� �ڸ��� ã�����Ƿ� break
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

		// ��� ���� ��ȸ
		for (size_t i = 0; i < Size; ++i)
		{
			// ���� _Comp�� �´� ���Ҹ� ã�´�.
			size_t FindIndex = i;
			for (size_t j = i; j < Size; ++j)
			{
				if (_Comp(_Arr[j], _Arr[FindIndex]))
					FindIndex = j;
			}
			// ã�� ���Ҹ� i��° ���ҿ� ��ȯ�Ѵ�.
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

		// ���� �������� �迭�� �ϳ� �����.
		Type* NewArr = reinterpret_cast<Type*>(malloc(sizeof(Type) * Size));

		// nullptr Check
		if (!NewArr)
			return;

		while (true)
		{

		}

		// �Ҵ��� �޸� ����
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
