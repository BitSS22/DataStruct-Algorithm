#include <vector>
// #include "vector.h"
#include <crtdbg.h>
#include "myvec.h"
#include <iostream>
#include <malloc.h>

class myclass
{
public:
	myclass()
		:ptr(nullptr)
	{
		std::cout << "생성자" << std::endl;
		ptr = new int;
		*ptr = 100;
	}
	~myclass()
	{
		std::cout << "소멸자" << std::endl;
		if (ptr != nullptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	myclass(const myclass& other)
	{
		std::cout << "복사생성자" << std::endl;
		ptr = new int(*(other.ptr));
	}

	myclass& operator=(const myclass& other)
	{
		std::cout << "대입연산자" << std::endl;
		if (ptr != nullptr)
			delete ptr;
		ptr = new int(*(other.ptr));
		return *this;
	}

private:
	int* ptr;
};

int main()
{
	// UVector<myclass> vec = {};
	std::vector<myclass> vec = {};

	vec.reserve(10);

	myclass* ptr = reinterpret_cast<myclass*>(malloc(sizeof(myclass)*10));
	std::cout << vec.size() << std::endl << vec.capacity() << std::endl;

	return 0;
}