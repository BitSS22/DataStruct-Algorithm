#pragma once
#include <assert.h>
#include <utility>
#include <memory>

#include "Utility.h"

// 마찬가지로 원활한 템플릿 작성을 위한 별칭.
using Type = int;

// 리스트 구현한다. 노드 형식으로 서로를 연결해서 알고 있는 자료구조.
// 순회 등에서 최적화의 여지를 생각해 본다면 한번 크기를 할당할때, 몇개 정도 분량의 적절한 사이즈를 땡겨서 미리 할당하고,
// 그 자리에 생성한다면 캐시 적중률이나 속도면에서 더 이득을 볼 수도 있지 않을까.

//template<typename Type>
class list
{
private:
	// list는 노드 형식이고, 외부에 그걸 알려줄 필요는 없다.
	// 그래서 inner class로 Node를 만든다.
	class node
	{
	private:
		// 칭구
		friend class list;
		node(){}
		// 편하게 쓰고 싶으니까 생성자 오버로딩.
		node(const Type& _Item)
			: Data(_Item) {}
		// R-Value도 만들어 둔다. 인자로 들어오면서 이름 생겼으니 Move로 R-Value Cast.
		node(Type&& _Item) noexcept
			: Data(Utility::Move(_Item)) {}

		// list.Emplace를 위한 node 생성자.
		// explicit == 암시적 형번환 (타입 추론)하지 마라.
		struct EmplaceTag {};
		// Tag용 구조체. 내부에서 쓰지도 않을거, 이름도 지을 필요 없다.
		template <typename... Types>
		explicit node(EmplaceTag, Types&&... _Items)
			: Data(Utility::Forward<Types>(_Items)...) {}

		~node()
		{
			// 메모리를 0으로 밀고 퇴장.
			Prev = nullptr;
			Next = nullptr;
		}

		// 코드 작성중에 내가 원하지 않는 복사나 이동을 확인하기 위해서.
		node(const node& _Other) = delete;
		node(node&& _Other) = delete;
		node& operator= (const node& _Other) = delete;
		node& operator= (node&& _Other) = delete;

	private:
		// 이전 노드, 다음 노드를 가진다.
		// 그리고 저장할 데이터도 가진다.
		// 만약 Size == 0 일때도 Prev, Next에 DummyNode를 생성한다면
		// 연결, 제거 등에서 if (node != nullptr) 같은 조건식이 필요 없을 것.
		node* Prev = nullptr;
		node* Next = nullptr;
		Type Data = {};

	private:
		// Prev와 Next로 양 node를 연결해주는 함수.
		// 인자 nullptr 들어오면 터진다. nullptr 체크 하고 써라.
		// 내부에서 쓸 함수니 위험해도 괜찮다. 내가 알아서 쓴다.
		// 그냥 두개 연결할거니 static으로 만들면 뭐 연결하던 쓸 수 있겠지.
		static void Connect(node* _Prev, node* _Next) noexcept
		{
			assert(_Prev && _Next);
			_Prev->Next = _Next;
			_Next->Prev = _Prev;
		}

		// 이 노드 깔끔하게 떼줘.
		// 당연히 nullptr 들어오면 터진다.
		// 중간 노드만 떼니, Head, Tail node일 때는 따로 연결 처리 해라.
		static void Detach(node* _DetachNode) noexcept
		{
			assert(_DetachNode);
			node* PrevNode = _DetachNode->Prev;
			node* NextNode = _DetachNode->Next;

			if (nullptr != PrevNode)
			{
				PrevNode->Next = NextNode;
			}
			if (nullptr != NextNode)
			{
				NextNode->Prev = PrevNode;
			}

			_DetachNode->Prev = nullptr;
			_DetachNode->Next = nullptr;
		}

	};

public:
	list() {}
	~list()
	{
		// 너 가기 전에 싹 밀어줘.
		Clear();
	}

	// 코드 작성중에 내가 원하지 않는 복사나 이동을 확인하기 위해서.
	list(const list& _Other) = delete;
	list(list&& _Other) = delete;

public:
	list& operator= (const list& _Other) = delete;
	list& operator= (list&& _Other) = delete;

private:
	// list는 뭘 가져야 할까. 양 끝의 노드와 size를 기억하자.
	node* Head = nullptr;
	node* Tail = nullptr;
	size_t Size = 0;

public:
	// 맨 끝에 넣자.
	void PushBack(const Type& _Item)
	{
		// 일단 새로운 노드 하나 만들자.
		node* NewNode = new node(_Item);

		// 비어있니?
		if (IsEmpty())
		{
			// 비어있으면 Head, Tail이 nullptr. 그러니 Head, Tail만 이 노드로 연결해주고 끝난다.
			Head = NewNode;
		}
		else
		{
			// 안 비어있다면 Tail에 무슨 노드가 있을것이다.
			// 두개 연결해 준다.
			node::Connect(Tail, NewNode);
		}

		// 맨 끝에 오니 어차피 해야 한다.
		Tail = NewNode;
		++Size;
	}

	// R-Value Version
	void PushBack(Type&& _Item)
	{
		// noexcept 일때만 이동시켜줘.
		node* NewNode = new node(std::move_if_noexcept(_Item));

		// 비어있니?
		if (IsEmpty())
		{
			// 비어있으면 Head, Tail이 nullptr. 그러니 Head, Tail만 이 노드로 연결해주고 끝난다.
			Head = NewNode;
		}
		else
		{
			// 안 비어있다면 Tail에 무슨 노드가 있을것이다.
			// 두개 연결해 준다.
			node::Connect(Tail, NewNode);
		}

		// 맨 끝에 오니 어차피 해야 한다.
		Tail = NewNode;
		++Size;
	}

	template <typename... Types>
	Type& EmplaceBack(Types&&... _Item)
	{
		// 새로 만들어줘 -> 인자 그대로 전달해서.
		// node::EmplaceTag 빈 객체 하나 Tag로 쓴다.
		node* NewNode = new node(node::EmplaceTag(), Utility::Forward<Types>(_Item)...);

		// 비어있니?
		if (IsEmpty())
		{
			Head = NewNode;
		}
		else
		{
			node::Connect(Tail, NewNode);
		}

		Tail = NewNode;
		++Size;

		// Data의 참조 반환.
		return NewNode->Data;
	}

	// PushFront는 Head, Tail만 바꿔주면 된다.
	void PushFront(const Type& _Item)
	{
		node* NewNode = new node(_Item);

		if (IsEmpty())
		{
			Tail = NewNode;
		}
		else
		{
			node::Connect(NewNode, Head);
		}

		Head = NewNode;
		++Size;
	}

	void PushFront(Type&& _Item)
	{
		node* NewNode = new node(std::move_if_noexcept(_Item));

		if (IsEmpty())
		{
			Tail = NewNode;
		}
		else
		{
			node::Connect(NewNode, Head);
		}

		Head = NewNode;
		++Size;
	}

	template <typename... Types>
	Type& EmplaceFront(Types&&... _Item)
	{
		node* NewNode = new node(node::EmplaceTag(), Utility::Forward<Types>(_Item)...);

		if (IsEmpty())
		{
			Tail = NewNode;
		}
		else
		{
			node::Connect(NewNode, Head);
		}

		Head = NewNode;
		++Size;

		return NewNode->Data;
	}

	// 맨 뒤에꺼 하나 빼줘.
	void PopBack()
	{
		// size 0이면 그냥 죽어.
		assert(Size);

		// Tail 노드를 지울거다.
		node* DeleteNode = Tail;
		Tail = Tail->Prev;
		// 연결 끊어줘.
		node::Detach(DeleteNode);

		// 이제 죽어.
		delete DeleteNode;

		--Size;

		// Size가 0이 됐다면?
		if (IsEmpty())
		{
			// 이거 안하면 Tail Node == nullptr; Size == 0; 인데 Head만 지워진 어딘가를 가리킬것.
			Head = nullptr;
		}
	}

	// PopFront는 PopBack의 반대.
	void PopFront()
	{
		assert(Size);

		node* DeleteNode = Head;
		Head = Head->Next;
		node::Detach(DeleteNode);
		delete DeleteNode;

		--Size;

		if (IsEmpty())
		{
			Tail = nullptr;
		}
	}

	// 모든 node를 찾으면서 순회해야 한다.
	void Clear() noexcept
	{
		// 비어 있으면 할 일도 없다.
		// 체크하지 않아도 Head == nullptr일테니 동작에 문제는 없다.
		if (IsEmpty())
			return;

		// 맨 처음부터 돌겠다.
		node* CurrentNode = Head;

		// Head == nullptr이라면 끝까지 왔다는 뜻.
		while (nullptr != CurrentNode)
		{
			// 다음 노드의 주소를 미리 받는다.
			node* NextNode = CurrentNode->Next;
			// 지워줘. 지운 다음에 Next를 참조하면 당연히 터진다.
			delete CurrentNode;
			// 받아둔 노드 주소로 갱신.
			CurrentNode = NextNode;
		}

		// 다 지웠어? 데이터 깔끔하게 밀어줘.
		Head = nullptr;
		Tail = nullptr;
		Size = 0;
	}

public:
	// 이미 구현한 vector에서 가져와야겠다. 어차피 거기서 거긴데.
	size_t GetSize() const noexcept
	{
		return Size;
	}
	bool IsEmpty() const noexcept
	{
		return Size == 0;
	}
	// Back, Front는 결국 Tail과 Head.
	Type& Back() noexcept
	{
		// 끝에 뭐 없으면 터트려.
		assert(Tail);
		return Tail->Data;
	}
	const Type& Back() const noexcept
	{
		assert(Tail);
		return Tail->Data;
	}
	Type& Front() noexcept
	{
		// 끝에 뭐 없으면 터트려.
		assert(Head);
		return Head->Data;
	}
	const Type& Front() const noexcept
	{
		assert(Head);
		return Head->Data;
	}
};