//{{AFX_INCLUDES()
#include "Controls\\dsspanel.h"
#include "Controls\\BtnST.h"
#include "FlexGrid\\msflexgrid.h"
#include "afxwin.h"
//}}AFX_INCLUDES

#if !defined(AFX_DLGSEARCH_H__A434D55D_3C7B_4799_A8A1_70E48A4BB7D1__INCLUDED_)
#define AFX_DLGSEARCH_H__A434D55D_3C7B_4799_A8A1_70E48A4BB7D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSearch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgSearch dialog
class CDlgSearch : public CDialogEx
{
// Construction
public:
	CDlgSearch(CWnd* pParent = NULL);   // standard constructor
	~CDlgSearch();  

// Dialog Data
	//{{AFX_DATA(CDlgSearch)
	enum { IDD = IDD_DIALOG_SEARCH };
	CMSFlexGrid	m_listSearch;
	CButtonST	m_btnExcelData;
	CButtonST	m_btnCheckGood;
	CButtonST	m_btnCheckBad;
	CButtonST	m_btnPrev;
	CButtonST	m_btnNext;
	CButtonST	m_btnInsertMaxGap;
	CButtonST	m_btnSearch;
	CButtonST   m_btnCheckCam1;
	CButtonST   m_btnCheckCam2;

	CDSSPanel	m_panToMT;
	CDSSPanel	m_panToDD;
	CDSSPanel	m_panToMM;
	CDSSPanel	m_panToTT;
	CDSSPanel	m_panToYY;
	CDSSPanel	m_panFromYY;
	CDSSPanel	m_panFromTT;
	CDSSPanel	m_panFromMM;
	CDSSPanel	m_panFromDD;
	CDSSPanel	m_panFromMT;
	CDSSPanel	m_panPage;
	CComboBox   m_comboModelName;
	//}}AFX_DATA

public:
	BOOL	SaveExcelFile(CString strPath);
	void	InitButtonControls();
	void	InitListHead();
    void	UpdateControlsData();
	void	SetPageData();
	long	SearchTextDataFile();
	void	SetListData(long nPage);
	void    SetCAM(long nCAM);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	int    m_nImagePathIndex;
	int    m_nPage;
	int    m_nTotalPage;
	long   m_nTotalRecordCount;
	ULONGLONG *m_pPageFilePtr;
	long    m_nItemTotalCount[GOM_MAX_NUM];
	long    m_nItemBadCount[GOM_MAX_NUM];

	// Generated message map functions
	//{{AFX_MSG(CDlgSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSearch();
	afx_msg void OnDblclkListSearch();
	afx_msg void OnBtnExcelData();
	afx_msg void OnBtnPrev();
	afx_msg void OnBtnNext();
	afx_msg void OnCheckGood();
	afx_msg void OnCheckBad();
	afx_msg void OnDeltaposSpinFromYy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinFromMm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinFromDd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinFromTt(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinToYy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinToMm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinToDd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinToTt(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinFromMt(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinToMt(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSEARCH_H__A434D55D_3C7B_4799_A8A1_70E48A4BB7D1__INCLUDED_)
