
#include "wispbase.hpp"
#include "wispcalcwnd.hpp"

enum
{
  BUTTON_CONFIG = 0,
  BUTTON_HELP = 1,
  EDIT_DISPLAY = 2,
  CHECKBOX_HEX = 3,
  CHECKBOX_DEC = 4,
  CHECKBOX_OCT = 5,
  CHECKBOX_BIN = 6,
  CHECKBOX_8BYTE = 7,
  CHECKBOX_4BYTE = 8,
  CHECKBOX_2BYTE = 9,
  CHECKBOX_1BYTE = 10,
  BUTTON_HYP = 11,
  BUTTON_INV = 12,
  BUTTON_A = 13,
  BUTTON_EE = 14,
  BUTTON_MR = 15,
  BUTTON_MOTHER = 16,
  BUTTON_MC = 17,
  BUTTON_AFUNCTION = 18,
  BUTTON_AC = 19,
  BUTTON_RSH = 20,
  BUTTON_PLUSMINUS = 21,
  BUTTON_B = 22,
  BUTTON_7 = 23,
  BUTTON_8 = 24,
  BUTTON_9 = 25,
  BUTTON_LBRACKET = 26,
  BUTTON_RBRACKET = 27,
  BUTTON_AND = 28,
  BUTTON_COS = 29,
  BUTTON_RECIPROCAL = 30,
  BUTTON_C = 31,
  BUTTON_4 = 32,
  BUTTON_5 = 33,
  BUTTON_6 = 34,
  BUTTON_MULTIPLYSIGN = 35,
  BUTTON_DIVESIONSIGN = 36,
  BUTTON_OR = 37,
  BUTTON_TAN = 38,
  BUTTON_FACTORIAL = 39,
  BUTTON_D = 40,
  BUTTON_1 = 41,
  BUTTON_2 = 42,
  BUTTON_3 = 43,
  BUTTON_ADDSIGN = 44,
  BUTTON_SUBSIGN = 45,
  BUTTON_LSH = 46,
  BUTTON_LOG = 47,
  BUTTON_SQUARE = 48,
  BUTTON_E = 49,
  BUTTON_DECIMAL = 50,
  BUTTON_0 = 51,
  BUTTON_EQUALSIGN = 52,
  BUTTON_PERCENTSIGN = 53,
  BUTTON_CMP = 54,
  BUTTON_MOD = 55,
  BUTTON_LN = 56,
  BUTTON_POWER = 57,
  BUTTON_F = 58,
  CONTROL_MAXNUM = 59,
};

	WISP_MSG_MAP_BEGIN(CWispCalcWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispWnd)

	CWispCalcWnd::CWispCalcWnd()
	{
		inverse = 0;
		refresh_display = 0;
		display_size = 20;
		m_NumberBitWdith = 0;
		input_limit = 0;
		input_count = 0;
		decimal_point = 0;
		precedence_base = 0;
		current_base = NB_DECIMAL;
		memory_num = 0;
		last_input = DIGIT;
		history_index = 0;
		key_pressed = 0;
		mInternalSpacing = 4;
	}

	CWispCalcWnd::~CWispCalcWnd()
	{
	}

	bool CWispCalcWnd::Create(int x, int y, CWispBaseWnd *parent, unsigned int CmdID)
	{
		return CreateWnd(WSTR("Calc"), x, y, 500, 204, parent, CmdID, 0xB, WISP_SH_MINSIZE);
	}

	bool CWispCalcWnd::OnCreate(WISP_MSG *pMsg)
	{
		WISP_RECT rc;
		rc.cy = 0;
		rc.cx = 0;
		rc.y = 0;
		rc.x = 0;

		bpA = new CWispButton;
		pbMR = new CWispButton;
		pbMRplusminus = new CWispButton;
		pbMC = new CWispButton;
		pbClear = new CWispButton;
		pbAC = new CWispButton;
		pbRsh = new CWispButton;
		pbplusmius = new CWispButton;
		bpB = new CWispButton;
		pb7 = new CWispButton;
		pb8 = new CWispButton;
		pb9 = new CWispButton;
		pbparenopen = new CWispButton;
		pbparenclose = new CWispButton;
		pband = new CWispButton;
		pbXor = new CWispButton;
		bpC = new CWispButton;
		pb4 = new CWispButton;
		pb5 = new CWispButton;
		pb6 = new CWispButton;
		pbX = new CWispButton;
		pbdivision = new CWispButton;
		pbor = new CWispButton;
		pbfactorial = new CWispButton;
		pbD = new CWispButton;
		pb1 = new CWispButton;
		pb2 = new CWispButton;
		pb3 = new CWispButton;
		pbplus = new CWispButton;
		pbminus = new CWispButton;
		pbshift = new CWispButton;
		pbsquare = new CWispButton;
		pbE = new CWispButton;
		pb0 = new CWispButton;
		pbequal = new CWispButton;
		pbnegate = new CWispButton;
		pbmod = new CWispButton;
		pbpower = new CWispButton;
		pbF = new CWispButton;

		anglebutton[0] = new CWispRadioBox;
		anglebutton[1] = new CWispRadioBox;
		anglebutton[2] = new CWispRadioBox;
		anglebutton[3] = new CWispRadioBox;

		basebutton[0] = new CWispRadioBox;
		basebutton[1] = new CWispRadioBox;
		basebutton[2] = new CWispRadioBox;
		basebutton[3] = new CWispRadioBox;

		//
		rc.x = 15;
		rc.y = 33;
		rc.cx = 40;
		rc.cy = 16;
		basebutton[0]->Create(WSTR("Hex"), rc, this, CHECKBOX_HEX, 0x20040, WISP_SH_MINSIZE);

		rc.y += 29;
		rc.x = 15;
		rc.cx = 40;
		rc.cy = 25;
		basebutton[1]->Create(WSTR("Dec"), rc, this, CHECKBOX_DEC, 0x20040, WISP_SH_MINSIZE);

		rc.y += 29;
		rc.x = 15;
		rc.cx = 40;
		rc.cy = 25;
		basebutton[2]->Create(WSTR("Oct"), rc, this, CHECKBOX_OCT, 0x20040, WISP_SH_MINSIZE);

		rc.y += 29;
		rc.x = 15;
		rc.cx = 40;
		rc.cy = 25;
		basebutton[3]->Create(WSTR("Bin"), rc, this, CHECKBOX_BIN, 0x20040, WISP_SH_MINSIZE);

		rc.x = 90;
		rc.y = 33;
		rc.cx = 58;
		rc.cy = 16;
		anglebutton[0]->Create(WSTR("64 Bit"), rc, this, CHECKBOX_8BYTE, 0x20040, WISP_SH_MINSIZE);

		rc.y += 29;
		rc.x = 90;
		rc.cx = 58;
		rc.cy = 25;
		anglebutton[1]->Create(WSTR("32 Bit"), rc, this, CHECKBOX_4BYTE, 0x20040, WISP_SH_MINSIZE);

		rc.y += 29;
		rc.x = 90;
		rc.cx = 58;
		rc.cy = 25;
		anglebutton[2]->Create(WSTR("16 Bit"), rc, this, CHECKBOX_2BYTE, 0x20040, WISP_SH_MINSIZE);

		rc.y += 29;
		rc.x = 90;
		rc.cx = 58;
		rc.cy = 25;
		anglebutton[3]->Create(WSTR(" 8 Bit"), rc, this, CHECKBOX_1BYTE, 0x20040, WISP_SH_MINSIZE);

		anglebutton[0]->Enable(0);
		anglebutton[1]->Enable(1);
		anglebutton[2]->Enable(0);
		anglebutton[3]->Enable(0);

		basebutton[0]->Enable(1);
		basebutton[1]->Enable(0);
		basebutton[2]->Enable(0);
		basebutton[3]->Enable(0);

		//
		WISP_RECT rc1;

		rc.y = 32;
		rc1.y = 32;
		rc.x = 268;
		rc1.x = 268;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbMR->Create(WSTR("MR"), rc1, this, BUTTON_MR, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc.y = 32;
		rc.cx = 40;
		rc.cy = 16;
		rc1.x = rc.x;
		rc1.y = 32;
		rc1.cx = 40;
		rc1.cy = 16;
		pbMRplusminus->Create(WSTR("M+-"), rc1, this, BUTTON_MOTHER, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc.y = 32;
		rc1.y = 32;
		rc1.x = rc.x;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbMC->Create(WSTR("MC"), rc1, this, BUTTON_MC, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc.y = 32;
		rc1.y = 32;
		rc1.x = rc.x;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbClear->Create(WSTR("C"), rc1, this, BUTTON_AFUNCTION, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc.y = 32;
		rc1.y = 32;
		rc1.x = rc.x;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbAC->Create(WSTR("AC"), rc1, this, BUTTON_AC, 0, WISP_SH_MINSIZE);

		rc.y += 20;
		rc.x = 180;
		rc1.x = 180;
		rc.cx = 40;
		rc.cy = 16;
		rc1.y = rc.y;
		rc1.cx = 40;
		rc1.cy = 16;
		pbplusmius->Create(WSTR("+/-"), rc1, this, BUTTON_PLUSMINUS, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb7->Create(WSTR("7"), rc1, this, BUTTON_7, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb8->Create(WSTR("8"), rc1, this, BUTTON_8, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb9->Create(WSTR("9"), rc1, this, BUTTON_9, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbparenopen->Create(WSTR("("), rc1, this, BUTTON_LBRACKET, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbparenclose->Create(WSTR(")"), rc1, this, BUTTON_RBRACKET, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pband->Create(WSTR("And"), rc1, this, BUTTON_AND, 0, WISP_SH_MINSIZE);

		rc.y += 20;
		rc.x = 224;
		rc1.x = 224;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb4->Create(WSTR("4"), rc1, this, BUTTON_4, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb5->Create(WSTR("5"), rc1, this, BUTTON_5, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc.cx = 40;
		rc.cy = 16;
		rc1.y = rc.y;
		rc1.cx = 40;
		rc1.cy = 16;
		pb6->Create(WSTR("6"), rc1, this, BUTTON_6, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbX->Create(WSTR("X"), rc1, this, BUTTON_MULTIPLYSIGN, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbdivision->Create(WSTR("/"), rc1, this, BUTTON_DIVESIONSIGN, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbor->Create(WSTR("Or"), rc1, this, BUTTON_OR, 0, WISP_SH_MINSIZE);

		rc.y += 20;
		rc.x = 180;
		rc1.x = 180;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbfactorial->Create(WSTR("x!"), rc1, this, BUTTON_FACTORIAL, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb1->Create(WSTR("1"), rc1, this, BUTTON_1, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb2->Create(WSTR("2"), rc1, this, BUTTON_2, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb3->Create(WSTR("3"), rc1, this, BUTTON_3, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbplus->Create(WSTR("+"), rc1, this, BUTTON_ADDSIGN, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc.cx = 40;
		rc.cy = 16;
		rc1.y = rc.y;
		rc1.cx = 40;
		rc1.cy = 16;
		pbminus->Create(WSTR("-"), rc1, this, BUTTON_SUBSIGN, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbshift->Create(WSTR("Lsh"), rc1, this, BUTTON_LSH, 0, WISP_SH_MINSIZE);

		rc.y += 20;
		rc.x = 180;
		rc1.x = 180;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbsquare->Create(WSTR("x^2"), rc1, this, BUTTON_SQUARE, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pb0->Create(WSTR("0"), rc1, this, BUTTON_0, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbequal->Create(WSTR("="), rc1, this, BUTTON_EQUALSIGN, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbnegate->Create(WSTR("Not"), rc1, this, BUTTON_CMP, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbmod->Create(WSTR("Mod"), rc1, this, BUTTON_MOD, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbRsh->Create(WSTR("Rsh"), rc1, this, BUTTON_RSH, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbXor->Create(WSTR("Cos"), rc1, this, BUTTON_COS, 0, WISP_SH_MINSIZE);

		rc.y += 20;
		rc.x = 180;
		rc1.x = 180;
		rc.cx = 40;
		rc.cy = 16;
		rc1.y = rc.y;
		rc1.cx = 40;
		rc1.cy = 16;
		pbpower->Create(WSTR("x^y"), rc1, this, BUTTON_POWER, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		bpA->Create(WSTR("A"), rc1, this, BUTTON_A, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		bpB->Create(WSTR("B"), rc1, this, BUTTON_B, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		bpC->Create(WSTR("C"), rc1, this, BUTTON_C, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbD->Create(WSTR("D"), rc1, this, BUTTON_D, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cx = 40;
		rc.cy = 16;
		rc1.cx = 40;
		rc1.cy = 16;
		pbE->Create(WSTR("E"), rc1, this, BUTTON_E, 0, WISP_SH_MINSIZE);

		rc.x += 44;
		rc.cx = 40;
		rc1.cx = 40;
		rc1.x = rc.x;
		rc1.y = rc.y;
		rc.cy = 16;
		rc1.cy = 16;
		pbF->Create(WSTR("F"), rc1, this, BUTTON_F, 0, WISP_SH_MINSIZE);

		//
		//set_precision();
		UpdateDisplay();
		//set_style();
		Base_Selected(0);
		//InitializeCalculator();
		LoadTitleDIB("\\WispApp\\Calc\\Calc.bmp", 0);
		m_pTitleDIB->SetType(2);
		m_pTitleDIB->SetColorKey(0);
		return true;
	}

	bool CWispCalcWnd::OnDestroy(WISP_MSG *pMsg)
	{
		bpA->Destroy();
		delete bpA;

		pbMR->Destroy();
		delete pbMR;

		pbMRplusminus->Destroy();
		delete pbMRplusminus;

		pbMC->Destroy();
		delete pbMC;

		pbClear->Destroy();
		delete pbClear;

		pbAC->Destroy();
		delete pbAC;

		pbRsh->Destroy();
		delete pbRsh;

		pbplusmius->Destroy();
		delete pbplusmius;

		bpB->Destroy();
		delete bpB;

		pb7->Destroy();
		delete pb7;

		pb8->Destroy();
		delete pb8;

		pb9->Destroy();
		delete pb9;

		pbparenopen->Destroy();
		delete pbparenopen;

		pbparenclose->Destroy();
		delete pbparenclose;

		pband->Destroy();
		delete pband;

		pbXor->Destroy();
		delete pbXor;

		bpC->Destroy();
		delete bpC;

		pb4->Destroy();
		delete pb4;

		pb5->Destroy();
		delete pb5;

		pb6->Destroy();
		delete pb6;

		pbX->Destroy();
		delete pbX;

		pbdivision->Destroy();
		delete pbdivision;

		pbor->Destroy();
		delete pbor;

		pbfactorial->Destroy();
		delete pbfactorial;

		pbD->Destroy();
		delete pbD;

		pb1->Destroy();
		delete pb1;

		pb2->Destroy();
		delete pb2;

		pb3->Destroy();
		delete pb3;

		pbplus->Destroy();
		delete pbplus;

		pbminus->Destroy();
		delete pbminus;

		pbshift->Destroy();
		delete pbshift;

		pbsquare->Destroy();
		delete pbsquare;

		pbE->Destroy();
		delete pbE;

		pb0->Destroy();
		delete pb0;

		pbequal->Destroy();
		delete pbequal;

		pbnegate->Destroy();
		delete pbnegate;

		pbmod->Destroy();
		delete pbmod;

		pbpower->Destroy();
		delete pbpower;

		pbF->Destroy();
		delete pbF;
		//
		anglebutton[3]->Destroy();
		delete anglebutton[3];

		anglebutton[2]->Destroy();
		delete anglebutton[2];

		anglebutton[1]->Destroy();
		delete anglebutton[1];

		anglebutton[0]->Destroy();
		delete anglebutton[0];
		//
		basebutton[3]->Destroy();
		delete basebutton[3];

		basebutton[2]->Destroy();
		delete basebutton[2];

		basebutton[1]->Destroy();
		delete basebutton[1];

		basebutton[0]->Destroy();
		delete basebutton[0];
		//
		return true;
	}

	bool CWispCalcWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.bKeyDown)
			OnKeyDown(pMsg);
		else	OnKeyUp(pMsg);
		Update((WISP_RECT*)0);
		return true;
	}

	bool CWispCalcWnd::OnKeyDown(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}

	bool CWispCalcWnd::OnKeyUp(WISP_MSG *pMsg)
	{
		//
		return true;
	}

	bool CWispCalcWnd::OnCommand(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CtrlType == WISP_CTRL_RADIO_BOX)
		{
			switch (pMsg->Command.CmdID)
			{
			case CHECKBOX_HEX:
				Hex_Selected(); break;
			case CHECKBOX_DEC:
				Dec_Selected(); break;
			case CHECKBOX_OCT:
				Oct_Selected(); break;
			case CHECKBOX_BIN:
				Bin_Selected(); break;
			case CHECKBOX_8BYTE:
				Qword_Selected(); break;
			case CHECKBOX_4BYTE:
				Dword_Selected(); break;
			case CHECKBOX_2BYTE:
				Word_Selected();  break;
			case CHECKBOX_1BYTE:
				Byte_Selected();  break;
			default: break;
			}
		}

		if (pMsg->Command.CtrlType == WISP_CTRL_BUTTON && pMsg->Command.CmdMsg == 0x80000002)
		{
			switch (pMsg->Command.CmdID)
			{
			
			}
		}
		return true;
	}

	bool CWispCalcWnd::OnUpdateClient(WISP_MSG *pMsg)
	{
		CWispWnd::OnUpdateClient(pMsg);

		WISP_RECT rc;
		WCHAR DisplayOut[100];

		rc.x = 5;
		rc.y = 31;
		rc.cx = 64;
		rc.cy = 117;
		m_ClientDC.DrawRect(&rc, m_pWispBase->m_pWispDrawObj->m_crSystem[2]);

		rc.x = 85;
		rc.y = 31;
		rc.cx = 66;
		rc.cy = 117;
		m_ClientDC.DrawRect(&rc, m_pWispBase->m_pWispDrawObj->m_crSystem[2]);

		rc.x = 5;
		rc.y = 156;
		rc.cx = 49;
		rc.cy = 17;
		m_ClientDC.DrawFrameRect(&rc, 0, 1);
		m_ClientDC.DrawString(WSTR("NORM"), &rc, 5, 0, -1);

		rc.x = 58;
		rc.y = 156;
		rc.cx = 49;
		rc.cy = 17;
		m_ClientDC.DrawFrameRect(&rc, 0, 1);
		m_ClientDC.DrawString(WSTR(""), &rc, 5, 0, -1);

		rc.x = 111;
		rc.y = 156;
		rc.cx = 374;
		rc.cy = 17;
		m_ClientDC.DrawFrameRect(&rc, 0, 1);

		rc.x = 5;
		rc.y = 4;
		rc.cx = 479;
		rc.cy = 22;
		m_ClientDC.DrawFullRect(&rc, m_pWispBase->m_pWispDrawObj->m_crSystem[1]);
		m_ClientDC.DrawRect(&rc, m_pWispBase->m_pWispDrawObj->m_crSystem[0]);
		rc.cx -= 4;
		TStrToStr(display_str, DisplayOut);
		m_ClientDC.DrawString(DisplayOut, &rc, 6, 0, -1);
		
		return false;
	}

	void CWispCalcWnd::Create()
	{
		if (IsWindow())
		{
			Show(WISP_SH_MINSIZE);
		} else
		{
			Create(0, 0, 0, 0);
			Center(nullptr);
		}
	}

	void CWispCalcWnd::UpdateDisplay()
	{
		//TODO
		Update((WISP_RECT*)0);
	}

	void CWispCalcWnd::angle_selected(int number)
	{
		if (number == 0)
		{
			m_NumberBitWdith = 0;
		} else
		if (number == 1)
		{
			m_NumberBitWdith = 1;
		} else
		if (number == 2)
		{
			m_NumberBitWdith = 2;
		} else
		if (number == 3)
		{
			m_NumberBitWdith = 1;
		} else
			m_NumberBitWdith = 1;
	}

	void CWispCalcWnd::Qword_Selected()
	{
		anglebutton[0]->Enable(1);
		anglebutton[1]->Enable(0);
		anglebutton[2]->Enable(0);
		anglebutton[3]->Enable(0);
		angle_selected(0);
	}

	void CWispCalcWnd::Dword_Selected()
	{
		anglebutton[0]->Enable(0);
		anglebutton[1]->Enable(1);
		anglebutton[2]->Enable(0);
		anglebutton[3]->Enable(0);
		angle_selected(1);
	}

	void CWispCalcWnd::Word_Selected()
	{
		anglebutton[0]->Enable(0);
		anglebutton[1]->Enable(0);
		anglebutton[2]->Enable(1);
		anglebutton[3]->Enable(0);
		angle_selected(2);
	}

	void CWispCalcWnd::Byte_Selected()
	{
		anglebutton[0]->Enable(0);
		anglebutton[1]->Enable(0);
		anglebutton[2]->Enable(0);
		anglebutton[3]->Enable(1);
		angle_selected(3);
	}

	void CWispCalcWnd::Hex_Selected()
	{
		basebutton[0]->Enable(1);
		basebutton[1]->Enable(0);
		basebutton[2]->Enable(0);
		basebutton[3]->Enable(0);
		Base_Selected(0);
	}

	void CWispCalcWnd::Dec_Selected()
	{
		basebutton[0]->Enable(0);
		basebutton[1]->Enable(1);
		basebutton[2]->Enable(0);
		basebutton[3]->Enable(0);
		Base_Selected(1);
	}

	void CWispCalcWnd::Oct_Selected()
	{
		basebutton[0]->Enable(0);
		basebutton[1]->Enable(0);
		basebutton[2]->Enable(1);
		basebutton[3]->Enable(0);
		Base_Selected(2);
	}

	void CWispCalcWnd::Bin_Selected()
	{
		basebutton[0]->Enable(0);
		basebutton[1]->Enable(0);
		basebutton[2]->Enable(0);
		basebutton[3]->Enable(1);
		Base_Selected(3);
	}

	void CWispCalcWnd::history_next()
	{
		//TODO
	}

	void CWispCalcWnd::history_prev()
	{
		//TODO
	}

	void CWispCalcWnd::Base_Selected(int number)
	{
		bpA->EnableWindow(number == 0);
		bpB->EnableWindow(number == 0);
		bpC->EnableWindow(number == 0);
		pbD->EnableWindow(number == 0);
		pbE->EnableWindow(number == 0);
		pbF->EnableWindow(number == 0);
		pb9->EnableWindow(number <= 1);
		pb8->EnableWindow(number <= 1);
		pb7->EnableWindow(number <= 2);
		pb6->EnableWindow(number <= 2);
		pb5->EnableWindow(number <= 2);
		pb4->EnableWindow(number <= 2);
		pb3->EnableWindow(number <= 2);
		pb2->EnableWindow(number <= 2);
		base_selected(number);
	}

	void CWispCalcWnd::base_selected(int number)
	{
		switch (number)
		{
		case 0:
			current_base = NB_HEX;
			display_size = 16;
			input_limit = 16;
			decimal_point = 0;
			UpdateDisplay();
			break;
		case 2:
			current_base = NB_OCTAL;
			display_size = 22;
			input_limit = 11;
			decimal_point = 0;
			UpdateDisplay();
			break;
		case 3:
			current_base = NB_BINARY;
			display_size = 64;
			input_limit = 32;
			decimal_point = 0;
			UpdateDisplay();
			break;
		default:
			current_base = NB_DECIMAL;
			display_size = 20;
			input_limit = 0;
			UpdateDisplay();
			break;
		}
	}
#if 0
bool CWispCalcWnd::OnKeyDown(WISP_MSG *pMsg)
{
	switch (pMsg->u.field_8)
	{
	case 0x8:
		key_pressed = 1;
		ClearAll();
		break;
	case 0xD:
		key_pressed = 1;
		EnterEqual();
		break;
	case 0x1B:
		key_pressed = 1;
		Clear();
		break;
	case 0x21:
		key_pressed = 1;
		Clear();
		break;
	case 0x22:
		key_pressed = 1;
		ClearAll();
		break;
	case 0x26:
		history_prev();
		break;
	case 0x28:
		history_next();
		break;
	case 0x2E:
		key_pressed = 1;
		ClearAll();
		break;
	case 0x30:
		key_pressed = 1;
		if (m_pWispBase->m_KeyMap[16])
			EnterCloseParen();
		else	EnterDigit(0);
		break;
	case 0x31:
		key_pressed = 1;
		if (m_pWispBase->m_KeyMap[16])
			EnterFactorial();
		else	EnterDigit(1);
		break;
	case 0x32:
		key_pressed = 1;
		button2();
		break;
	case 0x39:
		key_pressed = 1;
		if (m_pWispBase->m_KeyMap[16])
			EnterOpenParen();
		else	button9();
		break;

	case 0x41:
		key_pressed = 1;
		buttonA();
		break;
	case 0x42:
		key_pressed = 1;
		buttonB();
		break;
	case 0x43:
		key_pressed = 1;
		if (current_base == NB_HEX)
			buttonC();
		else	Xor();
	case 0x44:
		key_pressed = 1;
		buttonD();
		break;
	case 0x45:
		key_pressed = 1;
		if (current_base == NB_HEX && !display_error)
			buttonE();
		break;
	case 0x46:
		key_pressed = 1;
		buttonF();
		break;
	case 0x48:
		key_pressed = 1;
		break;
	case 0x49:
		key_pressed = 1;
		inverse = inverse == 0;
		break;

	//case 0x4A:
	//case 0x4B:

	case 0x4C:
		break;
	case 0x4D:
		key_pressed = 1;
		break;
	case 0x4E:
		key_pressed = 1;
		break;
	case 0x4F:
		key_pressed = 1;
		Or();
		break;
	case 0x52:
		key_pressed = 1;
		break;
	case 0x53:
		key_pressed = 1;
		Rshift();
		break;
	case 0x54:
		key_pressed = 1;
		break;
	case 0x58:
		key_pressed = 1;
		Multiply();
		break;
	case 0xBA:
		key_pressed = 1;
		if (m_pWispBase->m_KeyMap[16])
			Mod();
		break;
	case 0xBB:
		key_pressed = 1;
		if (m_pWispBase->m_KeyMap[16])
			Plus();
		else	EnterEqual();
		break;
	case 0xBD:
		key_pressed = 1;
		Minus();
		break;
	case 0xBE:
		key_pressed = 1;
		break;
	case 0xC0:
		key_pressed = 1;
		if (m_pWispBase->m_KeyMap[16])
			EnterNotCmp();
		break;
	case 0xDB:
		key_pressed = 1;
		EnterSquare();
		break;
	case 0xDC:
		key_pressed = 1;
		if (m_pWispBase->m_KeyMap[16])
			Or();
		else	EnterNegate();
		break;
	case 0x6F:
		key_pressed = 1;
		Divide();
		break;
	case 0x72:
		kcalcdefaults.style = kcalcdefaults.style == 0;
		set_style();
		break;
	
	}
}
#endif