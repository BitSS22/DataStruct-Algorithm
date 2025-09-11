#pragma once
#include <cassert>
#include <type_traits>

#include "Utility.h"

using Type1 = int;
using Type2 = int;
using Compare = bool(*)(const int&, const int&);

//template <typename Type1, typename Type2, typename Compare>
class map
{
public:
	struct Pair
	{
	public:
		// 기본 생성자, 소멸자는 명시적으로 default 선언.
		Pair() = default;
		Pair(const Pair&) = default;
		Pair(Pair&&) noexcept = default;
		~Pair() = default;

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
	};

	// 그냥 간단하게 End, Begin, ++, --, ->, * 정도의 operator 정의로 포인터처럼 동작하게만 만들겠다.
	// 이번엔 유효성 검사는 체크하지 않는다. 제대로 사용하면 문제 없는걸 기준으로 만든다.
	class Iterator
	{
	public:
		friend class map;
		friend class Node;
		Iterator() = default;
		Iterator(Node* _Node)
			: ptr(_Node) {}

	private:
		Node* ptr = nullptr;

	};

public:
	map() {}
	~map() {}

	map(const map& _Other) = delete;
	map(map&& _Other) noexcept = delete;

public:
	map& operator= (const map& _Other) = delete;
	map& operator= (map&& _Other) noexcept = delete;

private:
	Node* Root = nullptr;
	Compare* Comp = nullptr;
	size_t Size = 0;

public:
	Iterator Insert(const Pair& _Item)
	{
		if (0 == Size)
		{
			Root = new Node(_Item);
			++Size;
			return Iterator(Root);
		}

		Node* ParentNode = nullptr;
		Node* CurNode = Root;
		bool IsDownLeft = false;

		while (CurNode != nullptr)
		{
			ParentNode = CurNode;
			if ((*Comp)(CurNode->Data.Key, _Item.Key))
			{
				CurNode = CurNode->RightChild;
				IsDownLeft = false;
			}
			// 각각 서로 작지 않다 == 같다.
			// 사용자 key에 대해 operator==을 정의할 필요가 없다.
			else if (false == (*Comp)(CurNode->Data.Key, _Item.Key) && false == (*Comp)(_Item.Key, CurNode->Data.Key))
			{
				// 같으면 이미 존재하는 Key에 대한 Iterator 반환.
				return Iterator(CurNode);
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
		return Iterator(NewNode);
	}

};