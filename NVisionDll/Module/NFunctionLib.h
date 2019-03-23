#pragma once


// CNFunctionLib

class CNFunctionLib : public CWnd
{
	DECLARE_DYNAMIC(CNFunctionLib)

public:
	CNFunctionLib();
	virtual ~CNFunctionLib();

	float    FindAngleProj(unsigned char *pImage, long x1, long y1, long x2, long y2, 
							    	  long nImageWidth, long nImageHeight, float dStartAng, float dEndAng);

//
protected:
	DECLARE_MESSAGE_MAP()
};


