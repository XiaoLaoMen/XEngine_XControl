#include "XControl_Hdr.h"

XHTHREAD XControl_Thread_HttpTask()
{
	XCHAR tszTaskUrl[MAX_PATH];
	memset(tszTaskUrl, '\0', MAX_PATH);

	while (bIsRun)
	{
		int nBLen = 0;
		int nCode = 0;
		XCHAR* ptszMsgBody = NULL;

		sprintf(tszTaskUrl, st_ServiceConfig.tszTaskUrl, m_nTaskSerial);

		if (APIClient_Http_Request("GET", tszTaskUrl, NULL, &nCode, &ptszMsgBody, &nBLen))
		{
			if (200 == nCode)
			{
				XControl_Task_ProtocolParse(ptszMsgBody, nBLen);
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBody);
		}
		std::this_thread::sleep_for(std::chrono::seconds(st_ServiceConfig.st_Time.nHTTPThreadTime));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
bool XControl_Task_ProtocolParse(LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	XCHAR tszSDBuffer[4096];
	XCONTROL_PROTOCOLINFO st_ProtocolInfo;
	
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_ProtocolInfo, '\0', sizeof(XCONTROL_PROTOCOLINFO));

	if (!Protocol_Parse_JsonRoot(lpszMsgBuffer, nMsgLen, &st_ProtocolInfo))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:JSON解析错误");
		return false;
	}
	//获得任务序列号
	if (st_ProtocolInfo.nTaskSerial != m_nTaskSerial)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:任务序列号不正确");
		return false;
	}
	m_nTaskSerial++;
	//执行任务
	switch (st_ProtocolInfo.nOPCode)
	{
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_CONNECT:
	{
		int nPort = 0;
		XCHAR tszIPAddr[128];

		memset(tszIPAddr, '\0', sizeof(tszIPAddr));
		Protocol_Parse_Connect(lpszMsgBuffer, nMsgLen, tszIPAddr, &nPort);
		XClient_TCPSelect_Create(&hSocket, tszIPAddr, nPort);
	}
	break;
	default:
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求的操作码不支持,操作码:%d", st_ProtocolInfo.nOPCode);
		return false;
	}
	return true;
}