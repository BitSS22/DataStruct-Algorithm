#pragma once

#include <random>
#include "Utility.h"

std::random_device rd;
std::mt19937 gen(rd());

// Knuth Shuffle
template<typename Type, size_t _Size>
void ArrShuffle(Type (&_Arr)[_Size])
{
	if (_Size < 2)
		return;

	for (size_t i = _Size - 1; i > 0; --i)
		Utility::Swap(_Arr[i], _Arr[gen() % (i + 1)]);
}

