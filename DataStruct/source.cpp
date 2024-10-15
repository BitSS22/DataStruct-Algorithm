#include <vector>
// #include "vector.h"
#include <crtdbg.h>
#include "myvec.h"
#include <iostream>


//class myclass
//{
//public:
//	myclass()
//		: ptr(nullptr)
//	{
//		std::cout << "생성자 호출 시작" << std::endl;
//
//		ptr = new int;
//
//		++callCountConstructor;
//
//		std::cout << "생성자 호출 종료, count : " << callCountConstructor << std::endl;
//	}
//	~myclass()
//	{
//		std::cout << "소멸자 호출 시작" << std::endl;
//
//		if (ptr != nullptr)
//		{
//			delete ptr;
//			ptr = nullptr;
//		}
//
//		++callCountDestructor;
//
//		std::cout << "소멸자 호출 종료, count : " << callCountDestructor << std::endl;
//	}
//
//	myclass(const myclass& other)
//	{
//		ptr = new int;
//	}
//
//	// 복사 생성자
//	myclass(const myclass& other)
//	{
//		if (other.ptr != nullptr)
//		{
//			ptr = new int(*other.ptr); // 깊은 복사
//		}
//		else
//		{
//			ptr = nullptr;
//		}
//	}
//
//	myclass& operator=(const myclass& other)
//	{
//		ptr = new int;
//
//		return *this;
//	}
//
//	myclass& operator=(const myclass& other)
//	{
//		if (this != &other)
//		{
//			delete ptr; // 기존 메모리 해제
//			if (other.ptr != nullptr)
//			{
//				ptr = new int(*(other.ptr)); // 깊은 복사
//			}
//			else
//			{
//				ptr = nullptr;
//			}
//		}
//
//		return *this;
//	}
//
//private:
//	int* ptr;
//
//	static size_t callCountConstructor;
//	static size_t callCountCopyConstructor;
//	static size_t callCountDestructor;
//};
//
//
//class myclass
//{
//public:
//	myclass() : ptr(nullptr) {
//		std::cout << "생성자 호출 시작" << std::endl;
//		++callCountConstructor;
//		std::cout << "생성자 호출 종료, count : " << callCountConstructor << std::endl;
//	}
//
//	~myclass() {
//		std::cout << "소멸자 호출 시작" << std::endl;
//		if (ptr != nullptr) {
//			delete ptr;
//			ptr = nullptr;
//		}
//		++callCountDestructor;
//		std::cout << "소멸자 호출 종료, count : " << callCountDestructor << std::endl;
//	}
//
//	// 복사 생성자
//	myclass(const myclass& other) {
//		std::cout << "복사 생성자 호출" << std::endl;
//		if (other.ptr != nullptr) {
//			ptr = new int(*other.ptr); // 깊은 복사
//		}
//		else {
//			ptr = nullptr;
//		}
//		++callCountCopyConstructor;
//		std::cout << "복사 생성자 호출 종료, count : " << callCountCopyConstructor << std::endl;
//	}
//
//	// 이동 생성자
//	myclass(myclass&& other) noexcept : ptr(other.ptr) {
//		other.ptr = nullptr; // 이동 후 원본 객체의 포인터를 nullptr로 설정
//		std::cout << "이동 생성자 호출" << std::endl;
//	}
//
//	myclass& operator=(const myclass& other) {
//		std::cout << "복사 할당 연산자 호출" << std::endl;
//
//		if (this != &other) { // 자기 자신과의 대입 방지
//			delete ptr; // 기존 메모리 해제
//			if (other.ptr != nullptr) {
//				ptr = new int(*other.ptr); // 깊은 복사
//			}
//			else {
//				ptr = nullptr;
//			}
//		}
//		return *this;
//	}
//
//	// 이동 할당 연산자
//	myclass& operator=(myclass&& other) noexcept {
//		std::cout << "이동 할당 연산자 호출" << std::endl;
//
//		if (this != &other) { // 자기 자신과의 대입 방지
//			delete ptr; // 기존 메모리 해제
//			ptr = other.ptr; // 리소스 이동
//			other.ptr = nullptr; // 다른 객체는 더 이상 소유하지 않음
//		}
//		return *this;
//	}
//
//private:
//	int* ptr;
//	static size_t callCountConstructor;
//	static size_t callCountCopyConstructor;
//	static size_t callCountDestructor;
//};
//
//size_t myclass::callCountCopyConstructor = 0;
//size_t myclass::callCountConstructor = 0;
//size_t myclass::callCountDestructor = 0;


class myclass
{
public:
	myclass()
		:ptr(nullptr)
	{
		ptr = new int;
		*ptr = 100;
	}
	~myclass()
	{
		if (ptr != nullptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	myclass(const myclass& other)
	{
		ptr = new int(*(other.ptr));
	}

	myclass& operator=(const myclass& other)
	{
		ptr = new int(*(other.ptr));
		return *this;
	}

private:
	int* ptr;
};

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	////func();

	//UVector<myclass> vec = {};

	//vec.reserve(3);

	//for (size_t i = 0; i < 100; ++i)
	//{
	//	vec.push_back(myclass());
	//	std::cout << "i : " << i << std::endl;
	//}

	//return 0;



	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//func();









	//std::vector<myclass> vec = {};

	UVector<myclass> vec = {};

	






	
	
	// vec.reserve(1);
	// vec.reserve(2);
	// vec.reserve(3);
	// vec.reserve(4);
	// vec.reserve(5);
	// vec.reserve(6);
	// vec.reserve(7);


	for (size_t i = 0; i < 100; ++i)
	{
		std::cout << "i : " << i << " (for문 실행 시작)" << std::endl;
		vec.push_back(myclass());
		printf_s("i : %d, vec.size : %d, vec.capacity : %d \n\n", i, vec.size(), vec.capacity());
	}

	std::cout << "main 함수 종료" << std::endl;
	return 0;
}