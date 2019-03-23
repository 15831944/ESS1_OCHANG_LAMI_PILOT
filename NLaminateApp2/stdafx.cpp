
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// NLaminateApp.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

LOG_LIST       g_LOG;
REALTIME_DATA  g_DataRT;
LPBYTE  g_pImage[DEF_QUEUE_MAX_NUM];
LPBYTE  g_pBadImage[VIEW_MAX_NUM][DEF_QUEUE_MAX_NUM];
BOOL    g_PortCOM[10];


