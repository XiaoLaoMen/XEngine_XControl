CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADSO = -L ../XEngine_ThirdPart/jsoncpp
LIB = -ljsoncpp
LIBEX = 
OBJECTS = Configure_Json.o pch.o

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
FLAGS = -c -lc_p
DEBUG = -g -pg 
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
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif


all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXControl_Configure.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)


Configure_Json.o:./Configure_Json/Configure_Json.cpp                         
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Configure_Json/Configure_Json.cpp

pch.o:./pch.cpp                          
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXControl_Configure.$(FILEEXT)
	cp ./libXControl_Configure.$(FILEEXT) ../../XEngine_Release/libXControl_Configure.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.$(FILEEXT)                                         
