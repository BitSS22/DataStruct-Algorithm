#pragma once
#include "Utility.h"
#include "vector.h"

using Type = int;

// �ؽ��� ������ �����ϱ� ���� unordered_set.
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
	// �⺻������ ������ ���� ������ ��Ŷ(�迭)�� �ʿ��� ��.
	// �ؽ� �浹�� Chaining���� �ذ��� ����.
	// ���Ұ� ���� �ϴ��� �����ϱ� �� Ȯ���ؾ� �Ѵ�. ������ �����ͷ� ���� �ؾ� �� �� ����. => ��Ʈ ����ŷ���� 64������ üũ �غ���.

	Type* Bucket = nullptr;
	vector<vector<Type>> Chain = {};
	size_t* Check = {};

	size_t Size = 0;
	size_t Capacity = 0;

	size_t(*HashFunc)(const Type&) = [](const Type& _Item) { return static_cast<size_t>(_Item); };

public:
	// ��� ���Ǹ� ���� ũ��� 2�� ������, �ּ� 64���� �Ѵ�.
	bool ReAllocate(size_t _NewCapacity)
	{
		if (_NewCapacity < Capacity || _NewCapacity < Size || _NewCapacity < 64)
			return false;

		size_t NewCapacity = 64;
		if (_NewCapacity <= NewCapacity)
			NewCapacity <<= 1;

		Type* OldBucket = Bucket;
		size_t* OldCheck = Check;

		// ���ο� �޸� �Ҵ�.
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
						// TODO. ���ο� ��ġ�� �����ϴ� �ڵ�.

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
