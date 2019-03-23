#if !defined(AFX_PCI7230DAQ_H__54982994_9C4D_46FD_8D60_86C44CE347B4__INCLUDED_)
#define AFX_PCI7230DAQ_H__54982994_9C4D_46FD_8D60_86C44CE347B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pci7230Daq.h : header file
//
#include "Dask.h"

/////////////////////////////////////////////////////////////////////////////
// CPci7230Daq window
class CPci7230Daq : public CStatic
{
// Construction
public:
	CPci7230Daq();
	BOOL   Open();
	BOOL   Close();

    void   SetWritePort(long nPort, BOOL bSignal);
    void   WritePort(long nData);
    void   ReadPort(long *nData);
    BOOL   GetReadPort(long nPort);
	BOOL   GetReadPortABS(long nPort);

// Attributes
public:
	I16       m_nCardNum;
	unsigned long  m_nOldInput;
	unsigned long  m_nInput;
	unsigned long  m_nOutput;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPci7230Daq)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPci7230Daq();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPci7230Daq)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCI7230DAQ_H__54982994_9C4D_46FD_8D60_86C44CE347B4__INCLUDED_)
