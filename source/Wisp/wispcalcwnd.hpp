
#ifndef _WISPCALCWND_HPP_
#define _WISPCALCWND_HPP_

#include "../Code/define.h"
#include "../Code/tstrmem.hpp"
#include "../Code/alt/altlist.hpp"
#include "wispwnd.hpp"
#include "wispbutton.hpp"
#include "wispradiobox.hpp"

struct _DefStruct
{
	unsigned long forecolor;
	unsigned long backcolor;
	unsigned long numberButtonColor;
	unsigned long functionButtonColor;
	unsigned long hexButtonColor;
	unsigned long memoryButtonColor;
	unsigned long operationButtonColor;
	int precision;
	int fixedprecision;
	int style;
	char fixed;
	char beep;

	CWispFont font;
};

enum _last_input_type
{
  DIGIT = 1,
  OPERATION = 2,
  RECALL = 3,
  PASTE = 4,
};

enum _num_base
{
  NB_BINARY = 2,
  NB_OCTAL = 8,
  NB_DECIMAL = 10,
  NB_HEX = 16,
};

enum _angle_type
{
  ANG_DEGREE = 0,
  ANG_RADIAN = 1,
  ANG_GRADIENT = 2,
};

struct CWispCalcWnd: public CWispWnd
{
	CWispRadioBox *anglebutton[4];
	CWispRadioBox *basebutton[4];
	CWispButton *bpA;
	CWispButton *pbMR;
	CWispButton *pbMRplusminus;
	CWispButton *pbMC;
	CWispButton *pbClear;
	CWispButton *pbAC;
	CWispButton *pbRsh;
	CWispButton *pbplusmius;
	CWispButton *bpB;
	CWispButton *pb7;
	CWispButton *pb8;
	CWispButton *pb9;
	CWispButton *pbparenopen;
	CWispButton *pbparenclose;
	CWispButton *pband;
	CWispButton *pbXor;
	CWispButton *bpC;
	CWispButton *pb4;
	CWispButton *pb5;
	CWispButton *pb6;
	CWispButton *pbX;
	CWispButton *pbdivision;
	CWispButton *pbor;
	CWispButton *pbfactorial;
	CWispButton *pbD;
	CWispButton *pb1;
	CWispButton *pb2;
	CWispButton *pb3;
	CWispButton *pbplus;
	CWispButton *pbminus;
	CWispButton *pbshift;
	CWispButton *pbsquare;
	CWispButton *pbE;
	CWispButton *pb0;
	CWispButton *pbequal;
	CWispButton *pbnegate;
	CWispButton *pbmod;
	CWispButton *pbpower;
	CWispButton *pbF;

	_DefStruct kcalcdefaults;
	bool inverse;
	bool refresh_display;
	int display_size;
	int m_NumberBitWdith;
	int input_limit;
	int input_count;
	int decimal_point;
	int precedence_base;
	_num_base current_base;
	long long memory_num;
	_last_input_type last_input;
	char display_str[66];

	TList<long long> history_list;
	int history_index;
	bool key_pressed;
	int mInternalSpacing;

	CWispCalcWnd();
	virtual ~CWispCalcWnd();

	bool Create(int x, int y, CWispBaseWnd *parent, unsigned int CmdID);

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnKeyDown(WISP_MSG *pMsg);
	bool OnKeyUp(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);

	void Create();

	void UpdateDisplay();

	void angle_selected(int number);
	void Qword_Selected();
	void Dword_Selected();
	void Word_Selected();
	void Byte_Selected();

	void Hex_Selected();
	void Dec_Selected();
	void Oct_Selected();
	void Bin_Selected();

	void history_next();
	void history_prev();

	void Base_Selected(int number);
	void base_selected(int number);
};

#endif
