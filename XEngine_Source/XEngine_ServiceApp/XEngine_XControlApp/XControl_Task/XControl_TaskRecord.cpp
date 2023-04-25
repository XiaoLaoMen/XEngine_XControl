#include "../XControl_Hdr.h"

void CALLBACK XEngine_AVCollect_CBScreen(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, XPVOID lParam)
{
}
void CALLBACK XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, XPVOID lParam)
{
}

bool XControl_TaskRecord_Start(LPCXSTR lpszSMSUrl, bool bAudio)
{
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));
	//启用音频
	if (bAudio)
	{
#ifdef _MSC_BUILD
		xhAudio = AVCollect_Audio_Init("dshow", "virtual-audio-capturer", XEngine_AVCollect_CBAudio);
#elif __linux__
		xhAudio = AVCollect_Audio_Init("alsa", "virtual-audio-capturer", XEngine_AVCollect_CBAudio);
#else
		xhAudio = AVCollect_Audio_Init("avfoundation", "virtual-audio-capturer", XEngine_AVCollect_CBAudio);
#endif
		if (NULL == xhAudio)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "音频采集器请求失败,错误码:%lX", AVCollect_GetLastError());
			return false;
		}
		st_AVInfo.st_AudioInfo.bEnable = true;
		AVCollect_Audio_GetInfo(xhAudio, &st_AVInfo);
	}
	//屏幕采集
	AVCOLLECT_SCREENINFO st_AVScreen;
	memset(&st_AVScreen, '\0', sizeof(AVCOLLECT_SCREENINFO));

	st_AVScreen.nFrameRate = 24;
	st_AVScreen.nPosX = 0;
	st_AVScreen.nPosY = 0;
	strcpy(st_AVScreen.tszVideoSize, "1920x1080");

	xhVideo = AVCollect_Video_Init("dshow", "video=screen-capture-recorder", &st_AVScreen, XEngine_AVCollect_CBScreen);
	if (NULL == xhVideo)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "屏幕采集器请求失败,错误码:%lX", AVCollect_GetLastError());
		return false;
	}

	st_AVInfo.st_VideoInfo.bEnable = true;
	AVCollect_Video_GetInfo(xhVideo, &st_AVInfo);

	xhStream = StreamClient_StreamPush_Init(lpszSMSUrl, &st_AVInfo);
	if (NULL == xhStream)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "推流请求失败,错误码:%lX", StreamClient_GetLastError());
		return false;
	}
	AVCollect_Audio_Start(xhAudio);
	AVCollect_Video_Start(xhVideo);
	return 0;
}