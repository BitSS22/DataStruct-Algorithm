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
	template<std::integral Integer>
	constexpr size_t PrintDigitCount(Integer _Value) noexcept
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

	// RadixSort에서 쓸 함수를 여기 만들겠다.
	template<std::integral Integer>
	constexpr size_t DigitNumber(Integer _Value, size_t _DecimalDigit_Zerobase) noexcept
	{
		// CutValue로 뽑아 낼 자릿수를 잡는다.
		size_t CutValue = 1;
		for (size_t i = 0; i < _DecimalDigit_Zerobase; ++i)
		{
			CutValue *= 10;
		}

		return _Value / CutValue % 10;
	}

	// 출력이 아닌 순수하게 자릿수를 찾는다 양수만 받겠다. 0은 0 return.
	template<std::integral Integer>
	constexpr size_t UnsignedDigitCount(Integer _Value) noexcept
	{
		size_t Count = 0;

		while (_Value != 0)
		{
			_Value /= 10;
			++Count;
		}

		return Count;
	}
}
