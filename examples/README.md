- [NX-SDK Utility Applications](#nx-sdk-utility-applications)
- [NX-SDK Interface Template and Usage Applications](#nx-sdk-interface-template-and-usage-applications)
  
NOTE: Click on the App for source code & detailed description. Application RPMs are located at [rpm/RPMS](../rpm/RPMS).
  
# NX-SDK Utility Applications

| Application | Version | Language | Description | 
| --- | --- | --- | --- | 
|<b>[routeTracker](examples/python/routeTracker)</b> | v1.5.0 | Python | <ul><li>Track, Collect & Maintain history of interested route events as time series data.</li><li>Stream this time series data using streaming telemetry.</li></ul> |
|<b>[healthMonitor](examples/python/healthMonitor)</b> | v1.5.0 | Python | Computes & Monitors Health Score for various factors on a given switch. |
|<b>[pbwMonitor](examples/python/pbwMonitor)</b>| v1.0.0 | Python | Port Bandwidth Utilization Monitor |
|<b>[tmCfgBot](examples/python/tmCfgBot)</b> | v1.0.0 | Python | <ul><li>Telemetry Config helper Bot application.</li><li>To automatically configure & manage multiple telemetry path configs per given instance.</li><li> For Ex) "tmCfgBot apply-inst evpn default-template". tmCfgBot automatically configures & manages telemetry path configs to be streamed out for EVPN instance. Use, "show tmCfgBot default-instance" to show all the paths and instances managed by the app. </li></ul> |
|<b><a href="https://github.com/ndelecro/Nexus-9K-Programmability/tree/master/NX-SDK/PTP_Monitoring">ptpMonitoring</a></b>| v1.0.0 | Python| PTP Monitor Application |

# NX-SDK Interface Template and Usage Applications

 - Template and Usage Applications provide skeleton source code in different languages showcasing how to use various NX-SDK Interfaces.
 - Refer to API documentation for more usage details. 
 
| NX-SDK Interface | Version | App/Language | Description | 
| --- | --- | --- | --- |
| Custom Cli & syslog | v1.0.0 | <ul><li>[customCliApp](c++/customCliApp) - C++ </li><li>[customCliPyApp](python/customCliPyApp) - Python</li></ul> | Showcasing how to generate Custom [CLIs](../include/nx_cli.h) and [syslogs](../include/nx_trace.h).|