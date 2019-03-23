#if !defined(AFX_FUNCTIONETC_H__ECB4594F_5AB3_4918_BFB6_E95528731ABD__INCLUDED_)
#define AFX_FUNCTIONETC_H__ECB4594F_5AB3_4918_BFB6_E95528731ABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FunctionEtc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFunctionEtc window

class CFunctionEtc : public CStatic
{
// Construction
public:
	CFunctionEtc();

// Attributes
public:
    void     GetOffsetAdjustment(SETUP_MODEL_DATA *pData, long nIndex, POINT2 *ptT1, POINT2 *ptT2, POINT2 *ptT3, POINT2 *ptT4);
    void     GetTransPosision(float dSX, float dSY, float dDX, float dDY, float dDT, float dCX, float dCY, POINT2 *ptT);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFunctionEtc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFunctionEtc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFunctionEtc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCTIONETC_H__ECB4594F_5AB3_4918_BFB6_E95528731ABD__INCLUDED_)
