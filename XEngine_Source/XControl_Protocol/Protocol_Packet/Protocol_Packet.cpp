﻿#include "pch.h"
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
bool CProtocol_Packet::Protocol_Packet_EnumDevice(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AVHELP_DEVICEINFO*** pppSt_AudioList, AVHELP_DEVICEINFO*** pppSt_VideoList, int nACount, int nVCount)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::Value st_JsonVideo;
	Json::Value st_JsonAudio;
	Json::StreamWriterBuilder st_JsonBuilder;

	for (int i = 0; i < nACount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["nDeviceInout"] = (*pppSt_AudioList)[i]->nDeviceInout;
		st_JsonObject["nDeviceType"] = (*pppSt_AudioList)[i]->nDeviceType;
		st_JsonObject["tszName"] = (*pppSt_AudioList)[i]->st_MetaInfo.tszKey;
		st_JsonAudio.append(st_JsonObject);
	}
	for (int i = 0; i < nVCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["nDeviceInout"] = (*pppSt_AudioList)[i]->nDeviceInout;
		st_JsonObject["nDeviceType"] = (*pppSt_AudioList)[i]->nDeviceType;
		st_JsonObject["tszName"] = (*pppSt_AudioList)[i]->st_MetaInfo.tszKey;
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
	return true;
}
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
bool CProtocol_Packet::Protocol_Packet_ListFile(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XCHAR*** pppszFileList, int nListCount)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::StreamWriterBuilder st_JsonBuilder;

	for (int i = 0; i < nListCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["FileName"] = (*pppszFileList)[i];
		st_JsonObject["FileDir"] = (*pppszFileList)[i];
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["Count"] = nListCount;
	st_JsonRoot["data"] = st_JsonArray;
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = "success";
	st_JsonBuilder["emitUTF8"] = true;

	*pInt_MsgLen = Json::writeString(st_JsonBuilder, st_JsonRoot).length();
	memcpy(ptszMsgBuffer, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), *pInt_MsgLen);

	return true;
}