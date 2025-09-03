#pragma once
#include <assert.h>

// 마찬가지로 원활한 템플릿 작성을 위한 별칭.
using Type = int;

// 리스트 구현한다. 노드 형식으로 서로를 연결해서 알고 있는 자료구조.
// 순회 등에서 최적화의 여지를 생각해 본다면 한번 크기를 할당할때, 몇개 정도 분량의 적절한 사이즈를 땡겨서 미리 할당하고,
// 그 자리에 생성한다면 캐시 적중률이나 속도면에서 더 이득을 볼 수도 있지 않을까.

//template<typename Type>
class list
{
private:
	// list는 노드 형식이고, 외부에 그걸 알려줄 필요는 없다.
	// 그래서 inner class로 Node를 만든다.
	class node
	{
	private:
		node() {}
		~node()
		{
			// 메모리를 0으로 밀고 퇴장.
			Prev = nullptr;
			Next = nullptr;
		}

		// 코드 작성중에 내가 원하지 않는 복사나 이동을 확인하기 위해서.
		node(const node& _Other) = delete;
		node(node&& _Other) = delete;
		node& operator= (const node& _Other) = delete;
		node& operator= (node&& _Other) = delete;

	private:
		// 이전 노드, 다음 노드를 가진다.
		// 그리고 저장할 데이터도 가진다.
		node* Prev = nullptr;
		node* Next = nullptr;
		Type Data = {};

	private:
		// Prev와 Next로 양 node를 연결해주는 함수 두개.
		// 인자 nullpt 들어오면 터진다. nullptr 체크 하고 써라.
		// 내부에서 쓸 함수니 위험해도 괜찮다. 내가 알아서 쓴다.
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

	// 코드 작성중에 내가 원하지 않는 복사나 이동을 확인하기 위해서.
	list(const list& _Other) = delete;
	list(list&& _Other) = delete;

public:
	list& operator= (const list& _Other) = delete;
	list& operator= (list&& _Other) = delete;
};