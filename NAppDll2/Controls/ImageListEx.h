//////////////////////////////////////////////////////////////////////
// ImageListEx.h
//////////////////////////////////////////////////////////////////////
#ifndef _IMAGELISTEX_CLASS_
#define _IMAGELISTEX_CLASS_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
class CImageListEx : public CObject  
{
public:
	CImageListEx()
	{
		m_nImageListNum = 0;
		m_pImageList    = NULL;
	}
	virtual ~CImageListEx()
	{
		int i=0;
		if(m_pImageList!=NULL)
		{
			for(i=0; i<m_nImageListNum; i++) m_pImageList[i].DeleteImageList();
			delete [] m_pImageList;
			m_pImageList = NULL;
		}
	}

public:
	int		   m_nImageListNum;
	CImageList *m_pImageList;

public:
	void Create(int  nImageListNum, UINT *uiImageListID, UINT *uiDivNum)
	{
		int     i=0;
		CBitmap bmTmp;
		BITMAP  stBmp;
		m_nImageListNum = nImageListNum;
		m_pImageList    = new CImageList[m_nImageListNum];
		for(i=0; i<m_nImageListNum; i++)
		{
			bmTmp.LoadBitmap(uiImageListID[i]);
			bmTmp.GetBitmap(&stBmp);
			bmTmp.DeleteObject();
			m_pImageList[i].Create(uiImageListID[i],
				                   stBmp.bmWidth/uiDivNum[i],
								   1,
								   RGB(172, 172, 172));
		}
	}
	CImageList* GetImageList(int iIndex)
	{
		return &m_pImageList[iIndex];
	}
};
//////////////////////////////////////////////////////////////////////
#endif

