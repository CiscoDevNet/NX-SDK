## To be updated by the User

## 1. Simple Application build:
##    If your Application is a simple application with a single file 
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
SRCDIR       := src
SRCNXSDK_BIN :=  

## 2. Complex Application build
##    If your Application is a complex application with multiple files
##    and linking to other libraries then you need to add explicit 
##    recipes in this Makefile to build your project.
##    Lets say myApp.bin consists of two files myApp1.cpp & myApp2.cpp
##    A) Build your binary using the following example
##         ${TARGETDIR}/<AppBinName>: myApp1.cpp myApp2.cpp 
##            $(SDK_CXX) $(SDK_CXXFLAGS) $(LDFLAGS) $(RPATH) -o $(TARGETDIR)/$@  $(SRCDIR)/$@.cpp
##    B) Add ${TARGETDIR}/<AppBinName> to all: recipe. Or write your own recipe to
##       build your project if not in all.


################################################################################################
####################### Dont update beyond this point###########################################
################################################################################################

### VERSION of this Tool kit. 
MAJOR_VERSION := 1
MINOR_VERSION := 0

### Set this as the VERSION in the rpm SPEC file to build an RPM package.
VERSION    := ${MAJOR_VERSION}.${MINOR_VERSION}

### Directory Structure
HEADERDIR  := include
BUILDDIR   := build
EXSRCDIR   := examples
TARGETDIR  := bin
LIBDIR     := libs
NXLIBDIR   := /isan/lib
LIBSRCDIR  := stubs
LIBNAME    := libnxsdk.so
LIBTARGET  := ${LIBDIR}/${LIBNAME}
EXNXSDK_BIN:= customCliApp 

## Includes for the project
INCLUDES  := -I$(HEADERDIR) -I$(LIBSRCDIR) -I$(EXSRCDIR) -I$(LIBDIR)  

## Compiler
#SDK_CXXFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-local-typedefs -Wno-write-strings 
SDK_CXXFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-write-strings 

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

SDK_CXX       := $(CXX) -m32 $(INCLUDES) 

## Linking
LDFLAGS   := -ldl -L./$(LIBDIR)
RPATH     := -Wl,-rpath=${NXLIBDIR}

### All Binaries needed for this module
all: setup ${LIBTARGET} ${EXNXSDK_BIN} ${SRCNXSDK_BIN}

### Build NXSDK Library
# Create stub Objs needed for NXSDK Library
LIBNXSDK_CPP  := $(wildcard ${LIBSRCDIR}/*.cpp)
LIBNXSDK_OBJS := $(addprefix $(BUILDDIR)/,$(notdir $(LIBNXSDK_CPP:.cpp=.o)))

$(BUILDDIR)/%.o: ${LIBSRCDIR}/%.cpp
	@echo -e "\n### Building Shared NXSDK Library Objects - ($<)!!!"
	$(SDK_CXX) $(SDK_CXXFLAGS) -c -fPIC $< -o $@

${LIBTARGET}: ${LIBNXSDK_OBJS} 
	@echo -e "\n### Building Shared NXSDK Library - (${LIBTARGET})!!!"
	$(SDK_CXX) -shared -Wl,-soname,${LIBNAME} -o $@ $^ -lc -ldl    

### Build your simple App Binaries
# Create Objs needed for src programs
${SRCNXSDK_BIN}:
	@echo -e "\n### Building Source Apps  - ($@)!!!"
	$(SDK_CXX) $(SDK_CXXFLAGS) $(LDFLAGS) $(RPATH) -o $(TARGETDIR)/$@  $(SRCDIR)/$@.cpp -lnxsdk

### Build Example binaries
# Create Objs needed for Example programs
${EXNXSDK_BIN}:
	@echo -e "\n### Building Example TestApp  - ($@)!!!"
	$(SDK_CXX) $(SDK_CXXFLAGS) $(LDFLAGS) $(RPATH) -o $(TARGETDIR)/$@  $(EXSRCDIR)/$@.cpp -lnxsdk

setup:
	@mkdir -p ${BUILDDIR}
	@mkdir -p ${LIBDIR}
	@mkdir -p ${TARGETDIR}
 
clean:
	rm -rf *.o ${LIBDIR}/*.so ${TARGETDIR}/* ${BUILDDIR}/*.o 

cleaner:
	rm -rf  ${LIBDIR} ${TARGETDIR} ${BUILDDIR}
