#pragma once
#include <cassert>
#include <concepts>

namespace Utility
{
	// std::remove_reference.
	// �̰� �׳� ���ø��̴�. ���� Ÿ�Կ� �̸� T�� �ٿ��ִ°� ��.
	// struct/class�� ����� Type�� ���� �б� ó����, &, && �� Ÿ�Կ� ���� T ������� ���� ��Ų��.
	template<typename Type>
	struct remove_reference
	{
		using T = Type;
	};
	// �Ʒ� �ΰ��� ���ø� Ư��ȭ. Type&, Type&&�� ���ø� ���ڷ� ������ ���õȴ�.
	// �ȿ��� �ϴ� ���� �Ȱ���.
	template<typename Type>
	struct remove_reference<Type&>
	{
		using T = Type;
	};
	template<typename Type>
	struct remove_reference<Type&&>
	{
		using T = Type;
	};
	// ��, remove_ref<Type>::T�� �������� ������ Ÿ���� �ȴ�.

	// �׷��� ��, remove_ref<Type>::T�� ���� ������. �׷��� template alias�� �̸��� �����ְڴ�.
	template<typename Type>
	using remove_ref = typename remove_reference<Type>::T;
	// remove_reference<Type>::T �� ������ Ÿ������ �� ������. �׷��� �տ� typename�� �ٿ���� �Ѵ�.
	// ���� remove_ref<Type&&> Value; ó�� �����ϰ� ���ܸ��� �� �ִ�!
	// �׷��� �̰ɷ� �� �ϳ�? move�� forward ����Ŵ�.

	// std::move.
	// Type�� �޾� �������� ���� Type�� ���, �װ� �ٽ� &&�� ĳ������ ��ȯ�Ѵ�. ��, ���� R-Value�� ��������.
	// ������ ������ Type& -> return Type&;
	// forward reference = template���� Ÿ�� �߷�. Type&&���� L-Value, R-Value �� �޴´�.
	template <typename Type>
	constexpr remove_ref<Type>&& Move(Type&& _Value) noexcept
	{
		return static_cast<remove_ref<Type>&&>(_Value);
	}

	// L-Value Overload.
	template <typename Type>
	constexpr Type&& Forward(remove_ref<Type>& _Value) noexcept
	{
		// &&�� return �ϸ� Type& &&�� ���� �ر��� �Ͼ�� Type& return.
		return static_cast<Type&&>(_Value);
	}
	// R-Value Overload.
	template <typename Type>
	constexpr Type&& Forward(remove_ref<Type>&& _Value) noexcept
	{
		return static_cast<Type&&>(_Value);
	}
	// Type&& (Type&& _Value) ���� �������δ� �ȵǳ�? => Type�� ���� template ���� �� �߷� ����.
	// remove_ref<Type>�� Type�� ���� �Ǿ����� template�� �߷����� ����. -> �Ͻ��� �߷�, ������� ���� ���� ����.
	// ��, Forward<Type>(_Value)�� ���� ��Ȯ�� ȣ���ؾ� �Ѵ�.

	// ������ Move�� ����� swap �Լ� ����.
	// ���簡 �ƴ� �̵����� �ΰ� �ű��.
	template<typename Type>
	inline void Swap(Type& _Left, Type& _Right) noexcept
	{
		Type Temp = Move(_Left);
		_Left = Move(_Right);
		_Right = Move(Temp);
	}

	// Sort���� �� �⺻ �� �Լ�. �翬�� Type�� operator< �� overload �� �־�� �Ѵ�.
	template<typename Type>
	inline bool DefaultCompare(const Type& _Left, const Type& _Right) noexcept
	{
		return _Left < _Right;
	}

	// ���� Ÿ�Ը� �����ž�.
	template<std::integral Integer>
	constexpr size_t PrintDigitCount(Integer _Value) noexcept
	{
		// 0�� ���� 0�̴� 1�ڸ��� �ƴұ�?
		if (0 == _Value)
			return 1;

		size_t Count = 0;
		// ������ -��ȣ�� �ڸ����� ���ڴ�.
		if (0 > _Value)
			++Count;

		while (_Value != 0)
		{
			_Value /= 10;
			++Count;
		}

		return Count;
	}

	// RadixSort���� �� �Լ��� ���� ����ڴ�.
	template<std::integral Integer>
	constexpr size_t DigitNumber(Integer _Value, size_t _DecimalDigit_Zerobase) noexcept
	{
		// CutValue�� �̾� �� �ڸ����� ��´�.
		size_t CutValue = 1;
		for (size_t i = 0; i < _DecimalDigit_Zerobase; ++i)
		{
			CutValue *= 10;
		}

		return _Value / CutValue % 10;
	}

	// ����� �ƴ� �����ϰ� �ڸ����� ã�´� ����� �ްڴ�. 0�� 0 return.
	template<std::integral Integer>
	constexpr size_t UnsignedDigitCount(Integer _Value) noexcept
	{
		size_t Count = 0;

		while (_Value != 0)
		{
			_Value /= 10;
			++Count;
		}

		return Count;
	}
}
