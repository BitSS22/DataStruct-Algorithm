#pragma once

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

	size_t GetDigit(size_t _Value) noexcept
	{
		assert(_Value != 0);

		size_t Count = 0;

		size_t v = _Value;
		while (v > 0)
		{
			v /= 10;
			++Count;
		}

		return Count;
	}

	int GetDigit(int _Value) noexcept
	{
		assert(_Value != 0);

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
