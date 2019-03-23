// UmacLib.cpp : implementation file
//

#include "stdafx.h"
#include "UmacLib.h"

// CUmacLibLib
CUmacLib::CUmacLib()
: m_bDriverOpen(false)
, m_dwDevice(0)
{
	m_dwDevice		=	1;
	m_bDriverOpen	=	false;
	m_hPmacLib		=	NULL;
}

CUmacLib::~CUmacLib()
{
}

// CUmacLib ¸â¹ö ÇÔ¼ö
bool CUmacLib::OpenDevice(DWORD device)
{
	m_dwDevice	=	device;

	if (!m_hPmacLib)
		m_hPmacLib	=	RuntimeLink();

	if (m_hPmacLib == NULL)
		return	false;

	if (m_bDriverOpen)
		return	true;

	m_bDriverOpen	=	DeviceOpen(m_dwDevice);
	if (m_bDriverOpen)
		return	true;
	else
		return	false;
}

void CUmacLib::CloseDevice(void)
{
	if (m_bDriverOpen)
		m_bDriverOpen	=	!DeviceClose(m_dwDevice);

	if (m_hPmacLib)
	{
		FreeLibrary(m_hPmacLib);
		m_hPmacLib	=	NULL;
	}
}

double CUmacLib::GetDoubleValue(char ch, unsigned int num, double def)
{
	double	temp1	=	0.;

	temp1	=	::DeviceGetVariableDouble(m_dwDevice, ch, num, def);
	return temp1;
}

short int CUmacLib::GetIntValue(char ch, UINT num, short def)
{
	short int temp1;
   	temp1	=	::DeviceGetVariable(m_dwDevice, ch, num, def);

	return temp1;
}

void CUmacLib::SetDoubleValue(char ch, unsigned int num, double val)
{
	::DeviceSetVariableDouble(m_dwDevice,ch, num, val);
}

void CUmacLib::SetIntValue(char ch, UINT num, short int val)
{
    ::DeviceSetVariable(m_dwDevice, ch, num, val);
}

bool CUmacLib::GetResponse(CString command)
{
	TCHAR	response[255];

	if (::DeviceGetResponse(m_dwDevice, response, 255, (LPCSTR)(LPCTSTR)command) > 0)
		return	TRUE;
	else
		return FALSE;
}
