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
		// code..
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
		bool Passable;
		Cost CostMul;
	};

	struct Grid2D
	{
	public:
		unsigned int x;
		unsigned int y;
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