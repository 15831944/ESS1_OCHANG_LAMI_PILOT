#pragma once
#include "resource.h"
#include "afxcmn.h"

typedef struct
{
	TCHAR    m_strTitle[PLOT_ONE_MAX_NUM][100];
	long     m_nIndexF[PLOT_ONE_MAX_NUM];
	long     m_nIndexR[PLOT_ONE_MAX_NUM];
	long     m_nCamID;
	BOOL     m_bLink;


	float    m_fUpperL;
	float    m_fLowerL;
	float    m_fLowSpec;
	float    m_fHighSpec;
	long     m_nNum;
} PLOT_LINK;

// CDlgPlot 대화 상자입니다.

class CDlgPlot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPlot)

public:
	CDlgPlot(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPlot();

	void     InitPlotControls();
	long     GetSelectedTab(long nSel);
	void     SetTransLanguage(long nValue);
	void     EnablePlotWindow(long nID, BOOL bFlag);
	void     EnableFullSizePlot(long nID, bool bFlag);
	void     ResetPlotData();
	void     InitPlotData();
	void     UpdatePlotData(long nPageNo);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PLOT };

public :
	bool      m_bToggleSize;
	long      m_nFullSizeID;
	BOOL      m_nCurSel;

	PLOT_LINK m_PL[PLOT_PAGE_MAX_NUM*PLOT_MAX_NUM];
	NPlotDll_CallDATA    NPlotDll_Data;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CImageList    m_imgListSel;
	CRect         m_rcPlotRect;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CTabCtrl m_tabMulti;
	afx_msg void OnTcnSelchangeMultiTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
