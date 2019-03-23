#pragma once

#define MORPH_OK  0
#define MORPH_ERROR_WIDTH  1


// CNMorphologyLib
class CNMorphologyLib : public CWnd
{
	DECLARE_DYNAMIC(CNMorphologyLib)

public:
	CNMorphologyLib();
	virtual ~CNMorphologyLib();

	// Binary 함수들 
	// White 증가한다.
	long DilateBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
    // White 감소한다.
    long ErodeBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// White 영역내에 있는 검은 점들이 없어진다.( Dilate + Erode)
	long CloseBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// White 바깥에 붙은 White들이 없어진다. (Erode + Dilate)
	long OpenBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
    // Gray 함수들 
	// 화면이 대체로 밝아 진다.
    long DilateGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// 화면이 대체로 어두워진다.
    long ErodeGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// 영상의 밝은 부분을 잇는 평면으로 Smoothing되는 효과가 있다
	// 즉 Black 노이즈가 없어진다.
	long CloseGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// 영상의 어두은 점들을 잇는 평면으로 Smoothing되는 효과가 있다.
	// 즉 White 노이즈가 없어진다.
	long OpenGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);

private:
	long m_Black;
	long m_White;

// fmSour : 소스 영상의 시작번지
// fmDest : 타겟 영상의 시작번지
// nWidth : 소스영상의 폭       (ex 640)
// nHeight : 소스영상의 높이     (ex 480)
// nPitch :  소스영상의 메모리 폭(ex 1024)
// pKernel : kernel의 시작번지
// nKernelSizeX: kernel의 X방향 크기
// nKernelSizeY: kernel의 Y방향 크기 

protected:
	DECLARE_MESSAGE_MAP()
};


