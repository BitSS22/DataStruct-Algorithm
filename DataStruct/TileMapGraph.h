#pragma once
#include <vector>
#include <cassert>

#include "GraphAPI.h"

template <typename Cost_Type = DefaultCost, bool Diagonal = false>
class TileMapGraph
{
	// GraphAPI
public:
	using NodeID = unsigned int;
	using Cost = Cost_Type;

	template <typename CallBack>
	void ForEachNeighbor(NodeID _Node, CallBack&& _CallBack) const noexcept
	{
		Grid2D Index = ConvertGrid2D(_Node);
		Grid2D Dir[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

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

		// Todo.. Diagonal Code
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
		unsigned int x;
		unsigned int y;

	public:
		Grid2D operator+(Grid2D _Other) const noexcept
		{
			return Grid2D { x + _Other.x, y + _Other.y };
		}
	};

public:
	TileMapGraph(Grid2D _Size, Cost _BaseCost)
		: Size(_Size)
		, Grid(_Size.x * _Size.y, Tile { true, static_cast<Cost>(1) })
		, BaseCost(_BaseCost) {}

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

public:
	bool InBound(Grid2D _Index) const noexcept
	{
		return _Index.x < Size.x && _Index.y < Size.y;
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
		Result.x = _Node % Size.x;
		Result.y = _Node / Size.x;
		return Result;
	}

};