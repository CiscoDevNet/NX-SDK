<snippet>
  <content>
  
#NX-SDK

# Table of Contents

  - [Cisco NX-OS SDK](#cisco-nxos-sdk)
  - [Documentation](#documentation)
  - [Usage](#usage)
  - [Release](#release)
    * [Version V1.0](#version-v10)
    * [Future Versions](#future-versions)
  - [Custom Application Development using NX-OS SDK Requirements](#custom-application-development-using-nxos-sdk-requirements)
  - [Custom Application Development Flow using NX-OS SDK](#custom-application-development-flow-using-nxos-sdk)
    * [1. Install NX-OS SDK](#1-install-nxos-sdk)
      + [a) ENXOS SDK Build Environment [Optional]](#a-enxos-sdk-build-environment-optional)
      + [b) Get NX-OS SDK toolkit](#b-get-nxos-sdk-toolkit)
    * [2. Building Custom Applications](#2-building-custom-applications)
    * [3. Unit Testing Custom Application](#3-unit-testing-custom-application)
    * [4. Packaging Custom Application](#4-packaging-custom-application)
    * [5. Installing Custom Application in Switch](#5-installing-custom-application-in-switch)
    * [6. Running Custom Application in Switch](#6-running-custom-application-in-switch)
    * [7. Verify if Custom Application is running](#7-verify-if-custom-application-is-running)
    * [8. Stop Custom Application in Switch](#8-stop-custom-application-in-switch)
    * [9. Remove Custom Application from switch](#9-remove-custom-application-from-switch)
    * [10. Troubleshoot](#10-troubleshoot)
    * [11. Sample Python Application created using NX-SDK:](#11-sample-python-application-created-using-nxsdk-)


# Cisco NX-OS SDK

Cisco NX-OS SDK provides a simple, flexible and powerful tool for off the box third party custom Application development to gain access to Nexus infra 
functionalities which when run inside the Nexus switches allow the custom applications to run natively just like any other Cisco native Nexus applications.
It is appropriate for Do-it-Yourself Automation to develop custom applications to fit your needs. NX-OS SDK offers various functionalities like ability to 
generate custom CLIs, Syslogs, Event manager, Inter-Application communication, HA, Route Manager and much more.

NX-OS SDK provides Abstraction/plugin Library Layer thereby decoupling the Application from underlying infra being used.
Hence, its easy and simple to change infra without affecting the applications. Hence, NX-OS SDK can be used for
developing native Cisco Applications as well. 

It is built using C++ language. Other language (python, go, ruby etc) bindings will also be provided for NX-OS infra SDK hence custom applications 
can be developed and built in any language of users choice.

This github provides NX-OS SDK toolkit for custom application development in your favorite Linux environment to gain access to NX-OS infra.

# Documentation

For detailed description and directory structure of Cisco NX-OS Infra SDK toolkit, refer to description.md

# Usage

Detailed usage of NX-OS SDK:
  - For C++ custom Application development, public Classes and APIs to be used are available in <a href="http://gitlab.cisco.com/sathsrin/nxsdk/tree/master/include">include/</a>. Its description and usage are available 
    in <a href="http://gitlab.cisco.com/sathsrin/nxsdk/blob/master/doc/html/annotated.html">doc/html/annotated.html</a> (doxygen generated). Please do a git pull and open
    the doxygen generated html page. Refer to <a href="http://gitlab.cisco.com/sathsrin/nxsdk/tree/master/examples">examples/</a> for sample C++ Apps.
  - Python and other language bindings will be auto-generated from the above classes and APIs using SWIG. 
    Only Python language bindings for NX-OS SDK will be supported in Version 1.0
    Refer to <a href="http://gitlab.cisco.com/sathsrin/nxsdk/tree/master/python/examples">python/examples</a> for python Apps leveraging the NX-OS SDK functionalities for Python custom App development.

# Release 

## Version V1.0
  - NX-OS Infra SDK will be first available on N9K switches as part of F release.
  - Ability to develop C++ and Python custom application using NX-OS SDK.
  - Ability for custom applications to generate custom CLIs (config and show commands).
  - Ability to generate custom Syslogs, Events & Error history messages.
  - Ability to start/stop C++ and Python custom Applications from VSH just like any other native Nexus applications (like feature bgp)
    thereby providing HA capability for custom applications. 
  - C++ and Python NX-OS infra SDK custom Application development toolkit.

## Future Versions
  - Go and Ruby custom Application development toolkit.
  - Event manager (state change like Next-hop change, interface up/down, Rib route manager, DME events & more to custom application to take action based 
    on registered events)
  - Abstract Inter-process communication Infra.
  - Program and process Control and Data Plane Policies (ex Apps: DPI apps, TCP analytics and some other analytics App)
  - High Availability
  - Security

# Custom Application Development using NX-OS SDK Requirements
  - Linux (Currently supported platforms)
  - ENXOS SDK (For best results, build the App using NX-OS SDK as RPM package in ENXOS SDK. 
    Docker container with ENXOS SDK will be provided).
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
    ```        

#  Custom Application Development Flow using NX-OS SDK

## 1. Install NX-OS SDK

### a) ENXOS SDK Build Environment [Optional]

  - NOTE: Mandatory for Custom Apps to be started in VSH.
  - ENXOS SDK Build Environment can be obtained from https://hub.docker.com/r/dockercisco/nxsdk
    Pull the Container version of your choice using.

    ```
      docker pull dockercisco/nxsdk
    ```
    
  - Source 32-bit environment for your application 
 
    ```
      export ENXOS_SDK_ROOT=/enxos-sdk
      cd $ENXOS_SDK_ROOT
      source environment-setup-x86-wrsmllib32-linux 
    ```   

### b) Get NX-OS SDK toolkit
  - git clone NX-OS SDK.

    ```
      git clone https://github.com/CiscoDevNet/NX-SDK.git
      
      This will create,
         $PWD/NX-SDK
         ls (Makefile  README.md  doc  examples  include  python  rpm  src  stubs)
    ```
  - Refer to API usage and sample applications and develop your application based on your needs.

## 2. Building Custom Applications
  - C++ Application 
    - To build C++ custom application, add your application to the <a href="https://github.com/CiscoDevNet/NX-SDK/blob/master/Makefile">Makefile</a> just like example apps <a href="https://github.com/CiscoDevNet/NX-SDK/blob/master/examples/customCliApp.cpp">(example/customCliApp.cpp)</a>
      and follow the instructions in the Makefile.
    - Make sure the App builds without any errors using

      ```
        $PWD/NX-SDK# make clean
        $PWD/NX-SDK# make all
      ```
  - Nothing to build for Python Applications.      

## 3. Unit Testing Custom Application
  - For simple testing of the custom application, copy the application (binary) to the switch.
  - Start the custom application from BASH (step 6) for simple testing.
  - Once the custom application has been tested and verified in BASH then proceed to step (4) to start the application from VSH.
 
## 4. Packaging Custom Application

  - Needed for custom application to be started from VSH inorder to persist on switch reloads or system switchover,
    Write the respective <app>.spec to build an RPM package for your App.

    ```
      export RPM_ROOT=$ENXOS_SDK_ROOT/sysroots/x86_64-wrlinuxsdk-linux/usr/src/rpm
      ls $RPM_ROOT (BUILD  RPMS  SOURCES  SPECS  SRPMS)
      vi $RPM_ROOT/SPECS/<app>.spec
    ``` 
    Refer to the <a href="https://github.com/CiscoDevNet/NX-SDK/blob/master/rpm/SPECS/customCliApp.spec">rpm/SPECS/customCliApp.spec</a> file for the sample App <a href="https://github.com/CiscoDevNet/NX-SDK/blob/master/examples/customCliApp.cpp">examples/customCliApp.cpp</a>
    
  - Follow the instructions mentioned in the <a href="https://github.com/CiscoDevNet/NX-SDK/blob/master/rpm/SPECS/customCliApp.spec">rpm/SPECS/customCliApp.spec</a> to build a spec file for your Application.
  - To build an RPM package use
 
    ```
      rpm -ba $RPM_ROOT/SPECS/<app>.spec
    ```
  - If RPM build was successful then RPM file will be generated in 
    
    ```
      ls $RPM_ROOT/RPMS/x86_64/
    ```
  - NOTE: Soon a script will be provided to auto-generate RPM package for custom applications.

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
    (Recommended to place the app in /isan/bin. Follow the example spec file)

    ```
      ls <app-full-path>
    ```
  - NOTE: On Dual-Sup make sure that RPM package is installed on both Active and Standby Sups. If not 
          installed on standby, then on system switchover, Apps may not be started.
          

## 6. Running Custom Application in Switch
  - To run the custom Application using NxSDK in the switch, as a prerequisite enable 
 
    ```
      switch(config)# feature nxsdk
    ```   
  - To run C++ Custom Application from 
    - VSH
  
       ```
         switch(config)# nxsdk service-name <app-full-path> 
         (by default this will be /isan/bin/<app>)
         
         NOTE: An Application can be started from VSH only if it is installed through RPM Package. 
       ```
       
    - BASH
      ```
        switch(config)# run bash sudo su
        
        bash# <app-full-path> &
      ```
  - To run Python Custom Application
    - VSH
    
      ```
        switch(config)# nxsdk service-name <app-full-path> 
        NOTE: Python app should be made as executable to be started from VSH.
              chmod +x <app-full-path> 
              Place, #!/isan/bin/nxpython in the first line of python application.
              /isan/bin/nxpython should be used to run NXOS Infra SDK Python Apps.
      ``` 
      
    - BASH
    
      ```
        switch(config)# run bash sudo su
        
        bash# /isan/bin/python <app-full-path> &
        
        NOTE: To run the python App using NX-OS SDk in BASH, use /isan/bin/python to run the app as it sets the 
              necessary environment needed to run python Apps in BASH.
      ```

## 7. Verify if Custom Application is running
  - Check if your service is running by
    
    ```
      show nxsdk internal service
      
      switch(config)# sh nxsdk internal service 

      NXSDK total services (Max Allowed) : 2 (32)
      NXSDK Default App Path         : /isan/bin/nxsdk
      NXSDK Supported Versions       :  1.0 

      Service-name              Base App        Started(PID)    Version    RPM Package
      ------------------------- --------------- ------------    ---------- --------------------
      /isan/bin/capp1           nxsdk_app2      VSH(25270)      1.0        capp1-1.0-7.0.3.I6.1.x86_64
      /isan/bin/TestApp.py      nxsdk_app3      BASH(27823)     -          -    
      
      NOTE: App is running if the above command shows a valid PID.
    ```   
    
    - When custom Application is started, by default a few show commands are auto-generated for the App.
    ```
       show <appname> nxsdk state
       show <appname> nxsdk ?
       Note: Run the state command to verify if the App is started successfully.
    ```
    
## 8. Stop Custom Application in Switch
  - VSH
  
    ```
      switch(config)# no nxsdk service-name <app-full-path> 
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
      switch# install remove <app-rpm-package>
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

## 11. Sample Python Application created using NxSDK:
  - pbwApp: To Monitor Port Tx & Rx Bandwidth Utilization %
    ```
      switch(config)# sh run nxsdk 

      !Command: show running-config nxsdk
      !Time: Sat Apr 29 02:25:29 2017

      version 7.0(3)I6(1)
      feature nxsdk

      nxsdk service-name /isan/bin/pbwApp


      switch(config)# sh nxsdk internal service 

      NXSDK total services (Max Allowed) : 1 (32)
      NXSDK Default App Path         : /isan/bin/nxsdk
      NXSDK Supported Versions       :  1.0 

      Service-name              Base App        Started(PID)    Version    RPM Package
      ------------------------- --------------- --------------- ---------- --------------------
      /isan/bin/pbwApp          nxsdk_app1      VSH(3459)       1.0        pbwApp-1.0-7.0.3.I6.1.x86_64
      
      switch(config)# sh pbwApp nxsdk state 

      Custom App State infomration
      --------------------------------
      App Name                       : pbwApp(NXOS Port BW Percentage Utilzation Python App)
      Nexus Mapped App Name          : nxsdk_app1
      Uuid                           : 1379 (VSH)
      Sup State                      : Active
      Start Resason                  : switchover
      Start State                    : Stateless

      Custom CLI Cmd State infomration
      --------------------------------

      Name                           : pbwApp_show_port_bw_util_cmd
      Syntax                         : show pbwApp port bw utilization [<port>]
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

      switch(config)# sh pbwApp port bw utilization ?
      *** No matching command found in current mode, matching in (exec) mode ***
      <CR>          
      >             Redirect it to a file
      >>            Redirect it to a file in append mode
      ethernet      Ethernet IEEE 802.3z
      loopback      Loopback interface
      port-channel  Port Channel interface
      |             Pipe command output to filter

      switch(config)# sh pbwApp port bw utilization 
      Interface       Bw (Rx/Tx Sec Rate)  Tx_BW percentage   RX_BW percentage  
      --------------- -------------------- ------------------ ------------------
      Ethernet1/1     10 Gbps (30/30)                   10.00               17.23
      Ethernet1/2     10 Gbps (30/30)                   15.00               25.20
      .........................
      Ethernet1/51    40 Gbps (30/30)                    5.00               1.00
      Ethernet1/52    40 Gbps (30/30)                    2.00               0.00

      switch(config)# sh pbwApp port bw utilization e1/1
      Interface       Bw (Rx/Tx Sec Rate)  Tx_BW percentage   RX_BW percentage  
      --------------- -------------------- ------------------ ------------------
      Ethernet1/1     10 Gbps (30/30)                   9.00               15.23
    ```

</content>
  <tabTrigger>readme</tabTrigger>
</snippet>
