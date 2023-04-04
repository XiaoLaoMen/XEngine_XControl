#pragma once
/********************************************************************
//    Created:     2021/09/08  10:35:07
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Parse\Protocol_Parse.h
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Parse
//    File Base:   Protocol_Parse
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议模块解析类
//    History:
*********************************************************************/

class CProtocol_Parse
{
public:
    CProtocol_Parse();
    ~CProtocol_Parse();
public:
    XBOOL Protocol_Parse_JsonRoot(LPCXSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO* pSt_ProtocolInfo);
    XBOOL Protocol_Parse_Download(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszFileUrl, XCHAR* ptszSaveUrl);
    XBOOL Protocol_Parse_Delete(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszDelete);
    XBOOL Protocol_Parse_UPFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszUPFile, XCHAR* ptszUPUrl);
    XBOOL Protocol_Parse_ListFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszFindPath, XCHAR* ptszPostUrl);
    XBOOL Protocol_Parse_Exec(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszExecFile, int* pInt_ExecShow);
    XBOOL Protocol_Parse_Message(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMessageBox);
    XBOOL Protocol_Parse_Stop(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG* pInt_ProcessID);
    XBOOL Protocol_Parse_Shutdown(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG* pInt_SDType);
    XBOOL Protocol_Parse_System(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszExecCmd);
    XBOOL Protocol_Parse_Report(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszIPAddr, int* pInt_Type);
    XBOOL Protocol_Parse_EnumDevice(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszIPAddr);
    XBOOL Protocol_Parse_Serial(LPCXSTR lpszMsgBuffer, int nMsgLen, __int64u* pInt_Serial);
protected:
private:
};
