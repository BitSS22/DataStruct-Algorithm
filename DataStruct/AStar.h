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



template <GraphAPI Graph>
std::vector<typename Graph::Cost> AStar(const Graph& _Graph, typename Graph::NodeID _Start, typename Graph::NodeID _End)
{
	std::is_function

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
