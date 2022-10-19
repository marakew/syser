
#ifndef _ALT_ARRAY_HPP_
#define _ALT_ARRAY_HPP_

#include "../define.h"

template<class T, int N>
struct TArray
{
	int Count;
	int Size;
	T *m_ValueAry;
public:
	//TArray(const TArray &) {}
	//TArray(T *, int) {}
	TArray()
		: Count(0)
		, Size(0)
		, m_ValueAry(nullptr)
	{
		Resize(N);
	}

	~TArray()
	{
		SafeDeleteAry(m_ValueAry);
		Size = 0;
		Count = 0;
	}

	//void CopyConstruct(const TArray &) { }

	void Clear()
	{
		int OldSize = Size;
		SafeDeleteAry(m_ValueAry);
		Size = 0;
		Count = 0;
		Resize(OldSize);
	}

	bool Resize(int NewSize)
	{
	        if (NewSize != Size)
		{
			SafeDeleteAry(m_ValueAry);
			Size = NewSize;
			Count = 0;
			m_ValueAry = new T[NewSize];
		}
		return true;
	}

	bool Append(const T & Value)
	{
		if (Count >= Size)
			return false;
		m_ValueAry[Count] = Value;
		++Count;
		return true;
	}

	bool Remove()
	{
		if (Count <= 0)
			return false;
		--Count;
		return true;
	}

	T & operator[](int n)
	{
		int POS = (Size-Count+n)%Size;
		return m_ValueAry[POS];
	}

	//void Attach(T *, int) {}
	//void Detach() {}

	int Find(const T & Value) const
	{
		for (int n = 0; n < Count; ++n)
		{
			if (m_ValueAry[n] == Value)
				return n;
		}
		return 0;
	}
};

#endif