// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ClientSocket.h"
#include "..\MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket
CClientSocket::CClientSocket()
{

}

CClientSocket::CClientSocket(CMainFrame* pMainFrm)
{
    InitializeCriticalSection(&m_csSync);

	m_bAccept = false;
	m_bReceive = false;
	m_bClose = false;
	m_pMainFrm = pMainFrm;		// 다큐멘트에 대한 포인터를 받아서 설정
}

CClientSocket::~CClientSocket()
{
	DeleteCriticalSection(&m_csSync);
	// 소켓 파일의 메모리 해제
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions
void CClientSocket::OnReceive(int nErrorCode) 
{
	char cBuff[PACKET_BUFF_NUM];
	int iRecv;
	CString str;

	if (nErrorCode==0)
	{
		EnterCriticalSection(&m_csSync);
		iRecv = Receive(cBuff, PACKET_BUFF_NUM);
		str = cBuff;
		::memset(cBuff, 0x00, PACKET_BUFF_NUM*sizeof(char));

		m_bReceive = true;
		m_pMainFrm->OnSocketReceive(str);
		m_bReceive = false;
		LeaveCriticalSection(&m_csSync);
	}

	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode) 
{
	if (nErrorCode==0)
	{
		EnterCriticalSection(&m_csSync);
		m_bClose = true;
		m_pMainFrm->OnSocketClose();	
		m_bClose = false;
		LeaveCriticalSection(&m_csSync);
	}

	CSocket::OnClose(nErrorCode);
}
