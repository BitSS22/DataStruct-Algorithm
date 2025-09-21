#pragma once

#include <vector>
#include <queue>
#include <limits>
#include <cstdint>
#include <cassert>
#include <utility>
#include <functional>

#include "GraphAPI.h"

// 경로는 찾지 않는다. 모든 노드로의 비용만 계산.
template <GraphAPI Graph>
std::vector<typename Graph::Cost> Dijkstra(const Graph& _Graph, typename Graph::NodeID _Start)
{
	using NodeID = typename Graph::NodeID;
	using Cost = typename Graph::Cost;
	using Pair = std::pair<Cost, NodeID>;

	assert(_Graph.IsValid(_Start));

	// 현재 기록된 값의 저장
	// 초기 값은 최대치로 지정한다.
	std::vector<Cost> Costs(_Graph.GetNodeCount(), std::numeric_limits<Cost>::max());
	std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> VisitQ = {};

	// queue에 시작 지점을 넣고, 값을 0으로 설정한다.
	Costs[static_cast<size_t>(_Start)] = static_cast<Cost>(0);
	VisitQ.emplace(static_cast<Cost>(0), _Start);

	// 비지 않을 때 까지 반복.
	while (!VisitQ.empty())
	{
		// 가장 짧은거 하나 빼낸다.
		Pair pair = VisitQ.top();
		Cost C = pair.first;
		NodeID N = pair.second;

		VisitQ.pop();

		// 같지 않음 == 최신 데이터가 아님. 넘긴다.
		if (!(Costs[static_cast<size_t>(N)] == C))
			continue;

		// Node와 인접한 이웃을 전부 도는 API 함수.
		_Graph.ForeachNeighbor(N, [&Costs, &VisitQ, C](NodeID _ID, Cost _Cost)
			{
				// 음수 가중치는 허용하지 않음.
				assert(static_cast<Cost>(0) <= _Cost);

				Cost NewDist = C + _Cost;
				// 기존의 경로보다 값이 작다면.
				if (NewDist < Costs[static_cast<size_t>(_ID)])
				{
					// Cost를 갱신하고, 방문시킬 NodeQ에 집어 넣는다.
					Costs[static_cast<size_t>(_ID)] = NewDist;
					VisitQ.emplace(NewDist, _ID);
				}
			});
	}

	// 모든 노드로 가는 비용을 담은 vector
	return Costs;
}
