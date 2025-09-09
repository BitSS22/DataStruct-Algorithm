#pragma once
#include <assert.h>

#include "Utility.h"

using Type = int;

// template<typename Type>
class list
{
	// 이전 노드, 다음 노드, 값을 가진 구조체.
	// 외부에서 이런 구조체의 존재를 알 필요 없다. 이너클래스.
	struct Node
	{
		struct R_Value_Flag{};

		friend class list;
	private:
		Node() {}
		Node(const Type& _Item)
			: Data(_Item) {}
		Node(const Type& _Item)
			: Data(_Item) {}
		// 명시적으로 R-Value 생성자 호출.
		Node(R_Value_Flag, Type&& _Item)
			: Data(Utility::Move(_Item)) {}

		Node* Prev = nullptr;
		Node* Next= nullptr;
		Type Data = {};

	};

	// Iterator도 구현해본다.
	class Iterator
	{
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
			DereferenceInvalidCheck();
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
			DereferenceInvalidCheck();
#endif
			ptr = ptr->Next;
			return *this;
		}
		Iterator operator++(int)
		{
#ifdef _DEBUG
			DereferenceInvalidCheck();
#endif
			Iterator RetrunIter = *this;
			ptr = ptr->Next;
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
			// 주인이 nullptr이니?
			assert(Owner);
			// 주인이 이동, 복사, 등으로 바뀌었니?
			assert(Owner->Dummy);
			// 변경 된 list니?
			assert(FixLevel == Owner->FixLevel);
		}
		void DereferenceInvalidCheck()
		{
			BasicInvalidCheck();

			// ptr이 nullptr이니?
			assert(ptr);
			// 현재 가리키는게 더미노드니?
			assert(ptr != Owner->Dummy);
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
		Node* NewNode = new Node(Node::R_Value_Flag(), Utility::Forward<Type>(_Item));

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
		Node* NewNode = new Node(Node::R_Value_Flag(), Utility::Forward<Type>(_Item));

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

};