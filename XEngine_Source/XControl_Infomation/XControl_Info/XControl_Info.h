﻿#pragma once
/********************************************************************
//    Created:     2021/09/08  10:12:38
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Infomation\XControl_Info\XControl_Info.h
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Infomation\XControl_Info
//    File Base:   XControl_Info
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息收集类
//    History:
*********************************************************************/

class CXControl_Info
{
public:
    CXControl_Info();
    ~CXControl_Info();
public:
    bool XControl_Info_HardWare(XCHAR *ptszHWInfo, int *pInt_Len);
    bool XControl_Info_SoftWare(XCHAR *ptszSWInfo, int *pInt_Len);
private:
};
