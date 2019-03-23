#if !defined(AFX_OVERLAYCIRCLEGOM_H__749F090F_6292_43AF_8410_28022D99FE65__INCLUDED_)
#define AFX_OVERLAYCIRCLEGOM_H__749F090F_6292_43AF_8410_28022D99FE65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayCircleGOM.h : header file
//

#include "ImageBMP.h"
#include "FunctionEtc.h"
#include "Mathematic.h"
/////////////////////////////////////////////////////////////////////////////
// COverlayCircleGOM window

class COverlayCircleGOM : public CStatic
{
// Construction
public:
	COverlayCircleGOM();

// Attributes
public:
    void   DrawOverlayCircle(CDC *pDC, long nIndex, SETUP_MODEL_DATA *pData);
    void   DrawOverlayCircle2(CDC *pDC, long nIndex, SETUP_MODEL_DATA *pData);
    BOOL   SetSelectedCircle(SETUP_MODEL_DATA *pData, long nIndex, POINT2 ptMouse, long *nDist);
    BOOL   SetSelectedCircle2(SETUP_MODEL_DATA *pData, long nIndex, POINT2 ptMouse, long *nDist);
    BOOL   SetMoveCircle(SETUP_MODEL_DATA *pData, long nIndex, POINT2 ptMouse, POINT2 ptDelta, POINT2 *ptTarg, float *dInRadius);
    BOOL   SetMoveCircle2(SETUP_MODEL_DATA *pData, long nIndex, POINT2 ptMouse, POINT2 ptDelta, POINT2 *ptTarg, float *dInRadius, float *dOutRadius);
    long   GetMousePosMinDist(long nNum, POINT2 pt0, POINT2 *pt);
// Operations

private:
	CImageBMP           m_ImageBMP;
	CMathematic         m_libMath;
	CFunctionEtc        m_FuncEtc;
	GOM_ATTR           *m_pGOM;
	SCAN_ATTR          *m_pSCAN;
	INSPECT_DATA       *m_pINSP;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverlayCircleGOM)
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~COverlayCircleGOM();

	// Generated message map functions
protected:
	//{{AFX_MSG(COverlayCircleGOM)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYCIRCLEGOM_H__749F090F_6292_43AF_8410_28022D99FE65__INCLUDED_)
