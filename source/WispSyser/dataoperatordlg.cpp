
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "dataoperatordlg.hpp"
#include "resource.hpp"

	//TODO Value Hex/Dec update input

WISP_FORM_RES_ITEM DataOperatorDlg[] =
{
	{WISP_CTRL_FORM, {0, 0, 340, 240}, 0, 0xB, WSTR("Data Operation"), (void*)"\\SyserIcon\\Plugin.ico", 0},
	{WISP_CTRL_STATIC_GROUP, {11, 6, 313, 53}, 0, 0, WSTR("Description"), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {11, 64, 313, 80}, 0, 0, WSTR("Data Width"), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {11, 151, 234, 55}, 0, 0, WSTR("Length"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {25, 76, 96, 16}, 3, 0x1000006, WSTR("BYTE (Bit 8)"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {180, 76, 110, 16}, 4, 0x1000006, WSTR("WORD (Bit 16)"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {25, 96, 110, 16}, 5, 0x1000006, WSTR("DWORD (Bit 32)"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {180, 96, 110, 16}, 6, 0x1000006, WSTR("QWORD (Bit 64)"), 0, 0},
	{WISP_CTRL_STATIC_STRING, {14, 20, 310, 50}, 7, 0, WSTR(""), 0, 0},
	{WISP_CTRL_STATIC_STRING, {25, 120, 40, 16}, 0xA, 0, WSTR("Value:"), 0, 0},
	{WISP_CTRL_EDIT, {65, 120, 105, 16}, 0xE, 0x1000006, 0, 0, 0},
	{WISP_CTRL_RADIO_BOX, {180, 119, 65, 16}, 9, 0x1000006, WSTR("Decimal"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {250, 119, 110, 16}, 8, 0x1000006, WSTR("Hex"), 0, 0},
	{WISP_CTRL_STATIC_STRING, {16, 167, 90, 16}, 0xB, 0, WSTR("Begin Address:"), 0, 0},
	{WISP_CTRL_EDIT, {105, 166, 80, 16}, 0xC, 0x1000006, 0, 0, 0},
	{WISP_CTRL_STATIC_STRING, {16, 185, 90, 16}, 0xB, 0, WSTR("  End Address:"), 0, 0},
	{WISP_CTRL_EDIT, {105, 185, 80, 16}, 0xD, 0x1000006, 0, 0, 0},
	{WISP_CTRL_CHECK_BOX, {190, 184, 60, 16}, 0xF, 0x20000000, WSTR("Count"), 0, 0},
	{WISP_CTRL_BUTTON, {255, 155, 68, 20}, 1, 0, WSTR("OK"), 0, 0},
	{WISP_CTRL_BUTTON, {255, 185, 68, 20}, 2, 0, WSTR("Cancel"), 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CDataOperatorDlg)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

	WISP_EVENT_MAP_BEGIN(CDataOperatorDlg)
		WISP_EVENT_MAP(0x1, OnEventExecOp)
		WISP_EVENT_MAP(0x2, OnEventClose)
	WISP_EVENT_MAP_END

	WISP_CMD_MAP_BEGIN(CDataOperatorDlg)
		WISP_CMD_MAP(0xE, OnCmd)
	WISP_CMD_MAP_END

	CDataOperatorDlg::CDataOperatorDlg()
	{
		m_Resource = DataOperatorDlg;
	}
	CDataOperatorDlg::~CDataOperatorDlg()
	{
	}

	const WCHAR *DescOp[] =
		{
			WSTR("Fips the byte order of you data. For example, the \nword 0xABCD would become 0xCDAB."),
			WSTR("Inverts the bits of your data. For example, the \nvalue 0x35 (00110101 in binary) would become 0xCA\n(11001010 in binary)."),
			WSTR("Shift the bits of your data to the left. For \nexample,the value 0x08 (00001000 in binary) would \nbecome 0x10 (00010000 in binary)."),
			WSTR("Shift the bits of your data to the right. For \nexample,the value 0x08(00001000 in binary) would \nbecome 0x04 (00000100 in binary)."),
			WSTR("Rotates the bits of your data to the left. For\nexample,the value 0x98 (10011000 in binary) would \nbecome 0x31 (00110001 in binary)."),
			WSTR("Rotates the bits of your data to the right. For\nexampel,the value 0x61 (01100001 in binary) would \nbecome 0xB0 (10110000 in binary)."),
			WSTR("Shift the bits of your data to the left across byte\nboundaries. For example,the value 0x08(00001000 in\nbinary) would become 0x10 (00010000 in binary)."),
			WSTR("Shift the bits of your data to the right across byte\nboundaries. For example,the value 0x08 (00001000 in\nbinary) would become 0x04 (00000100 in binary)."),
			WSTR("Performs a XOR operation. For example,the value\n0xFO (11110000 in binary) XOR 0xAA (10101010 in \nbinary) is 0x5A (01011010 in binary)."),
			WSTR("Performs an OR operation. For example,the value\n0xF0 (11110000 in binary) OR 0xAA (10101010 in \nbinary) is 0xFA (11111010 in binary)."),
			WSTR("Performs an AND operation. For example,the value\n0xF0 (11110000 in binary) AND 0xAA (10101010 in \nbinary) is 0xA0 (10100000 in binary)."),
			WSTR("Change the sign of your data. For example,a value of\n5 would become -5 or a value of -5 would become 5."),
			WSTR("Replaces your data with your data plus another value\n(data = data + value)."),
			WSTR("Replaces your data with your data minus another\nvalue (data = data - value)."),
			WSTR("Replaces your data with your data multiplied by \nanother value (data = data * value)."),
			WSTR("Replaces your data with your data divided by another\nvalue (data = data / value)."),
			WSTR("Replaces your data with your data mod another value\n(data = data % value)."),
			WSTR("Replaces your character data with all upper case\ncharacters. For example,the text \"Test String\"\nwould become \"TEST STRING\"."),
			WSTR("Replaces your character data with all lower case\ncharacters. For example,the text \"Test String\"\nwould become \"test string\"."),
			WSTR("Replaces your character data with the inverse \ncharacter case. For example, the next \"Test String\" would\nbecome \"tEST sTRING\"."),
		};

	const WCHAR *NameOp[] =
		{
			WSTR("ByteFlip Operation"),
			WSTR("Inverse Bits Operation"),
			WSTR("Shift Left Operation"),
			WSTR("Shift Right Operation"),
			WSTR("Rotate Left Operation"),
			WSTR("Rotate Right Operation"),
			WSTR("Block Shift Left Operation"),
			WSTR("Block Shift Right Operation"),
			WSTR("XOR Operation"),
			WSTR("OR Operation"),
			WSTR("AND Operation"),
			WSTR("Change Sign Operation"),
			WSTR("Add Operation"),
			WSTR("Subtract Operation"),
			WSTR("Multiply Operation"),
			WSTR("Divide Operation"),
			WSTR("Mod Operation"),
			WSTR("Upper Case Operation"),
			WSTR("Lower Case Operation"),
			WSTR("Inverse Case Operation"),
		};

	void ByteFlipOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length && Size != 1)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char ReadBuffer[8];
				unsigned char WriteBuffer[8];

				if (gpSyser->m_pDebugger->ReadMemory(Address, ReadBuffer, Size) != Size) break;

				for (unsigned long n = 0; n <= Size-1; ++n)
				{
					WriteBuffer[n] = ReadBuffer[Size-1-n];
				}

				if (gpSyser->m_pDebugger->WriteMemory(Address, WriteBuffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void InverseOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1);
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, 1) != 1) break;

				Buffer[0] = ~Buffer[0];

				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, 1) != 1) break;
				++Address;
			}
		}
	}

	void SHLOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) <<= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) <<= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) <<= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) <<= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void SHROp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) >>= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) >>= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) >>= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) >>= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

#define ROL(x,n,r) (((x) >> (unsigned char)(((n)<<3)-(r))) | ((x) << (unsigned char)(r)))

#define ROR(x,n,r) (((x) << (unsigned char)(((n)<<3)-(r))) | ((x) >> (unsigned char)(r)))

	void ROLOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			unsigned long r = Value % (8 * Size);
			if (r)
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) = ROL(*(unsigned char*)&(Buffer[0]), 1, r);
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) = ROL(*(unsigned short*)&(Buffer[0]), 2, r);
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) = ROL(*(unsigned long*)&(Buffer[0]), 4, r);
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) = ROL(*(unsigned long long*)&(Buffer[0]), 8, r);
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void ROROp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			unsigned long r = Value % (8 * Size);
			if (r)
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) = ROR(*(unsigned char*)&(Buffer[0]), 1, r);
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) = ROR(*(unsigned short*)&(Buffer[0]), 2, r);
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) = ROR(*(unsigned long*)&(Buffer[0]), 4, r);
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) = ROR(*(unsigned long long*)&(Buffer[0]), 8, r);
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void BlockSHLOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1);
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, 1) != 1) break;

				*(unsigned char*)&(Buffer[0]) <<= Value;

				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, 1) != 1) break;
				++Address;
			}
		}
	}

	void BlockSHROp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1);
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, 1) != 1) break;

				*(unsigned char*)&(Buffer[0]) >>= Value;

				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, 1) != 1) break;
				++Address;
			}
		}
	}

	void XOROp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) ^= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) ^= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) ^= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) ^= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void OROp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) |= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) |= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) |= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) |= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void ANDOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) &= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) &= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) &= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) &= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void ChangeSignOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) = - *(unsigned char*)&(Buffer[0]);
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) = - *(unsigned short*)&(Buffer[0]);
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) = - *(unsigned long*)&(Buffer[0]);
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) = -*(unsigned long long*)&(Buffer[0]);
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void AddOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) += Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) += Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) += Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) += Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void SubtractOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) -= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) -= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) -= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) -= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void MultiplyOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) *= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) *= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) *= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) *= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void DivideOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length && Value)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) /= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) /= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) /= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) /= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void ModOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1) / Size;
		if (Length && Value)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size) != Size) break;
				switch (Size)
				{
				case 1:
					*(unsigned char*)&(Buffer[0]) %= Value;
					break;
				case 2:
					*(unsigned short*)&(Buffer[0]) %= Value;
					break;
				case 4:
					*(unsigned long*)&(Buffer[0]) %= Value;
					break;
				case 8:
					*(unsigned long long*)&(Buffer[0]) %= Value;
					break;
				}
				if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size) != Size) break;
				Address += Size;
			}
		}
	}

	void UpperCaseOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1);
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, 1) != 1) break;

				//if (Buffer[0] >= 'a' && Buffer[0] <= 'z')
				if (IS_LOWER(Buffer[0]))
				{
					Buffer[0] -= ' ';
					if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, 1) != 1) break;
				}
				++Address;
			}
		}
	}

	void LowerCaseOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1);
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, 1) != 1) break;

				//if (Buffer[0] >= 'A' && Buffer[0] <= 'Z')
				if (IS_UPPER(Buffer[0]))
				{
					Buffer[0] += ' ';
					if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, 1) != 1) break;
				}
				++Address;
			}
		}
	}

	void InverseCaseOp(ULONG_PTR Begin,ULONG_PTR End,int Size,long long Value)
	{
		ULONG_PTR Address = Begin;
		unsigned long Length = (End - Begin + 1);
		if (Length)
		{
			for (unsigned long i = 0; i < Length; ++i)
			{
				unsigned char Buffer[8];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Buffer, 1) != 1) break;

				//if ((Buffer[0] >= 'A' && Buffer[0] <= 'Z') || (Buffer[0] >= 'a' && Buffer[0] <= 'z'))
				if (IS_UPPER(Buffer[0]) || IS_LOWER(Buffer[0]))
				{
					Buffer[0] ^= 0x20;
					if (gpSyser->m_pDebugger->WriteMemory(Address, Buffer, 1) != 1) break;
				}
				++Address;
			}
		}
	}

	void (*FuncsOp[])(ULONG_PTR,ULONG_PTR,int,long long) =
		{
			ByteFlipOp,
			InverseOp,
			SHLOp,
			SHROp,
			ROLOp,
			ROROp,
			BlockSHLOp,
			BlockSHROp,
			XOROp,
			OROp,
			ANDOp,
			ChangeSignOp,
			AddOp,
			SubtractOp,
			MultiplyOp,
			DivideOp,
			ModOp,
			UpperCaseOp,
			LowerCaseOp,
			InverseCaseOp,
		};

	bool CDataOperatorDlg::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pOK = (CWispButton *)GetFormItem(1);
		m_pCancel = (CWispButton *)GetFormItem(2);

		m_pWidth8 = (CWispRadioBox *)GetFormItem(3);
		m_pWidth16 = (CWispRadioBox *)GetFormItem(4);
		m_pWidth32 = (CWispRadioBox *)GetFormItem(5);
		m_pWidth64 = (CWispRadioBox *)GetFormItem(6);

		m_pNameDesc = (CWispStaticStr *)GetFormItem(7);
		m_pDec = (CWispRadioBox *)GetFormItem(9);
		m_pHex = (CWispRadioBox *)GetFormItem(8);
		m_pCount = (CWispCheckBox *)GetFormItem(0xF);
		m_pValueName = (CWispStaticStr *)GetFormItem(0xA);
		m_pBegin = (CWispEdit *)GetFormItem(0xC);
		m_pEnd = (CWispEdit *)GetFormItem(0xD);
		m_pValue = (CWispEdit *)GetFormItem(0xE);

		WCHAR DisplayOut[20];

		TSPrintf(DisplayOut, WSTR("%x"), m_Begin);
		m_pBegin->SetWindowText(DisplayOut);

		TSPrintf(DisplayOut, WSTR("%x"), m_End);
		m_pEnd->SetWindowText(DisplayOut);

		if (m_Len == 1)
			m_pWidth8->Enable(1);
		if (m_Len == 2)
			m_pWidth16->Enable(1);
		if (m_Len == 4)
			m_pWidth32->Enable(1);

		m_pNameDesc->SetWindowText(DescOp[m_Op]);
		SetWindowText(NameOp[m_Op]);

		m_pHex->Enable(1);
		unsigned int Op = m_Op + EVENT_ID_OP_BFLIP; //0xA9;
		if (Op == EVENT_ID_OP_BFLIP || //0xA9
		    Op == EVENT_ID_OP_BINV || //0xAA
		    Op == EVENT_ID_OP_SIG || //0xB4
		    Op == EVENT_ID_OP_UPR || //0xBA
		    Op == EVENT_ID_OP_LWR || //0xBB
		    Op == EVENT_ID_OP_INV) //0xBC
		{
			m_pValue->EnableWindow(0);
			m_pDec->EnableWindow(0);
			m_pHex->EnableWindow(0);
		} else
		{
			m_pOK->EnableWindow(0);
		}
		return true;
	}
	bool CDataOperatorDlg::OnDestroy(WISP_MSG *pMsg)
	{
		return true;
	}

	bool CDataOperatorDlg::Create(CWispWnd *pParentWnd, int ShowMode, ULONG_PTR Start, ULONG_PTR End, unsigned long Length, unsigned int CmdID)
	{
		m_Begin = Start;
		m_End = End;
		m_Len = Length;
		m_Op = CmdID - EVENT_ID_OP_BFLIP;
		return CWispForm::Create(pParentWnd, ShowMode);
	}

	int CDataOperatorDlg::GetWidth()
	{
		if (m_pWidth16->m_Style & 0x400) return 2;
		if (m_pWidth32->m_Style & 0x400) return 4;
		if (m_pWidth64->m_Style & 0x400) return 8;
		return 1;
	}

	bool CDataOperatorDlg::OnCmd(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000703) //WISP_ID_EDIT_KEYDOWN
		{
			return (pMsg->Command.Param1 < 'g' || pMsg->Command.Param1 > 'z') &&
			       (pMsg->Command.Param1 < 'G' || pMsg->Command.Param1 > 'Z') &&
			       ((m_pHex->m_Style & 0x400) ||
			        ((pMsg->Command.Param1 < 'a' || pMsg->Command.Param1 > 'f') &&
                                 (pMsg->Command.Param1 < 'A' || pMsg->Command.Param1 > 'F')));
		} else
		if (pMsg->Command.CmdMsg == 0x80000705) //WISP_ID_EDIT_CHANGED
		{
			ULONG_PTR Num;
			if (m_pHex->m_Style & 0x400)
				m_pOK->EnableWindow(USHexStrToNum(m_pValue->GetWindowText().operator const WCHAR*(), &Num));
			else	m_pOK->EnableWindow(USDecStrToNum(m_pValue->GetWindowText().operator const WCHAR*(), &Num));
		}
		return true;
	}

	bool CDataOperatorDlg::OnEventExecOp(WISP_MSG *pMsg)
	{
		long long Value = 0;

		ULONG_PTR Begin;
		if (!USHexStrToNum(m_pBegin->GetWindowText().operator const WCHAR*(), &Begin))
			Begin = m_Begin;

		ULONG_PTR End;
		if (!USHexStrToNum(m_pEnd->GetWindowText().operator const WCHAR*(), &End))
			End = m_End;

		int Size = GetWidth();

		if (!m_pValue->GetWindowText().IsEmpty())
		{
			if (m_pHex->m_Style & 0x400)
			{
				if (!USHexStrToNum(m_pValue->GetWindowText().operator const WCHAR*(), &Value)) Value = 0;
			} else
			{
				if (!USDecStrToNum(m_pValue->GetWindowText().operator const WCHAR*(), &Value)) Value = 0;
			}
		}
		FuncsOp[m_Op](Begin, End, Size, Value);
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
		CWispBaseWnd::Close();
		return true;
	}

	bool CDataOperatorDlg::OnEventClose(WISP_MSG *pMsg)
	{
		CWispBaseWnd::Close();
		return true;
	}
