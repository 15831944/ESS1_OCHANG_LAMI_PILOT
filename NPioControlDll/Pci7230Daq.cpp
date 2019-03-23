// Pci7230Daq.cpp : implementation file
//

#include "stdafx.h"
#include "Pci7230Daq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPci7230Daq

CPci7230Daq::CPci7230Daq()
{
	m_nCardNum = -1;

	m_nInput = 0;
	m_nOldInput = 0;
	m_nOutput = 0;
}

CPci7230Daq::~CPci7230Daq()
{
}


BEGIN_MESSAGE_MAP(CPci7230Daq, CStatic)
	//{{AFX_MSG_MAP(CPci7230Daq)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPci7230Daq message handlers
BOOL CPci7230Daq::Open()
{
	m_nCardNum = Register_Card(PCI_7230, 0);
    if (m_nCardNum < 0) 
		return FALSE;

    return TRUE;
}

BOOL CPci7230Daq::Close()
{
	if (m_nCardNum >= 0)
	{
		Release_Card(m_nCardNum);
		return TRUE;
	}

	return FALSE;
}

BOOL CPci7230Daq::GetReadPort(long nPort)
{
	BOOL bRet=FALSE;

	DI_ReadPort(m_nCardNum, 0, &m_nInput);
    if (((m_nInput>>nPort)&0x01) != ((m_nOldInput>>nPort)&0x01) )
    {
        if ((m_nInput>>nPort) & 0x01)
			bRet = TRUE; 
	}

    m_nOldInput = m_nInput;
	return bRet;
}

BOOL CPci7230Daq::GetReadPortABS(long nPort)
{
	BOOL bRet=FALSE;

	DI_ReadPort(m_nCardNum, 0, &m_nInput);
    if ((m_nInput>>nPort) & 0x01)
		bRet = TRUE; 

	return bRet;
}

void CPci7230Daq::ReadPort(long *nData)
{
	DI_ReadPort(m_nCardNum, 0, &m_nInput);
	*nData = (long)m_nInput;
}

void CPci7230Daq::SetWritePort(long nPort, BOOL bSignal)
{
	DWORD nTemp=0x0001<<nPort;

	if (bSignal)
		m_nOutput |= nTemp;
	else
		m_nOutput &= (0xFFFF ^ nTemp);

	DO_WritePort(m_nCardNum, 0, m_nOutput);
}

void CPci7230Daq::WritePort(long nData)
{
	m_nOutput = (U16)nData;
	DO_WritePort(m_nCardNum, 0, m_nOutput);
}
