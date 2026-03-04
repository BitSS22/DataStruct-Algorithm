#pragma once

#include <vector>
#include <string>
#include <string_view>

// trie 구현
// trie는 head라는 node를 가지고,
// node는 vector<node> childs와 현재까지 왔을 때 문자열이 존재하는지 확인하는 bool값,
// node 자신의 char 값을 가져야 함.
// std::vector<node> childs
// bool _end

// trie interface
// 어느 문자열이 존재하는지 확인
// bool Contains(string_view str) const
// 어느 문자열 검색 후 마지막 노드 반환
// Node* Find(string_view str)
// 문자열 추가
// Node* Insert(string_view str)

// node interface
// 현재 노드가 end인지 확인
// bool is_end()
// 특정 값의 자식이 있는지 확인
// bool contains(char data)
// 특정 값의 자식을 추가
// node* insert(char data)
// 현재 노드의 end값 변경
// void set_end(bool b)
// 

class Trie
{
public:
	struct Node
	{
	private:
		std::vector<Node> _childs = {};
		bool _end = false;
		char _ch = 0;

	public:
		bool ContainsChilds(char ch) const
		{
			for (size_t i = 0; i < _childs.size(); ++i)
			{
				if (_childs[i]._ch == ch)
				{
					return true;
				}
			}
			return false;
		}
		Node* FindChilds(char ch)
		{
			for (size_t i = 0; i < _childs.size(); ++i)
			{
				if (_childs[i]._ch == ch)
				{
					return &_childs[i];
				}
			}
			return nullptr;
		}
		Node* AddChild(char ch)
		{
			if (ContainsChilds(ch))
			{
				return nullptr;
			}

			Node* new_node = &_childs.emplace_back();
			new_node->_ch = ch;
			return new_node;
		}

	public:
		bool IsEnd() const
		{
			return _end;
		}
		void SetEnd(bool b)
		{
			_end = b;
		}

	};

public:
	Trie() {}
	~Trie() {}
	Trie(const Trie& other) = delete;
	Trie(Trie&& other) = delete;

public:
	Trie& operator= (const Trie& other) = delete;
	Trie& operator= (Trie&& other) = delete;

private:
	Node _head = {};

public:
	bool Contains(std::string_view str)
	{
		Node* current_node = &_head;

		for (size_t i = 0; i < str.size(); ++i)
		{
			Node* next_node = current_node->FindChilds(str[i]);
			if (next_node == nullptr)
			{
				return false;
			}

			current_node = next_node;
		}

		return current_node->IsEnd();
	}
	Node* Find(std::string_view str)
	{
		Node* current_node = &_head;

		for (size_t i = 0; i < str.size(); ++i)
		{
			Node* next_node = current_node->FindChilds(str[i]);
			if (next_node == nullptr)
			{
				return nullptr;
			}

			current_node = next_node;
		}

		return current_node;
	}
	Node* Insert(std::string_view str)
	{
		Node* current_node = &_head;

		for (size_t i = 0; i < str.size(); ++i)
		{
			Node* next_node = current_node->FindChilds(str[i]);
			if (next_node == nullptr)
			{
				next_node = current_node->AddChild(str[i]);
			}
			current_node = next_node;
		}

		current_node->SetEnd(true);
		return current_node;
	}

};


// 구현 세부나 API 관련 점검 필요함