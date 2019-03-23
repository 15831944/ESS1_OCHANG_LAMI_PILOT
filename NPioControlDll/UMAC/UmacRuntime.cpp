/***************************************************************************/
/*  File: runtime.cpp - Dynamic link functions.                            */
/*                                                                         */
/***************************************************************************/

#include "stdafx.h"
#include "UmacRuntime.h"


//************************************************************************
// Function instants for runtime linking
//************************************************************************
HINSTANCE hPmacLib;

// Comm functions
#ifdef _UNICODE
//  SZLOADSTRINGW DeviceLoadString;
  SENDLINEW DeviceSendLine;
  SENDCTRLCHARW DeviceSendCtrlChar;
  SENDCOMMANDW DeviceSendCommand;
  GETLINEW DeviceGetLine;
  SENDCHARW DeviceSendChar;
  GETBUFFERW DeviceGetBuffer;
  GETRESPONSEW DeviceGetResponse;
  DOWNLOADW DeviceDownload;
  TESTDPRAM DeviceTestDPRAM;
  TESTDPRABORT DeviceTestDPRAMAbort;
  INBOOTSTRAP DeviceInBootStrapMode;

//  GETROMDATEW DeviceGetRomDate;
//  GETROMVERSIONW DeviceGetRomVersion;
#else
  SZLOADSTRINGA DeviceLoadString;
  SENDLINEA DeviceSendLine;
  SENDCTRLCHARA DeviceSendCtrlChar;
  SENDCOMMANDA DeviceSendCommand;
  GETLINEA DeviceGetLine;
  SENDCHARA DeviceSendChar;
  GETBUFFERA DeviceGetBuffer;
  GETRESPONSEA DeviceGetResponse;
  DOWNLOADA DeviceDownload;
  GETROMDATEA DeviceGetRomDate;
  GETROMVERSIONA DeviceGetRomVersion;
  TESTDPRAM DeviceTestDPRAM;
  TESTDPRABORT DeviceTestDPRAMAbort;
  INBOOTSTRAP DeviceInBootStrapMode;

#endif

PMACCONFIGURE DevicePmacConfigure;
READREADY DeviceReadReady;
OPENPMACDEVICE DeviceOpen;
CLOSEPMACDEVICE DeviceClose;
FLUSH DeviceFlush;
GETASCIICOMM DeviceGetAsciiComm;
SETASCIICOMM DeviceSetAsciiComm;
GETERROR DeviceGetError;
GETERRORSTRA DeviceGetErrorStr;
INTRTERMINATE DeviceINTRTerminate;
INTRWNDMSGINIT  DeviceINTRWndMsgInit;
INTRPROCESSFUNCTION DeviceINTRFuncInit;
MOTIONBUFOPEN DeviceMotionBufOpen;
ROTBUFOPEN DeviceRotBufOpen;
GETVARIABLE DeviceGetVariable;
GETVARIABLEDOUBLE DeviceGetVariableDouble;
GETIVARIABLE DeviceGetIVariable;
GETIVARIABLELONG DeviceGetIVariableLong;
GETIVARIABLEDOUBLE DeviceGetIVariableDouble;
SETVARIABLE DeviceSetVariable;
SETIVARIABLE DeviceSetIVariable;
SETIVARIABLELONG DeviceSetIVariableLong;
SETIVARIABLEDOUBLE DeviceSetIVariableDouble;
SETVARIABLEDOUBLE DeviceSetVariableDouble;
GETPROGRAMINFO DeviceGetProgramInfo;
GETUSERHANDLE DeviceGetUserHandle;
CONFIGURE DeviceConfigure;;
DPRVARBUFINIT DeviceDPRVarBufInit;
DPRVARBUFREMOVE DeviceDPRVarBufRemove;

////////////////////////////////////////////////////////////////////////////
// DPR Binary Rotary Buffer
////////////////////////////////////////////////////////////////////////////
DPRROTBUFINIT	DeviceDPRRotBufInit;
DPRASCIISTRTOROTA	DeviceDPRAsciiStrToRot;
DPRROTBUF	DeviceDPRRotBuf;
DPRBUFLAST	DeviceDPRBufLast;
DPRROTBUFREMOVE DeviceDPRRotBufRemove;
DPRROTBUFCHANGE DeviceDPRRotBufChange;

DPRVELOCITY DeviceDPRVelocity;
DPRVARBUFREAD DeviceDPRVarBufRead;
DPRGETMOTORMOTION DeviceDPRGetMotorMotion;
DPRGETPROGRAMMODE DeviceDPRGetProgramMode;
DPRSYSINPOSITION DeviceDPRSysInposition;
DPRROTBUFOPEN DeviceDPRRotBufOpen;
DPRPROGREMAINING DeviceDPRProgRemaining;
DPRGETFEEDRATEMODE DeviceDPRGetFeedRateMode;
GETPMACTYPE DeviceGetType;
DOWNLOADFIRMWAREFILE DeviceLoadFirmwareFile;
ABORTDOWNLOAD DeviceAbortDownload;
SETMAXDOWNLOADERRORS DeviceSetMaxDownloadErrors;

////////////////////////////////////////////////////////////////////////////
// DPR Real Time Data Buffer functions
////////////////////////////////////////////////////////////////////////////
DPRREALTIME DeviceDPRRealTime;
DPRSETHOSTBUSYBIT DeviceDPRSetHostBusyBit;
DPRGETHOSTBUSYBIT DeviceDPRGetHostBusyBit;
DPRGETPMACBUSYBIT DeviceDPRGetPMACBusyBit;
DPRGETSERVOTIMER DeviceDPRGetServoTimer;
DPRSETMOTORS DeviceDPRSetMotors;
DPRGETCOMMANDEDPOS DeviceDPRGetCommandedPos;
DPRPOSITION DeviceDPRPosition;
DPRFOLLOWERROR DeviceDPRFollowError;
DPRGETVEL DeviceDPRGetVel;
DPRGETMASTERPOS DeviceDPRGetMasterPos;
DPRGETCOMPENSATIONPOS DeviceDPRGetCompensationPos;
DPRGETPREVDAC DeviceDPRGetPrevDAC;
DPRGETMOVETIME DeviceDPRGetMoveTime;



HINSTANCE RuntimeLink()

{
  TCHAR buf[128];

  // Get handle to PMAC.DLL
  hPmacLib = LoadLibrary(DRIVERNAME);

  if(hPmacLib != NULL)
  {

    // Get all COMM the function handles required
#ifdef _UNICODE
    DeviceSendLine = (SENDLINEW)GetProcAddress(hPmacLib,"PmacSendLineW");
    DeviceGetLine = (GETLINEW)GetProcAddress(hPmacLib,"PmacGetLineW");
    DeviceSendChar = (SENDCHARW)GetProcAddress(hPmacLib,"PmacSendCharW");
    //DeviceSendCtrlChar = (SENDCTRLCHARW)GetProcAddress(hPmacLib,"PmacSendCtrlCharW");
    DeviceDownload = (DOWNLOADW)GetProcAddress(hPmacLib,"PmacDownloadW");
    //DeviceGetRomDate = (GETROMDATEW)GetProcAddress(hPmacLib,"PmacGetRomDateW");
    //DeviceGetRomVersion = (GETROMVERSIONW)GetProcAddress(hPmacLib,"PmacGetRomVersionW");
    DeviceSendCommand = (SENDCOMMANDW)GetProcAddress(hPmacLib,"PmacSendCommandW");
    DeviceGetResponse = (GETRESPONSEW)GetProcAddress(hPmacLib,"PmacGetResponseW");
    DeviceGetBuffer = (GETBUFFERW)GetProcAddress(hPmacLib,"PmacGetBufferW");
    //DeviceLoadString = (SZLOADSTRINGW)GetProcAddress(hPmacLib,"szLoadStringW");

#else
    DeviceSendLine = (SENDLINEA)GetProcAddress(hPmacLib,"PmacSendLineA");
    DeviceGetLine = (GETLINEA)GetProcAddress(hPmacLib,"PmacGetLineA");
    DeviceSendChar = (SENDCHARA)GetProcAddress(hPmacLib,"PmacSendCharA");
    //DeviceSendCtrlChar = (SENDCTRLCHARA)GetProcAddress(hPmacLib,"PmacSendCtrlCharA");
    DeviceDownload = (DOWNLOADA)GetProcAddress(hPmacLib,"PmacDownloadA");
    DeviceGetRomDate = (GETROMDATEA)GetProcAddress(hPmacLib,"PmacGetRomDateA");
    DeviceGetRomVersion = (GETROMVERSIONA)GetProcAddress(hPmacLib,"PmacGetRomVersionA");
    DeviceSendCommand = (SENDCOMMANDA)GetProcAddress(hPmacLib,"PmacSendCommandA");
    DeviceGetResponse = (GETRESPONSEA)GetProcAddress(hPmacLib,"PmacGetResponseA");
    DeviceGetBuffer = (GETBUFFERA)GetProcAddress(hPmacLib,"PmacGetBufferA");

    //DeviceLoadString = (SZLOADSTRINGA)GetProcAddress(hPmacLib,"szLoadStringA");
#endif

	DevicePmacConfigure = (PMACCONFIGURE)GetProcAddress(hPmacLib,"PmacConfigure");
    DeviceOpen = (OPENPMACDEVICE)GetProcAddress(hPmacLib,"OpenPmacDevice");
    DeviceClose = (CLOSEPMACDEVICE)GetProcAddress(hPmacLib,"ClosePmacDevice");
    DeviceReadReady = (READREADY)GetProcAddress(hPmacLib,"PmacReadReady");
    DeviceFlush = (FLUSH)GetProcAddress(hPmacLib,"PmacFlush");
    DeviceGetAsciiComm = (GETASCIICOMM)GetProcAddress(hPmacLib,"PmacGetAsciiComm");
    DeviceSetAsciiComm = (SETASCIICOMM)GetProcAddress(hPmacLib,"PmacSetAsciiComm");
    DeviceGetError = (GETERROR)GetProcAddress(hPmacLib,"PmacGetError");
    DeviceGetErrorStr = (GETERRORSTRA)GetProcAddress(hPmacLib,"PmacGetErrorStrA");
    DeviceINTRTerminate = (INTRTERMINATE)GetProcAddress(hPmacLib,"PmacINTRTerminate");
    DeviceINTRWndMsgInit = (INTRWNDMSGINIT)GetProcAddress(hPmacLib,"PmacINTRWndMsgInit");
	DeviceINTRFuncInit = (INTRPROCESSFUNCTION)GetProcAddress(hPmacLib,"PmacINTRFuncCallInit");
    DeviceGetType = (GETPMACTYPE)GetProcAddress(hPmacLib,"PmacGetPmacType");
    DeviceLoadFirmwareFile = (DOWNLOADFIRMWAREFILE)GetProcAddress(hPmacLib,"PmacDownloadFirmwareFile");
    DeviceAbortDownload = (ABORTDOWNLOAD)GetProcAddress(hPmacLib,"PmacAbortDownload");
    DeviceSetMaxDownloadErrors = (SETMAXDOWNLOADERRORS)GetProcAddress(hPmacLib,"PmacSetMaxDownloadErrors");

    DeviceGetError = (GETERROR)GetProcAddress(hPmacLib,"PmacGetError");
    DeviceGetAsciiComm = (GETASCIICOMM)GetProcAddress(hPmacLib,"PmacGetAsciiComm");
    DeviceSetAsciiComm = (SETASCIICOMM)GetProcAddress(hPmacLib,"PmacSetAsciiComm");
    DeviceMotionBufOpen = (MOTIONBUFOPEN)GetProcAddress(hPmacLib,"PmacMotionBufOpen");
    DeviceRotBufOpen = (ROTBUFOPEN)GetProcAddress(hPmacLib,"PmacRotBufOpen");
    DeviceGetIVariable = (GETIVARIABLE)GetProcAddress(hPmacLib,"PmacGetIVariable");
	DeviceGetVariable = (GETVARIABLE)GetProcAddress(hPmacLib,"PmacGetVariable");
	DeviceGetVariableDouble = (GETVARIABLEDOUBLE)GetProcAddress(hPmacLib,"PmacGetVariableDouble");
    DeviceGetIVariableLong = (GETIVARIABLELONG)GetProcAddress(hPmacLib,"PmacGetIVariableLong");
    DeviceGetIVariableDouble = (GETIVARIABLEDOUBLE)GetProcAddress(hPmacLib,"PmacGetIVariableDouble");
	DeviceSetVariable = (SETVARIABLE)GetProcAddress(hPmacLib,"PmacSetVariable");
	DeviceSetVariableDouble = (SETVARIABLEDOUBLE)GetProcAddress(hPmacLib,"PmacSetVariableDouble");
    DeviceSetIVariable = (SETIVARIABLE)GetProcAddress(hPmacLib,"PmacSetIVariable");
    DeviceSetIVariableLong = (SETIVARIABLELONG)GetProcAddress(hPmacLib,"PmacSetIVariableLong");
    DeviceSetIVariableDouble = (SETIVARIABLEDOUBLE)GetProcAddress(hPmacLib,"PmacSetIVariableDouble");
    DeviceGetProgramInfo = (GETPROGRAMINFO)GetProcAddress(hPmacLib,"PmacGetProgramInfo");
    DeviceGetUserHandle = (GETUSERHANDLE)GetProcAddress(hPmacLib,"PmacGetUserHandle");
    DeviceConfigure = (CONFIGURE)GetProcAddress(hPmacLib,"PmacConfigure");
    DeviceDPRVarBufInit = (DPRVARBUFINIT)GetProcAddress(hPmacLib,"PmacDPRVarBufInit");
    DeviceDPRVarBufRemove = (DPRVARBUFREMOVE)GetProcAddress(hPmacLib,"PmacDPRVarBufRemove");
    DeviceDPRVarBufRead = (DPRVARBUFREAD)GetProcAddress(hPmacLib,"PmacDPRVarBufRead");

    DeviceDPRVelocity = (DPRVELOCITY)GetProcAddress(hPmacLib,"PmacDPRVelocity");
    DeviceDPRGetMotorMotion = (DPRGETMOTORMOTION)GetProcAddress(hPmacLib,"PmacDPRGetMotorMotion");
    DeviceDPRGetProgramMode = (DPRGETPROGRAMMODE)GetProcAddress(hPmacLib,"PmacDPRGetProgramMode");
    DeviceDPRSysInposition = (DPRSYSINPOSITION)GetProcAddress(hPmacLib,"PmacDPRSysInposition");
    DeviceDPRRotBufOpen = (DPRROTBUFOPEN)GetProcAddress(hPmacLib,"PmacDPRRotBufOpen");
    DeviceDPRProgRemaining = (DPRPROGREMAINING)GetProcAddress(hPmacLib,"PmacDPRProgRemaining");
    DeviceDPRGetFeedRateMode = (DPRGETFEEDRATEMODE)GetProcAddress(hPmacLib,"PmacDPRGetFeedRateMode");
	DeviceInBootStrapMode = (INBOOTSTRAP)GetProcAddress(hPmacLib,"PmacInBootStrapMode");

	DeviceDPRBufLast = (DPRBUFLAST)GetProcAddress(hPmacLib,"PmacDPRBufLast");
	DeviceDPRRotBuf = (DPRROTBUF)GetProcAddress(hPmacLib,"PmacDPRRotBuf");
	DeviceDPRAsciiStrToRot = (DPRASCIISTRTOROTA)GetProcAddress(hPmacLib,"PmacDPRAsciiStrToRotA");
	DeviceDPRRotBufInit = (DPRROTBUFINIT)GetProcAddress(hPmacLib,"PmacDPRRotBufInit");
	DeviceDPRRotBufRemove =	(DPRROTBUFREMOVE)GetProcAddress(hPmacLib,"PmacDPRRotBufRemove");
	DeviceDPRRotBufChange = (DPRROTBUFCHANGE)GetProcAddress(hPmacLib,"PmacDPRRotBufChange");


////////////////////////////////////////////////////////////////////////////
// DPR Real Time Data Buffer functions
////////////////////////////////////////////////////////////////////////////
	DeviceDPRRealTime =(DPRREALTIME)GetProcAddress(hPmacLib,"PmacDPRRealTime");
	DeviceDPRSetHostBusyBit =(DPRSETHOSTBUSYBIT)GetProcAddress(hPmacLib,"PmacDPRSetHostBusyBit");
	DeviceDPRGetHostBusyBit =(DPRGETHOSTBUSYBIT)GetProcAddress(hPmacLib,"PmacDPRGetHostBusyBit");
	DeviceDPRGetPMACBusyBit =(DPRGETPMACBUSYBIT)GetProcAddress(hPmacLib,"PmacDPRGetPmacBusyBit");
	DeviceDPRGetServoTimer =(DPRGETSERVOTIMER)GetProcAddress(hPmacLib,"PmacDPRGetServoTimer");
	DeviceDPRSetMotors =(DPRSETMOTORS)GetProcAddress(hPmacLib,"PmacDPRSetMotors");
	DeviceDPRGetCommandedPos =(DPRGETCOMMANDEDPOS)GetProcAddress(hPmacLib,"PmacDPRGetCommandedPos");
	DeviceDPRPosition =(DPRPOSITION)GetProcAddress(hPmacLib,"PmacDPRPosition");
	DeviceDPRFollowError =(DPRFOLLOWERROR)GetProcAddress(hPmacLib,"PmacDPRFollowError");
	DeviceDPRGetVel =(DPRGETVEL)GetProcAddress(hPmacLib,"PmacDPRGetVel");
	DeviceDPRGetMasterPos =(DPRGETMASTERPOS)GetProcAddress(hPmacLib,"PmacDPRGetMasterPos");
	DeviceDPRGetCompensationPos =(DPRGETCOMPENSATIONPOS)GetProcAddress(hPmacLib,"PmacDPRGetCompensationPos");
	DeviceDPRGetPrevDAC  = (DPRGETPREVDAC)GetProcAddress(hPmacLib,"PmacDPRGetPrevDAC");;
	DeviceDPRGetMoveTime = (DPRGETMOVETIME) GetProcAddress(hPmacLib,"PmacDPRGetMoveTime");;
  DeviceTestDPRAM = (TESTDPRAM)GetProcAddress(hPmacLib,"PmacDPRTest");
  DeviceTestDPRAMAbort = (TESTDPRABORT)GetProcAddress(hPmacLib,"PmacAbortDPRTest");

////////////////////////////////////////////////////////////////////////////
// Check if all is well!
////////////////////////////////////////////////////////////////////////////

    if(!DevicePmacConfigure || !DeviceOpen || !DeviceClose || !DeviceReadReady || !DeviceSendLine || 
      !DeviceGetLine || !DeviceGetErrorStr ||
       !DeviceSendChar || !DeviceGetResponse || !DeviceGetAsciiComm || !DeviceSetAsciiComm ||
       !DeviceGetError || !DeviceDownload || !DeviceINTRTerminate || !DeviceINTRWndMsgInit ||
       !DeviceDPRGetProgramMode ||!DeviceGetVariable|| !DeviceGetIVariableLong || !DeviceGetUserHandle ||
	   !DeviceGetVariableDouble ||
       !DeviceDPRVarBufInit || !DeviceDPRVarBufRemove || !DeviceDPRVelocity || !DeviceDPRVarBufRead ||
       !DeviceDPRGetMotorMotion || !DeviceDPRGetProgramMode || !DeviceDPRSysInposition ||
       !DeviceDPRRotBufOpen || !DeviceDPRProgRemaining || !DeviceDPRGetFeedRateMode ||
       !DeviceSetVariable || !DeviceSetVariableDouble ||
	   //!DeviceLoadString ||
       !DeviceGetRomDate || !DeviceGetRomVersion || !DeviceGetType || !DeviceConfigure ||
       !DeviceFlush || !DeviceSendCommand || !DeviceLoadFirmwareFile ||
       !DeviceAbortDownload || !DeviceSetMaxDownloadErrors || !DeviceTestDPRAM || !DeviceTestDPRAMAbort
	   || !DeviceInBootStrapMode || !DeviceDPRBufLast || !DeviceDPRRotBuf || !DeviceDPRAsciiStrToRot
	   || !DeviceDPRRotBufInit || !DeviceDPRRotBufChange || !DeviceDPRRotBufRemove || !DeviceDPRRealTime ||
	   /*!DeviceDPRSetHostBusyBit|| !DeviceDPRGetHostBusyBit|| !DeviceDPRGetPMACBusyBit||*/!DeviceDPRGetServoTimer||
	   !DeviceDPRSetMotors||!DeviceDPRGetCommandedPos||!DeviceDPRPosition||!DeviceDPRFollowError||
	   !DeviceDPRGetVel||!DeviceDPRGetMasterPos||!DeviceDPRGetCompensationPos||!DeviceDPRGetPrevDAC||
	   !DeviceDPRGetMoveTime || !DeviceGetBuffer
	  )
    {
        sprintf_s(buf,"Error: %d in loading COMM library functions.",GetLastError());
	    MessageBox(0,buf,"Error",MB_OK);
        FreeLibrary(hPmacLib);
        hPmacLib = NULL;
        return NULL;
    }

    return hPmacLib;
  }
  else 
  {
      sprintf_s(buf,"Error: %d in loading PMAC library.",GetLastError());
      MessageBox(0,buf,"Error",MB_OK);
  }
  return NULL;
}

