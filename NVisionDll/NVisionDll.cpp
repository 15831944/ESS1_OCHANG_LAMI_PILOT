// NVisionDll.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "NVisionDll.h"
#include "Module\\NMorphologyLib.h"
#include "Module\\NFunctionLib.h"
#include "Module\\NVisionLib.h"
#include "Module\\NMatroxLib.h"
#include "Module\\NTransformLib.h"
#include "Module\\NCalLib.h"
#include "Module\\ClockTimer.h"

#if _DEBUG
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
//

// CNVisionDllApp

BEGIN_MESSAGE_MAP(CNVisionDllApp, CWinApp)
END_MESSAGE_MAP()


// CNVisionDllApp 생성

CNVisionDllApp::CNVisionDllApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CNVisionDllApp 개체입니다.

CNVisionDllApp theApp;

CClockTimer       m_lTimer;

#if _MIL_SETUP
CNMatroxLib       m_lMil;
#endif

CNVisionLib       m_lVis;
CNMorphologyLib   m_lMorpho;
CNFunctionLib     m_lFunc;
CNTransformLib    m_lTrans;
CNCalLib          m_lCAL[10];

// CNVisionDllApp 초기화

BOOL CNVisionDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

_declspec(dllexport) BOOL NVisionDll_Open(long nType, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;

//	if (IsLicenseKey()) return TRUE;
#if _MIL_SETUP
	bRet = m_lMil.Open(nType, nWidth, nHeight);
#endif

    m_lVis.Open(nType, nWidth, nHeight);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_Close()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;

#if _MIL_SETUP
	bRet = m_lMil.Close();
#endif

	m_lVis.Close();
	return bRet;
}

_declspec(dllexport) void NVisionDll_SetCameraFile(LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#if _MIL_SETUP
	strcpy_s(m_lMil.m_strCameraFile, strFileName);
#endif	
}

_declspec(dllexport) BOOL NVisionDll_SetSelectChannel(long nCh)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=FALSE;

#if _MIL_SETUP
	bRet = m_lMil.SetSelectChannel(nCh);
#endif	

	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_Grab(LPBYTE fmImage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=FALSE;

#if _MIL_SETUP
	bRet = m_lMil.Grab(fmImage);
#endif	

	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_GrabCH(long nCh, LPBYTE fmImage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=FALSE;

#if _MIL_SETUP
	bRet = m_lMil.Grab(nCh, fmImage);
#endif

	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_LoadImageFromFile(char *strPathName, unsigned char *fmImage, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=TRUE;

#if _MIL_SETUP
	m_lMil.LoadImageFromFile(strPathName, fmImage, nWidth, nHeight);
#else
	m_lVis.read_BMPJPEG_file(strPathName, fmImage, nWidth, nHeight);
#endif

	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_SaveImageToFile(char *strPathName, LPBYTE fmImage, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool isSaveSuccessful = true;
	
#if _MIL_SETUP
	m_lMil.SaveImageToFile(strPathName, fmImage, nWidth, nHeight);
#else
	CString str;
	str.Format("%s", strPathName);
	str = str.Right(4);
	str = str.MakeUpper();

	if (str == ".JPG")	
		m_lVis.write_JPEG_file(strPathName, fmImage, nWidth, nHeight, 80);
	else
		m_lVis.write_BMP_file(strPathName, fmImage, nWidth, nHeight);
#endif

	return (!isSaveSuccessful);
}

_declspec(dllexport) void NVisionDll_ResetTimer(long nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#if _MIL_SETUP
	m_lMil.ResetTimer(nID);
#else
	m_lTimer.SetClockTimer(nID);
#endif
}

_declspec(dllexport) double NVisionDll_GetTimer(long nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	double dTime;

#if _MIL_SETUP
	dTime = m_lMil.GetTimer(nID);
#else
	dTime = m_lTimer.GetClockTimer(nID);
#endif
 
	return dTime;
}

_declspec(dllexport) BOOL NVisionDll_SmoothProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

#if _MIL_SETUP
	bRet = m_lMil.SmoothProc(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
#else
	bRet = m_lVis.SmoothProc(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
#endif
	
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_OpenGrayProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lMorpho.OpenGray(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_CloseGrayProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lMorpho.CloseGray(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_DilateGrayProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lMorpho.DilateGray(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_ErodeGrayProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lMorpho.ErodeGray(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_OpenBinaryProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lMorpho.OpenBinary(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_CloseBinaryProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lMorpho.CloseBinary(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_DilateBinaryProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lMorpho.DilateBinary(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_ErodeBinaryProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lMorpho.ErodeBinary(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nIter);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_BinarizeProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

	bRet = m_lVis.BinarizeProc(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, nThresh);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_SobelEdgeProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

#if _MIL_SETUP
	bRet = m_lMil.SobelEdgeProc(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight);
#else
	bRet = m_lVis.SobelEdgeProc(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight);
#endif

	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_ImageRotation(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float dAngle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet;

//#if _MIL_SETUP
//	bRet = theMil.ImageRotation(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, dAngle);
//#else
	bRet = m_lVis.ImageRotation(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight, dAngle);
//#endif

	return bRet;
}

_declspec(dllexport) void NVisionDll_GetProjectFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_lVis.GetProjectFtX(pImage, x1, y1, x2, y2, nWidth, nHeight, ProjX);
}

_declspec(dllexport) void NVisionDll_GetProjectFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_lVis.GetProjectFtY(pImage, x1, y1, x2, y2, nWidth, nHeight, ProjY);
}

_declspec(dllexport) void NVisionDll_GetProjectDfDtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverDist, long *ProjX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_lVis.GetProjectDfDtX(pImage, x1, y1, x2, y2, nWidth, nHeight, nNaverDist, ProjX);
}

_declspec(dllexport) void NVisionDll_GetProjectDfDtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverDist, long *ProjY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_lVis.GetProjectDfDtY(pImage, x1, y1, x2, y2, nWidth, nHeight, nNaverDist, ProjY);
}

_declspec(dllexport) float NVisionDll_FindAngleProj(LPBYTE pSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float dSAngle, float dEAngle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	float fAngle;

	fAngle = m_lFunc.FindAngleProj(pSourImage, x1, y1, x2, y2, nWidth, nHeight, dSAngle, dEAngle);
	return fAngle;
}

_declspec(dllexport) float NVisionDll_GetSharpnessValue(LPBYTE pSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	float fValue;

	fValue = m_lVis.GetSharpnessValue(pSourImage, x1, y1, x2, y2, nWidth, nHeight);
	return fValue;
}

_declspec(dllexport) BOOL NVisionDll_CodeRead(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nType, long nNum, char *strCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet = TRUE;
#if _MIL_SETUP
	bRet = m_lMil.BarCodeRead(pImage, x1, y1, x2, y2, nWidth, nHeight, nType, nNum, strCode);
#endif
	return bRet;
}

_declspec(dllexport) void NVisionDll_ZoomingImage(LPBYTE pSour, LPBYTE pDest, float fDispX, float fDispY, long nIWidth, long nIHeight, long nFWidth, long nFHeight, float fScale)
{
	m_lVis.ZoomingImage(pSour, pDest, fDispX, fDispY, nIWidth, nIHeight, nFWidth, nFHeight, fScale);
}

_declspec(dllexport) LPBYTE NVisionDll_OpenTransTiltToRect(POINT2 pt1, POINT2 pt2, POINT2 pt3, POINT2 pt4, long *nWidth, long *nHeight)
{
	return m_lTrans.OpenTransTiltToRect(pt1, pt2, pt3, pt4, nWidth, nHeight);
}

_declspec(dllexport) void NVisionDll_ExecTransTiltToRect(LPBYTE pSour, POINT2 pt1, POINT2 pt2, POINT2 pt3, POINT2 pt4, long nImageWidth, long nImageHeight)
{
	m_lTrans.ExecTransTiltToRect(pSour, pt1, pt2, pt3, pt4, nImageWidth, nImageHeight);
}

_declspec(dllexport) void NVisionDll_CloseTransTiltToRect()
{
	m_lTrans.CloseTransTiltToRect();
}

_declspec(dllexport) void NVisionDll_LoadCalData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_lCAL[nCamID].LoadCalData(strFileName);
}

_declspec(dllexport) void NVisionDll_SaveCalData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_lCAL[nCamID].SaveCalData(strFileName);
}

_declspec(dllexport) BOOL NVisionDll_SetWarping(long nCamID, LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh, long nPol, long nRow, long nCol, float dGridSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=FALSE;

	bRet = m_lCAL[nCamID].SetWarping(pImage, x1, y1, x2, y2, nWidth, nHeight, nThresh, nPol, nRow, nCol, dGridSize);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_SetWarping2(long nCamID, long nBlobCnt, double *fCenX, double *fCenY, long nWidth, long nHeight, long nRow, long nCol, float dGridSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=FALSE;

    bRet = m_lCAL[nCamID].SetWarping(nBlobCnt, fCenX, fCenY, nWidth, nHeight, nRow, nCol, dGridSize);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_TransWarpingImage(long nCamID, LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=FALSE;

	bRet = m_lCAL[nCamID].TransWarpingImage(pSourImage, pDestImage, x1, y1, x2, y2, nWidth, nHeight);
	return bRet;
}

_declspec(dllexport) BOOL NVisionDll_TransWarpingPos(long nCamID, float i, float j, float *x, float *y, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=FALSE;

	bRet = m_lCAL[nCamID].TransWarpingPos(i, j, x, y, nWidth, nHeight);
	return bRet;
}

_declspec(dllexport) float NVisionDll_GetScaleFactor(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (float)m_lCAL[nCamID].m_fScaleFactor;
}

BOOL GetLicenseKey()
{
	BOOL bRet=TRUE;
	SYSTEMTIME	   m_sysNewTime;

	GetLocalTime(&m_sysNewTime);
	if (m_sysNewTime.wMonth==1 || m_sysNewTime.wMonth==2 || m_sysNewTime.wMonth==3)
		bRet = FALSE;
	else
		MessageBox(NULL, "  Error : License Key !  ", " Message Box ", MB_ICONEXCLAMATION|MB_OK);

	return bRet;
}
