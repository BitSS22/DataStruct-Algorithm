#pragma once
#include "Utility.h"
#include "vector.h"

// 필요한거 정리
// raw 메모리 풀.
// Size.
// Capacity
// hash collision 처리 (체이닝)
// equal 동등성 비교
// Hash 함수.

using Type = int;
using HashFunc = std::hash<Type>;
using Equal = std::equal_to<void>;

// 해쉬의 개념을 구현하기 위한 unordered_set.
//template <typename Type, typename HashFunc = std::hash<Type>, typename Equal = std::equal_to<void>>
class unordered_set
{
	enum class State : __int8
	{
		Empty, Occupied, Deleted
	};

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
	// 해시 충돌은 선형탐사로 해결해 본다.
	// 원소가 존재 하는지 접근하기 전 확인해야 한다. 별도의 데이터로 관리 해야 할 것 같다. => 비트 마스킹으로 64구간씩 체크 해본다.

	Type* Buckets = nullptr;
	State* States = nullptr;

	size_t Size = 0;
	size_t Capacity = 0;

	HashFunc Hash = std::hash<Type>{};
	Equal Eq = std::equal_to<void>{};

public:
	void ReAllocate(size_t _NewCapacity)
	{
		if (_NewCapacity <= Capacity)
			return;

		Type* NewBucket = nullptr;

		if constexpr (alignof(Type) < alignof(std::max_align_t))
		{
			NewBucket = static_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity));
		}
		else
		{
			NewBucket = static_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity, std::align_val_t(alignof(Type))));
		}
		

	}

private:
	void ReHash(Type* _Table, size_t _Size)
	{

	}
};
