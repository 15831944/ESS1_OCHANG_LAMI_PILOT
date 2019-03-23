#if !defined(AFX_OVERLAYLINEGOM_H__6C9E1A1C_80CF_4421_995B_30F004A5509D__INCLUDED_)
#define AFX_OVERLAYLINEGOM_H__6C9E1A1C_80CF_4421_995B_30F004A5509D__INCLUDED_

#include "ImageBMP.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayLineGOM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COverlayLineGOM window

class COverlayLineGOM : public CStatic
{
// Construction
public:
	COverlayLineGOM();


// Attributes
public:

// Operations
private:
	CImageBMP           m_ImageBMP;
	SETUP_MODEL_DATA   *m_pData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverlayLineGOM)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COverlayLineGOM();

	// Generated message map functions
protected:
	//{{AFX_MSG(COverlayLineGOM)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYLINEGOM_H__6C9E1A1C_80CF_4421_995B_30F004A5509D__INCLUDED_)
