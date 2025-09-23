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
//								크아앙



// GPT한테 테스트할 코드 짜달라고 함.

// C++20 예제: TileMapGraph + A* 경로 테스트 (대각선 On/Off 고침 버전)
#pragma once
#include <iostream>                             // // 콘솔 출력
#include <vector>                               // // std::vector
#include <algorithm>                            // // std::find, std::reverse

#include "GraphAPI.h"                           // // GraphAPI 개념/타입
#include "AStar.h"                              // // AStar(...) 선언
#include "TileMapGraph.h"                       // // TileMapGraph 템플릿

// // 격자와 경로를 ASCII로 출력
template <class Graph>
// 필요한 .h: <iostream>, <vector>, <algorithm>
static void PrintGridWithPath(const Graph& graph,
	typename Graph::Grid2D mapSize /* 맵 크기 */,
	typename Graph::NodeID start   /* 시작 */,
	typename Graph::NodeID goal    /* 목표 */,
	const std::vector<typename Graph::NodeID>& path /* 경로 */)
{
	using NodeID = typename Graph::NodeID;     /* 노드 ID 타입 */
	using Grid2D = typename Graph::Grid2D;     /* 좌표 타입    */

	for (int y /* 행 */ = 0; y < mapSize.y; ++y)
	{
		for (int x /* 열 */ = 0; x < mapSize.x; ++x)
		{
			Grid2D cell /* 좌표 */ = Grid2D{ x, y };
			NodeID id   /* ID  */ = graph.ConvertNodeID(cell);
			char ch     /* 문자 */ = '.';

			const typename Graph::Tile& tile /* 타일 */ = graph.GetTile(cell);
			if (!tile.Movable) { ch = '#'; }

			if (id == start)      ch = 'S';
			else if (id == goal)  ch = 'E';
			else
			{
				bool onPath /* 경로 여부 */ =
					(std::find(path.begin(), path.end(), id) != path.end());
				if (onPath) ch = '*';
			}

			std::cout << ch;
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

// // 타일맵 생성/장애물/고비용 배치 후 A* 실행 (4방향 vs 8방향)
static void Example_TileMap_AStar_Demo_Fixed()
{
	using G4 = TileMapGraph<DefaultCost, false>;       /* 4방향(대각선 X) */
	using G8 = TileMapGraph<DefaultCost, true >;       /* 8방향(대각선 O) */
	using Cst = typename G4::Cost;                      /* 비용 타입       */
	using ID = typename G4::NodeID;                    /* 노드 ID 타입    */

	// --- 맵 크기와 기본비용 (각 그래프의 Grid2D로 "각각" 생성) ---
	G4::Grid2D size4 /* 4방향 크기 */ = G4::Grid2D{ 12, 8 };
	G8::Grid2D size8 /* 8방향 크기 */ = G8::Grid2D{ 12, 8 };
	Cst        baseCost /* 기본 비용 */ = static_cast<Cst>(1);

	// --- 그래프 인스턴스 (각자 타입의 Grid2D로 생성자 호출) ---
	G4 g4 /* 4방향 그래프 */ = G4(size4, baseCost);    // // OK
	G8 g8 /* 8방향 그래프 */ = G8(size8, baseCost);    // // OK

	// --- 지형 배치 헬퍼 (두 그래프에 각각 좌표 타입 맞춰 세팅) ---
	// 필요한 .h: 없음 (TileMapGraph 인터페이스만 사용)
	auto set_wall = [](G4& g4ref, G8& g8ref, int x, int y) // // 벽
		{
			g4ref.SetTile(G4::Grid2D{ x, y }, false, static_cast<Cst>(1));
			g8ref.SetTile(G8::Grid2D{ x, y }, false, static_cast<Cst>(1));
		};
	auto set_swamp = [](G4& g4ref, G8& g8ref, int x, int y, Cst mul) // // 늪(고비용)
		{
			g4ref.SetTile(G4::Grid2D{ x, y }, true, mul);
			g8ref.SetTile(G8::Grid2D{ x, y }, true, mul);
		};

	// --- 장애물: y=3 가로벽 + (3,3)과 (8,3)을 통로로 남김 ---
	for (int x /* 열 */ = 1; x <= size4.x - 2; ++x)
	{
		if (x == 3 || x == 8) { continue; }
		set_wall(g4, g8, x, 3);
	}

	// --- 고비용 지형: 우하단 사각형(×3) ---
	for (int y /* 행 */ = 5; y < size4.y; ++y)
	{
		for (int x /* 열 */ = 7; x < size4.x; ++x)
		{
			set_swamp(g4, g8, x, y, static_cast<Cst>(3));
		}
	}

	// --- 시작/목표 (G4로부터 ID 계산해 재사용) ---
	ID start /* 시작 */ = g4.ConvertNodeID(G4::Grid2D{ 1, 1 });
	ID goal  /* 목표 */ = g4.ConvertNodeID(G4::Grid2D{ 10, 6 });

	// --- 4방향 A* ---
	// 필요한 .h: "AStar.h"
	std::vector<ID> path4 /* 4방향 경로 */ =
		AStar(g4, start, goal);                       // // A* 탐색

	std::cout << "[TileMap A*] 4-way (Manhattan) result:\n";
	PrintGridWithPath(g4, size4, start, goal, path4);

	// --- 8방향 A* ---
	// 필요한 .h: "AStar.h"
	std::vector<ID> path8 /* 8방향 경로 */ =
		AStar(g8, start, goal);                       // // A* 탐색

	std::cout << "[TileMap A*] 8-way (Diagonal) result:\n";
	PrintGridWithPath(g8, size8, start, goal, path8);

	// --- 요약 ---
	std::cout << "Path length (4-way): " << static_cast<int>(path4.size()) << "\n";
	std::cout << "Path length (8-way): " << static_cast<int>(path8.size()) << "\n";
}
