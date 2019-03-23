#if !defined(AFX_OVERLAYRESULTGOM_H__2F8A873A_3C82_4DE5_BCF7_5D5F5BADCAEC__INCLUDED_)
#define AFX_OVERLAYRESULTGOM_H__2F8A873A_3C82_4DE5_BCF7_5D5F5BADCAEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayResultGOM.h : header file

#include "ImageBMP.h"
#include "Mathematic.h"
#include "FunctionEtc.h"

/////////////////////////////////////////////////////////////////////////////
// COverlayResultGOM window

class COverlayResultGOM : public CStatic
{
// Construction
public:
	COverlayResultGOM();
    BOOL   DrawOverlayResult(CDC *pDC, SETUP_MODEL_DATA *pData);
    void   GetLinePos(long nIndex, long nType, long nDX, long nDY, CPoint *ptStart, CPoint *ptEnd);

// Operations
private:
	CImageBMP           m_ImageBMP;
	GOM_ATTR           *m_pGOM;
	SCAN_ATTR          *m_pSCAN;
	INSPECT_DATA       *m_pINSP;
	CMathematic         m_Math;
	CFunctionEtc        m_FuncEtc;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverlayResultGOM)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COverlayResultGOM();

	// Generated message map functions
protected:
	//{{AFX_MSG(COverlayResultGOM)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYRESULTGOM_H__2F8A873A_3C82_4DE5_BCF7_5D5F5BADCAEC__INCLUDED_)
