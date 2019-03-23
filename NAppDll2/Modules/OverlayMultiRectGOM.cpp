// OverlayMultiRectGOM.cpp : implementation file
//

#include "stdafx.h"
#include "OverlayMultiRectGOM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayMultiRectGOM

COverlayMultiRectGOM::COverlayMultiRectGOM()
{
}

COverlayMultiRectGOM::~COverlayMultiRectGOM()
{
}


BEGIN_MESSAGE_MAP(COverlayMultiRectGOM, CStatic)
	//{{AFX_MSG_MAP(COverlayMultiRectGOM)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayMultiRectGOM message handlers
BOOL COverlayMultiRectGOM::DrawOverlayRect(CDC *pDC, long nIndex, SETUP_MODEL_DATA *pData)
{

	return TRUE;
}