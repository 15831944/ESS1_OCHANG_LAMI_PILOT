
// NLaminateApp.h : NLaminateApp ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "DlgSplash.h"


// CNLaminateAppApp:
// �� Ŭ������ ������ ���ؼ��� NLaminateApp.cpp�� �����Ͻʽÿ�.
//
class CNLaminateAppApp : public CWinApp
{
public:
	CNLaminateAppApp();

	int     ExecMessageHandleAll();
	void	SetInitFinished();
	HWND	GetSplashHwnd();

	CDlgSplash* m_pDlgSplash;
	HANDLE m_hMutex;
// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	BOOL    FirstInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// �����Դϴ�.
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
};

extern CNLaminateAppApp theApp;
