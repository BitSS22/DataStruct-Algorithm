#pragma once
#include <assert.h>
#include <cstddef>
#include <new>
#include <memory>
#include <utility>

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
			// �ȿ� �޸� �� ������.
			Clear();

			// �޸� ���� ����.
			if constexpr (alignof(Type) > alignof(std::max_align_t))
			{
				::operator delete(Arr, std::align_val_t(alignof(Type)));
			}
			else
			{
				::operator delete(Arr);
			}

			// �ʱ� ������ �о������.
			Arr = nullptr;
			Capacity = 0;
		}
	}

	// �ڵ� �ۼ��߿� ���� ������ �ʴ� ���糪 �̵��� Ȯ���ϱ� ���ؼ�.
	vector(const vector& _Other) = delete;
	vector(vector&& _Other) = delete;

public:
	vector& operator= (const vector& _Other) = delete;
	vector& operator= (vector&& _Other) = delete;

	// [] operator, &�� ��ȯ�Ѵ�.
	// ���ܸ� ������ �ʴ´�.
	Type& operator[](size_t _Index) noexcept
	{
		// debug������ �����ϴ� �˻�
		assert(_Index < Size);
		return Arr[_Index];
	}
	// const& �����ε�.
	const Type& operator[](size_t _Index) const noexcept
	{
		assert(_Index < Size);
		return Arr[_Index];
	}

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
		// �����÷ο� ����.
		if (_NewCapacity > SIZE_MAX / sizeof(Type))
			return;

		// ũ�⸦ �ø����� ȣ���ض�. �ƴϸ� �θ��� ��.
		if (Capacity >= _NewCapacity)
			return;

		// �ܺο��� Reserve ȣ���ߴµ�, ���� Size���� �۰ų� ������ ����.
		if (Size >= _NewCapacity)
			return;

		// �ϴ� ������ ���� �ϳ� �����.
		Type* NewArr = nullptr;

		// alignof(Type) �̰� Ÿ���� ����� ���̾�. (ĳ�� ���� ����)
		// �ִ� ����� ũ�⺸�� ũ��?
		// if constexpr == ������ Ÿ�� �б� ó��.
		if constexpr (alignof(Type) > alignof(std::max_align_t))
		{
			// std::nothrow ���� ������ ���� nullptr ��.
			// ũ�� align_val_t�� ����� ����� ���� new�� ����Ѵ�.
			// std::align_val_t(alignof(Type)) Type�� align ������ŭ �����ؾ� ��. (���������)
			NewArr = static_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity, std::align_val_t(alignof(Type))));
		}
		else
		{
			// �ƴϸ� �׳� new ��.
			NewArr = static_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity));
		}
		
		// ���ݱ��� � �Ű��?
		size_t i = 0;
		// ���ܰ� �����ž�?
		try
		{
			while (i < Size)
			{
				// ���ο� ��ġ�� �̵� (R-Value�� noexcept�� �ƴ϶�� copy)
				new(NewArr + i) Type(std::move_if_noexcept(Arr[i]));
				++i;
			}
		}
		catch (...) // ���� �� ���� �߻�!
		{
			// ����Ȱ� �ٽ� �� ����
			for (size_t j = 0; j < i; ++j)
			{
				std::destroy_at(NewArr + j);
			}
			
			// ���� �Ҵ��� �޸𸮵� ����.
			if constexpr (alignof(Type) > alignof(std::max_align_t))
			{
				::operator delete(NewArr, std::align_val_t(alignof(Type)));
			}
			else
			{
				::operator delete(NewArr);
			}

			// ���� ���� ������.
			throw;
		}

		// ���� ������ ���� ����. == ���� �߻��� ���� �ȵ�.
		for (size_t j = 0; j < Size; ++j)
		{
			// ���� �迭�� ��ҵ��� �Ҹ��� ȣ����Ѽ� �����Ѵ�.
			std::destroy_at(Arr + j);
		}

		// Arr �޸� ��ȯ.
		// new ó�� Type�� ����ο� ���� ������ Ÿ�� �б� ó��.
		if constexpr (alignof(Type) > alignof(std::max_align_t))
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

	// �� �ڿ��� �ϳ� ������
	void PopBack() noexcept
	{
		// Size 0���� Ŀ�� ��.
		assert(Size);

		// �� �������� �ϳ� �Ҹ��� ȣ��.
		std::destroy_at(Arr + (Size - 1));
	}

	// �ȿ� �ִ°� �� ������.
	void Clear() noexcept
	{
		// �Ҹ��� ȣ�� ����.
		for (size_t i = 0; i < Size; ++i)
		{
			std::destroy_at(Arr + i);
		}

		// �Ҵ� �� �޸𸮴� �״��.
		Size = 0;
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
	
	// GetData�� �����ε� �Ѵ�.
	// const vector������ �Ʒ� ���� ȣ��ż� ���� �Ұ���. �׳� vector������ ���� ���� ȣ��ȴ�.
	Type* GetData() noexcept
	{
		return Arr;
	}
	const Type* GetData() const noexcept
	{
		return Arr;
	}

	// ���� �ִ°� ��.
	Type& Back() noexcept
	{
		// assert�� debug ���忡����. release���� ȣ���ϸ� ���ǵ��� ���� �������� ū ���� ��� ����.
		// �ٵ� std::vector�� ������ �̷���.
		assert(Size);
		return Arr[Size - 1];
	}
	const Type& Back() const noexcept
	{
		assert(Size);
		return Arr[Size - 1];
	}

	// operator[] �� ���� ū ��������, �ε��� ������ out_of_range exception�� �߻��ż� ���ĵ�.
	// �׷��� At �Լ��� noexcept�� �ƴϴ�.
	Type& At(size_t _Index)
	{
		// �׷��� debug���� �˻� �ؾ���.
		assert(Size);
		return Arr[_Index];
	}
	const Type& At(size_t _Index) const
	{
		assert(Size);
		return Arr[_Index];
	}
};