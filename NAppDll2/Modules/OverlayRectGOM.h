#if !defined(AFX_OVERLAYRECTGOM_H__4F88B0B6_827B_4C38_94F2_6553DF9B972B__INCLUDED_)
#define AFX_OVERLAYRECTGOM_H__4F88B0B6_827B_4C38_94F2_6553DF9B972B__INCLUDED_

#include "ImageBMP.h"
#include "Mathematic.h"
#include "FunctionEtc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayRectGOM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COverlayRectGOM window
class COverlayRectGOM : public CStatic
{
// Construction
public:
	COverlayRectGOM();

    BOOL   DrawOverlayRect(CDC *pDC, long nIndex, SETUP_MODEL_DATA *pData);
	BOOL   SetSelectedRect(SETUP_MODEL_DATA *pData, long nIndex, POINT2 rtMouse, long *nDist);
    BOOL   SetMoveRect(SETUP_MODEL_DATA *pData, long nIndex, POINT2 rtMouse, POINT2 ptDelta, 
								  POINT2 *ptTarg1, POINT2 *ptTarg2, POINT2 *ptTarg3, POINT2 *ptTarg4);
    long   GetMousePosMinDist(long nNum, POINT2 pt0, POINT2 *pt);
	void   SetImageSize(long nWidth, long nHeight);

// Attributes
public:

// Operations
private:
	CMathematic         m_libMath;
	CImageBMP           m_ImageBMP;
	CFunctionEtc        m_FuncEtc;

	GOM_ATTR           *m_pGOM;
	SCAN_ATTR          *m_pSCAN;
	INSPECT_DATA       *m_pINSP;

	long                m_nImageWidth;
	long                m_nImageHeight;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverlayRectGOM)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COverlayRectGOM();

	// Generated message map functions
protected:
	//{{AFX_MSG(COverlayRectGOM)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYRECTGOM_H__4F88B0B6_827B_4C38_94F2_6553DF9B972B__INCLUDED_)
