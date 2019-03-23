#pragma once


// CDlgShortQuery dialog

class CDlgShortQuery : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShortQuery)

public:
	CDlgShortQuery(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShortQuery();

// Dialog Data
	enum { IDD = IDD_DIALOG_SHORT_QUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
