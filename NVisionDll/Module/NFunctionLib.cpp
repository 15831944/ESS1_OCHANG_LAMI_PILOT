// NFunctionLib.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NFunctionLib.h"
#include "math.h"


// CNFunctionLib

IMPLEMENT_DYNAMIC(CNFunctionLib, CWnd)

CNFunctionLib::CNFunctionLib()
{

}

CNFunctionLib::~CNFunctionLib()
{
}

BEGIN_MESSAGE_MAP(CNFunctionLib, CWnd)
END_MESSAGE_MAP()

// CNFunctionLib 메시지 처리기입니다.
float CNFunctionLib::FindAngleProj(unsigned char *pImage, long x1, long y1, long x2, long y2, 
									  long nImageWidth, long nImageHeight, float fSAng, float fEAng)
{
	long   i, j, k, nIndex, Ok=0;
	long   nLength, nDiff;
	long   nSum, nPixelCnt, nMaxSum; 
	float  fSourDx, fSourDy, fTargDx, fTargDy;
	long   nTargDist, nSourDist, nCx, nCy;
	float  fCosV, fSinV, fTemp;
	float  a, b, fFindAng;
	CPoint ptSourPnt1, ptSourPnt2, ptSourCen;
	CPoint ptTargPnt1, ptTargPnt2, ptTargCen;
	float   fStepAng, fIndexAng;
	float   fStartAng, fEndAng;
	long   *pProjData;

	if (x1<0) x1 = 0;
	if (y1<0) y1 = 0;
	if (x2>=nImageWidth) x2 = nImageWidth-1;
	if (y2>=nImageHeight)y2 = nImageHeight-1;

	// 최대 거리 구하기 
	a = (float)(x2-x1);
	b = (float)(y2-y1);

	nLength = (long)sqrt(double(a*a + b*b));
	if (nLength<10) return 0.0f;

	// 메모리 영역 할당 
	pProjData = new long[nLength];

	// 변수 초기화 
	nCx = (x1 + x2)/2;
	nCy = (y1 + y2)/2;

	// projection data 초기화 
	for(i=0; i<nLength; i++) 
		pProjData[i] = 0;

	// 데이타 구하기 
    if (fSAng>fEAng) 
	{
		fTemp = fSAng;
		fSAng = fEAng;
		fEAng = fTemp;
	}

	nLength /= 2;
	fFindAng = 0.0f;
	for(k=0; k<3; k++)
	{
		if (k==0) 
		{
			fStartAng = fSAng;
			fEndAng = fEAng;
			fStepAng = 5.0f;
		}
		if (k==1) 
		{
			fStartAng = fFindAng-5;
			fEndAng = fFindAng+5;
			fStepAng = 1.0f;
        }
		if (k==2) 
		{
			fStartAng = fFindAng-1;
			fEndAng = fFindAng+1;
			fStepAng = 0.1f;
        }

		nMaxSum = 0;
		for(fIndexAng=fStartAng; fIndexAng<fEndAng; fIndexAng+=fStepAng) 
		{
			fSinV = (float)sin((fIndexAng+90)*RAD);
			fCosV = (float)cos((fIndexAng+90)*RAD);
			ptSourPnt1.y = (long)(nLength*fSinV + nCy);             
			ptSourPnt1.x = (long)(nLength*fCosV + nCx);            

			fSinV = (float)sin((fIndexAng-90)*RAD);
			fCosV = (float)cos((fIndexAng-90)*RAD);
			ptSourPnt2.y = (long)(nLength*fSinV + nCy);             
			ptSourPnt2.x = (long)(nLength*fCosV + nCx);            

			// Unit Vector & 거리 구하기 
    		a = (float)(ptSourPnt2.x-ptSourPnt1.x);
			b = (float)(ptSourPnt2.y-ptSourPnt1.y);

    		fSourDx = (float)(a / sqrt(a*a + b*b));
			fSourDy = (float)(b / sqrt(a*a + b*b));
			nSourDist = (long)(sqrt((double)(a*a + b*b)));
			
			nIndex=0;
			for(i=0; i<nSourDist; i+=2) 
			{
				ptSourCen.x = ptSourPnt1.x + (long)(i*fSourDx);
				ptSourCen.y = ptSourPnt1.y + (long)(i*fSourDy);

				if (ptSourCen.x<x1 || ptSourCen.x>x2 ||
					ptSourCen.y<y1 || ptSourCen.y>y2) continue;

				fSinV = (float)sin((fIndexAng+180)*RAD);
				fCosV = (float)cos((fIndexAng+180)*RAD);
				ptTargPnt1.y = (long)(nLength*fSinV + ptSourCen.y);             
				ptTargPnt1.x = (long)(nLength*fCosV + ptSourCen.x);            

				fSinV = (float)sin(fIndexAng*RAD);
				fCosV = (float)cos(fIndexAng*RAD);
				ptTargPnt2.y = (long)(nLength*fSinV + ptSourCen.y);             
				ptTargPnt2.x = (long)(nLength*fCosV + ptSourCen.x);   
				
				// Unit Vector & 거리 구하기 
    			a = (float)(ptTargPnt2.x-ptTargPnt1.x);
				b = (float)(ptTargPnt2.y-ptTargPnt1.y);

    			fTargDx = (float)(a / sqrt(a*a + b*b));
				fTargDy = (float)(b / sqrt(a*a + b*b));
				nTargDist = (long)(sqrt((double)(a*a + b*b)));

				nPixelCnt=0;
				nSum=0;
				for(j=0; j<nTargDist; j+=5)
				{
					ptTargCen.x = ptTargPnt1.x + (long)(j*fTargDx);
					ptTargCen.y = ptTargPnt1.y + (long)(j*fTargDy);

					if (ptTargCen.x<x1 || ptTargCen.x>x2 ||
						ptTargCen.y<y1 || ptTargCen.y>y2) continue;

					nSum += pImage[ptTargCen.y*nImageWidth + ptTargCen.x];
					nPixelCnt++;
				}

				if (nPixelCnt>0)
				{
					pProjData[nIndex] = nSum/nPixelCnt;
					nIndex++;
				}
			}

			nSum = 0;
			long nOffset = (long)(nIndex*0.25);
			for(i=nOffset; i<(nIndex-nOffset); i++) 
			{
				nDiff = abs(pProjData[i-1] - pProjData[i]);
				nSum += nDiff;
			}

			if (nSum > nMaxSum)
			{
				nMaxSum = nSum;
				fFindAng = fIndexAng;
			}
		}
	}

	Ok = 1;
	delete pProjData;

	return fFindAng;
}



