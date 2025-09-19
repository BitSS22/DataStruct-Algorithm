#pragma once

#include <type_traits>
#include <concepts>

// GPT ���� �޾Ƽ� �����.

// ���ø� �⺻ Ÿ�� ����
using DefaultNodeID = unsigned int;
using DefaultCost = float;

// ����� ���� ����. Tag�� struct
struct Directed {};
struct UnDirected {};

// �Ʒ� concept���� ������ ���� ���θ� Ȯ���ϱ� ���� ����ü. ��üȭ�� ������ �ƴ�.
template <typename Type>
struct NeighborProbe
{
	using NodeId = typename Type::NodeID;
	using Cost = typename Type::Cost;

	// �����ϴ�?
	void operator() (NodeId, Cost) const noexcept {}
};

// �׷����� API�� �̰͵��� �����ؾ� �Ѵ�.
template <typename Type>
concept GraphAPI = requires(const Type & _Type, typename Type::NodeID _ID)
{
	// �Ʒ� �� Type�� ���� �Ǿ� �־�� ��.
	typename Type::NodeID;
	typename Type::Cost;

	// �Ʒ� �� ������ �����ؾ� ��.
	{ _Type.GetNodeCount() } -> std::convertible_to<size_t>;
	{ _Type.IsValid(_ID) } -> std::same_as<bool>;
	_Type.ForEachNeighbor(_ID, NeighborProbe<Type>{});
};
