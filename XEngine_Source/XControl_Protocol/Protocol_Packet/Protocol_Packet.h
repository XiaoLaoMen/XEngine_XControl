#pragma once
/********************************************************************
//    Created:     2021/09/08  10:35:14
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Packet\Protocol_Packet.h
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Packet
//    File Base:   Protocol_Packet
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议模块打包类
//    History:
*********************************************************************/

class CProtocol_Packet
{
public:
    CProtocol_Packet();
    ~CProtocol_Packet();
public:
    BOOL Protocol_Packet_EnumDevice(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AVHELP_DEVICEINFO*** pppSt_AudioList, AVHELP_DEVICEINFO*** pppSt_VideoList, int nACount, int nVCount);
    BOOL Protocol_Packet_ListFile(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, CHAR*** pppszFileList, int nListCount);
protected:
private:
};
