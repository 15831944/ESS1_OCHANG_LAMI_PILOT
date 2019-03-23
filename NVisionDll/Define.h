#if !defined(_DEFINE_H)
#define _DEFINE_H

// Image Info.
#define _MIL_SETUP                   0
#define IMAGE_BAND                   1
#define IMAGE_DEPTH                  8

#define WHITE_LEVEL                255
#define BLACK_LEVEL                  0
// Image Info.

#define PI                3.141592653589793
#define RAD                 (PI / 180.0)
#define DEG                 (180.0 / PI)

typedef struct 
{
    float   x;
	float   y;
} POINT2;

typedef struct 
{
	long    m_nRow;
	long    m_nCol;
	double *m_pMat;
} DEF_MAT;

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////   배터리 비전 검사의 관련 구조체 정의  /////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#define  MAX_CHANNEL_NUM             2

#endif 