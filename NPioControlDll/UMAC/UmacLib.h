#pragma once
#include "UmacRuntime.h"

// CUmacLib
class CUmacLib : public CObject
{
public:
	CUmacLib();
	virtual ~CUmacLib();

	BOOL m_bDriverOpen;
	HINSTANCE	m_hPmacLib;
	unsigned int m_dwDevice;

	bool OpenDevice(DWORD device);
	void CloseDevice(void);
	double GetDoubleValue(char ch, UINT num, double def);
	short int GetIntValue(char ch, UINT num, short def);
	void SetDoubleValue(char ch, UINT num, double val);
	void SetIntValue(char ch, UINT num, short int val);
	bool GetResponse(CString command);

protected:
};


