# Deploying NX-SDK Applications Remotely

# NX-SDK Supported Version
  - NX-SDK V2.0.0 (refer to [versions](../versions.md) for more details)
  
# Table of Contents

   - [Description](#description)
   - [From NX-SDK V2.0.0](#from-nx-sdk-v2-0-0)
      - [Remote API support](#remote-api-support)
      - [NX-SDK Docker Remote Container](#nx-sdk-docker-remote-container)
      - [Supported Languages](#supported-languages)
      - [How to start NX-SDK application remotely](#how-to-start-nx-sdk-application-remotely)
      - [Build](#build)
      - [Run](#run)
      - [High Availability(HA)](#high-availability(ha))
      - [Limitations and Restrictions](#limitations-and-restrictions)
      - [Securing Remote Connections with TLS](#securing-remote-connections-with-tls)
      - [Troubleshooting](#troubleshooting)
      - [Notes](#notes)

# Description

  Prior to Remote API support, NX-SDK applications can be developed and built off the NX-OS switch using the NX-SDK NX-OS 
  Docker image <b>but can run only inside the NX-OS switch (on box)</b>. Now with Remote API support, NX-SDK applications 
  can run <b>anywhere</b> (on (or) off box, in a docker container or even in the cloud).

# From NX-SDK V2.0.0

  - Remote API Support for NX-SDK applications is introduced in NX-SDK V2.0.0.
  - NX-SDK apps (prior to NX-SDK V2.0.0) can be developed and built off the NX-OS switch using the NX-SDK NX-OS Docker image 
    but can run only inside the NX-OS switch. 
  - <b>Pros</b> of this approach
    - NX-SDk applications are also part of NX-OS just like any other Native NX-OS applications thereby giving 
      the same look and feel as a Native NX-OS application.
    - Performance factor. Its much faster as its running inside NX-OS itself.
  - <b>Cons:</b>
    - Dependency on NX-OS WRL toolchain. To run any apps in NX-OS it needs to be cross compiled with its WRL toolchain. 
    - Security concerns. Even though NX-SDK APIs are well defined and restrictive but the apps are tied to NX-OS 
      resources. 

## Remote API support

  - With NX-SDK Remote Api support, <b><i>"same NX-SDK apps"</b></i> can not only be developed and built Off the NX-OS 
    switch but can also <b><i>run outside of NX-OS</b></i>.
  - With Remote API support, NX-SDK apps can run <b>anywhere</b> inside or outside NX-OS, in a docker container         
    (or) even in the cloud. 
  - Hence, with NX-SDK Remote API support NX-SDK apps can be built as micro services and not tied to NX-OS.
  - <b>Pros</b> of this approach
    - Not dependent on NX-OS WRL toolchain. Apps need not be cross compiled against NX-OS WRL toolchain. 
    - Apps can be built as 64 bit apps.
    - NX-SDK APIs and its usage are the same irrespective of where its used (on box (or) remotely).
    - Since apps are running outside the switch thereby securing NX-OS and puts less load on NX-OS.
    - Provides the ability to connect to multiple NX-OS switches from a single remote application.
    - Now, NX-SDK apps can run as microservices.
  - <b>Cons:</b>
    - Lower Performance compared to running inside NX-OS.
  - <b>NOTE: </b> Not all NX-SDK APIs are remote capable. Refer to API documentation for more details. If an API is not remote
             capable then there will be details doxygen tag for the same. If there are no details tag then that API is remote 
	     capable. If a non-Remote capable API is used/called by a remote NX-SDK application then it throws an exception. 
	     Hence for the same NX-SDK application to run on box (or) remotely, the application must handle these exceptions
	     gracefully. 

## NX-SDK Docker Remote Container
   
   - To run NX-SDK application remotely, get the NX-SDK Remote docker image from 
     ```
      docker pull dockercisco/nxsdk-remote:<tag>
      Ex)    docker pull dockercisco/nxsdk-remote:v1
     ```
   - NOTE: Refer to [NX-SDK Docker readme](../readmes/NXSDK_docker.md) for more information on docker images provided by 
	   NX-SDK and which ones to use based on where the application will be deployed (on box or remotely).  
   - Remote Docker image is built with the following additional packages like (python, gcc, g++, git, go, vim etc)
   - Pre-built Dependencies: 
     - Grpc & protobuf pre-built C++ 64 bit libraries
     - rapidjson
     - Remote NX-SDK 64bit library (libnxsdk.so)
     
   - To run the docker container use
     ```
     docker run -it <image-id> /bin/bash
     NOTE: Get image-id from "docker images"
     ```
   - Remote project is available as part of NX-SDK git repo.
     ```
     ! This will clone the latest NX-SDK .
     git clone https://github.com/CiscoDevNet/NX-SDK.git
	  
     ! /NX-SDK is already downloaded in the remote docker image.
     cd /NX-SDK
       
     ! Get the latest content by using
     git pull
     ```
     
   - The necessary environment variables are already set in the container
     ```
     NXSDK_ROOT=/NX-SDK
     LD_LIBRARY_PATH=/NX-SDK/remote/libs
     ```
   - Now you have the environment ready to develop NX-SDK remote applications.

## Supported Languages

   Supported languages for remote NX-SDK applications are:
  - C++
  - Python
  - Go (pre EFT/Beta)
  
## How to start NX-SDK application remotely
      
  - To start an NX-SDK application "dupHostMonitor" remotely in the NX-SDK remote Docker container, the first step 
    is to start the NX-SDK lightweight server in NX-OS.
        
  - <b>NX-SDK remote server:</b> 
    - To start the NX-SDK lightweight server in NX-OS use the following CLI
      ```
      switch-config)# feature nxsdk
      switch-config)# nxsdk remote port 50002 namespace management
      switch-nxsdk-remote-config)# service-name dupHostMonitor
            
      NOTE: This CLI starts an NX-SDK lighweight server in the name of "dupHostMonitor" accepting remote connections
            in the port "50002" in the namespace/vrf "management". Hence the NX-SDK app running remotely should have 
	    the same name as defined in the server. 
      ```
    - This CLI is defined to ensure that only admin (person with right priviledges) of the switch can allow/control 
      the remote NX-SDK connections and also the NX-SDK applications that can run remotely.
          
  - <b>NX-SDK remote client library:</b>
    - For NX-SDK applications to run remotely, applications need to link to the pre-built remote [libnxsdk.so](./libs/libnxsdk.so)
      and not to the stub [libnxsdk.so](../libs/libnxsdk.so). Refer to [Build](#build) section for more details.
    - First step for NX-SDK application is to get the SDK instance using <b><i>getSDkInst</b></i> Api (gateway to gain access 
      to NX-OS infra).
    - For NX-SDK applications to run remotely, it needs to know the <b>NXOS Switch IP and the Port</b> on which the 
      server is running in order to establish the connection & it needs to be reachable from the remote conainter.
      This can be done in two ways.
            
      1) Using Extended getSdkInst API approach:
         - In V2.0.0, getSdkInst API had been extended to take remote parameters like (switch IP and port information) 
           in order to establish remote connection with the NX-SDK server running in the NXOS switch. Refer to API 
	   documentation for more information on this API and its usage.
         - With this Api, a remote NX-SDK application can connect to multiple switches by specifying different IP 
           addresses and port respectively thereby acquiring SDK instances per switch that they are connected to. 
           Each remote SDK instance represents the switch they are connected to.
           ```
           Python:                                                                               
              import nx_sdk_py 
              sdk_switchA = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True, "172.22.32.1", 50001)              
              sdk_switchB = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True, "172.22.32.2", 50001)
           ```
         - Useful when remote applications connect to multiple switches from the same application. 
         - NOTE: This requires your NX-SDK application to be changed to use the new API. IF this API is used in
                 the application running inside NX-OS (on box) itself then its a no-op ignoring the remote parameters
                 and is same as calling just getSdkInst() without the remote parameters
         - With this you can run <b>one centralized remote NX-SDK application on multiple switches</b>. 
             
      2) Using Environment Variable approach:
         - If the user wants to run the same NX-SDK application running in the switch remotely without having to 
           make any changes in the application they can do so by setting the following environment variables in 
           the NX-SDK remote container.   
           ```
           export NXSDK_DEST_SWITCH_PORT=50001 export NXSDK_DEST_SWITCH_IP=172.22.2.1	   
           ```           
           NOTE: As the apps running inside the switch uses getSdkInst API without remote parameters which will be
                 obtained from the above environment variables.
           
         - This approach is useful if you don’t want to change your NX-SDK application (note: It depends on how well
	   the application is written to handle error cases gracefully as not all APIs are remote capable) and if you 
	   are connecting to only one switch meaning if you want to run your app in only one switch.
    - NOTE: NX-SDK Remote connection over IPv6 address is pre-EFT/Beta for v2.0.0
    - Refer to sample remote applications     
  - GRPC is used as transport between NX-SDK Server and Client library.   
    
## Build
  - NX-SDK C++ & Go remote applications are built the same way as done for building on-box NX-SDK applications and 
    also there is no difference in NX-SDK APIs and its usage if the application is run on box (or) remotely.
  - NX-SDK <b>Makefiles</b> have been updated so that depending on which docker container (NX-SDK remote vs NX-OS) 
    the applicatios are built and links to the respective libnxsdk library (remote vs stubs).
  - To build C++ NX-SDK application refer to instructions in [Makefile](../Makefile)
  - To build GO NX-SDK application refer to instructions in [go/Makefile](../go/Makefile)
  - Just edit the respective Makefile to add your application to the build list.

## Run
  - For C++ & GO remote NX-SDK applications, once the application binary is available just run the binary.
  - For python remote NX-SDK applications just run "python <app>"

## High Availability(HA)
  - <b>NX-SDK Server in the switch</b>
  
    - NX-SDK server running in the switch servicing remote NX-SDK application is a <b>stateless</b> process. Meaning the 
      server connection will reset on NX-OS switch reloads, System switchover, server restarts & other network issues. 
      It then re-establishes the connection with the remote client once the respective NX-SDK server is up.
    - <b>Hence NX-SDK server does not maintain any state and the onus is on the remote client application to handle HA and 
      push the previous state if the server gets restarted.</b>  
    - Refer to postRemoteConnDownCb & postRemoteConnUpCb in [nx_sdk.h](../include/nx_sdk.h) and api documentation for more 
      details. As these Cbs are defined to indicate the remote client that the server connection going down & up & vice-versa.
    - Remote clients to use this trigger to purge any old instances and to re-establish its previous state. 
    - Refer to [dupHostMonitor](../readmes/dupHostMonitor_remote_NXSDK.md) sample remote app on the usage.
    
  - <b>NX-SDK Remote Client application</b>
  
    - For graceful cleanup, the guideline is for remote applications to delete the sdk instance before quiting so that
      server gets cleaned up gracefully.
    - If the remote application did not do a graceful cleanup (crash or forceful kill) then it takes a few seconds for the
      server to cleanup its state. Until then new instances of the same remote application will not be allowed to connect 
      until the server has undergone its cleanup for the previous instance of the remote application.
    
## Securing Remote Connections with TLS
NX-SDK remote connections are secured & authenticated using the TLS protocol.  Currently there is no option to use clear-text connections, so having a working TLS configuration is required in order to connect an application to remote services on the switch.  In order to simplify getting remote apps up and running quickly, there are two choices for TLS configuration:
- auto-generated temporary certificate and key
- user installed certificate and key (Recommended).

In both cases, the server certificate and key are specified in a nxsdk configuration file.  There is a read-only default configuration file at /etc/nxsdk_default.cfg, and it contains the paths to the temporary certificate and key.  If the user wishes to install a custom certificate and key, the default configuration file can be copied into /etc/nxsdk.cfg (or) get nxsdk.cfg from /NX-SDK/remote, and modified to contain the paths to the custom certificate and key.  The temporary certificate should be used for testing purposes only, and a proper certificate used for production applications.

### Auto-generated Certificate/Key
Each time the SUP becomes active, a new temporary certificate and key is generated.  This means the first time the switch boots, or when a System Switchover (SSO) occurs and the standby SUP becomes active, a new certificate and key is created.  The certificate is valid for only 24 hours.  After this time, the user must create a new temporary certificate/key (by reload or SSO) or install their own certificate and key.  NOTE: Existing connections will not be closed beyond this 24 hour window, however new connections beyond the expiration time will be rejected.

The generated certificate and key are found at:
- /var/volatile/tmp/nxsdkTmpCert.pem -- temporary public certificate
- /var/volatile/tmp/nxsdkTmpKey.key  -- temporary private key

### User installed Certificate/Key (Recommended)
A user may also install their own certificate and key pair obtained obtained from a trusted source.  This can be either a public CA, a private CA, or using a tool such as openssl to generate a self-signed certificate.  The certificate must contain a name (CN) for the device, and which clients will connect to.  This CN is used by the client to verify the device being connected matches the owner name in the server certificate.  
 
### Service (Switch-side) Configuration
In order to make a TLS connection, the server must have a PEM encoded X.509 certificate and matching private key.  Which certificate and key to use is controlled by a configuration file.  The read-only default configuration file is located at /etc/nxsdk_default.cfg, and contains the paths to the auto-generated temporary certificate and key.  If using the auto-generated certificate and key, the default configuration is already set to point at the proper locations.  When a new service is started, the configuration file is read to obtain the locations of the desired certificate and key, and the service will accept TLS connections using those parameters.  Once a client connects to a service, changing the certificate and key has no effect on existing connections.  If the configuration is changed, the service must be stopped and restarted in order to pick up the new certificate and key for new connections.  In order to use a new certificate and key, copy the /etc/nxsdk_default.cfg into a new file /etc/nxsdk.cfg (or) get nxsdk.cfg from /NXSDK/remote, and modify the following values to reflect the correct location:

<b>[grpc]
	
cert=/path/to/cert

key=/path/to/key </b>

Executing "copy r s" will save the new configuration file on the Active SUP, and copy the configuration along with the certificate and key to the Standby SUP if available.  In the case of user installed certificate and key, it is up to the switch administrator to ensure that the correct certificate and key are copied to both SUPs and saved.  As previously mentioned, the auto-generated certificate and key are created at runtime by the Active sup so there is no manual copying required on the switch.

NOTE: This step will be simplified in future releases.

### Client Configuration
The client needs the following TLS related information when making the TLS connection to the remote service on the switch:
- the signing certificate containing the public key (.pem) that signed the service side certificate.  This could be the CA certificate, or the same certificate as the server side in the case of a self-signed certificate
- the expected CN of the switch

The administrator must provide a copy of the correct certificate to the client to be used during the handshake verification process.  This is true for both temporary certificates and user installed certificates. The client uses the following ordered options to determine which certificate it will use:
 1. cert path passed into this function from getSdkInst()
    ```
       ! To be used if each switch has its own different certificate.
       
       ! Needed if CN Value is different from ems.cisco.com in the certificate generation
       export NXSDK_GRPC_HOST_OVERRIDE=<CN value>
       
       import nx_sdk_py                                                                 
       sdk_switchA = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True, "172.22.32.1", 50001, "/path/to/nxsdkCert_172_22_32_1.pem")              
       sdk_switchB = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True, "172.22.32.2", 50001, "/path/to/nxsdkCert_172_22_32_2.pem")
    ```
 2. cert specified in env var $NXSDK_SERVER_CERT_A_B_C_D where
    A_B_C_D is the ip of the switch. If a different certificate needs to be used per switch.
    ```
       ! To be used if each switch has its own different certificate
       ! Another way to achieve step (1) by using env varaibles instead of apis.
      
       ! Needed if CN Value is different from ems.cisco.com in the certificate generation
       export NXSDK_GRPC_HOST_OVERRIDE=<CN value>
       ! Certificate to be used for switch with ip 172.22.32.1
       export NXSDK_SERVER_CERT_172_22_32_1=/path/to/nxsdkCert_172_22_32_1.pem 
       ! Certificate to be used for switch with ip 172.22.32.2
       export NXSDK_SERVER_CERT_72_22_32_2=/path/to/nxsdkCert_172_22_32_2.pem      
    ```
 3. cert specified in env var $NXSDK_SERVER_CERT. 
    ```
       ! To be used if all the switches are using the same certificate

       ! Needed if CN Value is different from ems.cisco.com in the certificate generation
       export NXSDK_GRPC_HOST_OVERRIDE=<CN value>
  
       !!! Common certificate to be used to connect to all the NX-OS switches
       export NXSDK_SERVER_CERT=/path/to/nxsdkCert.pem
    ```
 4. temp cert "nxsdkTmpCert.pem" in current directory of application
    ```
       ! To be used if you are connecting to only one remote switch and using its tmp certificate.
       ! name the certificate as nxsdkTmpCert.pem. NX-SDK will
       ! automatically use this and no need to set any env variables.   
    ```
 5. cert "nxsdkServerCert.pem" in current directory of application
    ```
       ! To be used if all the switches are using the same certificate.
       ! name the certificate as nxsdkServerCert.pem. NX-SDK will
       ! automatically use this and no need to set any env variables.
       
       ! Needed if CN Value is different from ems.cisco.com in the certificate generation
       export NXSDK_GRPC_HOST_OVERRIDE=<CN value>
    ```
 
If none of these find a cert, it is an error and the client will terminate.

The client makes a connection to the remote service by calling 'getSdkInst()' using the IP address and port of the remote service.  During the TLS handshake, the client verifies that the server it is connecting to matches the CN presented in the service certificate.  There are two ways the client can determine the remote device name (CN) for this purpose:
- provide a mapping of IP address to device name (DNS, /etc/hosts, etc.)
- provide the expected certificate CN directly via host override environment variable

For example, if the client is connecting to 192.168.10.3:50004, and the CN value in switch cert is "SwitchA-1", there may be an /etc/hosts entry like:
192.168.10.3    SwitchA-1
Now when the getSdkInst() call is made for this IP, the name is looked up and used to validate against what is presented in the certificate received from the device.
Another option is to directly give the expected value to the client through an environment variable:

export NXSDK_GRPC_HOST_OVERRIDE=SwitchA-1

This can also be used in the case of sharing a single certificate and key across multiple devices.  In this way, the client is explicitly given the expected CN value in the server's certificate. NOTE: The above step is not needed if ems.cisco.com is used for CN value in certificate generation process.

## Limitations and Restrictions
  - In v2.0.0, only one remote NX-SDK application can be attached to a NX-SDK server running in the NX-OS switch.
  - A Max of only 10 remote NX-SDK applications can run at a time in a NX-OS switch.
  - NX-SDK server accepts remote requests only from the CLI configured application and not from random applications.
  - Exceptions are thrown for errors.
  - Creating custom CLIs on the switch is not supported via remote API support. 
  - NX-SDK Remote connection over IPv6 address is pre-EFT/Beta for v2.0.0
  - Refer to API documentation for the APIs that are \[not\] remote Capable. 
  - Remote client library code is not open sourced at this point. Hence, NX-SDK remote apps need to run in the NX-SDK provided 
    remote docker container. To use remote NX-SDK in your own OS, then place a request in the git with your OS and compiler 
    version and we can generate remote libnxsdk.so based on your version.

## Troubleshooting 

  - To troubleshoot NX-SDK remote server issues:
    ```
       show <remote-app-name> stats detail
       show <remote-app-name> nxsdk event-history events
       show <remote-app-name> nxsdk event-history errors
       show <remote-app-name> nxsdk event-history internal libnxsdk
    ```
  - To troubleshoot NX-SDK remote client library issues:
    ```
       !!! In the remote docker container,
       export NXSDK_LIB_DEBUG=1
       !!! This will dump NX-SDK LIB debugs in the console.
       
       !!! To re-direct NX-SDK LIB debugs to a log file, 
       export NXSDK_LIB_DEBUG_FILE=<dir-path> 
       
       !!! This will help NX-SDK team to debug client side issues.
    ```
  - To troubleshoot your NX-SDK remote Application issues:
    ```
       !!! If your remote Application uses NxTrace::Event, Error, Syslog api the output is then redirected to respective 
       !!! files.
       /<app_name>_event_<pid>_event.txt
       /<app_name>_event_<pid>_error.txt
       /<app_name>_event_<pid>_syslog.txt
       
       !!! To redirect these files to a target location of your choice then use
       export NXSDK_TRACE_LOGDIR=<dir-path-to-log-these-files>
       
       !!! To dump your trace logs on the console screen,
       export NXSDK_TRACE_CONSOLE = 1 
    ```   
## Sample Remote NX-SDK application
  
   - [dupHostMonitor](../readmes/dupHostMonitor_remote_NXSDK.md)
     - An NX-SDK remote python application that connects to multiple switches specified in [switch_details.json](../examples/python/switch_details.json) and registers for adjacency events using NX-SDK NxAdjMgr Apis from all the
	registered switches.
     - It then anaylyzes the adjacency events from all the registered switches to identify the duplicate hosts
        in the network.
     - New Switches can be added & existing switches can be deleted on the fly to/from switch_details.json 
  
 
