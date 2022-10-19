
#ifndef _ALT_FILE_HPP_
#define _ALT_FILE_HPP_

#include "../define.h"

namespace ALT
{
	struct ALTFileStream
	{
		virtual unsigned long Write(const void *Buffer, unsigned long Size) { return 0; }
		virtual unsigned long Read(void *Buffer, unsigned long Size) { return 0; }
	};
}

#endif