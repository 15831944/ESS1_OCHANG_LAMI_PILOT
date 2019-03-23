#if !defined(AFX_LISTENSOCKET_H__72A17009_8E0C_11D2_8D1C_00001C1BC187__INCLUDED_)
#define AFX_LISTENSOCKET_H__72A17009_8E0C_11D2_8D1C_00001C1BC187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CMainFrame;
class CListenSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CListenSocket();
	CListenSocket(CMainFrame* pMainFrm);
	virtual ~CListenSocket();

public:
	CMainFrame*    m_pMainFrm;	// 문서 객체에 대한 포인터

	bool    m_bAccept;
	bool    m_bReceive;
	bool    m_bClose;

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__72A17009_8E0C_11D2_8D1C_00001C1BC187__INCLUDED_)
