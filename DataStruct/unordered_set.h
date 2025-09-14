#pragma once
#include "Utility.h"
#include "vector.h"

using Type = int;

// 해쉬의 개념을 구현하기 위한 unordered_set.
//template <typename Type>
class unordered_set
{
public:
	unordered_set() = default;
	~unordered_set() = default;

	unordered_set(const unordered_set& _Other) = delete;
	unordered_set(unordered_set&& _Other) = delete;

public:
	unordered_set& operator= (const unordered_set& _Other) = delete;
	unordered_set& operator= (unordered_set&& _Other) = delete;

private:
	// 기본적으로 들어오는 값을 저장할 버킷(배열)이 필요할 것.
	// 해시 충돌은 Chaining으로 해결해 본다.
	// 원소가 존재 하는지 접근하기 전 확인해야 한다. 별도의 데이터로 관리 해야 할 것 같다. => 비트 마스킹으로 64구간씩 체크 해본다.

	Type* Bucket = nullptr;
	vector<vector<Type>> Chain = {};
	size_t* Check = {};

	size_t Size = 0;
	size_t Capacity = 0;

	size_t(*HashFunc)(const Type&) = [](const Type& _Item) { return static_cast<size_t>(_Item); };

public:
	// 계산 편의를 위해 크기는 2의 제곱수, 최소 64부터 한다.
	bool ReAllocate(size_t _NewCapacity)
	{
		if (_NewCapacity < Capacity || _NewCapacity < Size || _NewCapacity < 64)
			return false;

		size_t NewCapacity = 64;
		if (_NewCapacity <= NewCapacity)
			NewCapacity <<= 1;

		Type* OldBucket = Bucket;
		size_t* OldCheck = Check;

		// 새로운 메모리 할당.
		Bucket = static_cast<Type*>(::operator new(sizeof(Type) * NewCapacity));
		Check = static_cast<size_t*>(::operator new (sizeof(size_t) * (NewCapacity >> 4)));

		if (OldCheck != nullptr)
		{
			size_t CheckBit = Capacity >> 4;

			for (size_t i = 0; i < CheckBit; ++i)
			{
				for (size_t j = 1, Loop = 0; Loop < sizeof(size_t); j <<= 1, ++Loop)
				{
					if (j & OldCheck[CheckBit])
					{
						// TODO. 새로운 위치로 복사하는 코드.

						OldBucket[(CheckBit << 4) + Loop];
					}
				}
			}
			
		}

	}

	size_t GetHash(const Type& _Item)
	{
		return (Capacity - 1) & HashFunc(_Item);
	}

	bool Insert()
	{

	}

};
