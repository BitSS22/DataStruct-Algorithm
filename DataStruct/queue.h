#pragma once
#include <memory>

#include "Utility.h"

using Type = int;

// 간단하게 자료구조의 개념을 익히는 수준으로 구현해 본다.
// emplace, &&같은 구현은 고려하지 않음.
// 원형 큐로 구현한다.
template <typename Type>
class queue
{
public:
	queue() {}
	queue(bool _Overwrite)
		: Overwrite(_Overwrite) {}

private:
	Type* Arr = nullptr;
	size_t Size = 0;
	size_t Capacity = 0;
	size_t Pivot = 0;
	// 만약 true라면 기존의 값을 밀어버리고 덮어 써버린다.
	const bool Overwrite = false;

	bool Enqueue(const Type& _Item)
	{
		if ()
	}

public:
	// vector와 다르게 자동 확장은 하지 않을 것.
	bool ReAllocate(size_t _NewCapacity)
	{
		if (Capacity >= _NewCapacity || Size >= _NewCapacity)
			return false;

		Type* NewArr = ::operator new(sizeof(Type) * _NewCapacity);

		if (Arr != nullptr)
		{
			for (size_t i = 0; i < Size; ++i)
			{
				new(NewArr[i]) Type(Utility::Move(Arr[(Pivot + i) % Capacity]));
				std::destroy_at(Arr[(Pivot + i) % Capacity]);
			}

			::operator delete(Arr);
		}

		Arr = NewArr;
		Pivot = 0;
		Capacity = _NewCapacity;
	}

public:
	bool IsFull() const noexcept
	{
		return Size >= Capacity;
	}
	bool IsOverwrite() const noexcept
	{
		return Overwrite;
	}
	size_t GetSize() cosnt noexcept
	{
		return Size;
	}
	size_t GetCapacity() const noexcept
	{
		return Capacity;
	}

};