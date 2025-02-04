﻿#include "XControl_Hdr.h"

bool bIsRun = false;
bool bRecord = false;
XHANDLE xhVideo = NULL;
XHANDLE xhAudio = NULL;
XHANDLE xhStream = NULL;
XHANDLE xhLog = NULL;
XHANDLE xhSocket = NULL;
XHANDLE xhPacket = NULL;
XNETHANDLE xhClient = 0;
__int64u m_nTaskSerial = 0;

shared_ptr<std::thread> pSTDThread_Http = NULL;
XENGINE_SERVERCONFIG st_ServiceConfig;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, "服务器退出...");
		bIsRun = false;

		if (NULL != pSTDThread_Http)
		{
			pSTDThread_Http->join();
		}
		XClient_TCPSelect_StopEx(xhSocket);
		HelpComponents_Datas_Destory(xhPacket);
		HelpComponents_XLog_Destroy(xhLog);
		exit(0);
	}
}
int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	LPCXSTR lpszWndName = "XEngine_XControlApp";
#endif
	bIsRun = true;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVERCONFIG));

	if (!XControl_Parament(argc, argv))
	{
		printf("初始化参数失败,错误:%lX!\n", Config_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	st_XLogConfig.XLog_MaxBackupFile = st_ServiceConfig.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_ServiceConfig.st_XLog.nMaxSize;
	strcpy(st_XLogConfig.tszFileName, st_ServiceConfig.st_XLog.tszLogControl);
	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("初始化日志服务失败,无法继续!\n");
		goto NETSERVICE_APPEXIT;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_ServiceConfig.st_XLog.nLogLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，初始化日志系统成功");

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，初始化信号处理成功");

	xhPacket = HelpComponents_Datas_Init();
	if (NULL == xhPacket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，启动组包器失败,错误:%lX", Packets_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，启动组包器成功");

	xhSocket = XClient_TCPSelect_StartEx(XControl_TCPTask_Callback);
	if (NULL == xhSocket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，启动网络客户端失败,错误:%lX", XClient_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，启动网络客户端成功");

	if (st_ServiceConfig.bAutoStart)
	{
		if (!SystemApi_Process_AutoStart("XEngine", "XEngine_XControl"))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，注册软件开机启动失败!错误:%lX", SystemApi_GetLastError());
			goto NETSERVICE_APPEXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，注册软件开机启动成功");
	}
	if (st_ServiceConfig.bHideWnd)
	{
#ifdef _MSC_BUILD
		bool bIsFound = false;
		SetConsoleTitleA(lpszWndName);
		HWND hWnd = GetDesktopWindow();
		hWnd = GetWindow(hWnd, GW_CHILD);
		XCHAR tszTitle[MAX_PATH];
		while (NULL != hWnd)
		{
			memset(tszTitle, '\0', MAX_PATH);
			GetWindowTextA(hWnd, tszTitle, MAX_PATH);
			if (0 == strncmp(lpszWndName, tszTitle, strlen(lpszWndName)))
			{
				bIsFound = true;
				break;
			}
			hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
		}
		if (bIsFound)
		{
			ShowWindow(hWnd, SW_HIDE);
		}
#endif
	}
	//启用HTTP客户端
	pSTDThread_Http = make_shared<std::thread>(XControl_Thread_HttpTask);
	if (!pSTDThread_Http->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建HTTP任务线程失败");
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建HTTP任务线程成功");

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，所有服务已经启动完毕,程序运行中,XEngine版本:%s,服务器发行次数:%d,版本;%s", BaseLib_OperatorVer_XNumberStr(), st_ServiceConfig.st_Version.pStl_ListVer->size(), st_ServiceConfig.st_Version.pStl_ListVer->front().c_str());
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
NETSERVICE_APPEXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, "后台控制服务关闭，服务器退出...");
		bIsRun = false;

		if (NULL != pSTDThread_Http)
		{
			pSTDThread_Http->join();
		}
		XClient_TCPSelect_StopEx(xhSocket);
		HelpComponents_Datas_Destory(xhPacket);
		HelpComponents_XLog_Destroy(xhLog);
	}

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
