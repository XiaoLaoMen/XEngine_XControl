CC = g++ -Wall -std=c++17
PLATFORM = linux
PLATVER = 
PLATDIR = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADSO = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_AVCodec -L /usr/local/lib/XEngine_Release/XEngine_StreamMedia \
         -L ../../XControl_Infomation -L ../../XControl_Configure -L ../../XControl_Protocol \
		 -L ../../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_OPenSsl -lHelpComponents_XLog -lXEngine_ProcFile -lXEngine_SystemApi -lNetHelp_APIClient -lXEngine_AVCollect -lXEngine_AVHelp -lStreamMedia_XClient \
      -lXControl_Infomation -lXControl_Configure -lXControl_Protocol -ljsoncpp
OBJECTS = XControl_Config.o XControl_Handle.o XControl_Task.o XEngine_XControlApp.o \
          XControl_TaskRecord.o

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
ifeq ($(PLATFORM),android)
FLAGS = -c
DEBUG = -g
else
FLAGS = -c -lc_p
DEBUG = -g -pg 
endif
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
		PLATDIR = XEngine_Linux/Centos
	else
		PLATVER = -D __UBUNTU__
		PLATDIR = XEngine_Linux/Ubuntu
	endif
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),android)
	NDK_ROOT = $(HOME)/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
	CC = $(NDK_ROOT) -target aarch64-linux-android30 -Wall -std=c++17 -fPIC
	PLATVER = -D __ANDROID__
	PLATDIR = XEngine_Android
	LIBEX = -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
else ifeq ($(PLATFORM),ios)
	SDKPATH = /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk
	CC += -arch arm64 -Wno-deprecated-declarations -isysroot ${SDKPATH} -miphoneos-version-min=13.0
	PLATVER = -D __IOS__
	PLATDIR = XEngine_IOS
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o XEngine_XControlApp $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)
  
XControl_Config.o:./XControl_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XControl_Config.cpp 
XControl_Handle.o:./XControl_Handle.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XControl_Handle.cpp
XControl_Task.o:./XControl_Task.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XControl_Task.cpp
	
XControl_TaskRecord.o:./XControl_Task/XControl_TaskRecord.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XControl_Task/XControl_TaskRecord.cpp

XEngine_XControlApp.o:./XEngine_XControlApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_XControlApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_XControlApp
	cp -f ./XEngine_XControlApp ../../../XEngine_Release/XEngine_XControlApp
	
CleanAll:CleanObj CleanMk 
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_XControlApp
