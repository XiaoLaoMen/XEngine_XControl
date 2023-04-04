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
函数功能：解析协议主数据
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
XBOOL CProtocol_Parse::Protocol_Parse_JsonRoot(LPCXSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO* pSt_ProtocolInfo)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	pSt_ProtocolInfo->nOPCode = st_JsonRoot["unOperatorCode"].asUInt();
	pSt_ProtocolInfo->nTaskSerial = st_JsonRoot["nTaskSerial"].asUInt64();
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_Download
函数功能：下载协议解析器
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
 参数.三：ptszFileUrl
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出下载地址
 参数.四：ptszFileUrl
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出保存地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_Download(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszFileUrl, XCHAR* ptszSaveUrl)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	strcpy(ptszFileUrl, st_JsonRoot["DownloadUrl"].asCString());
	strcpy(ptszSaveUrl, st_JsonRoot["SaveUrl"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_Delete
函数功能：删除文件协议解析器
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
 参数.三：ptszDelete
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出删除的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_Delete(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszDelete)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	strcpy(ptszDelete, st_JsonRoot["DeleteFile"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_UPFile
函数功能：文件上传协议解析
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
 参数.三：ptszUPFile
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件的地址
 参数.四：ptszUPUrl
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出上传到的位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_UPFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszUPFile, XCHAR* ptszUPUrl)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	strcpy(ptszUPFile, st_JsonRoot["UPLoadFile"].asCString());
	strcpy(ptszUPUrl, st_JsonRoot["UPLoadUrl"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_ListFile
函数功能：枚举文件
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
 参数.三：ptszFindPath
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件的地址
 参数.四：ptszPostUrl
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出上传到的位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_ListFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszFindPath, XCHAR* ptszPostUrl)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	strcpy(ptszFindPath, st_JsonRoot["FilePath"].asCString());
	strcpy(ptszPostUrl, st_JsonRoot["PostUrl"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_Exec
函数功能：执行程序协议解析
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
 参数.三：ptszExecFile
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出要执行的进程地址
 参数.四：pInt_ExecShow
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出执行的方法
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_Exec(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszExecFile, int* pInt_ExecShow)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	*pInt_ExecShow = st_JsonRoot["ExecShow"].asInt();
	strcpy(ptszExecFile, st_JsonRoot["ExecFile"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_Message
函数功能：弹出消息协议
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
 参数.三：ptszMessageBox
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出弹出的消息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_Message(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMessageBox)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	strcpy(ptszMessageBox, st_JsonRoot["MessageBox"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_Message
函数功能：停止进程协议
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
 参数.三：pInt_ProcessID
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出结束的进程ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_Stop(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG* pInt_ProcessID)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	*pInt_ProcessID = st_JsonRoot["ProcessID"].asInt();

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_Shutdown
函数功能：关机协议
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
 参数.三：pInt_SDType
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：关闭计算机的方式
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_Shutdown(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG* pInt_SDType)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	*pInt_SDType = st_JsonRoot["ShutDownType"].asInt();

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_System
函数功能：执行命令
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
 参数.三：ptszExecCmd
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出要执行的命令
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_System(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszExecCmd)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	strcpy(ptszExecCmd, st_JsonRoot["tszExecCmd"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_Report
函数功能：上报信息协议
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
 参数.三：ptszIPAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出上报的地址
 参数.三：pInt_Type
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出上报的类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_Report(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszIPAddr, int* pInt_Type)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	*pInt_Type = st_JsonRoot["nType"].asInt();
	strcpy(ptszIPAddr, st_JsonRoot["tszIPAddr"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_EnumDevice
函数功能：枚举音视频设备协议
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
 参数.三：ptszIPAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出上报的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_EnumDevice(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszIPAddr)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	strcpy(ptszIPAddr, st_JsonRoot["tszIPAddr"].asCString());

	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_Serial
函数功能：设置序列号协议
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
 参数.三：pInt_Serial
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出设置的序列号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Parse::Protocol_Parse_Serial(LPCXSTR lpszMsgBuffer, int nMsgLen, __int64u* pInt_Serial)
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

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
		return FALSE;
	}
	*pInt_Serial = st_JsonRoot["tszIPAddr"].asUInt64();

	return TRUE;
}