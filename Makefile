## To be updated by the User

## 1. Simple C++ Application build:
##    If your Application is a simple C++ application with a single file 
##    and linking just to nxsdk Library and not to any others then just 
##    add your App name (same as filename without the extension)
##    in SRCNXSDK_BIN and on "make" your respective bins will be 
##    automatically built in the bin/ directory. You dont have
##    to add explicit recipes in this Makefile to build your project. 
##    Refer to EXNXSDK_BIN for building binaries of respective 
##    Examples App. 
##         And also for Ex) if your App is myApp.cpp then
##         just add SRCNXSDK_BIN := myApp
##    To build multiple simple Apps just add your App Name without
##    extensions separated by spaces.
##         For Ex) If you have two Apps myApp1.cpp & myApp2.cpp then
##                 just add SRCNXSDK_BIN := myApp1 myApp2
##                 This would automatically build myApp1 and myApp2
##                 binaries on Make. 
##    NOTE: App source code should be in src/ (create src directory
##          if not available) to be added into SRCNXSDK_BIN (or)
##          change the SRCDIR accordingly.
SRCDIR       := src
SRCNXSDK_BIN := 

## 2. Complex C++ Application build:
##    If your Application is a complex C++ application with multiple files
##    and linking to other libraries then you need to add explicit 
##    recipes in this Makefile to build your project.
##    Lets say myApp.bin consists of two files myApp1.cpp & myApp2.cpp
##    A) Build your binary using the following example
##         ${CXX_TARGETDIR}/<AppBinName>: myApp1.cpp myApp2.cpp 
##            $(SDK_CXX) $(SDK_CXXFLAGS) $(LDFLAGS) $(RPATH) -o $(CXX_TARGETDIR)/$@  $(SRCDIR)/$@.cpp
##    B) Add ${CXX_TARGETDIR}/<AppBinName> to all: recipe. Or write your own recipe to
##       build your project if not in all.


################################################################################################
####################### Dont update beyond this point###########################################
################################################################################################

### Set NX-SDK path                                                                                   
NXSDK_PATH     := ${NXSDK_ROOT}
ifeq "$(NXSDK_PATH)" ""
NXSDK_PATH     :=/NX-SDK
endif 

### Directory Structure                                                                      
CXX_HEADERDIR  := include
CXX_TYPESDIR   := include/types
CXX_BUILDDIR   := build
CXX_EXSRCDIR   := examples/c++
CXX_TARGETDIR  := bin
CXX_LIBNAME    := libnxsdk.so

### Check if its running in NXOS WRL toolkit
### if it is set the ENXOS_SDK_ROOT env
ENXOS_SDK_PATH := ${ENXOS_SDK_ROOT}
ifeq "$(ENXOS_SDK_PATH)" ""
SYSROOT        :=
M_ARCH         := -m64
C_STD          := c++0x
CXX_LIBDIR     := ${NXSDK_PATH}/remote/libs
NXLIBDIR       := ${NXSDK_PATH}/remote/libs
INCLUDES       := -I$(CXX_HEADERDIR) -I$(CXX_EXSRCDIR) -I$(CXX_LIBDIR) -I$(CXX_TYPESDIR)
else
SYSROOT        := ${ENXOS_SDK_PATH}/sysroots/n9000-wrs-linux
M_ARCH         := -m32
C_STD          := c++0x
CXX_LIBDIR     := libs
CXX_LIBSRCDIR  := stubs
NXLIBDIR       := /isan/lib
INCLUDES       := -I$(CXX_HEADERDIR) -I$(CXX_LIBSRCDIR) -I$(CXX_EXSRCDIR) -I$(CXX_LIBDIR) -I$(CXX_TYPESDIR)
endif

### NXSDK library target path
CXX_LIBTARGET  := ${CXX_LIBDIR}/${CXX_LIBNAME}

### Binaries to be build for Example C++ apps.
EXNXSDK_BIN    := customCliApp  featureMonitor intfMonitor 

## Compiler
SDK_CXXFLAGS = -g -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-write-strings 

## Check if $(CXX) is set, if not set it to g++
ifneq "$(CXX)" ""
GCCMAJORVERSION := 1
GCCMINORVERSION := 1
else
CXX = g++
GCCMAJORVERSION := $(shell expr `g++ -dumpversion | cut -f1 -d.` \>= 4)
GCCMINORVERSION := $(shell expr `g++ -dumpversion | cut -f2 -d.` \>= 4)
endif

## Check if the compiler version > 4.4
ifeq "$(GCCMAJORVERSION)" "1"
ifeq "$(GCCMINORVERSION)" "1"
SDK_CXXFLAGS = -std=c++0x 
endif
endif

SDK_CXX       := $(CXX) $(M_ARCH) $(INCLUDES) 

## Linking
LDFLAGS   := -ldl -L$(CXX_LIBDIR)
RPATH     := -Wl,-rpath=${NXLIBDIR}

### All Binaries needed for this module
ifeq "$(ENXOS_SDK_PATH)" ""                                                                                  
all: setup ${EXNXSDK_BIN} ${SRCNXSDK_BIN}                                                                    
else
all: setup ${CXX_LIBTARGET} ${EXNXSDK_BIN} ${SRCNXSDK_BIN}

### Build NXSDK Library
# Create stub Objs needed for NXSDK Library
LIBNXSDK_CPP  := $(wildcard ${CXX_LIBSRCDIR}/*.cpp)
LIBNXSDK_OBJS := $(addprefix $(CXX_BUILDDIR)/,$(notdir $(LIBNXSDK_CPP:.cpp=.o)))

$(CXX_BUILDDIR)/%.o: ${CXX_LIBSRCDIR}/%.cpp
	@echo -e "\n### Building Shared NXSDK Library Objects - ($<)!!!"
	$(SDK_CXX) $(SDK_CXXFLAGS) -c -fPIC $< -o $@

${LIBNXSDK_OBJS}: setup

${CXX_LIBTARGET}: ${LIBNXSDK_OBJS} 
	@echo -e "\n### Building Shared NXSDK Library - (${CXX_LIBTARGET})!!!"
	$(SDK_CXX) -shared -Wl,-soname,${CXX_LIBNAME} -o $@ $^ -lc -ldl    
endif

### Build your simple App Binaries
# Create Objs needed for src programs
${SRCNXSDK_BIN}: ${CXX_LIBTARGET}
	@echo -e "\n### Building Source Apps  - ($@)!!!"
	$(SDK_CXX) $(SDK_CXXFLAGS) $(LDFLAGS) $(RPATH) -o $(CXX_TARGETDIR)/$@  $(SRCDIR)/$@.cpp -lnxsdk -lpthread

### Build Example binaries
# Create Objs needed for Example programs
${EXNXSDK_BIN}: ${CXX_LIBTARGET} 
	@echo -e "\n### Building Example C++ App  - ($@)!!!"
	$(SDK_CXX) $(SDK_CXXFLAGS) $(LDFLAGS) $(RPATH) -o $(CXX_TARGETDIR)/$@  $(CXX_EXSRCDIR)/$@.cpp -lnxsdk -lpthread

setup:
	@mkdir -p ${CXX_BUILDDIR}
	@mkdir -p ${CXX_LIBDIR}
	@mkdir -p ${CXX_TARGETDIR}
 
clean:
	rm -rf *.o ${CXX_TARGETDIR}/* ${CXX_BUILDDIR}/*.o 

cleaner:
	rm -rf  ${CXX_TARGETDIR} ${CXX_BUILDDIR}
