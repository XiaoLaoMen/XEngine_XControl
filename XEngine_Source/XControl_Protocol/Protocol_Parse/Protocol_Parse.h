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
    bool Protocol_Parse_JsonRoot(LPCXSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO* pSt_ProtocolInfo);
    bool Protocol_Parse_Download(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszFileUrl, XCHAR* ptszSaveUrl);
    bool Protocol_Parse_Delete(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszDelete);
    bool Protocol_Parse_UPFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszUPFile, XCHAR* ptszUPUrl);
    bool Protocol_Parse_ListFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszFindPath);
    bool Protocol_Parse_Exec(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszExecFile, int* pInt_ExecShow);
    bool Protocol_Parse_Message(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMessageBox);
    bool Protocol_Parse_Stop(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG* pInt_ProcessID);
    bool Protocol_Parse_Shutdown(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG* pInt_SDType);
    bool Protocol_Parse_System(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszExecCmd);
    bool Protocol_Parse_Report(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_Type);
    bool Protocol_Parse_Serial(LPCXSTR lpszMsgBuffer, int nMsgLen, __int64u* pInt_Serial);
    bool Protocol_Parse_Connect(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszIPAddr, int* pInt_Port);
protected:
private:
};
