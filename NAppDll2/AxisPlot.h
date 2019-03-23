#if !defined(_AXISPLOT_H)
#define _AXISPLOT_H

// Control Tab Position
#define CONTROL_TAB_LEFT       10
#define CONTROL_TAB_TOP        10
#define CONTROL_TAB_RIGHT     300
#define CONTROL_TAB_BOTTOM    200
// Control Tab Position

// Control Frame Position
#define CONTROL_FRAME_LEFT     15
#define CONTROL_FRAME_TOP      30
#define CONTROL_FRAME_RIGHT   295
#define CONTROL_FRAME_BOTTOM  195
// Control Frame Position

typedef struct AXIS_PLOT_
{
	TCHAR     m_sTitleX[100];        // X Title String                
	TCHAR     m_sTitleY[100];        // Y Title String 
	CRect     m_rTitleX;             // X Title 위치  (내부적으로 정함)
	CRect     m_rTitleY;             // Y Title 위치  (내부적으로 정함)
	CRect     m_rClient;             // 프로파일 출력영역   
    CRect     m_rPlot;               // Plot의 위치   (내부적으로 정함)
	CRect     m_rChart;              // 범례의 위치   (내부적으로 정함)

	COLORREF  m_colorGridLine;       // Grid Line의 색     
	COLORREF  m_colorSliderLine;     // Slider Line의 색 
	COLORREF  m_colorBkClient;       // Background Client의 색 
	COLORREF  m_colorBkPlot;         // Background Plot의 색 
	COLORREF  m_colorTitleStr;       // Title의 색 
	COLORREF  m_colorPlotStr;        // Plot String의 색 
	COLORREF  m_colorUpLine;        // Plot String의 색 
	COLORREF  m_colorDownLine;        // Plot String의 색 
	COLORREF  m_colorDataLine;        // Plot String의 색 

    long      m_nLeftMargin;         // 왼쪽 여백. 
	long      m_nTopMargin;          // 상단 여백 
	long      m_nRightMargin;        // 오른쪽 여백 
	long      m_nBottomMargin;       // 아래쪽 여백 

	float     m_dScaleX;             // X Plot의 스케일
	float     m_dScaleY;             // Y Plot의 스케일

	long      m_nRangeMin;          // Range Min X프로파일 고정시 
	long 	  m_nRangeMax;          // Range Max X프로파일 고정시 

	long   	  m_nSlidePerPlot;      // Plot의 X Slide갯수 
	long 	  m_nPixelDistGrid;     // X Pixel갯수 -> 한개의 Grid

	long      m_nPlotStrSize;        // Plot String 폰트 Size  
	long      m_nTitleStrSize;       // X, Y Title String 폰트 Size 

	long      m_nScrollX;
    long      m_nZoomY;
	long      m_nBaseY;
} AXIS_PLOT;
// Graph와 데이타에 관련된 구조체 -------------------------------------------

#endif 