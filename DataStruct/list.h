#pragma once
#include <assert.h>

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
		node() {}
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
		node* Prev = nullptr;
		node* Next = nullptr;
		Type Data = {};

	private:
		// Prev�� Next�� �� node�� �������ִ� �Լ� �ΰ�.
		// ���� nullpt ������ ������. nullptr üũ �ϰ� ���.
		// ���ο��� �� �Լ��� �����ص� ������. ���� �˾Ƽ� ����.
		void LinkPrev(node* _Prev)
		{
			_Prev->Next = this;
			Prev = _Prev;
		}
		void LinkNext(node* _Next)
		{
			_Next->Prev = this;
			Next = _Next;
		}

	};

public:
	list() {}
	~list() {}

	// �ڵ� �ۼ��߿� ���� ������ �ʴ� ���糪 �̵��� Ȯ���ϱ� ���ؼ�.
	list(const list& _Other) = delete;
	list(list&& _Other) = delete;

public:
	list& operator= (const list& _Other) = delete;
	list& operator= (list&& _Other) = delete;
};