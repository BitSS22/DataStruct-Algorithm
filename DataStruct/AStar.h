#pragma once

#include <vector>
#include <queue>
#include <limits>
#include <cstdint>
#include <cassert>
#include <utility>
#include <functional>
#include <type_traits>
#include <concepts>
#include <algorithm>

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
	static constexpr typename G::Cost operator()(const G&, typename G::NodeID, typename G::NodeID) const noexcept
	{
		return static_cast<typename G::Cost>(0);
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

// 휴리스틱 함수를 호출하는 공통된 진입점.
struct HeuristicInvoke
{
	// UseGraphHeuristic이면 여기로 오겠지.
	template <GraphAPI G>
	static typename G::Cost Invoke(UseGraphHeuristic, const G& _Graph, typename G::NodeID _Start, typename G::NodeID _End)
	{
		return static_cast<typename G::Cost>(_Graph.Heuristic(_Start, _End));
	}

	// 그 외엔 이쪽으로 간다.
	template <GraphAPI G, typename H>
	requires HeuristicAPI<G, H>
	static constexpr typename G::Cost Invoke(H _Heuristic, const G& _Graph, typename G::NodeID _Start, typename G::NodeID _End)
	{
		if constexpr (std::invocable<const H&, const G&, typename G::NodeID, typename G::NodeID)
		{
			return static_cast<typename G::Cost>(_Heuristic(_Graph, _Start, _End));
		}
		else
		{
			return static_cast<typename G::Cost>(_Heuristic(_Start, _End));
		}
	}
};

template <GraphAPI Graph, typename Heuristic = DefaultHeuristic<Graph>>
requires HeuristicAPI<Graph, Heuristic>
std::vector<typename Graph::NodeID> AStar(const Graph& _Graph, typename Graph::NodeID _Start, typename Graph::NodeID _End, Heuristic&& _Heuristic = Heuristic{})
{
	// 다익스트라와 기본적인 매커니즘은 같다.
	// 다만, Cost를 저장할때 휴리스틱 추정치를 사용한다.
	// 만약 휴리스틱이 0이라면, 다익스트라 최단경로 알고리즘.
	using NodeID = typename Graph::NodeID;
	using Cost = typename Graph::Cost;
	using FNPair = std::pair<Cost, NodeID>;

	assert(_Graph.IsValid(_Start) && _Graph.IsValid(_End));

	if (_Start == _End)
	{
		return {};
	}

	const size_t NodeCount = _Graph.GetNodeCount();
	constexpr Cost InfCost = std::numeric_limits<Cost>::max();
	constexpr NodeID InvalidNode = std::numeric_limits<NodeID>::max();

	struct Data
	{
	public:
		Cost F = InfCost;
		Cost G = InfCost;
		NodeID Parent = InvalidNode;
		bool Close = false;
	};

	// 데이터를 갱신할 곳이 필요하다.
	std::vector<Data> Datas(_Graph.GetNodeCount());

	std::priority_queue<FNPair, std::vector<FNPair>, std::greater<FNPair>> Open = {};

	Datas[static_cast<size_t>(_Start)].G = static_cast<Cost>(0);
	Datas[static_cast<size_t>(_Start)].F = HeuristicInvoke::Invoke(_Heuristic, _Graph, _Start, _End);
	Open.emplace(Datas[static_cast<size_t>(_Start)].F, _Start);

	while (!Open.empty())
	{
		FNPair Pair = Open.top();
		Open.pop();
		NodeID CurNode = Pair.second;

		// 이미 닫힌 노드.
		if (Datas[static_cast<size_t>(CurNode)].Close)
		{
			continue;
		}

		Datas[static_cast<size_t>(CurNode)].Close = true;

		// 도착!
		if (CurNode == _End)
		{
			break;
		}

		_Graph.ForeachNeighbor(CurNode, [CurNode, &Datas, &Open, &_Graph, &_Heuristic, _End](NodeID _ID, Cost _Cost)
			{
				// 이미 닫힌 노드.
				if (Datas[static_cast<size_t>(_ID)].Close)
					return;

				Cost CurG = Datas[static_cast<size_t>(CurNode)].G + _Cost;

				// 올 때 든 비용이 기존에 기록된 비용보다 적을때.
				if (CurG < Datas[static_cast<size_t>(_ID)].G)
				{
					// Data의 값을 갱신해 준다.
					Datas[static_cast<size_t>(_ID)].G = CurG;
					Datas[static_cast<size_t>(_ID)].F = CurG + HeuristicInvoke::Invoke(_Heuristic, _Graph, _ID, _End);
					Datas[static_cast<size_t>(_ID)].Parent = CurNode;
					// 그리고 Open에 집어넣는다.
					Open.emplace(Datas[static_cast<size_t>(_ID)].F, _ID);
				}
			});
	}

	// 갈 수 없었대. 빈거 준다.
	if (Datas[static_cast<size_t>(_End)].Parent == InvalidNode)
	{
		return {};
	}

	std::vector<NodeID> Result = {};

	// 부모 역추적
	while (_End != _Start)
	{
		Result.push_back(_End);
		_End = Datas[static_cast<size_t>(_End)].Parent;
	}

	// 뒤집어서 준다. (역순이므로)
	std::reverse(Result.begin(), Result.end());
	return Result;
}
