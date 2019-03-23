#pragma once


// CDlgMessageBox 대화 상자입니다.

class CDlgMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessageBox)

public:
	CDlgMessageBox(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMessageBox();

	long     m_nMessageID;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MESSAGE_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLocCal();
	afx_msg void OnBnClickedBtnCompensCal();
	virtual BOOL OnInitDialog();
};
