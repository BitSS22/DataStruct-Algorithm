#pragma once
#include <vector>
#include <stack>
#include <cstdint>

#include "GraphAPI.h"

namespace DFS
{
	template <GraphAPI Graph, typename CallBack>
	void DFS(const Graph& _Graph, typename Graph::NodeID _Node, CallBack&& _CallBack)
	{
		if (!_Graph.IsValid(_Node))
			return;

		std::stack<typename Graph::NodeID> NodeList;
		std::vector<uint8_t> Visits(_Graph.GetNodeCount(), false);

		NodeList.push(_Node);
		Visits[static_cast<size_t>(_Node)] = true;

		while (!NodeList.empty())
		{
			typename Graph::NodeID Node = NodeList.top();
			_Graph.ForeachNeighbor(Node, [&NodeList, &Visits, &_CallBack](typename Graph::NodeID _ID, typename Graph::Cost _Cost)
				{
					if (Visits[_ID])
						return;

					_CallBack(_ID, _Cost);
					NodeList.push(_ID);
					Visits[_ID] = true;
				});
			NodeList.pop();
		}
	}

}