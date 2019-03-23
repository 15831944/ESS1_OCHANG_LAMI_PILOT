#pragma once

// CDlgModelManager 대화 상자입니다.

#include "Controls\ImageBMP.h"
#include "Controls\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CModelControl dialog
class CDlgModelManager : public CDialogEx
{
// Construction
public:
	CDlgModelManager(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelControl)
	enum { IDD = IDD_DIALOG_MODEL };

	CStatic	    m_staticFrame;
	CListBox	m_ctrlModel;
	CString	    m_sModelName;
	CButtonST   m_btnAutoRecip;
	CButtonST   m_btnApply;
	//}}AFX_DATA
	
	void FillListBoxWithFileName();
	void MakeNewModel();
	int  CheckModelName(CString sModelName);
	void UpdateModelName();
	void DeleteModelData(CString FileName);
    BOOL LoadModelData(CString strFileName);
    BOOL SaveModelData(CString strFileName);
    void MakeNewModelData(CString FileName);
	CString  GetRecipModel(CString str);
	void SetTransLanguage(long nSel);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CString    m_strModelName;
	CImageBMP  m_ImageBMP;
	LPBYTE     m_fmR;                    // Red Image Buffer
	LPBYTE     m_fmG;                    // Green Image Buffer
	LPBYTE     m_fmB;                    // Blue Image Buffer

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CModelControl)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonSelect();
	afx_msg void OnDblclkListModel();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonSaveas();
	afx_msg void OnButtonRename();
	afx_msg void OnButtonDelete();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApply();
	CString m_editPriorityNum;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckAutoRecip();
};