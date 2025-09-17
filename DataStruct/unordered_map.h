#pragma once
#include "Utility.h"
#include "vector.h"

// �ʿ��Ѱ� ����
// raw �޸� Ǯ.
// Size.
// Capacity
// hash collision ó�� (ü�̴�)
// equal ��� ��
// Hash �Լ�.

using Type1 = int;
using Type2 = int;
using HashFunc = std::hash<Type1>;
using Equal = std::equal_to<void>;

// �ؽ��� ������ �����ϱ� ���� unordered_set.
//template <typename Type1, typename Type2, typename HashFunc = std::hash<Key>, typename Equal = std::equal_to<void>>
class unordered_map
{
public:
	struct Pair
	{
	public:
		// �⺻ ������, �Ҹ��ڴ� ��������� default ����.
		Pair() = default;
		Pair(const Pair&) = default;
		Pair(Pair&&) noexcept = default;
		~Pair() = default;

	public:
		Pair& operator= (const Pair& _Other) = default;
		Pair& operator= (Pair&& _Other) = default;

		template <typename U1, typename U2>
			requires std::constructible_from<Type1, U1&&>&& std::constructible_from<Type1, U2&&>
		explicit(!std::convertible_to<U1&&, Type1> || !std::convertible_to<U2&&, Type2>)
			Pair(U1&& _Key, U2&& _Value)
			noexcept(noexcept(Type1(Utility::Forward<U1>(_Key))) && noexcept(Type2(Utility::Forward<U2>(_Value))))
			: Key(Utility::Forward<U1>(_Key))
			, Value(Utility::Forward<U2>(_Value)) {
		}
	public:
		Type1 Key = {};
		Type2 Value = {};

	};

	enum class State : __int8
	{
		Empty, Occupied, Deleted
	};

public:
	unordered_map() = default;
	~unordered_map()
	{
		for (size_t i = 0; i < Capacity; ++i)
		{
			if (States[i] == State::Occupied)
			{
				std::destroy_at(Bucket + i);
			}
		}

		if constexpr (alignof(Pair) > alignof(std::max_align_t))
		{
			::operator delete(Bucket, std::align_val_t(alignof(Pair)));
		}
		else
		{
			::operator delete(Bucket);
		}
		
		delete[] States;
		Size = 0;
		Capacity = 0;
	};

	unordered_map(const unordered_map& _Other) = delete;
	unordered_map(unordered_map&& _Other) = delete;

public:
	unordered_map& operator= (const unordered_map& _Other) = delete;
	unordered_map& operator= (unordered_map&& _Other) = delete;

private:
	// �⺻������ ������ ���� ������ ��Ŷ(�迭)�� �ʿ��� ��.
	// �ؽ� �浹�� ����Ž��� �ذ��� ����.
	// ���Ұ� ���� �ϴ��� �����ϱ� �� Ȯ���ؾ� �Ѵ�. ������ �����ͷ� ���� �ؾ� �� �� ����. => ��Ʈ ����ŷ���� 64������ üũ �غ���.

	Pair* Bucket = nullptr;
	State* States = nullptr;

	size_t Size = 0;
	size_t Capacity = 0;

	HashFunc Hash = std::hash<Type1>{};
	Equal Eq = std::equal_to<void>{};

	float LoadFactor = 0.75f;

public:
	void ReAllocate(size_t _NewCapacity)
	{
		if (_NewCapacity < Capacity)
			return;

		Pair* OldBucket = Bucket;
		State* OldStates = States;

		if constexpr (alignof(Pair) > alignof(std::max_align_t))
		{
			Bucket = static_cast<Pair*>(::operator new(sizeof(Pair) * _NewCapacity, std::align_val_t(alignof(Pair))));
		}
		else
		{
			Bucket = static_cast<Pair*>(::operator new(sizeof(Pair) * _NewCapacity));
		}

		States = new State[_NewCapacity](State::Empty);
		Size = 0;
		Capacity = _NewCapacity;
		
		if (!OldBucket || !OldStates)
			return;

		for (size_t i = 0; i < Capacity; ++i)
		{
			if (OldStates[i] == State::Occupied)
			{
				Insert(Utility::Move(OldBucket[i]));

				std::destroy_at(OldBucket + i);
			}
		}

		if constexpr (alignof(Pair) < alignof(std::max_align_t))
		{
			::operator delete(OldBucket, std::align_val_t(alignof(Pair)));
		}
		else
		{
			::operator delete(OldBucket);
		}

		::operator delete(OldStates);
	}

	void Insert(const Pair& _Item)
	{
		assert(Capacity);
		assert(Bucket);
		assert(States);

		if (IsThreshold())
		{
			ReAllocate(Capacity * 2);
		}

		size_t HashIndex = Hash(_Item.Key) % Capacity;
		size_t i = HashIndex;

		do
		{
			if (States[HashIndex] == State::Occupied)
			{
				(++i) % Capacity;
				continue;
			}

			new(Bucket + i) Pair(_Item);
			States[i] = State::Occupied;
			++Size;
			return;
		}
		while (i != HashIndex);
	}

	void Insert(Pair&& _Item)
	{

	}

	bool Contains(const Type1& _Key) const
	{
		size_t HashIndex = Hash(_Key) % Capacity;
		size_t i = HashIndex;

		do
		{
			switch (States[i])
			{
			case State::Occupied:
				if (Eq(_Key, Bucket[i].Key))
				{
					return true;
				}
				[[fallthrough]];
			case State::Deleted:
				(++i) % Capacity;
				continue;
			case State::Empty:
				return false;
			default:
				assert(!"Not Found state. Critical Error.");
				break;
			}

		} while (i != HashIndex);

		return false;
	}

	Pair* Find(const Type1& _Key)
	{
		size_t HashIndex = Hash(_Key) % Capacity;
		size_t i = HashIndex;

		do
		{
			switch (States[i])
			{
			case State::Occupied:
				if (Eq(_Key, Bucket[i].Key))
				{
					return Bucket + i;
				}
				[[fallthrough]];
			case State::Deleted:
				(++i) % Capacity;
				continue;
			case State::Empty:
				return nullptr;
			default:
				assert(!"Not Found state. Critical Error.");
				break;
			}

		} while (i != HashIndex);

		return nullptr;
	}

private:
	void ReHash()
	{
		ReAllocate(Capacity);
	}

public:
	bool IsThreshold() const noexcept
	{
		return LoadFactor * Capacity < Size;
	}
	bool SetLoadFactor(float _NewLoadFactor)
	{
		if (_NewLoadFactor < 0.5f || _NewLoadFactor > 1.0f)
			return false;

		LoadFactor = _NewLoadFactor;
		return true;
	}
	float GetLoadFactor() const noexcept
	{
		return LoadFactor;
	}
	
};
