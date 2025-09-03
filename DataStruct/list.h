#pragma once
#include <assert.h>

#include "Utility.h"

// ���������� ��Ȱ�� ���ø� �ۼ��� ���� ��Ī.
using Type = int;

// ����Ʈ �����Ѵ�. ��� �������� ���θ� �����ؼ� �˰� �ִ� �ڷᱸ��.
// ��ȸ ��� ����ȭ�� ������ ������ ���ٸ� �ѹ� ũ�⸦ �Ҵ��Ҷ�, � ���� �з��� ������ ����� ���ܼ� �̸� �Ҵ��ϰ�,
// �� �ڸ��� �����Ѵٸ� ĳ�� ���߷��̳� �ӵ��鿡�� �� �̵��� �� ���� ���� ������.

//template<typename Type>
class list
{
private:
	// list�� ��� �����̰�, �ܺο� �װ� �˷��� �ʿ�� ����.
	// �׷��� inner class�� Node�� �����.
	class node
	{
	private:
		// Ī��
		friend class list;
		node() {}
		// ���ϰ� ���� �����ϱ� ������ �����ε�.
		node(const Type& _Data)
			: Data(_Data) {}
		// R-Value�� ����� �д�. ���ڷ� �����鼭 �̸� �������� Move�� R-Value Cast.
		node(Type&& _Data) noexcept
			: Data(Utility::Move(_Data)) {}
		~node()
		{
			// �޸𸮸� 0���� �а� ����.
			Prev = nullptr;
			Next = nullptr;
		}

		// �ڵ� �ۼ��߿� ���� ������ �ʴ� ���糪 �̵��� Ȯ���ϱ� ���ؼ�.
		node(const node& _Other) = delete;
		node(node&& _Other) = delete;
		node& operator= (const node& _Other) = delete;
		node& operator= (node&& _Other) = delete;

	private:
		// ���� ���, ���� ��带 ������.
		// �׸��� ������ �����͵� ������.
		// ���� Size == 0 �϶��� Prev, Next�� DummyNode�� �����Ѵٸ�
		// ����, ���� ��� if (node != nullptr) ���� ���ǽ��� �ʿ� ���� ��.
		node* Prev = nullptr;
		node* Next = nullptr;
		Type Data = {};

	private:
		// Prev�� Next�� �� node�� �������ִ� �Լ�.
		// ���� nullpt ������ ������. nullptr üũ �ϰ� ���.
		// ���ο��� �� �Լ��� �����ص� ������. ���� �˾Ƽ� ����.
		// �׳� �ΰ� �����ҰŴ� static���� ����� �� �����ϴ� �� �� �ְ���.
		static void Connect(node* _Prev, node* _Next) noexcept
		{
			assert(_Prev && _Next);
			_Prev->Next = _Next;
			_Next->Prev = _Prev;
		}

		// �� ��� ����ϰ� ����.
		// �翬�� nullptr ������ ������.
		// �߰� ��常 ����, Head, Tail node�� ���� ���� ���� ó�� �ض�.
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
	// TODO. ������ �����ϴ� ���� �Ҹ��ڿ� �־��.
	~list() {}

	// �ڵ� �ۼ��߿� ���� ������ �ʴ� ���糪 �̵��� Ȯ���ϱ� ���ؼ�.
	list(const list& _Other) = delete;
	list(list&& _Other) = delete;

public:
	list& operator= (const list& _Other) = delete;
	list& operator= (list&& _Other) = delete;

private:
	// list�� �� ������ �ұ�. �� ���� ���� size�� �������.
	node* Head = nullptr;
	node* Tail = nullptr;
	size_t Size = 0;

public:
	// �� ���� ����.
	void PushBack(const Type& _Item)
	{
		// �ϴ� ���ο� ��� �ϳ� ������.
		node* NewNode = new node(_Item);

		// ����ִ�?
		if (IsEmpty())
		{
			// ��������� Head, Tail�� nullptr. �׷��� Head, Tail�� �� ���� �������ְ� ������.
			Head = NewNode;
		}
		else
		{
			// �� ����ִٸ� Tail�� ���� ��尡 �������̴�.
			// �ΰ� ������ �ش�.
			node::Connect(Tail, NewNode);
		}

		// �� ���� ���� ������ �ؾ� �Ѵ�.
		Tail = NewNode;
		++Size;
	}


public:
	// �̹� ������ vector���� �����;߰ڴ�. ������ �ű⼭ �ű䵥.
	size_t GetSize() const noexcept
	{
		return Size;
	}
	bool IsEmpty() const noexcept
	{
		return Size == 0;
	}
	// Back, Front�� �ᱹ Tail�� Head.
	Type& Back() noexcept
	{
		// ���� �� ������ ��Ʈ��.
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
		// ���� �� ������ ��Ʈ��.
		assert(Head);
		return Head->Data;
	}
	const Type& Front() const noexcept
	{
		assert(Head);
		return Head->Data;
	}
};