#pragma once
/********************************************************************
//    Created:     2021/09/07  14:46:23
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Protocol\Protocol_Define.h
//    File Path:   D:\XEngine_XContral\XEngine_Source\XContral_Protocol
//    File Base:   Protocol_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出的定义
//    History:
*********************************************************************/
typedef struct  
{
	TCHAR tszSourceStr[MAX_PATH];
	TCHAR tszDestStr[MAX_PATH];

	__int64u nTaskSerial;
	int nOPCode;
}XCONTROL_PROTOCOLINFO;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Protocol_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        封装类函数                                    */
/************************************************************************/
/********************************************************************
函数名称：Protocol_Packet_EnumDevice
函数功能：打包枚举的设备信息
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包的数据信息
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：pppSt_AudioList
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要打包的音频设备信息
 参数.四：pppSt_VideoList
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要打包的视频设备信息
 参数.五：nACount
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入音频设备个数
 参数.六：nVCount
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入视频设备个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_EnumDevice(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AVHELP_DEVICEINFO*** pppSt_AudioList, AVHELP_DEVICEINFO*** pppSt_VideoList, int nACount, int nVCount);
/********************************************************************
函数名称：Protocol_Packet_ListFile
函数功能：打包文件列表信息
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包的数据信息
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：pppszFileList
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要打包的文件列表
 参数.四：nListCount
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_ListFile(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, CHAR*** pppszFileList, int nListCount);
/************************************************************************/
/*                        解析类函数                                    */
/************************************************************************/
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
extern "C" BOOL Protocol_Parse_JsonRoot(LPCTSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO* pSt_ProtocolInfo);