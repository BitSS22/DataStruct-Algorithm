#pragma once
#include <cassert>

#include "Utility.h"

using Type = int;
using Compare = bool(*)(const int& _Left, const int& _Right);

// template <typename Type, typename Comp>
class priority_queue
{
public:
	priority_queue() {}
	~priority_queue() {}
	priority_queue(Type* _Arr, Compare _Comp)
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
};