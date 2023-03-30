#include "../XControl_Hdr.h"

void CALLBACK XEngine_AVCollect_CBScreen(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, LPVOID lParam)
{
}
void CALLBACK XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, LPVOID lParam)
{
}

BOOL XControl_TaskRecord_Start(LPCSTR lpszSMSUrl, BOOL bAudio)
{
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));
	//启用音频
	if (bAudio)
	{
		xhAudio = AVCollect_Audio_Init("virtual-audio-capturer", XEngine_AVCollect_CBAudio);
		if (NULL == xhAudio)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "音频采集器请求失败,错误码:%lX", AVCollect_GetLastError());
			return FALSE;
		}
		st_AVInfo.st_AudioInfo.bEnable = TRUE;
		AVCollect_Audio_GetInfo(xhAudio, &st_AVInfo);
	}
	//屏幕采集
	AVCOLLECT_SCREENINFO st_AVScreen;
	memset(&st_AVScreen, '\0', sizeof(AVCOLLECT_SCREENINFO));

	st_AVScreen.nFrameRate = 24;
	st_AVScreen.nPosX = 0;
	st_AVScreen.nPosY = 0;
	strcpy(st_AVScreen.tszVideoSize, "1920x1080");
	strcpy(st_AVScreen.tszVideoDev, "video=screen-capture-recorder");

	xhVideo = AVCollect_Video_Init(&st_AVScreen, XEngine_AVCollect_CBScreen);
	if (NULL == xhVideo)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "屏幕采集器请求失败,错误码:%lX", AVCollect_GetLastError());
		return FALSE;
	}

	st_AVInfo.st_VideoInfo.bEnable = TRUE;
	AVCollect_Video_GetInfo(xhVideo, &st_AVInfo);

	xhStream = XClient_StreamPush_Init(lpszSMSUrl, &st_AVInfo);
	if (NULL == xhStream)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "推流请求失败,错误码:%lX", StreamClient_GetLastError());
		return FALSE;
	}
	AVCollect_Audio_Start(xhAudio);
	AVCollect_Video_Start(xhVideo);
	return 0;
}