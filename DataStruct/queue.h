#pragma once
#include <memory>
#include <cassert>
#include <new>

#include "Utility.h"

// overwrite�� �����ϱ� ���� Enum
enum class OverwriteMode
{
	FALSE,
	TRUE
};

// using Type = int;

// �����ϰ� �ڷᱸ���� ������ ������ �������� ������ ����.
// emplace, &&���� ������ ������� ����.
// ���� ť�� �����Ѵ�.
template <typename Type>
class queue
{
public:
	queue() = default;
	queue(OverwriteMode _Overwrite)
		: Overwrite(_Overwrite) {}
	queue(size_t _Capacity)
	{
		ReAllocate(_Capacity);
	}
	queue(size_t _Capacity, OverwriteMode _Overwrite)
		: Overwrite(_Overwrite)
	{
		ReAllocate(_Capacity);
	}
	~queue()
	{
		Clear();

		if (Arr != nullptr)
		{
			::operator delete(Arr);
			Arr = nullptr;
		}
	}

	queue(const queue& _Other) = delete;
	queue(queue&& _Other) = delete;
public:
	queue& operator= (const queue& _Other) = delete;
	queue& operator= (queue&& _Other) = delete;

private:
	Type* Arr = nullptr;
	size_t Size = 0;
	size_t Capacity = 0;
	size_t Pivot = 0;
	// ���� true��� ������ ���� �о������ ���� �������. �߰��� �۵� ���� ����.
	const OverwriteMode Overwrite = OverwriteMode::FALSE;

public:
	// ����, ���� ���θ� bool�� ��ȯ�ϰڴ�.
	bool Push(const Type& _Item)
	{
		// �̹� ���� �� �ִ��� Ȯ��
		if (IsFull())
		{
			// Overwrite Mode�� �ƴϰų� Capacity�� �ƴϸ� ����.
			if (Overwrite == OverwriteMode::FALSE || Capacity == 0)
			{
				return false;
			}
			// ���� ���� �� ���Ҹ� ����������. (�� ó�� ���Ҹ� ���� �������.)
			// ������ �����ϴ� �Ϳ� �Ҹ��ڸ� ȣ���Ű��, ���� placement new�� ����ϴ� �͵� ������ ���� ����ϵ�.
			else
			{
				size_t Index = (Pivot + Size) % Capacity;
				Arr[Index] = _Item;
				// ó�� ��ġ�� �з����� Pivot�� �����ؾ� ��.
				Pivot = (Pivot + 1) % Capacity;
				return true;
			}
		}

		// �� ó��.
		size_t Index = (Pivot + Size) % Capacity;
		new(Arr + Index) Type(_Item);
		++Size;
		return true;
	}

	// ó���� ���� �� �ϳ� ����.
	bool Pop()
	{
		// ���� queue���� ���Ҹ� ������ �� ����.
		if (IsEmpty())
		{
			return false;
		}

		// �� ó�� �� �Ҹ��� ȣ�� ���༭ �����.
		std::destroy_at(Arr + Pivot);

		--Size;
		Pivot = (Pivot + 1) % Capacity;

		return true;
	}

	// vector�� �ٸ��� �ڵ� Ȯ���� ���� ���� ��.
	bool ReAllocate(size_t _NewCapacity)
	{
		// �������� �۾����ų� ���Ұ� �����Ǵ� ������ �����Ѵ�.
		if (Capacity >= _NewCapacity || Size >= _NewCapacity)
			return false;

		// ���ο� �޸� �Ҵ�
		Type* NewArr = static_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity));

		// �̹� �Ҵ� �� �迭�� �����Ҷ�
		if (Arr != nullptr)
		{
			// ��� ���鼭 ���Ҹ� �����ϰ�, ���� �迭 ����.
			for (size_t i = 0; i < Size; ++i)
			{
				size_t Index = (Pivot + i) % Capacity;
				new(NewArr + i) Type(Utility::Move(Arr[Index]));
				std::destroy_at(Arr + Index);
			}

			::operator delete(Arr);
		}

		// queue �缳��
		Arr = NewArr;
		Pivot = 0;
		Capacity = _NewCapacity;

		return true;
	}

	void Clear()
	{
		// ���鼭 ��� �Ҹ��� ȣ�� �����ش�.
		for (size_t i = 0; i < Size; ++i)
		{
			size_t Index = (Pivot + i) % Capacity;
			std::destroy_at(Arr + Index);
		}

		// ����ϰ� ���������� �ٽ� �ʱ�ȭ.
		Size = 0;
		Pivot = 0;
	}

public:
	bool IsFull() const noexcept
	{
		return Size >= Capacity;
	}
	bool IsOverwrite() const noexcept
	{
		return Overwrite == OverwriteMode::TRUE;
	}
	bool IsEmpty() const noexcept
	{
		return Size == 0;
	}
	size_t GetSize() const noexcept
	{
		return Size;
	}
	size_t GetCapacity() const noexcept
	{
		return Capacity;
	}

	// ���⸸ �ϼ���
	const Type& Front() const noexcept
	{
		assert(Size);
		return Arr[Pivot];
	}
	const Type& Back() const noexcept
	{
		assert(Size);
		size_t Index = (Pivot + Size - 1) % Capacity;
		return Arr[Index];
	}

};