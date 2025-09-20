#pragma once

#include <vector>
#include <queue>

#include "GraphAPI.h"

namespace Dijkstra
{
	template <GraphAPI Graph, typename CallBack>
	std::vector<typename Graph::NodeID> Dijkstra(const Graph& _Graph, typename Graph::NodeID _Start)
	{
		using NodeID = typename Graph::NodeID;
		using Cost = typename Graph::Cost;

		std::vector<NodeID> Result = {};

		if (!_Graph.IsValid(_Start) || !_Graph.IsValid(_End))
			return Result;

		std::priority_queue<NodeID> NodeQ = {};
		NodeQ.push(_Start);

		while (!NodeQ.empty())
		{
			NodeID Node = NodeQ.top();
			NodeQ.pop();

			_Graph.ForeachNeighbor(Node, [&NodeQ](NodeID _ID, Cost _Cost)
				{

				});

			
		}
		
	}
}