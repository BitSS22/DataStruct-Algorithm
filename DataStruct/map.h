#pragma once

#define INNER_CLASS

using T1 = int;
using T2 = float;
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
		// TODO. 원소를 찾는 코드
	}
	bool insert(MyPair _newData)
	{
		MyNode* CreateNode = new MyNode(_newData);
		MyNode* CurNode = RootNode;

		if (RootNode == nullptr)
		{
			RootNode = CreateNode;
		}
		else if (CurNode == nullptr)
		{
			
		}
		else if
		{
			
		}

		// 1. 데이터가 하나도 없을때 (RootNode가 nullptr일 때)
		// 1-1. 새로운 노드를 생성하고 RootNode를 새로운 노드로 설정
		//
		// 2. 데이터가 1개 이상 존재할 때
		// 2-1. 현재 삽입한 노드의 key와 기존 노드의 key 값이 동일한 노드가 존재 할 때 -> 노드를 만들지 않고 false return
		// 2-2. 현재 삽입한 노드의 key값이 기존 노드의 key 값보다 작을 때 -> LeftChild로 이동 후 2번 반복
		// 2-3. 현재 삽입한 노드의 key값이 기존 노드의 key 값보다 클 때 -> RightChild로 이동 후 2번 반복
		// 2-4. 이동한 노드가 nullptr일때 -> CreateNode의 Parent를 이전 노드로 설정, 이전 노드의 Left 혹은 RightChild의 값을 CreateNode로 설정
		

		++DataCount;
		return true;
	}
	Iterator erase(Iterator _iter)
	{
		// TODO. 원소를 삭제하는 코드
	}
	void clear()
	{
		// TODO. 모든 원소를 삭제하는 코드
	}
private:

public:
	INNER_CLASS
	class Iterator
	{
	public:
		Iterator(MyMap* _list, MyNode* _node)
			: Owner(_list)
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
		}
		bool operator==(const Iterator& _other)
		{
			if (Owner == _other.Owner && Indicate == _other.Indicate)
				return true;
			return false;
		}
		bool operator!=(const Iterator& _other)
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