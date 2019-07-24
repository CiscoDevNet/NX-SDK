# To be updated by the User

## By default, GO_PATH is set as ${NXSDK_ROOT}/go. So, create
## your go projects/applications in ${GO_PATH}/src. To
## build your simple GO project, just add your project name to 
## GO_BIN:= 
GO_BIN       :=

## NOTE: 1) GO bindings/Wrappers for NX-SDK are located at ${NXSDK_ROOT}/go/src/nx_sdk_go.   
##          Just, "import nx_sdk_go" to gain access to NX-SDK in your GO application.  
##       2) GO NX-SDK callback wrappers are simplified for GO application developers
##          and is located at ${NXSDK_ROOT}/go/nx_sdk_go_cb.go. Please copy nx_sdk_go_cb.go
##          to build your project. If you are doing "make" to build your applications then
##          Makefile recipes takes care of copying nx_sdk_go_cb.go to your project automatically.
##
## For complex GO project structure, please write your own Makefile recipes for your GO project.  
##
################################################################################################
####################### Dont update beyond this point###########################################
################################################################################################

### Set NX-SDK path                                                     
NXSDK_PATH     := ${NXSDK_ROOT}
ifeq "$(NXSDK_PATH)" ""                                                          
NXSDK_PATH     :=/NX-SDK
endif                                                                                   
                                                                                        
### Directory Structure                                                                                    
CXX_HEADERDIR  := ${NXSDK_PATH}/include
CXX_TYPESDIR   := include/types
CXX_BUILDDIR   := ${NXSDK_PATH}/build
CXX_TARGETDIR  := ${NXSDK_PATH}/bin
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
CXX_LIBDIR     := ${NXSDK_PATH}/libs
CXX_LIBSRCDIR  := ${NXSDK_PATH}/stubs
NXLIBDIR       := /isan/lib
INCLUDES       := -I$(CXX_HEADERDIR) -I$(CXX_LIBSRCDIR) -I$(CXX_EXSRCDIR) -I$(CXX_LIBDIR) -I$(CXX_TYPESDIR)
endif                                                                                        
                                                                                             
### NXSDK library target path                                                                   
CXX_LIBTARGET  := ${CXX_LIBDIR}/${CXX_LIBNAME}

### Example Go Apps
GO_PATH     := ${NXSDK_PATH}/go
EX_GO_PATH  := ${NXSDK_PATH}/examples/go
EX_GO_BIN   := customCliGoApp ribMgrGo featureMonitorGo ribRemoteGo

### Compiler
CXX              := ${CXX} ${M_ARCH} --sysroot=${SYSROOT}
CXXFLAGS         := ${INCLUDES} ${C_STD} -pthread -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-write-strings
LDXXFLAGS        := ${M_ARCH} --sysroot=${SYSROOT} -L${SYSROOT}/usr/lib -L${CXX_LIBDIR} -Wl,-rpath -Wl,${NXLIBDIR}
LD_LIBRARY_PATH  := ${SYSROOT}/usr/lib:$(NXSDK_LIBDIR)

### GO Env
export CGO_CFLAGS   =-I${CXX_HEADERDIR} -I$(CXX_HEADERDIR)/types -I${SYSROOT}/usr/include 
export CGO_CPPFLAGS =-I${CXX_HEADERDIR} -I$(CXX_HEADERDIR)/types -I${SYSROOT}/usr/include 
export CGO_LDFLAGS  =${M_ARCH} --sysroot=${SYSROOT} -L${SYSROOT}/usr/lib -L${CXX_LIBDIR} -Wl,-rpath -Wl,${NXLIBDIR} -lnxsdk

### All Binaries needed for this module
ifeq "$(ENXOS_SDK_PATH)" ""    
all: setup ${EX_GO_BIN} ${GO_BIN}                                              
else
all: setup ${CXX_LIBTARGET} ${EX_GO_BIN} ${GO_BIN}

### Build NXSDK Library
# Create stub Objs needed for NXSDK Library
LIBNXSDK_CPP  := $(wildcard ${CXX_LIBSRCDIR}/*.cpp)
LIBNXSDK_OBJS := $(addprefix $(CXX_BUILDDIR)/,$(notdir $(LIBNXSDK_CPP:.cpp=.o)))

$(CXX_BUILDDIR)/%.o: ${CXX_LIBSRCDIR}/%.cpp
	@echo -e "\n### Building Shared NX-SDK Library Objects - ($<) !!!"
	$(CXX) $(CXXFLAGS) -c -fPIC $< -o $@

${LIBNXSDK_OBJS}: setup

${CXX_LIBTARGET}: ${LIBNXSDK_OBJS} 
	@echo -e "\n### Building Shared NX-SDK Library - (${LIBTARGET})!!!"
	$(CXX) -shared -Wl,-soname,${CXX_LIBNAME} -o $@ $^ -lc -ldl    
endif

### Build Example Go binaries
# Create Objs needed for Example programs
${EX_GO_BIN}: ${CXX_LIBTARGET} ${GO_PATH}/src/nx_sdk_go/nx_sdk_go.go ${GO_PATH}/nx_sdk_go_cb.go
	@echo -e "\n### Building Example Go App  - ($@)!!!"
	test -d ${EX_GO_PATH}/src/nx_sdk_go || ln -s ${GO_PATH}/src/nx_sdk_go ${EX_GO_PATH}/src/nx_sdk_go  
	test -f ${EX_GO_PATH}/src/$@/nx_sdk_go_cb.go || ln -s ${GO_PATH}/nx_sdk_go_cb.go ${EX_GO_PATH}/src/$@/nx_sdk_go_cb.go
	GOPATH=${EX_GO_PATH} go install -v -work -x -ldflags="-extldflags '-Wl,-rpath,${NXLIBDIR}'" $@

### Build Example Go binaries
# Create Objs needed for Example programs
${GO_BIN}: ${CXX_LIBTARGET} ${GO_PATH}/src/nx_sdk_go/nx_sdk_go.go ${GO_PATH}/nx_sdk_go_cb.go
	@echo -e "\n### Building Your Go App  - ($@)!!!"
	test -f ${GO_PATH}/src/$@/nx_sdk_go_cb.go || ln -s ${GO_PATH}/nx_sdk_go_cb.go ${GO_PATH}/src/$@/nx_sdk_go_cb.go
	GOPATH=${GO_PATH} go install -v -work -x -ldflags="-extldflags '-Wl,-rpath,${NXLIBDIR}'" $@

setup:
	@mkdir -p ${CXX_BUILDDIR}
	@mkdir -p ${CXX_LIBDIR}
	@mkdir -p ${CXX_TARGETDIR}
 
clean:
	rm -rf *.o ${CXX_TARGETDIR}/* ${CXX_BUILDDIR}/*.o

cleaner:
	rm -rf  ${CXX_TARGETDIR} ${CXX_BUILDDIR} ${GO_PATH}/bin ${GO_PATH}/pkg ${EX_GO_PATH}/bin ${EX_GO_PATH}/pkg
