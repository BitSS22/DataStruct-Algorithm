#pragma once

#include <vector>
#include <queue>
#include <limits>
#include <cstdint>
#include <cassert>
#include <utility>
#include <functional>

#include "GraphAPI.h"

// ��δ� ã�� �ʴ´�. ��� ������ ��븸 ���.
template <GraphAPI Graph>
std::vector<typename Graph::Cost> Dijkstra(const Graph& _Graph, typename Graph::NodeID _Start)
{
	using NodeID = typename Graph::NodeID;
	using Cost = typename Graph::Cost;
	using Pair = std::pair<Cost, NodeID>;

	assert(_Graph.IsValid(_Start));

	// ���� ��ϵ� ���� ����
	// �ʱ� ���� �ִ�ġ�� �����Ѵ�.
	std::vector<Cost> Costs(_Graph.GetNodeCount(), std::numeric_limits<Cost>::max());
	std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> VisitQ = {};

	// queue�� ���� ������ �ְ�, ���� 0���� �����Ѵ�.
	Costs[static_cast<size_t>(_Start)] = static_cast<Cost>(0);
	VisitQ.emplace(static_cast<Cost>(0), _Start);

	// ���� ���� �� ���� �ݺ�.
	while (!VisitQ.empty())
	{
		// ���� ª���� �ϳ� ������.
		Pair pair = VisitQ.top();
		Cost C = pair.first;
		NodeID N = pair.second;

		VisitQ.pop();

		// ���� ���� == �ֽ� �����Ͱ� �ƴ�. �ѱ��.
		if (!(Costs[static_cast<size_t>(N)] == C))
			continue;

		// Node�� ������ �̿��� ���� ���� API �Լ�.
		_Graph.ForeachNeighbor(N, [&Costs, &VisitQ, C](NodeID _ID, Cost _Cost)
			{
				// ���� ����ġ�� ������� ����.
				assert(static_cast<Cost>(0) <= _Cost);

				Cost NewDist = C + _Cost;
				// ������ ��κ��� ���� �۴ٸ�.
				if (NewDist < Costs[static_cast<size_t>(_ID)])
				{
					// Cost�� �����ϰ�, �湮��ų NodeQ�� ���� �ִ´�.
					Costs[static_cast<size_t>(_ID)] = NewDist;
					VisitQ.emplace(NewDist, _ID);
				}
			});
	}

	// ��� ���� ���� ����� ���� vector
	return Costs;
}
