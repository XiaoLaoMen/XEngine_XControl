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
extern "C" BOOL Protocol_Packet_EnumDevice(CHAR * ptszMsgBuffer, int* pInt_MsgLen, AVHELP_DEVICEINFO * **pppSt_AudioList, AVHELP_DEVICEINFO * **pppSt_VideoList, int nACount, int nVCount)
{
	return m_ProtocolPacket.Protocol_Packet_EnumDevice(ptszMsgBuffer, pInt_MsgLen, pppSt_AudioList, pppSt_VideoList, nACount, nVCount);
}
extern "C" BOOL Protocol_Packet_ListFile(CHAR * ptszMsgBuffer, int* pInt_MsgLen, CHAR * **pppszFileList, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_ListFile(ptszMsgBuffer, pInt_MsgLen, pppszFileList, nListCount);
}
/************************************************************************/
/*                        解析类函数                                    */
/************************************************************************/
extern "C" BOOL Protocol_Parse_JsonRoot(LPCSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO * pSt_ProtocolInfo)
{
	return m_ProtocolParse.Protocol_Parse_JsonRoot(lpszMsgBuffer, nMsgLen, pSt_ProtocolInfo);
}
extern "C" BOOL Protocol_Parse_Download(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszFileUrl, CHAR * ptszSaveUrl)
{
	return m_ProtocolParse.Protocol_Parse_Download(lpszMsgBuffer, nMsgLen, ptszFileUrl, ptszSaveUrl);
}
extern "C" BOOL Protocol_Parse_Delete(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszDelete)
{
	return m_ProtocolParse.Protocol_Parse_Delete(lpszMsgBuffer, nMsgLen, ptszDelete);
}
extern "C" BOOL Protocol_Parse_UPFile(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszUPFile, CHAR * ptszUPUrl)
{
	return m_ProtocolParse.Protocol_Parse_UPFile(lpszMsgBuffer, nMsgLen, ptszUPFile, ptszUPUrl);
}
extern "C" BOOL Protocol_Parse_ListFile(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszFindPath, CHAR * ptszPostUrl)
{
	return m_ProtocolParse.Protocol_Parse_ListFile(lpszMsgBuffer, nMsgLen, ptszFindPath, ptszPostUrl);
}
extern "C" BOOL Protocol_Parse_Exec(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszExecFile, int* pInt_ExecShow)
{
	return m_ProtocolParse.Protocol_Parse_Exec(lpszMsgBuffer, nMsgLen, ptszExecFile, pInt_ExecShow);
}
extern "C" BOOL Protocol_Parse_Message(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszMessageBox)
{
	return m_ProtocolParse.Protocol_Parse_Message(lpszMsgBuffer, nMsgLen, ptszMessageBox);
}
extern "C" BOOL Protocol_Parse_Stop(LPCSTR lpszMsgBuffer, int nMsgLen, DWORD * pInt_ProcessID)
{
	return m_ProtocolParse.Protocol_Parse_Stop(lpszMsgBuffer, nMsgLen, pInt_ProcessID);
}
extern "C" BOOL Protocol_Parse_Shutdown(LPCSTR lpszMsgBuffer, int nMsgLen, DWORD * pInt_SDType)
{
	return m_ProtocolParse.Protocol_Parse_Shutdown(lpszMsgBuffer, nMsgLen, pInt_SDType);
}
extern "C" BOOL Protocol_Parse_System(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszExecCmd)
{
	return m_ProtocolParse.Protocol_Parse_System(lpszMsgBuffer, nMsgLen, ptszExecCmd);
}
extern "C" BOOL Protocol_Parse_Report(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszIPAddr, int* pInt_Type)
{
	return m_ProtocolParse.Protocol_Parse_Report(lpszMsgBuffer, nMsgLen, ptszIPAddr, pInt_Type);
}
extern "C" BOOL Protocol_Parse_EnumDevice(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszIPAddr)
{
	return m_ProtocolParse.Protocol_Parse_EnumDevice(lpszMsgBuffer, nMsgLen, ptszIPAddr);
}
extern "C" BOOL Protocol_Parse_Serial(LPCSTR lpszMsgBuffer, int nMsgLen, __int64u * pInt_Serial)
{
	return m_ProtocolParse.Protocol_Parse_Serial(lpszMsgBuffer, nMsgLen, pInt_Serial);
}