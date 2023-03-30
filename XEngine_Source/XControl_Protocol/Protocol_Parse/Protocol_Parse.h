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
    BOOL Protocol_Parse_JsonRoot(LPCSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO* pSt_ProtocolInfo);
    BOOL Protocol_Parse_Download(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFileUrl, CHAR* ptszSaveUrl);
    BOOL Protocol_Parse_Delete(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszDelete);
    BOOL Protocol_Parse_UPFile(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszUPFile, CHAR* ptszUPUrl);
    BOOL Protocol_Parse_ListFile(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFindPath, CHAR* ptszPostUrl);
    BOOL Protocol_Parse_Exec(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszExecFile, int* pInt_ExecShow);
    BOOL Protocol_Parse_Message(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszMessageBox);
    BOOL Protocol_Parse_Stop(LPCSTR lpszMsgBuffer, int nMsgLen, DWORD* pInt_ProcessID);
    BOOL Protocol_Parse_Shutdown(LPCSTR lpszMsgBuffer, int nMsgLen, DWORD* pInt_SDType);
    BOOL Protocol_Parse_System(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszExecCmd);
    BOOL Protocol_Parse_Report(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszIPAddr, int* pInt_Type);
    BOOL Protocol_Parse_EnumDevice(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszIPAddr);
    BOOL Protocol_Parse_Serial(LPCSTR lpszMsgBuffer, int nMsgLen, __int64u* pInt_Serial);
protected:
private:
};
