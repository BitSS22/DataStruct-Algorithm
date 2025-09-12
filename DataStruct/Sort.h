#pragma once

#include <assert.h>
#include "Utility.h"

// Debug def
 using Type = int;
 using Compare = std::less<Type>;

namespace Sort
{
#pragma region Sort Info
	// 매개 변수로 배열, 정렬 규칙 함수를 받는다.
	// 배열의 길이는 템플릿 타입 추론으로 구한다.
	// 함수 템플릿 오버로드로 기본 함수는 DefaultCompare(내림차순)를 사용한다.
	// (람다, 펑터, 함수 포인터 등을 전부 받을 수 있도록 템플릿 오버로드)
	// 함수 포인터가 nullptr이면 assert.

#pragma endregion
#pragma region Bubble
	template <typename Type, typename Compare = std::less<Type>>
	void BubbleSort(Type _Arr[], size_t _Size, Compare _Comp = Compare{}) noexcept
	{
		// 남은 원소 1 이하 == return
		while (_Size > 1)
		{
			// 정렬이 일어났는지를 체크해 최적화 할 수 있겠지만, 안하겠다. ex) bool Sorting = false;
			for (size_t i = 0; i < _Size - 1; ++i)
			{
				// 다음 원소와 현재 원소 비교, true == Swap
				if (_Comp(_Arr[i + 1], _Arr[i]))
					Utility::Swap(_Arr[i + 1], _Arr[i]);
			}
			// 마지막 원소는 정렬 된 원소이므로 제외
			--_Size;
		}
	}
#pragma endregion
#pragma region Insert
	template <typename Type, typename Compare = std::less<Type>>
	void InsertSort(Type _Arr[], size_t _Size, Compare _Comp = Compare{}) noexcept
	{
		// 첫번째 원소는 정렬 된 것으로 취급
		for (size_t i = 1; i < _Size; ++i)
		{
			// 현재 순회하는 인덱스 위치와 값을 받아둔다.
			size_t Iter = i;
			Type Inst = Utility::Move(_Arr[Iter]);

			// (정렬 된) 이전 값과 비교해서 이전 값을 밀어냄.
			while (Iter > 0 && _Comp(Inst, _Arr[Iter - 1]))
			{
				_Arr[Iter] = _Arr[Iter - 1];
				--Iter;
			}

			// 적절한 위치에 이동.
			_Arr[Iter] = Utility::Move(Inst);
		}
	}
#pragma endregion
#pragma region Selection
	template <typename Type, typename Compare = std::less<Type>>
	void SelectionSort(Type _Arr[], size_t _Size, Compare _Comp = Compare{}) noexcept
	{
		// 모든 원소 순회
		for (size_t i = 0; i < _Size; ++i)
		{
			// 가장 _Comp에 맞는 원소를 찾는다.
			size_t FindIndex = i;

			// j = i + 1 -> 자기 자신과 비교할 필요 없음
			for (size_t j = i + 1; j < _Size; ++j)
			{
				if (_Comp(_Arr[j], _Arr[FindIndex]))
					FindIndex = j;
			}

			// 찾은 원소를 i번째 원소와 교환한다.
			// 자기 자신이면 교환하지 않음.
			if (FindIndex != i)
			{
				Utility::Swap(_Arr[i], _Arr[FindIndex]);
			}
		}
	}
#pragma endregion
#pragma region Merge
	template <typename Type, typename Compare = std::less<Type>>
	class MergeClass
	{
	public:
		// 생성자를 delete해 객체를 만들 수 없게 하고,
		// Function friend와 private로 다른 함수에서 호출할 수 없게 만든다.
		MergeClass() = delete;
		template <typename T, typename Cmp>
		friend void MergeSort(T _Arr[], size_t _Size, Cmp _Comp) noexcept;

	private:
		// 정렬을 수행할 메모리를 저장할 포인터.
		// 기존 Arr, NewArr, Comp를 저장해 함수 인자 갯수를 줄일 것이다.
		// 배열 내의 객체를 이동, 복사를 최소화 하기 위해 index 기반으로 정렬 한다.
		// 당연히 데이터 영역에 하나만 있으므로, 쓰레드 언세이프 할 것이다.
		inline static Type* Arr = nullptr; // 만약 변수를 같이 쓰고 싶다면 void*로 관리하고, 캐스팅 함수를 제공하는 것도 좋은 방법이 될수도.
		inline static size_t* Index = nullptr;
		inline static size_t* Buffer = nullptr;
		inline static Compare* CompPtr = nullptr;

		// 실제 병합, 분할을 하는 함수
		static void Merge(size_t _StartIndex, size_t _Size) noexcept
		{
			// 배열 길이를 반으로 자른다. 분할정복 알고리즘.
			// 코드 가독성을 위해 변수를 많이 선언하겠다.
			size_t LeftStart = _StartIndex;
			size_t LeftSize = _Size / 2;
			size_t RightStart = LeftStart + LeftSize;
			size_t RightSize = _Size - LeftSize;

			// 배열 길이가 1이 될 때 까지 계속 나눈다.
			if (LeftSize > 1)
				Merge(LeftStart, LeftSize);

			if (RightSize > 1)
				Merge(RightStart, RightSize);

			// 여기부터는 최초로 올 때 나눈 배열 길이가 각각 1일 것.
			// 이제부터 병합을 시작한다.

			// 각각 배열의 인덱스를 따로 카운트 한다.
			size_t LeftIter = 0;
			size_t RightIter = 0;
			// 실제 기록 될 위치
			size_t WriteIndex = LeftStart;

			// 두 배열의 원소가 둘 다 남아있는가?
			while (LeftIter < LeftSize && RightIter < RightSize)
			{
				const Type& Left = Arr[Index[LeftStart + LeftIter]];
				const Type& Right = Arr[Index[RightStart + RightIter]];

				// Comp 함수가 true / false에 따라.
				if ((*CompPtr)(Right, Left))
				{
					Buffer[WriteIndex] = Index[RightStart + RightIter];
					++RightIter;
				}
				else
				{
					Buffer[WriteIndex] = Index[LeftStart + LeftIter];
					++LeftIter;
				}

				// 들어갔으니 다음 복사 될 위치는 다음 인덱스로
				++WriteIndex;
			}

			// 위 반복문이 끝나면 한쪽 배열에만 원소가 남아 있을 것.
			// 나머지 남은 요소들을 채워넣는다.
			// Left
			while (LeftIter < LeftSize)
			{
				Buffer[WriteIndex] = Index[LeftStart + LeftIter];
				++WriteIndex;
				++LeftIter;
			}
			// Right
			while (RightIter < RightSize)
			{
				Buffer[WriteIndex] = Index[RightStart + RightIter];
				++WriteIndex;
				++RightIter;
			}

			// Index 정렬
			for (size_t i = _StartIndex; i < _StartIndex + _Size; ++i)
			{
				Index[i] = Buffer[i];
			}
		}
	};

	template <typename Type, typename Compare = std::less<Type>>
	void MergeSort(Type _Arr[], size_t _Size, Compare _Comp = Compare{}) noexcept
	{
		// Size 값이 1 이하면 할 필요도 없다.
		if (1 >= _Size)
			return;

		// 같은 사이즈의 인덱스와 버퍼 배열을 하나씩 만들었다.
		MergeClass<Type, Compare>::Index = new size_t[_Size];
		MergeClass<Type, Compare>::Buffer = new size_t[_Size];

		// Index의 인덱스 값 초기화
		for (size_t i = 0; i < _Size; ++i)
		{
			MergeClass<Type, Compare>::Index[i] = i;
		}

		// MergeClass 변수 값을 설정해준다.
		MergeClass<Type, Compare>::Arr = _Arr;
		MergeClass<Type, Compare>::CompPtr = &_Comp;

		// Index 정렬은 여기서 수행.
		MergeClass<Type, Compare>::Merge(0, _Size);

		// 여기서 얻은 Index는 Index[i] == _Arr[i]가 어느 인덱스로 가야 하는가?

		// Buffer에 가야 할 위치의 정보를 집어넣는다.
		for (size_t k = 0; k < _Size; ++k)
			MergeClass<Type, Compare>::Buffer[MergeClass<Type, Compare>::Index[k]] = k;

		// 인덱스 기반으로 Arr 정렬 임시 객체 없이 swap.
		for (size_t i = 0; i < _Size; ++i)
		{
			// 현재 배열의 위치와 가야 할 위치가 맞는지 체크한다.
			// 체크 후 정확하다면 다음 인덱스로. 맞는 인덱스가 올때까지 반복.
			while (i != MergeClass<Type, Compare>::Buffer[i])
			{
				// j == 어디로 가야 하는가?
				size_t j = MergeClass<Type, Compare>::Buffer[i];
				Utility::Swap(_Arr[i], _Arr[j]);
				Utility::Swap(MergeClass<Type, Compare>::Buffer[i], MergeClass<Type, Compare>::Buffer[j]); // 실제 배열 swap 후 동기화 작업.
			}
		}

		// 할당한 메모리 해제.
		// 만약 같은 배열의 정렬을 자주 사용한다면 해제는 프로세스 종료시 마지막에 한번만 하는 것도 고려해봄직 하다.
		// 기존의 메모리 크기를 기억해두고, 모자라면 재할당하고, 충분하다면 재사용.
		// 다만 템플릿 함수이므로 여러 종류의 함수와 클래스가 만들어지면 메모리 누수가 크게 날 수 있을 것 같다.
		// 아니라면 안에서 동적 할당을 하지 않고, 임시 버퍼를 밖에서 인자로 받아 오게 해도 괜찮은 방법일수도 있다.
		delete[] MergeClass<Type, Compare>::Index;
		delete[] MergeClass<Type, Compare>::Buffer;

		// 머문 자리도 아름답게.
		MergeClass<Type, Compare>::Arr = nullptr;
		MergeClass<Type, Compare>::Index = nullptr;
		MergeClass<Type, Compare>::Buffer = nullptr;
		MergeClass<Type, Compare>::CompPtr = nullptr;
	}

#pragma endregion
#pragma region Shell
	// Ciura 수열. 실험적으로 찾아낸 ShellSort에 효율적인 수열.
	// 701 이상 값은 모른다. 알아서 하자.
	constexpr size_t CiuraSequence[] = { 701, 301, 132, 57, 23, 10, 4, 1, 0 };

	// Insert 함수에서 긁어왔다.
	// ShellSort는 결국 InsertSort의 반복.
	template <typename Type, typename Compare = std::less<Type>>
	void ShellSort(Type _Arr[], size_t _Size, Compare _Comp = Compare{}) noexcept
	{
		// 배열 크기 1 이하면 정렬할 필요도 없다.
		if (2 > _Size)
			return;

		// 시작할 적절한 Gap을 찾는다.
		size_t Ciura = 0;
		while (_Size < CiuraSequence[Ciura])
			++Ciura;

		// Gap == 1 은 InsertSort.
		while (CiuraSequence[Ciura] > 0)
		{
			// Gap Index까지(부분 배열의 첫번째 원소들)은 정렬 된 것.
			size_t Gap = CiuraSequence[Ciura];

			// i가 Gap만큼 커지면 부분 배열들의 원소 하나씩 확인 한 것.
			for (size_t i = Gap; i < _Size; ++i)
			{
				size_t Iter = i;
				Type Inst = std::move(_Arr[Iter]);

				// Iter가 Gap보다 작으면 음수 인덱스 접근.
				while (Iter >= Gap && _Comp(Inst, _Arr[Iter - Gap]))
				{
					_Arr[Iter] = std::move(_Arr[Iter - Gap]);
					Iter -= Gap; // Iter는 Gap만큼 이동해야 부분 배열을 순회한다.
				}

				_Arr[Iter] = std::move(Inst);
			}
			// 다음 부분 배열로.
			++Ciura;
		}
	}
#pragma endregion
#pragma region Heap
	// 만들어놓은 Heap 알고리즘 그대로 가져와야겠다.
	// 정적 배열에 맞게 코드 그냥 살짝만 바꾼다.
	template <typename Type, typename Compare = std::less<Type>>
	void HeapifyDown(Type _Arr[], size_t _Size, size_t _Index, Compare _Comp = Compare{})
	{
		if (_Size < 2)
		{
			return;
		}

		assert(_Index < _Size);

		while (true)
		{
			size_t Best = _Index;
			size_t LeftChild = _Index * 2 + 1;
			size_t RightChild = _Index * 2 + 2;

			if (LeftChild < _Size && _Comp(_Arr[Best], _Arr[LeftChild]))
			{
				Best = LeftChild;
			}
			if (RightChild < _Size && _Comp(_Arr[Best], _Arr[RightChild]))
			{
				Best = RightChild;
			}

			if (Best == _Index)
			{
				break;
			}

			Utility::Swap(_Arr[Best], _Arr[_Index]);
			_Index = Best;
		}
	}

	template <typename Type, typename Compare = std::less<Type>>
	void BuildHeap(Type _Arr[], size_t _Size, Compare _Comp = Compare{})
	{
		if (_Size < 2)
		{
			return;
		}

		for (size_t i = _Size / 2; i > 0; --i)
		{
			HeapifyDown(_Arr, _Size, i - 1, _Comp);
		}
	}

	template <typename Type, typename Compare = std::less<Type>>
	void HeapSort(Type _Arr[], size_t _Size, Compare _Comp = Compare{})
	{
		if (_Size < 2)
		{
			return;
		}

		// 배열을 힙화 시킨다.
		BuildHeap(_Arr, _Size, _Comp);

		// 남은 배열 크기가 1이 될때까지.
		while (_Size > 1)
		{
			// 마지막 원소와 자리를 바꾼다.
			Utility::Swap(_Arr[0], _Arr[--_Size]);
			// 맨 처음으로 온 원소를 다시 힙 구조를 맞춘다.
			HeapifyDown(_Arr, _Size, 0, _Comp);
		}
	}

#pragma endregion
#pragma region Quick
	// QuickSort Logic.
	// 1. Pivot을 하나 정한다.
	// 2. 양쪽에서 동시에 값을 읽는다.
	// 3. Pivot보다 작은값, Pivot보다 큰 값을 찾았다면 둘의 위치를 바꾼다.
	// 4. 읽는 두개의 포인터가 만난다면 Pivot과 만난 위치를 바꾼다.
	// 5. Pivot을 제외한 두 배열에 대해 1~4를 정렬 될 때 까지 반복한다.
	// 
	// 만약 피봇이 최소값, 최대값처럼 치우친 값이라면 정렬의 효율이 많이 떨어질 수 있다.
	// Pivot을 시작, 중간, 끝에서 세개를 잡고 중간 값을 사용하는 Median of Three 같은 방법이 존재하며,
	// InsertSort를 섞어 쓰는 방법도 존재한다.
	// Partition을 크다, 같다, 작다로 세개로 나누는 QuickSort는 중복키가 많을때 효율적.
	// Median of Three만 적용해서 구현 해 본다.
	// 
	// _Low, _High는 유효한 인덱스임을 보장해야 한다.
	template <typename Type, typename Compare = std::less<Type>>
	bool QuickSortMedianOfThree(Type _Arr[], size_t _Low, size_t _High, Compare _Comp = Compare{})
	{
		// 말도 안돼.
		assert(_Low <= _High);

		size_t size = _High - _Low + 1;
		// 정렬 해야 할 원소의 크기가 2개 이하라면 그냥 냅둔다..
		if (size < 3)
		{
			// 성공 여부를 반환 한다.
			return false;
		}

		// 중앙 인덱스를 찾는다.
		size_t _Mid = (_Low + _High) / 2;

		// 각각의 값을 받는다.
		Type& Low = _Arr[_Low];
		Type& Mid = _Arr[_Mid];
		Type& High = _Arr[_High];

		// 각각 값을 비교해 Low < Mid < High를 만족 시킨다.
		if (_Comp(Mid, Low))
			Utility::Swap(Mid, Low);
		if (_Comp(High, Low))
			Utility::Swap(High, Low);
		if (_Comp(High, Mid))
			Utility::Swap(High, Mid);

		// Low 다음 인덱스의 값과 Mid 값을 교체.
		Utility::Swap(_Arr[_Low + 1], _Arr[_Mid]);

		return true;
	}

	template <typename Type, typename Compare = std::less<Type>>
	void QuickSortPartition(Type _Arr[], size_t _Low, size_t _High, Compare _Comp = Compare{})
	{
		// 2개 이하
		if (!QuickSortMedianOfThree(_Arr, _Low, _High, _Comp))
		{
			// 원소가 2개라면 Swap으로 정리한다.
			if (_High - _Low + 1 == 2 && _Comp(_Arr[_High], _Arr[_Low]))
				Utility::Swap(_Arr[_High], _Arr[_Low]);
			return;
		}

		// _Low, _High는 이미 Pivot과 대소 관계가 명확해짐.
		Type& Pivot = _Arr[_Low + 1];
		size_t Low = _Low + 2;
		size_t High = _High - 1;

		// 3개 이상이면 정렬한다.
		while (true)
		{
			// 양쪽에서 알맞은 값을 찾는다.
			// 여기서 Low < Mid < High가 보장 되었기 때문에 while 탈출 조건은 항상 존재.
			while (_Comp(Pivot, _Arr[High]))
			{
				--High;
			}
			while (_Comp(_Arr[Low], Pivot))
			{
				++Low;
			}
			
			// 교차 했다면 break.
			if (Low >= High)
				break;

			// 그게 아니라면 자리를 바꿀 원소가 존재한다.
			Utility::Swap(_Arr[Low++], _Arr[High--]);
		}

		// 마지막으로 만난 중앙과 Pivot의 값을 바꾼다.
		Utility::Swap(Pivot, _Arr[High]);

		// 분할 된 두개의 파티션을 다시 QuickSorting 한다. 여기서 Pivot은 이미 정렬 되었다.
		QuickSortPartition(_Arr, _Low, High - 1, _Comp);
		QuickSortPartition(_Arr, High + 1, _High, _Comp);
	}

	template <typename Type, typename Compare = std::less<Type>>
	void QuickSort(Type _Arr[], size_t _Size, Compare _Comp = Compare{})
	{
		if (_Size < 2)
		{
			return;
		}

		QuickSortPartition(_Arr, 0, _Size - 1, _Comp);
	}
#pragma endregion
#pragma region Radix
	template <typename Type, typename Compare = std::less<Type>>
	void RadixSort(Type _Arr[], size_t _Size, Compare _Comp = Compare{})
	{
		if (_Size < 2)
		{
			return;
		}


	}
#pragma endregion
};
