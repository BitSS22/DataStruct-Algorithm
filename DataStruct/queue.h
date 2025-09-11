#pragma once
#include <memory>

#include "Utility.h"

using Type = int;

// �����ϰ� �ڷᱸ���� ������ ������ �������� ������ ����.
// emplace, &&���� ������ ������� ����.
// ���� ť�� �����Ѵ�.
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
	// ���� true��� ������ ���� �о������ ���� �������.
	const bool Overwrite = false;

	bool Enqueue(const Type& _Item)
	{
		if ()
	}

public:
	// vector�� �ٸ��� �ڵ� Ȯ���� ���� ���� ��.
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