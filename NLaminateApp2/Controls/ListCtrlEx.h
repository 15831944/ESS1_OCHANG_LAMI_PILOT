/////////////////////////////////////////////////////////////////////////////
// ListCtrlEx.h
/////////////////////////////////////////////////////////////////////////////
#ifndef _LISTCTRLEX_CLASS_
#define _LISTCTRLEX_CLASS_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
class CListCtrlEx : public CListCtrl
{
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

public:
	int        m_iTColNum;
	int        m_iSelectItem;
	int        m_iCopyItem;
	CPoint     m_cpMousePos;

public:
	void	   AddData(CString  pStr[] , int nNumber);
	void	   InsertData(CString  pStr[] , int nNumber);
	void       InitialHeader(CString  pStr[], int nNumber, int nSpace);
	void       InitListCtrl(int icolNum, CString *strText, long *lgWidth);
	int        GetSelectItem()               { return m_iSelectItem; }
	void       SetSelectItem(int iSelectItem){ m_iSelectItem = iSelectItem; }

public:
	DWORD DoSingleClick(UINT uiMenuID,
						int  iDisableNum,
						int  *iDisableItem);
public:
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
#endif 
