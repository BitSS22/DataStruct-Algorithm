// std Test
#include <vector>
#include <list>
#include <map>
#include <queue>

#include <utility>

// Memory Leak Check
#include <crtdbg.h>

// Test Class (Use New, Delete)
#include "myclass.h"

// Make DataStruct
#include "vector.h"
#include "list.h"
#include "map.h"
#include "queue.h"
#include "priority_queue.h"

// Make Algorithm
#include "Utility.h"
#include "Shuffle.h"
#include "Sort.h"

// IO
#include <iostream>
#include <iomanip>
#include <conio.h>

// math
#include <cmath>

// Timer
#include <chrono>

void SortTest();

int main()
{
	// Memory Leak Check
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	
	priority_queue<int> q = {};

	q.Push(7);
	q.Push(7);
	q.Push(3);
	q.Push(1);
	q.Push(8);
	q.Push(9);

	size_t size = q.GetSize();

	for (size_t i = 0; i < size; ++i)
	{
		int value = q.Top();
		q.Pop();
	}

	vector<int> v = {};

	v.PushBack(66);
	v.PushBack(1342);
	v.PushBack(44);
	v.PushBack(1);
	v.PushBack(0);
	v.PushBack(3333);
	v.PushBack(2222);

	v.BuildHeap();

	for (size_t i = 0; i < v.GetSize(); ++i)
	{
		std::cout << v[i] << " ";
	}

	// SortTest();
}

void SortTest()
{
	// Arr Option
	constexpr size_t ARRSIZE = 11;
	constexpr size_t CarriageReturnCount = 40;
	constexpr size_t Digit = Utility::GetPrintDigit(ARRSIZE);
	
	// Escape to InputKey == ESC
	int InputKey = 0;
	constexpr int EscapeKey = 27;
	
	// GetTimes
	std::vector<double> Elapseds = {};

	while (InputKey != EscapeKey)
	{
		// MakeArr
		int Arr[ARRSIZE] = {};
		for (size_t i = 0; i < ARRSIZE; ++i)
		{
			Arr[i] = static_cast<int>(i);
		}

		// Shuffle
		ArrShuffle(Arr);

		// Input
		std::cout << "Case : " << Elapseds.size() + 1 << std::endl;

		std::cout << "Shuffle Arr" << std::endl;
		for (size_t i = 0; i < ARRSIZE; ++i)
		{
			std::cout << std::setw(Digit) << Arr[i] << " ";
			if (i % CarriageReturnCount == CarriageReturnCount - 1)
			{
				std::cout << std::endl;
			}
		}
		std::cout << "\n";

		// Time Start
		auto Start = std::chrono::steady_clock::now();

		// Sorting
		Sort::ShellSort(Arr, ARRSIZE);
		//Sort::ShellSort(Arr, ARRSIZE, [](const int& _Left, const int& _Right) noexcept -> bool { return _Left > _Right; });

		// Time End
		auto End = std::chrono::steady_clock::now();

		// Output
		std::cout << "Sort Arr" << std::endl;
		for (size_t i = 0; i < ARRSIZE; ++i)
		{
			std::cout << std::setw(Digit) << Arr[i] << " ";
			if (i % CarriageReturnCount == CarriageReturnCount - 1)
			{
				std::cout << std::endl;
			}
		}

		// Show Info
		__int64 ns = std::chrono::duration_cast<std::chrono::nanoseconds>(End - Start).count();
		double Elapsed = ns / 1e6;
		std::cout << std::endl << "Elapsed : " << Elapsed << " ms\n\n";
		std::cout << "Press ESC to Escape.\nPress Any Key is One More Test.";
		Elapseds.push_back(Elapsed);

		// Input Key
		InputKey = _getch();

		// Screen Clear
		system("cls");
	}

	// Last Info
	double Average = 0.;

	for (size_t i = 0; i < Elapseds.size(); ++i)
	{
		std::cout << "Case " << i + 1 << " Elapsed : " << Elapseds[i] << " ms" << std::endl;
		Average += Elapseds[i];
	}
	std::cout << std::endl;
	std::cout << "ArrSize : " << ARRSIZE << std::endl;
	std::cout << "Total Case : " << Elapseds.size() << std::endl;
	std::cout << "Average Elapsed : " << Average / static_cast<double>(Elapseds.size()) << " ms" << std::endl;
}