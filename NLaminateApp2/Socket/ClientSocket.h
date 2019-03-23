#if !defined(AFX_CLIENTSOCKET_H__72A1700A_8E0C_11D2_8D1C_00001C1BC187__INCLUDED_)
#define AFX_CLIENTSOCKET_H__72A1700A_8E0C_11D2_8D1C_00001C1BC187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CMainFrame;
class CClientSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket();
	CClientSocket(CMainFrame* pMainFrm);	
	virtual ~CClientSocket();

public:
	CMainFrame* m_pMainFrm;		
	CRITICAL_SECTION m_csSync;

	bool    m_bAccept;
	bool    m_bReceive;
	bool    m_bClose;

	void	ReceiveData();	// 데이터를 받는 함수

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__72A1700A_8E0C_11D2_8D1C_00001C1BC187__INCLUDED_)
