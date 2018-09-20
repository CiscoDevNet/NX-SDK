<snippet>
  <content>
  
# NX-SDK 

# Table of Contents

  - [Cisco NX-SDK](#cisco-nx-sdk)
  - [Release](#release)
  - [Documentation and Usage](#documentation-and-usage)
  - [Description](#description)
  - [Custom Application Development using NX-SDK Requirements](#custom-application-development-using-nx-sdk-requirements)
  - [Custom Application Development Flow using NX-SDK](#custom-application-development-flow-using-nx-sdk)
    * [1. Install NX-SDK](#1-install-nx-sdk)
      + [a) ENXOS SDK Build Environment [Optional]](#a-enxos-sdk-build-environment-optional)
      + [b) Get NX-SDK toolkit](#b-get-nx-sdk-toolkit)
    * [2. Building Custom Applications](#2-building-custom-applications)
    * [3. Unit Testing Custom Application](#3-unit-testing-custom-application)
    * [4. Packaging Custom Application](#4-packaging-custom-application)
      + [a) Auto-generate RPM package using Script](#a-auto-generate-rpm-package-using-script)
      + [b) Manually-generate RPM Package](#b-manually-generate-rpm-package)
    * [5. Installing Custom Application in Switch](#5-installing-custom-application-in-switch)
    * [6. Running Custom Application in Switch](#6-running-custom-application-in-switch)
    * [7. Verify if Custom Application is running](#7-verify-if-custom-application-is-running)
    * [8. Stop Custom Application in Switch](#8-stop-custom-application-in-switch)
    * [9. Remove Custom Application from switch](#9-remove-custom-application-from-switch)
    * [10. Troubleshoot](#10-troubleshoot)
    * [11. Sample Custom Applications created using NX-SDK](#11-sample-custom-applications-created-using-nx-sdk)
    * [12. Helpful Notes](#12-helpful-notes)

# Cisco NX-SDK

<p align="justify">
Cisco NX-SDK provides a simple, flexible, modernized and powerful tool for off the box third party custom Application development to gain access to Nexus infrastructure 
functionalities which when run inside the Nexus switches allow the custom applications to run natively just like any other Cisco native Nexus applications.
It is appropriate for Do-it-Yourself Automation to develop custom applications to fit your needs thereby decoupling application development from Nexus releases. NX-SDK offers various functionalities like ability to 
generate custom CLIs, Syslogs, Event manager, HA, Route Manager, Streaming Telemetry and much more. 

NX-SDK provides Abstraction/plugin Library Layer thereby decoupling the Application from underlying infrastructure being used.
Hence, its easy and simple to change infra without affecting the applications. Hence, NX-SDK is being used for
developing native Cisco Applications as well. 

It is built using C++ language. Other language (python, go, ruby etc) bindings will also be provided for NX-SDK hence custom applications 
can be developed and built in any language of users choice. 

This github provides NX-SDK toolkit for custom application development in your favorite Linux environment to gain access to NX-OS infrastructure.
</p>

# Release 

- Refer to [NX-SDK versions](versions.md) for more details on different versions and its supported features.
- Refer to <a href="https://github.com/CiscoDevNet/NX-SDK/releases">NX-SDK releases</a> to get older NX-SDK versions. 

# Documentation and Usage

  - For C++ custom Application development, public APIs are available in [include/](include/). Its description and usage are available 
    in <a href="https://htmlpreview.github.io/?https://github.com/CiscoDevNet/NX-SDK/blob/master/doc/html/annotated.html">API documentation</a> (doxygen generated). 
  - Python and other language bindings will be auto-generated from the above C++ APIs using <a href="http://www.swig.org/">SWIG</a>. Refer to [version](versions.md) for more details on supported language bindings in each NX-SDK version.
  - Refer to [example NX-SDK applications](examples/README.md) for more details.

# Description

For detailed description and directory structure of Cisco NX-SDK toolkit, refer to [description](description.md).

# Custom Application Development using NX-SDK Requirements
 The user has two options in development environments when using the NX-SDK for application development:
  - Linux (Currently supported platforms) <br>
    Any recent version of the major distributions that have access to the GNU C/C++ toolchains and standard 
    libraries.  This also includes running Linux as a virtual machine. <br>
               (Or) <br>
  - Docker Recommended (Docker container with ENXOS SDK containing all tools to build an application to be run 
    in NXOS switches successfully. For best results, build the App using NX-SDK as RPM package in ENXOS SDK).<br>
    NOTE: It is required to have at least 8GB of free space available for the installation of the Docker environment.
          The Docker image and associated build tools are built to run in  64-bit host systems only.
    ```
      For Apps started in BASH,
          - Python Apps: 
               ENXOS SDK Environment is not needed. Python Apps can be directly copied to the switch and can be started in BASH. 
               Refer to steps on how to start python Apps in BASH.
          - C++ Apps:
               Its optional to use ENXOS SDK build environment and apps can be built using native g++ and run in the switch but at your own
               risk. For best results, build the Apps in ENXOS SDK build environment and run in the switch.
               
      For Apps started in VSH:
          - To start Custom Apps from VSH, it requires the Apps to be installed through RPM package. Hence its mandatory for the Apps 
            to be built as an RPM package in ENXOS SDK for VSH apps to persist across reloads and System switchover. 
            Hence, EXOS SDK build environment is mandatory for Apps to be started in VSH.
      
      NOTE: Please ensure that the name of the custom application does not collide with any existing Native Nexus applications.
    ```        

#  Custom Application Development Flow using NX-SDK

## 1. Install NX-SDK

### a) ENXOS SDK Build Environment 

  - NOTE: Mandatory for Custom Applications to be started in VSH.
  - ENXOS build environment can built using the provided Dockerfiles

  #### Build using Dockerfile
   ```
      Ex for ubuntu) docker build --squash -t  nxsdk_ubuntu containers/wrl5/ubuntu
   ``` 
  #### Download from Docker Hub
  - SDK Build Environment can be obtained from https://hub.docker.com/r/dockercisco/nxsdk
    Pull the Image version of your choice using.

    ```
      docker pull dockercisco/nxsdk:<tag>
      Ex)    docker pull dockercisco/nxsdk:v1
      NOTE: Tag latest needs to be updated. 
    ```

  #### To start a container
     
   ```
    If you built one from a Dockerfile:

    docker run -it nxsdk_ubuntu /bin/bash

    If you pulled one from dockerhub:

    docker images ! to get the <tag>
    docker run -it dockercisco/nxsdk:<tag> /bin/bash
   ```
   ---
  **NOTE**
  *Changes made in the docker container are transient and will go away after you 
  exit the container. Make plans accordingly.*
   
   ---

  **Docker Build Troubleshooting**
  
  | Issue | Solution |
  |---|---|
  |*Unknown option --squash*| *--squash* is a feature available in newer versions of docker. If you are running if you are running an older docker version use use build command without *--squash*.|
  |*Error message: "Directory renamed before its status could be extracted"*| This message has been seen in some cases when using *overlay/overlay2* as the docker storage-driver. Change docker configuration to use *aufs*.|
  
   ---
    
  - ENXOS SDK build environment is already installed in 
    ```
      /enxos-sdk
    ```
    
  - The following environment variables are set by default if you build your container using `docker build`
 
    ```
      ENXOS_SDK_ROOT=/enxos-sdk
    ```   
  - The enxos-sdk toolchain is sourced by default if you built your container using `docker build`
    ```
      source /enxos-sdk/environment-setup-x86-wrsmllib32-linux 
    ```

    *If you used `docker pull` to download your container, you need to set the above environment variables manually.*

### b) Get NX-SDK toolkit
  - ENXOS SDK docker image already has NX-SDK installed in /NX-SDK (default location) with NXSDK_ROOT environment variable set to /NX-SDK.
    ```
      export NXSDK_ROOT=/NX-SDK
    ```  

  - If NX-SDK is installed in any other location other than default /NX-SDK then
    its mandatory to set NXSDK_ROOT to the right location.
    ```
      export NXSDK_ROOT=<absolute-path-to-NX-SDK> (if not default /NX-SDK)
    ```  
    
  - To get latest version of NX-SDK do git pull of NX-SDK.
    ```
      cd $NXSDK_ROOT
      ls (Makefile  README.md  doc  examples  include  python  rpm  src  scripts stubs)
      git pull
    ```

  - To get the older version of NX-SDK, git clone the branch using the respective tag   
    ```
       git clone -b <tag> <url>
       For ex) To Clone older version NX-SDK v1.0.0
         git clone -b v1.0.0 https://github.com/CiscoDevNet/NX-SDK.git
    ```
    
  - Refer to API usage and sample applications to develop your application based on your needs.

## 2. Building Custom Applications
  - C++ Application 
    - To build C++ custom application, add your application to the [Makefile](Makefile) just like example apps [customCliApp.cpp](examples/customCliApp.cpp)
      and follow the instructions in the Makefile.
    - Make sure the App builds without any errors using

      ```
        $PWD/NX-SDK# make clean
        $PWD/NX-SDK# make all
      ```
    - If run into make errors then check to Source 32-bit environment for your application 
      ```    
         export ENXOS_SDK_ROOT=/enxos-sdk
         cd $ENXOS_SDK_ROOT
         source environment-setup-x86-wrsmllib32-linux 
      ```
    -  NOTE: <p>If building applications in the native Linux environment (outside the NX-SDK build environment), 
             use the proper options for the build tools to generate 32-bit binaries, e.g. "-m32".  This is 
             taken care of for you if using the NX-SDK build environment. </p>
  - Nothing to build for Python Applications.      

## 3. Unit Testing Custom Application
  - For simple testing of the custom application, copy the application (binary(C++) or source(Python)) to the switch.
  - Start the custom application from BASH (step 6) for simple testing.
  - Once the custom application has been tested and verified in BASH then proceed to step (4) to start the application from VSH.
 
## 4. Packaging Custom Application

  - Needed for custom application to be started from VSH inorder to persist on switch reloads or system switchover.
  - NOTE: RPM packaging needs to be done within the provided ENXOS Docker image. 

### a) Auto-generate RPM package using Script
 - Use [rpm_gen.py](scripts/rpm_gen.py) script to auto-generate RPM package 
   for your custom application. For usage and more information, please run the script with -h option.
   ```
     /NX-SDK# python scripts/rpm_gen.py -h 
    ``` 
 - NOTE: By default, NXSDK_ROOT is set to /NX-SDK. If NX-SDK is installed in any other location other than the
         default location then its mandatory to set NXSDK_ROOT env to the right location for the script to work.
   ```
      export NXSDK_ROOT=<absolute-path-to-NX-SDK> (if not default /NX-SDK)
   ```
  - Refer to the following screen captures (Script Usage & Help, Auto-generate RPM package for C++ App examples/customCliApp.cpp, 
    Auto-generate RPM package for python App python/examples/customCliPyApp)

#### Example 1: rpm_gen.py -h

```
root@f15a7b454b00:/NX-SDK# python scripts/rpm_gen.py -h
usage: rpm_gen.py [-h] [-s SOURCE] [-u | -t TARGET] [-d DESCRIPTION] [-v VERSION] name

This script generates RPM packages for custom applications built using NX-SDK. Please export the path
for NX-SDK as environment variable NXSDK_ROOT and ENXOS-SDK as environment variable ENXOS_SDK_ROOT
before calling the script. If not found, the script assumes NX-SDK to be present at the following
location /NX-SDK and ENXOS-SDK to be present at /enxos-sdk

positional arguments:
  name                  Name of the application binary or the file name in case of python

optional arguments:
  -h, --help            show this help message and exit
  -s SOURCE, --source SOURCE
                        Path of the folder where application source files are present (default: /NX-
                        SDK/examples)
  -u, --use_source_as_target
                        If the source file is the same as the executable (default: False)
  -t TARGET, --target TARGET
                        Path where the application binary or the python file is present (default: /NX-
                        SDK/bin)
  -d DESCRIPTION, --description DESCRIPTION
                        Application Description (default: RPM package for custom application)
  -v VERSION, --version VERSION
                        Application Version (default: 1.0)
root@f15a7b454b00:/NX-SDK#
```

#### Example 2: Generating an RPM for a C++ Application 

```
root@f15a7b454b00:/NX-SDK# ./scripts/rpm_gen.py customCliApp
####################################################################################################
Generating rpm package...
Executing(%prep): /bin/sh -e /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/var/tmp/rpm-tmp.26482
+ umask 022
+ cd /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../src/rpm/BUILD
+ exit 0
Executing(%build): /bin/sh -e /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/var/tmp/rpm-tmp.26482
+ umask 022
+ cd /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../src/rpm/BUILD
+ exit 0
Executing(%install): /bin/sh -e /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/var/tmp/rpm-tmp.26482
+ umask 022
+ cd /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../src/rpm/BUILD
+ /bin/rm -rf /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliApp-root
+ /bin/mkdir -p /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliApp-root
+ rm -rf /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliApp-root//isan/bin/nxsdk
+ mkdir -p /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliApp-root//isan/bin/nxsdk
+ cp -R /NX-SDK/bin/customCliApp /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliApp-root//isan/bin/nxsdk
+ exit 0
Processing files: customCliApp-1.0-1.5.0.x86_64
Provides: elf(buildid) = 0aafb98cd3ee12b036b83faff7d9383a1d905ee0
Requires: libc.so.6 libc.so.6(GLIBC_2.0) libc.so.6(GLIBC_2.1.3) libdl.so.2 libgcc_s.so.1 libgcc_s.so.1(GCC_3.0) libm.so.6 libnxsdk.so libstdc++.so.6 libstdc++.so.6(CXXABI_1.3) libstdc++.so.6(GLIBCXX_3.4) libstdc++.so.6(GLIBCXX_3.4.21) rtld(GNU_HASH)
Checking for unpackaged file(s): /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/check-files /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliApp-root
Wrote: /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/src/rpm/SRPMS/customCliApp-1.0-1.5.0.src.rpm
Wrote: /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/src/rpm/RPMS/x86_64/customCliApp-1.0-1.5.0.x86_64.rpm
Executing(%clean): /bin/sh -e /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/var/tmp/rpm-tmp.26482
+ umask 022
+ cd /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../src/rpm/BUILD
+ /bin/rm -rf /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliApp-root
RPM Package: /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/src/rpm/RPMS/x86_64/customCliApp-1.0-1.5.0.x86_64.rpm
RPM package has been built
####################################################################################################
SPEC file: /NX-SDK/rpm/SPECS/customCliApp.spec
RPM file : /NX-SDK/rpm/RPMS/customCliApp-1.0-1.5.0.x86_64.rpm
```

#### Example 3: Generating an RPM for a Python Application

```
root@f15a7b454b00:/NX-SDK# ./scripts/rpm_gen.py customCliPyApp -s examples/python -u
####################################################################################################
Generating rpm package...
Executing(%prep): /bin/sh -e /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/var/tmp/rpm-tmp.37123
+ umask 022
+ cd /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../src/rpm/BUILD
+ exit 0
Executing(%build): /bin/sh -e /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/var/tmp/rpm-tmp.37123
+ umask 022
+ cd /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../src/rpm/BUILD
+ exit 0
Executing(%install): /bin/sh -e /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/var/tmp/rpm-tmp.37123
+ umask 022
+ cd /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../src/rpm/BUILD
+ /bin/rm -rf /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliPyApp-root
+ /bin/mkdir -p /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliPyApp-root
+ rm -rf /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliPyApp-root//isan/bin/nxsdk
+ mkdir -p /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliPyApp-root//isan/bin/nxsdk
+ cp -R /NX-SDK/examples/python/customCliPyApp /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliPyApp-root//isan/bin/nxsdk
+ exit 0
Processing files: customCliPyApp-1.0-1.5.0.x86_64
Checking for unpackaged file(s): /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/check-files /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliPyApp-root
Wrote: /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/src/rpm/SRPMS/customCliPyApp-1.0-1.5.0.src.rpm
Wrote: /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/src/rpm/RPMS/x86_64/customCliPyApp-1.0-1.5.0.x86_64.rpm
Executing(%clean): /bin/sh -e /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/var/tmp/rpm-tmp.37123
+ umask 022
+ cd /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../src/rpm/BUILD
+ /bin/rm -rf /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/../lib/rpm/../../../var/tmp/customCliPyApp-root
RPM Package: /enxos-sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/src/rpm/RPMS/x86_64/customCliPyApp-1.0-1.5.0.x86_64.rpm
RPM package has been built
####################################################################################################
SPEC file: /NX-SDK/rpm/SPECS/customCliPyApp.spec
RPM file : /NX-SDK/rpm/RPMS/customCliPyApp-1.0-1.5.0.x86_64.rpm
root@f15a7b454b00:/NX-SDK#
```

### b) Manually-generate RPM Package
  - Write the respective <app>.spec to build an RPM package for your App. This is to address building RPM packaging 
    for complex applications that cannot be generated using the above script.
    ```
      export RPM_ROOT=$ENXOS_SDK_ROOT/sysroots/x86_64-wrlinuxsdk-linux/usr/src/rpm
      ls $RPM_ROOT (BUILD  RPMS  SOURCES  SPECS  SRPMS)
      vi $RPM_ROOT/SPECS/<app>.spec
    ``` 
    Refer to the [customCliApp.spec](rpm/SPECS/customCliApp.spec) file for the sample App [customCliApp.cpp](examples/customCliApp.cpp)
    
  - Follow the instructions mentioned in the [customCliApp.spec](rpm/SPECS/customCliApp.spec) to build a spec file for your Application.
  - To build an RPM package use
 
    ```
      rpm -ba $RPM_ROOT/SPECS/<app>.spec
    ```
  - If RPM build was successful then RPM package will be generated in 
    
    ```
      ls $RPM_ROOT/RPMS/x86_64/
    ```
  - NOTE: All sample application RPMs are located at [rpm/RPMS](rpm/RPMS).
  
## 5. Installing Custom Application in Switch 
  - Copy the Application [binary] (or) RPM package to the switch (/bootflash). 
  - To install RPM package in the switch use,
  - <b> VSH</b>
  
    ```
      ! Add the RPM package to the installer using,
      switch(config)# install add bootflash:<app-rpm-package>.rpm
       
      ! On successful installation, Check if the RPM package is part of the inactive list using,
      switch(config)# show install inactive 
      
      ! When you are ready, activate the RPM package using,
      ! NOTE: .rpm should not be given to install activate/deactivate and should be given only to install add/remove.
      switch(config)# install activate <app-rpm-package>
      
      ! On successful activation, check if the RPM package is part of the active list using,
      switch(config)# show install active
    ```
    
  - <b> BASH </b>
  
    ```
      switch(config)# run bash sudo su
      bash# yum install /bootflash/<app-rpm-package>.rpm
    ```
  - On successful RPM install, App will be installed in the location specified in the spec file.
    (Recommended to place the app in /isan/bin/nxsdk. Follow the example spec file)

    ```
      ls <app-full-path>
    ```
  - NOTE: On Dual-Sup make sure that RPM package is installed on both Active and Standby Sups. If not 
          installed on standby, then on system switchover, Apps may not be started.
          

## 6. Running Custom Application in Switch
  - To run the custom Application using NX-SDK in the switch, as a prerequisite enable 
 
    ```
      switch(config)# feature nxsdk
    ```   
  - To run C++ Custom Application from 
    - VSH
      - rpm_gen.py script installs the application in default location /isan/bin/nxsdk. 
        NOTE: To start an Application from the default location just use the App Name and 
        full path is not required.
  
        ```
          switch(config)# nxsdk service-name <app-name>
         
          ! To start an Application in the default location /isan/bin/nxsdk/customCliApp, use the App Name.
          Ex) switch(config)# nxsdk service-name customCliApp
        ```
      - If the application is installed in any other location then full path to the Application 
        needs to be specified.
        ```
          switch(conf ig)# nxsdk service-name <app-full-path/app-name>
         
          ! To Start an Application /isan/bin/testApp, use the full path of the App.
          Ex) switch(config)# nxsdk service-name /isan/bin/testApp        
        ```
      - NOTE: An Application can be started from VSH only if it is installed through RPM Package.  
      
    - BASH
      ```
        switch(config)# run bash sudo su
        
        bash# <app-full-path> &
        
        NOTE: Applications residing in bootflash cannot be given execute permissions. Hence, 
              cannot run an Application from boottflash. Move the application to /isan/bin 
              to give execute permission and to run the application.
      ```
  - To run Python Custom Application
    - VSH
      - Same as running C++ application from VSH. <br>
        NOTE: Python app should be made as executable to be started from VSH. <br>
              chmod +x app-full-path  <br>
              Place, #!/isan/bin/nxpython in the first line of python application. <br>
              /isan/bin/nxpython should be used to run NX-SDK Python Apps. <br>
      
    - BASH
    
      ```
        switch(config)# run bash sudo su
        
        bash# nohup /isan/bin/python <app-full-path> &
        
        NOTE: To run the python App using NX-SDk in BASH, use /isan/bin/python to run the app as it sets the 
              necessary environment needed to run python Apps in BASH.
      ```
  - NOTE: Maximum of only 32 NX-SDK applications can be run at a time both from VSH & BASH.     

## 7. Verify if Custom Application is running
  - Check if your service is running by
    
    ```
      show nxsdk internal service
      
      switch(config)# sh nxsdk internal service 

      NXSDK total services (Max Allowed) : 2 (32)
      NXSDK Default App Path         : /isan/bin/nxsdk
      NXSDK Supported Versions       :  1.0 

      Service-name              Base App        Started(PID)    Version    RPM Package
      ------------------------- --------------- ------------    ---------- -------------------
      /isan/bin/TestApp.py      nxsdk_app3      BASH(27823)     -          -    
      pbwMonitor                nxsdk_app1      VSH(28161)      1.0        pbwMonitor-1.0-7.0.3.I6.1.x86_64
      
      NOTE: App is running if the above command shows a valid PID.
    ```   
    
    - When custom Application is started, by default a few show commands are auto-generated for the App.
    ```
       show <appname> nxsdk state
       show <appname> nxsdk ?
       Note: Run the state command to verify if the App is started successfully. Custom configs created using NX-SDK 
             by an Application always automatically start/prefix with <appname> followed by the config syntax. 
             Similarly, custom show commands created always start as "show <appname>" followed by the syntax. 
             Automatic addition of <appname> is done in order to create unique custom commands for applications
             and not to overwrite existing commands in the box.
    ```
    
## 8. Stop Custom Application in Switch
  - VSH
  
    ```
      ! IF the application is installed in the default location /isan/bin/nxsdk
      switch(config)# no nxsdk service-name <app-name>
      
      ! If the application is installed in any other location 
      switch(config)# no nxsdk service-name <app-full-path/app-name>
    ```
      
  - BASH
  
    ```
      switch(config)# <appname> stop-event-loop
      NOTE: stop-event-loop is a auto-generated CLI for APPs started from BASH. This is to stop the event loop and to quit the App gracefully.
    ```
    
  - To remove all NxsdK Services
    
    ```
      no feature nxsdk
    ```
    
## 9. Remove Custom Application from switch
  - To uninstall the RPM package from switch,
  - <b> VSH </b>
    ```
      ! To deactivate an already active RPM package using,
      switch# install deactivate <app-rpm-package>
      
      ! Verify if the RPM package is deactivated using,
      switch# show install inactive
      
      ! To remove the RPM package use,
      switch# install remove <app-rpm-package>.rpm
    ```
    
  - <b> BASH </b>    
    ``` 
      yum remove <app-full-path>
    ```

## 10. Troubleshoot
  - Each API defined throws an exception in case of any errors.
  - Refer to the generated doxygen and usage for each API and the different exceptions they throw. 
  - Best practice is to verify your Application by running it in BASH first and then integrate your App in VSH
    for seamless integration into NXOS along with other Nexus native applications.
  - [Only in v1.0.0] If an erroneous application (ex syntactical errors, crashes at startup etc) is started from VSH then that
    application will be blocked for sometime (approx 15mins). Until the application is unblocked, no operation
    can be performed on the blocked application. Hence, a blocked application cannot be removed (or) another 
    instance of the blocked application cannot be started.
    
    ```
      switch(config)# nxsdk service-name /isan/bin/pkgMgmt
      switch(config)# sh nxsdk internal service 

      NXSDK Started/Temp unavailabe/Max services : 2/0/32
      NXSDK Default App Path         : /isan/bin/nxsdk
      NXSDK Supported Versions       :  1.0 

      Service-name              Base App        Started(PID)      Version    RPM Package
      ------------------------- --------------- ----------------- ---------- ------------------------
      /isan/bin/pbwMonitor      nxsdk_app1      VSH(28161)        1.0        pbwMonitor-1.0-7.0.3.I6.1.x86_64
      /isan/bin/pkgMgmt         nxsdk_app2      VSH(not running)  1.0        pkgMgmt-1.0-7.0.3.I7.1.x86_64 (Blocked)
      
      switch(config)# no nxsdk service-name /isan/bin/pkgMgmt
      % Err: /isan/bin/pkgMgmt has failed to start (or) register with NxSDK, hence any operation w.r.t App is
             blocked(15mins from start attempt). Test the App in BASH before starting in VSH
    ```

## 11. Sample Custom Applications created using NX-SDK

Refer to [example NX-SDK applications](examples/README.md) for more details.

NOTE: All sample application RPMs are located at [RPMS](rpm/RPMS).
  
### Sample Python Application created using NX-SDK:
    ```
      pbwMonitor: To Monitor Port Tx & Rx Bandwidth Utilization %

      switch(config)# sh run nxsdk 

      !Command: show running-config nxsdk
      !Time: Sat Apr 29 02:25:29 2017

      version 7.0(3)I6(1)
      feature nxsdk

      nxsdk service-name /isan/bin/pbwMonitor


      switch(config)# sh nxsdk internal service 

      NXSDK total services (Max Allowed) : 1 (32)
      NXSDK Default App Path         : /isan/bin/nxsdk
      NXSDK Supported Versions       :  1.0 

      Service-name              Base App        Started(PID)    Version    RPM Package
      ------------------------- --------------- --------------- ---------- --------------------
      /isan/bin/pbwMonitor      nxsdk_app1      VSH(3459)       1.0        pbwApp-1.0-7.0.3.I6.1.x86_64
      
      switch(config)# sh pbwMonitor nxsdk state 

      Custom App State infomration
      --------------------------------
      App Name                       : pbwMonitor(NXOS Port BW Percentage Utilzation Python App)
      Nexus Mapped App Name          : nxsdk_app1
      Uuid                           : 1379 (VSH)
      Sup State                      : Active
      Start Resason                  : switchover
      Start State                    : Stateless

      Custom CLI Cmd State infomration
      --------------------------------

      Name                           : pbwMonitor_show_port_bw_util_cmd
      Syntax                         : show pbwMonitor port bw utilization [<port>]
      Mode                           : Show
      State                          : ADDED_TO_PARSER

      CLI Parser Init                : Yes
      CLI Parser Err                 : Registered with NX CLI Parser
      CLI Callback Handler           : Registered

      Custom HA infomration
      --------------------------------
      Hot Standby                    : Up
      Recovery                       : Done

        DB                             | Status     | Record#
        ---------------------------------------------------------
        runtime_data                   | In-Use     | 0
        runtime_config                 | In-Use     | 0
        startup_config                 | In-Use     | 0

      switch(config)# sh pbwMonitor port bw utilization ?
      *** No matching command found in current mode, matching in (exec) mode ***
      <CR>          
      >             Redirect it to a file
      >>            Redirect it to a file in append mode
      ethernet      Ethernet IEEE 802.3z
      loopback      Loopback interface
      port-channel  Port Channel interface
      |             Pipe command output to filter

      switch(config)# sh pbwMonitor port bw utilization 
      Interface       Bw (Rx/Tx Sec Rate)  Tx_BW percentage   RX_BW percentage  
      --------------- -------------------- ------------------ ------------------
      Ethernet1/1     10 Gbps (30/30)                   10.00               17.23
      Ethernet1/2     10 Gbps (30/30)                   15.00               25.20
      .........................
      Ethernet1/51    40 Gbps (30/30)                    5.00               1.00
      Ethernet1/52    40 Gbps (30/30)                    2.00               0.00

      switch(config)# sh pbwMonitor port bw utilization e1/1
      Interface       Bw (Rx/Tx Sec Rate)  Tx_BW percentage   RX_BW percentage  
      --------------- -------------------- ------------------ ------------------
      Ethernet1/1     10 Gbps (30/30)                   9.00               15.23
    ```

## 12. Helpful Notes
   - Ensure that the name of the custom application does not collide with any existing Native Nexus applications.
   - For custom applications to be started in VSH, please name the application as one word containing alphabets. 
   - ENXOS Built Environment is Mandatory for Custom Applications to be started in VSH.
   - ENXOS SDK docker image already has NX-SDK v1.0.0 installed in (/NX-SDK). To get latest versions do a git pull.
   - On Dual-Sup make sure that RPM package is installed on both Active and Standby Supervisors. If not installed on standby, then on 
     system switchover, custom Applications may not be started.
   - An NX-SDK csutom Application can be started from VSH only if it is installed through RPM Package.
   - For NX-SDK Python application to be started in VSH, place ```#!/isan/bin/nxpython``` in the first line of python application.
   - For NX-SDK Python application to be started in BASH, use ```/isan/bin/python``` to run the application as it sets the 
     necessary environment needed to run python Apps in BASH.    
   - Perform all NX-SDK related initializations in one thread. Refer to sample example custom applications for sample template and usage.
     
</content>

  <tabTrigger>readme</tabTrigger>
</snippet>
