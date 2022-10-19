
#ifndef _PEEXPLORERFORM_HPP_
#define _PEEXPLORERFORM_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wisptabwnd.hpp"
#include "../Wisp/wispcombobox.hpp"
#include "../Wisp/wisplist.hpp"
#include "../EXEAnalyzer/pefile.hpp"

struct CPEHeaderWnd : public CWispWnd
{
	CWispList m_COFF;
	CWispList m_WIN;
	CListStringItem *m_pItems[33];

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool InitContext();
	bool Open(CPEFile *pPEFile);
};

struct CPERelcationWnd : public CWispWnd
{
	CWispList m_ModuleList;
	CWispList m_EntryList;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool InitContext();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
};

///
struct CPEImportModuleList : public CWispList
{
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnMouseDblClick(WISP_MSG *pMsg);
};

struct CPEImportEntryList : public CWispList
{
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnMouseDblClick(WISP_MSG *pMsg);
};

struct CPEImportWnd : public CWispWnd
{
	CPEImportModuleList m_ModuleList;
	CPEImportEntryList m_EntryList;
	CPEFile *m_pPEFile;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdSelectEntry(WISP_MSG *pMsg);

	bool InitContext();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool Open(CPEFile *pPEFile);
	ULONG_PTR GetImageBase(const char *szModuleName);
};
///
struct CPEExportList : public CWispList
{
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnMouseDblClick(WISP_MSG *pMsg);
};

struct CPEExportWnd : public CWispWnd
{
	CPEExportList m_ExportList;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool InitContext();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool Open(CPEFile *pPEFile);
};
///
struct CPEDataDirectoryList : public CWispList
{
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnMouseDblClick(WISP_MSG *pMsg);
};

struct CPEDataDirectoryWnd : public CWispWnd
{
	CPEDataDirectoryList m_DirectoryList;
	CListStringItem *m_pItems[16];

	bool OnCreate(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool InitContext();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool Open(CPEFile *pPEFile);
};

///
struct CPEExplorerForm : public CWispForm
{
	CPEFile m_MemPEFile;//CMemPEFile
	CPEHeaderWnd *m_pPEHeaderWnd;
	CWispTabWnd *m_pTabWnd;
	CPERelcationWnd *m_pPERelcationWnd;
	CPEImportWnd *m_pPEImportWnd;
	CPEExportWnd *m_pPEExportWnd;
	CPEDataDirectoryWnd *m_pPEDataDirectoryWnd;
	CWispComboBox *m_pModuleList;

	CPEExplorerForm();
	virtual ~CPEExplorerForm();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdModuleSelect(WISP_MSG *pMsg);
	bool SelectModule(ULONG_PTR Address);
	bool OpenModule(ULONG_PTR Address);
	void Open(ULONG_PTR Address);
	void Create();
};

#endif