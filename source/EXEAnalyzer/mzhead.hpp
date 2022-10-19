
#ifndef _MZHEAD_HPP_
#define _MZHEAD_HPP_

#pragma pack(push,1)
struct MZRELOC
{
  /* 0x0000 */ unsigned short Offset;
  /* 0x0002 */ unsigned short Segment;
}; /* size: 0x0004 */
#pragma pack(pop)

#pragma pack(push,1)
struct MZHEAD
{
  /* 0x0000 */ unsigned short Signature;
  /* 0x0002 */ unsigned short cbLastPage;
  /* 0x0004 */ unsigned short nPage;
  /* 0x0006 */ unsigned short nReloc;
  /* 0x0008 */ unsigned short cpHead;
  /* 0x000a */ unsigned short Minalloc;
  /* 0x000c */ unsigned short Maxalloc;
  /* 0x000e */ unsigned short SS;
  /* 0x0010 */ unsigned short SP;
  /* 0x0012 */ unsigned short CheckSum;
  /* 0x0014 */ unsigned short IP;
  /* 0x0016 */ unsigned short CS;
  /* 0x0018 */ unsigned short fpReloc;
  /* 0x001a */ unsigned short nOverlay;
  /* 0x001c */ unsigned short Reserved[4];
  /* 0x0024 */ unsigned short OemID;
  /* 0x0026 */ unsigned short OemInfo;
  /* 0x0028 */ unsigned short Reserved2[10];
  /* 0x003c */ long pfNewHead;
}; /* size: 0x0040 */
#pragma pack(pop)

#endif