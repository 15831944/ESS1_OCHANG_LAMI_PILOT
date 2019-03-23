// CNCalLib.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NCalLib.h"
#include "NBlobDll.h"
#include "math.h"

// CNCalLib

IMPLEMENT_DYNAMIC(CNCalLib, CWnd)

CNCalLib::CNCalLib()
{
    A_MAT=B_MAT=C_MAT=D_MAT=F_MAT=NULL;
	T_MAT=I_MAT=NULL;

	m_nGridColor = BLACK_LEVEL;
	m_nMinArea =  100;
	m_nMaxArea = 50000;
	m_fScaleFactor = 1.0f;

	for(long i=0; i<DEF_N; i++)
	{
		m_fInverseA[i] = 0.0f;
		m_fInverseB[i] = 0.0f;

		m_fForwardA[i] = 0.0f;
		m_fForwardB[i] = 0.0f;
	}
}

CNCalLib::~CNCalLib()
{

}

BEGIN_MESSAGE_MAP(CNCalLib, CWnd)
END_MESSAGE_MAP()



// CNCalLib 메시지 처리기입니다.

/////////////////////////////////////////////////////////////////////////////
void CNCalLib::LoadCalData(CString strFileName)
{
	CFile f;
	long i;

	if (f.Open(strFileName, CFile::modeRead)) 
	{
		for(i=0; i<DEF_N; i++)
		{
		    f.Read(&m_fInverseA[i], sizeof(double));
	    	f.Read(&m_fInverseB[i], sizeof(double));
		    f.Read(&m_fForwardA[i], sizeof(double));
	    	f.Read(&m_fForwardB[i], sizeof(double));
		}
	  
		f.Read(&m_fScaleFactor, sizeof(float));
		f.Close();
	}
}

void CNCalLib::SaveCalData(CString strFileName)
{
	CFile f;
	long i;

	if (f.Open(strFileName, CFile::modeCreate|CFile::modeWrite)) 
	{
		for(i=0; i<DEF_N; i++)
		{
		    f.Write(&m_fInverseA[i], sizeof(double));
	    	f.Write(&m_fInverseB[i], sizeof(double));
		    f.Write(&m_fForwardA[i], sizeof(double));
	    	f.Write(&m_fForwardB[i], sizeof(double));
		}

		f.Write(&m_fScaleFactor, sizeof(float));
		f.Close();
	}
}

DEF_MAT* CNCalLib::CreateMat(long nRow, long nCol)
{
	DEF_MAT *pMat = NULL;

	if (nRow>0 && nCol>0)
	{
    	pMat = new DEF_MAT;

		pMat->m_nRow = nRow;
		pMat->m_nCol = nCol;
		pMat->m_pMat = new double[nRow*nCol];
	}

	return pMat;
}

void CNCalLib::DeleteMat(DEF_MAT *pMat)
{
	if (pMat!=NULL)
	{
		delete pMat->m_pMat;
		delete pMat;
		pMat=NULL;
	}
}

BOOL CNCalLib::InverseMat(DEF_MAT *a, DEF_MAT *b)
{
	BOOL bRet=TRUE;
	long  i, j, k, l, l1, n;
	double c, d;
   	double SMALLEST=0.00000001;

	if (a->m_nRow==a->m_nCol && b->m_nRow==b->m_nCol && a->m_nRow==b->m_nRow)
	{
		n = a->m_nRow;
		for(i=0; i<n; i++) 
		for(j=0; j<n; j++)
		{
			 b->m_pMat[i*n+j] = 0.;
			 b->m_pMat[i*n+i] = 1.0;
		}

		for(l=0; l<n; l++)
		{
			 d = fabs(a->m_pMat[l*n+l]);
			 if (d<SMALLEST) 
			 {
				 l1 = l;
				 d = fabs(a->m_pMat[l1*n+l]);
				 while (d < SMALLEST && (++l1<n))
					 d = fabs(a->m_pMat[l1*n+l]);
				 if (l1>=n)	 return TRUE;

				 for(j=0; j<n; j++)
				 {
					 a->m_pMat[l*n+j] += (a->m_pMat[l1*n+j]);
					 b->m_pMat[l*n+j] += (b->m_pMat[l1*n+j]);
				 }
			 }

			 c = 1.0/(a->m_pMat[l*n+l]);
			 for(j=l;j<n;j++) a->m_pMat[l*n+j] *= c;
			 for(j=0;j<n;j++) b->m_pMat[l*n+j] *= c;

			 k = l+1;
			 for(i=k; i<n; i++)
			 {
				 c = a->m_pMat[i*n+l];
				 for (j=l; j<n; j++) a->m_pMat[i*n+j] -= (a->m_pMat[l*n+j] * c);
				 for (j=0; j<n; j++) b->m_pMat[i*n+j] -= (b->m_pMat[l*n+j] * c);
			 }
		}

		for(l=n-1; l>=0; l--)
		for(k=1; k<=l; k++)
		{
			 i = l-k;
			 c = a->m_pMat[i*n+l];
			 for (j=l;j<n;j++) a->m_pMat[i*n+j] -= (a->m_pMat[l*n+j] * c);
			 for (j=0;j<n;j++) b->m_pMat[i*n+j] -= (b->m_pMat[l*n+j] * c);
		}

		bRet=FALSE;
	}

	return bRet;
}

BOOL CNCalLib::TransposeMat(DEF_MAT *a, DEF_MAT *b)
{
	BOOL bRet=TRUE;
	long i, j;

	if (a->m_nRow<=b->m_nCol && a->m_nCol<=b->m_nRow)
	{
		for(i=0; i<a->m_nRow; i++)
		for(j=0; j<a->m_nCol; j++)
			b->m_pMat[j*b->m_nCol + i] = a->m_pMat[i*a->m_nCol + j];

		bRet=FALSE;
	}

	return bRet;
}

BOOL CNCalLib::DotProductMat(DEF_MAT *a, DEF_MAT *b, DEF_MAT *c) 
{
	BOOL bRet=TRUE;
	long i, j, k;
	double dSum;

	if (a->m_nCol<=b->m_nRow && a->m_nRow<=c->m_nRow && 
		b->m_nCol<=c->m_nCol)
	{
		for(i=0; i<a->m_nRow; i++)
		for(j=0; j<b->m_nCol; j++)
		{
			dSum = 0.0f;
			for(k=0; k<a->m_nCol; k++)
				dSum += (a->m_pMat[i*a->m_nCol + k] * b->m_pMat[k*b->m_nCol + j]);

			c->m_pMat[i*c->m_nCol + j] = dSum;
		}

		bRet=FALSE;
	}

	return bRet;
}

BOOL CNCalLib::SetWarping(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh, long nPolColor, long nRow, long nCol, float dGridSize)
{
	BOOL bRet=TRUE;
	long  i, j, k;
	long  nBlobCnt, nRealCnt;
	float fScale, fColDist, fRowDist;
	double a, b, dDist, dMin, dX, dY;
	LPBYTE pBinaryBuff;
	POINT2 ptS[4], ptT[4];

	// 캘리브레이션 GRID마크 위치 찾기 
	BLOB_STRUCT *pBlob;
	pBinaryBuff = new unsigned char[nWidth*nHeight];

	NBlobDll_Open();

	m_nGridColor = nPolColor;
	NBlobDll_BinarizeProc(pImage, pBinaryBuff, x1, y1, x2, y2, nWidth, nHeight, nThresh);
	pBlob = NBlobDll_BlobAnalysis(pBinaryBuff, x1, y1, x2, y2, nWidth, nHeight, m_nMinArea, m_nMaxArea, m_nGridColor);
	nBlobCnt = NBlobDll_GetBlobCount();
	// 캘리브레이션 GRID마크 위치 찾기 

	nRealCnt = nRow*nCol;
    if (nBlobCnt==nRealCnt && nRow>2 && nCol>2)
	{
		// 스케일 FACTOR구하기 
		ptS[0].x = (float)x1;   ptS[0].y = (float)y1;
		ptS[1].x = (float)x2;   ptS[1].y = (float)y1;
		ptS[2].x = (float)x1;   ptS[2].y = (float)y2;
		ptS[3].x = (float)x2;   ptS[3].y = (float)y2;

		for(i=0; i<4; i++)
		{
		    dMin = 10000.0f;
			for(j=0; j<nBlobCnt; j++)
			{
				dDist = GetDistance2D(ptS[i].x, ptS[i].y, pBlob[j].m_fCenX, pBlob[j].m_fCenY);
				if (dDist<dMin)
				{
					ptT[i].x = pBlob[j].m_fCenX;
					ptT[i].y = pBlob[j].m_fCenY;
					dMin = dDist;
				}
			}
		}

		a = GetDistance2D(ptT[0].x, ptT[0].y, ptT[2].x, ptT[2].y);
		b = GetDistance2D(ptT[1].x, ptT[1].y, ptT[3].x, ptT[3].y);
		fRowDist = (float)(a + b)/2.0f;
		fRowDist /= (nRow-1);

		a = GetDistance2D(ptT[0].x, ptT[0].y, ptT[1].x, ptT[1].y);
		b = GetDistance2D(ptT[2].x, ptT[2].y, ptT[3].x, ptT[3].y);
		fColDist = (float)(a + b)/2.0f;
		fColDist /= (nCol-1);

		fScale = (fRowDist+fColDist)/2.0f;
		if (fScale>0.0f)
	    	m_fScaleFactor = dGridSize/fScale;
		// 스케일 FACTOR구하기 

		// 매트릭스 데이타 할당 
 	    A_MAT = CreateMat(nRealCnt, 2);
	    B_MAT = CreateMat(nRealCnt, DEF_N);
		C_MAT = CreateMat(DEF_N, DEF_N);
		D_MAT = CreateMat(DEF_N, nRealCnt);
	    T_MAT = CreateMat(DEF_N, nRealCnt);
		I_MAT = CreateMat(DEF_N, DEF_N);
		F_MAT = CreateMat(DEF_N, 2);
		// 매트릭스 데이타 할당 

		// INVERSE MAPPING을 위한 FACTOR구하기  
		nBlobCnt = 0;
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			ptS[0].x = ptT[0].x + j*fColDist;
			ptS[0].y = ptT[0].y + i*fRowDist;

			dMin = 10000.0f;
			for(k=0; k<nRealCnt; k++)
			{
				dDist = GetDistance2D(pBlob[k].m_fCenX, pBlob[k].m_fCenY, ptS[0].x, ptS[0].y);
				if (dDist<dMin)
				{
					A_MAT->m_pMat[nBlobCnt*2 + 0] = pBlob[k].m_fCenX;
					A_MAT->m_pMat[nBlobCnt*2 + 1] = pBlob[k].m_fCenY;
					dMin = dDist;
				}
			}

			nBlobCnt++;
		}

		nBlobCnt = 0;
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			dX = ptT[0].x + j*fScale;
			dY = ptT[0].y + i*fScale;

			B_MAT->m_pMat[nBlobCnt*DEF_N + 0] = 1;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 1] = dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 2] = dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 3] = dX*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 4] = dX*dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 5] = dY*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 6] = dX*dX*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 7] = dX*dY*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 8] = dX*dX*dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 9] = dY*dY*dY;

			nBlobCnt++;
		}

		TransposeMat(B_MAT, T_MAT);
	    DotProductMat(T_MAT, B_MAT, C_MAT);
	    InverseMat(C_MAT, I_MAT);
		DotProductMat(I_MAT, T_MAT, D_MAT);
		DotProductMat(D_MAT, A_MAT, F_MAT);

		for(i=0; i<DEF_N; i++)
		{
			m_fInverseA[i] = F_MAT->m_pMat[i*2 + 0];
			m_fInverseB[i] = F_MAT->m_pMat[i*2 + 1];
		}
		// INVERSE MAPPING을 위한 FACTOR구하기  

		// FORWARD MAPPING을 위한 FACTOR구하기  
		nBlobCnt = 0;
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			A_MAT->m_pMat[nBlobCnt*2 + 0] = ptT[0].x + j*fScale;
			A_MAT->m_pMat[nBlobCnt*2 + 1] = ptT[0].y + i*fScale;
			nBlobCnt++;
		}

		nBlobCnt = 0;
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			ptS[0].x = ptT[0].x + j*fColDist;
			ptS[0].y = ptT[0].y + i*fRowDist;

			dMin = 10000.0f;
			for(k=0; k<nRealCnt; k++)
			{
			    dDist = GetDistance2D(pBlob[k].m_fCenX, pBlob[k].m_fCenY, ptS[0].x, ptS[0].y);
				if (dDist<dMin)
				{
					B_MAT->m_pMat[nBlobCnt*2 + 0] = pBlob[k].m_fCenX;
					B_MAT->m_pMat[nBlobCnt*2 + 1] = pBlob[k].m_fCenY;
					dMin = dDist;
				}
			}

			nBlobCnt++;
		}

		for(i=0; i<nRealCnt; i++)
		{
			pBlob[i].m_fCenX = (float)B_MAT->m_pMat[i*2 + 0];
			pBlob[i].m_fCenY = (float)B_MAT->m_pMat[i*2 + 1];
        }

		nBlobCnt = 0;
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			dX = pBlob[nBlobCnt].m_fCenX;
			dY = pBlob[nBlobCnt].m_fCenY;

			B_MAT->m_pMat[nBlobCnt*DEF_N + 0] = 1;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 1] = dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 2] = dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 3] = dX*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 4] = dX*dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 5] = dY*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 6] = dX*dX*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 7] = dX*dY*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 8] = dX*dX*dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 9] = dY*dY*dY;

			nBlobCnt++;
		}

		TransposeMat(B_MAT, T_MAT);
	    DotProductMat(T_MAT, B_MAT, C_MAT);
	    InverseMat(C_MAT, I_MAT);
		DotProductMat(I_MAT, T_MAT, D_MAT);
		DotProductMat(D_MAT, A_MAT, F_MAT);

		for(i=0; i<DEF_N; i++)
		{
			m_fForwardA[i] = F_MAT->m_pMat[i*2 + 0];
			m_fForwardB[i] = F_MAT->m_pMat[i*2 + 1];
		}
		// FORWARD MAPPING을 위한 FACTOR구하기  

		DeleteMat(A_MAT);
		DeleteMat(B_MAT);
		DeleteMat(C_MAT);
		DeleteMat(D_MAT);
		DeleteMat(F_MAT);
		DeleteMat(T_MAT);
		DeleteMat(I_MAT);

		bRet = FALSE;
	}

	NBlobDll_Close();
	delete pBinaryBuff;

	return bRet;
}

BOOL CNCalLib::SetWarping(long nBlobCnt, double *fCenX, double *fCenY, long nWidth, long nHeight, long nRow, long nCol, float dGridSize)
{
	BOOL bRet=TRUE;
	long  i, j, k, nRealCnt;
	float fScale, fColDist, fRowDist;
	double a, b, dDist, dMin, dX, dY;
	POINT2 ptS[4], ptT[4];

	nRealCnt = nRow*nCol;
    if (nBlobCnt==nRealCnt && nRow>2 && nCol>2)
	{
		// 스케일 FACTOR구하기 
		ptS[0].x = (float)0;      ptS[0].y = (float)0;
		ptS[1].x = (float)nWidth; ptS[1].y = (float)0;
		ptS[2].x = (float)0;      ptS[2].y = (float)nHeight;
		ptS[3].x = (float)nWidth; ptS[3].y = (float)nHeight;

		for(i=0; i<4; i++)
		{
		    dMin = 10000.0;
			for(j=0; j<nBlobCnt; j++)
			{
				dDist = GetDistance2D(ptS[i].x, ptS[i].y, fCenX[j], fCenY[j]);
				if (dDist<dMin)
				{
					ptT[i].x = (float)fCenX[j];
					ptT[i].y = (float)fCenY[j];
					dMin = dDist;
				}
			}
		}

		a = GetDistance2D(ptT[0].x, ptT[0].y, ptT[2].x, ptT[2].y);
		b = GetDistance2D(ptT[1].x, ptT[1].y, ptT[3].x, ptT[3].y);
		fRowDist = (float)(a + b)/2.0f;
		fRowDist /= (nRow-1);

		a = GetDistance2D(ptT[0].x, ptT[0].y, ptT[1].x, ptT[1].y);
		b = GetDistance2D(ptT[2].x, ptT[2].y, ptT[3].x, ptT[3].y);
		fColDist = (float)(a + b)/2.0f;
		fColDist /= (nCol-1);

		fScale = (fRowDist+fColDist)/2.0f;
		if (fScale>0.0f)
	    	m_fScaleFactor = dGridSize/fScale;
		// 스케일 FACTOR구하기 

		// 매트릭스 데이타 할당 
 	    A_MAT = CreateMat(nRealCnt, 2);
	    B_MAT = CreateMat(nRealCnt, DEF_N);
		C_MAT = CreateMat(DEF_N, DEF_N);
		D_MAT = CreateMat(DEF_N, nRealCnt);
	    T_MAT = CreateMat(DEF_N, nRealCnt);
		I_MAT = CreateMat(DEF_N, DEF_N);
		F_MAT = CreateMat(DEF_N, 2);
		// 매트릭스 데이타 할당 

		// INVERSE MAPPING을 위한 FACTOR구하기  
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			ptS[0].x = ptT[0].x + j*fColDist;
			ptS[0].y = ptT[0].y + i*fRowDist;

			dMin = 10000.0f;
			for(k=0; k<nRealCnt; k++)
			{
				dDist = GetDistance2D(fCenX[k], fCenY[k], ptS[0].x, ptS[0].y);
				if (dDist<dMin)
				{
					A_MAT->m_pMat[nBlobCnt*2 + 0] = fCenX[k];
					A_MAT->m_pMat[nBlobCnt*2 + 1] = fCenY[k];
					dMin = dDist;
				}
			}
		}

		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			dX = ptT[0].x + j*fScale;
			dY = ptT[0].y + i*fScale;

			B_MAT->m_pMat[nBlobCnt*DEF_N + 0] = 1;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 1] = dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 2] = dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 3] = dX*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 4] = dX*dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 5] = dY*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 6] = dX*dX*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 7] = dX*dY*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 8] = dX*dX*dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 9] = dY*dY*dY;
		}

		TransposeMat(B_MAT, T_MAT);
	    DotProductMat(T_MAT, B_MAT, C_MAT);
	    InverseMat(C_MAT, I_MAT);
		DotProductMat(I_MAT, T_MAT, D_MAT);
		DotProductMat(D_MAT, A_MAT, F_MAT);

		for(i=0; i<DEF_N; i++)
		{
			m_fInverseA[i] = F_MAT->m_pMat[i*2 + 0];
			m_fInverseB[i] = F_MAT->m_pMat[i*2 + 1];
		}
		// INVERSE MAPPING을 위한 FACTOR구하기  

		// FORWARD MAPPING을 위한 FACTOR구하기  
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			A_MAT->m_pMat[nBlobCnt*2 + 0] = ptT[0].x + j*fScale;
			A_MAT->m_pMat[nBlobCnt*2 + 1] = ptT[0].y + i*fScale;
		}

		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			ptS[0].x = ptT[0].x + j*fColDist;
			ptS[0].y = ptT[0].y + i*fRowDist;

			dMin = 10000.0f;
			for(k=0; k<nRealCnt; k++)
			{
                dDist = GetDistance2D(fCenX[k], fCenY[k], ptS[0].x, ptS[0].y);
				if (dDist<dMin)
				{
					B_MAT->m_pMat[nBlobCnt*2 + 0] = fCenX[k];
					B_MAT->m_pMat[nBlobCnt*2 + 1] = fCenY[k];
					dMin = dDist;
				}
			}
		}

		nBlobCnt = 0;
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			dX = fCenX[nBlobCnt];
			dY = fCenY[nBlobCnt];

			B_MAT->m_pMat[nBlobCnt*DEF_N + 0] = 1;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 1] = dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 2] = dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 3] = dX*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 4] = dX*dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 5] = dY*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 6] = dX*dX*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 7] = dX*dY*dY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 8] = dX*dX*dX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 9] = dY*dY*dY;

			nBlobCnt++;
		}

		TransposeMat(B_MAT, T_MAT);
	    DotProductMat(T_MAT, B_MAT, C_MAT);
	    InverseMat(C_MAT, I_MAT);
		DotProductMat(I_MAT, T_MAT, D_MAT);
		DotProductMat(D_MAT, A_MAT, F_MAT);

		for(i=0; i<DEF_N; i++)
		{
			m_fForwardA[i] = F_MAT->m_pMat[i*2 + 0];
			m_fForwardB[i] = F_MAT->m_pMat[i*2 + 1];
		}
		// FORWARD MAPPING을 위한 FACTOR구하기  

		DeleteMat(A_MAT);
		DeleteMat(B_MAT);
		DeleteMat(C_MAT);
		DeleteMat(D_MAT);
		DeleteMat(F_MAT);
		DeleteMat(T_MAT);
		DeleteMat(I_MAT);

		bRet = FALSE;
	}

	return bRet;
}

double CNCalLib::GetDistance2D(double dX1, double dY1, double dX2, double dY2)
{
	double a, b;

	a = dX2-dX1;
	b = dY2-dY1;

	return sqrt(a*a + b*b);
}

BOOL CNCalLib::TransWarpingImage(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	BOOL bRet=FALSE;
    long i, j, x, y;

	for(i=y1; i<y2; i++)
	for(j=x1; j<x2; j++)
	{
		x = (long)((m_fInverseA[0] + m_fInverseA[1]*j + m_fInverseA[2]*i + m_fInverseA[3]*i*j + 
			        m_fInverseA[4]*j*j + m_fInverseA[5]*i*i + m_fInverseA[6]*j*j*i + m_fInverseA[7]*j*i*i + 
					m_fInverseA[8]*j*j*j + m_fInverseA[9]*i*i*i)+0.5);
		y = (long)((m_fInverseB[0] + m_fInverseB[1]*j + m_fInverseB[2]*i + m_fInverseB[3]*i*j + 
			        m_fInverseB[4]*j*j + m_fInverseB[5]*i*i + m_fInverseB[6]*j*j*i + m_fInverseB[7]*j*i*i + 
					m_fInverseB[8]*j*j*j + m_fInverseB[9]*i*i*i)+0.5);
	
		if (x<0 || x>=nWidth || y<0 || y>=nHeight)
		{
			pDestImage[i*nWidth + j] = 0;
			continue;
		}
	
		pDestImage[i*nWidth + j] = pSourImage[y*nWidth + x];
	}

	return bRet;
}

BOOL CNCalLib::TransWarpingPos(float i, float j, float *x, float *y, long nWidth, long nHeight)
{
	BOOL bRet=FALSE;

	*x = (float)(m_fForwardA[0] + m_fForwardA[1]*i + m_fForwardA[2]*j + m_fForwardA[3]*i*j + 
	             m_fForwardA[4]*i*i + m_fForwardA[5]*j*j + m_fForwardA[6]*i*i*j + m_fForwardA[7]*i*j*j + 
		         m_fForwardA[8]*i*i*i + m_fForwardA[9]*j*j*j);
	*y = (float)(m_fForwardB[0] + m_fForwardB[1]*i + m_fForwardB[2]*j + m_fForwardB[3]*i*j + 
		         m_fForwardB[4]*i*i + m_fForwardB[5]*j*j + m_fForwardB[6]*i*i*j + m_fForwardB[7]*i*j*j + 
		         m_fForwardB[8]*i*i*i + m_fForwardB[9]*j*j*j);

	if (*x<0 || *y<0 || *x>=nWidth || *y>=nHeight) 
		bRet = TRUE;

	return bRet;
}


