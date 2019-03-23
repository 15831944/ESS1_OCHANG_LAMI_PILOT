// NPlotDll.h : NPlotDll DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

// CNPlotDllApp
// 이 클래스의 구현을 보려면 NPlotDll.cpp를 참조하십시오.
//

// Plot를 초기화 / Open 함수 
// nPlotID : (0~14 지정가능) 
// pWnd : 출력하고자하는 윈도우 핸들 
extern "C" _declspec(dllexport) void    NPlotDll_Open(long nPlotID, long pWnd);
extern "C" _declspec(dllexport) void    NPlotDll_SetWindowPtr(long nID, long pWnd);

extern "C" _declspec(dllexport) void    NPlotDll_SetEvent(long nID, LPVOID lpCallBack, long pCallData);

// Plot를 종료 / Close 함수 
// nPlotID : (0~14 지정가능) 
extern "C" _declspec(dllexport) void    NPlotDll_Close(long nPlotID);

// 해당 Plot ID의 설정 화면 띄우기 
extern "C" _declspec(dllexport) void    NPlotDll_ShowSetupDialog(long nPlotID);

// 설정값 변경 후 화면 UPDATE(화면 다시 그리기)  
extern "C" _declspec(dllexport) void    NPlotDll_Invalidate(long nPlotID);

// 기존 설정값을 읽어온다
extern "C" _declspec(dllexport) void    NPlotDll_LoadSetupData(long nPlotID, LPCTSTR strPathName);

// 기존 PLOT DATA를 읽어온다
extern "C" _declspec(dllexport) BOOL    NPlotDll_LoadPlotData(long nPlotID, LPCTSTR strFullPathName);

// 현재 PLOT DATA를 저장한다.
extern "C" _declspec(dllexport) void    NPlotDll_SavePlotData(long nPlotID, LPCTSTR strFullPathName);

extern "C" _declspec(dllexport) void    NPlotDll_LoadStickData(long nID, LPCTSTR strFullPathName);
extern "C" _declspec(dllexport) void    NPlotDll_SaveStickData(long nID, LPCTSTR strFullPathName);

// 현재 PLOT IMAGE DATA FILE로 저장한다.
extern "C" _declspec(dllexport) void    NPlotDll_SaveImageData(long nID, LPCTSTR strFullPathName);

// nPlotI의 모든 데이타를 초기화한다. (처음부터 다시 시작할 때 수행함)
extern "C" _declspec(dllexport) void    NPlotDll_InitialData(long nPlotID);

// TY MODE에서만 작동되며, 해당 AXIS ID(해당 Remark)에서 
// X Data는 시간으로 내부 타이머를 적용하고, Y Data만 입력함
extern "C" _declspec(dllexport) void    NPlotDll_AppendDataTY(long nPlotID, long nAxisID, float fDataY);

// XY MODE에서만 작동되며, 해당 AXIS ID(해당 Remark)에서 
// X Data, Y Data를 입력함
extern "C" _declspec(dllexport) void    NPlotDll_AppendDataXY(long nPlotID, long nAxisID, float fDataX, float fDataY);

extern "C" _declspec(dllexport) void    NPlotDll_SetMaxDataNum(long nPlotID, long nDataNum);
extern "C" _declspec(dllexport) void    NPlotDll_AppendStickData(long nPlotID, long nAxisID, float fDataX, float fDataY);

// XY MODE에서만 작동되며, 해당 AXIS ID(해당 Remark)에서 
// X Data, Y Data를 데이타의 수 nDataNum갯수만큼 한번에 입력함
extern "C" _declspec(dllexport) void    NPlotDll_SetData(long nPlotID, long nAxisID, long nDataNum, float *fDataX, float *fDataY);
extern "C" _declspec(dllexport) void    NPlotDll_SetPrecisionXY(long nPlotID, long nPrecisionX, long nPrecisionY);

extern "C" _declspec(dllexport) void    NPlotDll_SetGausianData(long nPlotID, float fLSL, float fUSL, long nDataNum, float *fDataX, float *fDataY);
//// 아래 설정값 변경 후 PLOT의 화면 UPDATE는 
//// NPlotDll_Invalidate(long nID)함수를 사용하여 다시 그려야 합니다.

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      가로 X 설정값 변경 함수      //////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// 가로 X CAPTION TITLE 명칭 
extern "C" _declspec(dllexport) void    NPlotDll_SetCaptionX(long nPlotID, LPCTSTR strCaption);

// 가로 X CAPTION TITLE 색상 (COLORREF) ->(long)으로 형변환  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorCaptionX(long nPlotID, long nColor);

// 가로 X CAPTION TITLE 폰트(LOGFONT)
extern "C" _declspec(dllexport) void    NPlotDll_SetFontCaptionX(long nPlotID, LOGFONT nFont);

// 가로 X CAPTION TITLE 명칭 위치지정
extern "C" _declspec(dllexport) void    NPlotDll_SetPosCaptionX(long nPlotID, long x, long y);

// 가로 X UNIT 단위 명칭  
extern "C" _declspec(dllexport) void    NPlotDll_SetUnitCaptionX(long nPlotID, LPCTSTR strCaption);

// 가로 X UNIT 단위 명칭 위치지정  
extern "C" _declspec(dllexport) void    NPlotDll_SetPosUnitCaptionX(long nPlotID, long x, long y);

// 가로 X SLIDE 수 지정  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideNumX(long nPlotID, long nNum);

// 가로 X SLIDE 라인 STILE지정  
// PS_SOLID 0, PS_DASH 1, PS_DOT 2, PS_DASHDOT 3, PS_DASHDOTDOT 4, PS_NULL 5
extern "C" _declspec(dllexport) void    NPlotDll_SetPenStileSlideX(long nPlotID, long nPenStile);

// 가로 X SLIDE 라인 색상 (COLORREF) ->(long)으로 형변환  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorSlideX(long nPlotID, long nColor);

// 가로 X의 테두리 SLIDE 라인의 두께 지정  
extern "C" _declspec(dllexport) void    NPlotDll_SetOutsideThickSlideX(long nPlotID, long nThick);

// 가로 X의 테두리 SLIDE 라인의 색상 지정 (COLORREF) ->(long)으로 형변환   
extern "C" _declspec(dllexport) void    NPlotDll_SetOutsideColorSlideX(long nPlotID, long nColor);

// 좌측 SLIDE MARGIN 지정(OFFSET 값)  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideMarginL(long nPlotID, long nMargin);

// 우측 SLIDE MARGIN 지정(OFFSET 값)  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideMarginR(long nPlotID, long nMargin);

// 가로 SCROLL BAR 위치지정(Y 위치설정)  
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScrollBarX(long nPlotID, long nY);

// 가로 SCROLL 위치지정 - X1(0.0%~100.0%), X2(0.0%~100.0%) 위치설정
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScrollX(long nID, float fPosL, float fPosR);

// 가로 SCROLL BAR ENABLE(ON=1/OFF=0)  
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableScrollBarX(long nPlotID, BOOL bShow);

// 가로 SCROLL BAR 바탕 색상 지정 (COLORREF) ->(long)으로 형변환   
extern "C" _declspec(dllexport) void    NPlotDll_SetBColorScrollBarX(long nPlotID, long nColor);

// 가로 SCROLL BAR 전면 색상 지정 (COLORREF) ->(long)으로 형변환   
extern "C" _declspec(dllexport) void    NPlotDll_SetFColorScrollBarX(long nPlotID, long nColor);

// 가로 SCALE TITLE(눈금 표시)의 X, Y의 위치지정   
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScaleCaptionX(long nPlotID, long nX, long nY);

// 가로 SCALE CAPTION 색상 지정 (COLORREF) ->(long)으로 형변환   
extern "C" _declspec(dllexport) void    NPlotDll_SetColorScaleCaptionX(long nPlotID, long nColor);

// 가로 SCALE CAPTION FONT 지정(LOGFONT)  
extern "C" _declspec(dllexport) void    NPlotDll_SetFontScaleCaptionX(long nPlotID, LOGFONT nfont);

// 가로 X PLOT의 초기 RANGE지정 
// dMinRange~dMaxRange : -1000000.0~1000000.0
extern "C" _declspec(dllexport) void    NPlotDll_SetRangeX(long nPlotID, double dMinRange, double dMaxRange);

// 가로 X PLOT의 초기 Display RANGE지정 
// dDispRange : 0~1000000.0
extern "C" _declspec(dllexport) void    NPlotDll_SetDispRangeX(long nPlotID, double dDispRange);
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      가로 X 설정값 변경 함수      //////////////////////////
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      세로 Y 설정값 변경 함수      //////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// 세로 Y CAPTION TITLE 명칭 
extern "C" _declspec(dllexport) void    NPlotDll_SetCaptionY(long nPlotID, LPCTSTR strCaption);

// 세로 Y CAPTION TITLE 색상 (COLORREF) ->(long)으로 형변환  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorCaptionY(long nPlotID, long nColor);

// 세로 Y CAPTION TITLE 폰트(LOGFONT)
extern "C" _declspec(dllexport) void    NPlotDll_SetFontCaptionY(long nPlotID, LOGFONT nFont);

// 세로 Y CAPTION TITLE 명칭 위치지정
extern "C" _declspec(dllexport) void    NPlotDll_SetPosCaptionY(long nPlotID, long x, long y);

// 세로 Y UNIT 단위 명칭  
extern "C" _declspec(dllexport) void    NPlotDll_SetUnitCaptionY(long nPlotID, LPCTSTR strCaption);

// 세로 Y UNIT 단위 명칭 위치지정  
extern "C" _declspec(dllexport) void    NPlotDll_SetPosUnitCaptionY(long nPlotID, long x, long y);

// 세로 Y SLIDE 수 지정  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideNumY(long nPlotID, long nNum);

// 세로 Y SLIDE 라인 STILE지정  
// PS_SOLID 0, PS_DASH 1, PS_DOT 2, PS_DASHDOT 3, PS_DASHDOTDOT 4, PS_NULL 5
extern "C" _declspec(dllexport) void    NPlotDll_SetPenStileSlideY(long nPlotID, long nPenStile);

// 세로 Y SLIDE 라인 색상 (COLORREF) ->(long)으로 형변환  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorSlideY(long nPlotID, long nColor);

// 세로 Y의 테두리 SLIDE 라인의 두께 지정  
extern "C" _declspec(dllexport) void    NPlotDll_SetOutsideThickSlideY(long nPlotID, long nThick);

// 세로 Y의 테두리 SLIDE 라인의 색상 지정 (COLORREF) ->(long)으로 형변환   
extern "C" _declspec(dllexport) void    NPlotDll_SetOutsideColorSlideY(long nPlotID, long nColor);

// 상측 SLIDE MARGIN 지정(OFFSET 값)  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideMarginT(long nPlotID, long nMargin);

// 하측 SLIDE MARGIN 지정(OFFSET 값)  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideMarginB(long nPlotID, long nMargin);

// 세로 SCROLL BAR 위치지정(X 위치설정)  
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScrollBarY(long nPlotID, long nX);

// 세로 SCROLL 위치지정 - Y1(0.0%~100.0%), Y2(0.0%~100.0%) 위치설정
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScrollY(long nID, float fPosT, float fPosB);

// 세로 SCROLL BAR ENABLE(ON=1/OFF=0)  
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableScrollBarY(long nPlotID, BOOL bShow);

// 세로 SCROLL BAR 바탕 색상 지정 (COLORREF) ->(long)으로 형변환   
extern "C" _declspec(dllexport) void    NPlotDll_SetBColorScrollBarY(long nPlotID, long nColor);

// 세로 SCROLL BAR 전면 색상 지정 (COLORREF) ->(long)으로 형변환   
extern "C" _declspec(dllexport) void    NPlotDll_SetFColorScrollBarY(long nPlotID, long nColor);

// 세로 SCALE TITLE(눈금표시)의 X, Y의 위치지정 (Left Scale의 위치)
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScaleCaptionLY(long nPlotID, long nX, long nY);

// 세로 SCALE TITLE(눈금표시)의 X, Y의 위치지정 (Right Scale의 위치)
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScaleCaptionRY(long nPlotID, long nX, long nY);

// 세로 RIGHT SCALE ENABLE(ON=1/OFF=0)   
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableRScale(long nPlotID, BOOL bShow);

// 세로 SCALE CAPTION 색상 지정 (COLORREF) ->(long)으로 형변환    
extern "C" _declspec(dllexport) void    NPlotDll_SetColorScaleCaptionY(long nPlotID, long nColor);

// 세로 SCALE CAPTION FONT 지정(LOGFONT)  
extern "C" _declspec(dllexport) void    NPlotDll_SetFontScaleCaptionY(long nPlotID, LOGFONT nfont);

// 세로 Left Scale Range Y PLOT의 초기 RANGE지정 
// dMinRange~dMaxRange : -1000000.0~1000000.0
extern "C" _declspec(dllexport) void    NPlotDll_SetRangeLY(long nPlotID, double dMinRange, double dMaxRange);

// 세로 Right Scale Range Y PLOT의 초기 RANGE지정 
// dMinRange~dMaxRange : -1000000.0~1000000.0
extern "C" _declspec(dllexport) void    NPlotDll_SetRangeRY(long nPlotID, double dMinRange, double dMaxRange);
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      세로 Y 설정값 변경 함수      //////////////////////////
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     PLOT(공통) 설정값 변경 함수     /////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// PLOT MODE 지정
// 0:XY MODE, 1:TY MODE
extern "C" _declspec(dllexport) void    NPlotDll_SetPlotMode(long nPlotID, long nMode);

// Remark(범례) 표시 위치지정
// nVertHori : 0(VERT.), 1(HORI.)
// nLoc : 0(LEFT Or Top), 1(Right Or Bottom)
extern "C" _declspec(dllexport) void    NPlotDll_SetLocRemark(long nPlotID, long nVertHori, long nLoc);

// Remark(범례) 표시구간(Split)의 크기를 지정
extern "C" _declspec(dllexport) void    NPlotDll_SetSizeSplitRemark(long nPlotID, long nSize);

// Plot 영역의 바탕색을 지정 (COLORREF) ->(long)으로 형변환  
extern "C" _declspec(dllexport) void    NPlotDll_SetBColorPlot(long nPlotID, long nColor);

// Remark 영역의 바탕색을 지정 (COLORREF) ->(long)으로 형변환  
extern "C" _declspec(dllexport) void    NPlotDll_SetBColorRemark(long nPlotID, long nColor);

// Remark 영역의 전면색을 지정 (COLORREF) ->(long)으로 형변환  
extern "C" _declspec(dllexport) void    NPlotDll_SetFColorRemark(long nPlotID, long nColor);

// Remark의 Font를 지정
extern "C" _declspec(dllexport) void    NPlotDll_SetFontRemark(long nPlotID, LOGFONT nFont);

// Remark 개수를 지정
extern "C" _declspec(dllexport) void    NPlotDll_SetRemarkNum(long nPlotID, long nNum);

// Remark들 사이의 간격을 지정
extern "C" _declspec(dllexport) void    NPlotDll_SetSpaceRemark(long nPlotID, long nDist);

// Remark들 사이의 간격을 지정
extern "C" _declspec(dllexport) void    NPlotDll_SetPosRemark(long nPlotID, long nX, long nY);

// nAxisID : Remark의 개별 TITLE를 지정
extern "C" _declspec(dllexport) void    NPlotDll_SetCaptionRemark(long nPlotID, long nAxisID, LPCTSTR strCaption);

// Remark의 개별 COLOR를 지정 (COLORREF) ->(long)으로 형변환  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorRemark(long nPlotID, long nAxisID, long nColor);

// nAxisID : Remark 개별 그래프의 펜 스타일을 지정
// nPen : PS_SOLID 0, PS_DASH 1, PS_DOT 2, PS_DASHDOT 3, PS_DASHDOTDOT 4, PS_NULL 5
extern "C" _declspec(dllexport) void    NPlotDll_SetPenStileRemark(long nPlotID, long nAxisID, long nPen);

// nAxisID : Remark 개별 그래프의 Left, Right스케일을 지정
// nSel : 0(Left Scale), 1(Right Scale)
extern "C" _declspec(dllexport) void    NPlotDll_SetSelScaleRemark(long nPlotID, long nAxisID, long nSel);

// bOnOff : 가로 Referance Level의 Line를 활성화(TRUE) 또는 비활성화(FALSE)
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableHoriRefLine(long nPlotID, BOOL bOnOff);

// bOnOff : 세로 Referance Level의 Line를 활성화(TRUE) 또는 비활성화(FALSE)
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableVertRefLine(long nPlotID, BOOL bOnOff);

// nPosX1, nPosX2 : 두 개의 수직 라인이 있으므로 두 라인의 X위치 
// nPosX1는 왼쪽 라인, nPosX2는 오른쪽 라인
extern "C" _declspec(dllexport) void    NPlotDll_SetPosVertRefLine(long nID, BOOL bMode, float fPosX1, float fPosX2);

// nPosY1, nPosY2 : 두 개의 수평 라인이 있으므로 두 라인의 Y위치 
// nPosY1는 상부쪽 라인, nPosY2는 하부쪽 라인
extern "C" _declspec(dllexport) void    NPlotDll_SetPosHoriRefLine(long nID, BOOL bMode, float fPosY1, float fPosY2);

// Referance Line의 펜 스타일을 지정
// nPen : PS_SOLID 0, PS_DASH 1, PS_DOT 2, PS_DASHDOT 3, PS_DASHDOTDOT 4, PS_NULL 5
extern "C" _declspec(dllexport) void    NPlotDll_SetPenStileRefLine(long nPlotID, long nPen);

// Referance Line의 색상 지정 (COLORREF) ->(long)으로 형변환 
extern "C" _declspec(dllexport) void    NPlotDll_SetColorRefLine(long nPlotID, long nColor);

// nAxisID : 선택한 Remark(AXIS ID)를 X PLOT으로 다른 Remark(다른 AXIS)를 Y PLOT과 함께 비교하기 위한
//           X PLOT로 지정된 Remark 항목 
extern "C" _declspec(dllexport) void    NPlotDll_SetSelXAxisRemark(long nPlotID, long nAxisID);
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     PLOT(공통) 설정값 변경 함수     /////////////////////////
////////////////////////////////////////////////////////////////////////////////////


class CNPlotDllApp : public CWinApp
{
public:
	CNPlotDllApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
