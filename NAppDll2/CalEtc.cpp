// CalEtc.cpp : implementation file
//
#include "stdafx.h"
#include "CalEtc.h"
#include "math.h"
#include "NBlobDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CCalEtc::CCalEtc()
{
    A_MAT=B_MAT=C_MAT=D_MAT=F_MAT=NULL;
	T_MAT=I_MAT=NULL;

	m_nGridColor = BLACK_LEVEL;
	m_nMinArea =  100;
	m_nMaxArea = 100000;
	m_fScaleFactor = 1.0f;

	for(long i=0; i<DEF_N; i++)
	{
		m_fInverseA[i] = 0.0f;
		m_fInverseB[i] = 0.0f;

		m_fForwardA[i] = 0.0f;
		m_fForwardB[i] = 0.0f;
	}
}

CCalEtc::~CCalEtc()
{

}

void CCalEtc::LoadCalData(CString strFileName)
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

void CCalEtc::SaveCalData(CString strFileName)
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


DEF_MAT* CCalEtc::CreateMat(long nRow, long nCol)
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

void CCalEtc::DeleteMat(DEF_MAT *pMat)
{
	if (pMat!=NULL)
	{
		delete pMat->m_pMat;
		delete pMat;
		pMat=NULL;
	}
}

BOOL CCalEtc::InverseMat(DEF_MAT *a, DEF_MAT *b)
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

BOOL CCalEtc::TransposeMat(DEF_MAT *a, DEF_MAT *b)
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

BOOL CCalEtc::DotProductMat(DEF_MAT *a, DEF_MAT *b, DEF_MAT *c) 
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

BOOL CCalEtc::SetWarping(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nRow, long nCol, float fGridSize)
{
	BOOL bRet=TRUE;
	long  i, j, k;
	long  nThresh, nBlobCnt, nRealCnt;
	float fScale, fDist, fMin, fX, fY;
	float fColDist, fRowDist;
	LPBYTE pBinaryBuff;
	POINT2 ptS[4], ptT[4];

	// 캘리브레이션 GRID마크 위치 찾기 
	BLOB_STRUCT *pBlob;
	pBinaryBuff = new unsigned char[nWidth*nHeight];

	NBlobDll_Open();
	nThresh = NBlobDll_FindAutoThreshold(pImage, x1, y1, x2, y2, nWidth);
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
		    fMin = 10000.0f;
			for(j=0; j<nBlobCnt; j++)
			{
				fX = pBlob[j].m_fCenX - ptS[i].x;
				fY = pBlob[j].m_fCenY - ptS[i].y;

				fDist = (float)sqrt((double)(fX*fX + fY*fY));
				if (fDist<fMin)
				{
					ptT[i].x = pBlob[j].m_fCenX;
					ptT[i].y = pBlob[j].m_fCenY;
					fMin = fDist;
				}
			}
		}

		fRowDist = ((ptT[2].y-ptT[0].y) + (ptT[3].y-ptT[1].y))/2.0f;
		fRowDist /= (nRow-1);
		fColDist = ((ptT[1].x-ptT[0].x) + (ptT[3].x-ptT[2].x))/2.0f;
		fColDist /= (nCol-1);
		fScale = (fRowDist+fColDist)/2.0f;
		if (fScale>0.0f)
	    	m_fScaleFactor = fGridSize/fScale;
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

			fMin = 10000.0f;
			for(k=0; k<nRealCnt; k++)
			{
				fX = ptS[0].x - pBlob[k].m_fCenX;
				fY = ptS[0].y - pBlob[k].m_fCenY;
			
				fDist = (float)sqrt((double)(fX*fX + fY*fY));
				if (fDist<fMin)
				{
					A_MAT->m_pMat[nBlobCnt*2 + 0] = pBlob[k].m_fCenX;
					A_MAT->m_pMat[nBlobCnt*2 + 1] = pBlob[k].m_fCenY;
					fMin = fDist;
				}
			}

			nBlobCnt++;
		}

		nBlobCnt = 0;
		for(i=0; i<nRow; i++)
		for(j=0; j<nCol; j++)
        {
			fX = ptT[0].x + j*fScale;
			fY = ptT[0].y + i*fScale;

			B_MAT->m_pMat[nBlobCnt*DEF_N + 0] = 1;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 1] = fX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 2] = fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 3] = fX*fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 4] = fX*fX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 5] = fY*fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 6] = fX*fX*fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 7] = fX*fY*fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 8] = fX*fX*fX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 9] = fY*fY*fY;

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

			fMin = 10000.0f;
			for(k=0; k<nRealCnt; k++)
			{
				fX = ptS[0].x - pBlob[k].m_fCenX;
				fY = ptS[0].y - pBlob[k].m_fCenY;
			
				fDist = (float)sqrt((double)(fX*fX + fY*fY));
				if (fDist<fMin)
				{
					B_MAT->m_pMat[nBlobCnt*2 + 0] = pBlob[k].m_fCenX;
					B_MAT->m_pMat[nBlobCnt*2 + 1] = pBlob[k].m_fCenY;
					fMin = fDist;
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
			fX = pBlob[nBlobCnt].m_fCenX;
			fY = pBlob[nBlobCnt].m_fCenY;

			B_MAT->m_pMat[nBlobCnt*DEF_N + 0] = 1;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 1] = fX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 2] = fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 3] = fX*fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 4] = fX*fX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 5] = fY*fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 6] = fX*fX*fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 7] = fX*fY*fY;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 8] = fX*fX*fX;
			B_MAT->m_pMat[nBlobCnt*DEF_N + 9] = fY*fY*fY;

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

BOOL CCalEtc::TransWarpingImage(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
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
	
		pDestImage[i*nWidth + j] = 0;
		if (x>0 && x<nWidth-1 && y>0 && y<nHeight-1)
			pDestImage[i*nWidth + j] = pSourImage[y*nWidth + x];
	}

	return bRet;
}

BOOL CCalEtc::TransWarpingPos(float i, float j, float *x, float *y, long nWidth, long nHeight)
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
