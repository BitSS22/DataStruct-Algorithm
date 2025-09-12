#pragma once
#include <assert.h>
#include <cstddef>
#include <new>
#include <memory>
#include <utility>
#include <stdexcept>

#include "Utility.h"

// 원활한 템플릿 코드 작성을 위한 using. 나중에 주석 처리한다.
//using Type = int;

// vector를 구현해보자.
// 커스텀 Allocator까지는 받지 않는다. 머리 아프다..
// 템플릿이니 헤더에 다 때려박아야 한다.
template <typename Type>
class vector
{
public:
	vector() {}
	~vector()
	{
		// Arr이 nullptr이 아니면 메모리 해제.
		if (nullptr != Arr)
		{
			// 안에 메모리 다 지워줘.
			Clear();

			// 메모리 해제 해줘.
			if constexpr (alignof(Type) > alignof(std::max_align_t))
			{
				::operator delete(Arr, std::align_val_t(alignof(Type)));
			}
			else
			{
				::operator delete(Arr);
			}

			// 초기 값으로 밀어버리기.
			Arr = nullptr;
			Capacity = 0;
		}
	}

	// 코드 작성중에 내가 원하지 않는 복사나 이동을 확인하기 위해서.
	vector(const vector& _Other) = delete;
	vector(vector&& _Other) = delete;

public:
	vector& operator= (const vector& _Other) = delete;
	vector& operator= (vector&& _Other) = delete;

	// [] operator, &로 반환한다.
	// 예외를 던지지 않는다.
	Type& operator[](size_t _Index) noexcept
	{
		// debug에서만 동작하는 검사
		assert(_Index < Size);
		return Arr[_Index];
	}
	// const& 오버로딩.
	const Type& operator[](size_t _Index) const noexcept
	{
		assert(_Index < Size);
		return Arr[_Index];
	}

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
		// move_if_noexcept Type의 R-Value constructor가 noexcept니?
		new(Arr + Size) Type(std::move_if_noexcept(_Item));
		++Size;
	}

	// &&로 인자'들'을 받는다.
	template <typename... Types>
	Type& EmplaceBack(Types&&... _Elements)
	{
		if (Size >= Capacity)
		{
			size_t NewCapacitySize = Capacity == 0 ? 1 : Capacity * 2;
			Reserve(NewCapacitySize);
		}

		Type* NewPtr = Arr + Size;

		// PushBack과 다른점. 인자를 그대로 전달받아서 아예 내부에서 직접 생성한다.
		// perfect forwarding. 그냥 넣으면 들어올 때 &&로 받았지만, 함수에 들어오며 이름이 생겨버렸다.
		// 그러니 함수에서 다시 &&로 전달하기 위해 완.벽.한.전.달.을 한다. == Forward를 쓴다.
		new(NewPtr) Type(Utility::Forward<Types>(_Elements)...);
		++Size;

		// 만든 곳 위치 참조로 반환.
		return *NewPtr;
	}
	// 배열 재 할당하는 함수. 내부에서도, 외부에서도 같이 쓸거다.
	void Reserve(size_t _NewCapacity)
	{
		// 오버플로우 방지.
		if (_NewCapacity > SIZE_MAX / sizeof(Type))
			return;

		// 크기를 늘릴때만 호출해라. 아니면 부르지 마.
		if (Capacity >= _NewCapacity)
			return;

		// 외부에서 Reserve 호출했는데, 기존 Size보다 작거나 같으면 무시.
		if (Size >= _NewCapacity)
			return;

		// 일단 포인터 변수 하나 만들기.
		Type* NewArr = nullptr;

		// alignof(Type) 이건 타입의 얼라인 값이야. (캐시 라인 정렬)
		// 최대 얼라인 크기보다 크니?
		// if constexpr == 컴파일 타임 분기 처리.
		if constexpr (alignof(Type) > alignof(std::max_align_t))
		{
			// std::nothrow 예외 던지지 말고 nullptr 줘.
			// 크면 align_val_t를 사용한 얼라인 정렬 new를 사용한다.
			// std::align_val_t(alignof(Type)) Type의 align 정보만큼 정렬해야 해. (오버얼라인)
			NewArr = static_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity, std::align_val_t(alignof(Type))));
		}
		else
		{
			// 아니면 그냥 new 써.
			NewArr = static_cast<Type*>(::operator new(sizeof(Type) * _NewCapacity));
		}
		
		// 지금까지 몇개 옮겼니?
		size_t i = 0;
		// 예외가 던질거야?
		try
		{
			while (i < Size)
			{
				// 새로운 위치로 이동 (R-Value가 noexcept가 아니라면 copy)
				new(NewArr + i) Type(std::move_if_noexcept(Arr[i]));
				++i;
			}
		}
		catch (...) // 복사 중 예외 발생!
		{
			// 복사된거 다시 다 지워
			for (size_t j = 0; j < i; ++j)
			{
				std::destroy_at(NewArr + j);
			}
			
			// 새로 할당한 메모리도 지워.
			if constexpr (alignof(Type) > alignof(std::max_align_t))
			{
				::operator delete(NewArr, std::align_val_t(alignof(Type)));
			}
			else
			{
				::operator delete(NewArr);
			}

			// 예외 위로 던져줘.
			throw;
		}

		// 여기 왔으면 예외 없음. == 예외 발생시 실행 안됨.
		for (size_t j = 0; j < Size; ++j)
		{
			// 기존 배열의 요소들의 소멸자 호출시켜서 정리한다.
			std::destroy_at(Arr + j);
		}

		// Arr 메모리 반환.
		// new 처럼 Type의 얼라인에 따라 컴파일 타임 분기 처리.
		if constexpr (alignof(Type) > alignof(std::max_align_t))
		{
			::operator delete(Arr, std::align_val_t(alignof(Type)));
		}
		else
		{
			::operator delete(Arr);
		}
		
		// 배열 주소와 Capacity 갱신.
		Arr = NewArr;
		Capacity = _NewCapacity;
	}

	// 맨 뒤에꺼 하나 지워줘
	void PopBack() noexcept
	{
		// Size 0보다 커야 해.
		assert(Size);

		// 맨 마지막꺼 하나 소멸자 호출.
		std::destroy_at(Arr + (Size - 1));
		--Size; // 하나 빠졌어.
	}

	// 안에 있는거 다 지워줘.
	void Clear() noexcept
	{
		// 소멸자 호출 해줘.
		for (size_t i = 0; i < Size; ++i)
		{
			std::destroy_at(Arr + i);
		}

		// 할당 된 메모리는 그대로.
		Size = 0;
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
	bool IsEmpty() const noexcept
	{
		return Size == 0;
	}
	// 이런 인터페이스 있으면 좋지 않을까?
	bool IsFull() const noexcept
	{
		return Size == Capacity;
	}
	// GetData를 오버로드 한다.
	// const vector에서는 아래 것이 호출돼서 수정 불가능. 그냥 vector에서는 위의 것이 호출된다.
	Type* GetData() noexcept
	{
		return Arr;
	}
	const Type* GetData() const noexcept
	{
		return Arr;
	}

	// 끝에 있는거 줘.
	Type& Back() noexcept
	{
		// assert는 debug 빌드에서만. release에서 호출하면 정의되지 않은 동작으로 큰 위험 요소 있음.
		// 근데 std::vector의 구현이 이렇다.
		assert(Size);
		return Arr[Size - 1];
	}
	const Type& Back() const noexcept
	{
		assert(Size);
		return Arr[Size - 1];
	}

	// operator[] 와 가장 큰 차이점은, 인덱스 오버시 out_of_range exception이 발생돼서 전파됨.
	// 그래서 At 함수는 noexcept가 아니다.
	Type& At(size_t _Index)
	{
		// 그래도 debug에서 검사 해야지.
		assert(_Index < Size);
		// 너 인덱스 초과야.
		if (_Index >= Size)
		{
			throw std::out_of_range("At() : index out of range");
		}

		return Arr[_Index];
	}
	const Type& At(size_t _Index) const
	{
		assert(_Index < Size);
		if (_Index >= Size)
		{
			throw std::out_of_range("At() const : index out of range");
		}

		return Arr[_Index];
	}

	// Heap을 여기 만들어 버리자.
public:
	// 서브트리 (부모, 왼쪽, 오른쪽)에 힙화. 자식의 서브트리들은 모두 Heap 상태여야 정상 작동한다.
		// 부분 트리에도 적용하기 위해 서브트리의 Index를 받는다.
	template <typename Compare = std::less<Type>>
	void HeapifyDown(size_t _Index, Compare _Comp = Compare{})
	{
		// 배열 갯수가 1가 이하인 배열은 이미 정렬 되었다고 볼 수 있다.
		if (Size < 2)
		{
			return;
		}

		// 당연히 Size를 넘는 Index가 들어오면 안된다.
		assert(_Index < Size);

		while (true)
		{
			size_t Best = _Index;
			size_t LeftChild = HeapLeftChild(_Index);
			size_t RightChild = HeapRightChild(_Index);

			// Size를 넘지 않고 _Comp를 만족하는 자식이 있는가?
			if (LeftChild < Size && _Comp(Arr[Best], Arr[LeftChild]))
			{
				Best = LeftChild;
			}
			if (RightChild < Size && _Comp(Arr[Best], Arr[RightChild]))
			{
				Best = RightChild;
			}

			// 없다면 끝.
			if (Best == _Index)
			{
				break;
			}

			// 있다면 둘의 값을 바꾼다.
			Utility::Swap(Arr[Best], Arr[_Index]);
			// 바뀐 자식으로 다시 Heapify.
			_Index = Best;
		}
	}

	// 배열을 완전히 Heap으로 만들어 준다.
	template <typename Compare = std::less<Type>>
	void BuildHeap(Compare _Comp = Compare{})
	{
		if (Size < 2)
		{
			return;
		}

		// _Size / 2 이전의 원소들만이 항상 자식을 가진다. 자식이 없는 서브트리는 이미 정렬된 서브트리. 
		for (size_t i = Size / 2; i > 0; --i)
		{
			// 말단 노드부터 Heap을 충족시킨다.
			HeapifyDown(i - 1, _Comp);
		}
	}

	// 힙 구조가 깨지지 않게 정리한다.
	// 배열은 이미 힙 구조라고 가정한다.
	// 당연히 _Comp는 힙화 할때와 같은 함수 기능으로 사용자가 넣어야 한다.
	template <typename Compare = std::less<Type>>
	void HeapPush(const Type& _Item, Compare _Comp = Compare{})
	{		
		PushBack(_Item);

		size_t CurIndex = Size - 1;

		while (CurIndex != 0)
		{
			size_t Parent = HeapParent(CurIndex);
			if (_Comp(Arr[Parent], Arr[CurIndex]))
			{
				Utility::Swap(Arr[Parent], Arr[CurIndex]);
				CurIndex = Parent;
			}
			else
			{
				break;
			}
		}
	}

	template <typename Compare = std::less<Type>>
	void HeapPush(Type&& _Item, Compare _Comp = Compare{})
	{
		PushBack(Utility::Forward<Type>(_Item));

		size_t CurIndex = Size - 1;

		while (CurIndex != 0)
		{
			size_t Parent = HeapParent(CurIndex);
			if (_Comp(Arr[Parent], Arr[CurIndex]))
			{
				Utility::Swap(Arr[Parent], Arr[CurIndex]);
				CurIndex = Parent;
			}
			else
			{
				break;
			}
		}
	}

	// 하나 빼줘.
	template <typename Compare = std::less<Type>>
	void HeapPop(Compare _Comp = Compare{})
	{
		Utility::Swap(Arr[0], Arr[Size - 1]);
		PopBack();

		HeapifyDown(0, _Comp);
	}

private:
	static size_t HeapParent(size_t _Index) noexcept
	{
		// _Index는 0이 아니어야 한다. 들어오면 죽이겠다.
		assert(_Index);
		return (_Index - 1) / 2;
	}

	static size_t HeapLeftChild(size_t _Index) noexcept
	{
		return _Index * 2 + 1;
	}

	static size_t HeapRightChild(size_t _Index) noexcept
	{
		return _Index * 2 + 2;
	}
};