#pragma once

using Type = int;

class Heap
{
public:
	Heap() {}
	Heap(Type* _Arr, size_t _Size)
		: Arr(_Arr)
		, Size(_Size)
	{

	}
	~Heap() {}

private:
	Type* Arr = nullptr;
	size_t Size = 0;
	size_t Depth = 0;

public:

private:
	size_t GetDepth(size_t _Size) noexcept
	{

	}
public:
	void SetArr(Type* _Arr, size_t _Size) noexcept
	{
		Arr = _Arr;
		Size = _Size;
	}

};