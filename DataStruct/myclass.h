#pragma once
#include <iostream>

class MyClass
{
public:
	MyClass()
		: ptr(nullptr)
		, value(0)
	{
		std::cout << "������ ȣ��" << std::endl;

		ptr = new int;
	}
	MyClass(int _value)
		: ptr(nullptr)
		, value(_value)
	{
		std::cout << "������ ȣ��" << std::endl;

		ptr = new int(_value);
	}
	~MyClass()
	{
		std::cout << "�Ҹ��� ȣ��" << std::endl;

		if (ptr != nullptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}
	MyClass(const MyClass& _other)
	{
		std::cout << "��������� ȣ��" << std::endl;

		ptr = new int(*(_other.ptr));
		value = _other.value;
	}
	MyClass& operator=(const MyClass& _other)
	{
		std::cout << "���� ���Կ����� ȣ��" << std::endl;

		if (ptr == nullptr)
			ptr = new int(*(_other.ptr));
		else
			*ptr = *_other.ptr;

		value = _other.value;

		return *this;
	}
	MyClass(MyClass&& _other) noexcept
	{
		std::cout << "�̵������� ȣ��" << std::endl;

		ptr = _other.ptr;
		value = _other.value;

		_other.ptr = nullptr;
	}
	MyClass& operator=(MyClass&& _other) noexcept
	{
		std::cout << "�̵� ���Կ����� ȣ��" << std::endl;

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

