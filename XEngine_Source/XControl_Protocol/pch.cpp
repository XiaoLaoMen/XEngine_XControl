#include "pch.h"
#include "Protocol_Packet/Protocol_Packet.h"
#include "Protocol_Parse/Protocol_Parse.h"
/********************************************************************
//    Created:     2021/09/08  10:39:47
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\pch.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
BOOL Protocol_IsErrorOccur = FALSE;
DWORD Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CProtocol_Parse m_ProtocolParse;
CProtocol_Packet m_ProtocolPacket;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Protocol_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Protocol_dwErrorCode;
}
/************************************************************************/
/*                        封装类函数                                    */
/************************************************************************/
extern "C" BOOL Protocol_Packet_EnumDevice(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AVHELP_DEVICEINFO * **pppSt_AudioList, AVHELP_DEVICEINFO * **pppSt_VideoList, int nACount, int nVCount)
{
	return m_ProtocolPacket.Protocol_Packet_EnumDevice(ptszMsgBuffer, pInt_MsgLen, pppSt_AudioList, pppSt_VideoList, nACount, nVCount);
}
/************************************************************************/
/*                        解析类函数                                    */
/************************************************************************/
extern "C" BOOL Protocol_Parse_JsonRoot(LPCTSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO * pSt_ProtocolInfo)
{
	return m_ProtocolParse.Protocol_Parse_JsonRoot(lpszMsgBuffer, nMsgLen, pSt_ProtocolInfo);
}