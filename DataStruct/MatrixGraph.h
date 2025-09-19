#pragma once
#include <limits>
#include <cassert>
#include <vector>
#include <type_traits>

#include "GraphAPI.h"

template <typename NodeID_Type = DefaultNodeID, typename Cost_Type = DefaultCost, typename DirectionTag = Directed>
class MatrixGraph
{
	// GraphAPI
public:
	using NodeID = NodeID_Type;
	using Cost = Cost_Type;

	template <typename CallBack>
	void ForEachNeighbor(NodeID _Node, CallBack&& _CallBack) const noexcept
	{
		for (size_t i = 0; i < NodeCount; ++i)
		{
			const Cost Weight = Matrix[Idx(_Node, static_cast<NodeID>(i))];
			if (Weight != Inf)
			{
				_CallBack(static_cast<NodeID>(i), Weight);
			}
		}
	}
	size_t GetNodeCount() const noexcept
	{
		return NodeCount;
	}
	bool IsValid(NodeID _Node) const noexcept
	{
		return static_cast<size_t>(_Node) < NodeCount;
	}

public:
	explicit MatrixGraph(size_t _NodeCount, Cost _InfValue = std::numeric_limits<Cost>::infinity())
		: NodeCount(_NodeCount)
		, Matrix(_NodeCount * _NodeCount, _InfValue)
		, Inf(_InfValue) {}

private:
	size_t NodeCount;
	std::vector<Cost> Matrix;
	Cost Inf;

public:
	void SetCost(NodeID _Start, NodeID _End, Cost _Weight)
	{
		assert(IsValid(_Start) && IsValid(_End));

		Matrix[Idx(_Start, _End)] = _Weight;

		if constexpr (std::is_same_v<DirectionTag, UnDirected>)
		{
			Matrix[Idx(_End, _Start)] = _Weight;
		}
	}

private:
	size_t Idx(NodeID _y, NodeID _x) const noexcept
	{
		return static_cast<size_t>(_y) * NodeCount + static_cast<size_t>(_x);
	}

public:
	Cost GetCost(NodeID _Start, NodeID _End) const noexcept
	{
		return Matrix[Idx(_Start, _End)];
	}

};

