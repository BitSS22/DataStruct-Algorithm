#pragma once
#include <assert.h>
#include <vector>

// template <typename T>
class MyVector
{
public:
	MyVector()
		: data(nullptr)
		, count(0)
		, max_count(1)
	{
		data = new float[max_count];
	}
	~MyVector()
	{
		Release();
	}

private:
	float* data;
	size_t count;
	size_t max_count;

public:
	float& operator[] (size_t _index) const { return data[_index]; }
	float& at(size_t _index) const
	{
		if (_index >= count)
			assert(nullptr);

		return data[_index];
	}
	size_t capacity()
	{
		return max_count;
	}
	size_t size()
	{
		return count;
	}
	void clear()
	{
		count = 0;
	}

	void push_back(const float& _data)
	{
		if (count >= max_count)
			resize(max_count * 2);

		data[count] = _data;
		++count;
	}

	void resize(size_t _capacity)
	{
		float* tempvector = new float[_capacity];
		
		for (size_t i = 0; i < count; ++i)
			tempvector[i] = data[i];

		delete data;
		data = tempvector;
		max_count = _capacity;
	}

private:
	void Release()
	{
		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
	}
};

void func()
{
	std::vector<float> vec = {};

	auto temp = vec.capacity();
	vec.reserve(1);
	vec.resize(1);

	MyVector myvec = {};
	
	for (size_t i = 0; i < 100; i++)
		myvec.push_back(static_cast<float>(i));
	
	for (size_t i = 0; i < 100; i++)
		printf_s("%f\n", myvec[i]);
	
	return;
}