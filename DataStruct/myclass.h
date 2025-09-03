#pragma once
#include <iostream>

class MyClass
{
public:
	MyClass()
		: ptr(nullptr)
		, value(0)
	{
		std::cout << "생성자 호출" << std::endl;

		ptr = new int;
	}
	MyClass(int _value)
		: ptr(nullptr)
		, value(_value)
	{
		std::cout << "생성자 호출" << std::endl;

		ptr = new int(_value);
	}
	~MyClass()
	{
		std::cout << "소멸자 호출" << std::endl;

		if (ptr != nullptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}
	MyClass(const MyClass& _other)
	{
		std::cout << "복사생성자 호출" << std::endl;

		ptr = new int(*(_other.ptr));
		value = _other.value;
	}
	MyClass& operator=(const MyClass& _other)
	{
		std::cout << "복사 대입연산자 호출" << std::endl;

		if (ptr == nullptr)
			ptr = new int(*(_other.ptr));
		else
			*ptr = *_other.ptr;

		value = _other.value;

		return *this;
	}
	MyClass(MyClass&& _other) noexcept
	{
		std::cout << "이동생성자 호출" << std::endl;

		ptr = _other.ptr;
		value = _other.value;

		_other.ptr = nullptr;
	}
	MyClass& operator=(MyClass&& _other) noexcept
	{
		std::cout << "이동 대입연산자 호출" << std::endl;

		if (this != &_other)
		{
			if (ptr != nullptr)
			{
				delete ptr;
				ptr = nullptr;
			}

			ptr = _other.ptr;
			value = _other.value;

			_other.ptr = nullptr;
		}

		return *this;
	}
	bool operator<(const MyClass& _other)
	{
		return *ptr < *(_other.ptr);
	}
	bool operator==(const MyClass& _other)
	{
		return *ptr == *(_other.ptr);
	}

	int GetValue()
	{
		return value;
	}


private:
	int* ptr;
	int value;
};

