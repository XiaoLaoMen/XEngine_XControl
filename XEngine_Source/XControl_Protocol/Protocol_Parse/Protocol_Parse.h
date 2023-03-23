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
    BOOL Protocol_Parse_JsonRoot(LPCTSTR lpszMsgBuffer, int nMsgLen, XCONTROL_PROTOCOLINFO* pSt_ProtocolInfo);
    BOOL Protocol_Parse_Download(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszFileUrl, TCHAR* ptszSaveUrl);
    BOOL Protocol_Parse_Delete(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszDelete);
    BOOL Protocol_Parse_UPFile(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszUPFile, TCHAR* ptszUPUrl);
    BOOL Protocol_Parse_ListFile(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszFindPath, TCHAR* ptszPostUrl);
    BOOL Protocol_Parse_Exec(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszExecFile, int* pInt_ExecShow);
    BOOL Protocol_Parse_Message(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszMessageBox);
    BOOL Protocol_Parse_Stop(LPCTSTR lpszMsgBuffer, int nMsgLen, DWORD* pInt_ProcessID);
    BOOL Protocol_Parse_Shutdown(LPCTSTR lpszMsgBuffer, int nMsgLen, DWORD* pInt_SDType);
    BOOL Protocol_Parse_System(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszExecCmd);
    BOOL Protocol_Parse_Report(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszIPAddr, int* pInt_Type);
    BOOL Protocol_Parse_EnumDevice(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszIPAddr);
    BOOL Protocol_Parse_Serial(LPCTSTR lpszMsgBuffer, int nMsgLen, __int64u* pInt_Serial);
protected:
private:
};
