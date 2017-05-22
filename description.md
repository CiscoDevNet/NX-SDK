# NX-SDK Custom Application Development ToolKit

 - NXSDK toolkit contains:
   ```
     git clone https://github.com/CiscoDevNet/NX-SDK.git
     cd $PWD/NX-SDK
     ls (Makefile doc  examples  include  python  rpm  src  stubs)
   ```

## NX-SDK toolkit Directory Structure
 - <a href="https://github.com/CiscoDevNet/NX-SDK/tree/master/include">include/</a>: <p align="justify"> Header files to be included and the abstract interfaces 
         to be used to develop custom applications in order to gain access to the supported NX-OS infrastructure. </p>
 - <a href="https://github.com/CiscoDevNet/NX-SDK/tree/master/doc">doc/</a>:       <p align="justify"> Doxygen generated html document for public APIs and its usage
         are located at <a href="https://github.com/CiscoDevNet/NX-SDK/blob/master/doc/html/annotated.html">doxygen-html</a>. Do a git clone 
         of NX-SDK and open <a href="https://github.com/CiscoDevNet/NX-SDK/blob/master/doc/html/annotated.html">doxygen-html</a> locally to get the list 
         of exported public APIs and its usage.</p>
 - <a href="https://github.com/CiscoDevNet/NX-SDK/tree/master/stubs">stubs/</a>: <p align="justify"> C++ Stub implementation for the interfaces defined in include/.
         When the C++ custom application is built in the development environment, the stubs are then compiled and are then dynamically linked to
         custom Application. Thereby allowing the custom application to be developed, built and tested in your own development environment even before running the Apps 
         in the switch. 
          <ul>On compiling/building the C++ Custom Application:
           <li> its Binary will be built/placed in auto-generated <b>bin/</b>. </li> 
           <li> Application objects are then built/placed in auto-generated <b>build/</b>.</li>
           <li> NXSDK Library built from the stubs implementations will be placed in auto-generated <b>libs/</b>. </li>
          </ul>  
         When the Application is then run in the Nexus switches it then links to the real NX-SDK library packaged in the Nexus
         images. Refer to <a href="https://github.com/CiscoDevNet/NX-SDK#release">Nexus releases</a> in which NX-SDK is available. NOTE: For python custom applications, stubs will soon be published.</p>
 - <a href="https://github.com/CiscoDevNet/NX-SDK/tree/master/examples">examples/</a>: <p align="justify"> Sample C++ example custom Applications showcasing the usage of NX-SDK.</p>
 - <a href="https://github.com/CiscoDevNet/NX-SDK/tree/master/python">python/</a>: <p align="justify"> NX-SDK also supports other language bindings. In V1.0, it supports
        python language binding for custom application development using NX-SDK. Refer to <a href="https://github.com/CiscoDevNet/NX-SDK/tree/master/python/examples">python/examples/</a> for sample python 
        custom Applications showcasing the usage of NX-SDK. </p>
 - <a href="https://github.com/CiscoDevNet/NX-SDK/tree/master/rpm">rpm/</a>: <p align="justify"> To start/stop custom applications from VSH, it needs to be built/packaged
     as an RPM package by using the provided ENXOS SDK docker container. It contains the SPEC file for sample example applications and its associated RPMS.
     Refer to <a href="https://github.com/CiscoDevNet/NX-SDK#4-packaging-custom-application">RPM packaging instructions</a> for more details on how to package your custom application as an RPM package.</p>
 - <a href="https://github.com/CiscoDevNet/NX-SDK/tree/master/Makefile">Makefile</a>: <p align="justify"> To build your C++ custom Applications using NX-SDK and
      to dynamically link to the right NX-SDK library so that when the C++ App is run inside the Nexus switch it can gain access to NXOS Infrastructure.
      Add your C++ custom Application to the Makefile just like Sample example C++ applications.
      Refer to Makefile for further instructions. Nothing needs to be done for Python custom Apps using NX-SDK.
      Refer to <a href="https://github.com/CiscoDevNet/NX-SDK#2-building-custom-applications">Building Custom applications</a> section         for more details.</p>
 
