#include "XContral_Hdr.h"

XHTHREAD APPManage_Thread_Process()
{
	while (bIsRun)
	{
		//��ѯ�������б�
		list<XENGINE_APPINFO>::iterator stl_ListIterator = st_APPConfig.pStl_ListApp->begin();
		for (; stl_ListIterator != st_APPConfig.pStl_ListApp->end(); stl_ListIterator++)
		{
			//�Ƿ�����
			if (stl_ListIterator->bEnable)
			{
				if (stl_ListIterator->nErrorTime > st_ServiceConfig.st_Time.nErrorTime)
				{
					stl_ListIterator->bEnable = FALSE;
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("��������%s�����ڳ���ָ������ʧ�ܴ�����%d����������⹦�ܱ��ر�..."), stl_ListIterator->tszAPPName, st_ServiceConfig.st_Time.nErrorTime);
					continue;
				}
				TCHAR tszCmdExe[1024];
				memset(tszCmdExe, '\0', sizeof(tszCmdExe));
				//�ж��Ƿ������˽�����������
				if (stl_ListIterator->nReTime > 0)
				{
					//��������������������ж�ʱ��
					__int64x nNowTime = time(NULL);
					if ((nNowTime - stl_ListIterator->nStartTime) > stl_ListIterator->nReTime)
					{
						if (stl_ListIterator->bService)
						{
#ifdef _WINDOWS
							_stprintf_s(tszCmdExe, _T("sc stop %s"), stl_ListIterator->tszAPPName);
#else
							_stprintf_s(tszCmdExe, _T("systemctl restart %s"), stl_ListIterator->tszAPPName);
#endif
							if (-1 == system(tszCmdExe))
							{
								stl_ListIterator->nErrorTime++;
								XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("������������%s ʧ�ܣ������룺%d..."), tszCmdExe, errno);
							}
							else
							{
#ifdef _WINDOWS
								memset(tszCmdExe, '\0', sizeof(tszCmdExe));
								_stprintf_s(tszCmdExe, _T("sc start %s"), stl_ListIterator->tszAPPName);
								if (-1 == system(tszCmdExe))
								{
									stl_ListIterator->nErrorTime++;
									XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("��������%s ʧ�ܣ������룺%d..."), tszCmdExe, GetLastError());
								}
								else
								{
									XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��������%s �ɹ�..."), stl_ListIterator->tszAPPName);
								}
#else
								XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��������%s �ɹ�..."), stl_ListIterator->tszAPPName);
#endif
							}
						}
						else
						{
							if (SystemApi_Process_Stop(stl_ListIterator->tszAPPName))
							{
								DWORD dwProcessId = 0;
								_stprintf_s(tszCmdExe, _T("%s%s"), stl_ListIterator->tszAPPPath, stl_ListIterator->tszAPPName);
								if (!SystemApi_Process_CreateProcess(&dwProcessId, tszCmdExe))
								{
									stl_ListIterator->nErrorTime++;
									XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ�������������%s ʧ�ܣ������룺%lX..."), tszCmdExe, SystemApi_GetLastError());
								}
								else
								{
									XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("�������̣�%s �ɹ�������ID��%d..."), stl_ListIterator->tszAPPName, dwProcessId);
								}
							}
							else
							{
								stl_ListIterator->nErrorTime++;
								XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ��ɱ�����̣�%s ʧ�ܣ������룺%lX..."), stl_ListIterator->tszAPPName, SystemApi_GetLastError());
							}
						}
						stl_ListIterator->nStartTime = time(NULL);
						continue;      //������������Ľ��̣���ô������Ҫ�ڼ�������Ƿ���Ҫ������
					}
				}
				//����0�����ݣ���ʾ���ڣ����߲�����
				if (SystemApi_Process_IsExist(stl_ListIterator->tszAPPName))
				{
					continue;      //���ھͼ�����һ�����
				}
				//����������Զ���������ô����ÿ�ζ���������
				if (stl_ListIterator->bAutoStart)
				{
					//�ж��Ƿ��Ƿ�������ģʽ
					if (stl_ListIterator->bService)
					{
#ifdef _WINDOWS
						_stprintf_s(tszCmdExe, _T("sc stop %s"), stl_ListIterator->tszAPPName);
#else
						_stprintf_s(tszCmdExe, _T("systemctl restart %s"), stl_ListIterator->tszAPPName);
#endif
						if (-1 == system(tszCmdExe))
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ�з������%s ʧ�ܣ������룺%d..."), tszCmdExe, errno);
							continue;
						}
#ifdef _WINDOWS
						memset(tszCmdExe, '\0', sizeof(tszCmdExe));
						_stprintf_s(tszCmdExe, _T("sc start %s"), stl_ListIterator->tszAPPName);
						if (-1 == system(tszCmdExe))
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("��������%s ʧ�ܣ������룺%d..."), tszCmdExe, GetLastError());
							continue;
						}
#endif
						XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���񲿴���,��������%s �ɹ�..."), stl_ListIterator->tszAPPName);
					}
					else
					{
						DWORD dwProcessId = 0;
						_stprintf_s(tszCmdExe, _T("%s%s"), stl_ListIterator->tszAPPPath, stl_ListIterator->tszAPPName);
						if (!SystemApi_Process_CreateProcess(&dwProcessId, tszCmdExe))
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ���Զ��������%s ʧ�ܣ������룺%lX..."), tszCmdExe, SystemApi_GetLastError());
							continue;
						}
						XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���̲�����,�������̣�%s �ɹ�������ID��%d..."), stl_ListIterator->tszAPPName, dwProcessId);
					}
				}
				else
				{
					//���û���ã���ô��ʾ����һ�ξͲ��ù���
					if (!stl_ListIterator->bStart)
					{
						stl_ListIterator->bStart = TRUE; //����Ϊ�Ѿ�ִ�У�����ִ�д�����
						if (stl_ListIterator->bService)
						{
#ifdef _WINDOWS
							_stprintf_s(tszCmdExe, _T("sc stop %s"), stl_ListIterator->tszAPPName);
#else
							_stprintf_s(tszCmdExe, _T("systemctl restart %s"), stl_ListIterator->tszAPPName);
#endif
							if (-1 == system(tszCmdExe))
							{
								stl_ListIterator->nErrorTime++;
								XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ�з����������%s ʧ�ܣ������룺%d..."), tszCmdExe, errno);
								continue;
							}
#ifdef _WINDOWS
							memset(tszCmdExe, '\0', sizeof(tszCmdExe));
							_stprintf_s(tszCmdExe, _T("sc start %s"), stl_ListIterator->tszAPPName);
							if (-1 == system(tszCmdExe))
							{
								stl_ListIterator->nErrorTime++;
								XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("��������%s ʧ�ܣ������룺%d..."), tszCmdExe, GetLastError());
								continue;
							}
#endif
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���񲿴���,��������һ�Σ�%s �ɹ�..."), stl_ListIterator->tszAPPName);
						}
						else
						{
							DWORD dwProcessId = 0;
							_stprintf_s(tszCmdExe, _T("%s%s"), stl_ListIterator->tszAPPPath, stl_ListIterator->tszAPPName);
							if (!SystemApi_Process_CreateProcess(&dwProcessId, tszCmdExe))
							{
								stl_ListIterator->nErrorTime++;
								XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("ִ���������%s ʧ�ܣ������룺%lX..."), tszCmdExe, SystemApi_GetLastError());
								continue;
							}
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("��鵽���̲�����,��������һ�Σ�%s �ɹ�������ID��%d..."), stl_ListIterator->tszAPPName, dwProcessId);
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
