#pragma once
#include "Utility.h"
#include "vector.h"

// �ʿ��Ѱ� ����
// raw �޸� Ǯ.
// Size.
// Capacity
// hash collision ó�� (ü�̴�)
// equal ��� ��
// Hash �Լ�.

using Type = int;
using HashFunc = std::hash<Type>;
using Equal = std::equal_to<void>;

// �ؽ��� ������ �����ϱ� ���� unordered_set.
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
	// �⺻������ ������ ���� ������ ��Ŷ(�迭)�� �ʿ��� ��.
	// �ؽ� �浹�� ����Ž��� �ذ��� ����.
	// ���Ұ� ���� �ϴ��� �����ϱ� �� Ȯ���ؾ� �Ѵ�. ������ �����ͷ� ���� �ؾ� �� �� ����. => ��Ʈ ����ŷ���� 64������ üũ �غ���.

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
