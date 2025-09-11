#pragma once
#include <cassert>
#include <type_traits>

#include "Utility.h"

//using Type1 = int;
//using Type2 = int;
//using Compare = bool(*)(const int&, const int&);

template <typename Type1, typename Type2, typename Compare = bool(*)(const Type1&, const Type1&)>
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

	private:
		// ���� ��ȸ ���� ���� ���, ���� ��带 ã�� �Լ��� ����ڴ�.
		// ���ο����� ����. Iterator ++, --���� ��� �� ��.
		static Node* InOrderNext(Node* _CurNode) noexcept
		{
			// ���� �� ã��.
			// 1. ������ ū ���� ��� ������ �� �ִ°�?
			// 1-1. ���� ���������� Ÿ�� ������ ������ ���� ������ ũ��. (Parent < RightChild)
			// 1-2. ���� ���� �ڽ����� ������ �θ���, �� �ڽĵ��� ���� ������ ũ��. (LeftChild < Parent)
			// -> + ���� �θ� Ÿ�� �ö󰡴ٰ� ���� �θ� �����Ѵٸ�, �� �θ�� ������ ũ��. (���� �ᱹ �� ������ �θ� Ÿ�� �����Ա� ������)
			// �׷��ٸ� ��� ������ ã�ƾ� �ϴ°�? (��� ���� �ٷ� �������� ū ���ΰ�?
			// 1-1. ���� ������ �ڽ� -> ���� �ڽ��� ���� �� ���� Ÿ�� ��������
			// 1-2. ���� ������ �ڽ�
			// 1-3. ������ �ڽ��� ���ٸ� ���� ������ �θ�
			// 1-4. ���� �θ�� Ÿ�� �ö󰡴ٰ� ������ �θ� �ִٸ�. (���ٸ� Root (nullptr)�� ���� ��)
			// 1-5. �� ������ ���� �ƴ϶�� ���� ���� ū ���̴�.

			// ������ �ڽ� �ִ�?
			if (_CurNode->RightChild != nullptr)
			{
				_CurNode = _CurNode->RightChild;
				// ������ ���� �ڽ� �� ��������.
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
					// ���� ���� �ڽ��̶��
					else if (_CurNode->Parent->LeftChild == _CurNode)
					{
						// �� �θ� ���� ���� ��
						return _CurNode->Parent;
					}
					// ������ �ڽĵ� ����, ������ �θ� ������ ���� �θ�� �ö󰡺���.
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
			// ���� �� ã��.
			// ū �� ã���� �ݴ� �����̴�.
			// 1-1. ���� �������� Ÿ�� ������ ������ ���� ������ �۴�.
			// 1-2. ���� ������ �ڽ����� ������ �θ���, �� �ڽĵ��� ���� ������ �۴�.
			// -> + ���� �θ� Ÿ�� �ö󰡴ٰ� ���� �θ� �����Ѵٸ�, �� �θ�� ������ �۴�. (���� �ᱹ �� ���� �θ� Ÿ�� �����Ա� ������)
			// �׷��ٸ� ��� ������ ã�ƾ� �ϴ°�? (��� ���� �ٷ� �������� ���� ���ΰ�?
			// 1-1. ���� ���� �ڽ� -> ������ �ڽ��� ���� �� ���� Ÿ�� ��������
			// 1-2. ���� ���� �ڽ�
			// 1-3. ���� �ڽ��� ���ٸ� ���� ������ �θ�
			// 1-4. ������ �θ�� Ÿ�� �ö󰡴ٰ� ���� �θ� �ִٸ�. (���ٸ� Root (nullptr)�� ���� ��)
			// 1-5. �� ������ ���� �ƴ϶�� ���� ���� ���� ���̴�.

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

	// �׳� �����ϰ� End, Begin, ++, --, ->, * ������ operator ���Ƿ� ������ó�� �����ϰԸ� ����ڴ�.
	// �̹��� ��ȿ�� �˻�� üũ���� �ʴ´�. ����� ����ϸ� ���� ���°� �������� �����.
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
	map()
		: Comp(Utility::DefaultCompare<Type1>) {
	}
	map(Compare _Comp)
		: Comp(_Comp) {
	}
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
	Compare Comp;
	size_t Size = 0;

private:
	// ������ ��ȸ�� �����, Node�� ���� ������ �Լ��� �ۼ��� ��.
	// ���� : �θ� -> ���� -> ������
	// ���� : ���� -> �θ� -> ������
	// ���� : ���� -> ������ -> �θ�
	// ��, �θ� ��带 ���� Ȯ���ϳ��� ����.
	// ����Լ��� �����ϰ� �����ϰڴ�.
	// map ���ο����� �� �Լ���, �߸� ȣ���ϸ� ��Ʈ�������ڴ�.

	// ���� ��ȸ
	void PreOrder(Node* _Node, void (*_Func)(Node*))
	{
		// nullptr �־���?
		assert(_Node);
		assert(_Func);
		// ��� �ִµ� �� �Ϸ���?
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
	// ���� ��ȸ
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
	// ���� ��ȸ
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

		// �� ���� ���߿� �ϸ� �װ� ���� ���� ��ȸ ���̴�.
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
			if ((*Comp)(CurNode->Data.Key, _Item.Key))
			{
				CurNode = CurNode->RightChild;
				IsDownLeft = false;
			}
			// ���� ���� ���� �ʴ� == ����.
			// ����� key�� operator==�� ������ �ʿ䰡 ����.
			else if (false == (*Comp)(CurNode->Data.Key, _Item.Key) && false == (*Comp)(_Item.Key, CurNode->Data.Key))
			{
				// ������ �̹� �����ϴ� Key�� ���� Iterator ��ȯ.
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
		return Iterator(this, NewNode);
	}

	Iterator Find(Type1 _Key)
	{
		Node* CurNode = Root;

		// CurNode�� nullptr�̸� ���� ���� ��.
		while (CurNode != nullptr)
		{
			// ����� �ѹ� �޾Ƽ� ����.
			bool LessLeft = (*Comp)(CurNode->Data.Key, _Key);

			if (LessLeft)
			{
				CurNode = CurNode->RightChild;
			}
			else if (false == LessLeft && false == (*Comp)(_Key, CurNode->Data.Key))
			{
				// ã�Ҵ�.
				break;
			}
			else
			{
				CurNode = CurNode->LeftChild;
			}
		}

		// ã�� ���ߴٸ� nullptr Iterator�� return �� ��.
		// ã�Ҵٸ� CurNode�� ���� Iterator�� return �� ��.
		return Iterator(this, CurNode);
	}

	Iterator Erase(Iterator _Where)
	{
		// �⺻���� ��ȿ�� �˻�.
		assert(_Where->Owner != nullptr);
		assert(_Where->ptr != nullptr);

		// BST�� ������ ��� ���̽��� ������.
		// 1. �ڽ��� ����.
		// Leaf Node�̹Ƿ� �θ���� ���Ḹ �����ָ� ��.
		// 2. �ڽ��� �ϳ��� �ִ�.
		// ���� �� �ڽ� ��ü�� �θ� ��ü�� �̾��ָ� ��.
		// 3. �ڽ��� �� �ִ�.
		// InOrderPrev Ȥ�� InOrderNext ���� �ڸ��� �ٲٰ�, ��ü�� ��ġ�� ��带 �����Ѵ�.
		// �ڽ��� �� �̻��� ����� ���� ��� (Ȥ�� ���� ���)�� �ڽ��� ���� �ƴ��� ����ȴ�.
		// -> InOrderNext Or Prev���� ��� �� �ڽ��� nullptr�� �� ���� ���������Ƿ�.
		// ���� �� Root��尡 ����Ǿ����� Ȯ�� �ϰ�, ����Ǿ��ٸ� �����Ѵ�.


	}

	void Clear()
	{
		// ��� �ִٸ� �׳� ����.
		if (Empty())
			return;

		// PostOrder�� Leaf���� �����ش�.
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
		// ����ִٸ� End Iterator�� �ְڴ�.
		if (Empty())
		{
			return End();
		}

		Node* CurNode = Root;

		// ���� LeftChild�� �����ϴ��� Ȯ��.
		// nullptr�̶�� ���� map���� ���� ���� key���� ��.
		while (CurNode->LeftChild != nullptr)
		{
			CurNode = CurNode->LeftChild;
		}

		return Iterator(this, CurNode);
	}
	Iterator End() noexcept
	{
		// End Iterator�� nullptr�� �����ϰڴ�.
		return Iterator(this, nullptr);
	}

};