#pragma once
#include <cassert>
#include <type_traits>

#include "Utility.h"

using Type1 = int;
using Type2 = int;


template <typename Type1, Type2> requires(Type1 a, Type1 b) { a < b; }
class map
{
public:
	struct Pair
	{
	public:
		// 기본 생성자, 소멸자는 명시적으로 default 선언.
		Pair() = default;
		Pair(const Pair&) = default;
		Pair(Pair&&) noexcept = default;
		~Pair() = default;

		// Type을 2개 받는다.
		template <typename U1, typename U2>
		// 템플릿 U1, U2 제약 조건. U1, U2로 각각 Type1, Type2의 생성자 호출이 가능한가?
			requires std::constructible_from<Type1, U1&&> &&	std::constructible_from<Type2, U2&&>
		// Key, Value 둘중 하나라도 암시적 변환이 불가능하다면 explicit 속성.
			explicit(!std::convertible_to<U1&&, Type1> || !std::convertible_to<U2&&, Type2>)
		// Forwarding Reference. template 인자로 &&이므로 &, && 다 받아 진다.
			Pair(U1&& _Key, U2&& _Value)
		// Type1, Type2의 선택 된 생성자가 noexcept인지 확인. 둘 다 noexcept라면 noexcept 속성 부여.
			noexcept(noexcept(Type1(Utility::Forward<U1>(_Key))) && noexcept(Type2(Utility::Forward<U2>(_Value))))
			: Key(Utility::Forward<U1>(_Key))
			, Value(Utility::Forward<U2>(_Value)) {}

	public:
		Type1 Key = {};
		Type2 Value = {};

	};

	class Node
	{
		friend class map;
	private:
		Node* Prev = nullptr;
		Node* Next = nullptr;
		Pair Data = {};
	};

public:
	map() {}
	~map() {}

	map(const map& _Other) = delete;
	map(map&& _Other) noexcept = delete;

public:
	map& operator= (const map& _Other) = delete;
	map& operator= (map&& _Other) noexcept = delete;

private:
	Node* Root = nullptr;
	size_t Size = 0;

};