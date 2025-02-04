﻿#include "pch.h"
#include "Configure_Json/Configure_Json.h"
/********************************************************************
//    Created:     2021/09/08  10:08:11
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Configure\pch.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Configure
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Config_IsErrorOccur = false;
XLONG Config_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CConfigure_Json m_ConfigJson;
//////////////////////////////////////////////////////////////////////////
//                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Config_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Config_dwErrorCode;
}
/************************************************************************/
/*                        配置读取导出                                  */
/************************************************************************/
extern "C" bool Config_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVERCONFIG * pSt_FileConfig)
{
	return m_ConfigJson.Config_Json_File(lpszConfigFile, pSt_FileConfig);
}