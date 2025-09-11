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
		// �⺻ ������, �Ҹ��ڴ� ��������� default ����.
		Pair() = default;
		Pair(const Pair&) = default;
		Pair(Pair&&) noexcept = default;
		~Pair() = default;

		// Type�� 2�� �޴´�.
		template <typename U1, typename U2>
		// ���ø� U1, U2 ���� ����. U1, U2�� ���� Type1, Type2�� ������ ȣ���� �����Ѱ�?
			requires std::constructible_from<Type1, U1&&>&& std::constructible_from<Type2, U2&&>
		// Key, Value ���� �ϳ��� �Ͻ��� ��ȯ�� �Ұ����ϴٸ� explicit �Ӽ�.
		explicit(!std::convertible_to<U1&&, Type1> || !std::convertible_to<U2&&, Type2>)
			// Forwarding Reference. template ���ڷ� &&�̹Ƿ� &, && �� �޾� ����.
			Pair(U1&& _Key, U2&& _Value)
			// Type1, Type2�� ���� �� �����ڰ� noexcept���� Ȯ��. �� �� noexcept��� noexcept �Ӽ� �ο�.
			noexcept(noexcept(Type1(Utility::Forward<U1>(_Key))) && noexcept(Type2(Utility::Forward<U2>(_Value))))
			: Key(Utility::Forward<U1>(_Key))
			, Value(Utility::Forward<U2>(_Value)) {
		}
		// �̰ɷ� & &, && &, & &&, && && �װ��� ��θ� ����� �� �ִ�. (�߿�� requires, Forward.)
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

	// �׳� �����ϰ� End, Begin, ++, --, ->, * ������ operator ���Ƿ� ������ó�� �����ϰԸ� ����ڴ�.
	// �̹��� ��ȿ�� �˻�� üũ���� �ʴ´�. ����� ����ϸ� ���� ���°� �������� �����.
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
			// ���� ���� ���� �ʴ� == ����.
			// ����� key�� ���� operator==�� ������ �ʿ䰡 ����.
			else if (false == (*Comp)(CurNode->Data.Key, _Item.Key) && false == (*Comp)(_Item.Key, CurNode->Data.Key))
			{
				// ������ �̹� �����ϴ� Key�� ���� Iterator ��ȯ.
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

		// ����ʿ��� �����Դ�?
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