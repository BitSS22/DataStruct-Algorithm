#pragma once
#include "Utility.h"
#include "vector"

// 해쉬의 개념을 구현하기 위한 unordered_set.
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
	// 기본적으로 들어오는 값을 저장할 버킷(배열)이 필요할 것.
	// 객체가 존재해야 하는지 확인해야 한다. 별도의 데이터로 관리 해야 할 것 같다.
	// 비트 마스킹으로 64구간씩 체크 해본다.
	Type* Bucket = nullptr;
	size_t* Check = {};

	size_t Size = 0;
	size_t Capacity = 0;

	

public:

	
};