
#ifndef _TXTFILE_HPP_
#define _TXTFILE_HPP_

#include "define.h"
#include "imagefile.hpp"
#include "alt/altlist.hpp"

struct CTXTFile : public CImageFile
{
	CTXTFile();
	~CTXTFile();

	virtual bool Open(const char *FileName, HANDLE OpenHandle) override;
	bool Save(const char *FileName);
	virtual void Close() override;
	int SetLineEndIdentity(const char *Buffer, unsigned long long Length);
	bool Insert(unsigned long Pos, unsigned long Length, const char *String);
	bool Replace(unsigned long Pos, unsigned long Length, const char *String, unsigned long NewPos);

	TList<char *> m_StrList;
};

#endif
