#pragma once
#include <assert.h>
#include <cstddef>
#include <new>
#include <memory>
#include <utility>
#include <stdexcept>

#include "Utility.h"

// ��Ȱ�� ���ø� �ڵ� �ۼ��� ���� using. ���߿� �ּ� ó���Ѵ�.
//using Type = int;

// vector�� �����غ���.
// Ŀ���� Allocator������ ���� �ʴ´�. �Ӹ� ������..
// ���ø��̴� ����� �� �����ھƾ� �Ѵ�.
template <typename Type>
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

	// &&�� ����'��'�� �޴´�.
	template <typename... Types>
	Type& EmplaceBack(Types&&... _Elements)
	{
		if (Size >= Capacity)
		{
			size_t NewCapacitySize = Capacity == 0 ? 1 : Capacity * 2;
			Reserve(NewCapacitySize);
		}

		Type* NewPtr = Arr + Size;

		// PushBack�� �ٸ���. ���ڸ� �״�� ���޹޾Ƽ� �ƿ� ���ο��� ���� �����Ѵ�.
		// perfect forwarding. �׳� ������ ���� �� &&�� �޾�����, �Լ��� ������ �̸��� ���ܹ��ȴ�.
		// �׷��� �Լ����� �ٽ� &&�� �����ϱ� ���� ��.��.��.��.��.�� �Ѵ�. == Forward�� ����.
		new(NewPtr) Type(Utility::Forward<Types>(_Elements)...);
		++Size;

		// ���� �� ��ġ ������ ��ȯ.
		return *NewPtr;
	}
	// �迭 �� �Ҵ��ϴ� �Լ�. ���ο�����, �ܺο����� ���� ���Ŵ�.
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
		--Size; // �ϳ� ������.
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
	bool IsEmpty() const noexcept
	{
		return Size == 0;
	}
	// �̷� �������̽� ������ ���� ������?
	bool IsFull() const noexcept
	{
		return Size == Capacity;
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
		assert(_Index < Size);
		// �� �ε��� �ʰ���.
		if (_Index >= Size)
		{
			throw std::out_of_range("At() : index out of range");
		}

		return Arr[_Index];
	}
	const Type& At(size_t _Index) const
	{
		assert(_Index < Size);
		if (_Index >= Size)
		{
			throw std::out_of_range("At() const : index out of range");
		}

		return Arr[_Index];
	}

	// Heap�� ���� ����� ������.
public:
	// ����Ʈ�� (�θ�, ����, ������)�� ��ȭ. �ڽ��� ����Ʈ������ ��� Heap ���¿��� ���� �۵��Ѵ�.
		// �κ� Ʈ������ �����ϱ� ���� ����Ʈ���� Index�� �޴´�.
	template <typename Compare = std::less<Type>>
	void HeapifyDown(size_t _Index, Compare _Comp = Compare{})
	{
		// �迭 ������ 1�� ������ �迭�� �̹� ���� �Ǿ��ٰ� �� �� �ִ�.
		if (Size < 2)
		{
			return;
		}

		// �翬�� Size�� �Ѵ� Index�� ������ �ȵȴ�.
		assert(_Index < Size);

		while (true)
		{
			size_t Best = _Index;
			size_t LeftChild = HeapLeftChild(_Index);
			size_t RightChild = HeapRightChild(_Index);

			// Size�� ���� �ʰ� _Comp�� �����ϴ� �ڽ��� �ִ°�?
			if (LeftChild < Size && _Comp(Arr[Best], Arr[LeftChild]))
			{
				Best = LeftChild;
			}
			if (RightChild < Size && _Comp(Arr[Best], Arr[RightChild]))
			{
				Best = RightChild;
			}

			// ���ٸ� ��.
			if (Best == _Index)
			{
				break;
			}

			// �ִٸ� ���� ���� �ٲ۴�.
			Utility::Swap(Arr[Best], Arr[_Index]);
			// �ٲ� �ڽ����� �ٽ� Heapify.
			_Index = Best;
		}
	}

	// �迭�� ������ Heap���� ����� �ش�.
	template <typename Compare = std::less<Type>>
	void BuildHeap(Compare _Comp = Compare{})
	{
		if (Size < 2)
		{
			return;
		}

		// _Size / 2 ������ ���ҵ鸸�� �׻� �ڽ��� ������. �ڽ��� ���� ����Ʈ���� �̹� ���ĵ� ����Ʈ��. 
		for (size_t i = Size / 2; i > 0; --i)
		{
			// ���� ������ Heap�� ������Ų��.
			HeapifyDown(i - 1, _Comp);
		}
	}

	// �� ������ ������ �ʰ� �����Ѵ�.
	// �迭�� �̹� �� ������� �����Ѵ�.
	// �翬�� _Comp�� ��ȭ �Ҷ��� ���� �Լ� ������� ����ڰ� �־�� �Ѵ�.
	template <typename Compare = std::less<Type>>
	void HeapPush(const Type& _Item, Compare _Comp = Compare{})
	{		
		PushBack(_Item);

		size_t CurIndex = Size - 1;

		while (CurIndex != 0)
		{
			size_t Parent = HeapParent(CurIndex);
			if (_Comp(Arr[Parent], Arr[CurIndex]))
			{
				Utility::Swap(Arr[Parent], Arr[CurIndex]);
				CurIndex = Parent;
			}
			else
			{
				break;
			}
		}
	}

	template <typename Compare = std::less<Type>>
	void HeapPush(Type&& _Item, Compare _Comp = Compare{})
	{
		PushBack(Utility::Forward<Type>(_Item));

		size_t CurIndex = Size - 1;

		while (CurIndex != 0)
		{
			size_t Parent = HeapParent(CurIndex);
			if (_Comp(Arr[Parent], Arr[CurIndex]))
			{
				Utility::Swap(Arr[Parent], Arr[CurIndex]);
				CurIndex = Parent;
			}
			else
			{
				break;
			}
		}
	}

	// �ϳ� ����.
	template <typename Compare = std::less<Type>>
	void HeapPop(Compare _Comp = Compare{})
	{
		Utility::Swap(Arr[0], Arr[Size - 1]);
		PopBack();

		HeapifyDown(0, _Comp);
	}

private:
	static size_t HeapParent(size_t _Index) noexcept
	{
		// _Index�� 0�� �ƴϾ�� �Ѵ�. ������ ���̰ڴ�.
		assert(_Index);
		return (_Index - 1) / 2;
	}

	static size_t HeapLeftChild(size_t _Index) noexcept
	{
		return _Index * 2 + 1;
	}

	static size_t HeapRightChild(size_t _Index) noexcept
	{
		return _Index * 2 + 2;
	}
};