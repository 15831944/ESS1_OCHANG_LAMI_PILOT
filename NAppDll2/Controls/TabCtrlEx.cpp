/////////////////////////////////////////////////////////////////////////////
// TabCtrlEx.cpp 
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TabCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTabCtrlEx, CTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CTabCtrlEx::CTabCtrlEx()
{
	m_pImageList = NULL;
	m_nCurPage   = 0;
	m_nPageNum   = 0;
	m_pPage		 = NULL;
}

CTabCtrlEx::~CTabCtrlEx()
{
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CTabCtrlEx::InsertPage(int         nPageNum,
						   CDialog    *pPage[] ,
						   UINT       *uiPageID,
						   CImageList *pImageList)
{
	int i=0;
	CString szCaption;
	TC_ITEM tcItem;

	m_nPageNum = nPageNum;
	m_pPage    = pPage;
	
	for(i=0; i<m_nPageNum; i++)
	{
		m_pPage[i]->Create(uiPageID[i], this);
	}
	
	DeleteAllItems();
	m_pImageList = pImageList;
	
	if(m_pImageList!=NULL)
	{
		SetImageList(m_pImageList);
		tcItem.mask = TCIF_TEXT | TCIF_IMAGE;
	}
	else 
		tcItem.mask = TCIF_TEXT; 

	for(i=0; i<m_nPageNum; i++)
	{
		m_pPage[i]->EnableWindow(FALSE);
		m_pPage[i]->ShowWindow(SW_HIDE);
		m_pPage[i]->GetWindowText(szCaption);
		tcItem.pszText = szCaption.GetBuffer(szCaption.GetLength());
		szCaption.ReleaseBuffer(szCaption.GetLength());
		if(m_pImageList!=NULL)
		{
			if(i==m_nCurPage) tcItem.iImage = 1;
			else			  tcItem.iImage = 0;
		}
		InsertItem(i, &tcItem);
	}
	m_pPage[m_nCurPage]->SetWindowPos(NULL, 1, 22 , 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);
	m_pPage[m_nCurPage]->EnableWindow(TRUE);
	m_pPage[m_nCurPage]->ShowWindow(SW_SHOW);
}

void CTabCtrlEx::SelectPage()
{
	CString szCaption;
	TC_ITEM tcItem;
	
	if(m_pImageList!=NULL)
		tcItem.mask = TCIF_TEXT|TCIF_IMAGE;
	else
		tcItem.mask = TCIF_TEXT;
	
	m_pPage[m_nCurPage]->GetWindowText(szCaption);
	tcItem.pszText = szCaption.GetBuffer(szCaption.GetLength());
	szCaption.ReleaseBuffer(szCaption.GetLength());
	
	if(m_pImageList!=NULL) tcItem.iImage = 0;
	SetItem(m_nCurPage, &tcItem);
	m_pPage[m_nCurPage]->EnableWindow(FALSE);
	m_pPage[m_nCurPage]->ShowWindow(SW_HIDE);
	m_nCurPage = GetCurSel();
	m_pPage[m_nCurPage]->GetWindowText(szCaption);
	tcItem.pszText = szCaption.GetBuffer(szCaption.GetLength());
	szCaption.ReleaseBuffer(szCaption.GetLength());
	if(m_pImageList!=NULL) tcItem.iImage = 1;
	SetItem(m_nCurPage, &tcItem);
	m_pPage[m_nCurPage]->SetWindowPos(NULL, 1, 22 , 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);
	m_pPage[m_nCurPage]->EnableWindow(TRUE) ;
	m_pPage[m_nCurPage]->ShowWindow(SW_SHOW) ;
}
