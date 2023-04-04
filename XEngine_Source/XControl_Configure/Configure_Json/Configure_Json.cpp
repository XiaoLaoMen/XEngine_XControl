#include "pch.h"
#include "Configure_Json.h"
/********************************************************************
//    Created:     2021/08/24  11:17:18
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\Configure_Json\Configure_Json.cpp
//    File Path:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\Configure_Json
//    File Base:   Configure_Json
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置文件读取类
//    History:
*********************************************************************/
CConfigure_Json::CConfigure_Json()
{

}
CConfigure_Json::~CConfigure_Json()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
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
XBOOL CConfigure_Json::Config_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_FileConfig)
{
	Config_IsErrorOccur = FALSE;

	if ((NULL == lpszConfigFile) || (NULL == pSt_FileConfig))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTROL_MODULE_CONFIG_JSON_PARAMENT;
		return FALSE;
	}
	JSONCPP_STRING st_JsonError;
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuilder;

	FILE* pSt_File = fopen(lpszConfigFile, "rb");
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTROL_MODULE_CONFIG_JSON_OPENFILE;
		return FALSE;
	}
	int nCount = 0;
	XCHAR tszMsgBuffer[4096];
	while (1)
	{
		int nRet = fread(tszMsgBuffer + nCount, 1, 2048, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
	fclose(pSt_File);

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nCount, &st_JsonRoot, &st_JsonError))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTROL_MODULE_CONFIG_JSON_PARSE;
		return FALSE;
	}

	strcpy(pSt_FileConfig->tszTaskUrl, st_JsonRoot["tszTaskUrl"].asCString());
	pSt_FileConfig->bAutoStart = st_JsonRoot["bAutoStart"].asInt();
	pSt_FileConfig->bHideWnd = st_JsonRoot["bHideWnd"].asInt();

	if (st_JsonRoot["ClientTime"].empty() || (1 != st_JsonRoot["ClientTime"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTROL_MODULE_CONFIG_JSON_TIME;
		return FALSE;
	}
	Json::Value st_JsonTime = st_JsonRoot["ClientTime"];
	pSt_FileConfig->st_Time.nHTTPThreadTime = st_JsonTime["nHTTPThreadTime"].asInt();

	if (st_JsonRoot["LogConfig"].empty() || (4 != st_JsonRoot["LogConfig"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTROL_MODULE_CONFIG_JSON_XLOG;
		return FALSE;
	}
	Json::Value st_JsonXLog = st_JsonRoot["LogConfig"];
	pSt_FileConfig->st_XLog.nMaxSize = st_JsonXLog["nMaxSize"].asInt();
	pSt_FileConfig->st_XLog.nMaxCount = st_JsonXLog["nMaxCount"].asInt();
	pSt_FileConfig->st_XLog.nLogLeave = st_JsonXLog["nLogLeave"].asInt();
	strcpy(pSt_FileConfig->st_XLog.tszLogControl, st_JsonXLog["tszLogControl"].asCString());

	if (st_JsonRoot["VersionList"].empty())
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTROL_MODULE_CONFIG_JSON_VERSION;
		return FALSE;
	}
	Json::Value st_JsonVersion = st_JsonRoot["VersionList"];

	pSt_FileConfig->st_Version.pStl_ListVer = new list<string>;
	for (unsigned int i = 0; i < st_JsonVersion.size(); i++)
	{
		pSt_FileConfig->st_Version.pStl_ListVer->push_back(st_JsonVersion[i].asCString());
	}
	return TRUE;
}