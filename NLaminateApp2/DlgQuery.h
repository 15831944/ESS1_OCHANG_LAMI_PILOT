#pragma once

#include "Controls\\dsspanel.h"
#include "Controls\\BtnST.h"
#include "FlexGrid\\msflexgrid.h"

// CDlgQuery 대화 상자입니다.
class CDlgQuery : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgQuery)

public:
	CDlgQuery(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgQuery();

	BOOL		SaveExcelFile(CString strPath);
	void        FillListBoxWithFileName();
	void        SetTransLanguage(long nValue);
	void		InitButtonControls();
	void		InitListHead();
	void        InitPlotControl(long nID);
    void		UpdateControlsData();
	void		SetPageData();
	long		SearchTextDataFile();
	void		SetListData(long nPage);
	void		SetCAM(long nCAM);
	
public:
	CMSFlexGrid	m_listSearch;
	CButtonST	m_btnExcelData;
	CButtonST	m_btnCheckGood;
	CButtonST	m_btnCheckBad;
	CButtonST	m_btnPrev;
	CButtonST	m_btnNext;
	CButtonST	m_btnSearch;
	CButtonST   m_btnExport;
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
	CDSSPanel   m_sspanSearch;
	CDSSPanel	m_panPage;
	CComboBox   m_comboModelName;
	CComboBox   m_comboLotID;

	long        m_nShortQuery;
	long		m_nHeadCnt;
	long        m_nBadCnt[GOM_MAX_NUM];
    TCHAR		m_strHeadTitle[GOM_MAX_NUM][100];

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_QUERY };

protected:
	int    m_nImagePathIndex;
	int    m_nPage;
	int    m_nTotalPage;
	long   m_nTotalRecordCount;
	ULONGLONG *m_pPageFilePtr;
	long    m_nItemTotalCount[GOM_MAX_NUM];
	long    m_nItemBadCount[GOM_MAX_NUM];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ClickListSearch();
	void DblclkListSearch();

	afx_msg void OnBtnSearch();
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
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnExport();

	DECLARE_EVENTSINK_MAP()	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
