
#ifndef _SRCTXTFILE_HPP_
#define _SRCTXTFILE_HPP_

#include "txtfile.hpp"

struct CSrcTXTFile : public CTXTFile
{
	TList<unsigned long> m_Lines;
	unsigned long m_Mode;

	bool OpenEx(const char *FileName, HANDLE OpenHandle, unsigned long Mode);
};

#endif