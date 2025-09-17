#pragma once

//CRTP. 자주 사용할 인터페이스에서 vptr로 인한 오버헤드가 꼴받는다.
template <typename Type>
class Graph
{
	// 필요한 함수의 내용은 그래프 구현에 따라 니들이 만들어라.
public:
	float GetLength() const
	{
		return static_cast<const Type&>(*this).GetLength();
	}

};

// 분리해서 생각해보자ㅣ.
// 그래프 자체는 로직에 필요한 부분을 제공할 수 있는 메서드만 정의한\[다.

// 그외 필ㅇ쇼한 데이터는 각자 정의한다.

// 그렇다면 이; 그래프에서 필요한 데이터는 어떻게 가져오ㅠㄹ 것인ㄱ사?
// CRTP? 상속 클래스? tempalte requires?

//그럼 필요한 데이터 자료에서 graph로서 작동할수 이;ㅆ는 함수 메서드만 정의하고,
// 제공하야 하는 데이터는 데이터측에서 함수를 작성해 호환되게 해야 하는가?
// 그렇다면 Graph class는 interface로서 작성하는 것이 바람직 할 것인가?