#pragma once
#include <assert.h>

template<typename DataType>
class UVector
{
public:
	~UVector()
	{
		if (nullptr != Datas)
		{
			delete[] Datas;
			Datas = nullptr;
		}
	}

	void reserve(size_t _capacity)
	{
		printf_s("데이터 재 할당 시작\n");

		if (CapacityValue > _capacity)
		{
			return;
		}

		DataType* PrevData = Datas;

		Datas = new DataType[_capacity];

		if (nullptr != PrevData)
		{
			// 기존 데이터를 복사해줘야 한다.
			for (size_t i = 0; i < SizeValue; i++)
			{
				Datas[i] = PrevData[i];
			}

			delete[] PrevData;
			PrevData = nullptr;
		}

		CapacityValue = _capacity;

		printf_s("데이터 재 할당 종료\n");
	}

	size_t capacity() const
	{
		return CapacityValue;
	}

	size_t size() const
	{
		return SizeValue;
	}

	void claer()
	{
		SizeValue = 0;
	}

	void push_back(DataType _Data)
	{
		//if (sizevalue + 1 > capacityvalue)
		//{
		//	// 정확한지는 모르겠습니다.
		//	reserve(capacityvalue * 1.5);
		//}

		if (SizeValue >= CapacityValue)
		{
			size_t value = CapacityValue * 2 <= 0 ? 1 : CapacityValue * 2;
			reserve(value);
		}

		// 내부 에러가 안납니다.
		// 그래서 만들때는 Typedef DataType으로 만들는걸선호합니다.
		// _Data->B;

		Datas[SizeValue] = _Data;

		++SizeValue;
	}

	DataType& operator[](size_t _index)
	{
		if (SizeValue <= _index)
		{
			assert(false);
		}

		return Datas[_index];
	}


private:
	DataType* Datas = nullptr;
	size_t CapacityValue = 0;
	size_t SizeValue = 0;
};

