#pragma once
#include "Utility.h"
#include "vector"

// �ؽ��� ������ �����ϱ� ���� unordered_set.
template <typename Type>
class unordered_set
{
public:
	unordered_set() = default;
	~unordered_set() = default;

	unordered_set(const unordered_set& _Other) = delete;
	unordered_set(unordered_set&& _Other) = delete;

public:
	unordered_set& operator= (const unordered_set& _Other) = delete;
	unordered_set& operator= (unordered_set&& _Other) = delete;

private:
	// �⺻������ ������ ���� ������ ��Ŷ(�迭)�� �ʿ��� ��.
	// ��ü�� �����ؾ� �ϴ��� Ȯ���ؾ� �Ѵ�. ������ �����ͷ� ���� �ؾ� �� �� ����.
	// ��Ʈ ����ŷ���� 64������ üũ �غ���.
	Type* Bucket = nullptr;
	size_t* Check = {};

	size_t Size = 0;
	size_t Capacity = 0;

	

public:

	
};