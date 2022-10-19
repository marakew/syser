
#include "Code/define.h"
#include "Code/polyfs.hpp"

int CopyCallback(CFileIO *pSrcIO, CFileIO *pDstIO,
		const char *pSrcPath, const char *pDstPath, unsigned long Length, void *Data)
{
	printf("copy: %s -> %s, %d\n", pSrcPath, pDstPath, Length);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("usage %s: File.dat UnpackDir\n", argv[0]);
		return 1;
	}

	CPFSFileIO PFSFileIO;
	PFSFileIO.ChangeFileIO(gpFileIO);
	
	if (!PFSFileIO.OpenDisk(argv[1], 1))
	{
		PFSFileIO.CloseDisk();
		printf("OpenDisk: error");
		return 1;
	}

	if (!CFileIO::XCopy(&PFSFileIO, gpFileIO, "\\", argv[2], CopyCallback, 0))
	{
		printf("XCopy: error");
	}

	PFSFileIO.CloseDisk();	
	return 0;
}
