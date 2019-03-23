/////////////////////////////////////////////////////////////////////////////
// TabCtrlEx.h
/////////////////////////////////////////////////////////////////////////////
#ifndef _TABCTRLEX_CLASS_
#define _TABCTRLEX_CLASS_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
class CTabCtrlEx : public CTabCtrl
{
public:
	CTabCtrlEx();
	virtual ~CTabCtrlEx();

protected:
	CImageList   *m_pImageList;
	CDialog		**m_pPage;

	int			  m_nCurPage;
	int			  m_nPageNum;
	
public:
	void InsertPage(int		   nPageNum,
					CDialog	   *pPage[],
		            UINT	   *uiPageID,
					CImageList *pImageList=NULL);
	void SelectPage();
	void AbortPage ()  { SetCurSel(m_nCurPage); }
	int  GetCurPage()  { return m_nCurPage;     }

public:
	//{{AFX_VIRTUAL(CTabCtrlEx)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CTabCtrlEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}




#endif
/////////////////////////////////////////////////////////////////////////////
