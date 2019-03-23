#pragma once


// CDataManageThread
class CDataManageThread 
{

public :
	LPBYTE         m_pRawImage[DEF_IMAGE_MAX_NUM];

public :
	CDataManageThread();
	virtual ~CDataManageThread();

	void     Close();


protected:

public:
//	afx_msg void OnClose();
};


