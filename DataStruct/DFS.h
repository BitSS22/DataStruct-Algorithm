#pragma once
#include <vector>
#include <stack>
#include <cstdint>

#include "GraphAPI.h"

template <GraphAPI Graph, typename CallBack>
void DFS(const Graph& _Graph, typename Graph::NodeID _Node, CallBack&& _CallBack)
{
	using NodeID = typename Graph::NodeID;
	using Cost = typename Graph::Cost;

	if (!_Graph.IsValid(_Node))
		return;

	std::stack<NodeID> NodeStack;
	std::vector<uint8_t> Visits(_Graph.GetNodeCount(), false);

	NodeStack.push(_Node);
	Visits[static_cast<size_t>(_Node)] = true;

	while (!NodeStack.empty())
	{
		NodeID Node = NodeStack.top();
		NodeStack.pop();

		_Graph.ForeachNeighbor(Node, [&NodeStack, &Visits, &_CallBack](NodeID _ID, Cost _Cost)
			{
				if (Visits[_ID])
					return;

				_CallBack(_ID, _Cost);
				NodeStack.push(_ID);
				Visits[_ID] = true;
			});
	}
}

