#pragma once
#include <assert.h>

#include "Utility.h"

// using Type = int;

template<typename Type>
class list
{
	// 이전 노드, 다음 노드, 값을 가진 구조체.
	// 외부에서 이런 구조체의 존재를 알 필요 없다. 이너클래스.
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
		// 명시적으로 Forward 생성자 호출.
		template<typename... Types>
		Node(Emplace_Flag, Types&&... _Items)
			: Data(Utility::Forward<Types>(_Items)...) {}

		Node* Prev = nullptr;
		Node* Next = nullptr;
		Type Data = {};

	};

public:
	// Iterator도 구현해본다.
	// ifdef로 디버깅 처리를 분기해서, 릴리즈 빌드시 오버헤드 없이 단순한 포인터처럼 작동.
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

		// 결국 iterator 구현에서 중요한건 operator?
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
		// Debug 빌드일때는 Iterator의 유효성을 검사할 것.
#ifdef _DEBUG
		const list* Owner = nullptr;
		size_t FixLevel = 0;
#endif
#ifdef _DEBUG
	private:
		void BasicInvalidCheck()
		{
			// ptr이 nullptr이니?
			assert(ptr);
			// 주인이 nullptr이니?
			assert(Owner);
			// 주인이 이동, 복사, 삭제등으로 바뀌었니?
			assert(Owner->Dummy);
			// 변경 된 list니?
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
	// Sentinel List를 만들거다. 더미 노드를 하나 두고 코드의 분기처리를 줄이고 통일성을 높인다.
	// Dummy Node를 하나를 두고 원형으로 이어진 구조로 만든다.
	list()
	{
		// 더미 노드를 만들고, 자기 자신을 가르키게 만든다.
		Dummy = new Node();
		Dummy->Prev = Dummy;
		Dummy->Next = Dummy;
	}
	~list()
	{
		// 있는 노드 싹다 지운다.
		Clear();

		// 더미 노드도 삭제한다.
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
	// debug일때는 변경 된 list인지 체크할 것이다.
#ifdef _DEBUG
	size_t FixLevel = 0;
#endif

public:
	void PushBack(const Type& _Item)
	{
		Node* NewNode = new Node(_Item);

		// Dummy.Prev는 Tail.
		// 이전 PrevNode를 받아둔다.
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

		// 디버깅을 위한 값 밀어버리기.
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

	// 중간 삽입.(_Where 뒤쪽에 삽입한다.)
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
		// 삽입한 노드를 가르키는 Iterator return.
		return Iterator(NewNode, this);
#else
		return Iterator(NewNode);
#endif
	}
	
	// 지운 다음 노드를 반환.
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

		// 기본 값으로 초기화 해준다.
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
	// Begin은 첫번째 노드를 반환.
	Iterator Begin() const noexcept
	{
#ifdef _DEBUG
		return Iterator(Dummy->Next, this);
#else
		return Iterator(Dummy->Next);
#endif
	}
	// End는 Dummy를 반환한다.
	Iterator End() const noexcept
	{
#ifdef _DEBUG
		return Iterator(Dummy, this);
#else
		return Iterator(Dummy);
#endif
	}

};