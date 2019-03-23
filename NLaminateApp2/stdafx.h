
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once


#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdisp.h>        // MFC Automation classes
#include <afxsock.h>            // MFC 소켓 확장
#include <omp.h>
#include <afx.h>
#include <ShellAPI.h>
#include <atlstr.h>
#include <afxtempl.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include "AppDefine.h"

extern LOG_LIST       g_LOG;
extern REALTIME_DATA  g_DataRT;
extern LPBYTE  g_pImage[DEF_QUEUE_MAX_NUM];
extern LPBYTE  g_pBadImage[VIEW_MAX_NUM][DEF_QUEUE_MAX_NUM];
extern BOOL    g_PortCOM[10];

static BOOL CreateDirectoryFullPath(CString strPath)
{
	if (::PathIsDirectory(strPath) != 0)
		return TRUE;
	else
	{
		int i=0;
		while(TRUE)
		{
			i = strPath.Find((TCHAR *)"\\",i+1);
			if(i<=0)
			{
				CreateDirectory(strPath,0);
				break;
			}
			CreateDirectory(strPath.Left(i+1),0);
		}
	}

	if (::PathIsDirectory(strPath)!=0)
		return TRUE;
	else
		return FALSE;

	return TRUE;
};

static void RemoveDir(CString strDir)
{
	::RemoveDirectory(strDir);
#if 0
	CFileFind finder;
	BOOL bWorking = TRUE;
	CString strDirFile = strDir + CString("\\*.*");

	bWorking = finder.FindFile(strDirFile);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots()) continue;
		if(finder.IsDirectory())    RemoveDir(finder.GetFilePath());
		else    ::DeleteFile(finder.GetFilePath());
	}

	finder.Close();  
#endif // 0

};

// CString → Char
static void StringToChar(CString str, char *szbuf)
{
    char *szStr = NULL;

#if defined(UNICODE) || defined(_UNICODE)
    int nLen = str.GetLength() + 1;
    TCHAR *tszTemp = NULL;
    tszTemp = new TCHAR[nLen];

    memset(tszTemp, 0x00, nLen*sizeof(TCHAR));
    wcscpy_s(tszTemp, nLen, str);

    // Get size (실제사용되는바이트사이즈)
    int nSize = WideCharToMultiByte(CP_ACP, 0, tszTemp, -1, NULL, NULL, NULL, NULL);
    szStr = new char[nSize];

    memset(szStr, 0x00, nSize);
    WideCharToMultiByte(CP_ACP, 0, tszTemp, -1, szStr, nSize, NULL, NULL);

	strcpy_s(szbuf, nSize, szStr);

	if(tszTemp) delete [] tszTemp;
	if(szStr)   delete [] szStr;
#else
    int nLen = str.GetLength() + 1;
	strcpy_s(szbuf, nLen, str);
#endif
}

// CString → TCHAR
static TCHAR* StringToTCHAR(CString str)
{
    TCHAR *tszStr = NULL;
    int nLen = str.GetLength() + 1;

    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    _tcscpy_s(tszStr, nLen, str);

    return tszStr;
}

// Char → CString
static CString CharToString(char *str)
{
    CString cStr;

#if defined(UNICODE) || defined(_UNICODE)
    long nLen;
	
	nLen = (long)strlen(str) + 1;
    TCHAR *tszTemp = NULL;

    tszTemp = new TCHAR[nLen];
    memset(tszTemp, 0x00, nLen*sizeof(TCHAR));
    MultiByteToWideChar(CP_ACP, 0, str, -1, tszTemp, nLen*sizeof(TCHAR));
    cStr.Format(_T("%s"), tszTemp);

    if(tszTemp)
    {
        delete [] tszTemp;
        tszTemp = NULL;
    }
#else
    cStr.Format("%s", str);
#endif

    return cStr;
}

// Char → TCHAR
TCHAR* CharToTCHAR(char *str)
{
    TCHAR *tszStr = NULL;

#if defined(UNICODE) || defined(_UNICODE)
    long nLen;
	
	nLen = (long)strlen(str) + 1;
    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    MultiByteToWideChar(CP_ACP, 0, str, -1, tszStr, nLen*sizeof(TCHAR));
#else

    long nLen;
	
	nLen = (long)strlen(str) + 1;
    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    strcpy_s(tszStr,nLen, str);
#endif

    return tszStr;
}

// TCHAR → CString
static CString TCHARToString(TCHAR *str)
{
    CString cStr;

    cStr.Format(_T("%s"), str);
    return cStr;
}

// TCHAR → Char
static char* TCHARToChar(TCHAR *str)
{
    char *szStr = NULL;

#if defined(UNICODE) || defined(_UNICODE)
    int nSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, NULL, NULL, NULL);
    szStr = new char[nSize];
    memset(szStr, 0x00, nSize);
    WideCharToMultiByte(CP_ACP, 0, str, -1, szStr, nSize, NULL, NULL);
#else
    int nLen = strlen(str) + 1;
    szStr = new char[nLen];
    memset(szStr, 0x00, nLen);
    strcpy_s(szStr, nLen, str);
#endif

    return szStr;
}

static CString WithoutSpaceToString(CString strSour)
{
	long i, nStart, nEnd;
	CString str;

	nStart = 0;
	for(i=0; i<strSour.GetLength(); i++)
	{
		if (strSour.Mid(i, 1)!=" ") {
		    nStart = i;
			break;
		}
	}

	nEnd = strSour.GetLength()-1;
	for(i=strSour.GetLength()-1; i>=0; i--)
	{
		if (strSour.Mid(i, 1)!=" ") {
		    nEnd = i;
			break;
		}
	}

	str = strSour.Mid(nStart, nEnd-nStart+1);
	str = str.MakeUpper();

	return str;
}

static void InitGlobalData()
{
	long i, j;

	for(i=0; i<10; i++)
	    g_PortCOM[i] = FALSE;

	g_DataRT.m_nMelHeadPnt = 0;
	g_DataRT.m_nMelTailPnt = 0;

	g_DataRT.m_nImgIndex = 0;
	g_DataRT.m_nImgHeadPnt = 0;
	g_DataRT.m_nImgTailPnt = 0;
	g_DataRT.m_nWidth = IMAGE_WIDTH;
	g_DataRT.m_nHeight = IMAGE_HEIGHT;

	for(i=0; i<DEF_QUEUE_MAX_NUM; i++)
	{
		g_DataRT.m_ImgCam[i] = 0;
		g_DataRT.m_ImgIndexP[i] = 0;
		g_DataRT.m_nBadIndex[i] = 0;

		g_pImage[i] = new unsigned char[IMAGE_WIDTH*IMAGE_HEIGHT];
		for(j=0; j<VIEW_MAX_NUM; j++)
		{
			g_pBadImage[j][i] = new unsigned char[IMAGE_WIDTH*IMAGE_HEIGHT];
			memset(g_pBadImage[j][i], 0, IMAGE_WIDTH*IMAGE_HEIGHT);
		}	

		memset(g_pImage[i], 0, IMAGE_WIDTH*IMAGE_HEIGHT);
	}

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		g_DataRT.m_InspCnt[i] = 0;
		g_DataRT.m_BadCnt[i] = 0;
	    g_DataRT.m_YieldRate[i] = 0;
	}
}

static void CloseGlobalData()
{
	long i, j;

	for(i=0; i<DEF_QUEUE_MAX_NUM; i++)
	{
		delete g_pImage[i];
		for(j=0; j<VIEW_MAX_NUM; j++)
			delete g_pBadImage[j][i];
	}
}

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


