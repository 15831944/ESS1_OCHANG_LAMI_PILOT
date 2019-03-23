// NPioControlDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "NPioControlDll.h"
#include "Serial.h"
#include "mmsystem.h"
#include "pIO.h"
#include "Pci7230Daq.h"
#include "UMAC\UmacLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//
// CNPioControlDllApp

BEGIN_MESSAGE_MAP(CNPioControlDllApp, CWinApp)
END_MESSAGE_MAP()

// The one and only CNPioControlDllApp object
CNPioControlDllApp  theApp;
CPci7230Daq         m_Pci7230Daq;
CSerial	            m_COM1;
CSerial	            m_COM[5];
BOOL                m_bPortOpen[5];
CUmacLib            m_UmacLib;
CRITICAL_SECTION	m_csSync;

#define DEF_MEASURE_VERSION       "V100"
//#define _PCI2416             1
//#define _PCI7230             1
#define _RS232C_LIGHT          1
#define _RS232C_LVS_TESTER     0
#define _RS232C_CHROMA_TESTER  0
#define _RS232C_CHROMA_TESTER2 1

#define ID_COMM	      100
#define STX		     0x02
#define ETX		     0x03

#define CH0_ONOFF       8
#define CH1_ONOFF       9
#define CH2_ONOFF      10
#define CH3_ONOFF      11
#define STORAGE        12
#define CH0            13
#define CH1            14
#define ENABLE_BIT     15
#define ON              0
#define OFF             1  

typedef union
{
	unsigned char m_nData;
    struct
    {
		unsigned char m_nBit0 : 1;
		unsigned char m_nBit1 : 1;
		unsigned char m_nBit2 : 1;
		unsigned char m_nBit3 : 1;
		unsigned char m_nBit4 : 1;
		unsigned char m_nBit5 : 1;
		unsigned char m_nBit6 : 1;
		unsigned char m_nBit7 : 1;
	} m_BIT;
} DIO_IO_MAP;

BOOL    m_bDone;
long    m_nInput;
long    m_nCnt;
BOOL    m_bExit;
long    m_nOldInput[16];
double  m_dSetTime;
double  m_dEndTime;
DIO_IO_MAP m_IO;

long    ReadDataRs232();
void    ResetTimer();
double  GetTimer();
BOOL    SetIO(long nCh, long nValue);
void    SetTimeEvent(long nMiliSec);
void    KillTimeEvent(long nTimeID);
void    CALLBACK EventFuncProc(UINT nTimeID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2); 

// CNPioControlDllApp construction
CNPioControlDllApp::CNPioControlDllApp()
{

}

// CNPioControlDllApp initialization
BOOL CNPioControlDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

// PioControlDll.cpp : Defines the initialization routines for the DLL.
_declspec(dllexport) BOOL NPioControlDll_Open(long nPort, long nBaud, long nSize, long nParity, long nStop)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bRet=TRUE;

	InitializeCriticalSection(&m_csSync);

#ifdef _PCI7230
	m_Pci7230Daq.Open();
#endif

#ifdef _PCI2416
	pIO_init();
	m_nInput = pGet_in(0);
#endif

	m_bDone = FALSE;
	for(long i=0; i<16; i++)
        m_nOldInput[i] = (m_nInput>>i) & 0x01;

	return bRet;
}

_declspec(dllexport) void NPioControlDll_Close()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef _PCI7230
	m_Pci7230Daq.Close();
#endif

#ifdef _PCI2416
    SetIO(0, 0);
    SetIO(1, 0);
    SetIO(2, 0);
    SetIO(3, 0);
	pIO_end();
#endif

	DeleteCriticalSection(&m_csSync);
}

_declspec(dllexport) void NPioControlDll_Write(long nData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	EnterCriticalSection(&m_csSync);

#ifdef _PCI7230
	m_Pci7230Daq.WritePort(nData);
#endif

#ifdef _PCI2416
    pSet_out(0, nData);
#endif

#ifdef _RS232C_DIO
 	char pBuf[100];

    m_IO.m_nData = (unsigned char)nData;
	sprintf_s(pBuf, "%c%c%c", 0xaa, 0x51, m_IO.m_nData);
	m_COM1.SendData(pBuf, 3);	

	Sleep(5);
	m_COM1.ReadData(pBuf, 50);
#endif

	LeaveCriticalSection(&m_csSync);
}

_declspec(dllexport) void NPioControlDll_BitWrite(long nBit, BOOL bSig)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	EnterCriticalSection(&m_csSync);

#ifdef _PCI7230
	m_Pci7230Daq.SetWritePort(nBit, bSig);
#endif

#ifdef _PCI2416
	if (bSig)
        pSet_out_bit(0, (short)nBit);
	else
        pReset_out_bit(0, (short)nBit);
#endif

#ifdef _RS232C_DIO
 	char pBuf[100];
	long nCh=2;

	if (nBit==0)      m_IO.m_BIT.m_nBit0 = bSig;
	else if (nBit==1) m_IO.m_BIT.m_nBit1 = bSig;
    else if (nBit==2) m_IO.m_BIT.m_nBit2 = bSig;
	else if (nBit==3) m_IO.m_BIT.m_nBit3 = bSig;
	else if (nBit==4) nCh = 2;
	else if (nBit==5) nCh = 3;

	if (nBit<4)
	{
		sprintf_s(pBuf, "%c%c%c", 0xaa, 0x51, m_IO.m_nData);
		m_COM1.SendData(pBuf, 3);
	}
	else
	{
		if (bSig) sprintf_s(pBuf, "%c%c%c", 0xAA, 0x61+nCh, (long)100);
		else      sprintf_s(pBuf, "%c%c%c", 0xAA, 0x61+nCh, (long)0);
		m_COM1.SendData(pBuf, 3);
	}

	Sleep(5);
	m_COM1.ReadData(pBuf, 50);
#endif

	LeaveCriticalSection(&m_csSync);
}

_declspec(dllexport) void NPioControlDll_SetLight(long nCh, long nBright)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	EnterCriticalSection(&m_csSync);

	if (nBright<0) nBright=0;
	else if (nBright>255) nBright=255;

#ifdef _RS232C_DIO
 	char pBuf[100];

	sprintf(pBuf, "%c%c%c", 0xAA, 0x61+nCh, (long)(nBright*100.0/255.0));
	m_COM.SendData(pBuf, 3);

	Sleep(5);
	m_COM.ReadData(pBuf, 50);
#endif

	LeaveCriticalSection(&m_csSync);
	return;
}

_declspec(dllexport) void NPioControlDll_Exit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_bExit = TRUE;
}

_declspec(dllexport) long NPioControlDll_Read()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	EnterCriticalSection(&m_csSync);
    long nData=0;

#ifdef _PCI7230
	m_Pci7230Daq.ReadPort(&nData);
#endif

#ifdef _PCI2416
	nData = pGet_in(0);
#endif

#ifdef _RS232C_DIO
    nData = ReadDataRs232();
#endif

	LeaveCriticalSection(&m_csSync);
	return nData;
}

_declspec(dllexport) BOOL NPioControlDll_BitReadABS(long nBit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	EnterCriticalSection(&m_csSync);
    BOOL bRet=FALSE;

#ifdef _PCI7230
	bRet = m_Pci7230Daq.GetReadPortABS(nBit);
#endif

#ifdef _PCI2416
	bRet = (BOOL)pGet_in_bit(0, nBit);
#endif

#ifdef _RS232C_DIO
    m_nInput = ReadDataRs232();
    bRet = (m_nInput>>nBit) & 0x01;
#endif

	LeaveCriticalSection(&m_csSync);
	return bRet;
}

_declspec(dllexport) BOOL NPioControlDll_BitRead(long nBit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	EnterCriticalSection(&m_csSync);
	BOOL bRet=FALSE;
	long nTemp;

#ifdef _PCI7230
	m_Pci7230Daq.ReadPort(&m_nInput);
#endif

#ifdef _PCI2416
	m_nInput = pGet_in(0);
#endif

#ifdef _RS232C_DIO
    m_nInput = ReadDataRs232();
#endif

    nTemp = (m_nInput>>nBit) & 0x01;
    if (nTemp != m_nOldInput[nBit])
        if (nTemp) bRet = TRUE; 

	m_nOldInput[nBit] = nTemp;

	LeaveCriticalSection(&m_csSync);
	return bRet;
}

////////////////////     지역 함수    ///////////////////
long ReadDataRs232()
{
	long nTemp;
	CString str;
    char szbuf[100]; 

	m_COM1.ReadData(szbuf, 50);

	sprintf_s(szbuf, "%c%c%c", 0xAA, 0x71, 0x00);
	m_COM1.SendData(szbuf, 3);

	Sleep(3);
	ResetTimer();

	m_bExit = FALSE;
	nTemp = 0;
	while(!m_bExit && GetTimer()<0.1)
	{
		m_COM1.ReadData(szbuf, 50);
		str.Format("%d", szbuf[2]);
		if (strlen(szbuf)==3) 
		{
			nTemp = 15-atoi(str);
			break;
		}
		Sleep(1);
	}

	return nTemp;
}

BOOL SetIO(long nCh, long nValue)
{
	/*
	short i;
	BOOL nBit[8];

    if (nCh==0)
	{
	    pSet_out_bit(0, CH0);
	    pSet_out_bit(0, CH1);
	    pSet_out_bit(0, CH0_ONOFF);
	}
    if (nCh==1)
	{
	    pReset_out_bit(0, CH0);
	    pSet_out_bit(0, CH1);
	    pSet_out_bit(0, CH1_ONOFF);
	}
    if (nCh==2)
	{
	    pSet_out_bit(0, CH0);
	    pReset_out_bit(0, CH1);
	    pSet_out_bit(0, CH2_ONOFF);
	}
    if (nCh==3)
	{
	    pReset_out_bit(0, CH0);
	    pReset_out_bit(0, CH1);
	    pSet_out_bit(0, CH3_ONOFF);
	}

	nValue = 255-nValue;
	for(i=0; i<8; i++)
	{
		nBit[i] = (BOOL)((nValue>>i) & 0x0001);
		if (nBit[i])
            pSet_out_bit(0, i);
		else
			pReset_out_bit(0, i);
	}
    
    if (nCh==0)
	    pReset_out_bit(0, CH0_ONOFF);
    else if (nCh==1)
	    pReset_out_bit(0, CH1_ONOFF);
    else if (nCh==2)
	    pReset_out_bit(0, CH2_ONOFF);
    else if (nCh==3)
	    pReset_out_bit(0, CH3_ONOFF);

	for(i=0; i<3; i++)
	{
		pReset_out_bit(0, STORAGE);
		SetTimeEvent(3);
		while(!m_bDone)
			if (m_nCnt++>5000000) m_bDone=TRUE;

		pSet_out_bit(0, STORAGE);
		SetTimeEvent(3);
		while(!m_bDone)
			if (m_nCnt++>5000000) m_bDone=TRUE;
	}
*/
	return TRUE;
}

void SetTimeEvent(long nMiliSec)
{
	m_bDone = FALSE;
	m_nCnt = 0;
    timeSetEvent(nMiliSec, 1, EventFuncProc, (DWORD)(LPVOID)NULL, TIME_PERIODIC);
}

void KillTimeEvent(long nTimeID)
{
	timeKillEvent(nTimeID);
}

void CALLBACK EventFuncProc(UINT nTimeID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	KillTimeEvent(nTimeID);
	m_bDone = TRUE;
}

void ResetTimer()
{
    m_dSetTime = GetTickCount();
}

double GetTimer()
{
	double dTime=0.0;

	m_dEndTime = GetTickCount();
	dTime = (m_dEndTime - m_dSetTime) / 1000.0;

	return dTime;
}
////////////////////     지역 함수    ///////////////////

_declspec(dllexport) BOOL NUmacDll_Open(long nDevice)
{
	return (BOOL)m_UmacLib.OpenDevice(nDevice);
}

_declspec(dllexport) void NUmacDll_Close()
{
	m_UmacLib.CloseDevice();
}

_declspec(dllexport) double NUmacDll_GetDoubleValue(char ch, long nAddr)
{
	double dV = 0.0;

	dV = m_UmacLib.GetDoubleValue(ch, (UINT)nAddr, NULL);
	return dV;
}

_declspec(dllexport) int NUmacDll_GetIntValue(char ch, long nAddr)
{
	int nV = 0;

	nV = (int)m_UmacLib.GetIntValue(ch, (UINT)nAddr, NULL);
	return nV;
}

_declspec(dllexport) void NUmacDll_SetDoubleValue(char ch, long nAddr, double dValue)
{
	m_UmacLib.SetDoubleValue(ch, (UINT)nAddr, dValue);
}

_declspec(dllexport) void NUmacDll_SetIntValue(char ch, long nAddr, short int nValue)
{
	m_UmacLib.SetIntValue(ch, (UINT)nAddr, nValue);
}

_declspec(dllexport) BOOL NComPort_Open(long nPort, long nBaud, long nSize, long nParity, long nStop)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	char strText[100];

//    EnterCriticalSection(&m_csSync);
	m_bPortOpen[nPort] = FALSE;

#if _RS232C_LVS_TESTER || _RS232C_CHROMA_TESTER || _RS232C_CHROMA_TESTER2
	m_bPortOpen[nPort] = m_COM[nPort].Open(nPort, nBaud, nSize, nParity, nStop);
	str.Format("PORT=%d RET=%d", nPort, m_bPortOpen[nPort]);
#endif

#if _RS232C_CHROMA_TESTER2
	sprintf_s(strText, ":DISP:LCT\n");
	m_COM[nPort].SendData(strText, 10);
#endif

//	LeaveCriticalSection(&m_csSync);
	return m_bPortOpen[nPort];
}

_declspec(dllexport) void NComPort_Close(long nPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	EnterCriticalSection(&m_csSync);

#if _RS232C_LVS_TESTER || _RS232C_CHROMA_TESTER ||  _RS232C_CHROMA_TESTER2
	if (m_bPortOpen[nPort])
	{
        m_COM[nPort].Close();  
		m_bPortOpen[nPort] = FALSE;
	}
#endif

//	DeleteCriticalSection(&m_csSync);
}

_declspec(dllexport) BOOL NComPort_Read(long nPort, char *szbuf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;
	long i, nLen, nBCC;
	unsigned char cBCC;
	CString str, strT;
	char strText[200];

	if (!m_bPortOpen[nPort]) 
		return bRet;

#if _RS232C_LVS_TESTER
	strcpy_s(szbuf, 200, "");
	strcpy_s(strText, 200, "");

	nLen = m_COM[nPort].ReadData(strText, 150);
	if (nLen>0)
	{
		nBCC = (0x02 ^ 0x37 ^ 0x06) | 0x80;
		sprintf(szbuf, "%c%c%c%c%c", 0x02, 0x37, 0x06, nBCC, 0x03);
		m_COM[nPort].SendData(szbuf, 5);

		strT.Format("%s", strText);
		str = strT.Left(nLen);
		strcpy(szbuf, str);
		bRet = nLen;

		Sleep(5);
	}
#endif

#if _RS232C_CHROMA_TESTER
	strcpy_s(szbuf, 200, "");
	strcpy_s(strText, 200, "");

	cBCC = (unsigned char)(0x01+0x70+0x03+0xB1+0x00+0xD7);
	cBCC = ~cBCC;
	cBCC += 1;

	sprintf_s(strText, "%c%c%c%c%c%c%c%c", 0xAB, 0x01, 0x70, 0x03, 0xB1, 0x00, 0xD7, cBCC);
	m_COM[nPort].SendData(strText, 8);

	Sleep(10);
	str = "";

	nLen = m_COM[nPort].ReadData(strText, 100);
	if (nLen>0)
	{
		for(i=0; i<nLen; i++)
		{
			strT.Format("%02X", unsigned char(strText[i]));
			str += strT;
		}

		strcpy(szbuf, str);
	    bRet = nLen;
	}
#endif

#if _RS232C_CHROMA_TESTER2
	strcpy_s(szbuf, 200, "");
	strcpy_s(strText, 200, "");
	
	cBCC = (unsigned char)(0x01+0x70+0x03+0xB1+0x00+0xD7);
	cBCC = ~cBCC;
	cBCC += 1;

	sprintf_s(strText, ":LCTest:MEASure:FETCh?\n");
	m_COM[nPort].SendData(strText, 23);

	Sleep(40);
	str = "";

	nLen = m_COM[nPort].ReadData(strText, 200);
	if (nLen>0)
	{
		/*
		for(i=0; i<nLen; i++)
		{
			strT.Format("%02X", unsigned char(strText[i]));
			str += strT;
		}
		*/

		strcpy(szbuf, strText);
	    bRet = nLen;
	}
 #endif

	return bRet;
}

_declspec(dllexport) BOOL NComPort_BufferClear(long nPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bPortOpen[nPort]) return TRUE;

	BOOL bRet = TRUE;
	long nBCC, nLen;
	char szbuf[20], strText[2000];
	CString str, strT;

#if _RS232C_LVS_TESTER
	nLen = m_COM[nPort].ReadData(strText, 2000);
	if (nLen>0)
	{
		Sleep(20);
		nBCC = (0x02 ^ 0x37 ^ 0x06) | 0x80;
		sprintf_s(szbuf, "%c%c%c%c%c", 0x02, 0x37, 0x06, nBCC, 0x03);
		m_COM[nPort].SendData(szbuf, 5);
		bRet = FALSE;
	}

	Sleep(5);
 #endif

	return bRet;
}

_declspec(dllexport) BOOL NComPort_Mode(long nPort, char chMode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bPortOpen[nPort]) return TRUE;

	BOOL bRet = TRUE;
	long nLen;
	unsigned char cBCC;
	char strText[100];

#if _RS232C_LVS_TESTER
	strcpy_s(strText, 1, "");

	cBCC = (0x02 ^ 0x31 ^ chMode) | 0x80;
	sprintf_s(strText, "%c%c%c%c%c", 0x02, 0x31, chMode, cBCC, 0x03);
	m_COM[nPort].SendData(strText, 5);

	Sleep(10);

	nLen = m_COM[nPort].ReadData(strText, 50);
	if (nLen>0)
	    bRet = FALSE;

	Sleep(10);
 #endif

	return bRet;
}

_declspec(dllexport) BOOL NComPort_GetVersion(long nPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	unsigned char cBCC;
	long i, nLen;
	BOOL bRet = TRUE;
	char strText[100];
	CString str, strT;

	if (!m_bPortOpen[nPort]) 
		return bRet;

#if _RS232C_LVS_TESTER
	cBCC = (0x02 ^ 0x30) | 0x80;
	sprintf_s(strText, "%c%c%c%c", 0x02, 0x30, cBCC, 0x03);
	m_COM[nPort].SendData(strText, 4);

	Sleep(10);

	nLen = m_COM[nPort].ReadData(strText, 50);
	if (nLen>0)
	{
		str.Format("%s", strText);
		if (str.Mid(2, 4) == DEF_MEASURE_VERSION) 
	        bRet = FALSE;
	}

	Sleep(5);
#endif

#if _RS232C_CHROMA_TESTER
	strcpy_s(strText, 100, "");

	cBCC = (unsigned char)(0x01+0x70+0x01+0x90);
	cBCC = ~cBCC;
	cBCC += 1;

	sprintf_s(strText, "%c%c%c%c%c%c", 0xAB, 0x01, 0x70, 0x01, 0x90, cBCC);
	m_COM[nPort].SendData(strText, 6);
	Sleep(5);

	nLen = m_COM[nPort].ReadData(strText, 50);
	if (nLen>0)
	{
		str = "";
		for(i=0; i<nLen; i++)
		{
			strT.Format("%02X", unsigned char(strText[i]));
			str += strT;
		}

		if (str.Find("4348524F")>0) //43C 48H 52R 4FO
			bRet = FALSE;
	}
#endif

#if _RS232C_CHROMA_TESTER2
	strcpy_s(strText, 100, "");

	cBCC = (unsigned char)(0x01+0x70+0x01+0x90);
	cBCC = ~cBCC;
	cBCC += 1;

	
	sprintf_s(strText, "*IDN?\n");
	m_COM[nPort].SendData(strText, 6);
	Sleep(5);

	nLen = m_COM[nPort].ReadData(strText, 50);
	if (nLen>0)
	{
		str.Format("%s", strText);
		if (str.Find("11210") != -1) //43C 48H 52R 4FO
		//if (str.Find("Chroma")>0)
			bRet = FALSE;
	}
#endif

	return bRet;
}

_declspec(dllexport) BOOL NComPort_ClearCount(long nPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bPortOpen[nPort]) return TRUE;

	BOOL bRet = TRUE;
	long nLen;
	unsigned char cBCC;
	char strText[100];

#if _RS232C_LVS_TESTER
	strcpy_s(strText, 1, "");

	cBCC = (0x02 ^ 0x36) | 0x80;
	sprintf_s(strText, "%c%c%c%c", 0x02, 0x36, cBCC, 0x03);
	m_COM[nPort].SendData(strText, 4);

	Sleep(10);

	nLen = m_COM[nPort].ReadData(strText, 50);
	if (nLen>0)
	    bRet = FALSE;

	Sleep(10);
 #endif

	return bRet;
}

_declspec(dllexport) BOOL NComPort_Write(long nPort, char chComm, char *szbuf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bPortOpen[nPort]) return TRUE;

	BOOL bRet = TRUE;
	long i, nLen;
	unsigned char cBCC;
	char strText[100];

#if _RS232C_LVS_TESTER
	strcpy_s(strText, 100, "");

	cBCC = 0x02 ^ 0x34 ^ chComm;
	for(i=0; i<4; i++)
		cBCC ^= szbuf[i];
	cBCC = cBCC | 0x80;

	sprintf_s(strText, "%c%c%c%s%c%c", 0x02, 0x34, chComm, szbuf, cBCC, 0x03);
	m_COM[nPort].SendData(strText, 10);

	Sleep(10);

	nLen = m_COM[nPort].ReadData(strText, 50);
	if (nLen>0)
		bRet = FALSE;

	Sleep(10);
 #endif

	return bRet;
}

_declspec(dllexport) void NComPort_SetLight(long nPort, long nCh, long *nBright)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long i;
//	EnterCriticalSection(&m_csSync);

	if (!m_bPortOpen[nPort]) return;

	for(i=0; i<6; i++) 
	{
		if (nBright[i]<0) nBright[i]=0;
		if (nBright[i]>999) nBright[i]=999;
	}

#if _RS232C_LIGHT
 	char pBuf[100];

  	sprintf_s(pBuf, ":00B01%02d1%03d,%03d,%03d,%03d,%03d,%03d%c%c", nCh, nBright[0], nBright[1], nBright[2], nBright[3], nBright[4], nBright[5], 0x0D, 0x0A);
	m_COM[nPort].SendData(pBuf, 34);

//  sprintf_s(pBuf, ":1%1d%03d,%03d,%03d,%03d%c%c", nCh, nBright[0], nBright[1], nBright[2], nBright[3], 0x0D, 0x0A);
//	m_COM[nPort].SendData(pBuf, 20);

	Sleep(20);
//	m_COM.ReadData(pBuf, 50);
#endif

//	LeaveCriticalSection(&m_csSync);
	return;
}
