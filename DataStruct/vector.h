#pragma once
#include <assert.h>
#include <vector>

//typedef float T;
template <typename T>
class MyVector
{
public:
	MyVector()
		: data(nullptr)
		, count(0)
		, max_count(0)
	{
	}
	~MyVector()
	{
		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
	}

private:
	T* data;
	size_t count;
	size_t max_count;

public:
	T& at(size_t _index) const
	{
		if (_index >= count)
			assert(nullptr);

		return data[_index];
	}
	size_t capacity() const
	{
		return max_count;
	}
	size_t size() const
	{
		return count;
	}
	void clear()
	{
		count = 0;
	}

	void push_back(const T& _data)
	{
		if (count >= max_count)
		{
			size_t value = max_count * 2 <= 0 ? 1 : max_count * 2;
			reserve(value);
		}

		data[count] = _data;
		++count;
	}

	void reserve(size_t _capacity)
	{
		if (_capacity <= max_count)
			return;

		T* tempvector = data;
		data = new T[_capacity];

		for (size_t i = 0; i < count; ++i)
			data[i] = tempvector[i];

		if (tempvector != nullptr)
		{
			delete[] tempvector;
			tempvector = nullptr;
		}
		max_count = _capacity;

#ifdef _DEBUG
		printf_s("데이터 재 할당\n");
#endif // DEBUG
	}

	T& operator[] (size_t _index) const
	{
		return data[_index];
	}

};

class myclass
{
public:
	myclass()
		: ptr(nullptr)
	{
		ptr = new int;
	}
	~myclass()
	{
		if (ptr != nullptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

private:
	int* ptr;
};
void func()
{
	std::vector<float> vec = {};

	auto temp = vec.capacity();
	vec.reserve(1);
	vec.resize(1);

	MyVector<myclass> myvec = {};

	printf_s("value = %s, size = %d, capacity = %d\n", "not found data", myvec.size(), myvec.capacity());

	//myvec.reserve(3);

	for (size_t i = 0; i < 100; i++)
	{
		myvec.push_back(myclass());
		printf_s("value = %f, size = %d, capacity = %d\n", (myvec[i]), myvec.size(), myvec.capacity());
	}
	
	return;
}