- [NX-SDK Utility Applications](#nx-sdk-utility-applications)
- [NX-SDK Interface Template and Usage Applications](#nx-sdk-interface-template-and-usage-applications)
  
NOTE: Click on the App for source code & detailed description. Application RPMs are located at [rpm/RPMS](../rpm/RPMS).
  
# NX-SDK Utility Applications

| Application | Version | Deployment Mode | Language | Description | 
| --- | --- | --- | --- | --- | 
|<b>featureMonitor</b> | v1.7.5 | NX-OS | <ul><li>[Python](python/featureMonitorPy)</li><li>[C++](c++/featureMonitor.cpp)</li><li>[Go](go/src/featureMonitorGo/featureMonitorGo.go)</ul> | Application to track switch feature enablement/disablement. |
|<b>[silentHostDiscovery](python/silentHostDiscovery)</b> | v1.7.5 | NX-OS | Python | Application to discover directly connected silent hosts. |
|<b>[healthMonitor](python/healthMonitor)</b> | v1.5.0 | NX-OS | Python | Computes & Monitors Health Score for various factors on a given switch. |
|<b><a href="https://github.com/ChristopherJHart/NX-SDK/tree/master/ip_move">ip_move</a></b>| v1.5.0 | NX-OS | Python | Tracking IP Movement Across Interfaces|
|<b><a href="https://github.com/ndelecro/Nexus-9K-Programmability/tree/master/NX-SDK/ECMP_Monitoring">ecmp_monitoring</a></b> | v1.5.0 | NX-OS | Python | ECMP Monitoring Application |
|<b>[routeTracker](python/routeTracker)</b> | v1.5.0 | NX-OS | Python | <ul><li>Track, Collect & Maintain history of interested route events as time series data.</li><li>Stream this time series data using streaming telemetry.</li></ul> |
|<b>[healthMonitor](python/healthMonitor)</b> | v1.5.0 | NX-OS | Python | Computes & Monitors Health Score for various factors on a given switch. |
|<b>[pbwMonitor](python/pbwMonitor)</b>| v1.0.0 | NX-OS | Python | Port Bandwidth Utilization Monitor |
|<b>[tmCfgBot](python/tmCfgBot)</b> | v1.0.0 | NX-OS | Python | <ul><li>Telemetry Config helper Bot application.</li><li>To automatically configure & manage multiple telemetry path configs per given instance.</li><li> For Ex) "tmCfgBot apply-inst evpn default-template". tmCfgBot automatically configures & manages telemetry path configs to be streamed out for EVPN instance. Use, "show tmCfgBot default-instance" to show all the paths and instances managed by the app. </li></ul> |
|<b><a href="https://github.com/ndelecro/Nexus-9K-Programmability/tree/master/NX-SDK/PTP_Monitoring">ptpMonitoring</a></b>| v1.0.0 | NX-OS | Python| PTP Monitor Application |
|<b>[dupHostMonitor](python/dupHostMonitor)</b>| v2.0.0 | Remote | Python | <ul><li>Anaylyzes the adjacency events from all the registered switches to identify the duplicate hosts in the network.</li><li>Refer to [dupHostMonitor readme](../readmes/dupHostMonitor_remote_NXSDK.md) for more details.</li></ul> |
|<b>[syslogMonitor](python/syslogMonitor)</b>| v2.5.0 | Remote | Python | <ul><li>Showcases the ability to get syslog events from all the registered switches remotely.</li><li>Refer to [dupHostMonitor readme](../readmes/dupHostMonitor_remote_NXSDK.md) on how to deploy a remote NX-SDK app.</li></ul> |

# NX-SDK Interface Template and Usage Applications

 - Template and Usage Applications provide skeleton source code in different languages showcasing how to use various NX-SDK Interfaces.
 - Refer to API documentation for more usage details. 
 
| NX-SDK Interface | Deployment Mode | App/Language | Description | 
| --- | --- | --- | --- |
| Custom Cli (NxCliParser) & syslog (NxTracer) | NX-OS | <ul><li>[customCliApp.cpp](c++/customCliApp.cpp) - C++ (v1.0.0) </li><li>[customCliPyApp](python/customCliPyApp) - Python (v1.0.0)</li><li>[customCliGoApp](go/src/customCliGoApp) - Go (v1.7.5)</li></ul> | Showcasing how to generate Custom [CLIs](../include/nx_cli.h) and [syslogs](../include/nx_trace.h).|
| NxRibMgr | NX-OS | <ul><li>[ribMgrGo](go/src/ribMgrGo) - Go (v.1.7.5) </li></ul> | Showcasing how to get routes and register for route updates. Refer to [nx_rib_mgr.h](../include/nx_rib_mgr.h) |
| NxIntfMgr | NX-OS | <ul><li>[intfMonitor.cpp](c++/intfMonitor.cpp) - C++ (V1.7.5) </li><li>[intfMonitorPy](python/intfMonitorPy) - Python (v1.7.5)</ul> | Showcasing how to get, update and register for interface events. Refer to [nx_intf_mgr.h](../include/nx_intf_mgr.h) |
| NxMacMgr | NX-OS | <ul><li>[macTracker](python/macTracker) - Python (V1.7.5) </li></ul> | Showcasing how to get, update and register for Mac events. Refer to [nx_mac_mgr.h](../include/nx_mac_mgr.h) |
| NxAdjMgr | NX-OS | <ul><li>[neighbourApp](python/neighbourApp) - Python (V1.7.5) </li></ul> | Showcasing how to get, update and register for adjacency events. Refer to [nx_adj_mgr.h](../include/nx_adj_mgr.h)|
