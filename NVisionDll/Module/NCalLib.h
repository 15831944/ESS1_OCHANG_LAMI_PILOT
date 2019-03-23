#pragma once
#define DEF_N   10

// CNCalLib

class CNCalLib : public CWnd
{
	DECLARE_DYNAMIC(CNCalLib)

public:
	CNCalLib();
	virtual ~CNCalLib();

	void        LoadCalData(CString strFileName);
    void        SaveCalData(CString strFileName);
	float       GetScaleFactor() {return m_fScaleFactor;}

	DEF_MAT*    CreateMat(long nRow, long nCol);
    void        DeleteMat(DEF_MAT *pMat);
    BOOL        InverseMat(DEF_MAT *a, DEF_MAT *b);
	BOOL        TransposeMat(DEF_MAT *a, DEF_MAT *b);
	BOOL        DotProductMat(DEF_MAT *a, DEF_MAT *b, DEF_MAT *c);
	BOOL        SetWarping(long nBlobCnt, double *fCenX, double *fCenY, long nWidth, long nHeight, long nRow, long nCol, float dGridSize);
	BOOL        SetWarping(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh, long nPol, long nRow, long nCol, float dGridSize);
	BOOL        TransWarpingImage(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
    BOOL        TransWarpingPos(float i, float j, float *x, float *y, long nWidth, long nHeight);
	double      GetDistance2D(double dX1, double dY1, double dX2, double dY2);

// Attributes
public:
	DEF_MAT *A_MAT, *B_MAT, *C_MAT;
	DEF_MAT *D_MAT, *F_MAT;
	DEF_MAT *T_MAT, *I_MAT;

	long	m_nGridColor;
	long	m_nMinArea;
	long	m_nMaxArea;

	float   m_fScaleFactor;
	double  m_fInverseA[DEF_N];
	double  m_fInverseB[DEF_N];
	double  m_fForwardA[DEF_N];
	double  m_fForwardB[DEF_N];
protected:
	DECLARE_MESSAGE_MAP()
};


