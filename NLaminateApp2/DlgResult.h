#pragma once

// CDlgResult 대화 상자입니다.
class CDlgResult : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResult)

public:
	CDlgResult(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgResult();

	void     InitListControls();
	void     SetTransLanguage(long nValue);
	void     UpdateResultList();
	void     InitPlotControls();
	void     ResetProductData();
	void     WriteProductData(long nCAM, BOOL bLink, long nOrderNum, long nROI);


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RESULT };

	long           m_nRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
