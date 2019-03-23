// NVisionLib.cpp : implementation file
//
#include "stdafx.h"
#include "NVisionLib.h"
#include "math.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NVisionLib
CNVisionLib::CNVisionLib()
{
}

CNVisionLib::~CNVisionLib()
{
}

BOOL CNVisionLib::Open(long nType, long nWidth, long nHeight)
{
	return FALSE;
}

BOOL CNVisionLib::Close()
{
	return FALSE;
}

BOOL CNVisionLib::LoadImageFromFile(LPCTSTR strFileName, LPBYTE pImage, long nWidth, long nHeight)
{
	return FALSE;
}

BOOL CNVisionLib::SaveImageToFile(LPCTSTR strFileName, LPBYTE fm, long width, long height)
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


////////////////////////////////////////////////////////////////////////
//                          Mil Morpology
////////////////////////////////////////////////////////////////////////
BOOL CNVisionLib::BinarizeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh)
{
	for(long i=y1; i<=y2; i++)
	for(long j=x1; j<=x2; j++) 
	{
		if (fmSourImage[i*nWidth + j] > nThresh)
            fmTargImage[i*nWidth + j] = WHITE_LEVEL;
		else
			fmTargImage[i*nWidth + j] = BLACK_LEVEL;
	}

	return FALSE;
}

BOOL CNVisionLib::SmoothProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter)
{

	return FALSE;
}

BOOL CNVisionLib::SharpnessProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	return FALSE;
}

BOOL CNVisionLib::SobelEdgeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	return FALSE;
}

BOOL CNVisionLib::EdgeDetectProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh)
{
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
//                          Image Rotate
////////////////////////////////////////////////////////////////////////
long CNVisionLib::ImageRotation(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float angle) 
{
	long nLen;
	int i,j,N,M, count=0;
	int Dum;
	double a,b,pX,pY;
	double cosAngle, sinAngle, Cx, Cy;

	if (abs(x2-x1)>abs(y2-y1)) nLen = (x2-x1)/2;
	else nLen = (y2-y1)/2;

    cosAngle=cos(angle*3.141592/180.0);
    sinAngle=sin(angle*3.141592/180.0);

	Cx = (x1+x2)/2.0;
	Cy = (y1+y2)/2.0;

	x1 = (long)Cx-nLen;
	y1 = (long)Cy-nLen;
	x2 = (long)Cx+nLen;
	y2 = (long)Cy+nLen;
	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth) x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	for(i=y1;i<=y2;i++)
    for(j=x1;j<=x2;j++)
    {  		    
        pX= cosAngle*(j-Cx)+sinAngle*(i-Cy)+Cx;
		pY=-sinAngle*(j-Cx)+cosAngle*(i-Cy)+Cy;

		N=(int)pX;
		M=(int)pY;
		a=pX-N;
		b=pY-M;
		Dum=M*nWidth+N;
		if(N==x2) N=x2-1;
		if(M==y2) M=y2-1;

		if(N<5 || N>nWidth-5 || M<5 ||M>nHeight-5) continue;
		*(pDestImage+nWidth*i+j)=(unsigned char)(*(pSourImage+Dum)*(1.0-a)*(1.0-b)+
							  *(pSourImage+Dum+1)*(a)*(1.0-b)+
							  *(pSourImage+Dum+nWidth)*(1.0-a)*(b)+
							  *(pSourImage+Dum+nWidth+1)*(a)*(b));
    }
	
	return FALSE;
}

float CNVisionLib::GetSharpnessValue(LPBYTE pSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight)
{
	long i, j, nDiffer;
	long nIndex = 0;
	long nSum = 0;

	for(i=y1; i<y2; i++) 
	for(j=x1+1; j<x2; j++)
	{
		nDiffer = pSourImage[i*nWidth + (j-1)] - pSourImage[i*nWidth + j];
		nSum += nDiffer * nDiffer;
		nIndex++;
	}

	for(i=x1; i<x2; i++) 
	for(j=y1+1; j<y2; j++)
	{
		nDiffer = pSourImage[(j-1)*nWidth + i] - pSourImage[j*nWidth + i];
		nSum += nDiffer * nDiffer;
		nIndex++;
	}

	// normalize & return
	if (nIndex<1) return 0.0;

	return (float)((float)nSum/(float)nIndex);
}

void CNVisionLib::GetProjectFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX)
{
    long i, j, nNum;
//	long *pTemp = new long[nHeight];

	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth)  x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	for(i=0; i<nWidth; i++) 
	{
		ProjX[i] = 0;
//		pTemp[i] = 0;
	}

	for(i=x1; i<=x2; i++) 
	{
		nNum = 0;
		for(j=y1; j<=y2; j++) 
		{
		    ProjX[i] += pImage[j*nWidth + i];
			nNum++;
		}

		if (nNum > 0) ProjX[i] /= nNum;
    }
/*
	ProjX[x1] = pTemp[x1];
	ProjX[x2] = pTemp[x2];

	for(i=x1+1; i<=x2-1; i++)
		ProjX[i] = (pTemp[i-1]+pTemp[i]+pTemp[i+1])/3;

	delete pTemp;*/
}

void CNVisionLib::GetProjectFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY)
{
    long i, j, nNum;
//	long *pTemp = new long[nHeight];

	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth) x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	for(i=0; i<nHeight; i++) {
		ProjY[i] = 0;
//		pTemp[i] = 0;
	}

	for(i=y1; i<=y2; i++) 
	{
	    nNum = 0;
		for(j=x1; j<=x2; j++) 
		{
		    ProjY[i] += (long)pImage[i*nWidth + j];
		    nNum++;
		}

		if (nNum > 0) 
			ProjY[i] /= nNum;
    }
/*
	ProjY[y1] = pTemp[y1];
	ProjY[y2] = pTemp[y2];

	for(i=y1+1; i<=y2-1; i++)
		ProjY[i] = (pTemp[i-1]+pTemp[i]+pTemp[i+1])/3;

	delete pTemp;
*/
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

void CNVisionLib::GetProjectDfDtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjX)
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
}

void CNVisionLib::GetProjectDfDtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjY)
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
	for(i=y1+1; i<y2; i++)
		pTemp[i] = (ProjY[i-1]+ProjY[i]+ProjY[i+1])/3;

	for(i=0; i<nHeight; i++) ProjY[i] = 0;
	for(i=y1+nSide; i<=y2-nSide; i++)
		ProjY[i] = pTemp[i+nSide] - pTemp[i-nSide];

	delete pTemp;
}

void CNVisionLib::ResetTimer(long nID)
{
	if (nID<TIMER_MAX_NUM)
	    m_dSetTime[nID] = GetTickCount();
}

double CNVisionLib::GetTimer(long nID)
{
	double dTime=0.0;

	if (nID<TIMER_MAX_NUM)
	{
		m_dEndTime[nID] = GetTickCount();
		dTime = (m_dEndTime[nID] - m_dSetTime[nID]) / 1000.0;
	}

	return dTime;
}

void CNVisionLib::ZoomingImage(LPBYTE pSour, LPBYTE pDest, float fDispX, float fDispY, long nIWidth, long nIHeight, long nFWidth, long nFHeight, float fScale)
{
	long i, j;
	float x, y;

	y = fDispY;
	for(i=0; i<nFHeight; i++)
	{
		y += fScale;
		x = fDispX;
		for(j=0; j<nFWidth; j++)
		{
			x += fScale;
			if (x<0 || y<0 || x>=nIWidth || y>=nIHeight) 
				pDest[(nFHeight-i-1)*nFWidth + j] = 0;
			else
    			pDest[(nFHeight-i-1)*nFWidth + j] = pSour[(long)y*nIWidth + (long)x];	
		}
	}
}

////////////////////////////////////////////////////////
// OPENCV3.x 관련 함수 정의      by shsong            //
////////////////////////////////////////////////////////
BYTE* CNVisionLib::matToBytes(Mat image)
{	  
	  int size = image.total() * image.elemSize();
	  BYTE* bytes = new byte[size];  // you will have to delete[] that later
	  memcpy(bytes, image.data,size * sizeof(BYTE));
	  return bytes;
}

Mat CNVisionLib::bytesToMat(BYTE* bytes, int width, int height)
{
	Mat image = Mat(height, width, CV_8U, bytes).clone(); // make a copy
	return image;
}

//////////////////////////////////////////////////////////////////////////
// Jpeg Turbo 라이브러리를 인터페이스만 변경하여 function 생성. 
// 2017.05.30 edited by shsong
//////////////////////////////////////////////////////////////////////////

void CNVisionLib::write_JPEG_file(char *filename, LPBYTE fmImage, int image_width, int image_height,  int quality) 
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  FILE *outfile;                /* target file */
  JSAMPROW row_pointer[1];      /* pointer to JSAMPLE row[s] */
  int row_stride;               /* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);  

  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = image_width;      /* image width and height, in pixels */
  cinfo.image_height = image_height;
  cinfo.input_components = 1;           /* # of color components per pixel */
  cinfo.in_color_space = JCS_GRAYSCALE; /* colorspace of input image */

  jpeg_set_defaults(&cinfo);

  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);  

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = image_width * 1; /* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = & fmImage[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);
}

void CNVisionLib::my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}  

#if 0
int CNVisionLib::read_JPEG_file (char *filename)
{
	struct jpeg_decompress_struct cinfo;

	struct my_error_mgr jerr;
	/* More stuff */
	FILE *infile;                 /* source file */
	JSAMPARRAY buffer;            /* Output row buffer */
	int row_stride;               /* physical row width in output buffer */

	if ((infile = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		return 0;
	}

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer)) {

		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 0;
	}

	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, infile);

	(void) jpeg_read_header(&cinfo, TRUE);

	(void) jpeg_start_decompress(&cinfo);

	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;
	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	while (cinfo.output_scanline < cinfo.output_height) {

		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		put_scanline_someplace(buffer[0], row_stride);
	}

	(void) jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

	return 1;
}  
#endif // 0

void CNVisionLib::read_BMPJPEG_file(char *filename,  LPBYTE fmImage, int image_width, int image_height)
{
	CString str;
	int i, j, nPitch, nS, nT;
	
	//IplImage *pImage;
	Mat pImage;

	str.Format("%s", filename);
	str = str.Right(4);
	str = str.MakeUpper();

	memset(fmImage, 0, sizeof(unsigned char)*image_width*image_height);

	if (str == ".JPG") // || str == ".BMP" || str == ".PNG" || str == ".TIF")
	{		
		pImage = imread((char *)filename, CV_8U);
		if (pImage.rows==image_height && pImage.cols==image_width)
		{			
    		memcpy(fmImage, pImage.data, sizeof(unsigned char)*image_width*image_height);
		}
	}
}

void CNVisionLib::write_BMP_file(char *filename,  LPBYTE fmImage, int image_width, int image_height)
{
		bool isSaveSuccessful = true;
		Mat pImage;

		pImage = bytesToMat(fmImage, image_width, image_height); // 흑백 8 bit
		isSaveSuccessful = (bool)imwrite((char*)LPCTSTR(filename), pImage); 
}