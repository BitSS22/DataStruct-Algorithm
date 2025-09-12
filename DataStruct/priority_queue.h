#pragma once
#include <cassert>

#include "Utility.h"
#include "vector.h"

// using Type = int;
// using Compare = std::less<Type>;

template <typename Type, typename Compare = std::less<Type>>
class priority_queue
{
public:
	priority_queue() {}
	priority_queue(Compare _Comp)
		: Comp(_Comp) {}
	~priority_queue() {}

private:
	// 이미 구현해 놓은 벡터 기능에서 자동화만 시킨다.
	vector<Type> Arr = {};
	Compare Comp = Compare{};

public:
	void Push(const Type& _Item)
	{
		Arr.HeapPush(_Item, Comp);
	}
	void Push(Type&& _Item)
	{
		Arr.HeapPush(Utility::Forward<Type>(_Item), Comp);
	}
	void Pop()
	{
		Arr.HeapPop(Comp);
	}
	size_t GetSize() const noexcept
	{
		return Arr.GetSize();
	}
	size_t GetCapacity() const noexcept
	{
		return Arr.GetCapacity();
	}
	bool IsEmpty() const noexcept
	{
		return Arr.IsEmpty();
	}
	const Type& Top() const noexcept
	{
		assert(GetSize());
		return Arr[0];
	}
};