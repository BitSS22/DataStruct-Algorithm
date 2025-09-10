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
		// �⺻ ������, �Ҹ��ڴ� ��������� default ����.
		Pair() = default;
		Pair(const Pair&) = default;
		Pair(Pair&&) noexcept = default;
		~Pair() = default;

		// Type�� 2�� �޴´�.
		template <typename U1, typename U2>
		// ���ø� U1, U2 ���� ����. U1, U2�� ���� Type1, Type2�� ������ ȣ���� �����Ѱ�?
			requires std::constructible_from<Type1, U1&&> &&	std::constructible_from<Type2, U2&&>
		// Key, Value ���� �ϳ��� �Ͻ��� ��ȯ�� �Ұ����ϴٸ� explicit �Ӽ�.
			explicit(!std::convertible_to<U1&&, Type1> || !std::convertible_to<U2&&, Type2>)
		// Forwarding Reference. template ���ڷ� &&�̹Ƿ� &, && �� �޾� ����.
			Pair(U1&& _Key, U2&& _Value)
		// Type1, Type2�� ���� �� �����ڰ� noexcept���� Ȯ��. �� �� noexcept��� noexcept �Ӽ� �ο�.
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