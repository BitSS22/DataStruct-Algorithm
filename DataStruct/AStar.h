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
	static constexpr typename G::Cost operator()(const G&, typename G::NodeID, typename G::NodeID) const noexcept
	{
		return static_cast<typename G::Cost>(0);
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

// �޸���ƽ �Լ��� ȣ���ϴ� ����� ������.
struct HeuristicInvoke
{
	// UseGraphHeuristic�̸� ����� ������.
	template <GraphAPI G>
	static typename G::Cost Invoke(UseGraphHeuristic, const G& _Graph, typename G::NodeID _Start, typename G::NodeID _End)
	{
		return static_cast<typename G::Cost>(_Graph.Heuristic(_Start, _End));
	}

	// �� �ܿ� �������� ����.
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
	// ���ͽ�Ʈ��� �⺻���� ��Ŀ������ ����.
	// �ٸ�, Cost�� �����Ҷ� �޸���ƽ ����ġ�� ����Ѵ�.
	// ���� �޸���ƽ�� 0�̶��, ���ͽ�Ʈ�� �ִܰ�� �˰���.
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

	// �����͸� ������ ���� �ʿ��ϴ�.
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

		// �̹� ���� ���.
		if (Datas[static_cast<size_t>(CurNode)].Close)
		{
			continue;
		}

		Datas[static_cast<size_t>(CurNode)].Close = true;

		// ����!
		if (CurNode == _End)
		{
			break;
		}

		_Graph.ForeachNeighbor(CurNode, [CurNode, &Datas, &Open, &_Graph, &_Heuristic, _End](NodeID _ID, Cost _Cost)
			{
				// �̹� ���� ���.
				if (Datas[static_cast<size_t>(_ID)].Close)
					return;

				Cost CurG = Datas[static_cast<size_t>(CurNode)].G + _Cost;

				// �� �� �� ����� ������ ��ϵ� ��뺸�� ������.
				if (CurG < Datas[static_cast<size_t>(_ID)].G)
				{
					// Data�� ���� ������ �ش�.
					Datas[static_cast<size_t>(_ID)].G = CurG;
					Datas[static_cast<size_t>(_ID)].F = CurG + HeuristicInvoke::Invoke(_Heuristic, _Graph, _ID, _End);
					Datas[static_cast<size_t>(_ID)].Parent = CurNode;
					// �׸��� Open�� ����ִ´�.
					Open.emplace(Datas[static_cast<size_t>(_ID)].F, _ID);
				}
			});
	}

	// �� �� ������. ��� �ش�.
	if (Datas[static_cast<size_t>(_End)].Parent == InvalidNode)
	{
		return {};
	}

	std::vector<NodeID> Result = {};

	// �θ� ������
	while (_End != _Start)
	{
		Result.push_back(_End);
		_End = Datas[static_cast<size_t>(_End)].Parent;
	}

	// ����� �ش�. (�����̹Ƿ�)
	std::reverse(Result.begin(), Result.end());
	return Result;
}
