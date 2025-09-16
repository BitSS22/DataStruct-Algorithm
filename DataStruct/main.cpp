// std Test
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <unordered_set>

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

	// iostream option
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	std::unordered_set<int> s;

	std::hash<MyClass>;


	SortTest();
}

void SortTest()
{
	// Arr Option
	constexpr size_t ARRSIZE = 10000;
	constexpr size_t CarriageReturnCount = 30;
	constexpr size_t Digit = Utility::PrintDigitCount(ARRSIZE);
	constexpr bool PrintNum = true;
	
	// Escape InputKey == ESC
	int InputKey = 0;
	constexpr int EscapeKey = 27;

	// MakeArr
	int* Arr = new int[ARRSIZE];
	for (size_t i = 0; i < ARRSIZE; ++i)
	{
		Arr[i] = static_cast<int>(i);
	}

	// GetTime
	std::vector<double> Elapseds = {};

	while (InputKey != EscapeKey)
	{
		// Shuffle
		ArrShuffle(Arr, ARRSIZE);

		// Input
		std::cout << "Case : " << Elapseds.size() + 1 << std::endl;

		if constexpr (PrintNum)
		{
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
		}

		// Time Start
		auto Start = std::chrono::steady_clock::now();

		// Sorting
		//Sort::BubbleSort(Arr, ARRSIZE);
		//Sort::InsertSort(Arr, ARRSIZE);
		//Sort::SelectionSort(Arr, ARRSIZE);
		//Sort::MergeSort(Arr, ARRSIZE);
		//Sort::ShellSort(Arr, ARRSIZE);
		//Sort::HeapSort(Arr, ARRSIZE);
		//Sort::QuickSort(Arr, ARRSIZE);
		Sort::RadixSort(Arr, ARRSIZE, static_cast<int>(ARRSIZE));
		//Sort::ShellSort(Arr, ARRSIZE, [](const int& _Left, const int& _Right) noexcept -> bool { return _Left > _Right; });

		// Time End
		auto End = std::chrono::steady_clock::now();

		// Output
		if constexpr (PrintNum)
		{
			std::cout << "\nSort Arr" << std::endl;
			for (size_t i = 0; i < ARRSIZE; ++i)
			{
				std::cout << std::setw(Digit) << Arr[i] << " ";
				if (i % CarriageReturnCount == CarriageReturnCount - 1)
				{
					std::cout << std::endl;
				}
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

	delete[] Arr;
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