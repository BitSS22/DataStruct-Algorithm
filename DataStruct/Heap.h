#pragma once
#include <utility>
#include "Utility.h"

using Type = int;
using Compare = bool(*)(const int& _Left, const int& _Right);

// ���������δ� �켱���� ť.
// static �Լ��� �迭�� ���� BuildHeap�� �����Ѵ�.
// template <typename Type, typename Compare>
class Heap
{
public:
	Heap() {}
	~Heap() {}
	Heap(Type* _Arr, Compare _Comp)
		: Arr(_Arr)
		, Comp(_Comp) {}

private:
	Type* Arr = nullptr;
	size_t Size = 0;
	size_t Capacity = 0;
	Compare Comp = Utility::DefaultCompare;

public:
	void Insert(const Type& _Item)
	{

	}

	void Pop()
	{
	}

	const Type& Peek()
	{
		return Arr[0];
	}

	void Heapify(Type* _Arr, size_t _Size)
	{

	}

	void BuildHeap(Type* _Arr, size_t _Size)
	{

	}
};