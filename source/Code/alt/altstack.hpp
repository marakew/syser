
#ifndef _ALT_STACK_HPP_
#define _ALT_STACK_HPP_

#include "../define.h"
#include "altqueue.hpp"

template<typename T, int N>
struct TStack : public TQueue<T,N>
{
public:
	//TStack(const TStack&) {}
	TStack() {}
	~TStack() {}

	void Push(const T & Value); //???
	bool Pop(T & Value); //???
	T & operator[](int n)
	{
		int POS = this->Count-1-n;
		return TQueue<T,N>::operator[](POS);
	}
};

#endif