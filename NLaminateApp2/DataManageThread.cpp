// DataManageThread.cpp : implementation file
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DataManageThread.h"

// CDataManageThread

CDataManageThread::CDataManageThread()
{
	long i, nWidth, nHeight;

	nWidth = IMAGE_WIDTH/DEF_IMAGE_REDUCE_RATE;
	nHeight = IMAGE_HEIGHT/DEF_IMAGE_REDUCE_RATE;

	for(i=0; i<DEF_IMAGE_MAX_NUM; i++);
	    m_pRawImage[i] = new unsigned char[nWidth*nHeight];
}

CDataManageThread::~CDataManageThread()
{

}

void CDataManageThread::Close()
{
	long i;

	for(i=0; i<DEF_IMAGE_MAX_NUM; i++)
	    delete m_pRawImage[i];
}

