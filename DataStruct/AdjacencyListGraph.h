#pragma once
#include <vector>
#include <cassert>

#include "GraphAPI.h"

template <typename NodeID_Type = DefaultNodeID, typename Cost_Type = DefaultCost, typename DirectionTag = Directed>
class AdjacencyListGraph
{
	// GraphAPI
public:
	using NodeID = NodeID_Type;
	using Cost = Cost_Type;

	template <typename CallBack>
	void ForEachNeighbor(NodeID _Node, CallBack&& _CallBack) const noexcept
	{
		const std::vector<Edge>& Bucket = List[static_cast<size_t>(_Node)];

		for (size_t i = 0; i < List.size(); ++i)
		{
			_CallBack(Bucket[i].Dest, Bucket[i].Weight);
		}
	}
	size_t GetNodeCount() const noexcept
	{
		return List.size();
	}

	bool IsValid(NodeID _Node) const noexcept
	{
		return static_cast<size_t>(_Node) < List.size();
	}

public:
	struct Edge
	{
	public:
		NodeID Dest = {};
		Cost Weight = {};
	};

public:
	explicit AdjacencyListGraph(size_t _NodeCount)
		: List(_NodeCount) {}

private:
	std::vector<std::vector<Edge>> List;

public:
	void AddEdge(NodeID _Start, NodeID _End, Cost _Weight)
	{
		assert(IsValid(_Start) && IsValid(_End));

		List[static_cast<size_t>(_Start)].push_back(Edge(_End, _Weight));

		if constexpr (std::is_same_v < DirectionTag, UnDirected)
		{
			List[static_cast<size_t>(_End)].push_back(Edge(_Start, _Weight));
		}
	}

};