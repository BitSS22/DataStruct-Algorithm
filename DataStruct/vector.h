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
	~vector()
	{
		// Arr�� nullptr�� �ƴϸ� �޸� ����.
		if (nullptr != Arr)
		{
			// �Ҹ��� ȣ�� ����.
			for (size_t i = 0; i < Size; ++i)
			{
				std::destroy_at(Arr + i);
			}

			// �޸� ���� ����.
			if constexpr (alignof(Type) > alignof(max_align_t))
			{
				::operator delete(Arr, std::align_val_t(alignof(Type)));
			}
			else
			{
				::operator delete(Arr);
			}
		}
	}

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
		// move_if_noexcept Type�� R-Value constructor�� noexcept��?
		new(Arr + Size) Type(std::move_if_noexcept(_Item));
		++Size;
	}

	void Reserve(size_t _NewCapacity)
	{
		// ũ�⸦ �ø����� ȣ���ض�. �ƴϸ� �θ��� ��.
		if (Capacity >= _NewCapacity)
			return;

		// �ϴ� ������ ���� �ϳ� �����.
		Type* NewArr = nullptr;

		// alignof(Type) �̰� Ÿ���� ����� ���̾�. (ĳ�� ���� ����)
		// �ִ� ����� ũ�⺸�� ũ��?
		// if constexpr == ������ Ÿ�� �б� ó��.
		if constexpr (alignof(Type) > alignof(max_align_t))
		{
			// std::nothrow ���� ������ ���� nullptr ��.
			// ũ�� align_val_t�� ����� ����� ���� new�� ����Ѵ�.
			// std::align_val_t(alignof(Type)) Type�� align ������ŭ �����ؾ� ��. (���������)
			NewArr = reinterpret_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity, std::align_val_t(alignof(Type)), std::nothrow));
		}
		else
		{
			// �ƴϸ� �׳� new ��.
			NewArr = reinterpret_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity, std::nothrow));
		}
		
		// NewArr == nullptr�̸� �Ҵ� ����.
		if (!NewArr)
		{
			// ���� �̻���. �ϴ� return. ���� ó�� ���Ŀ�. TODO.
			return;
		}

		// ���� Arr�� ��Ҹ� �̵���Ų��.
		for (size_t i = 0; i < Size; ++i)
		{
			// ���� ��ġ�� ��������. �̵� ������ ������ ȣ������. noexcept�϶���.
			new(NewArr + i) Type(std::move_if_noexcept(Arr[i]));
			// ���� �迭�� ��ҵ��� �Ҹ��� ȣ����Ѽ� �����Ѵ�.
			std::destroy_at(Arr + i);
		}

		// Arr �޸� ��ȯ.
		// new ó�� Type�� ����ο� ���� ������ Ÿ�� �б� ó��.
		if constexpr (alignof(Type) > alignof(max_align_t))
		{
			::operator delete(Arr, std::align_val_t(alignof(Type)));
		}
		else
		{
			::operator delete(Arr);
		}
		
		// �迭 �ּҿ� Capacity ����.
		Arr = NewArr;
		Capacity = _NewCapacity;
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