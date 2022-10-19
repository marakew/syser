
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "pluginlistform.hpp"

WISP_FORM_RES_ITEM PluginListForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 400, 200}, 0, 0xB, WSTR("Plugin List"), (void*)("\\SyserIcon\\Plugin.ico"), WSTR("Syser Plugin List !")},
	{WISP_CTRL_LIST, {5, 5, 300, 160}, 1, 0x10006, 0, 0, 0},
	{WISP_CTRL_STATIC_DIB, {315, 30, 64, 64}, 0, 0, 0, (void*)("\\SyserIcon\\Plugin.ico"), 0},
	{WISP_CTRL_BUTTON, {312, 120, 70, 20}, WISP_ID_OK, 0, WSTR("OK"), 0, 0},
	{0},
};

	CPluginListForm::CPluginListForm()
	{
		m_Resource = PluginListForm;
	}

	CPluginListForm::~CPluginListForm()
	{
	}

	WISP_MSG_MAP_BEGIN(CPluginListForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
	WISP_MSG_MAP_END(CWispForm)

	void CPluginListForm::UpdateContext()
	{
		for (map<TWideString, SYSER_PLUGIN_MODULE>::IT it = gpSyser->m_PluginMap.begin();
			it != gpSyser->m_PluginMap.end(); ++it)
		{
			CListStringItem *pItem = m_pList->InsertItem(it->first.operator const WCHAR*(), 0, 0, 0, -1);
			m_pList->SetItemText(pItem, 1, it->second.PluginInfo);
		}
	}

	bool CPluginListForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pList = (CWispList *)GetFormItem(1);
		m_pList->InsertColumn(WSTR("Module Name"), 100, 0, 0, -1);
		m_pList->InsertColumn(WSTR("Information"), 190, 0, 0, -1);
		UpdateContext();
		return true;
	}

	void CPluginListForm::Create()
	{
		if (IsWindow())
		{
			UpdateContext();
			Show(WISP_SH_MINSIZE);
		} else
		{
			CWispForm::Create(0, WISP_SH_MINSIZE);
		}
	}
