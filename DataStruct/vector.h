#pragma once
#include <assert.h>

// typedef float T;
template <typename T>
class MyVector
{
public:
	MyVector()
		: DataPtr(nullptr)
		, DataCount(0)
		, MaxCount(0)
	{
	}
	~MyVector()
	{
		if (DataPtr != nullptr)
		{
			delete[] DataPtr;
			DataPtr = nullptr;
		}
	}

private:
	T* DataPtr;
	size_t DataCount;
	size_t MaxCount;

public:
	T& at(size_t _index) const
	{
		if (_index >= MaxCount)
			assert(nullptr);

		return DataPtr[_index];
	}
	size_t capacity() const
	{
		return MaxCount;
	}
	size_t size() const
	{
		return DataCount;
	}
	void clear()
	{
		DataCount = 0;
	}

	void push_back(const T& _data)
	{
		if (DataCount >= MaxCount)
		{
			if (MaxCount < 4)
				reserve(MaxCount + 1);
			else
				reserve(static_cast<size_t>(MaxCount * 1.5));
		}

		DataPtr[DataCount] = _data;
		++DataCount;
	}

	void reserve(size_t _capacity)
	{
		if (MaxCount >= _capacity)
			return;

		if (MaxCount == 0)
		{
			DataPtr = new T[_capacity];
		}
		else
		{
			T* PrevDataPtr = DataPtr;
			DataPtr = new T[_capacity];

			for (size_t i = 0; i < DataCount; ++i)
				DataPtr[i] = PrevDataPtr[i];

			delete[] PrevDataPtr;
		}

		MaxCount = _capacity;
	}

	T& operator[] (size_t _index) const
	{
		return DataPtr[_index];
	}

private:

};

