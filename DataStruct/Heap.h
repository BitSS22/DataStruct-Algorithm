#pragma once
#include <cassert>

#include "Utility.h"

using Priority_Queue = heap;
using Type = int;
using Compare = bool(*)(const int& _Left, const int& _Right);

class heap
{
public:
	heap() {}
	~heap() {}
	heap(Type* _Arr, Compare _Comp)
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


	// static으로 배열을 힙으로 만들수 있게.
public:
	static void Heapify(Type* _Arr, size_t _Size, Compare _Comp = Utility::DefaultCompare)
	{

	}

	static void BuildHeap(Type* _Arr, size_t _Size, Compare _Comp = Utility::DefaultCompare)
	{

	}
};