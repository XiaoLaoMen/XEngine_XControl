#include "pch.h"
#include "Protocol_Packet.h"
/********************************************************************
//    Created:     2021/09/08  10:35:32
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Packet\Protocol_Packet.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Packet
//    File Base:   Protocol_Packet
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议模块打包类
//    History:
*********************************************************************/
CProtocol_Packet::CProtocol_Packet()
{
}
CProtocol_Packet::~CProtocol_Packet()
{
}
///////////////////////////////////////////////////////////////////////////////
//                      公有函数
///////////////////////////////////////////////////////////////////////////////
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
BOOL CProtocol_Packet::Protocol_Packet_EnumDevice(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AVHELP_DEVICEINFO*** pppSt_AudioList, AVHELP_DEVICEINFO*** pppSt_VideoList, int nACount, int nVCount)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::Value st_JsonVideo;
	Json::Value st_JsonAudio;
	Json::StreamWriterBuilder st_JsonBuilder;

	for (int i = 0; i < nACount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["nCardNumber"] = (*pppSt_AudioList)[i]->nCardNumber;
		st_JsonObject["nDeviceNumber"] = (*pppSt_AudioList)[i]->nDeviceNumber;
		st_JsonObject["tszName"] = (*pppSt_AudioList)[i]->tszName;
		st_JsonAudio.append(st_JsonObject);
	}
	for (int i = 0; i < nVCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["nCardNumber"] = (*pppSt_VideoList)[i]->nCardNumber;
		st_JsonObject["nDeviceNumber"] = (*pppSt_VideoList)[i]->nDeviceNumber;
		st_JsonObject["tszName"] = (*pppSt_VideoList)[i]->tszName;
		st_JsonVideo.append(st_JsonObject);
	}
	st_JsonArray["AArray"] = st_JsonAudio;
	st_JsonArray["VArray"] = st_JsonVideo;
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = "success";
	st_JsonRoot["data"] = st_JsonArray;
	st_JsonBuilder["emitUTF8"] = true;

	*pInt_MsgLen = Json::writeString(st_JsonBuilder, st_JsonRoot).length();
	memcpy(ptszMsgBuffer, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), *pInt_MsgLen);
	return TRUE;
}