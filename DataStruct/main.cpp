// std Library
#include <vector>
#include <list>
#include <map>

// Memory Leak Check
#include <crtdbg.h>

// Test Class (Use New, Delete)
#include "myclass.h"

// Make DataStruct
#include "vector.h"
#include "list.h"
#include "map.h"

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

	SortTest();
}

void SortTest()
{
	// Arr Option
	constexpr size_t ARRSIZE = 5;
	constexpr size_t CarriageReturnCount = 10;
	constexpr int Digit = Utility::GetDigit<ARRSIZE>();

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
		Sort::MergeSort(Arr);
		//Sort::SelectionSort(Arr, [](const int& _Left, const int& _Right) -> bool { return _Left > _Right; });

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