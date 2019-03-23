/////////////////////////////////////////////////////////////////////////////
// ListCtrlEx.cpp
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CListCtrlEx::CListCtrlEx()
{
	m_iTColNum = 0;
	m_iSelectItem = -1;
	m_iCopyItem = -1;
	m_cpMousePos  = CPoint(0,0);
}

CListCtrlEx::~CListCtrlEx()
{
}

/////////////////////////////////////////////////////////////////////////////
// AddData - 데이타를 추가한다
// Returns - 없음
// pStr - 추가시킬 문자열
// nNumber - 추가시킬 갯수
void CListCtrlEx::AddData(CString pStr[] , int nNumber)
{
	LV_ITEM	lvi;
	int nActItem ;

	lvi.mask = LVIF_TEXT;		
	lvi.iSubItem = 0;
	lvi.iItem = GetItemCount() + 1 ;
	lvi.pszText = pStr[0].GetBuffer(pStr[0].GetLength()) ;
	pStr[0].ReleaseBuffer(pStr[0].GetLength());
	nActItem = InsertItem(&lvi);

	for(int i = 1; i < nNumber; i++)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = i;
		lvi.iItem = nActItem;
		lvi.pszText = pStr[i].GetBuffer(pStr[i].GetLength()) ;
		pStr[i].ReleaseBuffer(pStr[i].GetLength());
		SetItem(&lvi);
	}
}

/////////////////////////////////////////////////////////////////////////////
// InsertData - 데이타를 삽입한다
// Returns - 없음
// pStr - 추가시킬 문자열
// nNumber - 추가시킬 갯수
void CListCtrlEx::InsertData(CString  pStr[] , int nNumber)
{
	CString str;
	LV_ITEM		lvi;
	int nActItem ;

	lvi.mask = LVIF_TEXT;		
	lvi.iSubItem = 0;
	lvi.iItem = 0 ;
	lvi.pszText = pStr[0].GetBuffer(pStr[0].GetLength());
	pStr[0].ReleaseBuffer(pStr[0].GetLength());

	nActItem = InsertItem(&lvi);

	for(int i = 1; i < nNumber; i++)
	{
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = i;
		lvi.iItem = nActItem;
		str.Format(_T("%s"), pStr[i]);
		lvi.pszText = pStr[i].GetBuffer(pStr[i].GetLength());
		pStr[i].ReleaseBuffer(pStr[i].GetLength());
		SetItem(&lvi);
	}
}


/////////////////////////////////////////////////////////////////////////////
// InitialHeader - 헤더를 초기화 한다
// Returns - 없음
// pStr - 추가시킬 문자열
// nNumber - 추가시킬 갯수
// nSpace - 헤더의 간격
void CListCtrlEx::InitialHeader(CString  pStr[], int nNumber, int nSpace)
{
	DeleteAllItems( );

	LV_COLUMN lcm;
	LPTSTR pszText ;

	for(int i = 0; i < nNumber; i++)
	{
		pszText = pStr[i].GetBuffer(pStr[i].GetLength());
		pStr[i].ReleaseBuffer(pStr[i].GetLength());

		lcm.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lcm.fmt	= LVCFMT_CENTER;
		lcm.cx	= (int)(GetStringWidth(pszText) * 1.5) + nSpace ;
		lcm.iSubItem = i;
		lcm.pszText = pszText ;
		InsertColumn( i, &lcm );
	}
}
void  CListCtrlEx::InitListCtrl(int        iColNum,
								CString    *strText,
								long       *lgWidth)
{ 
	LV_COLUMN	lvcolumn;
	m_iTColNum = iColNum;
//	SetImageList(pImageList, LVSIL_SMALL);
//	SetExtendedStyle(dwExtendedStyle | LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	
	for(int i = 0; i < iColNum; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcolumn.fmt	 = LVCFMT_CENTER;
		lvcolumn.pszText = strText[i].GetBuffer(500);
		lvcolumn.iSubItem = i;
		lvcolumn.cx	= lgWidth[i];
		//if (dwExtendedStyle != LVS_NOCOLUMNHEADER)
		InsertColumn(i, &lvcolumn );
	}
}
DWORD CListCtrlEx::DoSingleClick(UINT uiMenuID,
								int  iDisableNum,
								int  *iDisableItem)
{
	int   i=0;
    CMenu menu;
    int   iMenuID = 0;
    DWORD dwSelectionMade;
    VERIFY(menu.LoadMenu(uiMenuID));
    CMenu *pmenuPopup = menu.GetSubMenu(iMenuID);
    ASSERT(pmenuPopup!=NULL);

	for(i=0; i<iDisableNum; i++)
		pmenuPopup->EnableMenuItem(pmenuPopup->GetMenuItemID(iDisableItem[i]),
		                           MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
    dwSelectionMade = 
		pmenuPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON|
		                           TPM_NONOTIFY|TPM_RETURNCMD,
								   m_cpMousePos.x,
								   m_cpMousePos.y,
								   this);
    pmenuPopup->DestroyMenu();

    return dwSelectionMade;
}

void CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CRect rtWnd;
	GetWindowRect(rtWnd);
	m_cpMousePos = rtWnd.TopLeft()+point;
	CListCtrl::OnRButtonDown(nFlags, point);
}


