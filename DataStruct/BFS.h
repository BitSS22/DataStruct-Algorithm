#pragma once
#include <vector>
#include <queue>
#include <cstdint>

#include "GraphAPI.h"

namespace BFS
{
	template <GraphAPI Graph, typename CallBack>
	void BFS(const Graph& _Graph, typename Graph::NodeID _Node, CallBack&& _CallBack)
	{
		using NodeID = typename Graph::NodeID;
		using Cost = typename Graph::Cost;

		if (!_Graph.IsValid(_Node))
			return;

		std::queue<NodeID> NodeQ;
		std::vector<uint8_t> Visits(_Graph.GetNodeCount(), false);

		NodeQ.push(_Node);
		Visits[static_cast<size_t>(_Node)] = true;

		while (!NodeQ.empty())
		{
			NodeID Node = NodeQ.front();
			NodeQ.pop();

			_Graph.ForeachNeighbor(Node, [&NodeQ, &Visits, &_CallBack](NodeID _ID, Cost _Cost)
				{
					if (Visits[_ID])
						return;

					_CallBack(_ID, _Cost);
					NodeQ.push(_ID);
					Visits[_ID] = true;
				});
		}
	}
}

