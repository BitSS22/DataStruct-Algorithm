#pragma once

struct DestWeightVertex
{
public:
	DestWeightVertex() {}
	DestWeightVertex(size_t _Dest, float _Weight)
		: Dest(_Dest)
		, Weight(_Weight) {}
public:
	static constexpr size_t SentinelIndex = static_cast<size_t>(-1);

public:
	size_t Dest = SentinelIndex;
	float Weight = -1.f;

	constexpr bool ValidDest() const noexcept
	{
		return Dest != SentinelIndex;
	}
	bool ValidDest(size_t _OtherIndex) const noexcept
	{
		return Dest != _OtherIndex;
	}
	constexpr bool ValidWeight() const noexcept
	{
		return Weight < 0.f;
	}
};

struct DestVertex
{
public:
	DestVertex() {}
	DestVertex(size_t _Dest)
		: Dest(_Dest) {}
public:
	static constexpr size_t SentinelIndex = static_cast<size_t>(-1);

public:
	size_t Dest = SentinelIndex;

	constexpr bool ValidDest() const noexcept
	{
		return Dest != SentinelIndex;
	}
	bool ValidDest(size_t _OtherIndex) const noexcept
	{
		return Dest != _OtherIndex;
	}
	constexpr bool ValidWeight() const noexcept
	{
		return false;
	}
};

struct WeightVertex
{
public:
	WeightVertex() {}
	WeightVertex(float _Weight)
		: Weight(_Weight) {}
public:
	float Weight = -1.f;

	constexpr bool ValidDest() const noexcept
	{
		return false;
	}
	constexpr bool ValidDest(size_t _OtherIndex) const noexcept
	{
		return false;
	}
	constexpr bool ValidWeight() const noexcept
	{
		return Weight < 0.f;
	}
};

struct Vertex
{
public:
	Vertex() {}
	Vertex(size_t _Value)
		: Value(_Value) {}
public:
	static constexpr size_t SentinelValue = static_cast<size_t>(-1);

public:
	size_t Value = SentinelValue;

	constexpr bool ValidDest() const noexcept
	{
		return false;
	}
	constexpr bool ValidDest(size_t _OtherIndex) const noexcept
	{
		return false;
	}
	constexpr bool ValidWeight() const noexcept
	{
		return false;
	}
};