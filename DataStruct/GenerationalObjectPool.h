#pragma once

#include <memory>
#include <cassert>
#include <cstring>
#include <new>
#include <utility>

// 구조 정리를 좀 하고 시작하자
//
// data structure concept
// create(arg..) - 객체 생성, 생성자 예외에 대한 강 보장
// destroy(handle) - handle에 맞는 객체 삭제
// get(handle) - handle valid 일 시 T* return.
// clear() - 살아 있는 객체 전부 삭제
// 
// 요구사항
// handle은 객체 포인터를 직접 들지 않음.
// handle은 data structure
// 
// create는 빈 slot(size < capacity)이 존재하면 재사용.
// T constructor가 exception throw시 exception 강 보장.
// 
// destroy는 handle이 유효시 객체 파괴 후 return true
// 유효하지 않다면 return false
// 
// get
// 유효 핸들이면 T*, else return nullptr
// 
// 
// 
// 
// 기본적인 구조는 vector와 유사.
// 따라서 capacity와 size, raw pointer 구조로 가되,
// raw pointer에 대해 추가적인 데이터가 필요.
// 
// 구조가 vector와 linked list를 합친 느낌이 나는데
// 데이터의 저장은 vector처럼 array 형태로 저장 하고,
// linked list의 노드 형식을 더해 구조를 잡으면 되지 않을까?
// 동작 자체는 linked list 처럼 작동하며, 새로운 node가 필요하게 될 경우 기존에 할당해 둔 slot에서 사용한다.
//
// 그럼 pool은 제일 처음과 마지막 노드의 index를 알고 있으면 되겠고
// pool은 맨 처음 생성한 T의 index를 가지고 있는다 -> 활성화 된 객체를 순회하는데 유리하다. capacity가 아닌 size로 순회 가능.
// 마지막 생성한 T의 index를 가진다 -> 생성에 유리하다. O(1)로 위치를 찾는다.
// 이 때 pool이 마지막으로 생성한 객체에 볼일이 존재하긴 할까?
// 있다. 중간 node가 삭제 되었을 때, 마지막 생성한 slot이 가진 next value를 삭제된 객체의 index로 교체해야 한다.
// 또한 pool이 들고 있던 다음 index도 삭제된 객체의 index로 교체해야 한다.
// 그렇다면 삭제한 노드의 next index 값은 현재 pool이 들고 있던 next index로 변경하고,
// pool과 마지막 slot이 들고 있던 next index 값은 삭제한 노드로 변경한다.
// 이렇게 하면 배열 내에서 모두 순회하는 노드 구조가 성립할 것이다.
// 이 때, 각 slot의 next_index init value는 (current index + 1) % capacity 가 될 것이다.
// 
// 자신의 size와 capacity를 기록하고
// 각 node들은 다음에 찾아갈 index에 대해 기록하고 있으면 되는가?
// 그렇게 구현 했을 때 node가 다음 찾아갈 node에 대해 이미 존재하는 곳을 가리키고 있을 가능성은?
// 그리고 그렇다면 각 slot이 다음 slot에 대한 index를 어떤 값을 가지고 있어야 하는가?

// slot에서 가져야 할 데이터는 뭐가 있을까
// 일단 raw storage는 무조건 필요하다.
// 또, 구조상 next index또한 무조건 필요하다.
// 그렇다면 generation과 isAlive, 두가지에 대해 고민해 볼 필요가 있다.
// 객체가 유효한지 알기 위해 어떤 변수 값이 필요하지만, 어디까지 필요하냐의 문제.
// 객체의 life time 관리에 isAlive를 사용하고,
// 외부 handle에서 slot에 접근 할 때 generation을 통해 유효하지 않은 handle 여부를 검출한다.
// handle의 generation은 handle 생성시 slot의 generation 값을 가져오면 된다.
// 그렇다면 어떤 경우에 slot이 가진 generation 값이 증가 함이 옳은가?
// 즉, 이미 발급한 handle이 유효하지 않아지는 경우는 어떤 경우인가?
// -> 객체가 삭제 되었을 때. 즉, 같은 slot에 존재하는 객체가 현재 객체 이전에 살았던 입주자인가?
// slot에 객체를 삭제 할 때, slot의 generation을 증가시킨다.

// 만약 배열형에서 기존 자료에 대한 주소값을 보전하는 전략을 취하겠다면.
// 기존에 생성된 자료에 대해 메모리 반납을 하지 않는 구조가 되어야 할 것.
// 그렇다면 template에서 인자로 각 구역의 객체 갯수를 받으면 되지 않을까?

//using T = int;
//constexpr size_t chunk_in_slot_count = 32;
template <typename T, size_t chunk_in_slot_count = 32>
class GenerationalObjectPool
{
	static_assert(chunk_in_slot_count > 0);
public:
	struct Handle
	{
		size_t index = static_cast<size_t>(-1);
		size_t generation = static_cast<size_t>(-1);
	};

private:
	struct Slot
	{
		alignas(T) unsigned char storage[sizeof(T)] = {};
		size_t generation = 0;
		size_t next_index = static_cast<size_t>(-1);
		bool isAlive = false;
	};

public:
	GenerationalObjectPool() {}
	~GenerationalObjectPool()
	{
		Release();
	}

	GenerationalObjectPool(const GenerationalObjectPool& other) = delete;
	GenerationalObjectPool(GenerationalObjectPool&& other) noexcept
	{
		if (this == &other)
		{
			return;
		}

		slots = other.slots;
		next_index = other.next_index;
		chunk_used = other.chunk_used;
		chunk_capacity = other.chunk_capacity;
		total_size = other.total_size;
		total_capacity = other.total_capacity;

		other.slots = nullptr;
		other.next_index = 0;
		other.chunk_used = 0;
		other.chunk_capacity = 0;
		other.total_size = 0;
		other.total_capacity = 0;
	}

public:
	GenerationalObjectPool& operator=(const GenerationalObjectPool& other) = delete;
	GenerationalObjectPool& operator=(GenerationalObjectPool&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		if (slots != nullptr)
		{
			Release();
		}

		slots = other.slots;
		next_index = other.next_index;
		chunk_used = other.chunk_used;
		chunk_capacity = other.chunk_capacity;
		total_size = other.total_size;
		total_capacity = other.total_capacity;

		other.slots = nullptr;
		other.next_index = 0;
		other.chunk_used = 0;
		other.chunk_capacity = 0;
		other.total_size = 0;
		other.total_capacity = 0;

		return *this;
	}

private:
	Slot** slots = nullptr;
	size_t next_index = 0;
	size_t chunk_used = 0;
	size_t chunk_capacity = 0;
	size_t total_size = 0;
	size_t total_capacity = 0;

public:
	template<typename... args>
	Handle Create(args&&... arg)
	{
		if (total_size >= total_capacity)
		{
			AddChunk();
		}

		Slot* slot = GetSlot(next_index);
		assert(slot != nullptr);

		Handle handle = {};
		handle.index = next_index;
		handle.generation = slot->generation;

		std::construct_at(reinterpret_cast<T*>(slot->storage), std::forward<args>(arg)...);

		next_index = slot->next_index;
		slot->isAlive = true;
		++total_size;

		return handle;
	}

	bool Destroy(Handle handle) noexcept
	{
		if (!IsValidHandle(handle))
		{
			return false;
		}

		Slot* slot = GetSlot(handle.index);
		slot->isAlive = false;
		slot->generation += 1;
		std::destroy_at<T>(reinterpret_cast<T*>(slot->storage));

		slot->next_index = next_index;
		next_index = handle.index;

		--total_size;
		return true;
	}

	T* Get(Handle handle) noexcept
	{
		const GenerationalObjectPool& c_this = *this;
		return const_cast<T*>(c_this.Get(handle));
	}

	const T* Get(Handle handle) const noexcept
	{
		if (!IsValidHandle(handle))
		{
			return nullptr;
		}

		Slot* slot = GetSlot(handle.index);
		return reinterpret_cast<T*>(slot->storage);
	}

	bool IsAlive(Handle handle) const noexcept
	{
		return IsValidHandle(handle);
	}

	void Clear() noexcept
	{
		if (total_size <= 0)
		{
			return;
		}

		for (size_t i = 0; i < chunk_used; ++i)
		{
			for (size_t j = 0; j < chunk_in_slot_count; ++j)
			{
				Slot& slot = slots[i][j];
				slot.next_index = i * chunk_in_slot_count + j + 1;

				if (slot.isAlive)
				{
					slot.generation += 1;
					slot.isAlive = false;
					std::destroy_at<T>(reinterpret_cast<T*>(slot.storage));
				}
			}
		}

		next_index = 0;
		total_size = 0;
	}

	size_t Size() const noexcept
	{
		return total_size;
	}

	size_t Capacity() const noexcept
	{
		return total_capacity;
	}

private:
	Slot* GetSlot(size_t index) const
	{
		size_t chunk_index = index / chunk_in_slot_count;

		if (chunk_index >= chunk_used)
		{
			return nullptr;
		}
		auto& a = slots[chunk_index][index % chunk_in_slot_count];
		return &a;
	}

	void ResizeChunkCapacity(size_t capacity)
	{
		if (capacity <= chunk_capacity)
		{
			return;
		}

		Slot** new_ptr = static_cast<Slot**>(::operator new(sizeof(Slot*) * capacity));
		if (slots != nullptr)
		{
			memcpy(new_ptr, slots, sizeof(Slot*) * chunk_used);
			::operator delete(slots);
		}

		slots = new_ptr;
		chunk_capacity = capacity;
	}

	void AddChunk()
	{
		if (chunk_used >= chunk_capacity)
		{
			size_t capacity = chunk_capacity == 0 ? 1 : chunk_capacity * 2;
			ResizeChunkCapacity(capacity);
		}

		slots[chunk_used] = static_cast<Slot*>(::operator new(sizeof(Slot) * chunk_in_slot_count, std::align_val_t(alignof(Slot))));

		for (size_t i = 0; i < chunk_in_slot_count; ++i)
		{
			std::construct_at(&slots[chunk_used][i]);

			slots[chunk_used][i].generation = 0;
			slots[chunk_used][i].next_index = chunk_used * chunk_in_slot_count + i + 1;
			slots[chunk_used][i].isAlive = false;
		}

		total_capacity += chunk_in_slot_count;
		chunk_used += 1;
	}

	bool IsValidHandle(Handle handle) const noexcept
	{
		Slot* slot = GetSlot(handle.index);
		if (slot == nullptr)
		{
			return false;
		}
		if (!slot->isAlive)
		{
			return false;
		}
		if (handle.generation != slot->generation)
		{
			return false;
		}

		return true;
	}

	void Release()
	{
		Clear();

		for (size_t i = 0; i < chunk_used; ++i)
		{
			for (size_t j = 0; j < chunk_in_slot_count; ++j)
			{
				std::destroy_at(&slots[i][j]);
			}

			::operator delete(slots[i], std::align_val_t(alignof(Slot)));
			slots[i] = nullptr;
		}

		if (slots != nullptr)
		{
			::operator delete(slots);
			slots = nullptr;
		}

		next_index = 0;
		chunk_used = 0;
		chunk_capacity = 0;
		total_size = 0;
		total_capacity = 0;
	}
};
