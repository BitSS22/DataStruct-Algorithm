#pragma once
#include <vector>
#include <cassert>
#include <cmath>

#include "GraphAPI.h"

template <typename Cost_Type = DefaultCost, bool Diagonal = false>
class TileMapGraph
{
	// GraphAPI
public:
	using NodeID = unsigned int;
	using Cost = Cost_Type;

	template <typename CallBack>
	void ForeachNeighbor(NodeID _Node, CallBack&& _CallBack) const noexcept
	{
		Grid2D Index = ConvertGrid2D(_Node);
		static constexpr Grid2D Dir[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

		for (size_t i = 0; i < 4; ++i)
		{
			Grid2D Finder = Index + Dir[i];

			if (InBound(Finder))
			{
				const Tile& TileRef = GetTile(Finder);

				if (TileRef.Movable)
				{
					const Cost Weight = static_cast<Cost>(BaseCost * TileRef.CostMul);
					_CallBack(ConvertNodeID(Finder), Weight);
				}
			}
		}

		if constexpr (Diagonal)
		{
			static constexpr Grid2D DDir[4] = { {1, 1}, {-1, -1}, {-1, 1}, {1, -1} };
			static constexpr Cost DiaMul = static_cast<Cost>(std::sqrt(2.0));

			for (size_t i = 0; i < 4; ++i)
			{
				Grid2D Finder = Index + DDir[i];

				if (InBound(Finder))
				{
					const Tile& TileRef = GetTile(Finder);

					if (TileRef.Movable)
					{
						const Cost Weight = static_cast<Cost>(BaseCost * TileRef.CostMul * DiaMul);
						_CallBack(ConvertNodeID(Finder), Weight);
					}
				}
			}
		}
	}
	size_t GetNodeCount() const noexcept
	{
		return static_cast<size_t>(Size.x * Size.y);
	}
	bool IsValid(NodeID _Node) const noexcept
	{
		return static_cast<size_t>(_Node) < GetNodeCount();
	}

public:
	struct Tile
	{
	public:
		bool Movable;
		Cost CostMul;
	};

	struct Grid2D
	{
	public:
		int x;
		int y;

	public:
		constexpr Grid2D operator+ (Grid2D _Other) const noexcept
		{
			return Grid2D{ x + _Other.x, y + _Other.y };
		}
		constexpr Grid2D operator- (Grid2D _Other) const noexcept
		{
			return Grid2D{ x - _Other.x, y - _Other.y };
		}

	};

public:
	TileMapGraph(Grid2D _Size, Cost _BaseCost)
		: Size(_Size)
		, Grid(_Size.x * _Size.y, Tile { true, static_cast<Cost>(1) })
		, BaseCost(_BaseCost)
	{
		assert(_Size.x > 0 && _Size.y > 0);
	}

private:
	Grid2D Size;
	std::vector<Tile> Grid;
	Cost BaseCost;

public:
	void SetTile(Grid2D _Index, bool _Passable, Cost _CostMul)
	{
		assert(InBound(_Index));
		Grid[static_cast<size_t>(_Index.y * Size.x + _Index.x)] = Tile { _Passable, _CostMul };
	}
	Cost Heuristic(NodeID _Start, NodeID _End) const noexcept
	{
		Grid2D StartNode = ConvertGrid2D(_Start);
		Grid2D EndNode = ConvertGrid2D(_End);

		Grid2D Dist = StartNode - EndNode;
		Dist.x = Dist.x < 0 ? -Dist.x : Dist.x;
		Dist.y = Dist.y < 0 ? -Dist.y : Dist.y;

		if constexpr (!Diagonal)
		{
			return static_cast<Cost>(Dist.x + Dist.y) * static_cast<Cost>(BaseCost);
		}
		else
		{
			Cost Max = Dist.x < Dist.y ? Dist.y : Dist.x;
			Cost Min = Dist.y < Dist.x ? Dist.y : Dist.x;

			return static_cast<Cost>(Min * BaseCost * static_cast<Cost>(std::sqrt(2.0)) + (Max - Min) * BaseCost);
		}
	}

public:
	bool InBound(Grid2D _Index) const noexcept
	{
		return _Index.x >= 0 && _Index.y >= 0 && _Index.x < Size.x && _Index.y < Size.y;
	}
	const Tile& GetTile(Grid2D _Index) const noexcept
	{
		return Grid[static_cast<size_t>(_Index.y * Size.x + _Index.x)];
	}
	NodeID ConvertNodeID(Grid2D _Index) const noexcept
	{
		return static_cast<NodeID>(_Index.y * Size.x + _Index.x);
	}
	Grid2D ConvertGrid2D(NodeID _Node) const noexcept
	{
		Grid2D Result;
		Result.x = static_cast<int>(_Node) % Size.x;
		Result.y = static_cast<int>(_Node) / Size.x;
		return Result;
	}
	Cost GetBaseCost() const noexcept
	{
		return BaseCost;
	}
};