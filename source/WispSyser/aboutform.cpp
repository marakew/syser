
#include "aboutform.hpp"
#include "../wisp/wispstatic.hpp"

WISP_FORM_RES_ITEM AboutForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 300, 250}, 0, 0xB, WSTR("About Syser"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {10, 10, 32, 32}, 0, 0, 0, (void*)("\\SyserIcon\\Syser.ico:2"), 0},
	{WISP_CTRL_STATIC_DIB, {85, 20, 16, 16}, 0, 0, 0, (void*)("\\SyserIcon\\Explorer.ico:1"), 0},
	{WISP_CTRL_STATIC_URL, {110, 20, 180, 20}, 0, 0, WSTR("https://github.com/marakew/"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {85, 50, 16, 16}, 0, 0, 0, (void*)("\\SyserIcon\\Email.ico:1"), 0},
	{WISP_CTRL_STATIC_URL, {110, 50, 180, 20}, 0, 0, WSTR("mailto:newsyser@gmail.com"), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {10, 80, 270, 110}, 0, 0, WSTR("Version Info"), 0, 0},
	{WISP_CTRL_STATIC_STRING, {20, 100, 250, 90}, 1, 0, 0, 0, WSTR("Syser Version")},
	{WISP_CTRL_BUTTON, {120, 200, 50, 18}, WISP_ID_OK, 0, WSTR("OK"), 0, 0},
	{0},
};

	CAboutForm::CAboutForm()
	{
		m_Resource = AboutForm;
	}
	CAboutForm::~CAboutForm()
	{
	}

	WISP_MSG_MAP_BEGIN(CAboutForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
	WISP_MSG_MAP_END(CWispForm)
	
	bool CAboutForm::OnCreateForm(WISP_MSG *pMsg)
	{
		CWispStaticStr *pStringItem = (CWispStaticStr *)GetFormItem(1);
		if (pStringItem)
		{
			//WCHAR szDate[64];
			//WCHAR szTime[64];
			//TStrCpy(szDate, "" __DATE__ "");
			//TStrCpy(szTime, "" __TIME__ "");

			WCHAR Ver[32];
			WCHAR Date[64];
			TStrCpy(Ver, "Develop"); //1.99.1900.1120");
			TStrCpy(Date, "" __DATE__ " " __TIME__ "");
			WCHAR Build[512];
			TSPrintf(Build, WSTR("Version : %s \n"
					"Build Date    : %s \n\n"
					"Development Team\n\n"
					"reversecode"),
					Ver, Date);
			pStringItem->SetWindowText(Build);
		}
		return true;
	}
