#pragma once
#include <assert.h>

// 원활한 템플릿 코드 작성을 위한 using. 나중에 주석 처리한다.
using Type = int;

// vector를 구현해보자.
// 커스텀 Allocator까지는 받지 않는다. 머리 아프다..
// 템플릿이니 헤더에 다 때려박아야 한다.
//template <typename Type>
class vector
{
public:
	vector() {}
	~vector() {}

	// 코드 작성중에 내가 원하지 않는 복사나 이동을 확인하기 위해서.
	vector(const vector& _Other) = delete;
	vector(vector&& _Other) = delete;
	vector& operator= (const vector& _Other) = delete;
	vector& operator= (vector&& _Other) = delete;

private:
	// 필요한건 동적할당 받은 메모리 위치, 엘리먼트 갯수, 메모리의 크기.
	Type* Arr = nullptr;
	size_t Size = 0;
	size_t Capacity = 0;

public:
	// 외부에서 호출할 수 있는 함수들.
	// PushFront같은 함수는 안 만들거다. 그럴거면 vector 쓰지 마라.
	void PushBack(const Type& _Item)
	{
		// Size가 Capacity보다 크거나 같으면 할당 된 메모리가 부족한 것.
		if (Size >= Capacity)
		{
			// 재 할당할 사이즈를 정한다. 2배씩 늘리자.
			// Size가 0이면 0 * 2 == 0 이므로 0일때는 1로 정한다.
			size_t NewCapacitySize = Capacity == 0 ? 1 : Capacity * 2;
			Reserve(NewCapacitySize);
		}

		// 이제 공간이 무조건 있을 것이다. 엘리먼트 추가 후 사이즈 늘려준다.
		// 미리 할당해 둔 공간에 placement new를 한다.
		new(Arr + Size) Type(_Item);
		++Size;
	}

	// R-Value 버전 오버로드
	void PushBack(Type&& _Item)
	{
		if (Size >= Capacity)
		{
			size_t NewCapacitySize = Capacity == 0 ? 1 : Capacity * 2;
			Reserve(NewCapacitySize);
		}

		// _Item을 move로 R-Value 캐스팅 하는 부분이 다름.
		new(Arr + Size) Type(std::move(_Item));
		++Size;
	}

	void Reserve(size_t _NewCapacity)
	{
		// 크기를 늘릴때만 호출해라.
		if (Capacity >= _NewCapacity)
			return;

		// 새로운 메모리 공간'만' 할당 하겠다. _NewCapacity만큼.
		Type* NewArr = reinterpret_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity));
	}

private:
	// 내부에서만 쓸 함수.
	

public:
	// 간단한 Get, Set 함수들.
	// 내가 만들 클래스니 함수 명은 내 코딩 스탠다드를 따른다.
	size_t GetSize() const noexcept
	{
		return Size;
	}
	size_t GetCapacity() const noexcept
	{
		return Capacity;
	}
	Type* GetData() const noexcept
	{
		return Arr;
	}

};