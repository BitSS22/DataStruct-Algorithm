#pragma once

#include <type_traits>
#include <concepts>

// GPT 도움 받아서 만든다.

// 템플릿 기본 타입 정의
using DefaultNodeID = unsigned int;
using DefaultCost = float;

// 양방향 연결 여부. Tag용 struct
struct Directed {};
struct UnDirected {};

// 아래 concept에서 컴파일 가능 여부를 확인하기 위한 구조체. 객체화의 목적이 아님.
template <typename Type>
struct NeighborProbe
{
	using NodeId = typename Type::NodeID;
	using Cost = typename Type::Cost;

	// 가능하니?
	void operator() (NodeId, Cost) const noexcept {}
};

// 그래프의 API는 이것들을 포함해야 한다.
template <typename Type>
concept GraphAPI = requires(const Type & _Type, typename Type::NodeID _ID)
{
	// 아래 두 Type이 정의 되어 있어야 함.
	typename Type::NodeID;
	typename Type::Cost;

	// 아래 세 동작이 가능해야 함.
	{ _Type.GetNodeCount() } -> std::convertible_to<size_t>;
	{ _Type.IsValid(_ID) } -> std::same_as<bool>;
	_Type.ForEachNeighbor(_ID, NeighborProbe<Type>{});
};
