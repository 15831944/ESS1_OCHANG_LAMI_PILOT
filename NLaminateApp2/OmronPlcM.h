#pragma once

#if _OMRON
#include "CIPCoreAPI.h"
#include "CIPCorePortAPI.h"
#endif

// COmronPlcM
class COmronPlcM : public CWnd
{
	DECLARE_DYNAMIC(COmronPlcM)

public:
	COmronPlcM();
	virtual ~COmronPlcM();

	BOOL           OpenPLC();
	void           ClosePLC();

	BOOL           SendMessageToPLC(char *strTagName, DWORD nMsgID, int nSize, int *szCommBuff);
	BOOL           ReceiveMessageFromPLC(char *strTagName, DWORD nMsgID, char *szCommBuff);
	int            ReceiveMessageFromPLC_Int(char *strTagName, DWORD nMsgID, char *szCommBuff);


public:
#if _OMRON
	CIPAppHandle        m_hAppIP;
	CIPAppExplicitParam m_hConnectIP;
#endif


protected:
	DECLARE_MESSAGE_MAP()
};


