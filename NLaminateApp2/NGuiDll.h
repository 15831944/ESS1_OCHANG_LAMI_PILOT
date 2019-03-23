// NGuiDll.h : main header file for the NGuiDll DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CNGuiDllApp
// See NGuiDll.cpp for the implementation of this class

// List를 초기화 / Open 함수 
// nListID : (0~14 지정가능) 
// pWnd : 출력하고자하는 윈도우 핸들 
extern "C" _declspec(dllexport) void    NUiListDll_Open(long nListID, long pWnd);

// Plot를 종료 / Close 함수 
// nListID : (0~14 지정가능) 
extern "C" _declspec(dllexport) void    NUiListDll_Close(long nListID);

extern "C" _declspec(dllexport)	void    NUiListDll_SetRows(long nListID, long nCnt);
extern "C" _declspec(dllexport) void    NUiListDll_SetCols(long nListID, long nCnt);
extern "C" _declspec(dllexport)	BOOL    NUiListDll_GetCheck(long nListID, long nCol, long nRow);
extern "C" _declspec(dllexport)	void    NUiListDll_SetCheck(long nListID, long nCol, long nRow, BOOL bCheck);
extern "C" _declspec(dllexport)	void    NUiListDll_SetRow(long nListID, long nRowCnt);
extern "C" _declspec(dllexport)	void    NUiListDll_SetColEnableEdit(long nListID, long nCol, BOOL bEnable);
extern "C" _declspec(dllexport)	void    NUiListDll_SetColEnableMenu(long nListID, long nCol, BOOL bEnable);
extern "C" _declspec(dllexport)	void    NUiListDll_SetColEnableCheck(long nListID, long nCol, BOOL bEnable);
extern "C" _declspec(dllexport) void    NUiListDll_SetPrecisionNum(long nID, long nCol, long nNum);
extern "C" _declspec(dllexport)	void    NUiListDll_SetEnableTitle(long nListID, BOOL bEnable);
extern "C" _declspec(dllexport)	void    NUiListDll_SetRowHeight(long nListID, long nHeight);
extern "C" _declspec(dllexport)	void    NUiListDll_SetColWidth(long nListID, long nCol, long nWidth);
extern "C" _declspec(dllexport)	void    NUiListDll_SetDec(long nListID, long nCol, long nRow, BOOL bDec);
extern "C" _declspec(dllexport)	void    NUiListDll_SetCellBackColor(long nListID, long nSlideX, long nSlideY, long nColor);
extern "C" _declspec(dllexport)	void    NUiListDll_SetFontSize(long nListID, long nSize);
extern "C" _declspec(dllexport)	void    NUiListDll_SetBackColor(long nListID, long nColor);
extern "C" _declspec(dllexport)	void    NUiListDll_SetTitleColor(long nListID, long nColor);
extern "C" _declspec(dllexport)	void    NUiListDll_SetGridLineColor(long nListID, long nColor);
extern "C" _declspec(dllexport)	void    NUiListDll_GetText(long nListID, long nSlideX, long nSlideY, TCHAR *lpStr);
extern "C" _declspec(dllexport)	void    NUiListDll_SetText(long nListID, long nSlideX, long nSlideY, LPCTSTR strText);
extern "C" _declspec(dllexport)	void    NUiListDll_SetEnableEdit(long nListID, BOOL bEnable);
extern "C" _declspec(dllexport)	void    NUiListDll_ClearCellAll(long nListID);
extern "C" _declspec(dllexport)	void    NUiListDll_Invalidate(long nListID);
extern "C" _declspec(dllexport)	void    NUiListDll_UpdateEditControl(long nListID, BOOL bFlag);

class CNGuiDllApp : public CWinApp
{
public:
	CNGuiDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
