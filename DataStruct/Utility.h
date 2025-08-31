#pragma once

namespace Utility
{
	template<typename Type>
	inline void Swap(Type& _Left, Type& _Right)
	{
		Type Temp = std::move(_Left);
		_Left = std::move(_Right);
		_Right = std::move(Temp);
	}

	template<typename Type>
	inline bool DefaultCompare(const Type& _Left, const Type& _Right) noexcept
	{
		return _Left < _Right;
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
}
