#pragma once
#include "afxcmn.h"
#include "Controls\\ListCtrlEx.h"
#include "Controls\\mscomm.h"
#include "isevensegmen_ok_cou.h"
#include "ipiechartx_short_ng_position.h"
#include "ilabelx12.h"

#define  DEF_SHORTLIST_MAX_NUM      37

enum { 
	   SHORT_PASS=0,
	   CHARGE_FAIL,
	   CONTACT_FAIL,
	   HIGH_OVERLOAD,
	   LOW_OVERLOAD,
	   PD_FAIL, 
	   ETC_ERROR=11	
	 };

#define CHARGE_COLOR  RGB(255,0,0)
#define CONTACT_COLOR RGB(152,0,0)
#define HIGH_COLOR	  RGB(210,0,210)
#define LOW_COLOR	  RGB(166,166,166)
#define PO_COLOR	  RGB(218,165,032)
#define ETC_COLOR	  RGB(112,128,144)

// CDlgShortMain dialog
class CDlgShortMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShortMain)

public:
	CDlgShortMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShortMain();

	void     InitControls();
	void   	 InitListControls();
	void     UpdateListControls(long bDecF, long nInxF, float fF, long bDecR, long nInxR, float fR);
	void     InitIndexShort();
	void     SetShortTestCondition();
	void     GetStringFromText(long nPort, CString strText);
	long     GetIndexRearSide(long nIndexF);
	void     GetRS232CData();
	void     InitShortProc();
	void     CloseShortProc();
	void     SetEventCOM();
	void     WriteShortVToFile(BOOL bRear, long bDec, float fV);
	BOOL     GetTesterVersion(long nComPort);
	CString  ReadComPort(long nComPort, long *nLen);
	UINT static  ThreadShortCOM(LPVOID pParam);

public :
    NPlotDll_CallDATA    NPlotDll_Data;

	long     m_nRearIndex[2];
	long     m_nFirstIndex[2];
	char     m_cLastSeq[2];
	long     m_nGoodDec[2][SHORT_BUFFER_MAX_NUM];
	long     m_nNumID[2][SHORT_BUFFER_MAX_NUM];
	long     m_nVoltV[2][SHORT_BUFFER_MAX_NUM];
	long     m_nCurrV[2][SHORT_BUFFER_MAX_NUM];
	CString  m_strRemainT[2];
	

	long     m_nListIndexPoint;
	long     m_IndexData[DEF_SHORTLIST_MAX_NUM];
	float    m_FrontData[DEF_SHORTLIST_MAX_NUM];
	float    m_RearData[DEF_SHORTLIST_MAX_NUM];
	long     m_FrontDec[DEF_SHORTLIST_MAX_NUM];
	long     m_RearDec[DEF_SHORTLIST_MAX_NUM];
	BOOL     m_bOnRS232C;

	long	nTotalCou;
	long	nOk_Ratio;

	long	nTotalOkCou;
	long	nTotalNgCou;

	long	nChargeCou;
	long	nContactCou;
	long	nHighCou;
	long	nLowCou;
	long	nPoCou;
	long    nEtcCou;

	long	nTotalNgCou1;
	long	nChargeCou1;
	long	nContactCou1;
	long	nHighCou1;
	long	nLowCou1;
	long	nPoCou1;
	long    nEtcCou1;

	SYSTEMTIME	   m_sysShortOldTime, m_sysNewTime;

	HANDLE         m_hEventCOM;
	CWinThread    *m_pThreadCOM;

	CMSComm m_ctrlCOM[2];

	enum { IDD = IDD_DIALOG_SHORT_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx  m_listTestShort;
//	afx_msg LONG OnReadRS232CData(UINT, LONG);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	CIsevensegmen_ok_cou cTotalOkCou;
	CIsevensegmen_ok_cou cTotalNgCou;
	CIsevensegmen_ok_cou cChargeCou;
	CIsevensegmen_ok_cou cContactCou;
	CIsevensegmen_ok_cou cHighCou;
	CIsevensegmen_ok_cou cLowCou;
	CIsevensegmen_ok_cou cPoCou;
	CIsevensegmen_ok_cou cEtcCou;

	CIsevensegmen_ok_cou cTotalCou;
	CIpiechartx_short_ng_position cShort_Ng_PChart_Com;
	CIlabelx12 cGoodRatio;

	CIsevensegmen_ok_cou cRearNgCou;
	CIsevensegmen_ok_cou cFrontNgCou;
	CIsevensegmen_ok_cou cChargeNgCou;
	CIsevensegmen_ok_cou cContactNgCou;
	CIsevensegmen_ok_cou cHighNgCou;
	CIsevensegmen_ok_cou cLowNgCou;
	CIsevensegmen_ok_cou cPdNgCou;
	CIsevensegmen_ok_cou cEtcNgCou;
	afx_msg void OnBnClickedButton6();
};
