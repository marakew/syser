
#ifndef _PEHEAD_HPP_
#define _PEHEAD_HPP_

#pragma pack(push,1)
struct PE_DATA_DIR
{
  /* 0x0000 */ unsigned long VirtualAddress;
  /* 0x0004 */ unsigned long Size;
}; /* size: 0x0008 */
#pragma pack(pop)

#pragma pack(push,1)
struct PE_HEAD
{
  /* 0x0000 */ unsigned long Signature;
//_IMAGE_FILE_HEADER
  /* 0x0004 */ unsigned short Machine;
  /* 0x0006 */ unsigned short NumberOfSections;
  /* 0x0008 */ unsigned long TimeDateStamp;
  /* 0x000c */ unsigned long PointerToSymbolTable;
  /* 0x0010 */ unsigned long NumberOfSymbols;
  /* 0x0014 */ unsigned short SizeOfOptionalHeader;
  /* 0x0016 */ unsigned short Characteristics;
//_IMAGE_OPTIONAL_HEADER
  /* 0x0018 */ unsigned short Magic;
  /* 0x001a */ unsigned char MajorLinkerVersion;
  /* 0x001b */ unsigned char MinorLinkerVersion;
  /* 0x001c */ unsigned long SizeOfCode;
  /* 0x0020 */ unsigned long SizeOfInitializedData;
  /* 0x0024 */ unsigned long SizeOfUninitializedData;
  /* 0x0028 */ unsigned long AddressOfEntryPoint;
  /* 0x002c */ unsigned long BaseOfCode;
#ifndef _X64_
  /* 0x0030 */ unsigned long BaseOfData;
#endif
  /* 0x0034 */ ULONG_PTR ImageBase;
  /* 0x0038 */ unsigned long SectionAlignment;
  /* 0x003c */ unsigned long FileAlignment;
  /* 0x0040 */ unsigned short MajorOperatingSystemVersion;
  /* 0x0042 */ unsigned short MinorOperatingSystemVersion;
  /* 0x0044 */ unsigned short MajorImageVersion;
  /* 0x0046 */ unsigned short MinorImageVersion;
  /* 0x0048 */ unsigned short MajorSubsystemVersion;
  /* 0x004a */ unsigned short MinorSubsystemVersion;
  /* 0x004c */ unsigned long Win32VersionValue;
  /* 0x0050 */ unsigned long SizeOfImage;
  /* 0x0054 */ unsigned long SizeOfHeaders;
  /* 0x0058 */ unsigned long CheckSum;
  /* 0x005c */ unsigned short Subsystem;
  /* 0x005e */ unsigned short DllCharacteristics;
  /* 0x0060 */ ULONG_PTR SizeOfStackReserve;
  /* 0x0064 */ ULONG_PTR SizeOfStackCommit;
  /* 0x0068 */ ULONG_PTR SizeOfHeapReserve;
  /* 0x006c */ ULONG_PTR SizeOfHeapCommit;
  /* 0x0070 */ unsigned long LoaderFlags;
  /* 0x0074 */ unsigned long NumberOfRvaAndSizes;
//0 EXPORT
//1 IMPORT
//2 RESOURCE
//5 BASE RELOC
//9 TLS

  /* 0x0078 */ PE_DATA_DIR DataDirectory[16];
}; /* size: 0x00f8 */
#pragma pack(pop)

#endif