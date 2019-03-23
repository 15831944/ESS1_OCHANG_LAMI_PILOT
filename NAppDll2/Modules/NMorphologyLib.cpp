// NMorphologyLib.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NMorphologyLib.h"

// CNMorphologyLib

IMPLEMENT_DYNAMIC(CNMorphologyLib, CWnd)

CNMorphologyLib::CNMorphologyLib()
{
     m_White=255;    // Default Value
	 m_Black=0;
}

CNMorphologyLib::~CNMorphologyLib()
{
}

BEGIN_MESSAGE_MAP(CNMorphologyLib, CWnd)
END_MESSAGE_MAP()

// CNMorphologyLib 메시지 처리기입니다.
/////////////////////////////////////////////////////////////////////////////
// CNMorphologyLib message handlers
long CNMorphologyLib::DilateBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat)
{
	long pKernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	long nKernelSizeX = 3;
	long nKernelSizeY = 3;
    long Ok, i, j, count;

	unsigned char *Buffer;
	Buffer = new unsigned char[nWidth * nHeight];

	if (x1<0 || y1<0 || x2>=nWidth || y2>=nHeight) 
	{
		Ok = MORPH_ERROR_WIDTH;
		goto end;
	}

	memcpy(Buffer, fmSour, nWidth*nHeight); 
	for(count=0; count<repeat; count++) 
	{
		long k,l;
		long nOrgX, nOrgY;

		nOrgX=(long)(nKernelSizeX/2.0-0.5);
		nOrgY=(long)(nKernelSizeY/2.0-0.5);

		//BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
		for(i=0;i<=nOrgY;i++) 
		for(j=0;j<nWidth;j++)
		{
		  *(fmDest+nWidth*i+j)=*(Buffer+nWidth*i+j);
		  *(fmDest+nWidth*(nHeight-1-i)+j)=*(Buffer+nWidth*(nHeight-1-i)+j);
		}

		for(j=0; j<=nOrgX; j++)
		for(i=y1; i<=y2; i++) 
		{
		  *(fmDest+nWidth*i+j)=*(Buffer+nWidth*i+j);
		  *(fmDest+nWidth*i+(nWidth-1-j))=*(Buffer+nWidth*i+(nWidth-1-j));
		}
		//END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		for(i=y1; i<=y2-nKernelSizeY; i++)         //전체영역이 10x10이고 kernel 이 4x4이면 X,Y범위는 (0,0)~(6,6) 이다.
		for(j=x1; j<=x2-nKernelSizeX; j++)      //                     
		{
		   for(k=0;k<nKernelSizeY;k++)
		   for(l=0;l<nKernelSizeX;l++)
			   if( *(pKernel+nKernelSizeX*k+l) )                
				   if( *(Buffer+(i+k)*nWidth+j+l) )
				   {
					   *(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=(unsigned char)m_White;
					   goto LOOP;
				   }
   
		   *(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=(unsigned char)m_Black;					   
		LOOP:      continue;
		}

		if (count < repeat-1) 
		{
			for(i=y1; i<=y2; i++)
    		for(j=x1; j<=x2; j++) {
	    		Buffer[i*nWidth + j] = fmDest[i*nWidth + j];
			}
		}
	}

    Ok = MORPH_OK;

end:
	delete Buffer;
    return Ok;
}

long CNMorphologyLib::ErodeBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat)
{
	long pKernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	long nKernelSizeX = 3;
	long nKernelSizeY = 3;
	long Ok, i, j, count;

	unsigned char *Buffer;
	Buffer = new unsigned char[nWidth * nHeight];

	if (x1<0 || y1<0 || x2>=nWidth || y2>=nHeight) 
	{
		Ok = MORPH_ERROR_WIDTH;
		goto end;
	}

	memcpy(Buffer, fmSour, nWidth*nHeight); 
	for(count=0; count<repeat; count++) 
	{
		long k,l;
		long nOrgX, nOrgY;

		nOrgX=(long)(nKernelSizeX/2.0-0.5);
		nOrgY=(long)(nKernelSizeY/2.0-0.5);

		//BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
		for(i=0; i<=nOrgY; i++) 
		for(j=x1; j<=x2; j++)
		{
		  *(fmDest+nWidth*i+j)=*(Buffer+nWidth*i+j);
		  *(fmDest+nWidth*(nHeight-1-i)+j)=*(Buffer+nWidth*(nHeight-1-i)+j);
		}

		for(j=0; j<=nOrgX; j++)
		for(i=y1; i<=y2; i++) 
		{
		  *(fmDest+nWidth*i+j)=*(Buffer+nWidth*i+j);
		  *(fmDest+nWidth*i+(nWidth-1-j))=*(Buffer+nWidth*i+(nWidth-1-j));
		} 
		//END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		for(i=y1; i<=y2-nKernelSizeY; i++)     // 전체영역이 10x10이고 kernel 이 4x4이면 X,Y범위는 (0,0)~(6,6) 이다.
		for(j=x1; j<=x2-nKernelSizeX; j++)      //                     
		{
		   for(k=0;k<nKernelSizeY;k++)
		   for(l=0;l<nKernelSizeX;l++)
			   if( *(pKernel+nKernelSizeX*k+l) )                
				   if( !*(Buffer+(i+k)*nWidth+j+l) )
				   {
					   *(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=(unsigned char)m_Black;
					   goto LOOP;
				   }
   
		   *(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=(unsigned char)m_White;					   
		LOOP:      continue;
		}

		if (count < repeat-1) 
		{
			for(i=y1; i<=y2; i++)
    		for(j=x1; j<=x2 ; j++)
			{
	    		Buffer[i*nWidth + j] = fmDest[i*nWidth + j];
			}
		}
	}

    Ok = MORPH_OK;

end:
	delete Buffer;
    return Ok;
}

long CNMorphologyLib::OpenBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat)
{
   	LPBYTE fmTmp = new unsigned char[nWidth*nHeight];

    ErodeBinary(fmSour, fmTmp, x1, y1, x2, y2, nWidth, nHeight, repeat);
    DilateBinary(fmTmp, fmDest, x1, y1, x2, y2, nWidth, nHeight, repeat);

	delete fmTmp;
	return MORPH_OK;
}

long CNMorphologyLib::CloseBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat)
{
   	LPBYTE fmTmp = new unsigned char[nWidth*nHeight];

    DilateBinary(fmSour, fmTmp, x1, y1, x2, y2, nWidth, nHeight, repeat);
    ErodeBinary(fmTmp, fmDest, x1, y1, x2, y2, nWidth, nHeight, repeat);

	delete fmTmp;
	return MORPH_OK;
}

long CNMorphologyLib::DilateGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat)
{
	long pKernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	long nKernelSizeX = 3;
	long nKernelSizeY = 3;
    long Ok, i, j, count;

	unsigned char *Buffer;
	Buffer = new unsigned char[nWidth * nHeight];

	if (x1<0 || y1<0 || x2>=nWidth || y2>=nHeight) 
	{
		Ok = MORPH_ERROR_WIDTH;
		goto end;
	}

	memcpy(Buffer, fmSour, nWidth*nHeight); 
	for(count=0; count<repeat; count++) 
	{
		long k,l;
		long nOrgX, nOrgY;
		long Kernel_ALLZero=0;
		long MaxValue;

		nOrgX=(long)(nKernelSizeX/2.0-0.5);
		nOrgY=(long)(nKernelSizeY/2.0-0.5);

		//BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
		for(i=0;i<=nOrgY;i++) 
		for(j=x1; j<=x2; j++)
		{
			*(fmDest+nWidth*i+j)=*(Buffer+nWidth*i+j);
			*(fmDest+nWidth*(nHeight-1-i)+j)=*(Buffer+nWidth*(nHeight-1-i)+j);
		}

		for(j=0;j<=nOrgX;j++)
		for(i=y1; i<=y2; i++) 
		{
			*(fmDest+nWidth*i+j)=*(Buffer+nWidth*i+j);
			*(fmDest+nWidth*i+(nWidth-1-j))=*(Buffer+nWidth*i+(nWidth-1-j));
		}
		//END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		for(k=0;k<nKernelSizeY;k++)
		for(l=0;l<nKernelSizeX;l++)
		   if( *(pKernel+nKernelSizeX*k+l) ) Kernel_ALLZero=1;
  
	   if(Kernel_ALLZero==0) // 모든 kernel의 값이 ZERO이면 
	   {
		  for(i=y1;i<=y2-nKernelSizeY;i++)         
 		 for(j=x1;j<=x2-nKernelSizeX;j++)                           
		 {
			MaxValue=0;
			for(k=0;k<nKernelSizeY;k++)
			   for(l=0;l<nKernelSizeX;l++)
				   if( *(Buffer+(i+k)*nWidth+j+l) > MaxValue)
				   {
						MaxValue=*(Buffer+(i+k)*nWidth+j+l);
				   }
						   
				*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=(unsigned char)MaxValue;
		   }
	   }
	   else  //kernel이 어떤 값을 가지고 있을때
	   {
		  for(i=y1;i<=y2-nKernelSizeY;i++)         
 			 for(j=x1;j<=x2-nKernelSizeX;j++)                           
			 {
				MaxValue=0;
				for(k=0;k<nKernelSizeY;k++)
				   for(l=0;l<nKernelSizeX;l++)
					   if( *(Buffer+(i+k)*nWidth+j+l)+*(pKernel+nKernelSizeX*k+l) > MaxValue)
					   {
							MaxValue=*(Buffer+(i+k)*nWidth+j+l)+*(pKernel+nKernelSizeX*k+l);
					   }
				if(	MaxValue>255) MaxValue=255;	   
				*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=(unsigned char)MaxValue;
		   }
	   }

		if (count < repeat-1) {
			for(i=y1; i<=y2; i++)
    		for(j=x1; j<=x2; j++) {
	    		Buffer[i*nWidth + j] = fmDest[i*nWidth + j];
			}
		}
	}

    Ok = MORPH_OK;

end:
	delete Buffer;
    return Ok;
}

long CNMorphologyLib::ErodeGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat)
{
	long pKernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	long nKernelSizeX = 3;
	long nKernelSizeY = 3;
	long Ok, i, j, count;

	unsigned char *Buffer;
	Buffer = new unsigned char[nWidth * nHeight];

	if (x1<0 || y1<0 || x2>=nWidth || y2>=nHeight) 
	{
		Ok = MORPH_ERROR_WIDTH;
		goto end;
	}

	memcpy(Buffer, fmSour, nWidth*nHeight); 
	for(count=0; count<repeat; count++) 
	{
		long k,l;
		long nOrgX, nOrgY;
		long Kernel_ALLZero=0;
		long MinValue;

		nOrgX=(long)(nKernelSizeX/2.0-0.5);
		nOrgY=(long)(nKernelSizeY/2.0-0.5);

		//BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
		for(i=0;i<=nOrgY;i++) 
		for(j=x1;j<=x2;j++)
		{
		  *(fmDest+nWidth*i+j)=*(Buffer+nWidth*i+j);
		  *(fmDest+nWidth*(nHeight-1-i)+j)=*(Buffer+nWidth*(nHeight-1-i)+j);
		}

		for(j=0;j<=nOrgX;j++)
		for(i=y1; i<=y2; i++) 
		{
		  *(fmDest+nWidth*i+j)=*(Buffer+nWidth*i+j);
		  *(fmDest+nWidth*i+(nWidth-1-j))=*(Buffer+nWidth*i+(nWidth-1-j));
		}
		//END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		for(k=0;k<nKernelSizeY;k++)
		for(l=0;l<nKernelSizeX;l++)
		   if( *(pKernel+nKernelSizeX*k+l) ) Kernel_ALLZero=1;

		if (Kernel_ALLZero==0) // 모든 kernel의 값이 ZERO이면 
		{
			 for(i=y1;i<=y2-nKernelSizeY;i++)         
			 for(j=x1;j<=x2-nKernelSizeX;j++)                           
			 {
				MinValue=255;
				for(k=0;k<nKernelSizeY;k++)
			   for(l=0;l<nKernelSizeX;l++)
				   if(*(Buffer+(i+k)*nWidth+j+l) < MinValue)
				   {
						MinValue=*(Buffer+(i+k)*nWidth+j+l);
				   }
						   
				*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=(unsigned char)MinValue;
			}
		}
		else  //kernel이 어떤 값을 가지고 있을때
		{
			 for(i=y1;i<=y2-nKernelSizeY;i++)         
			 for(j=x1;j<=x2-nKernelSizeX;j++)                           
			 {
				MinValue=255;
				for(k=0;k<nKernelSizeY;k++)
			   for(l=0;l<nKernelSizeX;l++)
				   if( *(Buffer+(i+k)*nWidth+j+l)-*(pKernel+nKernelSizeX*k+l) < MinValue)
				   {
						MinValue=*(Buffer+(i+k)*nWidth+j+l)-*(pKernel+nKernelSizeX*k+l);
				   }
				if(	MinValue<0) MinValue=0;	   
				*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=(unsigned char)MinValue;
			}
		}

		if (count < repeat-1) {
			for(i=y1; i<=y2; i++)
    		for(j=x1; j<=x2 ; j++) {
	    		Buffer[i*nWidth + j] = fmDest[i*nWidth + j];
			}
		}
	}

    Ok = MORPH_OK;

end:
	delete Buffer;
    return Ok;
}

long CNMorphologyLib::OpenGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat)
{
   	LPBYTE fmTmp = new unsigned char[nWidth*nHeight];

    ErodeGray(fmSour, fmTmp, x1, y1, x2, y2, nWidth, nHeight, repeat);
    DilateGray(fmTmp, fmDest, x1, y1, x2, y2, nWidth, nHeight, repeat);

	delete fmTmp;
	return MORPH_OK;
}

long CNMorphologyLib::CloseGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat)
{
   	LPBYTE fmTmp = new unsigned char[nWidth*nHeight];

    DilateGray(fmSour, fmTmp, x1, y1, x2, y2, nWidth, nHeight, repeat);
    ErodeGray(fmTmp, fmDest, x1, y1, x2, y2, nWidth, nHeight, repeat);

	delete fmTmp;
	return MORPH_OK;
}

