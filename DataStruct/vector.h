#pragma once
#include <assert.h>

// ��Ȱ�� ���ø� �ڵ� �ۼ��� ���� using. ���߿� �ּ� ó���Ѵ�.
using Type = int;

// vector�� �����غ���.
// Ŀ���� Allocator������ ���� �ʴ´�. �Ӹ� ������..
// ���ø��̴� ����� �� �����ھƾ� �Ѵ�.
//template <typename Type>
class vector
{
public:
	vector() {}
	~vector() {}

	// �ڵ� �ۼ��߿� ���� ������ �ʴ� ���糪 �̵��� Ȯ���ϱ� ���ؼ�.
	vector(const vector& _Other) = delete;
	vector(vector&& _Other) = delete;
	vector& operator= (const vector& _Other) = delete;
	vector& operator= (vector&& _Other) = delete;

private:
	// �ʿ��Ѱ� �����Ҵ� ���� �޸� ��ġ, ������Ʈ ����, �޸��� ũ��.
	Type* Arr = nullptr;
	size_t Size = 0;
	size_t Capacity = 0;

public:
	// �ܺο��� ȣ���� �� �ִ� �Լ���.
	// PushFront���� �Լ��� �� ����Ŵ�. �׷��Ÿ� vector ���� ����.
	void PushBack(const Type& _Item)
	{
		// Size�� Capacity���� ũ�ų� ������ �Ҵ� �� �޸𸮰� ������ ��.
		if (Size >= Capacity)
		{
			// �� �Ҵ��� ����� ���Ѵ�. 2�辿 �ø���.
			// Size�� 0�̸� 0 * 2 == 0 �̹Ƿ� 0�϶��� 1�� ���Ѵ�.
			size_t NewCapacitySize = Capacity == 0 ? 1 : Capacity * 2;
			Reserve(NewCapacitySize);
		}

		// ���� ������ ������ ���� ���̴�. ������Ʈ �߰� �� ������ �÷��ش�.
		// �̸� �Ҵ��� �� ������ placement new�� �Ѵ�.
		new(Arr + Size) Type(_Item);
		++Size;
	}

	// R-Value ���� �����ε�
	void PushBack(Type&& _Item)
	{
		if (Size >= Capacity)
		{
			size_t NewCapacitySize = Capacity == 0 ? 1 : Capacity * 2;
			Reserve(NewCapacitySize);
		}

		// _Item�� move�� R-Value ĳ���� �ϴ� �κ��� �ٸ�.
		new(Arr + Size) Type(std::move(_Item));
		++Size;
	}

	void Reserve(size_t _NewCapacity)
	{
		// ũ�⸦ �ø����� ȣ���ض�.
		if (Capacity >= _NewCapacity)
			return;

		// ���ο� �޸� ����'��' �Ҵ� �ϰڴ�. _NewCapacity��ŭ.
		Type* NewArr = reinterpret_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity));
	}

private:
	// ���ο����� �� �Լ�.
	

public:
	// ������ Get, Set �Լ���.
	// ���� ���� Ŭ������ �Լ� ���� �� �ڵ� ���Ĵٵ带 ������.
	size_t GetSize() const noexcept
	{
		return Size;
	}
	size_t GetCapacity() const noexcept
	{
		return Capacity;
	}
	Type* GetData() const noexcept
	{
		return Arr;
	}

};