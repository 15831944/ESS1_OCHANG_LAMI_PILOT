#if !defined(AFX_OVERLAYTITLEGOM_H__606E1535_577B_425F_925E_D6201D9DAF0D__INCLUDED_)
#define AFX_OVERLAYTITLEGOM_H__606E1535_577B_425F_925E_D6201D9DAF0D__INCLUDED_

#include "ImageBMP.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayTitleGOM.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// COverlayTitleGOM window

class COverlayTitleGOM : public CStatic
{
// Construction
public:
	COverlayTitleGOM();

// Attributes
public:

// Operations
private:
	CImageBMP           m_ImageBMP;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverlayTitleGOM)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COverlayTitleGOM();

	// Generated message map functions
protected:
	//{{AFX_MSG(COverlayTitleGOM)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYTITLEGOM_H__606E1535_577B_425F_925E_D6201D9DAF0D__INCLUDED_)
