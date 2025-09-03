#pragma once
#include <cassert>

namespace Utility
{
	template<typename Type>
	inline void Swap(Type& _Left, Type& _Right)
	{
		Type Temp = std::move_if_noexcept(_Left);
		_Left = std::move_if_noexcept(_Right);
		_Right = std::move_if_noexcept(Temp);
	}

	template<typename Type>
	inline bool DefaultCompare(const Type& _Left, const Type& _Right) noexcept
	{
		return _Left < _Right;
	}

	constexpr size_t GetDigit(size_t _Value) noexcept
	{
		if (!_Value)
			return 0;

		size_t Count = 0;

		size_t v = _Value;
		while (v > 0)
		{
			v /= 10;
			++Count;
		}

		return Count;
	}

	constexpr int GetDigit(int _Value) noexcept
	{
		if (!_Value)
			return 0;

		int Count = 0;

		int v = _Value;
		while (v > 0)
		{
			v /= 10;
			++Count;
		}

		return Count;
	}
}
