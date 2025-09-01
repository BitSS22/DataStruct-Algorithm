#pragma once

#include <assert.h>
#include "Utility.h"

typedef bool(*Compare)(const int&, const int&);
typedef int Type;

namespace Sort
{
#pragma region Sort Info
	// �Ű� ������ �迭, ���� ��Ģ �Լ��� �޴´�.
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
	void InsertSort(Type(&_Arr)[Size], Compare _Comp) noexcept
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
	void SelectionSort(Type(&_Arr)[Size], Compare _Comp) noexcept
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
	template <typename Type, typename Compare>
	class MergeClass
	{
	public:
		// �����ڸ� delete�� ��ü�� ���� �� ���� �ϰ�,
		// Function friend�� private�� �ٸ� �Լ����� ȣ���� �� ���� �����.
		MergeClass() = delete;
		template <typename T, size_t i, typename Cmp>
		friend void MergeSort(T(&_Arr)[i], Cmp _Comp) noexcept;

	private:
		// ������ ������ �޸𸮸� ������ ������.
		// ���� Arr, NewArr, Comp�� ������ �Լ� ���� ������ ���� ���̴�.
		// �翬�� ������ ������ �ϳ��� �����Ƿ�, ������ ������ �� ���̴�.
		inline static Type* Arr = nullptr;
		inline static Type* NewArr = nullptr;
		inline static Compare Comp = nullptr;

		// ���� ����, ������ �ϴ� �Լ�
		// �迭 ũ�⸦ �߷��ϱ� ����� ���� �ε����� ����� ���� �ްڴ�.
		static void Merge(size_t _StartIndex, size_t _Size) noexcept
		{
			// �迭 ���̸� ������ �ڸ���. �������� �˰���.
			// �ڵ� �������� ���� ������ ���� �����ϰڴ�.
			size_t LeftStart = _StartIndex;
			size_t LeftSize = _Size / 2;
			size_t RightStart = LeftStart + LeftSize;
			size_t RightSize = _Size - LeftSize;

			// �迭 ���̰� 1�� �� �� ���� ��� ������.
			if (LeftSize > 1)
				Merge(LeftStart, LeftSize);

			if (RightSize > 1)
				Merge(RightStart, RightSize);

			// ������ʹ� ���ʷ� �� �� ���� �迭 ���̰� ���� 1�� ��.
			// �������� ������ �����Ѵ�.

			// ���� �迭�� �ε����� ���� ī��Ʈ �Ѵ�.
			size_t LeftIter = 0;
			size_t RightIter = 0;
			// ���� ��� �� ��ġ
			size_t WriteIndex = LeftStart;

			// �� �迭�� ���Ұ� �� �� �����ִ°�?
			while (LeftIter < LeftSize && RightIter < RightSize)
			{
				// Comp �Լ��� true / false�� ����.
				if (Comp(Arr[RightStart + RightIter], Arr[LeftStart + LeftIter]))
				{
					NewArr[WriteIndex] = Arr[RightStart + RightIter];
					++RightIter;
				}
				else
				{
					NewArr[WriteIndex] = Arr[LeftStart + LeftIter];
					++LeftIter;
				}

				// ������ ���� ���� �� ��ġ�� ���� �ε�����
				++WriteIndex;
			}

			// �� �ݺ����� ������ ���� �迭���� ���Ұ� ���� ���� ��.
			// ������ ���� ��ҵ��� ä���ִ´�.
			// Left
			while (LeftIter < LeftSize)
			{
				NewArr[WriteIndex] = Arr[LeftStart + LeftIter];
				++WriteIndex;
				++LeftIter;
			}
			// Right
			while (RightIter < RightSize)
			{
				NewArr[WriteIndex] = Arr[RightStart + RightIter];
				++WriteIndex;
				++RightIter;
			}

			// ���� �� NewArr�� _Arr�� �����Ѵ�.
			// �׳� for�� ���ڴ�.
			for (size_t i = _StartIndex; i < _StartIndex + _Size; ++i)
			{
				Arr[i] = MergeClass<Type, Compare>::NewArr[i]);
			}
		}
	};

	template <typename Type, size_t Size, typename Compare>
	void MergeSort(Type (&_Arr)[Size], Compare _Comp) noexcept
	{
		assert(Size);
		assert(_Comp);

		// ���� �������� �迭�� �ϳ� �������.
		// ū �� �ϳ� ���� ���� ����.
		MergeClass<Type, Compare>::NewArr = reinterpret_cast<Type*>(malloc(sizeof(Type) * Size));
		// malloc failed, nullptr return.
		if (nullptr == MergeClass<Type, Compare>::NewArr)
			return;

		// MergeClass ���� ���� �������ش�.
		MergeClass<Type, Compare>::Arr = _Arr;
		MergeClass<Type, Compare>::Comp = _Comp;

		// ���� ������ ���⼭ ����.
		MergeClass<Type, Compare>::Merge(0, Size);

		// �Ҵ��� �޸� ����.
		// ���� ������ ���� ����Ѵٸ� ������ ���μ��� ����� �������� �ѹ��� �ϴ� �͵� ����غ��� �ϴ�.
		// (������ �޸� ũ�⸦ ����صΰ�, ���ڶ�� ���Ҵ��ϰ�, ����ϴٸ� ����.)
		free(MergeClass<Type, Compare>::NewArr);

		// �ӹ� �ڸ��� �Ƹ����.
		MergeClass<Type, Compare>::Arr = nullptr;
		MergeClass<Type, Compare>::NewArr = nullptr;
		MergeClass<Type, Compare>::Comp = nullptr;
	}
	template <typename Type, size_t Size>
	void MergeSort(Type(&_Arr)[Size]) noexcept
	{
		MergeSort(_Arr, Utility::DefaultCompare<Type>);
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
