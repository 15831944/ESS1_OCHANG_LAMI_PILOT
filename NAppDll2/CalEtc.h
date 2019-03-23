#if !defined(AFX_CALETC_H__36719670_03E2_4D8F_A2F0_DB2B4FE4A0BD__INCLUDED_)
#define AFX_CALETC_H__36719670_03E2_4D8F_A2F0_DB2B4FE4A0BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalEtc.h : header file
//

#define DEF_N            10

typedef struct 
{
	long    m_nRow;
	long    m_nCol;
	double *m_pMat;
} DEF_MAT;

/////////////////////////////////////////////////////////////////////////////
// CCalEtc window

class CCalEtc
{
// Construction
public:
	CCalEtc();

	void        LoadCalData(CString strFileName);
    void        SaveCalData(CString strFileName);

	BOOL        DotProductMat(DEF_MAT *a, DEF_MAT *b, DEF_MAT *c);
    BOOL        TransposeMat(DEF_MAT *a, DEF_MAT *b);
    BOOL        InverseMat(DEF_MAT *a, DEF_MAT *b);
	void        DeleteMat(DEF_MAT *pMat);
	DEF_MAT*    CreateMat(long nRow, long nCol);

	float       GetScaleFactor() {return m_fScaleFactor;}
	BOOL        SetWarping(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nRow, long nCol, float fGridSize);
	BOOL        TransWarpingImage(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
    BOOL        TransWarpingPos(float i, float j, float *x, float *y, long nWidth, long nHeight);

// Attributes
public:
	DEF_MAT    *A_MAT, *B_MAT, *C_MAT;
	DEF_MAT    *D_MAT, *F_MAT;
	DEF_MAT    *T_MAT, *I_MAT;

	long		m_nGridColor;
	long		m_nMinArea;
	long		m_nMaxArea;

	float		m_fScaleFactor;
	double		m_fInverseA[DEF_N];
	double		m_fInverseB[DEF_N];
	double		m_fForwardA[DEF_N];
	double		m_fForwardB[DEF_N];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalEtc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCalEtc();

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALETC_H__36719670_03E2_4D8F_A2F0_DB2B4FE4A0BD__INCLUDED_)
