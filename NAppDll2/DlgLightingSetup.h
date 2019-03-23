#pragma once

#include "Controls\RangeSlider.h"
#include "Controls\BtnST.h"
#include "Modules\VisionLib.h"
#include "Controls\EditEx.h"
#include "AxisPlot.h"
#include "ModelData.h"
#include "DlgSliderBarCtrl.h"
#include "resource.h"

// CDlgLightingSetup dialog
class CDlgVDisp;
class CDlgLightingSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgLightingSetup)

public:
	CDlgLightingSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLightingSetup();

	void	 InitControls();
	void     UpdateControlsData();

// Dialog Data
	enum { IDD = IDD_DLG_LIGHT_SETUP };

public :
	CWnd     *m_pWndParent;

	CDlgSliderBarCtrl    *m_pExposureT;
	CDlgSliderBarCtrl    *m_pLightSlide[DEF_LIGHT_NUM];

    SETUP_MODEL_DATA     *m_pData;
	SETUP_MODEL_DATA     *m_pBackData;
	
private :
	CDlgVDisp            *m_pParent;

	CButtonST       m_btnSave;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnLightSave();
};
