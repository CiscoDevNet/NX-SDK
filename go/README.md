# GO Bindings for NX-SDK Applications

# Table of Contents

   - [Install GO](#install-go)
   - [Set ENXOS Environment](#set-enxos-environment)
   - [Documentaion](#documentation)
   - [Directory Structure](#directory-structure)
   - [Go NX-SDK Application guide](#go-nx-sdk-application-guide)
     * [Develop](#develop)
     * [Build](#build)
     * [RPM Package](#rpm-package)
     * [Install and run](#install-and-run)
   - [Notes](#notes)


# Install GO
  - Use the the following docker image for NX-SDK Go application development which already has ENXOS SDK 
    Build Environment & Go 1.7.4 preinstalled.
    ```
       docker pull dockercisco/nxsdk:<tag>
       Ex)    docker pull dockercisco/nxsdk:v1.7.5
       NOTE: Tag latest needs to be updated. 
    ```  
    
  - To start the container,
    ```
       docker run -it dockercisco/nxsdk:v1.7.5 /bin/bash
    ```  
  - Add GO to the path 
    ```
       export PATH=$PATH:/usr/local/go/bin
    ```   
           
  - Check GO is installed successfully 
    ``` 
       go version
       Output: go version go1.7.4 linux/386
    ```

# Set ENXOS Environment
  - Source 32-bit environment for your application
    ``` 
      export ENXOS_SDK_ROOT=/enxos-sdk
      cd $ENXOS_SDK_ROOT
      source environment-setup-x86-wrsmllib32-linux
    ```
    
  - For internal use, set NXSDK_ROOT to your nxsdk location     
    ```
      cd /
      git clone https://gitlab-sjc.cisco.com/sathsrin/nxsdk.git
      export NXSDK_ROOT=/nxsdk ! (by default if using internal git)
      
      ! If not in default location then
      export NXSDK_ROOT=<absolute-path-to-NX-SDK> 
  - Same for all languages.
  
# Documentation
  - Refer to API documentation and its usage and examples.

# Directory Structure
  ```
    go
    |____ Makfile
    |
    |____ src
          |
          |____ nx_sdk_go
    |
    |____ nx_sdk_go_cb.go
  ```    
  - [src/](src)   
    - Create your GO projects/applications in this directory as GOPATH is already set ${NXSDK_ROOT}/go
    - [nx_sdk_go](src/nx_sdk_go): GO Wrapper/bindings for NX-SDK Apis using SWIG. In your application, just "import nx_sdk_go" to gain access to NX-SDK functionalities from a GO application.
    - Refer to [sample Go applications](../examples/go/src).

  - [nx_sdk_go_cb.go](nx_sdk_go_cb.go)
    - Contains GO NX-SDK Callback Wrappers. 
    - Contains simplified callback wrappers for GO application developers hence only the respective PostCb functions need to be extended by them. 
    - Please copy nx_sdk_go_cb.go to the directory that has your go code extending NX-SDK PostCb functions. <br>
                      (or)
    - If you are doing "make" to build your applications then Makefile recipes takes care of symlinking to nx_sdk_go_cb.go in your project automatically. Hence nothing to be done when doing "make".
    - Refer to Makefile on how the [sample GO applications](../examples/go/src) are built.

  - [Makefile](Makefile)
    - Contains recipes to build your GO application. 
    - Refer to Makefile on how the sample example GO applications are built. Just do "make" in ${NXSDK_ROOT}/go.
    - Follow the instructions in the Makefile to build your GO application.
    
# Go NX-SDK Application guide

## Develop
   - Create your GO projects/applications in ${NXSDK_ROOT}/go. For ex) to create a pbwMonitorGo project,
     ```
        cd $NXSDK_ROOT/go/src
        mkdir pbwMonitorGo
        cd pbwMonitorGo
        touch pbwMonitorGo.go
        
        !In pbwMonitorGo add "import nx_sdk_go". Refer to sample GO applications.
     ```
     
## Build     
   - In ${NXSDK_ROOT}/go/Makefile add your GO Project to GO_BIN:=. For ex) to build pbwMonitorGo project
     ```
       cd ${NXSDK_ROOT}/go
       vi Makefile
       GO_BIN := pbwMonitorGo
       !save the changes to Makefile
       
       ! To build your Go project
       make
       
       Note: Makefile recipes automatically creates a symlink of nx_sdk_go_cb.go wrappers in pbwMonitorGo/ thereby 
             allowing GO application developers to simply extend the respective PostCb functions.
     ```  
   - Make Sure $NXSDK_ROOT environment variable is set to the right NX-SDK path you are working on.
   - On Successful build, application binary will be generated at ${NXSDK_ROOT}/go/bin.
     ```
       ls ${NXSDK_ROOT}/go/bin/pbwMonitorGo
     ```
     
## RPM Package
   - To build an RPM package for your GO application binary use the helper [rpm_gen.py](../scripts/rpm_gen.py) script. For ex)
     ```
       cd $NXSDK_ROOT/scripts
       
       ! For help,
       python rpm_gen.py -h 
       
       ! To generate RPM package for pbwMonitorGo binary,
       python rpm_gen.py pbwMonitorGo -s ../go/src -t ../go/bin
     ```
 
## Install and run
   - Installing and running the Go NX-SDK application in the switch is same as any other NX-SDK application. Refer to [README](../README.md)

# Notes
  - Exception hanlding in Go is not handled in V1.7.5
  - If "go version" fails then add GO to the path "export PATH=$PATH:/usr/local/go/bin"
  
