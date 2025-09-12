#pragma once
#include <memory>
#include <cassert>
#include <new>

#include "Utility.h"

// overwrite를 정의하기 위한 Enum
enum class OverwriteMode
{
	FALSE,
	TRUE
};

// using Type = int;

// 간단하게 자료구조의 개념을 익히는 수준으로 구현해 본다.
// emplace, &&같은 구현은 고려하지 않음.
// 원형 큐로 구현한다.
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
	// 만약 true라면 기존의 값을 밀어버리고 덮어 써버린다. 중간에 작동 변경 못함.
	const OverwriteMode Overwrite = OverwriteMode::FALSE;

public:
	// 성공, 실패 여부를 bool로 반환하겠다.
	bool Push(const Type& _Item)
	{
		// 이미 가득 차 있는지 확인
		if (IsFull())
		{
			// Overwrite Mode가 아니거나 Capacity가 아니면 실패.
			if (Overwrite == OverwriteMode::FALSE || Capacity == 0)
			{
				return false;
			}
			// 제일 오래 된 원소를 덮어써버린다. (맨 처음 원소를 덮어 써버린다.)
			// 기존에 존재하던 것에 소멸자를 호출시키고, 새로 placement new를 사용하는 것도 괜찮은 않은 방안일듯.
			else
			{
				size_t Index = (Pivot + Size) % Capacity;
				Arr[Index] = _Item;
				// 처음 위치가 밀렸으니 Pivot도 증가해야 함.
				Pivot = (Pivot + 1) % Capacity;
				return true;
			}
		}

		// 후 처리.
		size_t Index = (Pivot + Size) % Capacity;
		new(Arr + Index) Type(_Item);
		++Size;
		return true;
	}

	// 처음에 넣은 것 하나 제거.
	bool Pop()
	{
		// 없는 queue에서 원소를 제거할 순 없다.
		if (IsEmpty())
		{
			return false;
		}

		// 맨 처음 것 소멸자 호출 해줘서 지운다.
		std::destroy_at(Arr + Pivot);

		--Size;
		Pivot = (Pivot + 1) % Capacity;

		return true;
	}

	// vector와 다르게 자동 확장은 하지 않을 것.
	bool ReAllocate(size_t _NewCapacity)
	{
		// 기존보다 작아지거나 원소가 삭제되는 위험은 배제한다.
		if (Capacity >= _NewCapacity || Size >= _NewCapacity)
			return false;

		// 새로운 메모리 할당
		Type* NewArr = static_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity));

		// 이미 할당 된 배열이 존재할때
		if (Arr != nullptr)
		{
			// 모두 돌면서 원소를 복제하고, 기존 배열 제거.
			for (size_t i = 0; i < Size; ++i)
			{
				size_t Index = (Pivot + i) % Capacity;
				new(NewArr + i) Type(Utility::Move(Arr[Index]));
				std::destroy_at(Arr + Index);
			}

			::operator delete(Arr);
		}

		// queue 재설정
		Arr = NewArr;
		Pivot = 0;
		Capacity = _NewCapacity;

		return true;
	}

	void Clear()
	{
		// 돌면서 모두 소멸자 호출 시켜준다.
		for (size_t i = 0; i < Size; ++i)
		{
			size_t Index = (Pivot + i) % Capacity;
			std::destroy_at(Arr + Index);
		}

		// 깔끔하게 정리했으면 다시 초기화.
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

	// 보기만 하세요
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