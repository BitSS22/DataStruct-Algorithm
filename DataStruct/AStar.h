#pragma once

#include <vector>
#include <queue>
#include <limits>
#include <cstdint>
#include <cassert>
#include <utility>
#include <functional>

#include "GraphAPI.h"

// ���ͽ�Ʈ��� ���� �ٸ� ���� �޸���ƽ �Լ��� �̿��Ѵٴ� ��.
// Ÿ�Ըʰ��� ������ ����ư, ��Ŭ���� �Ÿ�ó�� �޸���ƽ(����ġ)�� ������� ���� ���� �� �ִ�.
// ������ ��Ģ���� ����, ���� ����ġ�� �������� �׷����� �޸���ƽ�� ����ڰ� ������ �� �ۿ� ����.
// �޸���ƽ ����ġ�� Ŭ���� ������ �ӵ��� ������. ������ �޸���ƽ ����ġ�� 1�� �ѱ�� �ִܰŸ��� ������ ���� �ʴ´�.
// �ݴ�� �޸���ƽ�� 0�� �������� ���ͽ�Ʈ�� ���������. �޸���ƽ�� 0�̸� ��ǻ� ���ͽ�Ʈ��.
// 
// �׷��ٸ� ������ �޸���ƽ�� 1 ���Ͽ��� �ϴ���? ��� �Ѵٸ� �� �𸣰ڴ�.
// ��> ���ʸ����� �Ѵ� ���Ͱ� �÷��̾��� ��ġ�� �׻� �ִܰŸ��� ã�ư��� �� �ʿ䰡 ������?
// ��> �ִܰŸ��� �������� �ʴ��� ������ �ٿ� ����ȭ�� ������ �ִ°� ���� ������?
// ��> �׸��� �׻� �ִܰŸ��� ���� ã�°� ���� "�ڿ��������?" (�����ε� ����� �׻� �ִܰŸ��� ���� ã�� ���ϴµ�.)

// �޸���ƽ �䱸 Ÿ��
// Graph::(����) �޸���ƽ ���� -> Cost Heuristic(NodeID, NodeID) const noexcept
// ����� ���� �޸���ƽ -> Cost Heuristic(const Graph&, NodeID, NodeID) noexcept

// �׷����� Heuristic �Լ��� ������ �ִ���?
template <typename G>
concept GraphHasHeuristic = requires(const G & _G, typename G::NodeID _N)
{
	typename G::NodeID;
	typename G::Cost;

	{ _G.Heuristic(_N, _N) } -> std::convertible_to<typename G::Cost>;
};

// �⺻ �޸���ƽ Ÿ�� �ΰ�.
struct UseGraphHeuristic {};
template <GraphAPI G>
struct ZeroHeuristic
{
	static constexpr typename G::Cost operator()() const noexcept
	{
		return static_cast<G::Cost>(0);
	}
};

// Graph�� �޸���ƽ �Լ��� ������ �װɷ�, ������ 0�� ��ȯ�ϴ� �޸���ƽ�� �����ϴ� alias,
// �׸��� Astar���� template �⺻ Ÿ������ �̰� �����Ѵ�.
template <GraphAPI G>
using DefaultHeuristic = std::conditional_t<GraphHasHeuristic<G>, UseGraphHeuristic, ZeroHeuristic<G>>;

// ��Ÿ �Լ��� ���� ������ ���..
// ������ �����ϸ� G::Cost H(G, G::NodeID, G::NodeID), �Ǵ� G::Cost H(G::NodeID, G::NodeID) ���·� ȣ���� �����ϰų�,
// H�� Ÿ���� UseGraphHeuristic �̸�, G�� GraphHasHeuristic concept�� �����ؾ� �Ѵٴ� concept ����.
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
	// ���ͽ�Ʈ��� �⺻���� ��Ŀ������ ����.
	// �ٸ�, Cost�� �����Ҷ� �޸���ƽ ����ġ�� ����Ѵ�.
	// ���� �޸���ƽ�� 0�̶��, ���ͽ�Ʈ�� �ִܰ�� �˰���.
	using NodeID = typename Graph::NodeID;
	using Cost = typename Graph::Cost;
	using Pair = std::pair<Cost, NodeID>;

	assert(_Graph.IsValid(_Start) && _Graph.IsValid(_End));

	// OpenNode�� CloseNode.
	std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> OpenNodes = {};
	std::vector<NodeID> CloseNodes = {};
	CloseNodes.reserve(_Graph.GetNodeCount());
	
	CloseNodes[static_cast<size_t>(_Start)] = static_cast<Cost>(0);
	OpenNodes.emplace(static_cast<Cost>(0), _Start);

	// ���� ���� �� ���� �ݺ�.
	while (!OpenNodes.empty())
	{
		// ���� ª���� �ϳ� ������.
		Pair pair = OpenNodes.top();
		Cost C = pair.first;
		NodeID N = pair.second;

		OpenNodes.pop();

		// ���� ���� == �ֽ� �����Ͱ� �ƴ�. �ѱ��.
		if (!(CloseNodes[static_cast<size_t>(N)] == C))
			continue;

		// Node�� ������ �̿��� ���� ���� API �Լ�.
		_Graph.ForeachNeighbor(N, [&CloseNodes, &OpenNodes, C](NodeID _ID, Cost _Cost)
			{
				// ���� ����ġ�� ������� ����.
				assert(static_cast<Cost>(0) <= _Cost);

				Cost NewDist = C + _Cost;
				// ������ ��κ��� ���� �۴ٸ�.
				if (NewDist < CloseNodes[static_cast<size_t>(_ID)])
				{
					// Cost�� �����ϰ�, �湮��ų NodeQ�� ���� �ִ´�.
					CloseNodes[static_cast<size_t>(_ID)] = NewDist;
					OpenNodes.emplace(NewDist, _ID);
				}
			});
	}

	// ��� ���� ���� ����� ���� vector
	return CloseNodes;
}
