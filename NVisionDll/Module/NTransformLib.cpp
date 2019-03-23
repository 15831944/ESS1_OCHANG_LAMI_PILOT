// NTransformLib.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NTransformLib.h"
#include "math.h"


// CNTransformLib

IMPLEMENT_DYNAMIC(CNTransformLib, CWnd)

CNTransformLib::CNTransformLib()
{
	m_pLocalImage = NULL;
}

CNTransformLib::~CNTransformLib()
{
}


BEGIN_MESSAGE_MAP(CNTransformLib, CWnd)
END_MESSAGE_MAP()



// CNTransformLib 메시지 처리기입니다.

/////////////////////////////////////////////////////////////////////////////
// CNTransformLib message handlers
// fm : Frame Memory 시작번지
// (left, top)-(right,bottom) : ROI
//  : Frame 폭 
// nSlope : 최소 Edge Slope 
// (*cx, *cy) : Circle Center
// return 1 : Success
//       <0 : Failure
long CNTransformLib::HoughCircle(unsigned char *fm, long left, long top, long right, long bottom, 
				long nObjectColor, long nDiameter, double *cx, double *cy, long nWidth, long nHeight)
{
	long i, j, k, ok=1;
	long nEdgeSense=(long)(sqrt((double)(2*765*765))*7/100);
	double gx,gy, gl;
	long xc, yc;
	long local_i, local_j, local_nPitch=(right-left+1), local_height=(bottom-top+1);
	long *pHough, nHough;
	long nHoughMax=0, Maxi, Maxj,  X_Start, X_End, Y_Start, Y_End;
	long nOffset;
	double sumx,sumy;
	double sumhough;
	long nThreshold;
	long nR1=(long)(nDiameter*0.2);
	long nR2=(long)(nDiameter*0.8);

	nOffset=(long)(nDiameter*0.2);

	if(nDiameter<50){
		nThreshold=20;
	}
	else if(nDiameter<100){
		nThreshold=25;
	}
	else{
		nThreshold=35;
	}

	pHough= (long*)malloc((right-left+1)*(bottom-top+1)*sizeof(long));

	for(i=0;i<local_height;i++)
		for(j=0;j<local_nPitch;j++)
			*(pHough+local_nPitch*i+j)=0;

	local_i=0;
	for(i=top+1;i<bottom;i++)
	{	
		local_i++;
		local_j=0;
		for(j=left+1;j<right;j++)
		{
			local_j++;
			gx= *(fm+nWidth*(i-1)+(j+1))-*(fm+nWidth*(i-1)+(j-1))+
				*(fm+nWidth*i+(j+1))-*(fm+nWidth*i+(j-1))+
				*(fm+nWidth*(i+1)+(j+1))-*(fm+nWidth*(i+1)+(j-1));
			gy= *(fm+nWidth*(i+1)+(j-1))-*(fm+nWidth*(i-1)+(j-1))+
				*(fm+nWidth*(i+1)+(j))-*(fm+nWidth*(i-1)+(j))+
				*(fm+nWidth*(i+1)+(j+1))-*(fm+nWidth*(i-1)+(j+1));
			if(nObjectColor==0)
			{
				gx*=(-1.0);
				gy*=(-1.0);
			}
			gl= sqrt(gx*gx+gy*gy);
			if(gl>=nEdgeSense)
			{
				for(k=nR1;k<=nR2;k++)
				{
					xc=(long)(local_j+k*(gx/gl)+0.5);
					yc=(long)(local_i+k*(gy/gl)+0.5);
					if(xc>0 && xc<local_nPitch && yc>0 && yc<local_height)
						*(pHough+local_nPitch*yc+xc)+=1;				
				}
			}	
		}
	}	

	//------------------------------중심 구하기 ---------------------------
	for(i=0;i<local_height;i++)
		for(j=0;j<local_nPitch;j++)
			if(*(pHough+local_nPitch*i+j)>nHoughMax)
			{
				nHoughMax=*(pHough+local_nPitch*i+j);
				Maxi=i;
				Maxj=j;
			}

	X_Start=Maxj-nOffset;
	if(X_Start<0) X_Start=0;
	X_End=Maxj+nOffset;
	if(X_End>=local_nPitch) X_End=local_nPitch-1;
	Y_Start=Maxi-nOffset;
	if(Y_Start<0) Y_Start=0;
	Y_End=Maxi+nOffset;
	if(Y_End>=local_height) Y_End=local_height-1;

	sumx=sumy=sumhough=0;
	for(i=Y_Start;i<=Y_End;i++)
		for(j=X_Start;j<=X_End;j++)
		{
			nHough=*(pHough+local_nPitch*i+j);
			if(nHough>nThreshold)
			{
				sumx+=(nHough*j);
				sumy+=(nHough*i);
				sumhough+=nHough;
			}
		}

	if(sumhough>0)
	{
		*cx=sumx/sumhough+left;
		*cy=sumy/sumhough+top;
	}
	else
	{
		ok=-1;
	}
	//--------------------------------------------------------------

	free(pHough);
	return ok;
}

// Digital 회전 함수
// SourImage : 원 영상의 메모리 
// DestImage : 변환된 영상 메모리 
// x1, y1, x2, y2 : 변환하고자하는 영상의 위치 
// angle : 회전각도 지정 
void CNTransformLib::ImageRotation(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, float angle, long nWidth, long nHeight)
{
	long  i, j, x, y, data;
    float cos_value, sin_value;
	CPoint cp, tp;
	unsigned char *Buffer = new unsigned char[nHeight*nWidth];

	cp.x = (x1+x2)/2;
	cp.y = (y1+y2)/2;

    cos_value = (float)cos(angle*RAD);              
    sin_value = (float)sin(angle*RAD);            

    for(i=0; i<nHeight; i++) 
    for(j=0; j<nWidth ; j++) {
		Buffer[i*nWidth + j] = 0;
	}

    for(i=y1; i<=y2; i++) 
    for(j=x1; j<=x2; j++) {
		data = SourImage[i*nWidth + j];

        y=(long)((j-cp.x)*sin_value + (i-cp.y)*cos_value + cp.y);             
        x=(long)((j-cp.x)*cos_value - (i-cp.y)*sin_value + cp.x);            

		if (x<0 || x>nWidth-1 || y<0 || y>nHeight-1) continue;
        Buffer[(long)y*nWidth + (long)x] = (unsigned char)data;
    }

    for(i=0; i<nHeight; i++) 
    for(j=0; j<nWidth ; j++) {
		DestImage[i*nWidth + j] = Buffer[i*nWidth + j];
	}

    for(i=1; i<nHeight-1; i++) 
    for(j=1; j<nWidth-1 ; j++) {
		data = Buffer[i*nWidth + j];
		data += Buffer[(i-1)*nWidth + (j-1)];
		data += Buffer[(i-1)*nWidth + (j+1)];
		data += Buffer[(i+1)*nWidth + (j-1)];
		data += Buffer[(i+1)*nWidth + (j+1)];

		DestImage[i*nWidth + j] = (unsigned char)(data/5);
	}

	delete Buffer;
}

// Digital Zoom 함수
// SourImage : 원 영상의 메모리 
// DestImage : 변환된 영상 메모리 
// x1, y1, x2, y2 : 변환하고자하는 영상의 위치 
// zoom_x : X축으로 Zooming Scale 
// zoom_y : Y축으로 Zooming Scale 
void CNTransformLib::ImageZoom(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, float zoom_x, float zoom_y, long nWidth, long nHeight)
{
    long i, j, data1, data2;
	float x, y, p, q;
	long value;
	CPoint cp;

	if (x1 < 0) x1 = 0;
	if (x2 > nWidth-1)  x2 = nWidth-1;

	if (y1 < 0) y1 = 0;
	if (y2 > nHeight-1) y2 = nHeight-1;

	for(i=0; i<nHeight; i++)
	for(j=0; j<nWidth ; j++) {
		DestImage[i*nWidth + j] = 0;
	}

    for(i=y1; i<=y2; i++) 
    for(j=x1; j<=x2; j++) {
        y=i/zoom_y;             
        x=j/zoom_x;            

		if (y>0) data1 = (long)y;
		else     data1 = (long)y-1;

		if (x>0) data2 = (long)x;
		else     data2 = (long)x-1;

		q = y-data1;
		p = x-data2;

		if (data1<0 || data1>nHeight-1 || data2<0 || data2>nWidth-1) 
			value = 0; 
		else {
			value = (long)((1.0-q) * ((1.0-p)*SourImage[data1*nWidth + data2]
						+ p * SourImage[data1*nWidth + data2+1])
						+ q * ((1.0-p) * SourImage[(data1+1)*nWidth + data2]
						+ p * SourImage[(data1+1)*nWidth + data2+1]));
		}

		if (value < 0)   value = 0;
		if (value > 255) value = 255;

		DestImage[i*nWidth + j] = (unsigned char)value;
    }

	return;
}

// 원을 직사각형으로 편는 Open함수(변수 초기화)
// cx, cy : 원의 중심 좌표 
// start_angle, end_angle : 직사각형으로 변환하고자 하는 초기 각도와 끝의 각도 
// in_radius, out_radius : 원의 중심에서 안쪽 반경 이상부터 밖의 반경까지를 직사각의 높이로 변환 
// interval_angle : 샘플링 각도 간격 지정     
void CNTransformLib::ImageRectToPolarOpen(long nPosX, long nPosY, float dStartAngle, float dEndAngle, float dInRadius, float dOutRadius, float dIntervalAngle,
						                 long *nWidth, long *nHeight)
{
	m_nCenterX = nPosX;
	m_nCenterY = nPosY;
	m_dStartAngle = dStartAngle;
	m_dEndAngle = dEndAngle;
	m_dInRadius = dInRadius;
	m_dOutRadius = dOutRadius;
	m_dIntervalAngle = dIntervalAngle;

	*nHeight  = (long)(m_dOutRadius-m_dInRadius);
	*nWidth = (long)((m_dEndAngle-m_dStartAngle) / m_dIntervalAngle);
}

// 원을 직사각형으로 편는 함수(주 함수)
// SourImage : 원 영상 메모리 
// DestImage : 변환된 영상  
// rect_height : 변환된 영상의 높이 
// rect_width : 변환된 영상의 폭 
void CNTransformLib::ImageRectToPolar(LPBYTE pSourImage, long nOrgWidth, long nOrgHeight, LPBYTE pDestImage, long nPolWidth, long nPolHeight)
{
    long  i, j, k, nEnd;
	float dAngle, dTempA, dTempB;
	float dTx, dTy, dDx, dDy;
	long  nTx, nTy;

	k=0;
    for(dAngle=m_dStartAngle; dAngle<m_dEndAngle; dAngle+=m_dIntervalAngle) 
	{
		dTx = m_nCenterX + (float)cos((double)dAngle*RAD) * m_dOutRadius;   
		dTy = m_nCenterY + (float)sin((double)dAngle*RAD) * m_dOutRadius;   

    	dTempA = (dTx - m_nCenterX);
		dTempB = (dTy - m_nCenterY);

    	dDx = dTempA / m_dOutRadius;
		dDy = dTempB / m_dOutRadius;

		nTx = (long)(m_nCenterX + m_dOutRadius*dDx);
		nTy = (long)(m_nCenterY + m_dOutRadius*dDy);
		if (nTx<0 || nTx>=nOrgWidth || nTy<0 || nTy>=nOrgHeight) 
		{
			for(i=0; i<nPolHeight; i++)
				pDestImage[i*nPolWidth + k] = 0;
		}
		else
		{
			j=0;
			nEnd = (long)m_dInRadius+nPolHeight;
			for(i=(long)m_dInRadius; i<nEnd; i++) 
			{
				nTx = (long)(m_nCenterX + i*dDx);
				nTy = (long)(m_nCenterY + i*dDy);
            
				pDestImage[j*nPolWidth + k] = pSourImage[nTy*nOrgWidth + nTx];
				j++;
			}
		}

		k++;
    }
}

// 원을 직사각형으로 편는 함수(메모리 해제, Close 함수)
void CNTransformLib::ImageRectToPolarClose()
{

}

LPBYTE CNTransformLib::OpenTransTiltToRect(POINT2 ptPos1, POINT2 ptPos2, POINT2 ptPos3, POINT2 ptPos4, long *nWidth, long *nHeight) 
{
	float a, b;
	long nW, nH;

    a = ptPos3.x-ptPos1.x;
	b = ptPos3.y-ptPos1.y;
	nW = (long)(sqrt(a*a + b*b)+0.5);
	if (nW<0) nW = 0;

    a = ptPos2.x-ptPos1.x;
	b = ptPos2.y-ptPos1.y;
	nH = (long)(sqrt(a*a + b*b)+0.5);
	if (nH<0) nH = 0;

    *nWidth = nW;
	*nHeight = nH;

	if (m_pLocalImage) 
	{
		delete m_pLocalImage;
		m_pLocalImage = NULL;
	}

	if (nW>0 && nH>0)
		m_pLocalImage = new unsigned char[nW*nH];

	return m_pLocalImage;
}

void CNTransformLib::ExecTransTiltToRect(LPBYTE pSourImage, POINT2 ptPos1, POINT2 ptPos2, POINT2 ptPos3, POINT2 ptPos4, long nWidth, long nHeight) 
{
	long  i, j, nValue;
	long  nLocalWidth, nLocalHeight, x, y;
	float a, b, dTx, dTy, dPosX, dPosY;
	float dVertDx, dVertDy, dHeriDx, dHeriDy;

    a = ptPos3.x-ptPos1.x;
	b = ptPos3.y-ptPos1.y;
	nLocalWidth = (long)(sqrt(a*a + b*b)+0.5);
	if (nLocalWidth>0)
	{
		dHeriDx = a/nLocalWidth;
		dHeriDy = b/nLocalWidth;
	}

    a = ptPos2.x-ptPos1.x;
	b = ptPos2.y-ptPos1.y;
	nLocalHeight = (long)(sqrt(a*a + b*b)+0.5);
	if (nLocalHeight>0)
	{
		dVertDx = a/nLocalHeight;
		dVertDy = b/nLocalHeight;
	}

	for(i=0; i<nLocalHeight; i++) 
	{
		dPosY = ptPos1.y + i*dVertDy;
		dPosX = ptPos1.x + i*dVertDx;
//#pragma omp parallel for
		for(j=0; j<nLocalWidth; j++) 
		{
			dTy = dPosY + j*dHeriDy;
			dTx = dPosX + j*dHeriDx;

			x = (long)dTx;
			y = (long)dTy;

	    	a = dTx-x;
			b = dTy-y;

			nValue = (long)((1.0-b) * ((1.0-a)*pSourImage[y*nWidth + x]
				+ a * pSourImage[y*nWidth + (x+1)])
				+ b * ((1.0-a) * pSourImage[(y+1)*nWidth + x]
				+ a * pSourImage[(y+1)*nWidth + (x+1)]));

			m_pLocalImage[i*nLocalWidth + j] = (unsigned char)nValue;
		}
	}
}

void CNTransformLib::CloseTransTiltToRect() 
{
    if (m_pLocalImage)
	{
		delete m_pLocalImage;
		m_pLocalImage = NULL;
	}
}


