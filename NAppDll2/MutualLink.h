#if !defined(AFX_MUTUALLINK_H__6D465BF4_7CB0_4D5E_AE7B_DEEDDD5CA21B__INCLUDED_)
#define AFX_MUTUALLINK_H__6D465BF4_7CB0_4D5E_AE7B_DEEDDD5CA21B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MutualLink.h : header file
//

#include "Modules\VisionLib.h"
#include "Modules\Mathematic.h"
#include "Modules\TransformLib.h"
#include "Modules\FunctionEtc.h"

/////////////////////////////////////////////////////////////////////////////
// CMutualLink window

class CMutualLink 
{
// Construction
public:
	CMutualLink();

    void     SetImageRawData(LPBYTE fmImage, long nWidth, long nHeight);
    void     SetModelData(SETUP_MODEL_DATA *pData) {m_pData = pData;};
    void     SetModelBackData(SETUP_MODEL_DATA *pData) {m_pBack = pData;};

// Attributes
public:
	void     InitializeMutualData();
	BOOL     InspectMutualLinkObjects();
	//BOOL     InspectPoint2PointDist(long nIndex);
	BOOL     InspectPoint2LineDist(long nIndex);
	BOOL     InspectLine2LineDist(long nIndex);
	BOOL	 Inspect1STLINE2LineDist(long nIndex);
	BOOL	 Inspect2STLINE2LineDist(long nIndex);
	BOOL	 InspectLine2LineNADist(long nIndex);

	BOOL     GetScaleFactor(LPBYTE m_fmR, double dReal, long *x1, long *y1, long *x2, long *y2, double *dDx, double *dDy);

// Operations
public:
	CMathematic        m_libMath;
	CTransformLib      m_libTrans;
	CFunctionEtc       m_FuncEtc;  
	CVisionLib         m_libVis;

	BOOL               m_bCompens;
	CAL_DATA          *m_pCAL;

// Implementation
public:
	virtual ~CMutualLink();

	// Generated message map functions
protected:
	SETUP_MODEL_DATA  *m_pData;
	SETUP_MODEL_DATA  *m_pBack;
	INSP_RESULT_DATA   m_pResData;

	LPBYTE             m_pRawImage;
	long               m_nImageWidth;
	long               m_nImageHeight;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUTUALLINK_H__6D465BF4_7CB0_4D5E_AE7B_DEEDDD5CA21B__INCLUDED_)
