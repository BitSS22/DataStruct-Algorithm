#pragma once

// Find ArrSize Function
template<typename Type, size_t i>
constexpr size_t ArrSize(Type(&)[i]) noexcept
{
	return i;
}

template<typename Type>
inline void Swap(Type& _Left, Type& _Right)
{
	Type Temp = std::move(_Left);
	_Left = std::move(_Right);
	_Right = std::move(Temp);
}

template<int _Value>
constexpr int GetDigit() noexcept
{
	static_assert(_Value != 0, "GetDigit Agument is Zero.");

	int Count = 0;

	int v = _Value;
	while (v > 0)
	{
		v /= 10;
		++Count;
	}

	return Count;
}