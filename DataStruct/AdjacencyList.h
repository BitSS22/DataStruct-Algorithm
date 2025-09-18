#pragma once
#include <vector>

struct Vertex
{
public:
	Vertex() {}
	Vertex(size_t _Dest, float _Weight)
		: DestVertex(_Dest)
		, Weight(_Weight) {}
public:
	static constexpr size_t SentinelIndex = static_cast<size_t>(-1);

public:
	size_t DestVertex = SentinelIndex;
	float Weight = -1.f;

	bool ValidDest() const noexcept
	{
		return DestVertex != SentinelIndex;
	}
	bool ValidWeight() const noexcept
	{
		return Weight < 0.f;
	}
};

class AdjacencyList
{
public:
	

public:
	AdjacencyList();
	~AdjacencyList();

	AdjacencyList(const AdjacencyList& _Other) = delete;
	AdjacencyList(AdjacencyList&& _Other) noexcept = delete;

public:
	AdjacencyList& operator=(const AdjacencyList& _Other) = delete;
	AdjacencyList& operator=(AdjacencyList&& _Other) noexcept = delete;

private:
	std::vector<std::vector<Vertex>> Vertices = {};

};
