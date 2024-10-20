#pragma once
#include <assert.h>

#define INNER_CLASS

//using T = float;


template <typename T>
class MyList
{
public:
	INNER_CLASS class MyNode;
	INNER_CLASS class Iterator;
public:
	MyList()
		: DataSize(0)
		, HeadNode(nullptr)
		, TailNode(nullptr)
	{
		HeadNode = new MyNode({});
		TailNode = new MyNode({});
		HeadNode->NextNode = TailNode;
		TailNode->PrevNode = HeadNode;
	}
	~MyList()
	{
		Clear();
		delete HeadNode;
		delete TailNode;
	}

private:
	size_t DataSize;
	MyNode* HeadNode;
	MyNode* TailNode;

public:
	void Clear()
	{
		MyNode* DeleteNode = HeadNode->NextNode;

		while (DeleteNode != TailNode)
		{
			MyNode* TempNode = DeleteNode;
			DeleteNode = TempNode->NextNode;
			delete TempNode;
		}

		HeadNode->NextNode = TailNode;
		TailNode->PrevNode = HeadNode;
		DataSize = 0;
	}
	void push_back(const T& _data)
	{
		MyNode* InsertNode = new MyNode(_data);
		MyNode* BackNode = TailNode->PrevNode;

		BackNode->NextNode = InsertNode;
		TailNode->PrevNode = InsertNode;

		InsertNode->NextNode = TailNode;
		InsertNode->PrevNode = BackNode;

		++DataSize;
	}
	void push_front(const T& _data)
	{
		MyNode* InsertNode = new MyNode(_data);
		MyNode* FrontNode = HeadNode->NextNode;

		FrontNode->PrevNode = InsertNode;
		HeadNode->NextNode = InsertNode;

		InsertNode->NextNode = FrontNode;
		InsertNode->PrevNode = HeadNode;

		++DataSize;
	}

	size_t size() const
	{
		return DataSize;
	}

	Iterator begin()
	{
		return Iterator(this, HeadNode->NextNode);
	}

	Iterator End()
	{
		return Iterator(this, TailNode);
	}

	Iterator erase(Iterator _iterator)
	{
		if (_iterator.Indicate == nullptr || _iterator.Owner == nullptr || _iterator.Indicate == _iterator.Owner->HeadNode || _iterator.Indicate == _iterator.Owner->TailNode)
			assert(nullptr);

		MyNode* prev = _iterator.Indicate->PrevNode;
		MyNode* next = _iterator.Indicate->NextNode;
		
		prev->NextNode = next;
		next->PrevNode = prev;

		delete _iterator.Indicate;
		--DataSize;

		return Iterator(_iterator.Owner, next);
	}

public:
	INNER_CLASS
	class Iterator
	{
		friend class MyList;
	public:
		Iterator(MyList* _list, MyNode* _node)
			: Owner(_list)
			, Indicate(_node)
		{}

	private:
		MyList* Owner;
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
		T& operator*()
		{
			return Indicate->Data;
		}
		Iterator& operator++()
		{
			if (Indicate == Owner->TailNode)
				assert(nullptr);
			Indicate = Indicate->NextNode;
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
			if (Indicate == Owner->HeadNode->NextNode)
				assert(nullptr);

			Indicate = Indicate->PrevNode;
			return *this;
		}
		Iterator operator--(int)
		{
			Iterator Temp = *this;
			--(*this);
			return Temp;
		}
	};

private:
	INNER_CLASS
	class MyNode
	{
		friend class MyList;
	private:
		MyNode(T _data)
			: Data(_data)
			, PrevNode(nullptr)
			, NextNode(nullptr)
		{}

	private:
		T Data;
		MyNode* PrevNode;
		MyNode* NextNode;
	};
};