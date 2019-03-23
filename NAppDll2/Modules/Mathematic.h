#if !defined(AFX_MATHEMATIC_H__7081DF50_7F0E_43C4_826B_8C64CCA0DDBA__INCLUDED_)
#define AFX_MATHEMATIC_H__7081DF50_7F0E_43C4_826B_8C64CCA0DDBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mathematic.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMathematic window
class CMathematic : public CStatic
{
public:
	CMathematic();

// Attributes
public:
	// 두점 사이의 거리 구하는 함수 
    double GetDistance2D(POINT2 p1, POINT2 p2);
    double GetDistance2D(CPoint p1, CPoint p2);
    float  GetDistance2D(float x1, float y1, float x2, float y2);

	// 두점 사이의 수평 각도를 구함 
	float  GetAngle(POINT2 p1, POINT2 p2);
	float  GetPerpendicularM(float dA);

	// 한점과 원이 직교하는 점을 구함 
    // p1 : 점
    // p2 : 원의 중심 
    // radius : 반지름 
    // cp : 직교점 
    void   GetDotCircleCrossPoint(POINT2 p1, POINT2 p2, long radius, POINT2 *cp);

	// 3점을 이용하여 원의 중심을 구하고 반지름을 구하는 함수임 
	long   GetCircleEquation(POINT2 *p1, POINT2 *p2, POINT2 *p3, POINT2 *cp, double *radius) ;   
	long   GetEllipseEquation(int N, double *x, double *y, int RemoveN, double ErrorLimit, 
				double *Cx, double *Cy, double *al, double *bl, double *angle);

    // 한점과 직선과의 교점 구하는 함수 
    // p1 : 점 
    // p2, p3 : 직선의 두점 
    // cp : 직교점 
    void   GetCrossPointDotLine(POINT2 ptDot, POINT2 ptLine1, POINT2 ptLine2, POINT2 *ptPos);
    void   GetCrossPointLineEquWithLine(float a, float b, POINT2 ptLine1, POINT2 ptLine2, POINT2 *ptPos);
    void   GetCrossPointLineEquWithDot(float a, float b, POINT2 ptDot, POINT2 *ptPos);
    void   GetCrossPointDotWithAngle(float dAngle, POINT2 ptDot1, POINT2 ptDot2, POINT2 *ptPos);
    void   GetCrossPointLineEqu(float a1, float b1, float a2, float b2, float *cx, float *cy);
    float  GetAngleWith2Line(POINT2 ptDot1, POINT2 ptDot2, POINT2 ptDot3, POINT2 ptDot4);
    float  GetDistanceDot2Line(POINT2 ptDot, POINT2 ptLine1, POINT2 ptLine2);

    // 두 직선의 교차점과 이루는 각을 구하는 함수 
    // p1 ,p2 : 첫번째 직선
    // p3, p4 : 두번째 직선 
    // cp : 교차하는 점 
    // 두 직선이 이루는 각 
    void   GetCrossPoint2Line(POINT2 p1, POINT2 p2, POINT2 p3, POINT2 p4, POINT2 *cp);

	// 직선과 원이 만나는 두 교차점을 구하는 함수
    // p1, p2 : 직선
    // p3 : 원의 중심
    // radius : 원의 반지름 
    // cp1, cp2 : 직교하는 점 
    void   GetLineCircleCrossPoint(POINT2 p1, POINT2 p2, POINT2 p3, long radius, POINT2 *cp1, POINT2 *cp2);

    // 두 원의 교점을 구하는 함수 
    // p1, radius1 : 원의 중심과 반경 
    // p2, radius2 : 원의 중심과 반경 
    // cp1, cp2 : 교차하는 점 
    void   GetCircleCircleCrossPoint(POINT2 p1, float radius1, POINT2 p2, float radius2, POINT2 *cp1, POINT2 *cp2);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMathematic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMathematic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMathematic)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATHEMATIC_H__7081DF50_7F0E_43C4_826B_8C64CCA0DDBA__INCLUDED_)
