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
	template <typename Type, typename Compare>
	void BubbleSort(Type _Arr[], size_t _Size, Compare _Comp) noexcept
	{
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
	template <typename Type>
	void BubbleSort(Type _Arr[], size_t _Size) noexcept
	{
		BubbleSort(_Arr, _Size, Utility::DefaultCompare<Type>);
	}
#pragma endregion
#pragma region Insert
	template <typename Type, typename Compare>
	void InsertSort(Type _Arr[], size_t _Size, Compare _Comp) noexcept
	{
		// ù��° ���Ҵ� ���� �� ������ ���
		for (size_t i = 1; i < _Size; ++i)
		{
			// ���� ��ȸ�ϴ� �ε��� ��ġ�� ���� �޾Ƶд�.
			size_t Iter = i;
			Type Inst = std::move(_Arr[Iter]);

			// (���� ��) ���� ���� ���ؼ� ���� ���� �о.
			while (_Comp(Inst, _Arr[Iter - 1]) && Iter > 0)
			{
				_Arr[Iter] = _Arr[Iter - 1];
				--Iter;
			}

			// ������ ��ġ�� �̵�.
			_Arr[Iter] = std::move(Inst);
		}
	}
	template <typename Type>
	void InsertSort(Type _Arr[], size_t _Size) noexcept
	{
		InsertSort(_Arr, _Size, Utility::DefaultCompare<Type>);
	}
#pragma endregion
#pragma region Selection
	template <typename Type, typename Compare>
	void SelectionSort(Type _Arr[], size_t _Size, Compare _Comp) noexcept
	{
		// ��� ���� ��ȸ
		for (size_t i = 0; i < _Size; ++i)
		{
			// ���� _Comp�� �´� ���Ҹ� ã�´�.
			size_t FindIndex = i;

			// j = i + 1 -> �ڱ� �ڽŰ� ���� �ʿ� ����
			for (size_t j = i + 1; j < _Size; ++j)
			{
				if (_Comp(_Arr[j], _Arr[FindIndex]))
					FindIndex = j;
			}

			// ã�� ���Ҹ� i��° ���ҿ� ��ȯ�Ѵ�.
			// �ڱ� �ڽ��̸� ��ȯ���� ����.
			if (FindIndex != i)
			{
				Utility::Swap(_Arr[i], _Arr[FindIndex]);
			}
		}
	}
	template <typename Type>
	void SelectionSort(Type _Arr[], size_t _Size) noexcept
	{
		SelectionSort(_Arr, _Size, Utility::DefaultCompare<Type>);
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
		template <typename T, typename Cmp>
		friend void MergeSort(T _Arr[], size_t _Size, Cmp _Comp) noexcept;

	private:
		// ������ ������ �޸𸮸� ������ ������.
		// ���� Arr, NewArr, Comp�� ������ �Լ� ���� ������ ���� ���̴�.
		// �迭 ���� ��ü�� �̵�, ���縦 �ּ�ȭ �ϱ� ���� index ������� ���� �Ѵ�.
		// �翬�� ������ ������ �ϳ��� �����Ƿ�, ������ ������ �� ���̴�.
		inline static Type* Arr = nullptr; // ���� ������ ���� ���� �ʹٸ� void*�� �����ϰ�, ĳ���� �Լ��� �����ϴ� �͵� ���� ����� �ɼ���.
		inline static size_t* Index = nullptr;
		inline static size_t* Buffer = nullptr;
		inline static Compare* CompPtr = nullptr;

		// ���� ����, ������ �ϴ� �Լ�
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
				const Type& Left = Arr[Index[LeftStart + LeftIter]];
				const Type& Right = Arr[Index[RightStart + RightIter]];

				// Comp �Լ��� true / false�� ����.
				if ((*CompPtr)(Right, Left))
				{
					Buffer[WriteIndex] = Index[RightStart + RightIter];
					++RightIter;
				}
				else
				{
					Buffer[WriteIndex] = Index[LeftStart + LeftIter];
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
				Buffer[WriteIndex] = Index[LeftStart + LeftIter];
				++WriteIndex;
				++LeftIter;
			}
			// Right
			while (RightIter < RightSize)
			{
				Buffer[WriteIndex] = Index[RightStart + RightIter];
				++WriteIndex;
				++RightIter;
			}

			// Index ����
			for (size_t i = _StartIndex; i < _StartIndex + _Size; ++i)
			{
				Index[i] = Buffer[i];
			}
		}
	};

	template <typename Type, typename Compare>
	void MergeSort(Type _Arr[], size_t _Size, Compare _Comp) noexcept
	{
		// Size ���� 1 ���ϸ� �� �ʿ䵵 ����.
		if (1 >= _Size)
			return;

		// ���� �������� �ε����� ���� �迭�� �ϳ��� �������.
		MergeClass<Type, Compare>::Index = new size_t[_Size];
		MergeClass<Type, Compare>::Buffer = new size_t[_Size];

		// Index�� �ε��� �� �ʱ�ȭ
		for (size_t i = 0; i < _Size; ++i)
		{
			MergeClass<Type, Compare>::Index[i] = i;
		}

		// MergeClass ���� ���� �������ش�.
		MergeClass<Type, Compare>::Arr = _Arr;
		MergeClass<Type, Compare>::CompPtr = &_Comp;

		// Index ������ ���⼭ ����.
		MergeClass<Type, Compare>::Merge(0, _Size);

		// ���⼭ ���� Index�� Index[i] == _Arr[i]�� ��� �ε����� ���� �ϴ°�?

		// Buffer�� ���� �� ��ġ�� ������ ����ִ´�.
		for (size_t k = 0; k < _Size; ++k)
			MergeClass<Type, Compare>::Buffer[MergeClass<Type, Compare>::Index[k]] = k;

		// �ε��� ������� Arr ���� �ӽ� ��ü ���� swap.
		for (size_t i = 0; i < _Size; ++i)
		{
			// ���� �迭�� ��ġ�� ���� �� ��ġ�� �´��� üũ�Ѵ�.
			// üũ �� ��Ȯ�ϴٸ� ���� �ε�����. �´� �ε����� �ö����� �ݺ�.
			while (i != MergeClass<Type, Compare>::Buffer[i])
			{
				// j == ���� ���� �ϴ°�?
				size_t j = MergeClass<Type, Compare>::Buffer[i];
				Utility::Swap(_Arr[i], _Arr[j]);
				Utility::Swap(MergeClass<Type, Compare>::Buffer[i], MergeClass<Type, Compare>::Buffer[j]); // ���� �迭 swap �� ����ȭ �۾�.
			}
		}

		// �Ҵ��� �޸� ����.
		// ���� ���� �迭�� ������ ���� ����Ѵٸ� ������ ���μ��� ����� �������� �ѹ��� �ϴ� �͵� ����غ��� �ϴ�.
		// ������ �޸� ũ�⸦ ����صΰ�, ���ڶ�� ���Ҵ��ϰ�, ����ϴٸ� ����.
		// �ٸ� ���ø� �Լ��̹Ƿ� ���� ������ �Լ��� Ŭ������ ��������� �޸� ������ ũ�� �� �� ���� �� ����.
		// �ƴ϶�� �ȿ��� ���� �Ҵ��� ���� �ʰ�, �ӽ� ���۸� �ۿ��� ���ڷ� �޾� ���� �ص� ������ ����ϼ��� �ִ�.
		delete[] MergeClass<Type, Compare>::Index;
		delete[] MergeClass<Type, Compare>::Buffer;

		// �ӹ� �ڸ��� �Ƹ����.
		MergeClass<Type, Compare>::Arr = nullptr;
		MergeClass<Type, Compare>::Index = nullptr;
		MergeClass<Type, Compare>::Buffer = nullptr;
		MergeClass<Type, Compare>::CompPtr = nullptr;
	}
	template <typename Type>
	void MergeSort(Type _Arr[], size_t _Size) noexcept
	{
		MergeSort(_Arr, _Size, Utility::DefaultCompare<Type>);
	}

#pragma endregion
#pragma region Shell
	// Insert �Լ����� �ܾ�Դ�.
	// ShellSort�� �ᱹ InsertSort�� �ݺ�.
	template <typename Type, typename Compare>
	void ShellSort(Type _Arr[], size_t _Size, Compare _Comp) noexcept
	{
		// �ʱ� Gap�� ����. Gap == 1 �� InsertSort. Gap�� ���ݾ� ������.
		for (size_t Gap = _Size / 2; Gap > 0; Gap /= 2)
		{
			for (size_t i = Gap; i < _Size; ++i)
			{
				size_t Iter = i;
				Type Inst = std::move(_Arr[Iter]);

				while (_Comp(Inst, _Arr[Iter - Gap]) && Iter >= Gap)
				{
					_Arr[Iter] = _Arr[Iter - Gap];
					Iter -= Gap;
				}

				_Arr[Iter] = std::move(Inst);
			}
		}
	}
	template <typename Type>
	void ShellSort(Type _Arr[], size_t _Size) noexcept
	{
		ShellSort(_Arr, _Size, Utility::DefaultCompare<Type>);
	}
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
