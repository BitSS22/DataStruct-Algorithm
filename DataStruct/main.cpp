// std Test
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>

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
#include "unordered_map.h"

#include "AdjacencyListGraph.h"
#include "MatrixGraph.h"
#include "TileMapGraph.h"

// Make Algorithm
#include "Utility.h"
#include "Shuffle.h"
#include "Sort.h"

#include "DFS.h"
#include "BFS.h"
#include "Dijkstra.h"

// IO
#include <iostream>
#include <iomanip>
#include <conio.h>

// math
#include <cmath>

// Timer
#include <chrono>

void GraphTest();
void SortTest();
void Example_TileMap_AStar_Demo_Fixed();

int main()
{
	// Memory Leak Check
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	// iostream option
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	Example_TileMap_AStar_Demo_Fixed();

	// SortTest();
	//GraphTest();
}

void GraphTest()
{
	AdjacencyListGraph<unsigned int, unsigned int, UnDirected> Graph(7);

	Graph.AddEdge(1, 2, 7);
	Graph.AddEdge(1, 3, 4);
	Graph.AddEdge(1, 4, 9);

	Graph.AddEdge(2, 5, 6);
	Graph.AddEdge(2, 6, 3);

	Graph.AddEdge(3, 4, 3);

	Graph.AddEdge(4, 6, 2);

	Graph.AddEdge(5, 6, 4);

	const unsigned int StartIndex = 1;
	std::vector<unsigned int> Result = Dijkstra(Graph, StartIndex);

	for (size_t i = 0; i < Result.size(); ++i)
	{
		std::cout << StartIndex << " -> " << i << " : Cost == ";

		if (Result[i] == std::numeric_limits<unsigned int>::max())
			std::cout << "Not Found" << std::endl;
		else
			std::cout << Result[i] << std::endl;
	}
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
		Sort::QuickSort(Arr, ARRSIZE);
		//Sort::RadixSort(Arr, ARRSIZE, static_cast<int>(ARRSIZE));
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

//        .==.        .==.          
//       //`^\\      //^`\\         
//      // ^ ^\(\__/)/^ ^^\\        
//     //^ ^^ ^/6  6\ ^^ ^ \\       
//    //^ ^^ ^/( .. )\^ ^ ^ \\      
//   // ^^ ^/\| v""v |/\^ ^ ^\\     
//  // ^^/\/ /  `~~`  \ \/\^ ^\\    
//  -----------------------------
//								ũ�ƾ�



// GPT���� �׽�Ʈ�� �ڵ� ¥�޶�� ��.

// C++20 ����: TileMapGraph + A* ��� �׽�Ʈ (�밢�� On/Off ��ħ ����)
#pragma once
#include <iostream>                             // // �ܼ� ���
#include <vector>                               // // std::vector
#include <algorithm>                            // // std::find, std::reverse

#include "GraphAPI.h"                           // // GraphAPI ����/Ÿ��
#include "AStar.h"                              // // AStar(...) ����
#include "TileMapGraph.h"                       // // TileMapGraph ���ø�

// // ���ڿ� ��θ� ASCII�� ���
template <class Graph>
// �ʿ��� .h: <iostream>, <vector>, <algorithm>
static void PrintGridWithPath(const Graph& graph,
	typename Graph::Grid2D mapSize /* �� ũ�� */,
	typename Graph::NodeID start   /* ���� */,
	typename Graph::NodeID goal    /* ��ǥ */,
	const std::vector<typename Graph::NodeID>& path /* ��� */)
{
	using NodeID = typename Graph::NodeID;     /* ��� ID Ÿ�� */
	using Grid2D = typename Graph::Grid2D;     /* ��ǥ Ÿ��    */

	for (int y /* �� */ = 0; y < mapSize.y; ++y)
	{
		for (int x /* �� */ = 0; x < mapSize.x; ++x)
		{
			Grid2D cell /* ��ǥ */ = Grid2D{ x, y };
			NodeID id   /* ID  */ = graph.ConvertNodeID(cell);
			char ch     /* ���� */ = '.';

			const typename Graph::Tile& tile /* Ÿ�� */ = graph.GetTile(cell);
			if (!tile.Movable) { ch = '#'; }

			if (id == start)      ch = 'S';
			else if (id == goal)  ch = 'E';
			else
			{
				bool onPath /* ��� ���� */ =
					(std::find(path.begin(), path.end(), id) != path.end());
				if (onPath) ch = '*';
			}

			std::cout << ch;
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

// // Ÿ�ϸ� ����/��ֹ�/���� ��ġ �� A* ���� (4���� vs 8����)
static void Example_TileMap_AStar_Demo_Fixed()
{
	using G4 = TileMapGraph<DefaultCost, false>;       /* 4����(�밢�� X) */
	using G8 = TileMapGraph<DefaultCost, true >;       /* 8����(�밢�� O) */
	using Cst = typename G4::Cost;                      /* ��� Ÿ��       */
	using ID = typename G4::NodeID;                    /* ��� ID Ÿ��    */

	// --- �� ũ��� �⺻��� (�� �׷����� Grid2D�� "����" ����) ---
	G4::Grid2D size4 /* 4���� ũ�� */ = G4::Grid2D{ 12, 8 };
	G8::Grid2D size8 /* 8���� ũ�� */ = G8::Grid2D{ 12, 8 };
	Cst        baseCost /* �⺻ ��� */ = static_cast<Cst>(1);

	// --- �׷��� �ν��Ͻ� (���� Ÿ���� Grid2D�� ������ ȣ��) ---
	G4 g4 /* 4���� �׷��� */ = G4(size4, baseCost);    // // OK
	G8 g8 /* 8���� �׷��� */ = G8(size8, baseCost);    // // OK

	// --- ���� ��ġ ���� (�� �׷����� ���� ��ǥ Ÿ�� ���� ����) ---
	// �ʿ��� .h: ���� (TileMapGraph �������̽��� ���)
	auto set_wall = [](G4& g4ref, G8& g8ref, int x, int y) // // ��
		{
			g4ref.SetTile(G4::Grid2D{ x, y }, false, static_cast<Cst>(1));
			g8ref.SetTile(G8::Grid2D{ x, y }, false, static_cast<Cst>(1));
		};
	auto set_swamp = [](G4& g4ref, G8& g8ref, int x, int y, Cst mul) // // ��(����)
		{
			g4ref.SetTile(G4::Grid2D{ x, y }, true, mul);
			g8ref.SetTile(G8::Grid2D{ x, y }, true, mul);
		};

	// --- ��ֹ�: y=3 ���κ� + (3,3)�� (8,3)�� ��η� ���� ---
	for (int x /* �� */ = 1; x <= size4.x - 2; ++x)
	{
		if (x == 3 || x == 8) { continue; }
		set_wall(g4, g8, x, 3);
	}

	// --- ���� ����: ���ϴ� �簢��(��3) ---
	for (int y /* �� */ = 5; y < size4.y; ++y)
	{
		for (int x /* �� */ = 7; x < size4.x; ++x)
		{
			set_swamp(g4, g8, x, y, static_cast<Cst>(3));
		}
	}

	// --- ����/��ǥ (G4�κ��� ID ����� ����) ---
	ID start /* ���� */ = g4.ConvertNodeID(G4::Grid2D{ 1, 1 });
	ID goal  /* ��ǥ */ = g4.ConvertNodeID(G4::Grid2D{ 10, 6 });

	// --- 4���� A* ---
	// �ʿ��� .h: "AStar.h"
	std::vector<ID> path4 /* 4���� ��� */ =
		AStar(g4, start, goal);                       // // A* Ž��

	std::cout << "[TileMap A*] 4-way (Manhattan) result:\n";
	PrintGridWithPath(g4, size4, start, goal, path4);

	// --- 8���� A* ---
	// �ʿ��� .h: "AStar.h"
	std::vector<ID> path8 /* 8���� ��� */ =
		AStar(g8, start, goal);                       // // A* Ž��

	std::cout << "[TileMap A*] 8-way (Diagonal) result:\n";
	PrintGridWithPath(g8, size8, start, goal, path8);

	// --- ��� ---
	std::cout << "Path length (4-way): " << static_cast<int>(path4.size()) << "\n";
	std::cout << "Path length (8-way): " << static_cast<int>(path8.size()) << "\n";
}
