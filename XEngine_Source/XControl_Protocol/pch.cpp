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
XBOOL Protocol_IsErrorOccur = FALSE;
XLONG Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CProtocol_Parse m_ProtocolParse;
CProtocol_Packet m_ProtocolPacket;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Protocol_GetLastError(int* pInt_SysError)
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
extern "C" XBOOL Protocol_Packet_EnumDevice(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, AVHELP_DEVICEINFO * **pppSt_AudioList, AVHELP_DEVICEINFO * **pppSt_VideoList, int nACount, int nVCount)
{
	return m_ProtocolPacket.Protocol_Packet_EnumDevice(ptszMsgBuffer, pInt_MsgLen, pppSt_AudioList, pppSt_VideoList, nACount, nVCount);
}
extern "C" XBOOL Protocol_Packet_ListFile(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XCHAR * **pppszFileList, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_ListFile(ptszMsgBuffer, pInt_MsgLen, pppszFileList, nListCount);
}
/************************************************************************/
/*                        解析类函数                                    */
/************************************************************************/
extern "C" XBOOL Protocol_Parse_JsonRoot(LPCXSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO * pSt_ProtocolInfo)
{
	return m_ProtocolParse.Protocol_Parse_JsonRoot(lpszMsgBuffer, nMsgLen, pSt_ProtocolInfo);
}
extern "C" XBOOL Protocol_Parse_Download(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszFileUrl, XCHAR * ptszSaveUrl)
{
	return m_ProtocolParse.Protocol_Parse_Download(lpszMsgBuffer, nMsgLen, ptszFileUrl, ptszSaveUrl);
}
extern "C" XBOOL Protocol_Parse_Delete(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszDelete)
{
	return m_ProtocolParse.Protocol_Parse_Delete(lpszMsgBuffer, nMsgLen, ptszDelete);
}
extern "C" XBOOL Protocol_Parse_UPFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszUPFile, XCHAR * ptszUPUrl)
{
	return m_ProtocolParse.Protocol_Parse_UPFile(lpszMsgBuffer, nMsgLen, ptszUPFile, ptszUPUrl);
}
extern "C" XBOOL Protocol_Parse_ListFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszFindPath, XCHAR * ptszPostUrl)
{
	return m_ProtocolParse.Protocol_Parse_ListFile(lpszMsgBuffer, nMsgLen, ptszFindPath, ptszPostUrl);
}
extern "C" XBOOL Protocol_Parse_Exec(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszExecFile, int* pInt_ExecShow)
{
	return m_ProtocolParse.Protocol_Parse_Exec(lpszMsgBuffer, nMsgLen, ptszExecFile, pInt_ExecShow);
}
extern "C" XBOOL Protocol_Parse_Message(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszMessageBox)
{
	return m_ProtocolParse.Protocol_Parse_Message(lpszMsgBuffer, nMsgLen, ptszMessageBox);
}
extern "C" XBOOL Protocol_Parse_Stop(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG * pInt_ProcessID)
{
	return m_ProtocolParse.Protocol_Parse_Stop(lpszMsgBuffer, nMsgLen, pInt_ProcessID);
}
extern "C" XBOOL Protocol_Parse_Shutdown(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG * pInt_SDType)
{
	return m_ProtocolParse.Protocol_Parse_Shutdown(lpszMsgBuffer, nMsgLen, pInt_SDType);
}
extern "C" XBOOL Protocol_Parse_System(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszExecCmd)
{
	return m_ProtocolParse.Protocol_Parse_System(lpszMsgBuffer, nMsgLen, ptszExecCmd);
}
extern "C" XBOOL Protocol_Parse_Report(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszIPAddr, int* pInt_Type)
{
	return m_ProtocolParse.Protocol_Parse_Report(lpszMsgBuffer, nMsgLen, ptszIPAddr, pInt_Type);
}
extern "C" XBOOL Protocol_Parse_EnumDevice(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszIPAddr)
{
	return m_ProtocolParse.Protocol_Parse_EnumDevice(lpszMsgBuffer, nMsgLen, ptszIPAddr);
}
extern "C" XBOOL Protocol_Parse_Serial(LPCXSTR lpszMsgBuffer, int nMsgLen, __int64u * pInt_Serial)
{
	return m_ProtocolParse.Protocol_Parse_Serial(lpszMsgBuffer, nMsgLen, pInt_Serial);
}