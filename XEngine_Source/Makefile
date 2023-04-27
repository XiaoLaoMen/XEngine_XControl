RELEASE = 0
UNICODE = 0
PLATFORM=linux
FLAGS= 

XCONTROL_JSON_PATH = ./XEngine_ThirdPart/jsoncpp

XCONTROL_INFO_PATH = ./XControl_Infomation
XCONTROL_CONFIG_PATH = ./XControl_Configure
XCONTROL_PROTOCOL_PATH = ./XControl_Protocol

XCONTROL_APP_PATH = ./XEngine_ServiceApp/XEngine_XControlApp

XENGINE_MODULE = libjsoncpp.so libXControl_Infomation.so libXControl_Configure.so libXControl_Protocol.so XEngine_XControl.exe

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULE)
	
libjsoncpp.so:
	make -C $(XCONTROL_JSON_PATH) PLATFORM=$(PLATFORM) $(FLAGS)

libXControl_Infomation.so:
	make -C $(XCONTROL_INFO_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXControl_Configure.so:
	make -C $(XCONTROL_CONFIG_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXControl_Protocol.so:
	make -C $(XCONTROL_PROTOCOL_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
	
XEngine_XControl.exe:
	make -C $(XCONTROL_APP_PATH) PLATFORM=$(PLATFORM) $(FLAGS)