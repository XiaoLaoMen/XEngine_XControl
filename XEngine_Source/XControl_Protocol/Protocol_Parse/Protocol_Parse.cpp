#include "pch.h"
#include "Protocol_Parse.h"
/********************************************************************
//    Created:     2021/09/08  10:35:21
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Parse\Protocol_Parse.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Parse
//    File Base:   Protocol_Parse
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议模块解析类
//    History:
*********************************************************************/
CProtocol_Parse::CProtocol_Parse()
{
}
CProtocol_Parse::~CProtocol_Parse()
{
}
///////////////////////////////////////////////////////////////////////////////
//                      公有函数
///////////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Protocol_Parse_JsonRoot
函数功能：翻译解析
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的内容
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入解析大小
 参数.三：pSt_ProtocolInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析好的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_JsonRoot(LPCTSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO* pSt_ProtocolInfo)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (0 == nMsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	pSt_ProtocolInfo->nOPCode = st_JsonRoot["unOperatorCode"].asUInt();
	pSt_ProtocolInfo->nTaskSerial = st_JsonRoot["nTaskSerial"].asUInt64();
	
	if (!st_JsonRoot["tszSourceStr"].isNull())
	{
		_tcscpy(pSt_ProtocolInfo->tszSourceStr, st_JsonRoot["tszSourceStr"].asCString());
	}
	if (!st_JsonRoot["tszDestStr"].isNull())
	{
		_tcscpy(pSt_ProtocolInfo->tszDestStr, st_JsonRoot["tszDestStr"].asCString());
	}
	return TRUE;
}