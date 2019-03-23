#if !defined(AFX_OVERLAYMULTIRECTGOM_H__CF618E32_D035_4D04_B29E_67CC33B78DF1__INCLUDED_)
#define AFX_OVERLAYMULTIRECTGOM_H__CF618E32_D035_4D04_B29E_67CC33B78DF1__INCLUDED_

#include "ImageBMP.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayMultiRectGOM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COverlayMultiRectGOM window

class COverlayMultiRectGOM : public CStatic
{
// Construction
public:
	COverlayMultiRectGOM();

    BOOL   DrawOverlayRect(CDC *pDC, long nIndex, SETUP_MODEL_DATA *pData);

// Attributes
public:

// Operations
private:
	CImageBMP           m_ImageBMP;
	SETUP_MODEL_DATA   *m_pData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverlayMultiRectGOM)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COverlayMultiRectGOM();

	// Generated message map functions
protected:
	//{{AFX_MSG(COverlayMultiRectGOM)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYMULTIRECTGOM_H__CF618E32_D035_4D04_B29E_67CC33B78DF1__INCLUDED_)
