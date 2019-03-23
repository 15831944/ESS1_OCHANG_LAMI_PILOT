// FunctionEtc.cpp : implementation file
#include "stdafx.h"
#include "math.h"
#include "VisionLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisionLib
CVisionLib::CVisionLib()
{
}

CVisionLib::~CVisionLib()
{
}

BEGIN_MESSAGE_MAP(CVisionLib, CStatic)
	//{{AFX_MSG_MAP(CVisionLib)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionLib message handlers

// 자동 문턱값 구하는 함수 
// fm : 원 영상의 메모리 
// x1, y1, x2, y2 : 처리하고자하는 영역 
// pitch : 영상의 WIDTH(대부분 640)
long CVisionLib::FindAutoThreshold(unsigned char* fm, long x1, long y1, long x2, long y2, long pitch)
{
	long i,j;
	const int GRAYLEVEL=256;
	DWORD  m_nHistoData[256];
	DWORD  m_nArea,nSum=0;
	double m_dU;     /* Average */
	long nCount=0, Pos[3];
	long nThreshold;
	double pt, qt, U1, U2, qtPrev;
	double SigmaB2, MaxSigmaB2=0;

	if(x1>x2 || y1>y2) return 0;
    
	/* 초기화 */
	for(i=0;i<GRAYLEVEL;i++) m_nHistoData[i]=0;

	/* Histogram Data 구하기 */
	for(i=y1;i<=y2;i++)
		for(j=x1;j<=x2;j++)
			m_nHistoData[*(fm+i*pitch+j)]++;

	for(i=0;i<GRAYLEVEL;i++)
	{
		nSum+=i*m_nHistoData[i];
	}

	/* 면적과 평균을 구한다. */
	m_nArea=(x2-x1+1)*(y2-y1+1);
	m_dU   =(double)nSum/(double)m_nArea;

	/* Histogram의 데이타가 NONZERO인 곳이 1,2있을때>>>>>>>>*/
	for(i=0;i<GRAYLEVEL;i++)
		if(m_nHistoData[i])
		{
			Pos[nCount]=i;
			nCount++;
			if(nCount>2) break;
		}
	if (nCount==1) return -2;
	else if(nCount==2) return (Pos[0]+Pos[1])/2;
	/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	qt=(double)m_nHistoData[0]/(double)m_nArea;
	if(qt==1.0) return 0;

	U1   = 0;
	U2   = (m_dU-qt*U1)/(1-qt);

	for(i=0;i<GRAYLEVEL-1;i++)
	{
		SigmaB2=qt*(1-qt)*(U2-U1)*(U2-U1);
		if(SigmaB2 > MaxSigmaB2 )
		{
			nThreshold=i;
			MaxSigmaB2=SigmaB2;
        }
		qtPrev=qt;
		pt=(double)m_nHistoData[i+1]/(double)m_nArea;
		qt=qtPrev+pt;
		if(qt==0)         continue;
		else if(qt>0.99999)  break;    /*qt가 1 이면 더이상은 의미가 없으므로 Break함*/
		U1=(qtPrev*U1+(i+1)*pt)/qt;
		U2=(m_dU-qt*U1)/(1-qt);		 
	}

	return nThreshold;
}

double CVisionLib::FindZerePos(int N, float *Pos, float *PosV)
{
	int i;
	float Mat_A[9], Mat_AInv[9], Mat_B[3];
	float Pos2, Pos3, Pos4;
	float Parm1, Parm2, Parm3;

	for(i=0;i<9;i++) Mat_A[i]=0;
	for(i=0;i<3;i++) Mat_B[i]=0;

	for(i=0;i<N;i++)
	{
		Pos2=Pos[i]*Pos[i];
		Pos3=Pos2*Pos[i];
		Pos4=Pos3*Pos[i];
		Mat_A[0]+=Pos4;
		Mat_A[1]+=Pos3;
		Mat_A[2]+=Pos2;
		Mat_A[3]+=Pos3;
		Mat_A[4]+=Pos2;
		Mat_A[5]+=Pos[i];
		Mat_A[6]+=Pos2;
		Mat_A[7]+=Pos[i];
		Mat_A[8]+=1.0;

		Mat_B[0]+=PosV[i]*Pos2;
		Mat_B[1]+=PosV[i]*Pos[i];
		Mat_B[2]+=PosV[i];
	}
	matrixInverse(Mat_A,3,Mat_AInv);

	Parm1=Mat_AInv[0]*Mat_B[0]+Mat_AInv[1]*Mat_B[1]+Mat_AInv[2]*Mat_B[2];
	Parm2=Mat_AInv[3]*Mat_B[0]+Mat_AInv[4]*Mat_B[1]+Mat_AInv[5]*Mat_B[2];
	Parm3=Mat_AInv[6]*Mat_B[0]+Mat_AInv[7]*Mat_B[1]+Mat_AInv[8]*Mat_B[2];

	return(-Parm2/(2*Parm1));
}

void CVisionLib::TwoCurveFitting(int N, double *Pos, double *PosV, double *a, double *b, double *c)
{
	int i;
	double Mat_A[9], Mat_AInv[9], Mat_B[3];
	double Pos2, Pos3, Pos4;

	for(i=0;i<9;i++) Mat_A[i]=0;
	for(i=0;i<3;i++) Mat_B[i]=0;

	for(i=0;i<N;i++)
	{
		Pos2=Pos[i]*Pos[i];
		Pos3=Pos2*Pos[i];
		Pos4=Pos3*Pos[i];
		Mat_A[0]+=Pos4;
		Mat_A[1]+=Pos3;
		Mat_A[2]+=Pos2;
		Mat_A[3]+=Pos3;
		Mat_A[4]+=Pos2;
		Mat_A[5]+=Pos[i];
		Mat_A[6]+=Pos2;
		Mat_A[7]+=Pos[i];
		Mat_A[8]+=1.0;

		Mat_B[0]+=PosV[i]*Pos2;
		Mat_B[1]+=PosV[i]*Pos[i];
		Mat_B[2]+=PosV[i];
	}
	matrixInverse((float *)Mat_A,3,(float *)Mat_AInv);

	*a=Mat_AInv[0]*Mat_B[0]+Mat_AInv[1]*Mat_B[1]+Mat_AInv[2]*Mat_B[2];
	*b=Mat_AInv[3]*Mat_B[0]+Mat_AInv[4]*Mat_B[1]+Mat_AInv[5]*Mat_B[2];
	*c=Mat_AInv[6]*Mat_B[0]+Mat_AInv[7]*Mat_B[1]+Mat_AInv[8]*Mat_B[2];
}
// 직선 성분이 많은 영상의 경우 회전된 각도를 구하는 함수 
// fm : 원 영상의 메모리 
// x1, y1, x2, y2 : 처리하고자하는 영역 
// pitch : 영상의 WIDTH(대부분 640)
double CVisionLib::FindImageAngle(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch)
{
	long   i,j;
	double sx,sy, ang, norm;
	double sumx=0, sumy=0;
	long   left=x1;
	long   top=y1;
	long right=x2;
	long bottom=y2;

	for(i=top+1;i<bottom-1;i++) {
		for(j=left+1;j<right-1;j++)	{
			sx=*(fm+pitch*(i-1)+j+1)-*(fm+pitch*(i-1)+j-1)+	
				2*(*(fm+pitch*(i)+j+1)-*(fm+pitch*(i)+j-1))+	
				*(fm+pitch*(i+1)+j+1)-*(fm+pitch*(i+1)+j-1);
			sy=*(fm+pitch*(i+1)+j-1)-*(fm+pitch*(i-1)+j-1)+	
				2*(*(fm+pitch*(i+1)+j)-*(fm+pitch*(i-1)+j))+	
				*(fm+pitch*(i+1)+j+1)-*(fm+pitch*(i-1)+j+1);

			norm=sqrt(sx*sx+sy*sy);
			ang=4*atan2(sy, sx);

			sx=norm*cos(ang);
			sy=norm*sin(ang);

			sumx+=sx;
			sumy+=sy;
		}
	}

	return ((atan(sumy/sumx)/4.0)*180/3.141592);
} 

// 평균 밝기값을 구하는 함수(0 ~ 255)
// frame : 원 영상의 메모리 
// x1, y1, x2, y2 : 처리하고자하는 영역 
// pitch : 영상의 WIDTH(대부분 640)
long CVisionLib::GetAveBrightness(unsigned char *frame, long x1, long y1, long x2, long y2, long pitch) 
{
	// TODO: Add your dispatch handler code here
    long i, j, nNum;
	long Sum = 0;

	nNum = (x2-x1+1)*(y2-y1+1);
	if (nNum<=0) return 0;

	for(i=y1; i<=y2; i++)
	for(j=x1; j<=x2; j++)
	    Sum += *(frame+pitch*i + j);

	return (long)(Sum/nNum);
}

long CVisionLib::GetAveBrightness2(unsigned char *frame, long x1, long y1, long x2, long y2, long pitch) 
{
	// TODO: Add your dispatch handler code here
    long i, j, nTemp, nNum, nThresh;
	long HISTO[256], Sum = 0;

	for(i=0; i<256; i++)
	    HISTO[i]= 0;

	nNum = 0;
	for(i=y1; i<=y2; i++)
	for(j=x1; j<=x2; j++)
	{
		nTemp = (long)frame[pitch*i + j];
		HISTO[nTemp]++;
		nNum++;
	}

	nThresh = (long)((float)nNum*0.05f);
	nTemp = 0;
	for(i=0; i<256; i++)
	{
		nTemp += HISTO[i];
		if (nTemp>=nThresh)
		{
			nThresh = i;
			break;
		}
	}

	nNum = 0;
	Sum = 0;
	for(i=y1; i<=y2; i++)
	for(j=x1; j<=x2; j++)
	{
		nTemp = (long)frame[pitch*i + j];
		if (nTemp>=nThresh)
		{
			nNum++;
			Sum += nTemp;
		}
	}

	nTemp = 0;
	if (nNum > 0)
	    nTemp = (long)(Sum/nNum);

	return nTemp;
}

float CVisionLib::GetSharpnessValue(unsigned char *fmImage, //프레임 메모리
                        long x1, long y1, long x2, long y2, // 구하고자하는 Sharpness영역
                        long nWidth, long nHeight) // Image Width
{
	long i, j, nDiffer;
	long nIndex;
	double dSum1, dSum2;
	long *pGrayX, *pGrayY;

	pGrayX = new long[nWidth];
	pGrayY = new long[nHeight];

	// calc
	nIndex = 0;
    dSum1 = 0.0f;
	dSum2 = 0.0f;
	for(i=y1; i<y2; i++) 
	{
		for(j=x1; j<x2; j++)
		{
			pGrayX[j] = (fmImage[i*nWidth + (j-1)] + 
			             fmImage[i*nWidth + j] + 
						 fmImage[i*nWidth + (j+1)]) / 3;
		}

		for(j=x1+1; j<x2-1; j++)
		{
			nDiffer = pGrayX[j-1] - pGrayX[j+1];
			dSum1 += sqrt((double)(nDiffer * nDiffer));
			nIndex++;
		}
	}

	for(i=x1; i<x2; i++) 
	{
		for(j=y1; j<y2; j++)
		{
			pGrayY[j] = (fmImage[(j-1)*nWidth + i] + 
			             fmImage[j*nWidth + i] + 
						 fmImage[(j+1)*nWidth + i]) / 3;
		}

		for(j=y1+1; j<y2-1; j++)
		{
			nDiffer = pGrayY[j-1] - pGrayY[j+1];
			dSum2 += sqrt((double)(nDiffer * nDiffer));
			nIndex++;
		}
	}

	delete pGrayX;
	delete pGrayY;

	// normalize & return
	if (nIndex<1) return 0.0;

	return (float)((dSum1 + dSum2)/(float)nIndex);
}

float CVisionLib::GetSharpnessValue(int dataN, float *x, float *y)
{
	long i;
	double dSumX, dSumY;

	dSumX = 0;
	dSumY = 0;
	for(i=1; i<dataN; i++)
	{
		dSumX += sqrt(((x[i-1]-x[i])*(x[i-1]-x[i])));
		dSumY += sqrt(((y[i-1]-y[i])*(y[i-1]-y[i])));
	}

    return (float)((dSumX+dSumY)/(dataN*2));
}

// 직선성 평가 
float CVisionLib::GetLinerityValue(int dataN, float *x, float *y)
{
	long   i;
	double xvalue, yvalue, RR, a, b;
	double Sxx, Syy, Sx, Sy, Sxy, Ax, Ay;

	RR = 0.0;
	if (dataN>2)
	{
		Sx=Sy=Sxx=Sxy=Syy=0.0;
		for(i=0;i<dataN;i++)
		{
			xvalue=x[i];
			yvalue=y[i];

			Sx += xvalue;
			Sy += yvalue;
			Sxx += (xvalue*xvalue);
			Syy += (yvalue*yvalue);
			Sxy += (xvalue*yvalue);
		}

		Ax = Sx/dataN;
		Ay = Sy/dataN;

		a = Sxy-dataN*(Ax*Ay);
		b = Sxx-dataN*(Ax*Ax);

		RR = fabs(a/b);
	}
	
/*
	RR = 0.0;
	if (dataN>2)
	{
		Sx=Sy=Sxx=Sxy=Syy=0.0;
		for(i=0;i<dataN;i++)
		{
			xvalue=x[i];
			yvalue=y[i];

			Sx += xvalue;
			Sy += yvalue;
			Sxx += (xvalue*xvalue);
			Syy += (yvalue*yvalue);
			Sxy += (xvalue*yvalue);
		}

		Ax = Sx/dataN;
		Ay = Sy/dataN;

		RR = sqrt(Syy);

		a = Sxy-dataN*(Ax*Ay);
		b = (Sxx-dataN*(Ax*Ax)) * (Syy-dataN*(Ay*Ay));

		RR = (a*a)/b;
	}
	*/

	return (float)RR;
}

// 이바리 검사 
float CVisionLib::GetDetectVariValue(int dataN, float *x, float *y)
{
	long   i;
	double xvalue, yvalue, RR, a, b;
	double Sxx, Syy, Sx, Sy, Sxy, Ax, Ay;

	RR = 0.0; 
	if (dataN>2)
	{
		Sx=Sy=Sxx=Sxy=Syy=0.0;
		for(i=0;i<dataN;i++)
		{
			xvalue=*(x+i);
			yvalue=*(y+i);

			Sx += xvalue;
			Sy += yvalue;
			Sxx += xvalue*xvalue;
			Syy += yvalue*yvalue;
			Sxy += xvalue*yvalue;
		}

		Ax = Sx/dataN;
		Ay = Sy/dataN;

		a = Sxy - dataN*(Ax * Ay);
		b = (Sxx - dataN*(Ax*Ax)) * (Syy - dataN*(Ay*Ay));

		RR = (a*a)/b;
	}

	return (float)RR;
}

// 영상에서 Edge 처리 Processing 함수
// sourImage : 원 영상 메모리
// destImage : 결과 영상 메모리 
// x1, y1, x2, y2 : 처리하고자하는 영상의 위치 
// differ : Edge의 문턱값 
long CVisionLib::EdgeDetect(unsigned char *sourImage, unsigned char *destImage, long x1, long y1, long x2, long y2, long differ, long nWidth, long nHeight) 
{
	long i, j, k;
	long min, max;
	unsigned char gray[5];
	unsigned char *buffer;

	buffer = new unsigned char[nWidth*nHeight];

	for(i=y1+1; i<=y2-1; i++)
	for(j=x1+1; j<=x2-1; j++) 
	{
		min = 255;
		max = 0;

		gray[0] = sourImage[(i-1)*nWidth + (j-1)];
		gray[1] = sourImage[(i-1)*nWidth + (j+1)];
		gray[2] = sourImage[i*nWidth + j];
		gray[3] = sourImage[(i+1)*nWidth + (j-1)];
		gray[4] = sourImage[(i+1)*nWidth + (j+1)];

		for(k=0; k<=4; k++) {
			if (min > gray[k]) min = gray[k];
			if (max < gray[k]) max = gray[k];
		}

		if (max-min > differ) 
			buffer[i*nWidth + j] = 255;
		else
			buffer[i*nWidth + j] = 0;
	}

	for(i=y1+1; i<=y2-1; i++)
	for(j=x1+1; j<=x2-1; j++) 
	{
        destImage[i*nWidth + j] = buffer[i*nWidth + j];
	}

	delete buffer;
	return 1;
}

// 이진화 처리 
// sourImage : 원 영상 메모리
// destImage : 결과 영상 메모리 
// x1, y1, x2, y2 : 처리하고자하는 영상의 위치 
// threshold : 문턱값 
void CVisionLib::ImageBinarize(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long threshold)
{
	long i, j;
 
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 > nWidth-1)  x2 = nWidth-1;
	if (y2 > nHeight-1) y2 = nHeight-1;

	for(i=y1; i<y2; i++)
	for(j=x1; j<x2; j++) 
	{
		if (SourImage[i*nWidth + j] > threshold)
            DestImage[i*nWidth + j] = WHITE_LEVEL;
		else
			DestImage[i*nWidth + j] = BLACK_LEVEL;
	}
}

void CVisionLib::BinarizeLinearThreshold(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nDir, long nStep, long nThresh)
{
	long i, j, n, m, nNum, nCnt;
	long nDiffer;
	long  *pProjX, *pProjY;
	long  *DX, *DY;
 
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 >= nWidth)  x2 = nWidth-1;
	if (y2 >= nHeight) y2 = nHeight-1;

	memset(DestImage, 0, nWidth*nHeight);
    if (nDir)
	{
    	pProjY = new long[nHeight];
    	DX = new long[nHeight];
    	DY = new long[nHeight];
		for(i=x1; i<x2-nStep; i+=(nStep/2))
		{
			nNum = 0;
			for(j=y1; j<=y2; j++)
			{
				pProjY[nNum] = 0;
				nCnt=0;
				for(n=i; n<i+nStep; n++) 
				{
					pProjY[nNum] += SourImage[j*nWidth + n];
					nCnt++;
				}

				if (nCnt>0)
			    	pProjY[nNum] /= nCnt;

				nNum++;
			}

			nCnt = GetPeakData(nNum, pProjY, DX, DY);
			for(j=0; j<nCnt-1; j++)
			{
				nDiffer = DY[j]-DY[j+1];
				if (nDiffer>nThresh)
				{
					for(n=i; n<i+nStep; n++)
					for(m=DX[j]; m<=DX[j+1]; m++) 
						DestImage[(m+y1)*nWidth + n] = WHITE_LEVEL;
				}
			}
		}

		delete pProjY;
		delete DX;
		delete DY;
	}
	else
	{
    	pProjX = new long[nWidth];
    	DX = new long[nWidth];
    	DY = new long[nWidth];
		for(i=y1; i<y2-nStep; i+=(nStep/2))
		{
			nNum = 0;
			for(j=x1; j<=x2; j++)
			{
				pProjX[nNum] = 0;
				nCnt=0;
				for(n=i; n<i+nStep; n++) 
				{
					pProjX[nNum]+=SourImage[n*nWidth + j];
					nCnt++;
				}

				if (nCnt>0)
			    	pProjX[nNum] /= nCnt;

				nNum++;
			}

			nCnt = GetPeakData(nNum, pProjX, DX, DY);
			for(j=0; j<nCnt-1; j++)
			{
				nDiffer = DY[j+1]-DY[j];
				if (nDiffer>nThresh)
				{
					for(n=i; n<i+nStep; n++)
					for(m=DX[j]; m<=DX[j+1]; m++) 
						DestImage[n*nWidth + (m+x1)] = WHITE_LEVEL;
				}
			}
		}

		delete pProjX;
		delete DX;
		delete DY;
	}
}

/*
void CVisionLib::BinarizeLinearThreshold(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nDir, long nStep, long nThresh)
{
	long i, j, k, nNum, nCnt, nRet, nTemp, nStart, nEnd;
	float a, b, t;
	long  *pProjX, *pProjY;
	float *DX, *DY;
 
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 >= nWidth)  x2 = nWidth-1;
	if (y2 >= nHeight) y2 = nHeight-1;

	memset(DestImage, 0, nWidth*nHeight);
    if (nDir)
	{
    	pProjY = new long[nHeight];
    	DX = new float[nHeight];
    	DY = new float[nHeight];
		for(i=x1; i<x2-nStep; i+=(nStep/2))
		{
			nNum = 0;
			for(j=y1; j<=y2; j++)
			{
				pProjY[j] = 0;
				nCnt=0;
				for(k=i; k<i+nStep; k++) 
				{
					pProjY[j]+=SourImage[j*nWidth + k];
					nCnt++;
				}

				if (nCnt>0)
			    	pProjY[j] /= nCnt;
			}

			nStart = y1;
			nEnd = y2;
			for(j=y1; j<=y2; j++)
			{
				if (pProjY[j]<WHITE_LEVEL-2) {
					nStart = j;
					break;
				}
			}

			for(j=y2; j>y1; j--)
			{
				if (pProjY[j]<WHITE_LEVEL-2) {
					nEnd = j;
					break;
				}
			}

			for(j=nStart; j<=nEnd; j++)
			{
		    	DX[nNum] = (float)j;
				DY[nNum] = (float)pProjY[j];
				nNum++;
			}

			if (nNum>2)
			{
				nRet = LineFitting(nNum, DX, DY, 0, 5, &t, &a, &b);
				if (nRet>0)
				{
					for(j=nStart; j<=nEnd; j++)
					{
						nTemp = (long)((a*j + b) - pProjY[j]);
						if (abs(nTemp)>nThresh)
						{
							for(k=i; k<i+nStep; k++) 
								DestImage[j*nWidth + k] = WHITE_LEVEL;
						}
						else
						{
							for(k=i; k<i+nStep; k++) 
								DestImage[j*nWidth + k] = BLACK_LEVEL;
						}
					}
				}
			}
		}	
	
		delete pProjY;
		delete DX;
		delete DY;
	}
	else
	{
    	pProjX = new long[nWidth];
    	DX = new float[nWidth];
    	DY = new float[nWidth];
		for(i=y1; i<y2-nStep; i+=(nStep/2))
		{
			nNum = 0;
			for(j=x1; j<=x2; j++)
			{
				pProjX[j] = 0;
				nCnt=0;
				for(k=i; k<i+nStep; k++) 
				{
					pProjX[j]+=SourImage[k*nWidth + j];
					nCnt++;
				}

				if (nCnt>0)
			    	pProjX[j] /= nCnt;
			}

			nStart = x1;
			nEnd = x2;
			for(j=x1; j<=x2; j++)
			{
				if (pProjX[j]<WHITE_LEVEL-2) {
					nStart = j;
					break;
				}
			}

			for(j=x2; j>x1; j--)
			{
				if (pProjX[j]<WHITE_LEVEL-2) {
					nEnd = j;
					break;
				}
			}

			for(j=nStart; j<=nEnd; j++)
			{
		    	DX[nNum] = (float)j;
				DY[nNum] = (float)pProjX[j];
				nNum++;
			}

			nRet = LineFitting(nNum, DX, DY, 0, 5, &t, &a, &b);
			if (nRet>0)
			{
				for(j=nStart; j<=nEnd; j++)
				{
					nTemp = (long)((a*j + b) - pProjX[j]);
					if (abs(nTemp)>nThresh)
					{
						for(k=i; k<i+nStep; k++) 
							DestImage[k*nWidth + j] = WHITE_LEVEL;
					}
					else
					{
						for(k=i; k<i+nStep; k++) 
							DestImage[k*nWidth + j] = BLACK_LEVEL;
					}
				}
			}
		}	
	
		delete pProjX;
		delete DX;
		delete DY;
	}
}
*/

BOOL CVisionLib::GetWhiteBlob(LPBYTE SourImage1, LPBYTE SourImage2, LPBYTE pMaskImage, long x1, long y1, long x2, long y2, long nOffset, long nWidth, long nHeight)
{
	BOOL bRet=FALSE;
	long i, j, nSumOut, nOutCnt, nSumIn, nInCnt;
	long nTopX, nTopY, nBotX, nBotY;
	long nSizeX, nSizeY;

	nSizeX = (long)((x2-x1)*0.5);
	if (nSizeX<3) nSizeX = 3;

	nSizeY = (long)((y2-y1)*0.5);
	if (nSizeY<3) nSizeY = 3;

	nTopY = y1-nSizeY;
	if (nTopY<1) nTopY=1;
	nBotY = y2+nSizeY;
	if (nBotY>=nHeight-1) nBotY=nHeight-2;

	nTopX = x1-nSizeX;
	if (nTopX<1) nTopX=1;
	nBotX = x2+nSizeX;
	if (nBotX>=nWidth-1) nBotX=nWidth-2;

 	nSumIn=0;
	nInCnt=0;
    for(i=y1; i<=y2; i++)
	for(j=x1; j<=x2; j++)
	{
		if ((pMaskImage[i*nWidth + (j-1)]<MEDIUM_LEVEL) ||
			(pMaskImage[i*nWidth + (j+1)]<MEDIUM_LEVEL) ||
			(pMaskImage[(i-1)*nWidth + j]<MEDIUM_LEVEL) ||
			(pMaskImage[(i+1)*nWidth + j]<MEDIUM_LEVEL))
		{
			nSumIn += SourImage1[i*nWidth + j];
			nInCnt++;
		}
	}

	nSumOut=0;
	nOutCnt=0;
    for(i=nTopY; i<=nBotY; i++)
	for(j=nTopX; j<=nBotX; j++)
	{
		if ((pMaskImage[i*nWidth + (j-1)]<MEDIUM_LEVEL) ||
			(pMaskImage[i*nWidth + (j+1)]<MEDIUM_LEVEL) ||
			(pMaskImage[(i-1)*nWidth + j]<MEDIUM_LEVEL) ||
			(pMaskImage[(i+1)*nWidth + j]<MEDIUM_LEVEL)) continue;

		nSumOut += SourImage2[i*nWidth + j];
		nOutCnt++;
	}

	if (nOutCnt>0 && nInCnt>0)
	{
		if ((long)(nSumIn/nInCnt)<(long)(nSumOut/nOutCnt+nOffset)*0.9)
			bRet = TRUE;	
	}

	return bRet;
}

long CVisionLib::GetProjectFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX)
{
    long i, j, nNum;

	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth)  x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	for(i=0; i<nWidth; i++) 
		ProjX[i] = 0;

	nNum = y2-y1+1;
	for(i=y1; i<=y2; i++) 
	for(j=x1; j<=x2; j++) 
	    ProjX[j] += pImage[i*nWidth + j];

	if (nNum>0)
	{
		for(i=x1; i<=x2; i++) 
			ProjX[i] /= nNum;
	}

	return 1;
}

long CVisionLib::GetProjectFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY)
{
    long i, j, nNum;

	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth) x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	for(i=0; i<nHeight; i++) 
		ProjY[i] = 0;

	nNum = x2-x1+1;
	for(i=y1; i<=y2; i++) 
	for(j=x1; j<=x2; j++) 
		ProjY[i] += (long)pImage[i*nWidth + j];

	if (nNum>0)
	{
		for(i=y1; i<=y2; i++) 
			ProjY[i] /= nNum;
	}

	return 1;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

long CVisionLib::GetProjectDfDtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjX)
{
    long i, nSide;
	long *pTemp = new long[nWidth];

	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth)  x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	nSide = abs(nNaverPixel);
	GetProjectFtX(pImage, x1, y1, x2, y2, nWidth, nHeight, ProjX);

	memcpy(pTemp, ProjX, sizeof(long)*nWidth);
	for(i=x1+1; i<x2; i++)
		pTemp[i] = (ProjX[i-1]+ProjX[i]+ProjX[i+1])/3;

	for(i=0; i<nWidth; i++) ProjX[i] = 0;
	for(i=x1+nSide; i<=x2-nSide; i++)
		ProjX[i] = pTemp[i+nSide] - pTemp[i-nSide];

	delete pTemp;
	return 1;
}

long CVisionLib::GetProjectDfDtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjY)
{
    long i, nSide;
	long *pTemp = new long[nHeight];

	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth)  x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	nSide = abs(nNaverPixel);
	GetProjectFtY(pImage, x1, y1, x2, y2, nWidth, nHeight, ProjY);

	memcpy(pTemp, ProjY, sizeof(long)*nHeight);

	for(i=0; i<nHeight; i++) ProjY[i] = 0;
	for(i=y1+nSide; i<=y2-nSide; i++)
		ProjY[i] = pTemp[i+nSide] - pTemp[i-nSide];

	delete pTemp;
	return 1;
}

float CVisionLib::GetAverDistance(long nType, POINT2 ptPos1, POINT2 ptPos2, POINT2 ptPos3, POINT2 ptPos4, float a1, float b1, float a2, float b2)
{
	// 평균 거리 구하기 
	long i, nIndex, nDist;
	POINT2 p1, p2, p3, p4, cp, sp;
	double dAngle, dSum, dTemp;

	nIndex = 0;
	if (nType==DIR_TOP_BOTTOM || nType==DIR_BOTTOM_TOP) 
	{
		m_libMath.GetCrossPointLineEquWithLine(a1, b1, ptPos1, ptPos3, &p1);
		m_libMath.GetCrossPointLineEquWithLine(a1, b1, ptPos2, ptPos4, &p2);

		m_libMath.GetCrossPointLineEquWithLine(a2, b2, ptPos1, ptPos3, &p3);
		m_libMath.GetCrossPointLineEquWithLine(a2, b2, ptPos2, ptPos4, &p4);

		nDist = (long)GetDistance2D(p1.x, p1.y, p2.x, p2.y);
		dTemp = p2.x-p1.x;
		if (dTemp==0.0)
			dTemp = 0.00001;

		dAngle = atan((double)(p2.y-p1.y) / dTemp) * DEG;

    	dSum = (float)0.0;
		for(i=1; i<nDist; i++)
		{
			nIndex++;
			sp.x = (float)(i * cos(dAngle*RAD) + p1.x);
			sp.y = (float)(i * sin(dAngle*RAD) + p1.y);

			m_libMath.GetCrossPointDotLine(sp, p3, p4, &cp);
//			m_libMath.GetCrossPointLineEquWithDot(a2, b2, sp, &cp);
			dSum += GetDistance2D(sp.x, sp.y, cp.x, cp.y);
		}
	}
	else 
	{
		m_libMath.GetCrossPointLineEquWithLine(a1, b1, ptPos1, ptPos2, &p1);
		m_libMath.GetCrossPointLineEquWithLine(a1, b1, ptPos3, ptPos4, &p2);

		m_libMath.GetCrossPointLineEquWithLine(a2, b2, ptPos1, ptPos2, &p3);
		m_libMath.GetCrossPointLineEquWithLine(a2, b2, ptPos3, ptPos4, &p4);

		nDist = (long)GetDistance2D(p1.x, p1.y, p2.x, p2.y);
		dTemp = p2.x-p1.x;
		if (dTemp==0.0)
			dTemp = 0.00001;

		dAngle = atan((double)(p2.y-p1.y) / dTemp) * DEG;

    	dSum = (float)0.0;
		for(i=1; i<nDist; i++) 
		{
			nIndex++;
			sp.x = (float)(i * cos(dAngle*RAD) + p1.x);
			sp.y = (float)(i * sin(dAngle*RAD) + p1.y);

			m_libMath.GetCrossPointDotLine(sp, p3, p4, &cp);
//			m_libMath.GetCrossPointLineEquWithDot(a2, b2, sp, &cp);
			dSum += GetDistance2D(sp.x, sp.y, cp.x, cp.y);
		}
	}

	if (nIndex>0)
		dTemp = (dSum/nIndex);

	return (float)dTemp;
}

float CVisionLib::GetPosDetectEdge_DfDt(long *pData, long nStart, long nEnd, long nDir, long nPolality, long nThresh, long nMaxThr, long nMinThr)
{
	long i, nPos=-1;
	long nS, nE, nMax;
	float fEdgePos;
	long grayavg = 0;

	fEdgePos = -1.0f;
	if (nDir==DIR_LEFT_RIGHT || nDir==DIR_TOP_BOTTOM)
	{
		if (nPolality==POL_BLACK_WHITE)
		{
			for(i=nStart+2; i<nEnd-2; i++)
			{		
				if ( pData[i+2]-pData[i-2] > nThresh)
				{	
					grayavg = (long)( (float)(pData[i+2]+pData[i+3]) / 2.0 );
					if(grayavg <= nMaxThr && grayavg >= nMinThr)
					{
						nPos = i;
						break;
					}
				}
			}

			if (nPos>0)
			{
				nS = nPos-2;
				nE = nPos+2;
				if (nS<nStart) nS=nStart;
				if (nE>nEnd)   nE=nEnd;

				nMax = -1;
				for(i=nS+1; i<=nE-1; i++)
				{
					if ((pData[i+1]-pData[i-1])>nMax)
					{
						nPos = i;
						nMax = pData[i+1]-pData[i-1];
					}
				}
			}
		}

		if (nPolality==POL_WHITE_BLACK)
		{
			for(i=nStart+2; i<nEnd-2; i++)
			{
				if (pData[i-2]-pData[i+2]>nThresh) 
				{
					grayavg = (long)( (float)(pData[i+2]+pData[i+3]) / 2.0); // (long)(float)(pData[i+4] + pData[i+5] + pData[i+6]) / 3.0;
					if(grayavg <= nMaxThr && grayavg >= nMinThr)
					{
						nPos = i;
						break;
					}
				}
			}

			if (nPos>0)
			{
				nS = nPos-2;
				nE = nPos+2;
				if (nS<nStart) nS=nStart;
				if (nE>nEnd)   nE=nEnd;

				nMax = -1;
				for(i=nS+1; i<=nE-1; i++)
				{
					if ((pData[i-1]-pData[i+1])>nMax)
					{
						nPos = i;
						nMax = pData[i-1]-pData[i+1];
					}
				}
			}
		}
	}

	if (nDir==DIR_RIGHT_LEFT || nDir==DIR_BOTTOM_TOP)
	{
		if (nPolality==POL_BLACK_WHITE)
		{
			for(i=nEnd-2; i>nStart+2; i--)
			{
				if (pData[i-2]-pData[i+2]>nThresh) 
				{
					grayavg = (long)( (float)(pData[i-2]+pData[i-3]) / 2.0); // (long)(float)(pData[i+4] + pData[i+5] + pData[i+6]) / 3.0;
					if(grayavg <= nMaxThr && grayavg >= nMinThr)
					{
						nPos = i;
						break;
					}
				}
			}

			if (nPos>0)
			{
				nS = nPos-2;
				nE = nPos+2;
				if (nS<nStart) nS=nStart;
				if (nE>nEnd)   nE=nEnd;

				nMax = -1;
				for(i=nS+1; i<=nE-1; i++)
				{
					if ((pData[i-1]-pData[i+1])>nMax)
					{
						nPos = i;
						nMax = pData[i-1]-pData[i+1];
					}
				}
			}
		}

		if (nPolality==POL_WHITE_BLACK)
		{
			for(i=nEnd-2; i>nStart+2; i--)
			{
				if (pData[i+2]-pData[i-2]>nThresh)
				{
					grayavg = (long)( (float)(pData[i-2]+pData[i-3]) / 2.0); //(long)(float)(pData[i+4] + pData[i+5] + pData[i+6]) / 3.0;
					if(grayavg <= nMaxThr && grayavg >= nMinThr)
					{
						nPos = i;
						break;
					}
				}
			}

			if (nPos>0)
			{
				nS = nPos-2;
				nE = nPos+2;
				if (nS<nStart) nS=nStart;
				if (nE>nEnd)   nE=nEnd;

				nMax = -1;
				for(i=nS+1; i<=nE-1; i++)
				{
					if ((pData[i+1]-pData[i-1])>nMax)
					{
						nPos = i;
						nMax = pData[i+1]-pData[i-1];
					}
				}
			}
		}
    }
	
	fEdgePos = (float)nPos;

	return fEdgePos;
}

float CVisionLib::GetPosDetectEdge_DfDt2(long *pData, long nStart, long nEnd, long nDir, long nPolality, long nThresh)
{
	long i, nPos=-1;
	long nS, nE, nMax;
	float fEdgePos;

	fEdgePos = -1.0f;
	if (nDir==DIR_LEFT_RIGHT || nDir==DIR_TOP_BOTTOM)
	{
		if (nPolality==POL_BLACK_WHITE)
		{
			for(i=nStart+2; i<nEnd-2; i++)
			{
				if (pData[i+2]-pData[i-2]>nThresh)
				{
					nPos = i;
					break;
				}
			}

			if (nPos>0)
			{
				nS = nPos-2;
				nE = nPos+2;
				if (nS<nStart) nS=nStart;
				if (nE>nEnd)   nE=nEnd;

				nMax = -1;
				for(i=nS+1; i<=nE-1; i++)
				{
					if ((pData[i+1]-pData[i-1])>nMax)
					{
						nPos = i;
						nMax = pData[i+1]-pData[i-1];
					}
				}
			}
		}

		if (nPolality==POL_WHITE_BLACK)
		{
			for(i=nStart+2; i<nEnd-2; i++)
			{
				if (pData[i-2]-pData[i+2]>nThresh) 
				{
					nPos = i;
					break;
				}
			}

			if (nPos>0)
			{
				nS = nPos-2;
				nE = nPos+2;
				if (nS<nStart) nS=nStart;
				if (nE>nEnd)   nE=nEnd;

				nMax = -1;
				for(i=nS+1; i<=nE-1; i++)
				{
					if ((pData[i-1]-pData[i+1])>nMax)
					{
						nPos = i;
						nMax = pData[i-1]-pData[i+1];
					}
				}
			}
		}
	}

	if (nDir==DIR_RIGHT_LEFT || nDir==DIR_BOTTOM_TOP)
	{
		if (nPolality==POL_BLACK_WHITE)
		{
			for(i=nEnd-2; i>nStart+2; i--)
			{
				if (pData[i-2]-pData[i+2]>nThresh) 
				{
					nPos = i;
					break;
				}
			}

			if (nPos>0)
			{
				nS = nPos-2;
				nE = nPos+2;
				if (nS<nStart) nS=nStart;
				if (nE>nEnd)   nE=nEnd;

				nMax = -1;
				for(i=nS+1; i<=nE-1; i++)
				{
					if ((pData[i-1]-pData[i+1])>nMax)
					{
						nPos = i;
						nMax = pData[i-1]-pData[i+1];
					}
				}
			}
		}

		if (nPolality==POL_WHITE_BLACK)
		{
			for(i=nEnd-2; i>nStart+2; i--)
			{
				if (pData[i+2]-pData[i-2]>nThresh)
				{
					nPos = i;
					break;
				}
			}

			if (nPos>0)
			{
				nS = nPos-2;
				nE = nPos+2;
				if (nS<nStart) nS=nStart;
				if (nE>nEnd)   nE=nEnd;

				nMax = -1;
				for(i=nS+1; i<=nE-1; i++)
				{
					if ((pData[i+1]-pData[i-1])>nMax)
					{
						nPos = i;
						nMax = pData[i+1]-pData[i-1];
					}
				}
			}
		}
    }
	
	fEdgePos = (float)nPos;

	return fEdgePos;
}

BOOL CVisionLib::GetAngle(float dX1, float dY1, float dX2, float dY2, float *dAngle)
{
	BOOL bRet=FALSE;
	float fDx, fDy;

	fDx = dX2-dX1;
	fDy = dY2-dY1;

	if (fabs(fDy) > fabs(fDx))
	    *dAngle = (float)(atan(fDx / fDy) * DEG);
	else
		*dAngle = (float)(atan(fDy / fDx) * DEG);

	bRet = TRUE;
	return bRet;
}

float CVisionLib::GetDistance2D(float x1, float y1, float x2, float y2)
{
	float a, b;

	a = x2-x1;
	b = y2-y1;
	
	return ((float)sqrt(a*a + b*b));
}                     
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
// 실제 프로파일 분석으로 산의 갯수를 구함 
// Data : 한 라인의 데이타 메모리 
// nDataNum : 데이타 길이 
// differ : Profile의 높이 폭이 이 수치보다 크면 Check함 
long CVisionLib::GetPeakData(long nDataNum, long *Data, long *PEAK_X, long *PEAK_Y) 
{
	bool upFlag;
    long i;
	long nPeakCnt;
	long privData, nextData, peakData;

	if (nDataNum<2) return 0;

	PEAK_X[0] = 0;
    privData = Data[0];
	upFlag = true;

	nPeakCnt = 0;
	for(i=1; i<nDataNum; i++)
	{
		nextData = Data[i];
		if (privData < nextData) {
			if (!upFlag) {
				PEAK_X[nPeakCnt] = i;
                PEAK_Y[nPeakCnt] = privData;
				nPeakCnt++;
				peakData = privData;
            }

			upFlag = true;
			privData = nextData;
			continue;
		}
		if (privData > nextData) {
			if (upFlag) {
				PEAK_X[nPeakCnt] = i;
                PEAK_Y[nPeakCnt] = privData;
				nPeakCnt++;
				peakData = privData;
            }

			upFlag = false;
			privData = nextData;
            continue;
        }
	}

	if (nPeakCnt<nDataNum && nPeakCnt>0)
	{
		PEAK_X[nPeakCnt] = nDataNum-1;
		PEAK_Y[nPeakCnt] = nextData;
		nPeakCnt++;
	}

	return nPeakCnt;
}

//Line Fitting
//ty=ax+b
//dataN : 주어진 (x,y)수
//*x, *y; (x,y)좌표 
//removeN : LineFitting후 Line에서 가장 멀리있는 점을 없애고 다시 fitting울 반복한다.
//          이 반복횟수를 나타낸다.
//ErrorLimit: LineFitting후 Line에서 가장 멀리있는 점이 이값 이하이면 반복을 중지한다.
//결과값 return: 1: 정상수행
//				 <0 : FAILURE
//              *t, *a, *b (ty=ax+b)

int CVisionLib::LineFitting(int dataN, float *x, float *y, int removeN, float ErrorLimit, float *t, float *a, float *b)
{
	BOOL bRet=FALSE;
	int k,i,j;
	int nCount, MaxPos,*nValid;  
	double xvalue,yvalue;
	double Sxx, Sx, Sy,Sxy, SxSy, dd, MaxDist=0,dist;
	double ZeroData=0.00001;

	MaxPos = 0;
	*a = 0.0f;
	*b = 0.0f;
	*t = 0.0f;
	if (dataN<2) return -1;

	if(removeN>(dataN-2)) removeN=(dataN-2);

	nValid=(int*)malloc(dataN*sizeof(int));
	if(nValid==0) return -1;

	for(i=0;i<dataN;i++) nValid[i]=0;

	for(k=0;k<removeN+1;k++)
	{
		if(k==0)
		{
			Sx=Sy=Sxx=Sxy=SxSy=0.0;
			for(i=0;i<dataN;i++)
			{
				if (*(x+i) < 1 || *(y+i) < 1) continue;
				xvalue=*(x+i);
				yvalue=*(y+i);
				Sx+=xvalue;
				Sy+=yvalue;
				Sxx+=xvalue*xvalue;
				Sxy+=xvalue*yvalue;
			}
		}
		else
		{
			if (*(x+i) < 1 || *(y+i) < 1) continue;
			xvalue=*(x+MaxPos);
			yvalue=*(y+MaxPos);
			Sx-=xvalue;
			Sy-=yvalue;
			Sxx-=xvalue*xvalue;
			Sxy-=xvalue*yvalue;		
		}

		nCount=dataN-k;
		SxSy=Sx*Sy;

		dd=nCount*Sxx-Sx*Sx;
		if(fabs(dd)<ZeroData)
		{
			*t=0;
			*a=1;
			*b=(float)(-Sx/nCount);
			goto done;
		}
		else
		{
			*t=1;
			*a=(float)((nCount*Sxy-Sx*Sy)/dd);
			*b=(float)((-Sxy*Sx+Sy*Sxx)/dd);
		}

		dd=sqrt(*a**a+1);
		MaxDist=0;
		for(j=0;j<dataN;j++)
		{
			if(nValid[j]) continue;
			dist=fabs(-*(y+j)+*a**(x+j)+*b);
			if(dist>MaxDist)
			{
				MaxDist=dist;
				MaxPos=j;
			}
		}

		if(MaxDist/dd<ErrorLimit) goto done;
		else nValid[MaxPos]=1;
	}

done:
	if (nValid) free(nValid);
	return 1;
}

//1st Line Fitting(Line이 거의 수직일때)
// return 1: SUCCESS
//        -1: FAILURE
int CVisionLib::LineFitting1(int dataN, float *x, float *y, int removeN, float ErrorLimit,
				 float *t, float *a, float *b)
{
	int returnValue;
	float la,lb,lt;

	*a = 0.0f;
	*b = 0.0f;
	*t = 0.0f;

	returnValue=LineFitting(dataN,y, x, removeN, ErrorLimit, &lt,&la,&lb);
	if (returnValue==1)	{
		if(fabs(la)<0.0001)  //기울기가 10000이상은 y축에 평행한것으로 간주한다.
		{
			*t=0;
			*a=lt;
			*b=-lb;
		}
		else
		{
			*t=1;
			*a=lt/la;
			*b=-lb/la;
		}	
		return 1;
	}
	else return returnValue;
}

/////////////////////////////////////////////////////////////////////////////
// CCircleFitting message handlers
long CVisionLib::FindFittingCenter(int N, float *x, float *y, int nRemove, float dLimit, 
					   float *Cx, float *Cy, float *MaxRadius,float *MinRadius,float *AvgRadius)
{
	long nResult=-1;
	int i, k, index;
	float dX, dY;
	float SX, SY, SArea, SXY, SXX, SYY, SXXX, SYYY, SYYX, SXXY;
	float Mat_A[9], Mat_AInv[9], Mat_B[3];
	float Parm1, Parm2, Parm3;
	float dDist, dMaxDist, dMinDist, dDistSum;
	int   nCount, nMaxPos, nMinPos;
	int   *pflag;

	index = 0;
	for(i=0; i<N; i++) {
		if (x[i]<=0 || y[i]<=0) continue;
		x[index] = x[i];
		y[index] = y[i];
		index++;
	}
	N = index;

	pflag=(int *)malloc(N*sizeof(int));
	if (N<5) goto done;

	if(pflag==0) nRemove=0;

	if(nRemove)
		for(i=0;i<N;i++) pflag[i]=1;
	
	for(k=0;k<=nRemove;k++)
	{
		SX=SY=SArea=SXY=SXX=SYY=SXXX=SYYY=SYYX=SXXY=0;
		for(i=0;i<N;i++)
		{
			if(pflag[i])
			{
				dX=x[i];
				dY=y[i];

				SArea++;
				SX+=dX;
				SY+=dY;
				SXY+=dY*dX;
				SXX+=dX*dX;
				SYY+=dY*dY;
				SXXX+=dX*dX*dX;
				SYYY+=dY*dY*dY;
				SYYX+=dY*dY*dX;
				SXXY+=dX*dX*dY;
			}
		}

		Mat_A[0]=(float)SXX;
		Mat_A[1]=(float)SXY;
		Mat_A[2]=(float)SX;
		Mat_A[3]=(float)SXY;
		Mat_A[4]=(float)SYY;
		Mat_A[5]=(float)SY;
		Mat_A[6]=(float)SX;
		Mat_A[7]=(float)SY;
		Mat_A[8]=(float)SArea;

		Mat_B[0]=(float)(-SXXX-SYYX);
		Mat_B[1]=(float)(-SYYY-SXXY);
		Mat_B[2]=(float)(-SXX-SYY);

		matrixInverse(Mat_A,3,Mat_AInv);

		Parm1=Mat_AInv[0]*Mat_B[0]+Mat_AInv[1]*Mat_B[1]+Mat_AInv[2]*Mat_B[2];
		Parm2=Mat_AInv[3]*Mat_B[0]+Mat_AInv[4]*Mat_B[1]+Mat_AInv[5]*Mat_B[2];
		Parm3=Mat_AInv[6]*Mat_B[0]+Mat_AInv[7]*Mat_B[1]+Mat_AInv[8]*Mat_B[2];

		*Cx=(float)(-Parm1*0.5);	
		*Cy=(float)(-Parm2*0.5);	

		nMaxPos=nMinPos=nCount=0;
		dDistSum=dMaxDist=0;
		dMinDist=1000000;
		for(i=0;i<N;i++)
		{
			if(pflag[i]==0) continue;
			dDist=(x[i]-*Cx)*(x[i]-*Cx)+(y[i]-*Cy)*(y[i]-*Cy);
			if(dDist>dMaxDist)
			{
				dMaxDist=dDist;
				nMaxPos=i;
			}
			else if(dDist<dMinDist)
			{
				dMinDist=dDist;
				nMinPos=i;
			}
			dDistSum+=dDist;
			nCount++;
		}

		*AvgRadius=(float)sqrt(dDistSum/nCount);
		dMaxDist=(float)sqrt(dMaxDist);
		dMinDist=(float)sqrt(dMinDist);
		if(k==0)
		{
			*MaxRadius=dMaxDist;
			*MinRadius=dMinDist;
		}

		if((dMaxDist-*AvgRadius) > (*AvgRadius-dMinDist))
		{
			pflag[nMaxPos]=0;
			if((*AvgRadius-dMinDist)<=dLimit) break;
		}
		else
		{
			pflag[nMinPos]=0;
			if((dMaxDist-*AvgRadius)<=dLimit) break;
		}
	}

	nResult=0;  //OK

done:
	if(pflag) free(pflag);
	return nResult;
}


long CVisionLib::matrixInverse(float *a,int n,float *b)
{
	 int   i,j,k,l,l1;
	 float c,d;
   	 double SMALLEST=0.00000001;

	 for (i=0;i<n;i++) {
		 for (j=0; j<n;j++)
			 *(b+i*n+j) = 0.;
		 *(b+i*n+i) = 1.0;
	 }
	 for (l=0;l<n;l++){
		 d = (float)fabs((*(a+l*n+l)));
		 if (d<SMALLEST) {
			 l1 = l;
			 d = (float)fabs((*(a+l1*n+l)));
			 while (d < SMALLEST && (++l1<n))
				 d = (float)fabs((*(a+l1*n+l)));
			 if (l1>=n)
				 return -1;
			 for (j=0;j<n;j++){
				 *(a+l*n+j) += (*(a+l1*n+j));
				 *(b+l*n+j) += (*(b+l1*n+j));
			 }
		 }
		 c = (float)(1.0/(*(a+l*n+l)));
		 for (j=l;j<n;j++) *(a+l*n+j) *= c;
		 for (j=0;j<n;j++) *(b+l*n+j) *= c;
		 k = l+1;
		 for (i=k;i<n;i++){
			 c = *(a+i*n+l);
			 for (j=l;j<n;j++) *(a+i*n+j) -= (*(a+l*n+j) * c);
			 for (j=0;j<n;j++) *(b+i*n+j) -= (*(b+l*n+j) * c);
		 }
	 }
	 for (l=n-1; l>=0;l--){
		 for (k=1;k<=l;k++){
			 i = l-k;
			 c = *(a+i*n+l);
			 for (j=l;j<n;j++) *(a+i*n+j) -= (*(a+l*n+j) * c);
			 for (j=0;j<n;j++) *(b+i*n+j) -= (*(b+l*n+j) * c);
		 }
	 }
	 return 0;
}


//---------------------------------------------------------------
//     Timer 관련함수 
//----------------------------------------------------------------
void CVisionLib::SetClockTimer(long id)
{
	m_dSetTime[id] = GetTickCount();
}

double CVisionLib::GetClockTimer(long id)
{
    m_dEndTime[id] = GetTickCount();
    return (m_dEndTime[id] - m_dSetTime[id]) / 1000.0;
}
//---------------------------------------------------------------
//     Timer 관련함수 
//----------------------------------------------------------------

void CVisionLib::SaveTextFile(CString strPathName, CString strText)
{
	TCHAR szbuf[100];
	FILE *fp;

	_tprintf(szbuf, _T("%s"), strPathName);
    _tfopen_s(&fp, szbuf, _T("a+"));
  
	_ftprintf(fp, _T("%s \n"), strText);
	fclose(fp);	
}

BOOL CVisionLib::SaveBitmapImage(LPCTSTR strFileName, LPBYTE fm, long width, long height)
{
	CFile f;
	int i, j;

    if (!f.Open(strFileName, CFile::modeCreate|CFile::modeWrite)) return FALSE;

	// Image가 4의 배수인가 아닌가결정해서 아니면 아래와 같이 함.
	DWORD dwBytes = ((DWORD) width * 8) / 32;
	if(((DWORD) width * 8) % 32) 	dwBytes++;
	dwBytes *= 4;
	int SizeImage = dwBytes * height; 
	
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	int nSize =  sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 +  SizeImage;
	int nSizeBITMAPFILEHEADER=sizeof(BITMAPFILEHEADER);
	bmfh.bfSize = nSize + sizeof(BITMAPFILEHEADER);
	// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
					 sizeof(RGBQUAD) * 256;	
	
	BITMAPINFOHEADER bmih;
	bmih.biSize=40;
    bmih.biWidth=width; 		// BMP  폭
    bmih.biHeight=height; 		// BMP  높이
    bmih.biPlanes=1; 			// =1
    bmih.biBitCount=8; 			// 픽셀당 Bit 수  8Bit Gray,  24Bit RGB
    bmih.biCompression=BI_RGB;// ( No Compression 대부분이 이에 해당 )
	if((int)dwBytes!=width)
	{
		bmih.biSizeImage=SizeImage;    	 
		bmih.biXPelsPerMeter=0;  
		bmih.biYPelsPerMeter=0; 	 
		bmih.biClrUsed=256; 		 
		bmih.biClrImportant=256; 	 
	}
	else
	{
		bmih.biSizeImage=0;    	 
		bmih.biXPelsPerMeter=0;  
		bmih.biYPelsPerMeter=0; 	 
		bmih.biClrUsed=0; 		 
		bmih.biClrImportant=0; 	 
	}

	RGBQUAD rgbq[256];
	for(i=0;i<256;i++)
	{
		rgbq[i].rgbBlue=i; 
		rgbq[i].rgbGreen=i; 
		rgbq[i].rgbRed=i; 
		rgbq[i].rgbReserved=0; 
	}

	LPBYTE fmTmp= new BYTE[SizeImage];

	// BMP 파일로 Format으로 만듦.
	for(i=0;i<height;i++)
	for(j=0;j<width;j++)
		*(fmTmp+dwBytes*i+j)=*(fm+width*(height-i-1)+j);

	TRY {
		f.Write((LPVOID) &bmfh, nSizeBITMAPFILEHEADER);
		f.Write((LPVOID) &bmih, sizeof(BITMAPINFOHEADER));
		f.Write((LPVOID) rgbq, sizeof(RGBQUAD)*256);
		f.Write((LPVOID) fmTmp, SizeImage);

		f.Close();
	}
	CATCH (CException, e) {
		return FALSE;
	}
	END_CATCH


	delete fmTmp;
	return TRUE;
}


