
#include "stdafx.h"
#include "math.h"

// Template를 Rotate하는 함수 (160msec)
int MakeRotatedTemp(LPBYTE fmS, int left_S, int top_S, int right_S, int bottom_S, 
					LPBYTE fmD, int left_D, int top_D, int right_D, int bottom_D, 
					int nPitch, BYTE color, double Cx, double Cy, double angle)
{
	int i,j,N,M, count=0;
	int Dum;
	double a,b,pX,pY;
	double cosAngle, sinAngle;	

    cosAngle=cos(angle*3.141592/180.0);
    sinAngle=sin(angle*3.141592/180.0);

	for(i=top_D;i<=bottom_D;i++)
    for(j=left_D;j<=right_D;j++)
    {  		    
        pX= cosAngle*(j-Cx)+sinAngle*(i-Cy)+Cx;
		pY=-sinAngle*(j-Cx)+cosAngle*(i-Cy)+Cy;

		N=(int)pX;
		M=(int)pY;
		a=pX-N;
		b=pY-M;
		Dum=M*nPitch+N;
		if(N==right_S) N=right_S-1;
		if(M==bottom_S) M=bottom_S-1;

		if(N<left_S || N>=right_S || M<top_S ||M>=bottom_S)
			*(fmD+nPitch*i+j)=color;
		else
			*(fmD+nPitch*i+j)=(unsigned char)(*(fmS+Dum)*(1.0-a)*(1.0-b)+
											  *(fmS+Dum+1)*(a)*(1.0-b)+
											  *(fmS+Dum+nPitch)*(1.0-a)*(b)+
											  *(fmS+Dum+nPitch+1)*(a)*(b));
    }

    return 0;
}
