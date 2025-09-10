#pragma once
#include <cassert>
#include <type_traits>

#include "Utility.h"

using Type1 = int;
using Type2 = int;


//template <typename Type1, Type2>
class map
{
public:
	struct pair
	{
	public:
		pair() = default;
		pair(const pair&) = default;
		pair(pair&&) noexcept = default;
		~pair() = default;

		template <typename U1, typename U2>
			requires std::constructible_from<Type1, U1&&>&&
		std::constructible_from<Type2, U2&&>
			explicit(!std::convertible_to<U1&&, Type1> ||
				!std::convertible_to<U2&&, Type2>)
			pair(U1&& k, U2&& v)
			noexcept(noexcept(Type1(std::forward<U1>(k))) &&
				noexcept(Type2(std::forward<U2>(v))))
			: Key(std::forward<U1>(k))
			, Value(std::forward<U2>(v)) {}

	public:
		Type1 Key = {};
		Type2 Value = {};

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

	size_t Size = 0;

};