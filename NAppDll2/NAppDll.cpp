// NAppDll.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//
#include "stdafx.h"
#include "NAppDll.h"
#include "NVisionDll.h"
#include "NLicenseKeyDll.h"
#include "DlgVDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며 
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
// CNAppDllApp

BEGIN_MESSAGE_MAP(CNAppDllApp, CWinApp)
END_MESSAGE_MAP()

// CNAppDllApp 생성
CNAppDllApp::CNAppDllApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

CNAppDllApp theApp;
CDlgVDisp   *m_pDlgVDisp[10];
BOOL         m_bIsLicenseKey;

// CNAppDllApp 초기화
BOOL CNAppDllApp::InitInstance()
{
	CWinApp::InitInstance();

	long i;
	for(i=0; i<10; i++)
	    m_pDlgVDisp[i] = NULL;

	m_bIsLicenseKey = FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////    DLL 초기화 및 종료 메서드   ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_Open(long pWnd, long nCamID, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd *pWndPtr = (CWnd *)pWnd;

	if (m_pDlgVDisp[nCamID]) delete m_pDlgVDisp[nCamID];
	m_pDlgVDisp[nCamID] = new CDlgVDisp(pWndPtr);
	m_pDlgVDisp[nCamID]->Create(IDD_DLG_VISION_DISP, pWndPtr);
	m_pDlgVDisp[nCamID]->SetCameraNum(nCamID);
	m_pDlgVDisp[nCamID]->SetImageSize(nWidth, nHeight);
	m_pDlgVDisp[nCamID]->ShowWindow(SW_SHOW);
}

_declspec(dllexport) void NAppDll_Close(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID]) delete m_pDlgVDisp[nCamID];
	m_pDlgVDisp[nCamID] = NULL;
}

_declspec(dllexport) void NAppDll_SetEvent(long nCAM, LPVOID lpCallBack, long pCallData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCAM])
	{
		m_pDlgVDisp[nCAM]->SetCallBackEvent(pCallData);
    	m_pDlgVDisp[nCAM]->m_lpCallBack = (void (__cdecl *)())lpCallBack;
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////    DLL 초기화 및 종료 메서드   ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////    모델 데이타 화일 관련 메서드   //////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_CreateModelData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		m_pDlgVDisp[nCamID]->CreateModelData(strFileName);
}

_declspec(dllexport) long NAppDll_GetModelData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		return (long)m_pDlgVDisp[nCamID]->m_pData;

	return FALSE;
}

_declspec(dllexport) BOOL NAppDll_LoadModelData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet=TRUE;

	if (m_pDlgVDisp[nCamID])
	{
		bRet = m_pDlgVDisp[nCamID]->LoadModelData(strFileName);
		m_pDlgVDisp[nCamID]->LoadOffsetFile(strFileName);
	}

	return bRet;
}

_declspec(dllexport) BOOL NAppDll_SaveModelData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pModelData->CopyModelData(m_pDlgVDisp[nCamID]->m_pBackData,  m_pDlgVDisp[nCamID]->m_pData);
		m_pDlgVDisp[nCamID]->m_pModelData->SaveModelData(strFileName);
		m_pDlgVDisp[nCamID]->m_strFullPathName = strFileName;
	}

	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////    모델 데이타 화일 관련 메서드   //////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////    영상 데이타 입출력 메서드    ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_SetImageSize(long nCamID, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->SetImageSize(nWidth, nHeight);
}

_declspec(dllexport) void NAppDll_GetImageSize(long nCamID, long *nWidth, long *nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->m_pImageModule->GetImageSize(nWidth, nHeight);
}

_declspec(dllexport) void NAppDll_SetFrameSize(long nCamID, long nWidth, long nHeight, long nBand, long nDepth)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->SetFrameSize(nWidth, nHeight, nBand, nDepth);
}

_declspec(dllexport) long NAppDll_GetImagePtr(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
    	return m_pDlgVDisp[nCamID]->GetRawImagePtr();

	return 0;
}

_declspec(dllexport) void NAppDll_SetImagePtr(long nCamID, long pImage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
    	m_pDlgVDisp[nCamID]->SetRawImagePtr(pImage);
}

_declspec(dllexport) void NAppDll_LoadImageData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->LoadImageData(strFileName);
}

_declspec(dllexport) void NAppDll_SaveImageData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		m_pDlgVDisp[nCamID]->m_pImageModule->SaveImageFile(strFileName);
}

_declspec(dllexport) void NAppDll_AsSaveImageData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		m_pDlgVDisp[nCamID]->m_pImageModule->SaveAsImageFile(strFileName);
}

_declspec(dllexport) void NAppDll_SetFileName(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		m_pDlgVDisp[nCamID]->m_strFullPathName = strFileName;
}

_declspec(dllexport) void NAppDll_SetEtcFilePath(long nCamID, LPCTSTR strFilePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	{
		CreateDirectoryFullPath(strFilePath);
    	m_pDlgVDisp[nCamID]->m_pModuleLink->SetFilePath(strFilePath);
	}
}
/////////////////////////////////////////////////////////////////////////////
////////////////////    영상 데이타 입출력 메서드    ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////     실시간 검사 관련 메서드     ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_InitResultData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->m_pModuleLink->InitializeInspData();
}

_declspec(dllexport) void NAppDll_RealTimeMode(long nCamID, BOOL bRealTime)
{
    if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pModelData->SetRealTimeMode(bRealTime);
	}
}

_declspec(dllexport) long NAppDll_ExecuteInspection(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long nRet;

    if (m_pDlgVDisp[nCamID])
	{
	    m_pDlgVDisp[nCamID]->m_pModuleLink->ExecuteInspection();
        nRet = m_pDlgVDisp[nCamID]->m_pModuleLink->GetResultData();
		m_pDlgVDisp[nCamID]->InvalidateView();
	}

	return nRet;
}

_declspec(dllexport) long NAppDll_GetResultData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long nRet;

    if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pModuleLink->ProcessResultDecision();
	    nRet = m_pDlgVDisp[nCamID]->m_pModuleLink->GetResultData();
	}

	return nRet;
}

_declspec(dllexport) void NAppDll_SetSpecData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	INSP_RESULT_DATA *pResData;

    if (m_pDlgVDisp[nCamID])
	{
		pResData = (INSP_RESULT_DATA *)m_pDlgVDisp[nCamID]->m_pModuleLink->GetResultData();

		pResData->m_nCamID = nCamID;
		m_pDlgVDisp[nCamID]->m_pModelData->SetSpecData(pResData);
		m_pDlgVDisp[nCamID]->m_pModelData->SaveModelData(m_pDlgVDisp[nCamID]->m_strFullPathName);
	}
}

_declspec(dllexport) void NAppDll_SetTransLanguage(long nCamID, long nValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->m_nLngSel = nValue;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////     실시간 검사 관련 메서드     ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////       Remote 조명 제어 함수     ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_SetRemoteLight(long nCamID, BOOL bLight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->SetRemoteLight(bLight);
}

_declspec(dllexport) long NAppDll_GetBrightnessValue(long nCamID, long nCh)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long nRet;

	if (m_pDlgVDisp[nCamID])
	    nRet = m_pDlgVDisp[nCamID]->GetBrightnessValue(nCh);

	return nRet;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////       Remote 조명 제어 함수     ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////      화면 변경 및 영상개선      ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_SetZoomScale(long nCamID, float fScale)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pImageModule->SetZoomScale(fScale);
//		m_pDlgVDisp[nCamID]->UpdateScrollPos();
		m_pDlgVDisp[nCamID]->InvalidateView();
	}
}

_declspec(dllexport) float NAppDll_GetZoomScale(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	float nRet = 0.0;
	if (m_pDlgVDisp[nCamID])
	{
	  
//		m_pDlgVDisp[nCamID]->UpdateScrollPos();
//		m_pDlgVDisp[nCamID]->InvalidateView();
		nRet =  m_pDlgVDisp[nCamID]->m_pImageModule->GetZoomScale();
	}

	return nRet;
}

_declspec(dllexport) void NAppDll_InvalidateView(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->InvalidateView();
}

_declspec(dllexport) void NAppDll_ActiveSetupDialog(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#if 0
	//	if (!IsLicenseKey(_T("HASP-BASIC")))
	{
		//		MessageBox(NULL, _T(" Error in license key driver !  Please insert the License Key(USB). "),  _T(" Error Message "), MB_ICONEXCLAMATION&MB_ICONWARNING);
		//		return;
	}  
#endif // 0 // shsong


	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->ShowSetupDialog();
}

_declspec(dllexport) void NAppDll_SetPressKey(long nCamID, long nCtrlKey, long nKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID] && m_pDlgVDisp[nCamID]->m_bModifySetup)
	{
		m_pDlgVDisp[nCamID]->m_pModelData->SetPressKey(nCtrlKey, nKey);
		m_pDlgVDisp[nCamID]->InvalidateView();
	}
}
/////////////////////////////////////////////////////////////////////////////
////////////////////      화면 변경 및 영상개선      ////////////////////////
/////////////////////////////////////////////////////////////////////////////




