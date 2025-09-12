#pragma once
#include <assert.h>

#include "Utility.h"

// using Type = int;

template<typename Type>
class list
{
	// ���� ���, ���� ���, ���� ���� ����ü.
	// �ܺο��� �̷� ����ü�� ���縦 �� �ʿ� ����. �̳�Ŭ����.
	struct Node
	{
		struct Emplace_Flag{};

		friend class list;
	private:
		Node() {}
		Node(const Type& _Item)
			: Data(_Item) {}
		Node(Type&& _Item)
			: Data(Utility::Move(_Item)) {}
		// ��������� Forward ������ ȣ��.
		template<typename... Types>
		Node(Emplace_Flag, Types&&... _Items)
			: Data(Utility::Forward<Types>(_Items)...) {}

		Node* Prev = nullptr;
		Node* Next = nullptr;
		Type Data = {};

	};

public:
	// Iterator�� �����غ���.
	// ifdef�� ����� ó���� �б��ؼ�, ������ ����� ������� ���� �ܼ��� ������ó�� �۵�.
	class Iterator
	{
		friend class list;
	public:
		Iterator() {}
#ifdef _DEBUG
		Iterator(Node* _Node, const list* _Owner)
			: ptr(_Node)
			, Owner(_Owner)
			, FixLevel(_Owner->FixLevel){}
#else
		Iterator(Node* _Node)
			: ptr(_Node) {}
#endif
		~Iterator() {}

		Iterator(const Iterator& _Other) = default;
		Iterator(Iterator&& _Other) = default;

	public:
		Iterator& operator= (const Iterator& _Other) = default;
		Iterator& operator= (Iterator&& _Other) = default;

		// �ᱹ iterator �������� �߿��Ѱ� operator?
		Type& operator*() noexcept
		{
#ifdef  _DEBUG
			BasicInvalidCheck();
			assert(ptr != Owner->Dummy);
#endif
			return ptr->Data;
		}

		bool operator== (Iterator _Other) const noexcept
		{
			return ptr == _Other.ptr;
		}
		bool operator!= (Iterator _Other) const noexcept
		{
			return !(*this == _Other);
		}
		Iterator& operator++()
		{
#ifdef _DEBUG
			IncrementInvalidCheck();
#endif
			ptr = ptr->Next;
			return *this;
		}
		Iterator operator++(int)
		{
#ifdef _DEBUG
			IncrementInvalidCheck();
#endif
			Iterator ReturnIter = *this;
			ptr = ptr->Next;
			return ReturnIter;
		}
		Iterator& operator--()
		{
#ifdef _DEBUG
			DecrementInvalidCheck();
#endif
			ptr = ptr->Prev;
			return *this;
		}
		Iterator operator--(int)
		{
#ifdef _DEBUG
			DecrementInvalidCheck();
#endif
			Iterator RetrunIter = *this;
			ptr = ptr->Prev;
			return RetrunIter;
		}

	private:
		Node* ptr = nullptr;
		// Debug �����϶��� Iterator�� ��ȿ���� �˻��� ��.
#ifdef _DEBUG
		const list* Owner = nullptr;
		size_t FixLevel = 0;
#endif
#ifdef _DEBUG
	private:
		void BasicInvalidCheck()
		{
			// ptr�� nullptr�̴�?
			assert(ptr);
			// ������ nullptr�̴�?
			assert(Owner);
			// ������ �̵�, ����, ���������� �ٲ����?
			assert(Owner->Dummy);
			// ���� �� list��?
			assert(FixLevel == Owner->FixLevel);
		}
		void IncrementInvalidCheck()
		{
			BasicInvalidCheck();

			assert(ptr != Owner->Dummy);
		}
		void DecrementInvalidCheck()
		{
			BasicInvalidCheck();

			assert(ptr->Prev != Owner->Dummy);
		}
#endif
	};

public:
	// Sentinel List�� ����Ŵ�. ���� ��带 �ϳ� �ΰ� �ڵ��� �б�ó���� ���̰� ���ϼ��� ���δ�.
	// Dummy Node�� �ϳ��� �ΰ� �������� �̾��� ������ �����.
	list()
	{
		// ���� ��带 �����, �ڱ� �ڽ��� ����Ű�� �����.
		Dummy = new Node();
		Dummy->Prev = Dummy;
		Dummy->Next = Dummy;
	}
	~list()
	{
		// �ִ� ��� �ϴ� �����.
		Clear();

		// ���� ��嵵 �����Ѵ�.
		if (Dummy != nullptr)
		{
			delete Dummy;
			Dummy = nullptr;
		}
	}

	list(const list& _Other) = delete;
	list(list&& _Other) = delete;

public:
	list& operator= (const list& _Other) = delete;
	list& operator= (list&& _Other) = delete;

private:
	Node* Dummy = nullptr;
	size_t Size = 0;
	// debug�϶��� ���� �� list���� üũ�� ���̴�.
#ifdef _DEBUG
	size_t FixLevel = 0;
#endif

public:
	void PushBack(const Type& _Item)
	{
		Node* NewNode = new Node(_Item);

		// Dummy.Prev�� Tail.
		// ���� PrevNode�� �޾Ƶд�.
		Node* Prev = Dummy->Prev;
		Prev->Next = NewNode;
		Dummy->Prev = NewNode;

		NewNode->Prev = Prev;
		NewNode->Next = Dummy;

		++Size;
#ifdef _DEBUG
		++FixLevel;
#endif
	}
	void PushBack(Type&& _Item)
	{
		Node* NewNode = new Node(Utility::Forward<Type>(_Item));

		Node* Prev = Dummy->Prev;
		Prev->Next = NewNode;
		Dummy->Prev = NewNode;

		NewNode->Prev = Prev;
		NewNode->Next = Dummy;

		++Size;
#ifdef _DEBUG
		++FixLevel;
#endif
	}
	template<typename... Types>
	Type& EmplaceBack(Types&&... _Items)
	{
		Node* NewNode = new Node(Node::Emplace_Flag(), Utility::Forward<Types>(_Items)...);

		Node* Prev = Dummy->Prev;
		Prev->Next = NewNode;
		Dummy->Prev = NewNode;

		NewNode->Prev = Prev;
		NewNode->Next = Dummy;

		++Size;
#ifdef _DEBUG
		++FixLevel;
#endif
		return NewNode->Data;
	}

	void PushFront(const Type& _Item)
	{
		Node* NewNode = new Node(_Item);

		Node* Next = Dummy->Next;
		Next->Prev = NewNode;
		Dummy->Next = NewNode;

		NewNode->Prev = Dummy;
		NewNode->Next = Next;

		++Size;
#ifdef _DEBUG
		++FixLevel;
#endif
	}
	void PushFront(Type&& _Item)
	{
		Node* NewNode = new Node(Utility::Forward<Type>(_Item));

		Node* Next = Dummy->Next;
		Next->Prev = NewNode;
		Dummy->Next = NewNode;

		NewNode->Prev = Dummy;
		NewNode->Next = Next;

		++Size;
#ifdef _DEBUG
		++FixLevel;
#endif
	}
	template<typename... Types>
	Type& EmplaceFront(Types&&... _Items)
	{
		Node* NewNode = new Node(Node::Emplace_Flag(), Utility::Forward<Types>(_Items)...);

		Node* Next = Dummy->Next;
		Next->Prev = NewNode;
		Dummy->Next = NewNode;

		NewNode->Prev = Dummy;
		NewNode->Next = Next;

		++Size;
#ifdef _DEBUG
		++FixLevel;
#endif
		return NewNode->Data;
	}

	void PopBack()
	{
		assert(Dummy->Prev != Dummy);

		Node* DeleteNode = Dummy->Prev;
		DeleteNode->Prev->Next = Dummy;
		Dummy->Prev = DeleteNode->Prev;

		// ������� ���� �� �о������.
		DeleteNode->Prev = nullptr;
		DeleteNode->Next = nullptr;

		delete DeleteNode;

		--Size;
#ifdef _DEBUG
		++FixLevel;
#endif
	}

	void PopFront()
	{
		assert(Dummy->Next != Dummy);

		Node* DeleteNode = Dummy->Next;
		DeleteNode->Next->Prev = Dummy;
		Dummy->Next = DeleteNode->Next;

		DeleteNode->Prev = nullptr;
		DeleteNode->Next = nullptr;

		delete DeleteNode;

		--Size;
#ifdef _DEBUG
		++FixLevel;
#endif
	}

	// �߰� ����.(_Where ���ʿ� �����Ѵ�.)
	Iterator Insert(Iterator _Where, const Type& _Item)
	{
#ifdef _DEBUG
		_Where.BasicInvalidCheck();
#endif
		Node* NewNode = new Node(_Item);

		Node* WNode = _Where.ptr;
		Node* WPrev = WNode->Prev;

		WNode->Prev = NewNode;
		WPrev->Next = NewNode;

		NewNode->Prev = WPrev;
		NewNode->Next = WNode;

		++Size;
#ifdef _DEBUG
		++FixLevel;
		// ������ ��带 ����Ű�� Iterator return.
		return Iterator(NewNode, this);
#else
		return Iterator(NewNode);
#endif
	}
	
	// ���� ���� ��带 ��ȯ.
	Iterator Erase(Iterator _Start, Iterator _End) noexcept
	{
#ifdef _DEBUG
		_Start.BasicInvalidCheck();
		_End.BasicInvalidCheck();
		assert(_Start != End());
		assert(_Start != _End);
#endif
		Node* WNode = _Start.ptr;
		Node* WPrev = WNode->Prev;

		while (WNode != _End.ptr)
		{
			assert(WNode != Dummy);
			Node* DeleteNode = WNode;
			WNode = WNode->Next;
			delete DeleteNode;
			--Size;
		}

		WPrev->Next = WNode;
		WNode->Prev = WPrev;

#ifdef _DEBUG
		++FixLevel;
		return Iterator(WNode, this);
#else
		return Iterator(WNode);
#endif
	}

	Iterator Erase(Iterator _Where) noexcept
	{
		Iterator NextIter = _Where;

		return Erase(_Where, ++NextIter);
	}


	void Clear()
	{
		Node* CurNode = Dummy->Next;
		while (CurNode != Dummy)
		{
			Node* DeleteNode = CurNode;
			CurNode = CurNode->Next;
			delete DeleteNode;
		}

		// �⺻ ������ �ʱ�ȭ ���ش�.
		Dummy->Prev = Dummy;
		Dummy->Next = Dummy;
		Size = 0;
#ifdef _DEBUG
		++FixLevel;
#endif
	}

public:
	size_t GetSize() const
	{
		return Size;
	}

	Type& GetBack() noexcept
	{
		assert(Size);

		return Dummy->Prev->Data;
	}
	const Type& GetBack() const noexcept
	{
		assert(Size);

		return Dummy->Prev->Data;
	}

	Type& GetFront() noexcept
	{
		assert(Size);

		return Dummy->Next->Data;
	}
	const Type& GetFront() const noexcept
	{
		assert(Size);

		return Dummy->Next->Data;
	}
	// Begin�� ù��° ��带 ��ȯ.
	Iterator Begin() const noexcept
	{
#ifdef _DEBUG
		return Iterator(Dummy->Next, this);
#else
		return Iterator(Dummy->Next);
#endif
	}
	// End�� Dummy�� ��ȯ�Ѵ�.
	Iterator End() const noexcept
	{
#ifdef _DEBUG
		return Iterator(Dummy, this);
#else
		return Iterator(Dummy);
#endif
	}

};