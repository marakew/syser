
#ifndef _ALT_QUEUE_HPP_
#define _ALT_QUEUE_HPP_

#include "../define.h"
#include "altarray.hpp"

template<class T, int N>
struct TQueue : public TArray<T,N>
{
	int m_HeadIndex;
public:
	TQueue()
	{
		m_HeadIndex = -1;
	}
	~TQueue()
	{
	}

	void Clear()
	{
		TArray<T,N>::Clear();
		m_HeadIndex = -1;
	}

	bool Append(const T & Value)
	{
		if (this->Size == 0) return false;
		if (this->Count < this->Size) this->Count++;
		m_HeadIndex = (m_HeadIndex+1)%this->Size;
		this->m_ValueAry[m_HeadIndex] = Value;
		return true;
	}

	T * Append(const T *Value)
	{
		if (this->Size == 0) return 0;
		if (this->Count < this->Size) this->Count++;
		m_HeadIndex = (m_HeadIndex+1)%this->Size;
		if (Value)
			this->m_ValueAry[m_HeadIndex] = *Value;
		return &this->m_ValueAry[m_HeadIndex];
	}

	T & operator[](int n)
	{
		int POS = m_HeadIndex+1+n;
		return TArray<T,N>::operator[](POS);
	}
};

#endif