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

// 휴리스틱 요구 타입
// Graph::(제공) 휴리스틱 정의 -> Cost Heuristic(NodeID, NodeID) const noexcept
// 사용자 정의 휴리스틱 -> Cost Heuristic(const Graph&, NodeID, NodeID) noexcept

// 그래프가 Heuristic 함수를 가지고 있는지?
template <typename G>
concept GraphHasHeuristic = requires(const G & _G, typename G::NodeID _N)
{
	typename G::NodeID;
	typename G::Cost;

	{ _G.Heuristic(_N, _N) } -> std::convertible_to<typename G::Cost>;
};

// 기본 휴리스틱 타입 두개.
struct UseGraphHeuristic {};
template <GraphAPI G>
struct ZeroHeuristic
{
	static constexpr typename G::Cost operator()() const noexcept
	{
		return static_cast<G::Cost>(0);
	}
};

// Graph가 휴리스틱 함수가 있으면 그걸로, 없으면 0을 반환하는 휴리스틱을 선택하는 alias,
// 그리고 Astar에서 template 기본 타입으로 이걸 지정한다.
template <GraphAPI G>
using DefaultHeuristic = std::conditional_t<GraphHasHeuristic<G>, UseGraphHeuristic, ZeroHeuristic<G>>;

// 메타 함수로 인해 내용이 길다..
// 간단히 정리하면 G::Cost H(G, G::NodeID, G::NodeID), 또는 G::Cost H(G::NodeID, G::NodeID) 형태로 호출이 가능하거나,
// H의 타입이 UseGraphHeuristic 이며, G가 GraphHasHeuristic concept를 만족해야 한다는 concept 제약.
template <typename G, typename H>
concept HeuristicAPI = (std::invocable<const H&, const G&, typename G::NodeID, typename G::NodeID>
						&& std::convertible_to<std::invoke_result_t<const H&, const G&, typename G::NodeID, typename G::NodeID>, typename G::Cost>)
					|| (std::invocable<const H&, typename G::NodeID, typename G::NodeID>
						&& std::convertible_to<std::invoke_result_t<const H&, typename G::NodeID, typename G::NodeID>, typename G::Cost>)
					|| (std::same_as<std::remove_cvref_t<H>, UseGraphHeuristic> && GraphHasHeuristic<G>);

template <GraphAPI Graph, typename Heuristic = DefaultHeuristic<Graph>>
requires HeuristicAPI<Graph, Heuristic>
std::vector<typename Graph::Cost> AStar(const Graph& _Graph, typename Graph::NodeID _Start, typename Graph::NodeID _End, Heuristic&& _Heuristic = Heuristic{})
{
	// 다익스트라와 기본적인 매커니즘은 같다.
	// 다만, Cost를 저장할때 휴리스틱 추정치를 사용한다.
	// 만약 휴리스틱이 0이라면, 다익스트라 최단경로 알고리즘.
	using NodeID = typename Graph::NodeID;
	using Cost = typename Graph::Cost;
	using Pair = std::pair<Cost, NodeID>;

	assert(_Graph.IsValid(_Start) && _Graph.IsValid(_End));

	// OpenNode와 CloseNode.
	std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> OpenNodes = {};
	std::vector<NodeID> CloseNodes = {};
	CloseNodes.reserve(_Graph.GetNodeCount());
	
	CloseNodes[static_cast<size_t>(_Start)] = static_cast<Cost>(0);
	OpenNodes.emplace(static_cast<Cost>(0), _Start);

	// 비지 않을 때 까지 반복.
	while (!OpenNodes.empty())
	{
		// 가장 짧은거 하나 빼낸다.
		Pair pair = OpenNodes.top();
		Cost C = pair.first;
		NodeID N = pair.second;

		OpenNodes.pop();

		// 같지 않음 == 최신 데이터가 아님. 넘긴다.
		if (!(CloseNodes[static_cast<size_t>(N)] == C))
			continue;

		// Node와 인접한 이웃을 전부 도는 API 함수.
		_Graph.ForeachNeighbor(N, [&CloseNodes, &OpenNodes, C](NodeID _ID, Cost _Cost)
			{
				// 음수 가중치는 허용하지 않음.
				assert(static_cast<Cost>(0) <= _Cost);

				Cost NewDist = C + _Cost;
				// 기존의 경로보다 값이 작다면.
				if (NewDist < CloseNodes[static_cast<size_t>(_ID)])
				{
					// Cost를 갱신하고, 방문시킬 NodeQ에 집어 넣는다.
					CloseNodes[static_cast<size_t>(_ID)] = NewDist;
					OpenNodes.emplace(NewDist, _ID);
				}
			});
	}

	// 모든 노드로 가는 비용을 담은 vector
	return CloseNodes;
}
