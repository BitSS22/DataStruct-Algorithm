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
		MyNode* CurIndicateNode = RootNode;

		if (CurIndicateNode == nullptr)
		{
			RootNode = new MyNode(_newData);
		}
		else
		{
			MyNode* PrevIndicateNode = nullptr;
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
	}
	Iterator erase(Iterator _iter)
	{
		// TODO. 원소를 삭제하는 코드
		--DataCount;
	}
	void clear()
	{
		if (RootNode == nullptr)
			return;

		MyNode* CurIndicateNode = RootNode;

		while (true)
		{
			if (CurIndicateNode->LeftChild != nullptr)
			{
				CurIndicateNode = CurIndicateNode->LeftChild;
			}
			else if (CurIndicateNode->RightChild != nullptr)
			{
				CurIndicateNode = CurIndicateNode->RightChild;
			}
			else
			{
				if (CurIndicateNode == RootNode)
				{
					RootNode = nullptr;
					delete CurIndicateNode;
					break;
				}

				MyNode* PrevIndicateNode = CurIndicateNode;
				CurIndicateNode = CurIndicateNode->Parent;

				if (CurIndicateNode->LeftChild == PrevIndicateNode)
					CurIndicateNode->LeftChild = nullptr;
				else
					CurIndicateNode->RightChild = nullptr;

				delete PrevIndicateNode;
			}
		}

		DataCount = 0;
	}
	Iterator begin()
	{
		MyNode* CurIndicateNode = RootNode;
		if (RootNode == nullptr)
			return end();
		else
			while (CurIndicateNode->LeftChild != nullptr)
				CurIndicateNode = CurIndicateNode->LeftChild;

		return Iterator(this, CurIndicateNode);
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
			if (*this == Owner->end())
				assert(nullptr);

			if (Indicate->RightChild != nullptr)
			{
				Indicate = Indicate->RightChild;

				while (Indicate->LeftChild != nullptr)
					Indicate = Indicate->LeftChild;
			}
			else
			{
				MyNode* PrevIndicate = nullptr;

				while(true)
				{
					PrevIndicate = Indicate;

					if (Indicate->Parent == nullptr)
					{
						*this = Owner->end();
						break;
					}
					else
					{
						Indicate = Indicate->Parent;

						if (PrevIndicate == Indicate->LeftChild)
							break;
					}
				}
			}
			
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator Temp = *this;
			++(*this);
			return Temp;
		}
		Iterator& operator--()
		{
			// TODO. 이전 원소를 가리키는 코드
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