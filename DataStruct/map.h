#pragma once
#include <cassert>
#include <type_traits>

#include "Utility.h"

//using Type1 = int;
//using Type2 = int;
//using Compare = bool(*)(const int&, const int&);

template <typename Type1, typename Type2, typename Compare = std::less<Type1>>
class map
{
public:
	friend class Iterator;
	struct Pair
	{
	public:
		// 기본 생성자, 소멸자는 명시적으로 default 선언.
		Pair() = default;
		Pair(const Pair&) = default;
		Pair(Pair&&) noexcept = default;
		~Pair() = default;

	public:
		Pair& operator= (const Pair& _Other) = default;
		Pair& operator= (Pair&& _Other) = default;
		// Type을 2개 받는다.
		template <typename U1, typename U2>
		// 템플릿 U1, U2 제약 조건. U1, U2로 각각 Type1, Type2의 생성자 호출이 가능한가?
			requires std::constructible_from<Type1, U1&&>&& std::constructible_from<Type2, U2&&>
		// Key, Value 둘중 하나라도 암시적 변환이 불가능하다면 explicit 속성.
		explicit(!std::convertible_to<U1&&, Type1> || !std::convertible_to<U2&&, Type2>)
			// Forwarding Reference. template 인자로 &&이므로 &, && 다 받아 진다.
			Pair(U1&& _Key, U2&& _Value)
			// Type1, Type2의 선택 된 생성자가 noexcept인지 확인. 둘 다 noexcept라면 noexcept 속성 부여.
			noexcept(noexcept(Type1(Utility::Forward<U1>(_Key))) && noexcept(Type2(Utility::Forward<U2>(_Value))))
			: Key(Utility::Forward<U1>(_Key))
			, Value(Utility::Forward<U2>(_Value)) {
		}
		// 이걸로 & &, && &, & &&, && && 네가지 모두를 사용할 수 있다. (중요는 requires, Forward.)
	public:
		Type1 Key = {};
		Type2 Value = {};

	};

	class Node
	{
	public:
		friend class map;
		friend class Iterator;
	private:
		Node() = default;
		Node(const Pair& _Item)
			: Data(_Item) {
		}
		Node(Pair&& _Item)
			: Data(Utility::Move(_Item)) {
		}

	private:
		Node* Parent = nullptr;
		Node* LeftChild = nullptr;
		Node* RightChild = nullptr;
		Pair Data = {};

	private:
		// 중위 순회 기준 다음 노드, 이전 노드를 찾는 함수를 만들겠다.
		// 내부에서만 쓴다. Iterator ++, --에도 사용 할 것.
		static Node* InOrderNext(Node* _CurNode) noexcept
		{
			// 다음 값 찾기.
			// 1. 나보다 큰 값은 어디에 존재할 수 있는가?
			// 1-1. 나의 오른쪽으로 타고 내려간 노드들은 전부 나보다 크다. (Parent < RightChild)
			// 1-2. 나를 왼쪽 자식으로 가지는 부모들과, 그 자식들은 전부 나보다 크다. (LeftChild < Parent)
			// -> + 나의 부모를 타고 올라가다가 우측 부모가 존재한다면, 그 부모는 나보다 크다. (나는 결국 그 오른쪽 부모를 타고 내려왔기 때문에)
			// 그렇다면 어느 순서로 찾아야 하는가? (어느 값이 바로 다음으로 큰 값인가?
			// 1-1. 나의 오른쪽 자식 -> 왼쪽 자식이 없을 때 까지 타고 내려가기
			// 1-2. 나의 오른쪽 자식
			// 1-3. 오른쪽 자식이 없다면 나의 오른쪽 부모
			// 1-4. 왼쪽 부모로 타고 올라가다가 오른쪽 부모가 있다면. (없다면 Root (nullptr)을 만날 것)
			// 1-5. 위 사항이 전부 아니라면 내가 가장 큰 값이다.

			// 오른쪽 자식 있니?
			if (_CurNode->RightChild != nullptr)
			{
				_CurNode = _CurNode->RightChild;
				// 있으면 왼쪽 자식 쭉 내려가자.
				while (_CurNode->LeftChild != nullptr)
				{
					_CurNode = _CurNode->LeftChild;
				}
			}
			else
			{
				while (true)
				{
					// Finder == Root
					if (_CurNode->Parent == nullptr)
					{
						return nullptr;
					}
					// 내가 왼쪽 자식이라면
					else if (_CurNode->Parent->LeftChild == _CurNode)
					{
						// 그 부모가 나의 다음 값
						return _CurNode->Parent;
					}
					// 오른쪽 자식도 없고, 오른쪽 부모도 없으면 왼쪽 부모로 올라가보자.
					else
					{
						_CurNode = _CurNode->Parent;
					}
				}
			}

			return _CurNode;
		}

		static Node* InOrderPrev(Node* _CurNode) noexcept
		{
			// 이전 값 찾기.
			// 큰 값 찾기의 반대 개념이다.
			// 1-1. 나의 왼쪽으로 타고 내려간 노드들은 전부 나보다 작다.
			// 1-2. 나를 오른쪽 자식으로 가지는 부모들과, 그 자식들은 전부 나보다 작다.
			// -> + 나의 부모를 타고 올라가다가 좌측 부모가 존재한다면, 그 부모는 나보다 작다. (나는 결국 그 왼쪽 부모를 타고 내려왔기 때문에)
			// 그렇다면 어느 순서로 찾아야 하는가? (어느 값이 바로 다음으로 작은 값인가?
			// 1-1. 나의 왼쪽 자식 -> 오른쪽 자식이 없을 때 까지 타고 내려가기
			// 1-2. 나의 왼쪽 자식
			// 1-3. 왼쪽 자식이 없다면 나의 오른쪽 부모
			// 1-4. 오른쪽 부모로 타고 올라가다가 왼쪽 부모가 있다면. (없다면 Root (nullptr)을 만날 것)
			// 1-5. 위 사항이 전부 아니라면 내가 가장 작은 값이다.

			if (_CurNode->LeftChild != nullptr)
			{
				_CurNode = _CurNode->LeftChild;

				while (_CurNode->RightChild != nullptr)
				{
					_CurNode = _CurNode->RightChild;
				}
			}
			else
			{
				while (true)
				{
					if (_CurNode->Parent == nullptr)
					{
						return nullptr;
					}
					else if (_CurNode->Parent->RightChild == _CurNode)
					{
						return _CurNode->Parent;
					}
					else
					{
						_CurNode = _CurNode->Parent;
					}
				}
			}

			return _CurNode;
		}

	};

	// 그냥 간단하게 End, Begin, ++, --, ->, * 정도의 operator 정의로 포인터처럼 동작하게만 만들겠다.
	// 이번엔 유효성 검사는 체크하지 않는다. 제대로 사용하면 문제 없는걸 기준으로 만든다.
	class Iterator
	{
	public:
		friend class map;
		friend class Node;
		Iterator() = default;
		Iterator(map* _Owner, Node* _Node)
			: Owner(_Owner)
			, ptr(_Node) {
		}

	public:
		bool operator== (Iterator _Other) const
		{
			return Owner == _Other.Owner && ptr == _Other.ptr;
		}
		bool operator!= (Iterator _Other) const
		{
			return !(*this == _Other);
		}
		Pair& operator*()
		{
			return ptr->Data;
		}
		Pair* operator->()
		{
			return &ptr->Data;
		}
		Iterator& operator++()
		{
			assert(ptr);

			ptr = Node::InOrderNext(ptr);
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator Copy = *this;
			++*this;
			return Copy;
		}
		Iterator& operator--()
		{
			if (ptr == nullptr)
			{
				Node* Finder = Owner->Root;
				assert(Finder);

				while (Finder->RightChild != nullptr)
				{
					Finder = Finder->RightChild;
				}

				ptr = Finder;
				return *this;
			}

			ptr = Node::InOrderPrev(ptr);
			assert(ptr);

			return *this;
		}
		Iterator operator--(int)
		{
			Iterator Copy = *this;
			--*this;
			return Copy;
		}

	private:
		map* Owner = nullptr;
		Node* ptr = nullptr;

	};

public:
	map() {}
	map(Compare _Comp)
		: Comp(_Comp) {}
	~map()
	{
		Clear();
	}

	map(const map& _Other) = delete;
	map(map&& _Other) noexcept = delete;

public:
	map& operator= (const map& _Other) = delete;
	map& operator= (map&& _Other) noexcept = delete;

private:
	Node* Root = nullptr;
	Compare Comp = Compare{};
	size_t Size = 0;

private:
	// 세개의 순회를 만들고, Node에 대해 동작할 함수를 작성할 것.
	// 전위 : 부모 -> 왼쪽 -> 오른쪽
	// 중위 : 왼쪽 -> 부모 -> 오른쪽
	// 후위 : 왼쪽 -> 오른쪽 -> 부모
	// 즉, 부모 노드를 언제 확인하냐의 차이.
	// 재귀함수로 간단하게 구현하겠다.
	// map 내부에서만 쓸 함수고, 잘못 호출하면 터트려버리겠다.

	// 전위 순회
	void PreOrder(Node* _Node, void (*_Func)(Node*))
	{
		// nullptr 넣었니?
		assert(_Node);
		assert(_Func);
		// 비어 있는데 뭘 하려고?
		assert(!Empty());

		_Func(_Node);

		if (nullptr != _Node->LeftChild)
		{
			PreOrder(_Node->LeftChild, _Func);
		}
		if (nullptr != _Node->RightChild)
		{
			PreOrder(_Node->RightChild, _Func);
		}
	}
	// 중위 순회
	void InOrder(Node* _Node, void (*_Func)(Node*))
	{
		assert(_Node);
		assert(_Func);
		assert(!Empty());

		if (nullptr != _Node->LeftChild)
		{
			InOrder(_Node->LeftChild, _Func);
		}

		_Func(_Node);

		if (nullptr != _Node->RightChild)
		{
			InOrder(_Node->RightChild, _Func);
		}
	}
	// 후위 순회
	void PostOrder(Node* _Node, void (*_Func)(Node*))
	{
		assert(_Node);
		assert(_Func);
		assert(!Empty());

		if (nullptr != _Node->LeftChild)
		{
			PostOrder(_Node->LeftChild, _Func);
		}
		if (nullptr != _Node->RightChild)
		{
			PostOrder(_Node->RightChild, _Func);
		}

		// 할 일을 나중에 하면 그게 전위 후위 순회 차이다.
		_Func(_Node);
	}

public:
	Iterator Insert(const Pair& _Item)
	{
		if (0 == Size)
		{
			Root = new Node(_Item);
			++Size;
			return Iterator(this, Root);
		}

		Node* ParentNode = nullptr;
		Node* CurNode = Root;
		bool IsDownLeft = false;

		while (CurNode != nullptr)
		{
			ParentNode = CurNode;
			if (Comp(CurNode->Data.Key, _Item.Key))
			{
				CurNode = CurNode->RightChild;
				IsDownLeft = false;
			}
			// 서로 서로 작지 않다 == 같다.
			// 사용자 key가 operator==을 정의할 필요가 없다.
			else if (false == Comp(CurNode->Data.Key, _Item.Key) && false == Comp(_Item.Key, CurNode->Data.Key))
			{
				// 같으면 이미 존재하는 Key에 대한 Iterator 반환.
				return Iterator(this, CurNode);
			}
			else
			{
				CurNode = CurNode->LeftChild;
				IsDownLeft = true;
			}
		}

		Node* NewNode = new Node(_Item);
		NewNode->Parent = ParentNode;

		// 어느쪽에서 내려왔니?
		if (IsDownLeft)
		{
			ParentNode->LeftChild = NewNode;
		}
		else
		{
			ParentNode->RightChild = NewNode;
		}

		++Size;
		return Iterator(this, NewNode);
	}

	Iterator Find(Type1 _Key)
	{
		Node* CurNode = Root;

		// CurNode가 nullptr이면 빠져 나올 것.
		while (CurNode != nullptr)
		{
			// 결과를 한번 받아서 재사용.
			bool LessLeft = Comp(CurNode->Data.Key, _Key);

			if (LessLeft)
			{
				CurNode = CurNode->RightChild;
			}
			else if (false == LessLeft && false == Comp(_Key, CurNode->Data.Key))
			{
				// 찾았다.
				break;
			}
			else
			{
				CurNode = CurNode->LeftChild;
			}
		}

		// 찾지 못했다면 nullptr Iterator가 return 될 것.
		// 찾았다면 CurNode를 가진 Iterator가 return 될 것.
		return Iterator(this, CurNode);
	}

	// GPT 버전. 훨씬 간결하네? 좀 현타옴. 정리해서 작성.
	Iterator Erase(Iterator _Where)
	{
		// 유효성 검사.
		assert(_Where.Owner == this);
	 	assert(_Where.ptr != nullptr);

		// 현재 노드와 다음 노드
		Node* CurNode = _Where.ptr;
		Node* NextNode = Node::InOrderNext(CurNode);

		// 반환 Iterator
		Iterator ReturnIterator = Iterator(this, NextNode);

		// 자식이 둘
		if (CurNode->LeftChild != nullptr && CurNode->RightChild != nullptr)
		{
			// 둘의 데이터만 교환. 반환 노드도 CurNode.
			Utility::Swap(NextNode->Data, CurNode->Data);
			ReturnIterator.ptr = CurNode;
			// 실제 지울 노드 위치는 NextNode
			CurNode = NextNode;
		}

		// Parent == nullptr -> Root
		Node* Parent = CurNode->Parent;
		// 자식이 하나라도 있으면 nullptr이 아닐 것.
		Node* Child = CurNode->LeftChild != nullptr ? CurNode->LeftChild : CurNode->RightChild;

		// 자식이 하나 있는 경우. 자식의 부모를 Parent로 연결해준다. CurNode == Root라면 Child->Parent == nullptr -> Root가 된다.
		if (Child != nullptr)
		{
			Child->Parent = Parent;
		}

		// CurNode == Root라면 Root 갱신
		if (Parent == nullptr)
		{
			Root = Child;
		}
		// Parent가 있음. 왼쪽 자식인 경우.
		else if (Parent->LeftChild == CurNode)
		{
			Parent->LeftChild = Child;
		}
		// Parent가 있음. 왼쪽 자식도 아님 == 오른쪽 자식
		else
		{
			Parent->RightChild = Child;
		}
	
		// 다 지워졌다. 후처리.
		delete CurNode;
		--Size;
		return ReturnIterator;
	}

	// Iterator Erase(Iterator _Where)
	// {
	// 	// 기본적인 유효성 검사.
	// 	assert(_Where.Owner == this);
	// 	assert(_Where->ptr != nullptr);
	// 
	// 	// BST의 삭제를 몇가지 케이스로 나눈다.
	// 	// 1. 자식이 없다.
	// 	// Leaf Node이므로 부모와의 연결만 끊어주면 끝.
	// 	// 2. 자식이 하나만 있다.
	// 	// 삭제 후 자식 객체와 부모 객체를 이어주면 끝.
	// 	// 3. 자식이 둘 있다.
	// 	// InOrderPrev 혹은 InOrderNext 노드와 자리를 바꾸고, 교체된 위치의 노드를 제거한다.
	// 	// 자식이 둘 이상인 노드의 다음 노드 (혹은 이전 노드)는 자식이 둘이 아님이 보장된다.
	// 	// -> InOrderNext Or Prev에서 오른쪽(왼쪽) 자식이 무조건 있고, 왼쪽(오른쪽) 자식이 nullptr일 때 까지 내려갔으므로.
	// 	// 삭제 후 Root노드가 변경되었는지 확인 하고, 변경되었다면 갱신한다.
	// 
	// 	Node* CurNode = _Where.ptr;
	// 
	// 	Node* Parent = CurNode->Parent;
	// 	Node* LeftChild = CurNode->LeftChild;
	// 	Node* RightChild = CurNode->RightChild;
	// 
	// 	Node* NextNode = Node::InOrderNext(CurNode);
	// 
	// 	// 자식이 둘이다.
	// 	// 자식이 둘이라고 함은, Begin(), --End()가 아님이 보장된다.
	// 	// 둘 체인을 완전히 교체한다.
	// 
	// 	if (LeftChild != nullptr && RightChild != nullptr)
	// 	{
	// 		Parent = CurNode->Parent = NextNode->Parent;
	// 		LeftChild = CurNode->LeftChild = NextNode->LeftChild;
	// 		RightChild = CurNode->RightChild = NextNode->RightChild;
	// 
	// 		NextNode->Parent = Parent;
	// 		NextNode->LeftChild = LeftChild;
	// 		NextNode->RightChild = RightChild;
	// 
	// 		// 바뀌는 노드가 Root인가?
	// 		if (CurNode == Root)
	// 		{
	// 			Root = NextNode;
	// 		}
	// 	}
	// 
	// 	// 자식이 없니?
	// 	if (LeftChild == nullptr && RightChild == nullptr)
	// 	{
	// 		// 연결 끊어주고 지우면 끝난다.
	// 		if (Parent != nullptr)
	// 		{
	// 			// 내가 어느쪽 자식인지 확인하고 연결 끊기.
	// 			if (Parent->LeftChild == CurNode)
	// 			{
	// 				Parent->LeftChild = nullptr;
	// 			}
	// 			else if (Parent->RightChild == CurNode)
	// 			{
	// 				Parent->RightChild = nullptr;
	// 			}
	// 		}
	// 		else  // ParentNode == nullptr -> CurNode == Root -> Size == 1.
	// 		{
	// 			Root = nullptr;
	// 		}
	// 	}
	// 
	// 	Node* Child = LeftChild != nullptr ? LeftChild : RightChild;
	// 	// 자식이 하나만 있니?
	// 	if (Child != nullptr)
	// 	{
	// 		// 내가 어느쪽 자식인지, 아닌지 확인 하고 연결한다.
	// 		if (Parent != nullptr) // Root가 아님
	// 		{
	// 			if (Parent->LeftChild == CurNode)
	// 			{
	// 				Parent->LeftChild = Child;
	// 			}
	// 			else if (Parent->RightChild == CurNode)
	// 			{
	// 				Parent->RightChild = Child;
	// 			}
	// 		}
	// 		else
	// 		{
	// 			Root = Child;
	// 		}
	// 
	// 		Child->Parent = Parent;
	// 	}
	// 
	// 	delete CurNode;
	// 	--Size;
	// 	return Iterator(this, NextNode);;
	// }

	void Clear()
	{
		// 비어 있다면 그냥 종료.
		if (Empty())
			return;

		// PostOrder로 Leaf부터 지워준다.
		PostOrder(Root, [](Node* _Node) {delete _Node; });
		Root = nullptr;
		Size = 0;
	}

public:
	size_t GetSize() const noexcept
	{
		return Size;
	}
	bool Empty() const noexcept
	{
		return Size == 0;
	}
	Iterator Begin() noexcept
	{
		// 비어있다면 End Iterator를 주겠다.
		if (Empty())
		{
			return End();
		}

		Node* CurNode = Root;

		// 먼저 LeftChild가 존재하는지 확인.
		// nullptr이라면 현재 map에서 가장 작은 key값일 것.
		while (CurNode->LeftChild != nullptr)
		{
			CurNode = CurNode->LeftChild;
		}

		return Iterator(this, CurNode);
	}
	Iterator End() noexcept
	{
		// End Iterator는 nullptr로 정의하겠다.
		return Iterator(this, nullptr);
	}

};