#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <list>
#include <thread>
#include <memory>
#include <json/json.h>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../../XControl_Protocol.h"
#include "../../XControl_Configure/Config_Define.h"
#include "../../XControl_Configure/Config_Error.h"
#include "../../XControl_Infomation/XInfo_Define.h"
#include "../../XControl_Infomation/XInfo_Error.h"
#include "../../XControl_Protocol/Protocol_Define.h"
#include "../../XControl_Protocol/Protocol_Error.h"

extern BOOL bIsRun;
extern XLOG xhLog;
extern __int64u m_nTaskSerial;

extern shared_ptr<std::thread> pSTDThread_Http;
extern XENGINE_SERVERCONFIG st_ServiceConfig;

#include "XControl_Config.h"
#include "XControl_Handle.h"
#include "XControl_Task.h"

#ifdef _MSC_BUILD
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"../../x64/Debug/jsoncpp")
#pragma comment(lib,"../../x64/Debug/XControl_Configure.lib")
#pragma comment(lib,"../../x64/Debug/XControl_Infomation.lib")
#pragma comment(lib,"../../x64/Debug/XControl_Protocol.lib")
#else
#pragma comment(lib,"../../x64/Release/jsoncpp")
#pragma comment(lib,"../../x64/Release/XControl_Configure.lib")
#pragma comment(lib,"../../x64/Release/XControl_Infomation.lib")
#pragma comment(lib,"../../x64/Release/XControl_Protocol.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"../../Debug/jsoncpp")
#pragma comment(lib,"../../Debug/XControl_Configure.lib")
#pragma comment(lib,"../../Debug/XControl_Infomation.lib")
#pragma comment(lib,"../../Debug/XControl_Protocol.lib")
#else
#pragma comment(lib,"../../Release/jsoncpp")
#pragma comment(lib,"../../Release/XControl_Configure.lib")
#pragma comment(lib,"../../Release/XControl_Infomation.lib")
#pragma comment(lib,"../../Release/XControl_Protocol.lib")
#endif
#endif
#endif