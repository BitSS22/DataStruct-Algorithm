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
	template<std::integral Type>
	constexpr size_t GetPrintDigit(Type _Value) noexcept
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

	// ��ƿ��Ƽ Ŭ���� Heap�� ����� ����.
	// �迭�� Heapó�� �� �� �ְ� ����� �ִ� ��� ����.
	class Heap
	{
		// �� Ŭ������ ����� Ŭ������ �ƴϾ�.
		Heap() = delete;
	public:
		// ����Ʈ�� (�θ�, ����, ������)�� ��ȭ. �ڽ��� ����Ʈ������ ��� Heap ���¿��� ���� �۵��Ѵ�.
		// �κ� Ʈ������ �����ϱ� ���� ����Ʈ���� Index�� �޴´�.
		template <typename Type, typename Compare = bool(*)(const Type&, const Type&)>
		static void HeapifyDown(Type* _Arr, size_t _Size, size_t _Index = 0, Compare _Comp = Utility::DefaultCompare)
		{
			// �̷� �� �� ���� ��.
			assert(_Arr);
			// �翬�� Size�� �Ѵ� Index�� ������ �ȵȴ�.
			assert(_Index < _Size);
			// �� �Լ� �� ����.
			assert(_Comp);

			while (true)
			{
				size_t Best = _Index;
				size_t LeftChild = GetLeftChild(_Index);
				size_t RightChild = GetRightChild(_Index);

				// Size�� ���� �ʰ� _Comp�� �����ϴ� �ڽ��� �ִ°�?
				if (LeftChild < _Size && _Comp(_Arr[LeftChild], _Arr[Best]))
				{
					Best = LeftChild;
				}
				if (RightChild < _Size && _Comp(_Arr[RightChild], _Arr[Best]))
				{
					Best = RightChild;
				}

				// ���ٸ� ��.
				if (Best == _Index)
				{
					break;
				}

				// �ִٸ� ���� ���� �ٲ۴�.
				Utility::Swap(_Arr[_Index], _Arr[Best]);
				// �ٲ� �ڽ����� �ٽ� Heapify.
				_Index = Best;
			}
		}

		// �迭�� ������ Heap���� ����� �ش�.
		template <typename Type, typename Compare = bool(*)(const Type&, const Type&)>
		static void BuildHeap(Type* _Arr, size_t _Size, Compare _Comp = Utility::DefaultCompare)
		{
			// �迭 ������ 1�� ������ �迭�� �̹� ���� �Ǿ��ٰ� �� �� �ִ�.
			if (_Size < 2)
			{
				return;
			}

			// _Size / 2 ������ ���ҵ鸸�� �׻� �ڽ��� ������. �ڽ��� ���� ����Ʈ���� �̹� ���ĵ� ����Ʈ��. 
			for (size_t i = _Size / 2 - 1; i >= 0; --i)
			{
				// ���� ������ Heap�� ������Ų��.
				HeapifyDown(_Arr, _Size, i, _Comp);
			}
		}

		// �� ������ ������ �ʰ� �����Ѵ�.
		// ������ �迭�� �̹� �� ������� �����Ѵ�.
		// _Arr[_CurrentSize] �� �ε��� �˻�� ���� ����.
		// �Լ� ���� -> ������ ���� �ܿ� ��� ���� �����Ѵ�.
		// ������ ���Ҵ� ���ĵ��� �ʰ� ���� �� ä�� �޾Ҵ�. -> ��, ������ ���Ҹ� �������ִ� �Լ�.
		// �翬�� _Comp�� ��ȭ �Ҷ��� ���� �Լ� ������� ����ڰ� �־�� �Ѵ�.
		template <typename Type, typename Compare = bool(*)(const Type&, const Type&)>
		static void HeapifyUp(Type* _Arr, size_t _Size,  Compare _Comp = Utility::DefaultCompare)
		{
			// �̷� �� �� ���� ��.
			assert(_Arr);
			// �� �Լ� �� ����.
			assert(_Comp);

			if (_Size < 2)
			{
				return;
			}

			size_t Best = _Size - 1;

		}

		static size_t GetParent(size_t _Index) noexcept
		{
			// _Index�� 0�� �ƴϾ�� �Ѵ�. ������ ���̰ڴ�.
			assert(_Index);
			return (_Index - 1) / 2;
		}

		static size_t GetLeftChild(size_t _Index) noexcept
		{
			return _Index * 2 + 1;
		}

		static size_t GetRightChild(size_t _Index) noexcept
		{
			return _Index * 2 + 2;
		}
	};
}
