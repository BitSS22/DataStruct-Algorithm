#pragma once

#define INNER_CLASS

using T1 = int;
using T2 = int;
//template<typename T1, typename T2>
class MyMap
{
public:
	INNER_CLASS class MyPair;
	INNER_CLASS class MyNode;
	INNER_CLASS class Iterator;
public:
	MyMap()
		: RootNode(nullptr)
		, DataCount(0)
	{}
	~MyMap()
	{
		clear();
	}
private:
	MyNode* RootNode;
	size_t DataCount;

public:
	Iterator find(T1 _key)
	{
		MyNode* CurIndicateNode = RootNode;

		while (CurIndicateNode != nullptr)
		{
			if (_key == CurIndicateNode->Data.first)
				return Iterator(this, CurIndicateNode);
			else if (_key < CurIndicateNode->Data.first)
				CurIndicateNode = CurIndicateNode->LeftChild;
			else
				CurIndicateNode = CurIndicateNode->RightChild;
		}

		return end();
	}
	bool insert(MyPair _newData)
	{
		// TODO. 원소를 삽입하는 코드
		MyNode* CurIndicateNode = RootNode;

		if (CurIndicateNode == nullptr)
			RootNode = new MyNode(_newData);
		else
		{
			MyNode* PrevIndicateNode = CurIndicateNode;
			bool IsLeft = true;

			while (CurIndicateNode != nullptr)
			{
				PrevIndicateNode = CurIndicateNode;

				if (_newData.first == CurIndicateNode->Data.first)
					return false;
				else if (_newData.first < CurIndicateNode->Data.first)
				{
					CurIndicateNode = CurIndicateNode->LeftChild;
					IsLeft = true;
				}
				else
				{
					CurIndicateNode = CurIndicateNode->RightChild;
					IsLeft = false;
				}
			}

			MyNode* CreateNode = new MyNode(_newData);

			CreateNode->Parent = PrevIndicateNode;
			if (IsLeft == true)
				PrevIndicateNode->LeftChild = CreateNode;
			else
				PrevIndicateNode->RightChild = CreateNode;
		}

		++DataCount;
		return true;

		// 1. 데이터가 하나도 없을때 (RootNode가 nullptr일 때)
		// 1-1. 새로운 노드를 생성하고 RootNode를 새로운 노드로 설정
		//
		// 2. 데이터가 1개 이상 존재할 때
		// 2-1. 현재 삽입한 노드의 key와 기존 노드의 key 값이 동일한 노드가 존재 할 때 -> 노드를 만들지 않고 false return
		// 2-2. 현재 삽입한 노드의 key값이 기존 노드의 key 값보다 작을 때 -> LeftChild로 이동 후 반복
		// 2-3. 현재 삽입한 노드의 key값이 기존 노드의 key 값보다 클 때 -> RightChild로 이동 후 반복
		// 2-4. 이동한 노드가 nullptr일때 -> CreateNode의 Parent를 이전 노드로 설정, 이전 노드의 LeftChild 혹은 RightChild의 값을 CreateNode로 설정
	}
	Iterator erase(Iterator _iter)
	{
		// TODO. 원소를 삭제하는 코드
	}
	void clear()
	{
		// TODO. 모든 원소를 삭제하는 코드
	}
	Iterator begin()
	{
		return Iterator(this, nullptr);
		// TODO. 첫번째 원소를 리턴하는 이터레이터 작성
	}
	Iterator end()
	{
		return Iterator(this, nullptr);
	}
private:

public:
	INNER_CLASS
	class Iterator
	{
	public:
		Iterator(MyMap* _map, MyNode* _node)
			: Owner(_map)
			, Indicate(_node)
		{}

	private:
		MyMap* Owner;
		MyNode* Indicate;

	public:
		Iterator& operator=(const Iterator& _other)
		{
			Owner = _other.Owner;
			Indicate = _other.Indicate;
			return *this;
		}
		bool operator==(const Iterator& _other) const
		{
			if (Owner == _other.Owner && Indicate == _other.Indicate)
				return true;
			return false;
		}
		bool operator!=(const Iterator& _other) const
		{
			return !(*this == _other);
		}
		MyPair& operator*()
		{
			return Indicate->Data;
		}
		MyPair* operator->()
		{
			return &Indicate->Data;
		}
		Iterator& operator++()
		{
			// TODO.
		}
		Iterator operator++(int)
		{
			Iterator Temp = *this;
			++(*this);
			return Temp;
		}
		Iterator& operator--()
		{
			// TODO.
		}
		Iterator operator--(int)
		{
			Iterator Temp = *this;
			--(*this);
			return Temp;
		}
	};

public:
	INNER_CLASS
	class MyPair
	{
	public:
		MyPair(T1 _first, T2 _second)
			: first(_first)
			, second(_second)
		{}

	public:
		T1 first;
		T2 second;
	};
private:
	INNER_CLASS
	class MyNode
	{
		friend class MyMap;
		friend class Iterator;
	private:
		MyNode(MyPair _data)
			: Data(_data)
			, Parent(nullptr)
			, LeftChild(nullptr)
			, RightChild(nullptr)
		{}

	private:
		MyPair Data;
		MyNode* Parent;
		MyNode* LeftChild;
		MyNode* RightChild;
	};
};