#include "XContral_Hdr.h"

BOOL APPManage_CreateService(XENGINE_APPINFO *pSt_APPInfo)
{
	TCHAR tszCmdExe[1024];
	memset(tszCmdExe, '\0', sizeof(tszCmdExe));

#ifdef _WINDOWS
	_stprintf_s(tszCmdExe, _T("sc stop %s"), pSt_APPInfo->tszAPPName);
#else
	_stprintf_s(tszCmdExe, _T("systemctl restart %s"), pSt_APPInfo->tszAPPName);
#endif
	if (-1 == system(tszCmdExe))
	{
		return FALSE;
	}
#ifdef _WINDOWS
	memset(tszCmdExe, '\0', sizeof(tszCmdExe));
	_stprintf_s(tszCmdExe, _T("sc start %s"), pSt_APPInfo->tszAPPName);
	if (-1 == system(tszCmdExe))
	{
		return FALSE;
	}
#endif
	return TRUE;
}
BOOL APPManage_CreateProcess(XENGINE_APPINFO* pSt_APPInfo, DWORD* pdwProcessID)
{
	TCHAR tszCmdExe[1024];
	memset(tszCmdExe, '\0', sizeof(tszCmdExe));

	SystemApi_Process_Stop(pSt_APPInfo->tszAPPName);
	_stprintf_s(tszCmdExe, _T("%s%s"), pSt_APPInfo->tszAPPPath, pSt_APPInfo->tszAPPName);
	if (!SystemApi_Process_CreateProcess(pdwProcessID, tszCmdExe))
	{
		return FALSE;
	}
	return TRUE;
}

XHTHREAD APPManage_Thread_Process()
{
	while (bIsRun)
	{
		//��ѯ�������б�
		list<XENGINE_APPINFO>::iterator stl_ListIterator = st_APPConfig.pStl_ListApp->begin();
		for (; stl_ListIterator != st_APPConfig.pStl_ListApp->end(); stl_ListIterator++)
		{
			//�Ƿ�����
			if (!stl_ListIterator->bEnable)
			{
				continue;
			}
			//���Դ����ж�
			if (stl_ListIterator->nErrorTime > st_ServiceConfig.st_Time.nErrorTime)
			{
				stl_ListIterator->bEnable = FALSE;
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("��������%s�����ڳ���ָ������ʧ�ܴ�����%d����������⹦�ܱ��ر�..."), stl_ListIterator->tszAPPName, st_ServiceConfig.st_Time.nErrorTime);
				continue;
			}
			TCHAR tszCmdExe[1024];
			memset(tszCmdExe, '\0', sizeof(tszCmdExe));
			XENGINE_APPINFO st_APPInfo = *stl_ListIterator;

			if (stl_ListIterator->nReTime > 0)
			{
				//�����Զ�����
				__int64x nNowTime = time(NULL);
				if ((nNowTime - stl_ListIterator->nStartTime) > stl_ListIterator->nReTime)
				{
					if (stl_ListIterator->bService)
					{
						if (APPManage_CreateService(&st_APPInfo))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���񲻴���,��������%s �ɹ�..."), stl_ListIterator->tszAPPName);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ�з������%s ʧ�ܣ������룺%d..."), tszCmdExe, errno);
						}
					}
					else
					{
						DWORD dwProcessId = 0;
						if (APPManage_CreateProcess(&st_APPInfo, &dwProcessId))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("�������̣�%s �ɹ�������ID��%d..."), stl_ListIterator->tszAPPName, dwProcessId);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ��ɱ�����̣�%s ʧ�ܣ������룺%lX..."), stl_ListIterator->tszAPPName, SystemApi_GetLastError());
						}
					}
					stl_ListIterator->nStartTime = time(NULL);
				}
			}
			else if (stl_ListIterator->bAutoStart)
			{
				//�����Զ�����
				XENGINE_APPINFO st_APPInfo = *stl_ListIterator;
				//���̲����ڲ�����
				if (!SystemApi_Process_IsExist(st_APPInfo.tszAPPName))
				{
					if (stl_ListIterator->bService)
					{
						if (APPManage_CreateService(&st_APPInfo))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���񲻴���,��������%s �ɹ�..."), stl_ListIterator->tszAPPName);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ�з������%s ʧ�ܣ������룺%d..."), tszCmdExe, errno);
						}
					}
					else
					{
						DWORD dwProcessId = 0;
						if (APPManage_CreateProcess(&st_APPInfo, &dwProcessId))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���̲�����,�������̣�%s �ɹ�������ID��%d..."), stl_ListIterator->tszAPPName, dwProcessId);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ���Զ��������%s ʧ�ܣ������룺%lX..."), tszCmdExe, SystemApi_GetLastError());
						}
					}
				}
			}
			else
			{
				//���û���ã���ô��ʾ����һ�ξͲ��ù���
				if (!SystemApi_Process_IsExist(st_APPInfo.tszAPPName))
				{
					stl_ListIterator->bEnable = FALSE; //����Ϊ�Ѿ�ִ�У�����ִ�д�����
					if (stl_ListIterator->bService)
					{
						if (APPManage_CreateService(&st_APPInfo))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���񲿴���,��������һ�Σ�%s �ɹ�..."), stl_ListIterator->tszAPPName);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ�з����������%s ʧ�ܣ������룺%d..."), tszCmdExe, errno);
						}
					}
					else
					{
						DWORD dwProcessId = 0;
						if (APPManage_CreateProcess(&st_APPInfo, &dwProcessId))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���̲�����,��������һ�Σ�%s �ɹ�������ID��%d..."), stl_ListIterator->tszAPPName, dwProcessId);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ���������%s ʧ�ܣ������룺%lX..."), tszCmdExe, SystemApi_GetLastError());
						}
					}
				}
			}
		}
		//�����û����õ�ʱ��
		std::this_thread::sleep_for(std::chrono::seconds(st_ServiceConfig.st_Time.nCheckTime));
	}
	return 0;
}
