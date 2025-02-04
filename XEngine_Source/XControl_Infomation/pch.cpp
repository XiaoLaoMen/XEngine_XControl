﻿#include "pch.h"
#include "XControl_Info/XControl_Info.h"
/********************************************************************
//    Created:     2021/09/08  10:14:19
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Infomation\pch.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Infomation
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     后台管理服务导出函数实现
//    History:
*********************************************************************/
bool BackManage_IsErrorOccur = false;
XLONG BackManage_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CXControl_Info m_XInfo;
//////////////////////////////////////////////////////////////////////////
//                    导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG XInfo_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return BackManage_dwErrorCode;
}
/************************************************************************/
/*                    导出的系统信息获取函数                            */
/************************************************************************/
extern "C" bool XControl_Info_HardWare(XCHAR * ptszHWInfo, int* pInt_Len)
{
	return m_XInfo.XControl_Info_HardWare(ptszHWInfo, pInt_Len);
}
extern "C" bool XControl_Info_SoftWare(XCHAR * ptszSWInfo, int* pInt_Len)
{
	return m_XInfo.XControl_Info_SoftWare(ptszSWInfo, pInt_Len);
}