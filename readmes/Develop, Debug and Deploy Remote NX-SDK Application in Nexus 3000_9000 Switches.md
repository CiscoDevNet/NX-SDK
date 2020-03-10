# Develop, Debug and Deploy Remote NX-SDK Application in Nexus 3000/9000 Switches

## Introduction

This document serves as a best practices user guide for building and deploying NXSDK remote applications 

## Prerequisites

1.	Understanding of SSL/TLS Handshake
2.	Remote Docker container obtained from <https://github.com/CiscoDevNet/NX-SDK> page
3.	Connectivity between the N3K/N9K switches and host running the remote Docker container. This can be over management, default or non-default vrf.

## Components Used

The information in this user guide is based on the NXOS 9.3(1) software release, which provides support for NXSDK v2.0.0 in addition to all its prior versions. The NXSDK v2.0.0 adds following features:
1.	Remote API Support
2.	Security profiles for on-switch and remote applications
3.	Write APIs throttling
4.	NXAPI support for custom commands

This document, however, focuses only on remote API Support and security profiles.

**NOTE**: The information in this document was created from the devices in a specific lab environment. Devices used in this document started with a cleared (default) configuration. If your network is live, make sure that you understand the potential impact of any command, and take appropriate measures to save the working configuration so that any changes may be reverted.  

## Background

1. With NXSDK v2.0.0 Cisco NXOS provides the capability to run applications on a remote device (client), which can talk to the server (switch). The communication takes place over a gRPC layer. For more information on gRPC click <https://grpc.io/docs/guides/>. 
2. The native encryption provided by gRPC layer uses the SSL/TLS authentication mechanism, which makes use of the certificate and key pair. Starting with NXOS 9.3(1) the NXSDK services running on the switch make use of the certificate/key pair to establish communication with applications running on remote client.
3. Starting with NXOS 9.3(1) all applications started on the switch have an associated security profile.  By default, the profile restricts changing the system state and gives read-only access.  If an application requires changing system state, a writable profile must be created and applied to the application.

## Develop and Deploy Applications

### Configure Security Profiles on the Nexus switch

*Step 1*: Enable feature nxsdk on the switch. Create a security profile on the switch. If applications are associated with no profile then by default all APIs that modify the switch configuration will be blocked (deny all).

```Switch(config)# nxsdk profile profile1
Switch(config-nxsdk-profile)# system modifications permit
Switch(config-nxsdk-profile)# end
```
*Step 2*: Check privileges provided by the profile

```Switch(config)# show nxsdk internal profile
Service Profile:   profile1
----------------
NxSDK Manager      Write API Policy
--------------- --------------------------
NxCliParser     Allowed
NxRibMgr        Allowed
NxAdjMgr        Allowed
NxMacMgr        Allowed
NxIntfMgr       Allowed
NxDmeMgr        Allowed

Switch(config)#
```
*Step 3*: Associate an application with the new profile

```Switch(config)# nxsdk service-name customCliPyApp profile profile1
% This could take some time. "show nxsdk internal service" to check if your App is Started & Runnning
Switch(config)#
```
*Step 4*: Ensure the application has the right profile associated
```
Switch(config)# show nxsdk internal service customCliPyApp

 Service-name : customCliPyApp
 Started(PID) : VSH(18658)
  App Version : 1.0
  SDK Version : 1.0
     Base App : nxsdk_app4
Action-needed : None
  RPM Package : customCliPyApp-1.0-1.0.0.x86_64
      Profile : profile1
Switch(config)#
```
### Configure Remote Server on the Nexus switch

**NOTE:** 
1.	For information on developing and deploying on-switch applications please follow below guide - 
<https://www.cisco.com/c/en/us/support/docs/ios-nx-os-software/nx-os-software/213396-develop-debug-and-deploy-nx-sdk-python.html>
2.	For NXSDK API documentation please follow guide <https://nx-sdk.readthedocs.io/en/latest/>.

#### Background

1. Each NXSDK application has an sdkThread function which instantiates the NXSDK. With v2.0.0 the API getSdkInst() now takes below additional arguments for remote API support – 
   - remoteIp – Ipv4 address of the switch
   - remotePort – Port on the switch where the server is listening to client connections. Allowed range for NXSDK remote services is 50002-51000. There can be only one service running on a single port.
   - certificate – [Optional] pointer to certificate used in TLS/SSL authentication
2. On the switch, when the feature nxsdk is enabled, a temporary certificate and key pair is generated with a validity period of 24 hours. 
3. A new temporary certificate/key pair is generated each time a switch reload or switchover happens. 
4. This also creates a config file at /etc/nxsdk_default.cfg, which points to the location of the temporary certificate /key pair. 
5. Remote services when started on the switch use this temporary certificate/key pair.
6. In order to have seamless working of remote services user would need to obtain a permanent certificate and key from a certificate issuing authority and save it on the switch. Ensure there are proper access permissions set on the permanent certificate and key files.
7. Once the permanent certificate/key pair is obtained, copy the /etc/nxsdk_default.cfg to /etc/nxsdk.cfg and edit the path for certificate and key in the /etc/nxsdk.cfg file. In a dual-SUP setup ensure the files exists on standby SUP as well. This can be done by doing ‘copy r s’, which will copy the configuration file and the certificate/key to the standby.
8. All remote services started after creation of /etc/nxsdk.cfg file would no longer use the temporary certificate /key. They would use the permanent certificate /key as specified in the file.
9. In order for services started with temporary certificate /key to use the permanent certificate /key pair user would need to stop and start the service.
10. A single remote app can connect to multiple switches using multiple getSdkInst() calls. 
11. A maximum of 10 remote applications can connect to a single switch.

*Step 1*: Start remote service on the switch
```
Switch(config)# nxsdk remote port 51000 namespace management
Switch(config-nxsdk-remote)# service-name service_1 profile profile1
% This could take some time. "show nxsdk internal service" to check if your App is Started & Runnning
Switch(config-nxsdk-remote)#
```
*Step 2*: Ensure remote service is running on the switch
```
Switch(config)# show nxsdk internal service

NXSDK Started/Temp unavailabe/Max services : 1/0/32
NXSDK Default App Path         : /isan/bin/nxsdk
NXSDK Supported Versions       : 1.0 1.5 1.7.5 2.0

 Action: R - RPM was un-installed
 Action: U - RPM was upgraded (app restart required)
 Action: W - RPM is not installed yet

Service-name              Base App    Started(PID)  SDK Version  RPM Package(Action)
------------------------- ----------- ------------- ------------ ------------------------
service_1                 nxsdk_app1  VSH(29888)                 Remote:51000 (management)
Switch(config)#
```
*Step 3*: Ensure correct certificate used by the service
```
Switch(config)# show logging logfile | grep certificate
2019 Jun 28 22:19:19 Switch nxsdk: GRPC Server initializing secure TLS credentials, using certificate = /var/volatile/tmp/nxsdkTmpCert.pem  key = /var/volatile/tmp/nxsdkTmpKey.key
Switch(config)#
```
*Step 4*: Check service stats before client is up
```
Switch(config)# show service_1 stats detail

NX-SDK Server Stats information
-------------------------------
Port                                : 51000
Sync Thread Started                 : Yes
Async Thread Started                : No
Sync Req/Resp/Exception Msg#        : 0/0/0
Sync Req/Resp/Exception Msg Size    : 0/0/0
Async Msg Count/Size                : 0/0
Switch(config)#
```
### Configure Remote Client

1. 	Copy out the temporary or permanent certificate from the switch onto the client (Docker container). It can be saved at /nxsdk/remote/cert or directory of your choice.
2. Ensure the certificate files on switch and client match by running a checksum.
3. Ensure the date/time settings on client and switch are in sync.
4. In order to successfully complete the SSL/TLS Handshake the applications running on the client need ONE of the following to be present. The search for the certificate is done in the order specified below.
    - Absolute path to the certificate provided in the getSdkInst() API call. OR
    - Environment variable NXSDK_SERVER_CERT_A.B.C.D, where A_B_C_D is the ip address of the switch, points to the certificate       location (absolute path). OR
    - Environment variable NXSDK_SERVER_CERT points to the certificate location (absolute path). OR
    - Temporary certificate nxsdkTmpCert.pem in application current directory. OR
    - Permanent certificate nxsdkServerCert.pem in application current directory. Note that the certificate name must be   nxsdkServerCert.pem if none of the above methods is used.
5. In addition, for the purpose of server validation the client machine should be able to obtain the hostname associated with remoteIp. This can be done in one of the following ways – 
    - DNS lookup
    - Entry in /etc/hosts 
    - Environment variable NXSDK_GRPC_HOST_OVERRIDE points to the CN in the certificate (when this method is used the value of the environment variable need not match the server hostname)

### Writing Remote Applications

#### Prerequisites

1.	Install the Docker container with remote API support from GitHub page.
2.	Make sure below necessary environment variable is set on the container. 
[Optional] Download the latest version of container by doing a git pull at the /nxsdk directory.
```
export NXSDK_ROOT=/nxsdk
```

3.	Ensure necessary environment is set for applications to find the certificate
4.	Ensure client is configured correctly for server validation

**NOTE:** Creating custom CLIs on the switch is not supported via remote API support. 

#### Mandatory for remote applications:

1.	**nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, remoteIp, remotePort, certificate)**: API to create an SDK instance. For remote applications, this call uses the remoteIp, remotePort parameters. The certificate parameter is optional if other ways of locating it are set.
2.	**class pySdkHandler**: This class in the nx_sdk_py library inherits the NxSdkHandler class which has below two callback functions. User can overload these functions to perform actions after remote connection goes down and comes back up.
a.	**postRemoteConnDownCb(self, nxsdk)**: Function to perform user defined actions when the remote gRPC connection between the server and the client goes down.
b.	**postRemoteConnUpCb(self, nxsdk)**: Function to perform user-defined actions when the remote gRPC connection between the server and the client comes back up. It is important for applications to re-init with all the SDK managers as it is a stateless connection. All objects created before connection came back up are obsolete and hence should be purged.


#### Sample code:

```
def sdkInit(nxsdk, re_init):
    # Re-initialize the SDK managers and perform any other tasks here needed after connection reset

class pySdkHandler(nx_sdk_py.NxSdkHandler):
    def postRemoteConnUpCb(self, nxsdk):
        print ("IP %s connection up, do re-init" % nxsdk.getRemoteIp())
        nxsdk.purgeAllSdkObjs()
        sdkInit(nxsdk, True)
     def postRemoteConnDownCb(self, nxsdk):
        print ("IP %s connection down, all objs obsolete" % nxsdk.getRemoteIp())
```

### Debugging Remote Connection Failures

Given that there is network connectivity between the remote client and the switch, remote applications can fail to connect to the switch running remote services for one or more of the following reasons – 
1.	Remote IP and remote port in getSdkInst() call do not match the IP of the switch (server) and the port where remote service is started.
2.	Remote port not reachable – an application is already running on the port
3.	File mismatch – verify the checksum on files to be same for self-signed certificates
4.	Date/time are not in sync – verify the time zone and date settings. Ensure certificate is not expired.
5.	Certificate is no longer valid – check the expiration date/time
6.	Client is unable to locate the certificate – Verify the absolute path or environment variables’ values
7.	Client is unable to perform the host validation – verify host lookup can be done 
8.	CN (Common Name) in the SSL/TLS Handshake does not match the CN specified in the certificate – View the contents of certificate with ‘openssl x509 -in <cert>  -text -noout’

On the switch, remote services can fail to come up due to one of the following reasons – 
1.	Certificate is expired – View the contents of certificate with ‘openssl x509 -in <cert>  -text -noout’
2.	Path to certificate /key pair is invalid – Check the /etc/nxsdk.cfg file
3.	Key file read error – 
    -	Certificate or key files do not have proper permissions set. Change permissions with chmod.
    -	Verify that the private key matches the certificate. Below command can be used - 
```
openssl x509 -noout -modulus -in server.crt | openssl md5
openssl rsa -noout -modulus -in server.key | openssl md5
openssl req -noout -modulus -in server.csr | openssl md5 
```
