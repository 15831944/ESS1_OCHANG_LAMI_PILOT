// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "..\MainFrm.h"
#include "ListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
}

// 문서 객체에 대한 포인터 변수를 초기화한다.
CListenSocket::CListenSocket(CMainFrame* pMainFrm)	
{
	m_bAccept = false;
	m_bReceive = false;
	m_bClose = false;
	m_pMainFrm = pMainFrm;
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

// 클라이언트로부터 접속 요청에 대한 메세지가 들어오면 호출된다.
void CListenSocket::OnAccept(int nErrorCode) 
{
	m_bAccept = true;
	m_pMainFrm->OnSocketAccept();
	m_bAccept = false;

	CSocket::OnAccept(nErrorCode);
}

void CListenSocket::OnClose(int nErrorCode) 
{
	CSocket::OnClose(nErrorCode);
}
