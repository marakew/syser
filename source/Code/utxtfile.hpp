
#ifndef _UTXTFILE_HPP_
#define _UTXTFILE_HPP_

#include "define.h"
#include "imagefile.hpp"
#include "alt/altlist.hpp"

struct UCTXTFile : public CImageFile
{
	UCTXTFile();
	~UCTXTFile();

	virtual bool Open(const char *FileName, HANDLE OpenHandle) override;
	bool Save(const char *FileName);
	virtual void Close() override;
	int SetLineEndIdentity(const WCHAR *Buffer, unsigned long long Length);
	bool Insert(unsigned long Pos, unsigned long Length, const WCHAR *String);
	bool Replace(unsigned long Pos, unsigned long Length, const WCHAR *String, unsigned long NewPos);

	TList<WCHAR *> m_StrList;
};

#endif
