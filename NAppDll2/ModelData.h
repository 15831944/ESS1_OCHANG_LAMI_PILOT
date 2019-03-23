#if !defined(AFX_MODELDATA_H__5585EE7A_1BB9_40F3_ACE2_085AD5714F2B__INCLUDED_)
#define AFX_MODELDATA_H__5585EE7A_1BB9_40F3_ACE2_085AD5714F2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelData window
#include "Modules\Mathematic.h"
#include "Modules\VisionLib.h"

class CModelData 
{
// Construction
public:
	CModelData();

// Attributes
public:
	void    InitModelData();
	void    CloseModelData();
    void    CreateModelData();
	BOOL    LoadModelData(CString strFileName);
	BOOL    SaveModelData(CString strFileName);

	SETUP_MODEL_DATA*    GetModelDataPtr() {return &m_pData;};
	SETUP_MODEL_DATA*    GetModelBackDataPtr() {return &m_pBackData;};

	void	ShowROI(LPCTSTR sTitle, BOOL bShow);
	void    ShowAllROI(BOOL bShow);
	void	ShowTitleROI(LPCTSTR sTitle, BOOL bShow);
    long    AddROI(LPCTSTR strTitle, long nGOMType, long nInspType, long nThick, COLORREF nColor);
	BOOL    CopyFrontToRear();
	void    SetSpecData(INSP_RESULT_DATA *pResData);
	void    SetRealTimeMode(BOOL bRealTime);
	long    GetEmptyROI();
	void    SetMoveROI(long nDir, BOOL bLoc);
	void	DeleteAllROI();
	void    DeleteSelectedROI();
	void    DeleteROI(LPCTSTR strTitle);
    CString GetTitleSelectedROI();
	CString GetSelectedTitle();
	void    SetLimitData(LPCTSTR strTitle, float fLowL, float fHighL);
	void    SetLinkLimitData(LPCTSTR strTitle, float fLowL, float fHighL);
	long    AddCalROI(LPCTSTR strTitle, long x1, long y1, long x2, long y2);
    BOOL    SetTransRectROI(long nSelectedROI, float dAngle);
    long    GetIndexROI(CString strTitle);
	long    GetNextROI();
	void    SetPressKey(long nCtrlKey, long nKey);
	void    AppendCopyROI(long nDest, long nSour);
	long    GetIndexSelectedROI();
    void    GetRegionROI(long nSel, long *x1, long *y1, long *x2, long *y2);
	void    CopyROI(long nSelectedROI);
    void    InitPatternSelectedROI(long nSelectedROI, BOOL bFlag, SETUP_MODEL_DATA *pTarg);
    void    CopySelectedROI(long nSelectedROI, SETUP_MODEL_DATA *pSour, SETUP_MODEL_DATA *pDest);
    void    CopyModelData(SETUP_MODEL_DATA *pSour, SETUP_MODEL_DATA *pDest);
    void    SetSelectedROI(CString strTitle);
	void    SetRepositionROI(float dPX, float dPY, float dDX, float dDY, float dDT, long nImageWidth, long nImageHeight);
    BOOL    GetMidPointLocation(float *dPosX, float *dPosY, float *dPosT);
	long    RegisterPattern(long nIndexROI, LPBYTE pImage, long x1, long y1, long x2, long y2, long nImageWidth, long nImageHeight, long nPatNum, long *nWidth, long *nHeight);
    long    RegisterBLOB(long nIndexROI, LPBYTE pImage, long x1, long y1, long x2, long y2, long nImageWidth, long nImageHeight, long nPol, long nThresh, long *nWidth, long *nHeight);

public:
	SETUP_MODEL_DATA   m_pData;
	SETUP_MODEL_DATA   m_pBackData;
	CMathematic        m_libMath;
	CVisionLib         m_libVis;

private:
	void    InitPosROI(long nIndex, long nType);
	BOOL    GetModifyInsertPos(LPCTSTR strTitle);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelData)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModelData();

	// Generated message map functions
protected:
	long              m_nImageWidth;
	long              m_nImageHeight;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELDATA_H__5585EE7A_1BB9_40F3_ACE2_085AD5714F2B__INCLUDED_)
