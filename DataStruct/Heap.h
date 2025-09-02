#pragma once
#include <utility>
#include "Utility.h"

using Type = int;
using Compare = bool(*)(const int& _Left, const int& _Right);

// 실질적으로는 우선순위 큐.
// static 함수로 배열에 대한 BuildHeap을 지원한다.
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