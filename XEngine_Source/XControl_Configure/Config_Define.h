#pragma once
/********************************************************************
//    Created:     2021/09/08  10:07:10
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Configure\Config_Define.h
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Configure
//    File Base:   Config_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置模块导出
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct tag_XEngine_ServerConfig
{
	CHAR tszTaskUrl[MAX_PATH];
	BOOL bAutoStart;
	BOOL bHideWnd;
	struct
	{
		int nHTTPThreadTime;
	}st_Time;
	struct
	{
		CHAR tszLogControl[MAX_PATH];
		int nMaxSize;
		int nMaxCount;
		int nLogLeave;
	}st_XLog;
	struct
	{
		list<string>* pStl_ListVer;
	}st_Version;
}XENGINE_SERVERCONFIG;
//////////////////////////////////////////////////////////////////////////
//                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        配置读取导出                                  */
/************************************************************************/
/********************************************************************
函数名称：Config_Json_File
函数功能：JSON基础配置读取
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件地址
 参数.二：pSt_FileConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Config_Json_File(LPCSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_FileConfig);