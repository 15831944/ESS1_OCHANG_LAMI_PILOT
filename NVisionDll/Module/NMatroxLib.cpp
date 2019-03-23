// NMatroxLib.cpp : implementation file
//
#include "stdafx.h"
#include "NMatroxLib.h"
#include "math.h"

#if _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNMatroxLib
BOOL GetLicenseKey();

CNMatroxLib::CNMatroxLib()
{
	m_nType = 0;
	m_nCh = 0;
	m_bOpen = FALSE;
	strcpy_s(m_strCameraFile, DCF_FILE);
}

CNMatroxLib::~CNMatroxLib()
{
}

/////////////////////////////////////////////////////////////////////////////
// NVisionLib
BOOL CNMatroxLib::Open(long nType, long nWidth, long nHeight)
{
	if (m_bOpen) return TRUE;

#if _MIL_SETUP
	m_nType = nType;
    if (nType==0)
	{
		MappAlloc(M_DEFAULT, &MilApplication); // MIL를 사용하기 전에 맨 먼저 선언한다.
		MsysAlloc(M_SYSTEM_VGA, M_DEFAULT ,M_COMPLETE, &MilSystem[0]);
//		MsysAlloc(M_SYSTEM_VGA, M_DEFAULT ,M_COMPLETE, &MilSystem[1]);

		MbufAlloc2d(MilSystem[0], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilImage[0]);
//		MbufAlloc2d(MilSystem[1], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilImage[1]);
		MbufAlloc2d(MilSystem[0], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &ProcessImage[0]);
//		MbufAlloc2d(MilSystem[1], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &ProcessImage[1]);
	}
	else
	{
//		MappAlloc(M_DEFAULT, &MilApplication); // MIL를 사용하기 전에 맨 먼저 선언한다.
//		MsysAlloc(M_SYSTEM_METEOR_II, M_DEV0, M_COMPLETE, &MilSystem[0]);
//		MsysAlloc(M_SYSTEM_METEOR_II, M_DEV1, M_COMPLETE, &MilSystem[1]);
//		MdigAlloc(MilSystem[0], M_DEFAULT, DCF_FILE, M_DEFAULT,&MilDigitizer[0]);
//		MdigAlloc(MilSystem[1], M_DEFAULT, DCF_FILE, M_DEFAULT,&MilDigitizer[1]);
//		MdigControl(MilDigitizer[0], M_GRAB_MODE, M_ASYNCHRONOUS);
//		MdigControl(MilDigitizer[1], M_GRAB_MODE, M_ASYNCHRONOUS);

//		MbufAlloc2d(MilSystem[0], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_PROC, &MilImage[0]);
//		MbufAlloc2d(MilSystem[1], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_PROC, &MilImage[1]);
//		MbufAlloc2d(MilSystem[0], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &ProcessImage[0]);
//		MbufAlloc2d(MilSystem[1], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &ProcessImage[1]);
//		MappControl(M_ERROR, M_PRINT_DISABLE);
	}
#endif

	m_bOpen = TRUE;
	return FALSE;
}

BOOL CNMatroxLib::Close()
{
	if (!m_bOpen)     return TRUE;

#if _MIL_SETUP
	if (m_nType==0)
	{
//		if (ProcessImage[1])   MbufFree(ProcessImage[1]);
		if (ProcessImage[0])   MbufFree(ProcessImage[0]);
//		if (MilImage[1])       MbufFree(MilImage[1]);
		if (MilImage[0])       MbufFree(MilImage[0]);
//		if (MilSystem[1])	   MsysFree(MilSystem[1]);
		if (MilSystem[0])	   MsysFree(MilSystem[0]);
		if (MilApplication)    MappFree(MilApplication);
	}
	else
	{
//		if (ProcessImage[1])   MbufFree(ProcessImage[1]);
//		if (ProcessImage[0])   MbufFree(ProcessImage[0]);
//		if (MilImage[1])       MbufFree(MilImage[1]);
//		if (MilImage[0])       MbufFree(MilImage[0]);
//		if (MilDigitizer[1])   MdigFree(MilDigitizer[1]);
//    	if (MilDigitizer[0])   MdigFree(MilDigitizer[0]);
//		if (MilSystem[1])	   MsysFree(MilSystem[1]);
//		if (MilSystem[0])	   MsysFree(MilSystem[0]);
//		if (MilApplication)    MappFree(MilApplication);
    }
#endif

	m_bOpen = FALSE;
	return FALSE;
}

BOOL CNMatroxLib::Grab(LPBYTE pImage)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MdigHalt(MilDigitizer[m_nCh]);
	MdigGrab(MilDigitizer[m_nCh], MilImage[m_nCh]);
	MdigGrabWait(MilDigitizer[m_nCh], M_GRAB_END);
	MbufGet(MilImage[m_nCh], pImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::Grab(long nCh, LPBYTE pImage)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MdigHalt(MilDigitizer[nCh]);
	MdigGrab(MilDigitizer[nCh], MilImage[nCh]);
	MdigGrabWait(MilDigitizer[nCh], M_GRAB_END);
	MbufGet(MilImage[nCh], pImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::SetSelectChannel(long nCh)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	/*
	MdigHalt(MilDigitizer);
	switch (nCh) 
	{
	    case 0:
		    MdigChannel(MilDigitizer, M_CH0);
	        break;
	    case 1:
		    MdigChannel(MilDigitizer, M_CH1);
	        break;
	    case 2:
		    MdigChannel(MilDigitizer, M_CH2);
	        break;
    	case 3:
	    	MdigChannel(MilDigitizer, M_CH3);
   		    break;
		case 4 :
		    MdigChannel(MilDigitizer, M_CH4);
	        break;
		case 5 :
		    MdigChannel(MilDigitizer, M_CH5);
	        break;
    	default :
	    	break;
	}

    Sleep(20);
	*/
#endif

	m_nCh = nCh;
	return FALSE;
}

BOOL CNMatroxLib::LoadImageFromFile(LPCTSTR strFileName, LPBYTE pImage, long nWidth, long nHeight)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	CString str;

	str.Format("%s", strFileName);
	str = str.Right(4);
	if (str == ".jpg" || str == ".JPG")
		MbufImport((char *)strFileName, M_JPEG_LOSSY, M_LOAD, MilSystem[m_nCh], &MilImage[m_nCh]);
	else
		MbufImport((char *)strFileName, M_BMP, M_LOAD, MilSystem[m_nCh], &MilImage[m_nCh]);

	MbufGet(MilImage[m_nCh], pImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::SaveImageToFile(LPCTSTR strFileName, LPBYTE pImage, long nWidth, long nHeight)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	CString str;
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, pImage);

	str.Format("%s", strFileName);
	str = str.Right(4);
	if (str == ".jpg" || str == ".JPG")
		MbufExport((char *)strFileName, M_JPEG_LOSSY, MilProcImage);//MilImage[m_nCh]);
	else
    	MbufExport((char *)strFileName, M_BMP, MilProcImage);//MilImage[m_nCh]);

	MbufFree(MilProcImage);
#endif

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
//                          Mil Morpology
////////////////////////////////////////////////////////////////////////
BOOL CNMatroxLib::DilateProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long iter)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, fmSourImage);

	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);
//    MimDilate(SubImage, SubImage, iter, M_GRAYSCALE);
	MbufGet(MilProcImage, fmTargImage);

    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::ErodeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long iter)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, fmSourImage);

	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);
//    MimErode(SubImage, SubImage, iter, M_GRAYSCALE);
	MbufGet(MilProcImage, fmTargImage);

    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::OpenProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long iter)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, fmSourImage);

	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);
//    MimOpen(SubImage, SubImage, iter, M_GRAYSCALE);
	MbufGet(MilProcImage, fmTargImage);

    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::CloseProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long iter)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, fmSourImage);

	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);
 //   MimClose(SubImage, SubImage, iter, M_GRAYSCALE);
	MbufGet(MilProcImage, fmTargImage);

    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::BinarizeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh)
{
	if (!m_bOpen) return TRUE;

	for(long i=y1; i<=y2; i++)
	for(long j=x1; j<=x2; j++) 
	{
		if (fmSourImage[i*nWidth + j] > nThresh)
            fmTargImage[i*nWidth + j] = WHITE_LEVEL;
		else
			fmTargImage[i*nWidth + j] = BLACK_LEVEL;
	}

	return FALSE;
}

BOOL CNMatroxLib::SmoothProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, fmSourImage);

	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);
//	for(i=0; i<nIter; i++)
//		MimConvolve(SubImage, SubImage, M_SMOOTH);
	MbufGet(MilProcImage, fmTargImage);

    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::SharpnessProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, fmSourImage);

	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);
//	MimConvolve(SubImage, SubImage, M_SHARPEN);
	MbufGet(MilProcImage, fmTargImage);

    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::SobelEdgeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, fmSourImage);

	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);
//	MimConvolve(SubImage, SubImage, M_SOBEL);
	MbufGet(MilProcImage, fmTargImage);

    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return FALSE;
}

BOOL CNMatroxLib::EdgeDetectProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh)
{
	if (!m_bOpen) return TRUE;

#if _MIL_SETUP
	MIL_ID MilProcImage;

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, fmSourImage);

	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);
//    MimEdgeDetect(SubImage, SubImage, M_NULL, M_SOBEL, M_REGULAR_EDGE_DETECT, nThresh);

	MbufGet(MilProcImage, fmTargImage);

    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return FALSE;
}

#define MAX_CODE_READ_NUM     3
BOOL CNMatroxLib::BarCodeRead(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nType, long nNum, char *strCode)
{
	BOOL bRet=TRUE;
	CString str="";

#if _MIL_CODE
	strcpy_s(strCode, "");
	if (nType<0 || nType>4)
	{
		AfxMessageBox(" Not mismatch code type ! ");
		return bRet;
	}

	long i;
	char	csBarCodeData[256];
    MIL_DOUBLE nTotalNum = 0;
    MIL_DOUBLE nLength = 0;
    MIL_INT    lBarCodeStatus;        

	if (!m_bOpen) return TRUE;
	MIL_ID MilProcImage, MilBarCode, CodeResults;

	long CODE[5] = {M_CODE128, M_CODE39, M_EAN13, M_PDF417, M_UPC_A};

	MbufAlloc2d(MilSystem[m_nCh], nWidth, nHeight, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilProcImage);
	MbufPut(MilProcImage, pImage);
	MbufChild2d(MilProcImage, x1, y1, x2-x1, y2-y1, &SubImage);

	McodeAlloc(MilSystem[m_nCh], M_DEFAULT, M_DEFAULT, &MilBarCode);
    McodeModel(MilBarCode, M_ADD, CODE[nType], M_NULL, M_DEFAULT, M_NULL);
 
	McodeAllocResult(MilSystem[m_nCh], M_DEFAULT, &CodeResults);
    McodeControl(MilBarCode, M_SEARCH_ANGLE,  360);
	McodeControl(MilBarCode, M_SEARCH_ANGLE_DELTA_NEG,  180);
	McodeControl(MilBarCode, M_SEARCH_ANGLE_DELTA_POS,  180);
	McodeControl(MilBarCode, M_POSITION_ACCURACY, M_LOW);
    McodeControl(MilBarCode, M_NUMBER, MAX_CODE_READ_NUM);

   	McodeRead(MilBarCode, SubImage, CodeResults);
	McodeGetResult(CodeResults, M_STATUS+M_TYPE_MIL_INT, &lBarCodeStatus);
    McodeGetResult(CodeResults, M_NUMBER ,&nTotalNum);

//	if (lBarCodeStatus == M_STATUS_OK)
	{
	    for(i=0; i<nTotalNum; i++)
		{
		    McodeGetResultSingle(CodeResults, i, M_STRING_SIZE, &nLength);
		    McodeGetResultSingle(CodeResults, i, M_STRING, csBarCodeData);
		    if (nLength==nNum)
			{
				strcpy(strCode, csBarCodeData);
				bRet = FALSE;
				break;
			}
		}
	}

	McodeFree(CodeResults);
	McodeFree(MilBarCode);
    MbufFree(SubImage);
	MbufFree(MilProcImage);
#endif

	return bRet;
}

void CNMatroxLib::ResetTimer(long nID)
{
//    MappTimer(M_TIMER_RESET, &Mil_timer);
}

double CNMatroxLib::GetTimer(long nID)
{
//	MappTimer(M_TIMER_READ, &Mil_timer);
	return Mil_timer;
}

