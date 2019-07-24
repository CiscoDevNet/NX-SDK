<snippet>
  <content>
  
# NX-SDK 

# Table of Contents

  - [Cisco NX-SDK](#cisco-nx-sdk)
  - [Release](#release)
  - [Documentation and Usage](#documentation-and-usage)
  - [Description](#description)
  - [NX-SDK Deployment Modes](#nx-sdk-deployment-modes)
  - [Sample Custom Applications created using NX-SDK](#sample-custom-applications-created-using-nx-sdk)
  - [Notes](notes)

# Cisco NX-SDK

<p align="justify">
Cisco NX-SDK provides a simple, flexible, modernized and powerful tool for off the box third party custom Application development to gain access to Nexus infrastructure 
functionalities which when run inside (or) outside the Nexus switches allow the custom applications to run natively just like any other Cisco native Nexus applications.
It is appropriate for Do-it-Yourself Automation to develop custom applications to fit your needs thereby decoupling application development from Nexus releases. NX-SDK offers various functionalities like ability to 
generate custom CLIs, Syslogs, Event manager, HA, Route Manager, Streaming Telemetry and much more. 

NX-SDK provides Abstraction/plugin Library Layer thereby decoupling the Application from underlying infrastructure being used.
Hence, its easy and simple to change infra without affecting the applications. Hence, NX-SDK is being used for
developing native Cisco Applications as well. 

It is built using C++ language. Other language (python, go, ruby etc) bindings will also be provided for NX-SDK hence custom applications 
can be developed and built in any language of users choice. Starting from NX-SDK v2.0.0, NX-SDK applications can run anywhere (inside or outside of NX-OS).

This github provides NX-SDK toolkit for custom application development in your favorite Linux environment to gain access to NX-OS infrastructure.
</p>

# Release 

- Refer to [NX-SDK versions](versions.md) for more details on different versions and its supported features.
- Refer to <a href="https://github.com/CiscoDevNet/NX-SDK/releases">NX-SDK releases</a> to get older NX-SDK toolkit versions. 

# Documentation and Usage

  - For C++ custom Application development, public APIs are available in [include/](include/). Its description and usage are available 
    in <a href="https://htmlpreview.github.io/?https://github.com/CiscoDevNet/NX-SDK/blob/master/doc/html/annotated.html">API documentation</a> (doxygen generated). 
  - Python and other language bindings will be auto-generated from the above C++ APIs using <a href="http://www.swig.org/">SWIG</a>. Refer to [version](versions.md) for more details on supported language bindings in each NX-SDK version.
  - Refer to [example NX-SDK applications](examples/README.md) for more details.

# Description

For detailed description and directory structure of Cisco NX-SDK toolkit, refer to [description](description.md).

# NX-SDK Deployment Modes
  - Starting from [NX-SDK V2.0.0](versions.md), NX-SDK applications can be deployed anywhere (inside (or) outside 
    of NX-OS)
  - Depending on the use case & where you want your NX-SK application to be deployed:
  
## Inside NX-OS (on box)
   Refer to [steps to deploy NX-SDK applications in NX-OS](readmes/NXSDK_in_NXOS.md).
     
## Outside of NX-OS (Remotely)
   Refer to [steps to deploy NX-SDK applications remotely](remote/README.md).
     
  - <b>NOTE:</b> NX-SDK APIs & application development flow is the same between the two modes and the only difference is 
       the [build environment](readmes/NXSDK_docker.md). 

# Sample Custom Applications created using NX-SDK

Refer to [example NX-SDK applications](examples/README.md) for more details.
 
# Notes
   - Ensure that the name of the custom application does not collide with any existing Native Nexus applications.
   - For NX-SDK applications to be started in VSH, please name the application as one word containing alphabets. 
   - NX-SDK docker images already has NX-SDK toolkit installed in (/NX-SDK). To get latest versions do a git pull.  
   - Perform all NX-SDK related initializations in one thread. Refer to sample example custom applications for sample 
     template and usage.
   - Each API defined returns an error (or) throws an exception in case of any failures. Refer to the generated doxygen and 
     usage for each API and the different exceptions they throw.
   
</content>

  <tabTrigger>readme</tabTrigger>
</snippet>
