#include "XControl_Hdr.h"

void CALLBACK XControl_TCPTask_Callback(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_NETCLIENT_TCPEVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nLen, XPVOID lParam)
{
	XCHAR tszClient[128];
	memset(tszClient, '\0', 128);

	_xstprintf(tszClient, _X("%lld"), xhClient);

	if (!HelpComponents_Datas_PostEx(xhPacket, tszClient, lpszMsgBuffer, nLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP客户端,投递任务失败,错误码:%lX", Packets_GetLastError());
		return;
	}

	while (1)
	{
		int nMsgLen = 0;
		XCHAR* ptszMsgBuffer;
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		if (!HelpComponents_Datas_GetMemoryEx(xhPacket, tszClient, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			break;
		}
		XControl_TCPTask_ProtocolParse(&st_ProtocolHdr, ptszMsgBuffer, nMsgLen);
	}
}
//////////////////////////////////////////////////////////////////////////
bool XControl_TCPTask_ProtocolParse(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	XCHAR tszSDBuffer[4096];
	
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_BACK != pSt_ProtocolHdr->unOperatorType)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:协议错误,非后台协议");
		return false;
	}
	//执行任务
	switch (pSt_ProtocolHdr->unOperatorCode)
	{
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DOWNFILE:
	{
		XCHAR tszFileUrl[MAX_PATH];
		XCHAR tszSaveUrl[MAX_PATH];

		memset(tszFileUrl, '\0', MAX_PATH);
		memset(tszSaveUrl, '\0', MAX_PATH);
		Protocol_Parse_Download(lpszMsgBuffer, nMsgLen, tszFileUrl, tszSaveUrl);

		XHANDLE xhTask = APIClient_File_Create(tszFileUrl, tszSaveUrl);
		if (NULL == xhTask)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:下载任务处理失败,错误码:%lX", APIClient_GetLastError());
			return false;
		}
		while (true)
		{
			NETHELP_FILEINFO st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(NETHELP_FILEINFO));

			if (APIClient_File_Query(xhTask, &st_TaskInfo))
			{
				if ((ENUM_NETHELP_APICLIENT_FILE_STATUS_INIT != st_TaskInfo.en_DownStatus) || (ENUM_NETHELP_APICLIENT_FILE_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus))
				{
					break;
				}
			}
		}
		APIClient_File_Delete(xhTask);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:下载任务处理成功,下载地址:%s,保存地址:%s"), tszFileUrl, tszSaveUrl);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEFILE:
		XCHAR tszDelFile[MAX_PATH];
		memset(tszDelFile, '\0', MAX_PATH);

		Protocol_Parse_Delete(lpszMsgBuffer, nMsgLen, tszDelFile);
		if (-1 == remove(tszDelFile))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:删除文件任务处理失败,错误码:%d", errno);
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:删除文件处理成功,删除的文件:%s"), tszDelFile);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEDIR:
		XCHAR tszDelDir[MAX_PATH];
		memset(tszDelDir, '\0', MAX_PATH);

		Protocol_Parse_Delete(lpszMsgBuffer, nMsgLen, tszDelDir);
		if (!SystemApi_File_DeleteMutilFolder(tszDelDir))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:删除文件夹任务处理失败,错误码:%lX", SystemApi_GetLastError());
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:删除文件夹处理成功,删除的文件夹:%s"), tszDelDir);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_UPFILE:
	{
		XCHAR tszUPFile[MAX_PATH];
		XCHAR tszUPUrl[MAX_PATH];

		memset(tszUPFile, '\0', MAX_PATH);
		memset(tszUPUrl, '\0', MAX_PATH);

		Protocol_Parse_UPFile(lpszMsgBuffer, nMsgLen, tszUPFile, tszUPUrl);
		XHANDLE xhTask = APIClient_File_Create(tszUPFile, tszUPUrl, false);
		if (NULL == xhTask)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:FTP上传任务处理失败,错误码:%lX", APIClient_GetLastError());
			return false;
		}
		while (true)
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
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:上传文件处理成功,上传的文件:%s,上传的地址:%s"), tszUPFile, tszUPUrl);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_GETLIST:
	{
		int nListCount = 0;
		XCHAR** ppszFileList;
		XCHAR tszFindPath[MAX_PATH];

		memset(tszFindPath, '\0', MAX_PATH);

		Protocol_Parse_ListFile(lpszMsgBuffer, nMsgLen, tszFindPath);
		if (!SystemApi_File_EnumFile(tszFindPath, &ppszFileList, &nListCount))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:请求文件列表失败,错误码:%lX", SystemApi_GetLastError());
			return false;
		}
		Protocol_Packet_ListFile(tszSDBuffer, &nSDLen, &ppszFileList, nListCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppszFileList, nListCount);

		pSt_ProtocolHdr->unPacketSize = nSDLen;
		XClient_TCPSelect_SendEx(xhSocket, xhClient, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		XClient_TCPSelect_SendEx(xhSocket, xhClient, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:获取文件列表成功,回复大小:"), nSDLen);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_EXEC:
	{
		XLONG dwProcessID = 0;
		int nExeType = 0;
		XCHAR tszExecFile[MAX_PATH];

		memset(tszExecFile, '\0', MAX_PATH);

		Protocol_Parse_Exec(lpszMsgBuffer, nMsgLen, tszExecFile, &nExeType);
		if (!SystemApi_Process_CreateProcess(&dwProcessID, tszExecFile, NULL, nExeType))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:请求创建进程:%s 失败,错误码:%lX", tszExecFile, SystemApi_GetLastError());
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "TCP任务:请求创建进程成功,进程:%s", tszExecFile);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_POPMESSAGE:
	{
		XCHAR tszMessageBox[MAX_PATH];
		memset(tszMessageBox, '\0', MAX_PATH);

		Protocol_Parse_Message(lpszMsgBuffer, nMsgLen, tszMessageBox);
#ifdef _MSC_BUILD
		MessageBoxA(NULL, tszMessageBox, "提示", MB_OK);
#endif
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "TCP任务:请求弹出消息:%s", tszMessageBox);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_STOPPROCESS:
	{
		XLONG dwProcessID = 0;

		Protocol_Parse_Stop(lpszMsgBuffer, nMsgLen, &dwProcessID);
		if (!SystemApi_Process_Stop(NULL, dwProcessID))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:请求停止进程失败,错误码:%lX", SystemApi_GetLastError());
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:请求停止进程成功,进程ID:%ld"), dwProcessID);
	}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SHUTDOWN:
	{
		XLONG dwType = 0;

		Protocol_Parse_Shutdown(lpszMsgBuffer, nMsgLen, &dwType);
		if (!SystemApi_System_Shutdown(dwType))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:请求关机失败,错误码:%lX", SystemApi_GetLastError());
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:请求关闭系统成功,关闭方式:%ld"), dwType);
	}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ECMD:
		XCHAR tszExecCmd[MAX_PATH];
		memset(tszExecCmd, '\0', MAX_PATH);

		Protocol_Parse_System(lpszMsgBuffer, nMsgLen, tszExecCmd);
		if (-1 == system(tszExecCmd))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:请求执行命令失败,错误码:%lX", SystemApi_GetLastError());
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:请求执行命令成功,执行的命令:%s"), tszExecCmd);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_REPORT:
	{
		int nType = 0;
		Protocol_Parse_Report(lpszMsgBuffer, nMsgLen, &nType);
		if (0 == nType)
		{
			XControl_Info_HardWare(tszSDBuffer, &nSDLen);

			pSt_ProtocolHdr->unPacketSize = nSDLen;
			XClient_TCPSelect_SendEx(xhSocket, xhClient, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
			XClient_TCPSelect_SendEx(xhSocket, xhClient, tszSDBuffer, nSDLen);
		}
		else
		{
			XControl_Info_SoftWare(tszSDBuffer, &nSDLen);

			pSt_ProtocolHdr->unPacketSize = nSDLen;
			XClient_TCPSelect_SendEx(xhSocket, xhClient, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
			XClient_TCPSelect_SendEx(xhSocket, xhClient, tszSDBuffer, nSDLen);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:请求上报信息成功"));
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ENUMDEVICE:
	{
		int nACount = 0;
		int nVCount = 0;
		AVHELP_DEVICEINFO** ppSt_AudioList;
		AVHELP_DEVICEINFO** ppSt_VideoList;

		AVHelp_Device_EnumDevice(&ppSt_AudioList, &ppSt_VideoList, &nACount, &nVCount);
		Protocol_Packet_EnumDevice(tszSDBuffer, &nSDLen, &ppSt_AudioList, &ppSt_VideoList, nACount, nVCount);
		BaseLib_OperatorMemory_Free((void***)&ppSt_AudioList, nACount);
		BaseLib_OperatorMemory_Free((void***)&ppSt_VideoList, nVCount);

		pSt_ProtocolHdr->unPacketSize = nSDLen;
		XClient_TCPSelect_SendEx(xhSocket, xhClient, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		XClient_TCPSelect_SendEx(xhSocket, xhClient, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:请求枚举音视频设备成功,音频设备个数:%d,视频设备个数:%d"), nACount, nVCount);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_RECORD:
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SERIAL:
		Protocol_Parse_Serial(lpszMsgBuffer, nMsgLen, &m_nTaskSerial);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP任务:请求设置序列号成功,序列号:%lld,"), m_nTaskSerial);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_USER:
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_NOTHINGTODO:
		break;
	default:
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "TCP任务:请求的操作码不支持,操作码:%d",pSt_ProtocolHdr->unOperatorCode);
		return false;
	}
	return true;
}