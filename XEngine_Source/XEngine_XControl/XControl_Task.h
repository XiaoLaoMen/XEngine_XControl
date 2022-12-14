#pragma once

XHTHREAD XControl_Thread_HttpTask();
XHTHREAD XControl_Thread_TCPTask();

BOOL XControl_Task_ProtocolParse(LPCSTR lpszMsgBuffer, int nMsgLen);