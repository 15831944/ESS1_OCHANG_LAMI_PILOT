
// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// NLaminateApp.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"

LOG_LIST       g_LOG;
REALTIME_DATA  g_DataRT;
LPBYTE  g_pImage[DEF_QUEUE_MAX_NUM];
LPBYTE  g_pBadImage[VIEW_MAX_NUM][DEF_QUEUE_MAX_NUM];
BOOL    g_PortCOM[10];


