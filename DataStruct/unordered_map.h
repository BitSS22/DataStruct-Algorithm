#pragma once
#include <memory>

#include "Utility.h"

// 필요한거 정리
// raw 메모리 풀.
// Size.
// Capacity
// hash collision 처리 (체이닝)
// equal 동등성 비교
// Hash 함수.

// using Type1 = int;
// using Type2 = int;
// using HashFunc = std::hash<Type1>;
// using Equal = std::equal_to<void>;

// 해쉬의 개념을 구현하기 위한 unordered_set.
template <typename Type1, typename Type2, typename HashFunc = std::hash<Type1>, typename Equal = std::equal_to<void>>
class unordered_map
{
public:
	struct Pair
	{
	public:
		// 기본 생성자, 소멸자는 명시적으로 default 선언.
		Pair() = default;
		Pair(const Pair&) = default;
		Pair(Pair&&) noexcept = default;
		~Pair() = default;

	public:
		Pair& operator= (const Pair& _Other) = default;
		Pair& operator= (Pair&& _Other) = default;

		template <typename U1, typename U2>
			requires std::constructible_from<Type1, U1&&>&& std::constructible_from<Type2, U2&&>
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
		Empty = 0, Occupied, Deleted
	};

public:
	unordered_map(size_t _Capacity = 16)
	{
		ReAllocate(_Capacity);
	}
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
	// 기본적으로 들어오는 값을 저장할 버킷(배열)이 필요할 것.
	// 해시 충돌은 선형탐사로 해결해 본다.
	// 원소가 존재 하는지 접근하기 전 확인해야 한다. 별도의 데이터로 관리 해야 할 것 같다. => 비트 마스킹으로 64구간씩 체크 해본다.

	Pair* Bucket = nullptr;
	State* States = nullptr;

	size_t Size = 0;
	size_t Capacity = 0;

	HashFunc Hash = {};
	Equal Eq = {};

	float LoadFactor = 0.75f;

public:
	void ReAllocate(size_t _NewCapacity)
	{
		assert(_NewCapacity);

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

		States = new State[_NewCapacity]{};
		Size = 0;
		size_t OldCapacity = Capacity;
		Capacity = _NewCapacity;
		
		if (!OldBucket || !OldStates)
			return;

		for (size_t i = 0; i < OldCapacity; ++i)
		{
			if (OldStates[i] == State::Occupied)
			{
				Insert(Utility::Move(OldBucket[i]));

				std::destroy_at(OldBucket + i);
			}
		}

		if constexpr (alignof(Pair) > alignof(std::max_align_t))
		{
			::operator delete(OldBucket, std::align_val_t(alignof(Pair)));
		}
		else
		{
			::operator delete(OldBucket);
		}

		delete[] OldStates;
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
		size_t Sentinel = static_cast<size_t>(-1);
		size_t DIndex = Sentinel;

		do
		{
			if (States[i] == State::Occupied)
			{
				if (Eq(Bucket[i].Key, _Item.Key))
				{
					Bucket[i].Value = _Item.Value;
					return;
				}
			}
			else if (States[i] == State::Deleted)
			{
				if (DIndex == Sentinel)
				{
					DIndex = i;
				}
			}
			else
			{
				if (DIndex != Sentinel)
				{
					i = DIndex;
				}
				new(Bucket + i) Pair(_Item);
				States[i] = State::Occupied;
				++Size;
				return;
			}

			i = (i + 1) % Capacity;
		} while (i != HashIndex);
	}

	void Insert(Pair&& _Item)
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
		size_t Sentinel = static_cast<size_t>(-1);
		size_t DIndex = Sentinel;

		do
		{
			if (States[i] == State::Occupied)
			{
				if (Eq(Bucket[i].Key, _Item.Key))
				{
					Bucket[i].Value = Utility::Move(_Item.Value);
					return;
				}
			}
			else if (States[i] == State::Deleted)
			{
				if (DIndex == Sentinel)
				{
					DIndex = i;
				}
			}
			else
			{
				if (DIndex != Sentinel)
				{
					i = DIndex;
				}
				new(Bucket + i) Pair(Utility::Move(_Item));
				States[i] = State::Occupied;
				++Size;
				return;
			}

			i = (i + 1) % Capacity;
		} while (i != HashIndex);
	}

	bool Contains(const Type1& _Key) const
	{
		assert(Capacity);
		assert(Bucket);
		assert(States);

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
				i = (i + 1) % Capacity;
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
		assert(Capacity);
		assert(Bucket);
		assert(States);

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
				i = (i + 1) % Capacity;
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

	bool Erase(const Type1& _Key)
	{
		assert(Capacity);
		assert(Bucket);
		assert(States);

		size_t HashIndex = Hash(_Key) % Capacity;
		size_t i = HashIndex;

		do
		{
			switch (States[i])
			{
			case State::Occupied:
				if (Eq(_Key, Bucket[i].Key))
				{
					std::destroy_at(Bucket + i);
					States[i] = State::Deleted;
					--Size;
					return true;
				}
				[[fallthrough]];
			case State::Deleted:
				i = (i + 1) % Capacity;
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

private:
	void ReHash()
	{
		ReAllocate(Capacity);
	}

public:
	bool IsThreshold() const noexcept
	{
		return LoadFactor * static_cast<float>(Capacity) <= static_cast<float>(Size);
	}
	// Min : 0.3f, Max : 0.99f
	bool SetLoadFactor(float _NewLoadFactor)
	{
		if (_NewLoadFactor < 0.3f || _NewLoadFactor > 0.99f)
			return false;

		LoadFactor = _NewLoadFactor;
		return true;
	}
	float GetLoadFactor() const noexcept
	{
		return LoadFactor;
	}
	
};
