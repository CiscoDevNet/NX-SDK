# NX-SDK v1.5.0 (latest):

## Nexus Release:
   - N9Ks, N3Ks from I7(3) release. 
   
## Features: 
| Features  | Details |
| --- | --- |
|Languages | C++, Python |
| NxRibMgr | <ul><li>Ability for custom applications to register with Nexus L3 RIB for route updates & lookups.</li><li>For more details on the NxRibMgr APIs and its usage, refer to [nx_rib_mgr.h](include/nx_rib_mgr.h)</li><li>For sample application using NxRibMgr, refer to [routeTracker](examples/python/routeTracker) NX-SDK application.</li><li>Known Bug: nx_rib_mgr.h APIs works only in VSH not in BASH.</li></ul>|
| Streaming telemetry | <ul><li>Ability to stream custom information from custom NX-SDK application through NXOS telemetry.</li><li>To stream custom data, define custom "show" command in your NX-SDK application to directly ouput JSON data to be streamed out of the switch.</li><li>Set <b>“query-condition show-output-format=json”</b> in the telemetry path config as follows to stream custom JSON data from custom NX-SDK Show cli.</li><li>For sample application using streaming telemetry, refer to [routeTracker](examples/python/routeTracker) NX-SDK application.</li></ul> |
| CPU Limit | <ul><li>CGROUP support for NX-SDK Applications ensure NX-SDK user applications, do not overconsume system resources(CPU cycles, system memory) from other native system processes.</li><li>NX-SDK supports three cgroup modes: <ul><li>Low: All NX-SDK apps started in this mode combined can use upto 25% of CPU and 768M of Memory in contention.</li><li>Med: All NX-SDK apps started in this mode combined can use upto 50% of CPU and 1GB of Memory in contention.</li>High: All NX-SDK apps started in this mode combined can use upto 75% of CPU and 1.5GB of Memory in contention.</li></ul><li>By default, all NX-SDK Apps will be started as Low priority app.</li><li>App priority can be changed using the API, `setAppPriority(nxos::prio_e prio)` defined in [nx_sdk.h](include/nx_sdk.h).</li></ul> |
| Memory footprint | <ul><li>Memory footprint for all NX-SDK objects created and its sizes for an NX-SDK Application.</li><li>To get the memory footprint use, "show $appname nxsdk mem-stats".</li></ul>|
| Advanced Exception | <ul><li>On failure, APIs throw exceptions. Refer to the API documentation for more details on the failures cases for each & every API.</li><li>In v1.0.0, failures are thrown as simple exceptions as Strings.</li><li>From v1.5.0, applications can either catch simple or advanced exceptions. Advanced exceptions are of common type "NxException" across all languages with more detailed information on the failure.</li><li>Refer to [nx_exception.h](include/nx_exception.h) for more details on the supported APIs & its usage.</li></ul>|

## Sample Applications:
   - routeTracker, healthMonitor. Refer to [examples](examples) for more details. 
   

# NX-SDK v1.0.0:

## Nexus Release:
   - N9Ks from I6(1) release. 
   - N3Ks from I7(1) release.

## Git Release:
   - Refer to <a href="https://github.com/CiscoDevNet/NX-SDK/releases/tag/v1.0.0">NX-SDK v1.0.0</a> if you need to access this version.
   - To operate on older NX-SDK versions, clone the branch using the respective tag.
   
## Features: 
| Features  | Details |
| --- | --- |
|Languages | C++, Python |
|Custom Cli | <ul><li>APIs to generate custom CLIs (config and show commands) and get callbacks.</li><li>For more details on the custom CLI APIs and its usage, refer to [nx_cli.h](include/nx_cli.h).</li><li>For sample application generating custom CLI, refer to [customCliApp.cpp](examples/c++/customCliApp.cpp) app.</li></ul>|
|Custom Syslog|<ul><li>APIs to generate custom syslogs and to log application Events & Error history.</li><li>To view application logged Events & Error history use "show $appname nxsdk event-history events or errors".</li><li>For more details on the APIs and its usage, refer to [nx_trace.h](include/nx_trace.h).</li><li>For sample application using custom syslog, refer to [customCliApp.cpp](examples/c++/customCliApp.cpp) app.</li></ul>|
|Start/Stop Apps |<ul><li>To start/stop custom NX-SDK Applications from VSH just like any other native Nexus applications (like feature bgp) thereby persisting custom applications across system reloads, System Switchover etc.</li><li>Refer to [README.md](README.md) for more details</li><li>Supports upto 32 NX-SDK applications.</li></ul>            |

## Sample Applications:
   - tmCfgBot, pbwMonitor. Refer to [examples](examples) for more details. 
