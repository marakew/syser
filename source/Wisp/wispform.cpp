
#include "wispform.hpp"
#include "wispbase.hpp"

#include "wispstatic.hpp"
#include "wispbutton.hpp"
#include "wispprogress.hpp"
#include "wisptabwnd.hpp"
#include "wispedit.hpp"
#include "wisplist.hpp"
#include "wispcombobox.hpp"
#include "wispradiobox.hpp"
#include "wispcheckbox.hpp"

	CWispForm::CWispForm()
	{
		m_Result = 0;
		m_Resource = 0;
		m_CtrlType = WISP_CTRL_FORM;
	}
	CWispForm::~CWispForm()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispForm)
		WISP_MSG_MAP_ON_EVENT
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
	WISP_MSG_MAP_END(CWispWnd)
#if 0
	WISP_EVENT_MAP_BEGIN(CWispForm)
	WISP_EVENT_MAP_END
#else
	bool CWispForm::OnEvent(WISP_MSG *pMsg)
	{
		return CWispBaseWnd::OnEvent(pMsg);
	}
#endif
	//
	WISP_CMD_MAP_BEGIN(CWispForm)
		WISP_CMD_MAP(WISP_ID_CLOSE, OnCmdClose)
		WISP_CMD_MAP(WISP_ID_OK, OnCmdOk)
		WISP_CMD_MAP(WISP_ID_CANCEL, OnCmdCancel)
		WISP_CMD_MAP_DEFAULT(OnCmdDefault)
	WISP_CMD_MAP_END

	bool CWispForm::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType == VK_ESCAPE &&
			pMsg->KeyEvent.bKeyDown &&
			(m_Style & 8))
			CWispBaseWnd::Close();
		return true;
	}
	bool CWispForm::OnCmdOk(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
		{
			m_Result = WISP_ID_OK;
			CWispBaseWnd::Close();
		}
		return false; //???
	}
	bool CWispForm::OnCmdCancel(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
		{
			m_Result = WISP_ID_CANCEL;
			CWispBaseWnd::Close();
		}
		return false; //???
	}

	bool CWispForm::OnCmdDefault(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
		{
			if (pMsg->Command.CmdID > WISP_ID_FORM_CLOSE_ID_START &&
			    pMsg->Command.CmdID < WISP_ID_FORM_CLOSE_ID_END)
			{
				m_Result = pMsg->Command.CmdID;
				CWispBaseWnd::Close();
			}
		}
		return true;
	}

	bool CWispForm::LoadForm(WISP_FORM_RES_ITEM *ItemAry)
	{
		//CWispStaticDIB
		//CWispEdit
		//CWispButton
		//CWispStaticURL
		//CWispStaticGroup
		//CWispStaticStr
		//CWispList
		//CWispCheckBox
		//CWispTabWnd
		//CWispRadioBox
		//CWispProgress
		//CWispComboBox

		CWispBaseWnd *pWnd;

		while (ItemAry->Type)
		{
			switch (ItemAry->Type)
			{
			case WISP_CTRL_STATIC_STRING:
				pWnd = new CWispStaticStr;
				pWnd->Create(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1; //WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_STATIC_DIB:
				pWnd = new CWispStaticDIB;
				((CWispStaticDIB*)pWnd)->CreateEx(gpCurWisp->m_DIBLib.GetDIB((const char*)ItemAry->pData, 0),
						ItemAry->rc.x, ItemAry->rc.y, this, ItemAry->CmdID);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_STATIC_GROUP:
				pWnd = new CWispStaticGroup;
				pWnd->Create(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_STATIC_URL:
				pWnd = new CWispStaticURL;
				pWnd->Create(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_BUTTON:
				pWnd = new CWispButton;
				pWnd->Create(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_EDIT:
				pWnd = new CWispEdit;
				((CWispEdit*)pWnd)->Create(ItemAry->rc, ItemAry->Style, this, ItemAry->CmdID);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_LIST:
				pWnd = new CWispList;
				((CWispList*)pWnd)->CreateWnd(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_CHECK_BOX:
				pWnd = new CWispCheckBox;
				pWnd->Create(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_TAB:
				pWnd = new CWispTabWnd;
				((CWispTabWnd*)pWnd)->CreateWnd(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_RADIO_BOX:
				pWnd = new CWispRadioBox;
				pWnd->Create(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_PROGRESS:
				pWnd = new CWispProgress;
				pWnd->Create(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			case WISP_CTRL_FORM:

				if (!CreateWnd(ItemAry->pName, ItemAry->rc, m_ParentWnd, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE))
					return false;
				if (ItemAry->pData)
					LoadTitleDIB((const char*)ItemAry->pData, 0);

				if (ItemAry->pTipString)
					EnableHelpTip(ItemAry->pTipString);
				if (m_Style & 0x800)
					Focus();
				break;
			case WISP_CTRL_COMBO_BOX:
				pWnd = new CWispComboBox;
				((CWispTabWnd*)pWnd)->CreateWnd(ItemAry->pName, ItemAry->rc, this, ItemAry->CmdID, ItemAry->Style, WISP_SH_MINSIZE);
				pWnd->m_AdvStyle |= 1;	//WISP_ADV_AUTODEL
				if (ItemAry->CmdID)
					m_FormItemMap.insert(_Tmap_pair<unsigned int,CWispBaseWnd*>(ItemAry->CmdID, pWnd));
				if (ItemAry->pTipString)
					pWnd->EnableHelpTip(ItemAry->pTipString);
				if (pWnd->m_Style & 0x800)
					pWnd->Focus();
				break;
			default:
				; //???
			}
			++ItemAry;
		}

		return true;
	}
	bool CWispForm::Create(CWispWnd *pParentWnd, int ShowMode)
	{
		m_ParentWnd = pParentWnd;
		if (!LoadForm(m_Resource))
			return false;

		WISP_MSG Msg;
		m_Result = 0;
		Msg.hWnd = this;
		Msg.Msg = WISP_WM_CREATE_FORM;
		m_pWispBase->SendMessage(&Msg);
		Center(pParentWnd);
		Focus();
		Show(ShowMode);
		return true;
	}
	bool CWispForm::Destroy()
	{
		m_FormItemMap.clear();
		return CWispWnd::Destroy();
	}
	CWispBaseWnd *CWispForm::GetFormItem(unsigned int CmdID)
	{
		map<unsigned int, CWispBaseWnd*>::IT it = m_FormItemMap.find(CmdID);
		if (it == m_FormItemMap.end())
			return nullptr;
		return it->second;
	}

	unsigned int CWispForm::GetFormResult()
	{
		return m_Result;
	}
