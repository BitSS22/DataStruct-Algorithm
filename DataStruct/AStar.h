#pragma once

#include <vector>
#include <queue>
#include <limits>
#include <cstdint>
#include <cassert>
#include <utility>
#include <functional>

#include "GraphAPI.h"

// 다익스트라와 가장 다른 점은 휴리스틱 함수를 이용한다는 것.
// 타입맵같은 구조는 맨해튼, 유클리드 거리처럼 휴리스틱(추정치)을 어느정도 쉽게 구할 수 있다.
// 하지만 규칙성이 없고, 간선 가중치가 제각각인 그래프는 휴리스틱을 사용자가 제공할 수 밖에 없다.
// 휴리스틱 추정치가 클수록 연산의 속도가 빠르다. 하지만 휴리스틱 추정치가 1을 넘기면 최단거리의 보장이 되지 않는다.
// 반대로 휴리스틱이 0에 가까울수록 다익스트라에 가까워진다. 휴리스틱이 0이면 사실상 다익스트라.
// 
// 그렇다면 무조건 휴리스틱은 1 이하여야 하느냐? 라고 한다면 잘 모르겠다.
// ㄴ> 수십마리가 넘는 몬스터가 플레이어의 위치를 항상 최단거리로 찾아가야 할 필요가 있을까?
// ㄴ> 최단거리를 보장하지 않더라도 연산을 줄여 최적화에 도움을 주는게 낫지 않을까?
// ㄴ> 그리고 항상 최단거리로 길을 찾는게 정말 "자연스러울까?" (실제로도 사람은 항상 최단거리로 길을 찾지 못하는데.)



template <GraphAPI Graph>
std::vector<typename Graph::Cost> AStar(const Graph& _Graph, typename Graph::NodeID _Start, typename Graph::NodeID _End)
{
	std::is_function

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
