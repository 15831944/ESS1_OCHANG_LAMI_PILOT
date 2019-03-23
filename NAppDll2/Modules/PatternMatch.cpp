// PatternMatch.cpp : implementation file
#include "stdafx.h"
#include <math.h>
#include "PatternMatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatternMatch
CPatternMatch::CPatternMatch()
{
    m_nPatternCnt = 0;
	for(long i=0; i<MAX_PATTERN_NUM; i++)
	{
		m_Pat[i].m_bRegist = FALSE;
		m_Pat[i].m_nWidth = 0;
		m_Pat[i].m_nHeight = 0;
    	m_Pat[i].m_pImage = NULL;
	}
}

CPatternMatch::~CPatternMatch()
{
    m_nPatternCnt = 0;

	for(long i=0; i<MAX_PATTERN_NUM; i++)
	{
		m_Pat[i].m_bRegist = FALSE;
		m_Pat[i].m_nWidth = 0;
		m_Pat[i].m_nHeight = 0;
    	if (m_Pat[i].m_pImage)
			delete m_Pat[i].m_pImage;
	}
}

BEGIN_MESSAGE_MAP(CPatternMatch, CStatic)
	//{{AFX_MSG_MAP(CPatternMatch)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatternMatch message handlers
//#include "stdafx.h"

long CPatternMatch::FileLoadPattern(CString strFileName)
{
	long  i, nPatNum=0;
   	CFile f;

	if (!f.Open(strFileName, CFile::modeRead))
	{
		return nPatNum;
	}
	else
	{
		for(i=0; i<MAX_PATTERN_NUM; i++)
		{
			f.Read(&m_Pat[i].m_bRegist, sizeof(BOOL));
			if (m_Pat[i].m_bRegist)
			{
	    		f.Read(&m_Pat[i].m_nWidth, sizeof(long));
	    		f.Read(&m_Pat[i].m_nHeight, sizeof(long));

				if (m_Pat[i].m_pImage)  delete m_Pat[i].m_pImage;
				m_Pat[i].m_pImage = new unsigned char[m_Pat[i].m_nWidth * m_Pat[i].m_nHeight];

				f.Read(m_Pat[i].m_pImage,  m_Pat[i].m_nWidth * m_Pat[i].m_nHeight);
                nPatNum++;
			}
		}
	}

	f.Close();
	return nPatNum;
}

void CPatternMatch::FileSavePattern(CString strFileName)
{
  	long  i;
   	CFile f;

	if (!f.Open(strFileName, CFile::modeCreate|CFile::modeWrite))
	{
		return;
	}
	else
	{
		for(i=0; i<MAX_PATTERN_NUM; i++)
		{
			f.Write(&m_Pat[i].m_bRegist, sizeof(BOOL));
			if (m_Pat[i].m_bRegist)
			{
	    		f.Write(&m_Pat[i].m_nWidth, sizeof(long));
	    		f.Write(&m_Pat[i].m_nHeight, sizeof(long));
				f.Write(m_Pat[i].m_pImage,  m_Pat[i].m_nWidth * m_Pat[i].m_nHeight);
			}
		}
	}

	f.Close();
}

void CPatternMatch::RegisterPattern(long nID, LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nPitch)
{
	long i, j, nWidth, nHeight;

    nWidth = x2-x1+1;
	nHeight= y2-y1+1;

	if (nWidth<MIN_PATSIDE || nHeight<MIN_PATSIDE)
		return;

	if (nID>=0 && nID<MAX_PATTERN_NUM && fmSourImage)
	{
		m_Pat[nID].m_nWidth  = nWidth;
		m_Pat[nID].m_nHeight = nHeight;

		if (m_Pat[nID].m_pImage) delete m_Pat[nID].m_pImage;
		m_Pat[nID].m_pImage = new unsigned char[m_Pat[nID].m_nWidth * m_Pat[nID].m_nHeight];

		for(i=y1; i<=y2; i++)
		for(j=x1; j<=x2; j++)
		{
            m_Pat[nID].m_pImage[(i-y1)*nWidth + (j-x1)] = fmSourImage[i*nPitch + j];
		}

        m_Pat[nID].m_bRegist = TRUE;
		m_nPatternCnt++;
	}
}

void CPatternMatch::DeletePattern(long nID)
{
	if (nID>=0 && nID<MAX_PATTERN_NUM)
	{
		if (m_Pat[nID].m_pImage)
		{
			delete m_Pat[nID].m_pImage;
            m_Pat[nID].m_pImage = NULL;
		}

        m_Pat[nID].m_bRegist = FALSE;
		m_nPatternCnt--;
	}
}

void CPatternMatch::DeletePatternAll()
{
	long i;

	for(i=0; i<MAX_PATTERN_NUM; i++)
		DeletePattern(i);    
}

long CPatternMatch::FindPatternMatching(long nID, LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nPitch, 
		                                          float dMinScore, float *dPosX, float *dPosY, float *dPatScore)
{   
	long nWidth, nHeight;
	long nRet = 0;
    
    nWidth  = m_Pat[nID].m_nWidth;
	nHeight = m_Pat[nID].m_nHeight;

	if (nWidth<MIN_PATSIDE || nHeight<MIN_PATSIDE)
		return nRet;

	if (m_Pat[nID].m_bRegist)
	{
		PATMAT PatMat;
		PatMat = PatternBuildPattern(m_Pat[nID].m_pImage, m_Pat[nID].m_nWidth, 0, 0, nWidth-1, nHeight-1, 0, 0, 0);
        *dPosX = 0;
        *dPosY = 0;
		if (PatMat)
		{
    		nRet = PatternSearch(PatMat, fmSourImage, nPitch, x1, y1, x2, y2, dMinScore, 1, dPosX, dPosY, dPatScore);
		    PatternFreePattern(&PatMat);

			*dPosX = (float)(*dPosX + m_Pat[nID].m_nWidth/2.0);
        	*dPosY = (float)(*dPosY + m_Pat[nID].m_nHeight/2.0);
	        *dPatScore = (float)(*dPatScore*100.0);
		}
/*
		dScore=GrayMatch(fmSourImage, nPitch, x1, y1, x2-x1, y2-y1,
	                 m_Pat[nID].m_pImage, m_Pat[nID].m_nWidth, m_Pat[nID].m_nWidth, m_Pat[nID].m_nHeight,
					 dPosX, dPosY, 0, 3, (float)(dMinScore/100.0), (float)0.95);
*/
	}

	return nRet;
}

BOOL CPatternMatch::FindPatternMatching(LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nPitch, 
		                                LPBYTE fmPattern, long nWidth, long nHeight, float dMinScore, float *dPosX, float *dPosY, float *dPatScore)
{   
	BOOL bRet = FALSE;
    PATMAT PatMat;
	
    *dPosX = 0;
    *dPosY = 0;
	*dPatScore = 0;

	if (nWidth<MIN_PATSIDE || nHeight<MIN_PATSIDE)
		return bRet;

	PatMat = PatternBuildPattern(fmPattern, nWidth, 0, 0, nWidth-1, nHeight-1, 0, 0, 0);
	if (PatMat)
	{
    	bRet = (BOOL)PatternSearch(PatMat, fmSourImage, nPitch, x1, y1, x2, y2, dMinScore, 1, dPosX, dPosY, dPatScore);
		PatternFreePattern(&PatMat);

		if (bRet)
		{
			*dPosX = (float)(*dPosX + nWidth/2.0);
			*dPosY = (float)(*dPosY + nHeight/2.0);
			*dPatScore = (float)(*dPatScore*100.0);
		}
	}

	return bRet;
}

void CPatternMatch::CopyPattern(long pPatLib)
{
    long i;
	CPatternMatch *pPat = (CPatternMatch *)pPatLib;

	for(i=0; i<MAX_PATTERN_NUM; i++)
	{
		m_Pat[i].m_bRegist = pPat->m_Pat[i].m_bRegist;
		m_Pat[i].m_nWidth  = pPat->m_Pat[i].m_nWidth;
		m_Pat[i].m_nHeight = pPat->m_Pat[i].m_nHeight;

		if (m_Pat[i].m_bRegist)
		{
			if (m_Pat[i].m_pImage) delete m_Pat[i].m_pImage;
			m_Pat[i].m_pImage = new unsigned char[m_Pat[i].m_nWidth * m_Pat[i].m_nHeight];

			memcpy(m_Pat[i].m_pImage, pPat->m_Pat[i].m_pImage, m_Pat[i].m_nWidth * m_Pat[i].m_nHeight);
		}
	}
}

void CPatternMatch::SaveImage(CString strFileName, LPBYTE pImage, long nWidth, long nHeight)
{
   	CFile f;

	if (!f.Open(strFileName, CFile::modeCreate|CFile::modeWrite))
	{
		return;
	}
	else
	{
		f.Write(pImage, sizeof(BYTE)*nWidth*nHeight);
	}

	f.Close();
}

#pragma	warning(disable:4244)
void (*PatternShowErrorFunc)(LPCTSTR)=0;

// function

void CPatternMatch::PatternInit(PBYTE (*alloc_8)(int),void (*free_8)(PBYTE)) 
{

}

void CPatternMatch::PatternSetErrorDisplay(void (*showError)(LPCTSTR)) 
{
	PatternShowErrorFunc = showError;
}

PATMAT CPatternMatch::PatternBuildPattern(PBYTE src,int memw,
		int left,int top,int right,int bottom,
		float hotOffX,float hotOffY,int bUseSrc) 
{
	int	ok=0;
	PATMAT	pat=0;
	int	width,height;
	int	i,x,y;
	unsigned int	val,sum,sumsum;
	float	sumsumF;

	// alloc pattern
	if (!(pat=(PATMAT)malloc(sizeof(struct tagPATMAT)))) {
		PatternMemErr();
		goto done;
	}
	
	// assign
	pat->hotOffX = hotOffX;
	pat->hotOffY = hotOffY;
	pat->bUseSrc = bUseSrc;
	pat->nLevel = 0;
	for (i=0; i<MAX_LEVEL; i++)
		pat->image[i] = 0;
	// set level 0
	if (!(pat->image[0]=(PATIMG)malloc(sizeof(struct tagPATIMG)))) {
		PatternMemErr();
		goto done;
	}
	pat->image[0]->patMask = 0;
	width = right-left+1;
	height = bottom-top+1;
	if (width<MIN_PATSIDE || height<MIN_PATSIDE)
		goto done;
	if (bUseSrc) {
		pat->image[0]->bits = src+top*memw+left;
		pat->image[0]->memw = memw;
		sum = 0;
		sumsumF = 0.0;
		for (y=top; y<=bottom; y++) {
			sumsum = 0;
			for (x=left; x<=right; x++) {
				val = src[y*memw+x];
				sum += val;
				sumsum += val*val;
			}
			sumsumF += (float)sumsum;
		}
	}
	else {
		if (!(pat->image[0]->bits=(PBYTE)malloc(width*height))) {
			PatternMemErr();
			goto done;
		}
		pat->image[0]->memw = width;
		sum = 0;
		sumsumF = 0.0;
		for (y=top; y<=bottom; y++) {
			sumsum = 0;
			for (x=left; x<=right; x++) {
				val = src[y*memw+x];
				pat->image[0]->bits[(y-top)*width+x-left] = val;
				sum += val;
				sumsum += val*val;
			}
			sumsumF += (float)sumsum;
		}
	}

	pat->image[0]->width = width;
	pat->image[0]->height = height;
	pat->image[0]->patS = (float)sum;
	pat->image[0]->patSS = sumsumF;
	pat->image[0]->patLen = width*height;
	pat->image[0]->dAngle = 180.0*atan(2.0/sqrt((float)((width-1)*(width-1)+(height-1)*(height-1))))/PI;

	pat->nLevel = 1;
	// build levels
	for (i=1; i<MAX_LEVEL; i++) {
		width = pat->image[i-1]->width>>1;
		height = pat->image[i-1]->height>>1;
		if (width<MIN_PATSIDE || height<MIN_PATSIDE)
			break;
		if (!(pat->image[i]=(PATIMG)malloc(sizeof(struct tagPATIMG)))) {
			PatternMemErr();
			goto done;
		}
		pat->image[i]->patMask = 0;
		if (!(pat->image[i]->bits=(PBYTE)malloc(width*height))) {
			PatternMemErr();
			goto done;
		}
		pat->image[i]->memw = width;
		pat->image[i]->width = width;
		pat->image[i]->height = height;
		PatternShrink(pat->image[i-1],pat->image[i],1);
		pat->image[i]->dAngle = 180.0*atan(2.0/
		 sqrt((float)((width-1)*(width-1)+(height-1)*(height-1))))/PI;
		if (PatternOrgMatch(pat->image[0],pat->image[i],i)<=ORG_SCORE*ORG_SCORE) {
			free(pat->image[i]->bits);
			free(pat->image[i]);
			pat->image[i] = 0;
			break;
		}
		pat->nLevel++;
	}
	// done
	ok = 1;

done:
	if (!ok) 
	{
		PatternFreePattern(&pat);
		PatternShowError(_T("FAIL: PatternBuildPattern"));
        pat = 0;
	}

	return pat;
}

PATMAT CPatternMatch::PatternBuildMaskPattern(PBYTE src,int memw,
		int left,int top,int right,int bottom,
		float hotOffX,float hotOffY,
		PBYTE mask,int mpitch) 
{
	int	ok=0;
	PATMAT	pat=0;
	int	width,height;
	int	i,x,y,u,v;
	unsigned int	val,sum,sumsum;
	float	sumsumF;
	int	len;

	// alloc pattern
	if (!(pat=(PATMAT)malloc(sizeof(struct tagPATMAT)))) {
		PatternMemErr();
		goto done;
	}
	// assign
	pat->hotOffX = hotOffX;
	pat->hotOffY = hotOffY;
	pat->bUseSrc = 0;
	pat->nLevel = 0;
	for (i=0; i<MAX_LEVEL; i++)
		pat->image[i] = 0;
	// set level 0
	if (!(pat->image[0]=(PATIMG)malloc(sizeof(struct tagPATIMG)))) {
		PatternMemErr();
		goto done;
	}
	pat->image[0]->patMask = 0;
	width = right-left+1;
	height = bottom-top+1;
	if (width<MIN_PATSIDE || height<MIN_PATSIDE)
		goto done;
	if (!(pat->image[0]->bits=(PBYTE)malloc(width*height))) {
		PatternMemErr();
		goto done;
	}
	if (!(pat->image[0]->patMask=(PBYTE)malloc(width*height))) {
		PatternMemErr();
		goto done;
	}
	pat->image[0]->memw = width;
	sum = 0;
	sumsumF = 0.0;
	len = 0;
	for (y=top,v=0; y<=bottom; y++,v++) {
		sumsum = 0;
		for (x=left,u=0; x<=right; x++,u++) {
			if (pat->image[0]->patMask[v*width+u]=mask[v*mpitch+u]) {
				len++;
				val = src[y*memw+x];
				pat->image[0]->bits[(y-top)*width+x-left] = val;
				sum += val;
				sumsum += val*val;
			}
		}
		sumsumF += (float)sumsum;
	}
	pat->image[0]->width = width;
	pat->image[0]->height = height;
	pat->image[0]->patS = (float)sum;
	pat->image[0]->patSS = sumsumF;
	pat->image[0]->patLen = len;
	pat->image[0]->dAngle = 180.0*atan(2.0/
	 sqrt((float)((width-1)*(width-1)+(height-1)*(height-1))))/PI;
	pat->nLevel = 1;
	// build levels
	for (i=1; i<MAX_LEVEL; i++) {
		width = pat->image[i-1]->width>>1;
		height = pat->image[i-1]->height>>1;
		if (width<MIN_PATSIDE || height<MIN_PATSIDE)
			break;
		if (!(pat->image[i]=(PATIMG)malloc(sizeof(struct tagPATIMG)))) {
			PatternMemErr();
			goto done;
		}
		pat->image[i]->patMask = 0;
		if (!(pat->image[i]->bits=(PBYTE)malloc(width*height))) {
			PatternMemErr();
			goto done;
		}
		if (!(pat->image[i]->patMask=(PBYTE)malloc(width*height))) {
			PatternMemErr();
			goto done;
		}
		pat->image[i]->memw = width;
		pat->image[i]->width = width;
		pat->image[i]->height = height;
		PatternShrink(pat->image[i-1],pat->image[i],1);
		pat->image[i]->dAngle = 180.0*atan(2.0/
		 sqrt((float)((width-1)*(width-1)+(height-1)*(height-1))))/PI;
		if (PatternOrgMatch(pat->image[0],pat->image[i],i)<=ORG_SCORE*ORG_SCORE) {
			free(pat->image[i]->bits);
			free(pat->image[i]->patMask);
			free(pat->image[i]);
			pat->image[i] = 0;
			break;
		}
		pat->nLevel++;
	}
	// done
	ok = 1;
done:
	if (!ok) {
		PatternFreePattern(&pat);
		PatternShowError(_T("FAIL: PatternBuildMaskPattern"));
	}
	return pat;
}

void CPatternMatch::PatternFreePattern(PATMAT *ppat) 
{
	PATMAT	pat;
	int	i;

	if (!ppat || !(pat=*ppat))
		return;
	if (!pat->bUseSrc)
		free(pat->image[0]->bits);
	if (pat->image[0]->patMask)
		free(pat->image[0]->patMask);
	free(pat->image[0]);
	for (i=1; i<pat->nLevel; i++) {
		free(pat->image[i]->bits);
		if (pat->image[i]->patMask)
			free(pat->image[i]->patMask);
		free(pat->image[i]);
	}
	free(pat);
	*ppat = 0;
}

#define	MAP(x,y)	map[((y)+1)*(sw+2)+(x)+1]

int	CPatternMatch::PatternSearch(PATMAT pat, PBYTE dst, int memw,
		int left,int top,int right,int bottom,float minScore,int pixRes,
		float *locX,float *locY,float *score) 
{
	int	ok=0;
	PATIMG	dimg[MAX_LEVEL];
	int	i,j;
	int	width,height;
	float	*map=0;
	int	sw,sh;
	int	level,pixLevel;
	int	x,y;
	float	maxScore;
	float	tmp;
	SUBINFO	maxSubInfo=0,subInfo=0;
	float	LM,RM,MT,MB;
	float	rx,ry;

	// init
	for (i=0; i<MAX_LEVEL; i++)	dimg[i] = 0;
	level = pat->nLevel;

	// check score,width,height
	if (minScore<0.0) minScore = 0.0;
	minScore = minScore / 100.0;

	minScore *= minScore;
	if (pixRes<1)
		pixRes = 1;
	for (i=0,j=1; i<level; i++,j<<=1)
		if (j<=pixRes)
			pixLevel = i;
	width = right-left+1;
	height = bottom-top+1;
	if (width<pat->image[0]->width || height<pat->image[0]->height)
		goto done;
	// build dimg
	if (!(dimg[0]=(PATIMG)malloc(sizeof(struct tagPATIMG)))) {
		PatternMemErr();
		goto done;
	}
	dimg[0]->patMask = 0;
	dimg[0]->bits = dst+top*memw+left;
	dimg[0]->memw = memw;
	dimg[0]->width = width;
	dimg[0]->height = height;
	for (i=1; i<level; i++) {
		width = dimg[i-1]->width>>1;
		height = dimg[i-1]->height>>1;
		if (!(dimg[i]=(PATIMG)malloc(sizeof(struct tagPATIMG)))) {
			PatternMemErr();
			goto done;
		}
		dimg[i]->patMask = 0;
		if (!(dimg[i]->bits=(PBYTE)malloc(width*height))) {
			PatternMemErr();
			goto done;
		}
		dimg[i]->memw = width;
		dimg[i]->width = width;
		dimg[i]->height = height;
		PatternShrink(dimg[i-1],dimg[i],0);
	}
	// search at level-1 to map
	sw = dimg[level-1]->width-pat->image[level-1]->width+1;
	sh = dimg[level-1]->height-pat->image[level-1]->height+1;
	if (!(map=(float*)malloc((sw+2)*(sh+2)*sizeof(float)))) {
		PatternMemErr();
		goto done;
	}
	for (x=-1; x<=sw; x++)
		MAP(x,-1) = MAP(x,sh) = 0.0;
	for (y=0; y<=sh-1; y++)
		MAP(-1,y) = MAP(sw,y) = 0.0;
	for (y=0; y<sh; y++)
		for (x=0; x<sw; x++)
			MAP(x,y) = PatternCorr(pat->image[level-1]->bits,pat->image[level-1]->memw,
			 pat->image[level-1]->patS,pat->image[level-1]->patSS,
			 pat->image[level-1]->patLen,pat->image[level-1]->patMask,
			 dimg[level-1]->bits+y*dimg[level-1]->memw+x,dimg[level-1]->memw,
			 pat->image[level-1]->width,pat->image[level-1]->height);
	// find location
	if (!(maxSubInfo=(SUBINFO)malloc(sizeof(struct tagSUBINFO)))) {
		PatternMemErr();
		goto done;
	}
	maxScore = -2.0;
	if (level>1) {
		if (!(subInfo=(SUBINFO)malloc(sizeof(struct tagSUBINFO)))) {
			PatternMemErr();
			goto done;
		}
		for (y=0; y<sh; y++)
		for (x=0; x<sw; x++)
			if ((tmp=MAP(x,y))>minScore) {
				if (tmp>=MAP(x-1,y-1) && tmp>=MAP(x+1,y+1) &&
				 tmp>=MAP(x-1,y+1) && tmp>=MAP(x+1,y-1) &&
				 tmp>=MAP(x-1,y) && tmp>=MAP(x+1,y) &&
				 tmp>=MAP(x,y-1) && tmp>=MAP(x,y+1)) {
					tmp = (level-1==pixLevel)?
					 PatternBottomSearch(dimg[pixLevel],pat->image[pixLevel],pixLevel,
					 x,y,subInfo) :
					 PatternDownSearch(dimg,pat->image,level-2,pixLevel,minScore,
					 x<<1,y<<1,subInfo);
					if (tmp>minScore && tmp>maxScore) {
						maxScore = tmp;
						memcpy(maxSubInfo,subInfo,sizeof(struct tagSUBINFO));
					}
				}
			}
	}
	else {
		maxSubInfo->level = 0;
		for (y=0; y<sh; y++)
		for (x=0; x<sw; x++)
			if ((tmp=MAP(x,y))>minScore) {
				if (tmp>=MAP(x-1,y-1) && tmp>=MAP(x+1,y+1) &&
				 tmp>=MAP(x-1,y+1) && tmp>=MAP(x+1,y-1) &&
				 tmp>=(LM=MAP(x-1,y)) && tmp>=(RM=MAP(x+1,y)) &&
				 tmp>=(MT=MAP(x,y-1)) && tmp>=(MB=MAP(x,y+1))) {
					if (tmp>maxScore) {
						maxScore = tmp;
						maxSubInfo->x = x;
						maxSubInfo->y = y;
						maxSubInfo->sl = LM;
						maxSubInfo->st = MT;
						maxSubInfo->sr = RM;
						maxSubInfo->sb = MB;
						maxSubInfo->sm = tmp;
					}
				}
			}
	}
	if (maxScore<=0.0)	goto done;

	PatternSubPixel(dimg[maxSubInfo->level],pat->image[maxSubInfo->level],maxSubInfo, &rx,&ry);

	if (locX)  *locX  = ((float)left + rx + pat->hotOffX);
	if (locY)  *locY  = ((float)top + ry + pat->hotOffY);
	if (score) *score = sqrt(maxScore);
	// done
	ok = 1;

done:
	if (dimg[0])
		free(dimg[0]);
	for (i=1; i<MAX_LEVEL; i++)
		if (dimg[i]) {
			free(dimg[i]->bits);
			free(dimg[i]);
		}
	if (map)        free(map);
	if (maxSubInfo) free(maxSubInfo);
	if (subInfo)	free(subInfo);

	return ok;
}

#undef	MAP

#define	MAP(x,y,a)	map[((a)+1)*((srw+2)*(srh+2))+((y)+1)*(srw+2)+(x)+1]

int	CPatternMatch::PatternSearchWithAngle(PATMAT pat,PBYTE dst,int memw,
		int left,int top,int right,int bottom,float loDegree,float hiDegree,
		float minScore,int pixRes,
		float *locX,float *locY,float *degree,float *score) 
{
	int	ok=0;
	PATIMG	dimg[MAX_LEVEL];
	int	i,j;
	int	width,height;
	float	*map=0;
	int	srw,srh,sa,sw,sh;
	float	a0,da,angle;
	int	level,pixLevel;
	int	x,y,a;
	float	maxScore;
	float	tmp;
	SUBINFO	maxSubInfo=0,subInfo=0;
	float	LM,RM,MT,MB,UP,DN;
	float	rx,ry;
	float	cost,sint;

	// init
	for (i=0; i<MAX_LEVEL; i++)
		dimg[i] = 0;
	level = pat->nLevel;
	// check score,width,height
	if (minScore<0.0)
		minScore = 0.0;
	minScore *= minScore;
	if (pixRes<1)
		pixRes = 1;
	for (i=0,j=1; i<level; i++,j<<=1)
		if (j<=pixRes)
			pixLevel = i;
	width = right-left+1;
	height = bottom-top+1;
	if (loDegree>=hiDegree)
		goto done;
	// build dimg
	if (!(dimg[0]=(PATIMG)malloc(sizeof(struct tagPATIMG)))) {
		PatternMemErr();
		goto done;
	}
	dimg[0]->patMask = 0;
	dimg[0]->bits = dst+top*memw+left;
	dimg[0]->memw = memw;
	dimg[0]->width = width;
	dimg[0]->height = height;
	for (i=1; i<level; i++) {
		width = dimg[i-1]->width>>1;
		height = dimg[i-1]->height>>1;
		if (!(dimg[i]=(PATIMG)malloc(sizeof(struct tagPATIMG)))) {
			PatternMemErr();
			goto done;
		}
		dimg[i]->patMask = 0;
		if (!(dimg[i]->bits=(PBYTE)malloc(width*height))) {
			PatternMemErr();
			goto done;
		}
		dimg[i]->memw = width;
		dimg[i]->width = width;
		dimg[i]->height = height;
		PatternShrink(dimg[i-1],dimg[i],0);
	}
	// search at level-1 to map
	sw = pat->image[level-1]->width;
	sh = pat->image[level-1]->height;
	da = pat->image[level-1]->dAngle;
	srw = dimg[level-1]->width;
	srh = dimg[level-1]->height;
	for (angle=0.5*(loDegree+hiDegree); angle>loDegree; angle-=da);
	a0 = angle;
	sa = (int)(2.0*(0.5*(loDegree+hiDegree)-a0)/da+1.5);
	if (!(map=(float*)malloc((srw+2)*(srh+2)*(sa+2)*sizeof(float)))) {
		PatternMemErr();
		goto done;
	}
	for (x=0; x<srw; x++)
		for (y=0; y<srh; y++)
			MAP(x,y,-1) = MAP(x,y,sa) = 0.0;
	for (a=-1; a<=sa; a++) {
		for (x=-1; x<=srw; x++)
			MAP(x,-1,a) = MAP(x,srh,a) = 0.0;
		for (y=0; y<=srh-1; y++)
			MAP(-1,y,a) = MAP(srw,y,a) = 0.0;
	}
	for (a=0; a<sa; a++)
	for (y=0; y<srh; y++)
	for (x=0; x<srw; x++)
		MAP(x,y,a) = PatternCorr_Angle(
		 pat->image[level-1]->bits,pat->image[level-1]->memw,sw,sh,
		 pat->image[level-1]->patS,pat->image[level-1]->patSS,
		 pat->image[level-1]->patLen,pat->image[level-1]->patMask,
		 dimg[level-1]->bits,dimg[level-1]->memw,
		 dimg[level-1]->width,dimg[level-1]->height,
		 x,y,a0+a*da);
	// find location
	if (!(maxSubInfo=(SUBINFO)malloc(sizeof(struct tagSUBINFO)))) {
		PatternMemErr();
		goto done;
	}
	maxScore = -2.0;
	if (level>1) {
		if (!(subInfo=(SUBINFO)malloc(sizeof(struct tagSUBINFO)))) {
			PatternMemErr();
			goto done;
		}
		for (a=0; a<sa; a++)
		for (y=0; y<srh; y++)
		for (x=0; x<srw; x++)
			if ((tmp=MAP(x,y,a))>minScore) {
				if (tmp>=MAP(x-1,y-1,a) && tmp>=MAP(x+1,y+1,a) &&
				 tmp>=MAP(x-1,y+1,a) && tmp>=MAP(x+1,y-1,a) &&
				 tmp>=MAP(x-1,y,a) && tmp>=MAP(x+1,y,a) &&
				 tmp>=MAP(x,y-1,a) && tmp>=MAP(x,y+1,a) &&
				 tmp>=MAP(x,y,a-1) && tmp>=MAP(x,y,a+1) &&
				 tmp>=MAP(x-1,y-1,a-1) && tmp>=MAP(x+1,y+1,a+1) &&
				 tmp>=MAP(x+1,y+1,a-1) && tmp>=MAP(x-1,y-1,a+1) &&
				 tmp>=MAP(x-1,y+1,a-1) && tmp>=MAP(x+1,y-1,a+1) &&
				 tmp>=MAP(x+1,y-1,a-1) && tmp>=MAP(x-1,y+1,a+1) &&
				 tmp>=MAP(x-1,y,a-1) && tmp>=MAP(x+1,y,a+1) &&
				 tmp>=MAP(x+1,y,a-1) && tmp>=MAP(x-1,y,a+1) &&
				 tmp>=MAP(x,y-1,a-1) && tmp>=MAP(x,y+1,a+1) &&
				 tmp>=MAP(x,y+1,a-1) && tmp>=MAP(x,y-1,a+1)) {
					tmp = (level-1==pixLevel)?
					 PatternBottomSearch_Angle(dimg[pixLevel],pat->image[pixLevel],pixLevel,
					 x,y,a0+a*da,subInfo) :
					 PatternDownSearch_Angle(dimg,pat->image,level-2,pixLevel,minScore,
					 x<<1,y<<1,a0+a*da,subInfo);
					if (tmp>maxScore) {
						maxScore = tmp;
						memcpy(maxSubInfo,subInfo,
						 sizeof(struct tagSUBINFO));
					}
				}
			}
	}
	else {
		maxSubInfo->level = 0;
		for (a=0; a<sa; a++)
		for (y=0; y<srh; y++)
		for (x=0; x<srw; x++)
			if ((tmp=MAP(x,y,a))>minScore) {
				if (tmp>=MAP(x-1,y-1,a) && tmp>=MAP(x+1,y+1,a) &&
				 tmp>=MAP(x-1,y+1,a) && tmp>=MAP(x+1,y-1,a) &&
				 tmp>=(LM=MAP(x-1,y,a)) && tmp>=(RM=MAP(x+1,y,a)) &&
				 tmp>=(MT=MAP(x,y-1,a)) && tmp>=(MB=MAP(x,y+1,a)) &&
				 tmp>=(UP=MAP(x,y,a-1)) && tmp>=(DN=MAP(x,y,a+1)) &&
				 tmp>=MAP(x-1,y-1,a-1) && tmp>=MAP(x+1,y+1,a+1) &&
				 tmp>=MAP(x+1,y+1,a-1) && tmp>=MAP(x-1,y-1,a+1) &&
				 tmp>=MAP(x-1,y+1,a-1) && tmp>=MAP(x+1,y-1,a+1) &&
				 tmp>=MAP(x+1,y-1,a-1) && tmp>=MAP(x-1,y+1,a+1) &&
				 tmp>=MAP(x-1,y,a-1) && tmp>=MAP(x+1,y,a+1) &&
				 tmp>=MAP(x+1,y,a-1) && tmp>=MAP(x-1,y,a+1) &&
				 tmp>=MAP(x,y-1,a-1) && tmp>=MAP(x,y+1,a+1) &&
				 tmp>=MAP(x,y+1,a-1) && tmp>=MAP(x,y-1,a+1)) {
					if (tmp>maxScore) {
						maxScore = tmp;
						maxSubInfo->x = x;
						maxSubInfo->y = y;
						maxSubInfo->degree = a0+a*da;
						maxSubInfo->da = da;
						maxSubInfo->sl = LM;
						maxSubInfo->st = MT;
						maxSubInfo->sr = RM;
						maxSubInfo->sb = MB;
						maxSubInfo->sm = tmp;
						maxSubInfo->su = UP;
						maxSubInfo->sd = DN;
					}
				}
			}
	}
	if (maxScore<=0.0)
		goto done;
	PatternSubPixel_Angle(dimg[maxSubInfo->level],pat->image[maxSubInfo->level],maxSubInfo,
	 &rx,&ry,&angle);
	cost = PatternCos(PI*angle/180.0);
	sint = PatternSin(PI*angle/180.0);
	if (locX)
		*locX = (float)left+rx+cost*pat->hotOffX-sint*pat->hotOffY;
	if (locY)
		*locY = (float)top+ry+sint*pat->hotOffX+cost*pat->hotOffY;
	if (degree)
		*degree = angle;
	if (score)
		*score = sqrt(maxScore);
	// done
	ok = 1;
done:
	if (dimg[0])
		free(dimg[0]);
	for (i=1; i<MAX_LEVEL; i++)
		if (dimg[i]) {
			free(dimg[i]->bits);
			free(dimg[i]);
		}
	if (map)
		free(map);
	if (maxSubInfo)
		free(maxSubInfo);
	if (subInfo)
		free(subInfo);
	return ok;
}

#undef	MAP

// private

void CPatternMatch::PatternShowError(LPCTSTR msg) 
{
	if (PatternShowErrorFunc)
		(*PatternShowErrorFunc)(msg);
}

void CPatternMatch::PatternMemErr() 
{
	PatternShowError(_T("No Memory Heap"));
}

void CPatternMatch::PatternShrink(PATIMG dn,PATIMG up,int bIsPat) 
{
	PBYTE	db,ub,dmsk,umsk;
	int	dm,dw,dh,um,uw,uh;
	int	dx,dy,ux,uy;
	int	offset,moff;
	unsigned int	val,sum,sumsum;
	float	sumsumF;
	BYTE	maskVal;
	int	len;

	db = dn->bits;
	dm = dn->memw;
	dw = dn->width;
	dh = dn->height;
	ub = up->bits;
	um = up->memw;
	uw = up->width;
	uh = up->height;
	if (bIsPat) {
		sum = 0;
		sumsumF = 0.0;
		if (!dn->patMask) {
			for (uy=0,dy=0; uy<uh; uy++,dy+=2) {
				sumsum = 0;
				for (ux=0,dx=0; ux<uw; ux++,dx+=2) {
					offset = dy*dm+dx;
					val = ub[uy*um+ux] = ((unsigned int)db[offset]+
					 (unsigned int)db[offset+1]+
					 (unsigned int)db[offset+dm]+
					 (unsigned int)db[offset+dm+1])>>2;
					sum += val;
					sumsum += val*val;
				}
				sumsumF += (float)sumsum;
			}
			up->patLen = uw*uh;
		}
		else {
			dmsk = dn->patMask;
			umsk = up->patMask;
			len = 0;
			for (uy=0,dy=0; uy<uh; uy++,dy+=2) {
				sumsum = 0;
				for (ux=0,dx=0; ux<uw; ux++,dx+=2) {
					moff = dy*dw+dx;
					maskVal = umsk[uy*uw+ux] = (dmsk[moff]&dmsk[moff+1])&
					 (dmsk[moff+dw]&dmsk[moff+dw+1]);
					if (!maskVal)
						continue;
					len++;
					offset = dy*dm+dx;
					val = ub[uy*um+ux] = ((unsigned int)db[offset]+
					 (unsigned int)db[offset+1]+
					 (unsigned int)db[offset+dm]+
					 (unsigned int)db[offset+dm+1])>>2;
					sum += val;
					sumsum += val*val;
				}
				sumsumF += (float)sumsum;
			}
			up->patLen = len;
		}
		up->patS = (float)sum;
		up->patSS = sumsumF;
	}
	else {
		for (uy=0,dy=0; uy<uh; uy++,dy+=2)
			for (ux=0,dx=0; ux<uw; ux++,dx+=2) {
				offset = dy*dm+dx;
				ub[uy*um+ux] = ((int)db[offset]+(int)db[offset+1]+
				 (int)db[offset+dm]+(int)db[offset+dm+1])>>2;
			}
	}
}

float CPatternMatch::PatternOrgMatch(PATIMG img0,PATIMG img,int level) 
{
	int	val1,val2;
	unsigned int	sum1,sum2,sum11,sum12,sum22;
	float	sum11F,sum12F,sum22F;
	int	u,w_1,h;
	int	smemw,dmemw,dmemwz,width;
	PBYTE	soff,doff,soffe,dhead,moff;
	float	s1,s2,s11,s12,s22;
	float	len,upper,below;
	int	x,y;
	int	level2;
	int	zoom;

	sum1 = sum2 = sum11 = sum12 = sum22 = 0;
	sum11F = sum12F = sum22F = 0.0;
	width = img->width;
	w_1 = width-1;
	h = img->height;
	level2 = level<<1;
	zoom = 0x1<<level;
	smemw = img->memw;
	dmemw = img0->memw;
	dmemwz = dmemw<<level;
	if (!img0->patMask) {
		for (soff=img->bits,soffe=img->bits+h*smemw,doff=img0->bits;
		 soff<soffe; soff+=smemw,doff+=dmemwz) {
			sum11 = sum12 = sum22 = 0;
			for (u=0,dhead=doff; u<=w_1; u++,dhead+=zoom) {
				val1 = soff[u];
				sum1 += val1<<level2;
				sum11 += (val1*val1)<<level2;
				for (y=0; y<zoom; y++)
					for (x=0; x<zoom; x++) {
						val2 = dhead[y*dmemw+x];
						sum2 += val2;
						sum22 += val2*val2;
						sum12 += val1*val2;
					}
			}
			sum11F += (float)sum11;
			sum12F += (float)sum12;
			sum22F += (float)sum22;
		}
	}
	else {
		for (soff=img->bits,soffe=img->bits+h*smemw,doff=img0->bits,moff=img->patMask;
		 soff<soffe; soff+=smemw,doff+=dmemwz,moff+=width) {
			sum11 = sum12 = sum22 = 0;
			for (u=0,dhead=doff; u<=w_1; u++,dhead+=zoom) {
				if (!moff[u])
					continue;
				val1 = soff[u];
				sum1 += val1<<level2;
				sum11 += (val1*val1)<<level2;
				for (y=0; y<zoom; y++)
					for (x=0; x<zoom; x++) {
						val2 = dhead[y*dmemw+x];
						sum2 += val2;
						sum22 += val2*val2;
						sum12 += val1*val2;
					}
			}
			sum11F += (float)sum11;
			sum12F += (float)sum12;
			sum22F += (float)sum22;
		}
	}
	if (!sum1 || !sum2)
		return -1.0;
	len = (float)img0->patLen;
	s1 = (float)sum1;
	s2 = (float)sum2;
	s11 = sum11F;
	s12 = sum12F;
	s22 = sum22F;
	upper = len*s12-s1*s2;
	below = (len*s11-s1*s1)*(len*s22-s2*s2);
	return (upper<=0.0 || below<0.01)? -1.0 : upper*upper/below;
}

float CPatternMatch::PatternCorr(PBYTE src,int smemw,float s_s,float s_ss,int s_len,
	PBYTE mask,PBYTE dst,int dmemw,int w,int h) 
{
	unsigned int	val2;
	unsigned int	sum2,sum12,sum22;
	int	u,w_1;
	PBYTE	soff,doff,soffe,moff;
	float	s2,s12,s22;
	float	upper,below;

	sum2 = 0;
	s12 = s22 = 0.0;
	w_1 = w-1;
	if (!mask) {
		for (soff=src,soffe=src+h*smemw,doff=dst; soff<soffe; soff+=smemw,doff+=dmemw) {
			sum12 = sum22 = 0;
			for (u=0; u<=w_1; u++) {
				val2 = doff[u];
				sum2 += val2;
				sum22 += val2*val2;
				sum12 += soff[u]*val2;
			}
			s12 += (float)sum12;
			s22 += (float)sum22;
		}
	}
	else {
		for (soff=src,soffe=src+h*smemw,doff=dst,moff=mask; soff<soffe;
		 soff+=smemw,doff+=dmemw,moff+=w) {
			sum12 = sum22 = 0;
			for (u=0; u<=w_1; u++) {
				if (!moff[u])
					continue;
				val2 = doff[u];
				sum2 += val2;
				sum22 += val2*val2;
				sum12 += soff[u]*val2;
			}
			s12 += (float)sum12;
			s22 += (float)sum22;
		}
	}
	s2 = (float)sum2;
	upper = s_len*s12-s_s*s2;
	below = (s_len*s_ss-s_s*s_s)*(s_len*s22-s2*s2);
	return (upper<=0.0 || below<0.01)? 0.0 : upper*upper/below;
}

float CPatternMatch::PatternDownSearch(PATIMG *dimg,PATIMG *patimg,int level,int pixLevel,
		float minScore,int x,int y,SUBINFO subInfo) 
{
	PBYTE	dst,src,msk;
	int	dmemw,dw,dh,smemw,sw,sh,minU,maxU,minV,maxV;
	float	maxScore,score;
	int	u,v,uMax,vMax;
	float	s_s,s_ss;
	int	s_len;

	// check
	if (level==pixLevel)
		return PatternBottomSearch(dimg[pixLevel],patimg[pixLevel],pixLevel,x,y,subInfo);
	// set
	dst = dimg[level]->bits;
	dmemw = dimg[level]->memw;
	dw = dimg[level]->width;
	dh = dimg[level]->height;
	src = patimg[level]->bits;
	smemw = patimg[level]->memw;
	sw = patimg[level]->width;
	sh = patimg[level]->height;
	s_s = patimg[level]->patS;
	s_ss = patimg[level]->patSS;
	s_len = patimg[level]->patLen;
	msk = patimg[level]->patMask;
	if ((minU=x-1)<0)
		minU = 0;
	if ((maxU=x+2)+sw>dw)
		maxU = dw-sw;
	if ((minV=y-1)<0)
		minV = 0;
	if ((maxV=y+2)+sh>dh)
		maxV = dh-sh;
	// search max
	maxScore = -2.0;
	uMax = x;
	vMax = y;
	for (v=minV; v<=maxV; v++)
		for (u=minU; u<=maxU; u++) {
			score = PatternCorr(src,smemw,s_s,s_ss,s_len,msk,
			 dst+v*dmemw+u,dmemw,sw,sh);
			if (score>maxScore) {
				maxScore = score;
				uMax = u;
				vMax = v;
			}
		}
	// decide recursion
	if (maxScore>=minScore)
		return PatternDownSearch(dimg,patimg,level-1,pixLevel,minScore,
		 uMax<<1,vMax<<1,subInfo);
	// otherwise
	subInfo->level = level;
	subInfo->x = uMax;
	subInfo->y = vMax;
	return maxScore;
}

float CPatternMatch::PatternBottomSearch(PATIMG dimg,PATIMG patimg,int pixLevel,
			int x,int y,SUBINFO subInfo)
 {
	PBYTE	dst,src,msk;
	int	dmemw,dw,dh,smemw,sw,sh;
	float	s_s,s_ss;
	float	map[5][5];
	int	i,j,u,v;
	float	maxScore,score;
	int	maxI,maxJ;
	int	s_len;

	// set
	dst = dimg->bits;
	dmemw = dimg->memw;
	dw = dimg->width;
	dh = dimg->height;
	src = patimg->bits;
	smemw = patimg->memw;
	sw = patimg->width;
	sh = patimg->height;
	s_s = patimg->patS;
	s_ss = patimg->patSS;
	s_len = patimg->patLen;
	msk = patimg->patMask;
	// search max
	maxScore = -2.0;
	for (i=0; i<=4; i++)
		map[i][0] = map[i][4] = -1.0;
	for (j=1; j<=3; j++)
		map[0][j] = map[4][j] = -1.0;
	for (j=1; j<=3; j++) {
		v = y+j-2;
		if (v<0 || v+sh>dh) {
			for (i=1; i<=3; i++)
				map[i][j] = 0.0;
			continue;
		}
		for (i=1; i<=3; i++) {
			u = x+i-2;
			if (u<0 || u+sw>dw) {
				map[i][j] = 0.0;
				continue;
			}
			score = map[i][j] = PatternCorr(src,smemw,s_s,s_ss,s_len,msk,
			 dst+v*dmemw+u,dmemw,sw,sh);
			if (score>maxScore) {
				maxScore = score;
				maxI = i;
				maxJ = j;
			}
		}
	}
	subInfo->level = pixLevel;
	subInfo->x = x+maxI-2;
	subInfo->y = y+maxJ-2;
	subInfo->sl = map[maxI-1][maxJ];
	subInfo->st = map[maxI][maxJ-1];
	subInfo->sr = map[maxI+1][maxJ];
	subInfo->sb = map[maxI][maxJ+1];
	subInfo->sm = maxScore;
	return maxScore;
}

void CPatternMatch::PatternSubPixel(PATIMG img,PATIMG patimg,SUBINFO subInfo,float *rx,float *ry) 
{
	PBYTE	src,dst,msk;
	int	smemw,sw,sh,dmemw,dx,dy,dw,dh;
	float	s,ss;
	float	under;
	float	resX,resY,resMult;
	int	slen;

	src = patimg->bits;
	smemw = patimg->memw;
	sw = patimg->width;
	sh = patimg->height;
	s = patimg->patS;
	ss = patimg->patSS;
	slen = patimg->patLen;
	msk = patimg->patMask;
	dst = img->bits;
	dmemw = img->memw;
	dx = subInfo->x;
	dy = subInfo->y;
	dw = img->width;
	dh = img->height;
	if (subInfo->sl<-0.5)
		subInfo->sl = (dx>0)? PatternCorr(src,smemw,s,ss,slen,msk,
		 dst+dy*dmemw+dx-1,dmemw,sw,sh) : 0.0;
	if (subInfo->st<-0.5)
		subInfo->st = (dy>0)? PatternCorr(src,smemw,s,ss,slen,msk,
		 dst+(dy-1)*dmemw+dx,dmemw,sw,sh) : 0.0;
	if (subInfo->sr<-0.5)
		subInfo->sr = (dx+sw<=dw)? PatternCorr(src,smemw,s,ss,slen,msk,
		 dst+dy*dmemw+dx+1,dmemw,sw,sh) : 0.0;
	if (subInfo->sb<-0.5)
		subInfo->sb = (dy+sh<=dh)? PatternCorr(src,smemw,s,ss,slen,msk,
		 dst+(dy+1)*dmemw+dx,dmemw,sw,sh) : 0.0;
	under = 2.0*(subInfo->sl+subInfo->sr-2.0*subInfo->sm);
	resX = (under>-0.0000001)? (float)dx : (float)dx+(subInfo->sl-subInfo->sr)/under;
	under = 2.0*(subInfo->st+subInfo->sb-2.0*subInfo->sm);
	resY = (under>-0.0000001)? (float)dy : (float)dy+(subInfo->st-subInfo->sb)/under;
	resMult = pow(2.0,subInfo->level);
	*rx = resMult*resX;
	*ry = resMult*resY;
}

float CPatternMatch::PatternCorr_Angle(PBYTE src,int smemw,int sw,int sh,
		float s_s,float s_ss,int s_len,PBYTE mask,
		PBYTE dst,int dmemw,int dw,int dh,int x,int y,float degree) 
{
	unsigned int	val2;
	unsigned int	sum2,sum12,sum22;
	float	cost,sint;
	int	dx,u;
	float	drx,dry,dcx,dcy;
	PBYTE	soff,soffe,moff;
	float	s2,s12,s22;
	float	upper,below;

	// check
	if (x<0 || x>=dw || y<0 || y>=dh)
		return 0.0;
	cost = PatternCos(PI*degree/180.0);
	sint = PatternSin(PI*degree/180.0);
	// check
	dx = (int)(x+cost*(sw-1)+0.5);
	u = (int)(y+sint*(sw-1)+0.5);
	if (dx<0 || dx>=dw || u<0 || u>=dh)
		return 0.0;
	dx = (int)(x-sint*(sh-1)+0.5);
	u = (int)(y+cost*(sh-1)+0.5);
	if (dx<0 || dx>=dw || u<0 || u>=dh)
		return 0.0;
	dx = (int)(x+cost*(sw-1)-sint*(sh-1)+0.5);
	u = (int)(y+sint*(sw-1)+cost*(sh-1)+0.5);
	if (dx<0 || dx>=dw || u<0 || u>=dh)
		return 0.0;
	// begin
	sum2 = 0;
	s12 = s22 = 0.0;
	dx = sw-1;
	if (!mask) {
		for (soff=src,soffe=src+sh*smemw,drx=(float)x+0.5,dry=(float)y+0.5; soff<soffe;
		 soff+=smemw,drx-=sint,dry+=cost) {
			sum12 = sum22 = 0;
			for (u=0,dcx=drx,dcy=dry; u<=dx; u++,dcx+=cost,dcy+=sint) {
				val2 = dst[((int)dcy)*dmemw+(int)dcx];
				sum2 += val2;
				sum22 += val2*val2;
				sum12 += soff[u]*val2;
			}
			s12 += (float)sum12;
			s22 += (float)sum22;
		}
	}
	else {
		for (soff=src,soffe=src+sh*smemw,drx=(float)x+0.5,dry=(float)y+0.5,moff=mask;
		 soff<soffe; soff+=smemw,drx-=sint,dry+=cost,moff+=sw) {
			sum12 = sum22 = 0;
			for (u=0,dcx=drx,dcy=dry; u<=dx; u++,dcx+=cost,dcy+=sint) {
				if (!moff[u])
					continue;
				val2 = dst[((int)dcy)*dmemw+(int)dcx];
				sum2 += val2;
				sum22 += val2*val2;
				sum12 += soff[u]*val2;
			}
			s12 += (float)sum12;
			s22 += (float)sum22;
		}
	}
	s2 = (float)sum2;
	upper = s_len*s12-s_s*s2;
	below = (s_len*s_ss-s_s*s_s)*(s_len*s22-s2*s2);
	return (upper<=0.0 || below<0.01)? 0.0 : upper*upper/below;
}

float CPatternMatch::PatternDownSearch_Angle(PATIMG *dimg,PATIMG *patimg,int level,int pixLevel,
			float minScore,int x,int y,float degree,SUBINFO subInfo)
 {
	PBYTE	dst,src,msk;
	int	dmemw,dw,dh,smemw,sw,sh;
	float	maxScore,score;
	int	uMax,vMax;
	float	aMax;
	int	u,v,a;
	float	s_s,s_ss;
	float	da;
	int	s_len;

	// check
	if (level==pixLevel)
		return PatternBottomSearch_Angle(dimg[pixLevel],patimg[pixLevel],pixLevel,x,y,degree,subInfo);
	// set
	dst = dimg[level]->bits;
	dmemw = dimg[level]->memw;
	dw = dimg[level]->width;
	dh = dimg[level]->height;
	src = patimg[level]->bits;
	smemw = patimg[level]->memw;
	sw = patimg[level]->width;
	sh = patimg[level]->height;
	da = patimg[level]->dAngle;
	s_s = patimg[level]->patS;
	s_ss = patimg[level]->patSS;
	s_len = patimg[level]->patLen;
	msk = patimg[level]->patMask;
	// search max
	maxScore = -2.0;
	uMax = x;
	vMax = y;
	aMax = degree;
	for (a=-1; a<=1; a++)
	for (v=y-1; v<=y+2; v++)
	for (u=x-1; u<=x+2; u++) {
		score = PatternCorr_Angle(src,smemw,sw,sh,s_s,s_ss,s_len,msk,
		 dst,dmemw,dw,dh,u,v,degree+a*da);
		if (score>maxScore) {
			maxScore = score;
			uMax = u;
			vMax = v;
			aMax = degree+a*da;
		}
	}
	// decide recursion
	if (maxScore>=minScore)
		return PatternDownSearch_Angle(dimg,patimg,level-1,pixLevel,minScore,
		 uMax<<1,vMax<<1,aMax,subInfo);
	// otherwise
	subInfo->level = level;
	subInfo->x = uMax;
	subInfo->y = vMax;
	subInfo->degree = aMax;
	return maxScore;
}

void CPatternMatch::PatternSubPixel_Angle(PATIMG img,PATIMG patimg,SUBINFO subInfo,
		float *rx,float *ry,float *degree) 
{
	PBYTE	src,dst,msk;
	int	smemw,sw,sh,dmemw,dx,dy,dw,dh;
	float	dangle,da;
	float	s,ss;
	float	under;
	float	resX,resY,resMult;
	int	slen;

	src = patimg->bits;
	smemw = patimg->memw;
	sw = patimg->width;
	sh = patimg->height;
	s = patimg->patS;
	ss = patimg->patSS;
	slen = patimg->patLen;
	msk = patimg->patMask;
	dst = img->bits;
	dmemw = img->memw;
	dx = subInfo->x;
	dy = subInfo->y;
	dangle = subInfo->degree;
	da = subInfo->da;
	dw = img->width;
	dh = img->height;
	if (subInfo->sl<-0.5)
		subInfo->sl = PatternCorr_Angle(src,smemw,sw,sh,s,ss,slen,msk,
		 dst,dmemw,dw,dh,dx-1,dy,dangle);
	if (subInfo->st<-0.5)
		subInfo->st = PatternCorr_Angle(src,smemw,sw,sh,s,ss,slen,msk,
		 dst,dmemw,dw,dh,dx,dy-1,dangle);
	if (subInfo->sr<-0.5)
		subInfo->sr = PatternCorr_Angle(src,smemw,sw,sh,s,ss,slen,msk,
		 dst,dmemw,dw,dh,dx+1,dy,dangle);
	if (subInfo->sb<-0.5)
		subInfo->sb = PatternCorr_Angle(src,smemw,sw,sh,s,ss,slen,msk,
		 dst,dmemw,dw,dh,dx,dy+1,dangle);
	if (subInfo->su<-0.5)
		subInfo->su = PatternCorr_Angle(src,smemw,sw,sh,s,ss,slen,msk,
		 dst,dmemw,dw,dh,dx,dy,dangle-da);
	if (subInfo->sd<-0.5)
		subInfo->sd = PatternCorr_Angle(src,smemw,sw,sh,s,ss,slen,msk,
		 dst,dmemw,dw,dh,dx,dy,dangle+da);
	under = 2.0*(subInfo->sl+subInfo->sr-2.0*subInfo->sm);
	resX = (under>-0.0000001)? (float)dx : (float)dx+(subInfo->sl-subInfo->sr)/under;
	under = 2.0*(subInfo->st+subInfo->sb-2.0*subInfo->sm);
	resY = (under>-0.0000001)? (float)dy : (float)dy+(subInfo->st-subInfo->sb)/under;
	resMult = pow(2.0,subInfo->level);
	*rx = resMult*resX;
	*ry = resMult*resY;
	under = 2.0*(subInfo->su+subInfo->sd-2.0*subInfo->sm);
	*degree = (under>-0.0000001)? dangle : dangle+(subInfo->su-subInfo->sd)/under;
}

float CPatternMatch::PatternBottomSearch_Angle(PATIMG dimg, PATIMG patimg, int pixLevel,
		int x, int y, float degree, SUBINFO subInfo) 
{
	PBYTE	dst,src,msk;
	int	    dmemw,dw,dh,smemw,sw,sh;
	float	s_s, s_ss;
	float	map[5][5][5];
	int	    i,j,k,u,v;
	float	a;
	float	maxScore,score;
	int	    maxI,maxJ,maxK;
	float	da;
	int	    s_len;

	// set
	dst = dimg->bits;
	dmemw = dimg->memw;
	dw = dimg->width;
	dh = dimg->height;
	src = patimg->bits;
	smemw = patimg->memw;
	sw = patimg->width;
	sh = patimg->height;
	s_s = patimg->patS;
	s_ss = patimg->patSS;
	s_len = patimg->patLen;
	msk = patimg->patMask;
	da = patimg->dAngle;
	// search max
	maxScore = -2.0;
	for (k=0; k<=4; k++) {
		for (i=0; i<=4; i++)
			map[i][0][k] = map[i][4][k] = -1.0;
		for (j=1; j<=3; j++)
			map[0][j][k] = map[4][j][k] = -1.0;
	}
	for (i=1; i<=3; i++)
		for (j=1; j<=3; j++)
			map[i][j][0] = map[i][j][4] = -1.0;
	for (k=1; k<=3; k++) {
		a = degree+(k-2)*da;
		for (j=1; j<=3; j++) {
			v = y+j-2;
			for (i=1; i<=3; i++) {
				u = x+i-2;
				score = map[i][j][k] = PatternCorr_Angle(src,smemw,sw,sh,
				 s_s,s_ss,s_len,msk,dst,dmemw,dw,dh,u,v,a);
				if (score>maxScore) {
					maxScore = score;
					maxI = i;
					maxJ = j;
					maxK = k;
				}
			}
		}
	}
	subInfo->level = pixLevel;
	subInfo->x = x+maxI-2;
	subInfo->y = y+maxJ-2;
	subInfo->degree = degree+(maxK-2)*da;
	subInfo->da = da;
	subInfo->sl = map[maxI-1][maxJ][maxK];
	subInfo->st = map[maxI][maxJ-1][maxK];
	subInfo->sr = map[maxI+1][maxJ][maxK];
	subInfo->sb = map[maxI][maxJ+1][maxK];
	subInfo->su = map[maxI][maxJ][maxK-1];
	subInfo->sd = map[maxI][maxJ][maxK+1];
	subInfo->sm = maxScore;
	return maxScore;
}

float CPatternMatch::PatternCos(float theta) 
{
	static float	t0=0.0;
	static float	val=1.0;

	if (t0==theta)
		return val;
	return (val=cos(t0=theta));
}

float CPatternMatch::PatternSin(float theta)
 {
	static float	t0=0.0;
	static float	val=0.0;

	if (t0==theta)
		return val;
	return (val=sin(t0=theta));
}






#define  PATTERN_OFFSET	  180000
/*//////////////////////////////////////*/


int CPatternMatch::ImgAlloc_Alloc(int w, int h)
{
	return w*h;
}

LPBYTE CPatternMatch::ImgAlloc_GetStart(int a)
{
	return (unsigned char *)malloc(a*sizeof(int));
}

void CPatternMatch::ImgAlloc_Free(LPBYTE aaa)
{
	free(aaa);
}

/*BEGIN:  Gray Pattern Matching >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*///////////////////////////////////////
//------------------------------------------------------------------------------
// 한칸식 건너뛰며 선택된 Pixel에 대해서 그 Pixel과 이를 둘러싼 8개의 Pixel 즉
// 3x3의 Pixel의 평균값을 구해서 압축된 이미지의 Pixel값을 만든다.                   */
int CPatternMatch::MakePyramid(LPBYTE fmS,LPBYTE fmD,int nWidth, int nHeight, int WIDTH_MEM)
{
	int i,j;  
	LPBYTE fm1;
	LPBYTE fm2;
	LPBYTE fm3;
	const float W025=0.25;
	const float W050=0.50;
	LPBYTE fmD_=fmD;

	  
	  for(i=1; i < nHeight-1 ; i+=2) 
	     for(j=1; j < nWidth-1; j+=2) 
		 {

		     fm1= fmS+j+(i-1)*WIDTH_MEM;
		     fm2= fm1+WIDTH_MEM;
		     fm3= fm2+WIDTH_MEM;

		     *(fmD++)= W025*(W025**(fm1-1)+W050**fm1+W025**(fm1+1))+
		               W050*(W025**(fm2-1)+W050**fm2+W025**(fm2+1))+
		               W025*(W025**(fm3-1)+W050**fm3+W025**(fm3+1));
	        		    
		 }
	  
	  return 0;
}


float	CPatternMatch::GrayMatch(LPBYTE fstart,int fwidth,int x,int y,int w,int h,
	              LPBYTE mstart,int mwidth,int mw,int mh,float *rx,float *ry,int SUBFLAG,
				  int nCandiNumber, float m_dConfuseCorrValue, float m_dAcceptCorrValue)
{
	int i;
	int nPyramidLevel;
    /* Template관련 데이타 */

	float m_dCorrValue;				 /* Return 할 Corelation 값을 저장한다. */
	float m_dCorrX;					 /* Return 할 *rx의 중간값을 여기서 보관 */
	float m_dCorrY;					 /* Return 할 *ry의 중간값을 여기서 보관 */

	//KONEW(3)
	float dMaxCorrValue;
	int nMaxCorrValuePos;
	int m_nFoundCandiNumber;
	float CandiData[MAXCANDIDATENUMBER][3];  /* [0]: x좌표, [1]: y좌표, [2]: correlation */
	                        
	/* BEGIN-0 : TEMPLATE TEACHING START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /*  Template의 Width 와 Height  */
    m_TempWidth[0] =mw;                             /*  1     예) 64     66    */
    m_TempWidth[1] =(int)(m_TempWidth[0]/2.0-0.5);  /*  1/2       31     32    */
    m_TempWidth[2] =(int)(m_TempWidth[1]/2.0-0.5);  /*  1/4       15     15    */
    m_TempWidth[3] =(int)(m_TempWidth[2]/2.0-0.5);  /*  1/8        7      7    */

    m_TempHeight[0]=mh;	        					/*  1         65     67    */
    m_TempHeight[1]=(int)(m_TempHeight[0]/2.0-0.5); /*  1/2       32     33	   */
    m_TempHeight[2]=(int)(m_TempHeight[1]/2.0-0.5); /*  1/4       15     16	   */
    m_TempHeight[3]=(int)(m_TempHeight[2]/2.0-0.5); /*  1/8        7      7    */

	if( mw> 60 && mh> 60)      nPyramidLevel=4;
	else if(mw >30 && mh >30 ) nPyramidLevel=3;
	else if(mw >15 && mh >15 ) nPyramidLevel=2;
	else  					   return -1.0;
		          
	/* Template와 같은 영상을 m_fmTemp[0]에 저장*/
	m_fmTemp[0]=mstart;
	for(i=1;i<nPyramidLevel;i++) 
	{
		m_fmTemp[i] = new unsigned char[m_TempWidth[i] * m_TempHeight[i]];
	}

	/* Template를 1/2, 1/4, 1/8로 축소한다 */
	MakePyramid(m_fmTemp[0],m_fmTemp[1],m_TempWidth[0],m_TempHeight[0],mwidth);
	for(i=2;i<nPyramidLevel;i++)
	   MakePyramid(m_fmTemp[i-1],m_fmTemp[i],m_TempWidth[i-1],m_TempHeight[i-1],m_TempWidth[i-1]);
	/* END-0 : TEMPLATE TEACHING END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

	/* BEGIN-0 : SEARCH AREA TEACHING START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /* Search Area 의 Width 와 Height  */
    m_SearchWidth[0] =w;                                /*  1     예) 64     66    */
    m_SearchWidth[1] =(int)(m_SearchWidth[0]/2.0-0.5);  /*  1/2       31     32    */
    m_SearchWidth[2] =(int)(m_SearchWidth[1]/2.0-0.5);  /*  1/4       15     15    */
    m_SearchWidth[3] =(int)(m_SearchWidth[2]/2.0-0.5);  /*  1/8        7      7	   */

    m_SearchHeight[0]=h;							    /*  1         65     67	   */
    m_SearchHeight[1]=(int)(m_SearchHeight[0]/2.0-0.5); /*  1/2       32     33	   */
    m_SearchHeight[2]=(int)(m_SearchHeight[1]/2.0-0.5); /*  1/4       15     16	   */
    m_SearchHeight[3]=(int)(m_SearchHeight[2]/2.0-0.5); /*  1/8        7      7    */
 
    
	/* 새로운 메모리를 할당 받는다. */
	m_fmSearch[0]=fstart+fwidth*y+x;
	for(i=1;i<nPyramidLevel;i++) 
		m_fmSearch[i]=new unsigned char[m_SearchWidth[i] * m_SearchHeight[i]];

//	m_fmSearch[1]=m_fmTemp[3]+m_TempWidth[3]*m_TempHeight[3];
//	m_fmSearch[2]=m_fmSearch[1]+m_SearchWidth[1]*m_SearchHeight[1];
//	m_fmSearch[3]=m_fmSearch[2]+m_SearchWidth[2]*m_SearchHeight[2];

		          
	/* Searching Area를 1/2, 1/4, 1/8로 축소한다 */
	MakePyramid(m_fmSearch[0],m_fmSearch[1],m_SearchWidth[0],m_SearchHeight[0],fwidth);
	for(i=2;i<nPyramidLevel;i++)
	   MakePyramid(m_fmSearch[i-1],m_fmSearch[i],m_SearchWidth[i-1],m_SearchHeight[i-1],m_SearchWidth[i-1]);
	/* BEGIN-0 : SEARCH AREA TEACHING END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


///////////////////////////////////// 서치영역, 패턴 Display /////////////////////////////////////
// OK
/*	fm=GrabNLive(1);

	for(i=0;i<m_SearchHeight[3];i++)
		for(j=0;j<m_SearchWidth[3];j++)
			*(fm+IMAGE_PITCH*(i+240)+j+320)=*(m_fmSearch[3]+m_SearchWidth[3]*i+j);
	for(i=0;i<m_TempHeight[3];i++)
		for(j=0;j<m_TempWidth[3];j++)
			*(fm+IMAGE_PITCH*(i+300)+j+50)=*(m_fmTemp[3]+m_TempWidth[3]*i+j);
	ShowImage(1);
	T_Delay(5000);
	fm=GrabNLive(1);
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

	switch(	nPyramidLevel )
	{
	   case 4: if( MatchingSubForTop(m_fmTemp[3],m_fmSearch[3], m_TempWidth[3],m_TempHeight[3], 
				   m_SearchWidth[3],m_SearchHeight[3], nCandiNumber, m_dConfuseCorrValue,
				   CandiData,&m_nFoundCandiNumber))     goto END; //KONEW

		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
			    MatchingSub(m_fmTemp[2],m_fmSearch[2], m_TempWidth[2],m_TempHeight[2], 
				   m_SearchWidth[2],m_SearchHeight[2],&CandiData[i][0],&CandiData[i][1]);
			    MatchingSub(m_fmTemp[1],m_fmSearch[1], m_TempWidth[1],m_TempHeight[1], 
				   m_SearchWidth[1],m_SearchHeight[1],&CandiData[i][0],&CandiData[i][1]);
	            MatchingForBottom(m_fmTemp[0],m_fmSearch[0], m_TempWidth[0],m_TempHeight[0], 
				   m_SearchWidth[0],m_SearchHeight[0],&CandiData[i][0],&CandiData[i][1],&CandiData[i][2],fwidth, mwidth);
                /* 구한 Correlation 값이 m_dAcceptCorrValue보다 크면 여기서 STOP한다 */
				if(CandiData[i][2]>m_dAcceptCorrValue)
				{   
					m_nFoundCandiNumber=i+1;
					break;
				}
			 }
			   break;
	   case 3: if( MatchingSubForTop(m_fmTemp[2],m_fmSearch[2], m_TempWidth[2],m_TempHeight[2], 
				   m_SearchWidth[2],m_SearchHeight[2],nCandiNumber, m_dConfuseCorrValue,
				   CandiData, &m_nFoundCandiNumber))   goto END; //KONEW
		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
			   MatchingSub(m_fmTemp[1],m_fmSearch[1], m_TempWidth[1],m_TempHeight[1], 
				   m_SearchWidth[1],m_SearchHeight[1],&CandiData[i][0],&CandiData[i][1]);
	           MatchingForBottom(m_fmTemp[0],m_fmSearch[0], m_TempWidth[0],m_TempHeight[0], 
				   m_SearchWidth[0],m_SearchHeight[0],&CandiData[i][0],&CandiData[i][1],&CandiData[i][2],fwidth, mwidth);
                /* 구한 Correlation 값이 m_dAcceptCorrValue보다 크면 여기서 STOP한다 */
                if(CandiData[i][2]>m_dAcceptCorrValue)
				{   
					m_nFoundCandiNumber=i+1;
					break;
				}
			 }
			   break;
	   case 2: if( MatchingSubForTop(m_fmTemp[1],m_fmSearch[1], m_TempWidth[1],m_TempHeight[1], 
				   m_SearchWidth[1],m_SearchHeight[1], nCandiNumber, m_dConfuseCorrValue,
				   CandiData, &m_nFoundCandiNumber))        goto END; 
		   		     //KONEW  
		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
	            MatchingForBottom(m_fmTemp[0],m_fmSearch[0], m_TempWidth[0],m_TempHeight[0], 
				   m_SearchWidth[0],m_SearchHeight[0],&CandiData[i][0],&CandiData[i][1],&CandiData[i][2],fwidth, mwidth);

                /* 구한 Correlation 값이 m_dAcceptCorrValue보다 크면 여기서 STOP한다 */
                if(CandiData[i][2]>m_dAcceptCorrValue)
				{   
					m_nFoundCandiNumber=i+1;
					break;
				}
			 }
			   break;
	}
	/* BEGIN-0 : 모든 후보에 대한 Corelation 값이 구해졌으면 이중에 가장 높은 값을 
	//           구하고자 하는 값으로 결정짓는다. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
	dMaxCorrValue=CandiData[0][2];
	nMaxCorrValuePos=0;
	for(i=1;i<m_nFoundCandiNumber;i++)
	   if(CandiData[i][2]>dMaxCorrValue)
	   {
		   dMaxCorrValue=CandiData[i][2];
		   nMaxCorrValuePos=i;
	   }

	m_dCorrValue=dMaxCorrValue;
	m_dCorrX=CandiData[nMaxCorrValuePos][0];
	m_dCorrY=CandiData[nMaxCorrValuePos][1];

	if(m_dCorrValue <=0 ) goto END; /* 가장높은 Correlation값이 0이하면 STOP */
	/*END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
	
	if(SUBFLAG)
	{
	    if(MatchingForSubPixel(m_fmTemp[0],m_fmSearch[0], m_TempWidth[0],m_TempHeight[0], 
				               m_SearchWidth[0],m_SearchHeight[0], &m_dCorrX, &m_dCorrY, &m_dCorrValue, fwidth, mwidth))goto END;

	}
	*rx=m_dCorrX+x;
	*ry=m_dCorrY+y;

	for(i=1;i<nPyramidLevel;i++) 
	{
	   delete m_fmTemp[i];
	   delete m_fmSearch[i];
	}
	return 	m_dCorrValue;


END: *rx=*ry=0;
	 for(i=1;i<nPyramidLevel;i++) 
	 {
	   delete m_fmTemp[i];
	   delete m_fmSearch[i];
	 }
	 
	 return -1.0;
}

float pCorrMap[86][66];
int CPatternMatch::MatchingSubForTop(LPBYTE Tfm,LPBYTE Sfm, int Tw, int Th, int Sw, int Sh, int m_nCandiNumber, int ThreCorr,
					  float CandiData[][3], int *m_nFoundCandiNumber)
{	
	int i,j,k,l,ii, dum;	 //KONEW
	int FindLocalPeak;
	LPBYTE fmT;
	LPBYTE fmS;
	int nTempWidth,nTempHeight;
	int nSearchWidth,nSearchHeight;

    int tempT, tempS;
	LPBYTE fmT_;
	LPBYTE fmS_;
	LPBYTE fmS__;
	/*///////////////// Correration Data ///////////////////////////////*/
	float Area, Sig_Ti, Sig_Ti2;
	float       Sig_Si, Sig_Si2;
	float       Sig_TiSi;
	float dDiv;
	float dCorrData, dMaxCorrData=0;
    int   Iter_Number=0;

	//KONEW
	float dThreshold=(float)0.2;
	float dMinCorrValue;
	int   nMinCorrPos;
	int  Corr_N, Corr_M;
//	float **pCorrMap;


    fmT        = Tfm;
	nTempWidth = Tw;
	nTempHeight= Th;

	fmS          = Sfm;
	nSearchWidth = Sw;
	nSearchHeight= Sh;

    //Memory Allocation>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	Corr_N=nSearchWidth-nTempWidth+5;
	Corr_M=nSearchHeight-nTempHeight+5;
/*
	pCorrMap= (float **)malloc( Corr_N*sizeof(float *));
	if(!pCorrMap) return 1;
	for(i=0;i<Corr_N;i++)
		pCorrMap[i]=(float *)malloc(Corr_M*sizeof(float));
*/
    
    /* BEGIN-0 : Template데이타 구하기 >>>>>>>*/
    Sig_Ti = Sig_Ti2  =0;
	Area = nTempWidth*nTempHeight;
	for(i=0;i<nTempHeight;i++) 
	 {
		fmT_ = fmT + i*nTempWidth;
		for(j=0;j<nTempWidth ;j++) 
		{
		   tempT    = *fmT_++;
		   Sig_Ti  += tempT;
		   Sig_Ti2 += tempT*tempT;
		}
	 }
	/*END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	/* BEGIN-0 : Correlation Map 구하기 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
	for(k=0; k <=nSearchHeight-nTempHeight ; k++) 
	{
	   for(l=0; l<=nSearchWidth-nTempWidth ; l++) 
	   {
          Sig_Si = Sig_Si2  = Sig_TiSi =0;
		  fmS_ = fmS + nSearchWidth*k+l;
		  for(i=0;i< nTempHeight ;i++) 
		  {
			 fmT_  = fmT + nTempWidth*i;
			 fmS__ = fmS_+ nSearchWidth*i;
			 for(j=0 ;j< nTempWidth ;j++) 
			 {
				tempT   = *fmT_++;
				tempS   = *fmS__++;
				Sig_Si  += tempS;
				Sig_Si2 += tempS*tempS;
				Sig_TiSi+= tempS*tempT;
			 }
		  }
		  dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );
		  if (dDiv > 0.0001 ) dCorrData = ( Area*Sig_TiSi - Sig_Ti*Sig_Si)/dDiv;
		  else                dCorrData = 0.0;
		  pCorrMap[l+2][k+2]=dCorrData;
	   }
	}
	/* END-0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


   	/* BEGIN-0: Correlation Map으로 부터 후보점들을 찾는다. >>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /* Correlaton Map 주위 각 2픽셀씩을 0으로 채운다.  */
	dum=nSearchHeight-nTempHeight+5-2;
	for(l=0; l<nSearchWidth-nTempWidth+5 ; l++) 
		pCorrMap[l][0]=pCorrMap[l][1]=pCorrMap[l][dum]=pCorrMap[l][dum+1]=0.0;

	dum=nSearchWidth-nTempWidth+5-2;
	for(k=0; k <nSearchHeight-nTempHeight+5 ; k++) 
		pCorrMap[0][k]=pCorrMap[1][k]=pCorrMap[dum][k]=pCorrMap[dum+1][k]=0.0;

	/*END-1: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

////////////////////////////////DEBUG////////////////////////////////////////////////
/*	fm=FRAME_Origin(FA0, AD_GetOffsetX(), AD_GetOffsetY());
	for(i=0;i<(nSearchHeight-nTempHeight+5)*4;i++)
		for(j=0;j<(nSearchWidth-nTempWidth+5)*4;j++)
			if(pCorrMap[j/4][i/4]>0)
				*(fm+IMAGE_PITCH*(i+70)+j+50)=pCorrMap[j/4][i/4]*200;
			else
				*(fm+IMAGE_PITCH*(i+70)+j+50)=0;
*/
//////////////////////////////////////////////////////////////////////////////////////



	/* BEGIN-1: Local Max를 찾아서 후보데이타를 구한다. >>>>>>>>>>>>>>>>>>>> */
	for(k=2; k <=nSearchHeight-nTempHeight+2 ; k++) 
	{
	   for(l=2; l<=nSearchWidth-nTempWidth+2 ; l++) 
	   {
		  if(pCorrMap[l][k] >= dThreshold)			 
		  {
			 FindLocalPeak=1;
			 for(ii=k-2; ii<=k+2; ii++)			    
				if( pCorrMap[l-2][ii] > pCorrMap[l][k] || pCorrMap[l-1][ii] > pCorrMap[l][k] ||
				    pCorrMap[l  ][ii] > pCorrMap[l][k] || pCorrMap[l+1][ii] > pCorrMap[l][k] ||
				    pCorrMap[l+2][ii] > pCorrMap[l][k] )  { FindLocalPeak =0; break; }

			 /*BEGIN-2 : 이점이 Local Max이라면 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
			 if(FindLocalPeak)
			 {	 
			    if( Iter_Number < m_nCandiNumber) /*는 데이타가 구하고자하는 수보다 작으면 저장*/
			    {
			       CandiData[Iter_Number][0]=(float)(l-2);
			       CandiData[Iter_Number][1]=(float)(k-2);
			       CandiData[Iter_Number++][2]=pCorrMap[l][k];
//				   *(fm+IMAGE_PITCH*(70+k*4+2)+50+l*4+2)=255;//@
			    }
			    else/*이미 후보데이타보다 많이 찾았다면 후보중 가장 낮은값을 구해서 이를 없애고 */
			    {	 /* 새로 찾은 값을 저장한다. */

				   dMinCorrValue=CandiData[0][2];
				   nMinCorrPos=0;
				   for(ii=1; ii<m_nCandiNumber; ii++)
					  if(	CandiData[ii][2]< dMinCorrValue) 
				      { 
					     dMinCorrValue=CandiData[ii][2];
					     nMinCorrPos=ii;
				      }
	               dThreshold=dMinCorrValue;
				   if( pCorrMap[l][k] >	dMinCorrValue) /* 구한값이 기존의 가장작은것 보다 크면 저장 */
				   {
				      CandiData[nMinCorrPos][0]=(float)(l-2);
			          CandiData[nMinCorrPos][1]=(float)(k-2);
			          CandiData[nMinCorrPos][2]=pCorrMap[l][k];
//					  *(fm+IMAGE_PITCH*(70+k*4+2)+50+l*4+2)=255;//@
				   }
			    }
		     } /* end of if */
			 /*END-2<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
	      }
       }  /* end of loop */
	}
	/*END-1<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	/* 구한값을 소팅한다 >>>*/
     MatchSort(Iter_Number, CandiData);
	 *m_nFoundCandiNumber= Iter_Number;

//	ShowImage(1);	//@
//	T_Delay(3000);	//@
//	ShowImage(0);	//@

//	 for(i=0;i<Corr_N;i++) free(pCorrMap[i]);
//	 free(pCorrMap);
	/*END-0<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	 if(Iter_Number==0) return 1; 
	 else               return 0;
}

int CPatternMatch::MatchSort(int count, float Value[][3])
{
	int i,j;
	float MaxValue;
	int Number;
	float StoreValue, StoreXData, StoreYData;

	   for(i=0;i<count-1;i++) /* N개의 Sortting시 N-1의 루프만돌리면 된다 */
	   {
		  Number=i;
		  StoreXData=Value[i][0];
		  StoreYData=Value[i][1];
		  MaxValue=StoreValue=Value[i][2];
	      
		  for(j=i+1;j<count;j++)
			 if(Value[j][2] > MaxValue) { MaxValue=Value[j][2]; Number=j; }  
		  
		  Value[i][0]=Value[Number][0];
		  Value[i][1]=Value[Number][1];
		  Value[i][2]=Value[Number][2];

		  Value[Number][0]=StoreXData;
		  Value[Number][1]=StoreYData;
		  Value[Number][2]=StoreValue;
	   }
	   return 0;
}


int CPatternMatch::MatchingSub(LPBYTE Tfm,LPBYTE Sfm, int Tw, int Th, int Sw, int Sh, float *ReturnX, float *ReturnY)
{	
	int i,j,k,l;
	LPBYTE fmT;
	LPBYTE fmS;
	int nTempWidth,nTempHeight;
	int nSearchWidth,nSearchHeight;

    int tempT, tempS;
	LPBYTE fmT_;
	LPBYTE fmS_;
	LPBYTE fmS__;
	/*///////////////// Correlation Data ///////////////////////////////*/
	float Area, Sig_Ti, Sig_Ti2;
	float       Sig_Si, Sig_Si2;
	float       Sig_TiSi;
	float dDiv;
	float dCorrData,dMaxCorrData=0;
    int Iter_Number=0;
    int nLeft, nTop;    /*새로 검사할 영역의 좌상귀의 좌표 */
	int StartX,EndX, StartY,EndY;

	nLeft = (int)(2**ReturnX);
	nTop  = (int)(2**ReturnY);

	nTempWidth = Tw;
	nTempHeight= Th;
    fmT        = Tfm;

	nSearchWidth = Sw;
	nSearchHeight= Sh;
	fmS          = Sfm+nSearchWidth*nTop+nLeft;


    /*BEGIN-0 : 피라밋의 한단계 아래로 내려왔을때 서치 영역을 5x5로 잡기위해서
	//          아래와 같이 한다. 특히 서치영역을 넘어가는것을 방지하기 위해서
	//          If문을 사용했음 */
	if(nLeft>0) StartX=-1;
    else        StartX=0;
	if(nTop>0)  StartY=-1;
    else        StartY=0;
    if( (nLeft+nTempWidth+2)<nSearchWidth ) EndX=3;
	else									EndX=nSearchWidth-(nLeft+nTempWidth);
    if( (nTop+nTempHeight+2)<nSearchHeight ) EndY=3;
	else									 EndY=nSearchHeight-(nTop+nTempHeight);

    /*END -0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*BEGIN-0 : Template데이타 구하기 >>>>>>>*/
    Sig_Ti = Sig_Ti2  =0;
	Area = nTempWidth*nTempHeight;
	for(i=0;i<nTempHeight;i++) 
	{
		fmT_ = fmT + i*nTempWidth;
		for(j=0;j<nTempWidth ;j++) 
		{
		   tempT    = *fmT_++;
		   Sig_Ti  += tempT;
		   Sig_Ti2 += tempT*tempT;
		}
	 }
	/*END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	for(k=StartY; k <= EndY ; k++) 
	{
	   for(l=StartX; l<= EndX ; l++) 
	   {
          Sig_Si = Sig_Si2  = Sig_TiSi =0;
		  fmS_ = fmS + nSearchWidth*k+l;
		  for(i=0;i< nTempHeight ;i++) 
		  {
			 fmT_  = fmT + nTempWidth*i;
			 fmS__ = fmS_+ nSearchWidth*i;
			 for(j=0 ;j< nTempWidth ;j++) 
			 {
				tempT   = *fmT_++;
				tempS   = *fmS__++;
				Sig_Si  += tempS;
				Sig_Si2 += tempS*tempS;
				Sig_TiSi+= tempS*tempT;
			 }
		  }
		  dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );
		  if (dDiv > 0.0001 ) dCorrData = ( Area*Sig_TiSi - Sig_Ti*Sig_Si)/dDiv;
		  else                dCorrData = 0.0;

		  /* Classification of Match Degree */
		  if(dCorrData >= 0.2) 
		  {
			 Iter_Number++;
			 if(dCorrData > dMaxCorrData) 
			 {
				*ReturnX=(float)(nLeft+l);
				*ReturnY=(float)(nTop +k);
			    dMaxCorrData=dCorrData;
			 }
		  } /* end of if */
	    }
	 }  /* end of loop */
	 if(Iter_Number==0) return 1;
	 else               return 0;
}

int CPatternMatch::MatchingForBottom(LPBYTE Tfm,LPBYTE Sfm, int Tw, int Th, int Sw, int Sh,
					  float *ReturnX, float *ReturnY, float *m_dCorrValue, int WIDTH_MEM, 
					  int mwidth)
{	
	int i,j,k,l;
	LPBYTE fmT;
	LPBYTE fmS;
	int nTempWidth,nTempHeight;
	int nSearchWidth,nSearchHeight;

    int tempT, tempS;
	LPBYTE fmT_;
	LPBYTE fmS_;
	LPBYTE fmS__;
	/*///////////////// Correlation Data ///////////////////////////////*/
	float Area, Sig_Ti, Sig_Ti2;
	float       Sig_Si, Sig_Si2;
	float       Sig_TiSi;
	float dDiv;
	float dCorrData,dMaxCorrData=0;
    int Iter_Number=0;
    int nLeft, nTop;    /*새로 검사할 영역의 좌상귀의 좌표 */
	int StartX,EndX, StartY,EndY;


	nLeft = (float)(2**ReturnX);
	nTop  = (float)(2**ReturnY);

	nTempWidth = Tw;
	nTempHeight= Th;
    fmT        = Tfm;

	nSearchWidth = Sw;
	nSearchHeight= Sh;
	fmS          = Sfm+WIDTH_MEM*nTop+nLeft;

    /*BEGIN-0 : 피라밋의 한단계 아래로 내려왔을때 서치 영역을 5x5로 잡기위해서
	//          아래와 같이 한다. 특히 서치영역을 넘어가는것을 방지하기 위해서
	//          If문을 사용했음  */
	if(nLeft>0) StartX=-1;
    else        StartX=0;
	if(nTop>0)  StartY=-1;
    else        StartY=0;
    if( (nLeft+nTempWidth+2)<nSearchWidth ) EndX=3;
	else									EndX=nSearchWidth-(nLeft+nTempWidth);
    if( (nTop+nTempHeight+2)<nSearchHeight ) EndY=3;
	else									 EndY=nSearchHeight-(nTop+nTempHeight);
    /*END -0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


    /*BEGIN-0 : Template데이타 구하기 >>>>>>>*/
    Sig_Ti = Sig_Ti2  =0;
	Area = 0;
	for(i=0;i<nTempHeight;i+=2) 
	{
		fmT_ = fmT + i*mwidth;
		for(j=0;j<nTempWidth ;j+=2) 
		{
		   tempT    = *(fmT_+j);
		   Sig_Ti  += tempT;
		   Sig_Ti2 += tempT*tempT;
		   Area++;
		}
	 }
	/*END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	for(k=StartY; k <= EndY ; k++) 
	{
	   for(l=StartX; l<= EndX ; l++) 
	   {
          Sig_Si = Sig_Si2  = Sig_TiSi =0;
		  fmS_ = fmS + WIDTH_MEM*k+l;
		  for(i=0;i< nTempHeight ;i+=2) 
		  {
			 fmT_  = fmT + mwidth*i;
			 fmS__ = fmS_+ WIDTH_MEM*i;
			 for(j=0 ;j< nTempWidth ;j+=2) 
			 {
				tempT   = *(fmT_+j);
				tempS   = *(fmS__+j);
				Sig_Si  += tempS;
				Sig_Si2 += tempS*tempS;
				Sig_TiSi+= tempS*tempT;
			 }
		  }
		  dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );
		  if (dDiv > 0.0001 ) dCorrData = ( Area*Sig_TiSi - Sig_Ti*Sig_Si)/dDiv;
		  else                dCorrData = 0.0;

		  /* Classification of Match Degree */
		  if(dCorrData >= 0.2) 
		  {
			 Iter_Number++;
			 if(dCorrData > dMaxCorrData) 
			 {
				*ReturnX=(float)(nLeft+l);
				*ReturnY=(float)(nTop +k);
			    *m_dCorrValue=dMaxCorrData=dCorrData;
			 }
		  } /* end of if */
	    }
	 }  /* end of loop */

	
	if(Iter_Number==0) return 1;
	else               return 0;
}

int CPatternMatch::MatchingForSubPixel(LPBYTE Tfm,LPBYTE Sfm, int Tw, int Th, int Sw, int Sh,
					    float *ReturnX, float *ReturnY, float *m_dCorrValue, int WIDTH_MEM, 
					    int mwidth)
{	
	int i,j,k,l;
	LPBYTE fmT;
	LPBYTE fmS;
	int nTempWidth,nTempHeight;
	int nSearchWidth,nSearchHeight;

    int tempT, tempS;
	LPBYTE fmT_;
	LPBYTE fmS_;
	LPBYTE fmS__;
	/*///////////////// Correlation Data ///////////////////////////////*/
	float Area, Sig_Ti, Sig_Ti2;
	float       Sig_Si, Sig_Si2;
	float       Sig_TiSi;
	float dDiv;
	float dCorrData,dMaxCorrData=0;
    int nLeft, nTop;    /*새로 검사할 영역의 좌상귀의 좌표 */
	/* SubPixel을 위해 사용하는 변수들 */
	float dCorrX, dCorrY;
	float dCorData[9][3];
	int nCount=0;


	nLeft = (float)*ReturnX;
	nTop  = (float)*ReturnY;

	nTempWidth = Tw;
	nTempHeight= Th;
    fmT        = Tfm;

	nSearchWidth = Sw;
	nSearchHeight= Sh;
	fmS          = Sfm+WIDTH_MEM*nTop+nLeft;


    /* BEGIN-0 : Template데이타 구하기 >>>>>>>*/
    Sig_Ti = Sig_Ti2  =0;
	Area = 0;
	for(i=0;i<nTempHeight;i+=2) 
	{
		fmT_ = fmT + i*mwidth;
		for(j=0;j<nTempWidth ;j+=2) 
		{
		   tempT    = *(fmT_+j);
		   Sig_Ti  += tempT;
		   Sig_Ti2 += tempT*tempT;
		   Area++;
		}
	 }
	/* END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/



/*BEGIN-0: SubPixel처리하는 부분  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  
	if(  nLeft>0 && (nLeft+nTempWidth) < nSearchWidth  && 
		 nTop>0  && (nTop+nTempHeight) < nSearchHeight   )
	{

	  for(k=-1;k<=1;k++)
		for(l=-1;l<=1;l++)
	    {
          Sig_Si = Sig_Si2  = Sig_TiSi =0;
		  fmS_ = fmS + WIDTH_MEM*k+l;
		   for(i=0;i< nTempHeight ;i+=2) 
		   {
			  fmT_  = fmT + mwidth*i;
			  fmS__ = fmS_+ WIDTH_MEM*i;
			  for(j=0 ;j< nTempWidth ;j+=2) 
			  {
				 tempT   = *(fmT_+j);
				 tempS   = *(fmS__+j);
				 Sig_Si  += tempS;
				 Sig_Si2 += tempS*tempS;
				 Sig_TiSi+= tempS*tempT;
			  }
		   }
		   dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );
		   if (dDiv > 0.0001 ) dCorrData = ( Area*Sig_TiSi - Sig_Ti*Sig_Si)/dDiv;
		   else                dCorrData = 0.0;

		   dCorData[nCount][0] = *ReturnX+l;
		   dCorData[nCount][1] = *ReturnY+k;
		   dCorData[nCount++][2] = dCorrData;

	    }
		

	    if(!FindSecondPolynomialPeak(9,dCorData,&dCorrX,&dCorrY))
			if( fabs( *ReturnX - dCorrX) <1 && fabs( *ReturnY - dCorrY) <1)
			{
		       *ReturnX= dCorrX;
	           *ReturnY= dCorrY;
			}
	  }

  /*END-0:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	*/
  for(i=0;i<9;i++) if(dCorData[i][2]==0) return 1;
  return 0;
}


float A[9][9];
float AT[9][9];
float ATA[9][9];
float ATA_Inverse[9][9];
float B[9],ATB[9],R[9];

int CPatternMatch::FindSecondPolynomialPeak(int N, float Data[][3], float *rx, float *ry)
{
	int i;
	int  StartX, StartY;
	
	StartX=Data[0][0]-1;
	StartY=Data[0][1]-1;

	/* 위치데이타 값이 크면 메트릭스 연산때 float를 넘어가므로 한단위 숫자로 바꿔준다 */
	for(i=0;i<N;i++)
	{ 
	   A[i][0]=(Data[i][0]-StartX)*(Data[i][0]-StartX);
	   A[i][1]=(Data[i][1]-StartY)*(Data[i][1]-StartY);
	   A[i][2]=(Data[i][0]-StartX)*(Data[i][1]-StartY);
	   A[i][3]=(Data[i][0]-StartX);
	   A[i][4]=(Data[i][1]-StartY);
	   A[i][5]=1;
	   B[i]   =Data[i][2];
	}

	MatrixTranspose(9, 6, A, AT);
	MatrixMultiplicant(6, 9,AT, 9, 6, A, ATA);
	MatrixInverse(6, ATA, ATA_Inverse);
	MatrixMultiplicant2(6, 9, AT, B, ATB);
	MatrixMultiplicant2(6, 6, ATA_Inverse, ATB, R);

	if( (R[2]*R[2]-4*R[0]*R[1]) !=0.)
	{
		*rx=(2*R[3]*R[1]-R[2]*R[4])/(R[2]*R[2]-4*R[0]*R[1])+StartX;
		*ry=(2*R[0]*R[4]-R[3]*R[2])/(R[2]*R[2]-4*R[0]*R[1])+StartY;
	}
	else 
		return 1;

	return 0;
}

int CPatternMatch::MatrixTranspose(int n, int m, float A[][9], float Ai[][9])
{
	int i,j;
	for(i=0;i<n;i++)
	   for(j=0;j<m;j++)
          Ai[j][i]=A[i][j];	      

	return 0;
}
/* Matrix Mulpiplicant
// Size [n1 x m1] [n2 x m2] => [n1 x m2]    m1==n2
// [A][B]=[C]                           */
int CPatternMatch::MatrixMultiplicant(int n1, int m1, float A[][9], int n2, int m2, float B[][9], float C[][9])
{
	int i,j,k;

	if(	n1 > 9 || m1 >9 || n2 >9 || m2 >9 || m1!=n2) return 1;
	
	for(i=0 ; i<n1; i++)
		for(j=0; j<m2; j++)
		   C[i][j]=0;


	for(i=0 ; i<n1; i++)
		for(j=0; j<m2; j++)
		    for(k=0; k<m1; k++)
			   C[i][j]+=A[i][k]*B[k][j];

	return 0;
}

/* Matrix Mulpiplicant
// Size [n x m] [m x 1] => [n x 1]
// [A][B]=[C]					    */
int CPatternMatch::MatrixMultiplicant2(int n, int m, float A[][9], float B[], float C[])
{
	int i,j;

	if(	n > 9 || m >9 ) return 1;
	
	for(i=0 ; i<n; i++)
		   C[i]=0;


	for(i=0 ; i<n; i++)
		for(j=0; j<m; j++)
			   C[i]+=A[i][j]*B[j];

	return 0;
}

/* MAtrix Inverse
// Size n x n
// Inverse [A] => [Ai]
// return 0: 정상 NONZERO 불량   */
int CPatternMatch::MatrixInverse(int n, float A[][9], float Ai[][9])
{
   float big, pivot_inverse, temp, abs_element;
   int pivot_flag[9], swap_col[9], swap_row[9];
   int i,j, row,col,swap,irow,icol;

   if(	n > 9) return 1;
   
   for(i=0;i<n;i++)   
	   for(j=0;j<n;j++)
   			Ai[i][j]=A[i][j];

   for(i=0;i<9;i++)
     pivot_flag[i]=swap_row[i]=swap_col[i]=0;
   

   for(i=0;i<n;i++)  /* iterations of pivots */
   {
	   big=0.0;
       for(row=0; row <n ;row++)
	   {
		   if(!pivot_flag[row]) /* only unused pivotd */
		   {
			   for(col=0; col<n ;col++)
	           {
                  if(!pivot_flag[col])
			      {
				     abs_element=fabs(Ai[row][col]);
				     if(abs_element >= big)
				     {
					    big=abs_element;
					    irow=row;
					    icol=col;
				     }
			      }
		       }
	        }
       }
       pivot_flag[icol]++;
   
   /* swap rows to make this diagonal the bggest absolute pivot */
       if(irow!=icol)
       {
	      for(col=0;col <n;col++)
	      {
		     temp=Ai[irow][col];
		     Ai[irow][col]=Ai[icol][col];
		     Ai[icol][col]=temp;
	      }
       }

   /*  store what we swaped */
       swap_row[i]=irow;
       swap_col[i]=icol;

   /* Bad News if the pivot is zero	*/
       if(Ai[icol][icol]==0.0)  return 1;	   
		   

   /*  divide the row by the pivot*/
       pivot_inverse = 1.0/Ai[icol][icol];
       Ai[icol][icol]=1.0;
       for(col=0;col<n;col++)
	      Ai[icol][col]=Ai[icol][col]*pivot_inverse;

   /* Fix the other rows by substraction */
       for(row=0; row <n; row++)
	      if(row!=icol)
          {
	         temp=Ai[row][icol];
		     Ai[row][icol]=0.0;
		     for(col=0; col<n ; col++)
			    Ai[row][col] = Ai[row][col]-Ai[icol][col]*temp;
          }
   }
   for(swap=n-1;swap>=0;swap--)
   {
	   if(swap_row[swap] !=swap_col[swap])
	   {
		   for(row=0;row<n;row++)
		   {
			   temp=Ai[row][swap_row[swap]];
			   Ai[row][swap_row[swap]]=Ai[row][swap_col[swap]];
			   Ai[row][swap_col[swap]]=temp;
		   }
	   }
   }

   
    return 0;
}


int CPatternMatch::matrixInverse(float *a,int n,float *b)
{
	 int i,j,k,l,l1;
	 float c,d;
	 float SMALLEST=(float)0.00000001;
	 for (i=0;i<n;i++) {
		 for (j=0; j<n;j++)
			 *(b+i*n+j) = 0.;
		 *(b+i*n+i) = 1.0;
	 }
	 for (l=0;l<n;l++){
		 d = fabs((*(a+l*n+l)));
		 if( d<SMALLEST ){
			 l1 = l;
			 d = fabs((*(a+l1*n+l)));
			 while (d < SMALLEST && (++l1<n))
				 d = fabs((*(a+l1*n+l)));
			 if (l1>=n)
				 return -1;
			 for (j=0;j<n;j++){
				 *(a+l*n+j) += (*(a+l1*n+j));
				 *(b+l*n+j) += (*(b+l1*n+j));
			 }
		 }
		 c = 1.0/(*(a+l*n+l));
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
/*END:  Gray Pattern Matching >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/








