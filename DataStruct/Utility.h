#pragma once
#include <cassert>
#include <concepts>

namespace Utility
{
	// std::remove_reference.
	// 이건 그냥 템플릿이다. 받은 타입에 이름 T를 붙여주는게 끝.
	// struct/class로 만들어 Type에 대한 분기 처리와, &, && 등 타입에 따라 T 결과값을 노출 시킨다.
	template<typename Type>
	struct remove_reference
	{
		using T = Type;
	};
	// 아래 두개는 템플릿 특수화. Type&, Type&&가 템플릿 인자로 들어오면 선택된다.
	// 안에서 하는 일은 똑같다.
	template<typename Type>
	struct remove_reference<Type&>
	{
		using T = Type;
	};
	template<typename Type>
	struct remove_reference<Type&&>
	{
		using T = Type;
	};
	// 즉, remove_ref<Type>::T는 참조성을 제거한 타입이 된다.

	// 그런데 즉, remove_ref<Type>::T로 쓰기 귀찮다. 그래서 template alias로 이름을 지어주겠다.
	template<typename Type>
	using remove_ref = typename remove_reference<Type>::T;
	// remove_reference<Type>::T 이 값인지 타입인지 알 수없다. 그러니 앞에 typename을 붙여줘야 한다.
	// 이제 remove_ref<Type&&> Value; 처럼 간단하게 벗겨먹을 수 있다!
	// 그래서 이걸로 뭘 하냐? move와 forward 만들거다.

	// std::move.
	// Type을 받아 참조성을 벗긴 Type을 얻고, 그걸 다시 &&로 캐스팅해 반환한다. 즉, 전부 R-Value로 내보낸다.
	// 벗기지 않으면 Type& -> return Type&;
	// forward reference = template만의 타입 추론. Type&&으로 L-Value, R-Value 다 받는다.
	template <typename Type>
	constexpr remove_ref<Type>&& Move(Type&& _Value) noexcept
	{
		return static_cast<remove_ref<Type>&&>(_Value);
	}

	// L-Value Overload.
	template <typename Type>
	constexpr Type&& Forward(remove_ref<Type>& _Value) noexcept
	{
		// &&로 return 하면 Type& &&로 참조 붕괴가 일어나서 Type& return.
		return static_cast<Type&&>(_Value);
	}
	// R-Value Overload.
	template <typename Type>
	constexpr Type&& Forward(remove_ref<Type>&& _Value) noexcept
	{
		return static_cast<Type&&>(_Value);
	}
	// Type&& (Type&& _Value) 같은 형식으로는 안되나? => Type에 대한 template 형식 재 추론 금지.
	// remove_ref<Type>로 Type이 변형 되었으니 template이 추론하지 않음. -> 암시적 추론, 예상되지 못한 동작 방지.
	// 즉, Forward<Type>(_Value)와 같이 명확히 호출해야 한다.

	// 구현한 Move를 사용해 swap 함수 구현.
	// 복사가 아닌 이동으로 싸게 옮기기.
	template<typename Type>
	inline void Swap(Type& _Left, Type& _Right) noexcept
	{
		Type Temp = Move(_Left);
		_Left = Move(_Right);
		_Right = Move(Temp);
	}

	// Sort에서 쓸 기본 비교 함수. 당연히 Type은 operator< 가 overload 돼 있어야 한다.
	template<typename Type>
	inline bool DefaultCompare(const Type& _Left, const Type& _Right) noexcept
	{
		return _Left < _Right;
	}

	// 정수 타입만 받을거야.
	template<std::integral Type>
	constexpr size_t GetPrintDigit(Type _Value) noexcept
	{
		// 0은 숫자 0이니 1자릿수 아닐까?
		if (0 == _Value)
			return 1;

		size_t Count = 0;
		// 음수의 -기호도 자릿수로 세겠다.
		if (0 > _Value)
			++Count;

		while (_Value != 0)
		{
			_Value /= 10;
			++Count;
		}

		return Count;
	}

	// 유틸리티 클리스 Heap을 만들어 본다.
	// 배열을 Heap처럼 쓸 수 있게 만들어 주는 기능 묶음.
	class Heap
	{
		// 이 클래스는 만드는 클래스가 아니야.
		Heap() = delete;
	public:
		// 서브트리 (부모, 왼쪽, 오른쪽)에 힙화. 자식의 서브트리들은 모두 Heap 상태여야 정상 작동한다.
		// 부분 트리에도 적용하기 위해 서브트리의 Index를 받는다.
		template <typename Type, typename Compare = bool(*)(const Type&, const Type&)>
		static void HeapifyDown(Type* _Arr, size_t _Size, size_t _Index = 0, Compare _Comp = Utility::DefaultCompare)
		{
			// 이런 것 좀 넣지 마.
			assert(_Arr);
			// 당연히 Size를 넘는 Index가 들어오면 안된다.
			assert(_Index < _Size);
			// 비교 함수 왜 안줘.
			assert(_Comp);

			while (true)
			{
				size_t Best = _Index;
				size_t LeftChild = GetLeftChild(_Index);
				size_t RightChild = GetRightChild(_Index);

				// Size를 넘지 않고 _Comp를 만족하는 자식이 있는가?
				if (LeftChild < _Size && _Comp(_Arr[LeftChild], _Arr[Best]))
				{
					Best = LeftChild;
				}
				if (RightChild < _Size && _Comp(_Arr[RightChild], _Arr[Best]))
				{
					Best = RightChild;
				}

				// 없다면 끝.
				if (Best == _Index)
				{
					break;
				}

				// 있다면 둘의 값을 바꾼다.
				Utility::Swap(_Arr[_Index], _Arr[Best]);
				// 바뀐 자식으로 다시 Heapify.
				_Index = Best;
			}
		}

		// 배열을 완전히 Heap으로 만들어 준다.
		template <typename Type, typename Compare = bool(*)(const Type&, const Type&)>
		static void BuildHeap(Type* _Arr, size_t _Size, Compare _Comp = Utility::DefaultCompare)
		{
			// 배열 갯수가 1가 이하인 배열은 이미 정렬 되었다고 볼 수 있다.
			if (_Size < 2)
			{
				return;
			}

			// _Size / 2 이전의 원소들만이 항상 자식을 가진다. 자식이 없는 서브트리는 이미 정렬된 서브트리. 
			for (size_t i = _Size / 2 - 1; i >= 0; --i)
			{
				// 말단 노드부터 Heap을 충족시킨다.
				HeapifyDown(_Arr, _Size, i, _Comp);
			}
		}

		// 힙 구조가 깨지지 않게 정리한다.
		// 들어오는 배열은 이미 힙 구조라고 가정한다.
		// _Arr[_CurrentSize] 의 인덱스 검사는 하지 않음.
		// 함수 가정 -> 마지막 원소 외엔 모두 힙을 만족한다.
		// 마지막 원소는 정렬되지 않고 삽입 된 채로 받았다. -> 즉, 마지막 원소를 정렬해주는 함수.
		// 당연히 _Comp는 힙화 할때와 같은 함수 기능으로 사용자가 넣어야 한다.
		template <typename Type, typename Compare = bool(*)(const Type&, const Type&)>
		static void HeapifyUp(Type* _Arr, size_t _Size,  Compare _Comp = Utility::DefaultCompare)
		{
			// 이런 것 좀 넣지 마.
			assert(_Arr);
			// 비교 함수 왜 안줘.
			assert(_Comp);

			if (_Size < 2)
			{
				return;
			}

			size_t Best = _Size - 1;

		}

		static size_t GetParent(size_t _Index) noexcept
		{
			// _Index는 0이 아니어야 한다. 들어오면 죽이겠다.
			assert(_Index);
			return (_Index - 1) / 2;
		}

		static size_t GetLeftChild(size_t _Index) noexcept
		{
			return _Index * 2 + 1;
		}

		static size_t GetRightChild(size_t _Index) noexcept
		{
			return _Index * 2 + 2;
		}
	};
}
