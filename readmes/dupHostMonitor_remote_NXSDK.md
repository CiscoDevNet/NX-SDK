# Steps to deploy "dupHostMonitor" Remote NX-SDK sample application
   
## Description

   "dupHostMonitor" is an NX-SDK Python Remote application. "dupHostMonitor" tries to establish remote connection with 
   multiple NX-OS switches specified in the provided "switch_details.json" file and registers for adjacency events using 
   NX-SDK NxAdjMgr Apis from all the registered switches. It then anaylyzes the adjacency events from all the registered 
   switches to identify the duplicate hosts in the network. New Switches can be added & existing switches can be deleted 
   on the fly to/from switch_details.json 
     
## Details

   | <p align="left">App Name</p> | <p align="left">dupHostMonitor</p> |
   |---|---|
   | <b>Language</b>        | Python  |
   | <b>NX-SDK Supported Version</b>  | v2.0.0  |
   | <b>Deployment Mode</b> | Remote  |
   | <b>Docker image to use</b> | NX-SDK Remote Docker |
   | <b>Dependent/Associated files</b> | <ul><li>switch_details.json : Contain details about <ul><li>NX-OS Switches IP addresses to connect to remotely.</li><li>NX-OS Switch Names.</li><li>Remote NX-SDK application to Switch Port mapping.</li></ul></li><li>Certificate/Key: Auto-generated tmp (or) user installed certificate (pem file) |
   
## Requirements

   - NX-OS switches with 9.3.0 images.
   - Install Docker for your OS(make sure there is enough memory).

## a. NX-OS switches:
### Step 1: NX-OS version
    ```
       ! Check NX-OS version in the switches using
       show version
       
       ! make sure the version is >= 9.3.0
    ```
### Step 2: Start "dupHostMonitor" light-weight NX-SDK server in NX-OS switches:

   ```
      ! Enable feature nxsdk
      feature nxsdk
      
      ! Start "dupHostMonitor" light-weight NX-SDK server in the permitted available port & namespace/vrf.
      nxsdk remote port 50002 namespace management
      
      ! Bind the remote service to the port
      service-name dupHostMonitor
      
      ! Check if dupHostMonitor server is running. 
      show nxsdk internal service 

      NXSDK Started/Temp unavailabe/Max services : 1/0/32 
      NXSDK Default App Path         : /isan/bin/nxsdk
      NXSDK Supported Versions       : 1.0 1.5 1.7.5 2.0 

      Action: R - RPM was un-installed
      Action: U - RPM was upgraded (app restart required)
      Action: W - RPM is not installed yet

      Service-name              Base App    Started(PID)  SDK Version  RPM Package(Action)
      ------------------------- ----------- ------------- ------------ ------------------------
      dupHostMonitor            nxsdk_app1  VSH(9356)                  Remote:50002 (management)  
      
      ! To get the switch stats on dupHostMonitor server
      show dupHostMonitor stats detail 

      NX-SDK Server Stats information
      -------------------------------
      Port                                : 50002
      Sync Thread Started                 : Yes
      Async Thread Started                : Yes
      Sync Req/Resp/Exception Msg#        : 10/10/0
      Sync Req/Resp/Exception Msg Size    : 804/169/0
      Async Msg Count/Size                : 3/373

      --------------------------------------------------------------
      App Name                       : dupHostMonitor(758)
      Status                         : Up(0d 00:17:08)
      Connection Count               : 2
      Total Msg Count/Size           : 80/3063 (bytes)
      KA Count/Size                  : 68/2312
      Last Rcvd KA                   : 5 seconds
      Sync req Count/Size            : 5/401 (bytes)
      Sync resp Count/Size           : 5/84 (bytes)
      Sync err Count/Size            : 0/0 (bytes)
      Async Obj                      : 0xd3902e58 [Pending Async Events: 0]
      Async Event Count/Size         : 2/266 (bytes)
      Async Batch Event Thread       : Yes
      Hit Async Max Batch            : 0 (1000 per batch)
      Async Max Batch thread         : 3 (avg 1.00 updates per batch)
      Hit Async Batch thread no-work : 0
      Total NX-OS post Rcvd          : 3 [In Q: 0]
      --------------------------------------------------------------
   ``` 

## b. Setup NX-SDK Remote Docker Environment:

### Step 1: Install NX-SDK Remote Docker
   
   - NOTE: Following Docker commands are for ubuntu linux OS. Please use respective Docker commands for your OS.
   ```
      ! Check if docker is running
      docker version
      
      ! Pull NX-SDK docker Remote image
     docker pull dockercisco/nxsdk-remote:v1
      
      ! Get the NX-SDK Docker Remote Image id
      docker images
      
      ! Run NX-SDK docker Remote container
      docker run -it <nxsdk-remote-docker-image-id> /bin/bash
      
      ! NX-SDK toolkit is already available
      cd /NX-SDK
      
      ! Set the necessary environment, by running setup.sh script
      sh setup.sh     
   ```   
   
### Step 2: Edit switch_details.json
   - "dupHostMonitor" application is located at 
     ```
        cd /NX-SDK/examples/python    
     ```
     
   - Edit "switch_details.json" file to edit/add the NX-OS switches (IP address, port to app mapping etc) 
     that you want "dupHostMonitor" to connect. Refer to sample. NOTE: Make sure that "switch_details.json" 
     is in the same location as dupHostMonitor application.
     ```
      {
        "switchInfo": [{
                        "addr": "x.x.x.x",
                        "name": "sinai",
                        "port-apps": [{
                                        "port" : 50002,
                                        "app"  : "dupHostMonitor"
                                      },
                                      {
                                        "port" : 50003,
                                        "app"  : "routeMonitor"
                                      }]

                },
                {
                        "addr": "y.y.y.y",
                        "name": "cyprus-n9k",
                        "port-apps" : [{
                                        "port" : 50002,
                                        "app"  : "dupHostMonitor"
                                      },
                                      {
                                        "port" : 50003,
                                        "app"  : "routeMonitor"
                                      }]
                }
            ]
       }
     ```

### Step 3: Get the Certificate for Remote Client to establish remote connection with the server securely:

   - Remote Client needs the server certificate to connect to the server securely. 
   - The onus is on the network admin to provide (or) give secure access to the server certificate to the remote clients.   
   - This guide explores both options using the auto-generated tmp & user generated certificate. 
   - It is recommended to use <b>"user generated cerftificate by a trusted signed authority"</b>. As tmp generated 
     certificate to be used only to test the feature as its valid only for 24 hours.
 
#### User Generated Common Certificate (Recommended):
   - Load your certificate & key in **all the NX-OS switches** you will be connecting to.
   
     ```
        ! Copy your certificate to /etc so that your certificate persist across reloads/SSo etc
        run bash sudo su
        mv /bootflash/nxsdk.pem /etc/cert.pem
        mv /bootflash/nxsdk.key /etc/cert.key
        
        ! Edit the config file to point to your certificate.
        cp /etc/nxsdk_default.cfg /etc/nxsdk.cfg
        chmod 777 nxsdk.cfg
        vi /etc/nxsdk.cfg
        
        ! Update the following in nxsdk.cfg
        [grpc]
        cert=/etc/cert.pem
        key=/etc/cert.key      
   
        ! (or) get the config file from /NX-SDK/remote/nxsdk.cfg
        ! Other method is to package certificate and nxsdk.cfg as an RPM & install 
        ! them on all the switches to be connected remotely.
        
        ! Restart dupHostMonitor App if its already running (or) remove/start the app again
        ! for the new certificate & key to take effect.
        nxsdk remote port 50002 namespace management
           no service-name dupHostMonitor
           service-name dupHostMonitor
     ```
    - NOTE: This step is done <b>only once</b> till the certificate expires. This step will be simplified in the next release. 
    - Copy the certificate (cert.pem) file to remote Docker container in the same location as "dupHostMonitor" application (/NX-SDK/examples/python). 
     
   - Once copied, set the respective environment variables to point to right certificate.
   
     ```
       ! use the appropriate CN value used in the certificate generation
       export NXSDK_GRPC_HOST_OVERRIDE=ems.cisco.com 
       
       !!! Common certificate to be used to connect to all the NX-OS switches
       export NXSDK_SERVER_CERT=./cert.pem
     ```
#### Auto-generated tmp certificate:
   - The auto-generated certificate in NX-OS switches can be found at:<br>
         <ul><li>/var/volatile/tmp/nxsdkTmpCert.pem -- temporary public certificate</li></ul>
   - Copy the certificate (.pem) file **from all the NX-OS switches** you will be connecting to remote Docker container in 
     the same location as "dupHostMonitor" application (/NX-SDK/examples/python). 
     
     ```
       ! In the NX-OS switches, copy the tmp certificate pem file to bootflash.
       run bash sudo su
       cp /var/volatile/tmp/nxsdkTmpCert.pem /bootflash
       
       ! From the Remote Docker container, copy the cert.pem file from bootflash
       cd /NX-SDk/examples/python
       scp admin@x.x.x.x/nxsdkTmpCert.pem ./nxsdkTmpCert_x_x_x_x.pem
       scp admin@y.y.y.y/nxsdkTmpCert.pem ./nxsdkTmpCert_y_y_y_y.pem
     ```
     
   - NOTE: Each switch has its own unique certificate (.pem) file. Hence name them accordingly.
   - Once copied, set the respective environment variables to point to right certificate.
   
     ```
       export NXSDK_GRPC_HOST_OVERRIDE=ems.cisco.com
       export NXSDK_SERVER_CERT_x_x_x_x=./nxsdkTmpCert_x_x_x_x.pem
       export NXSDK_SERVER_CERT_y_y_y_y=./nxsdkTmpCert_y_y_y_y.pem      
     ```
   
### Step 4: Run "dupHostMonitor" in NX-SDK Remote Docker Container

    ```
       cd /NX-SDK/examples/python
       python dupHostMonitor
       
       
       Time: 2019-07-02 22:32:20.558734

       Switch          | Addr:Port                 | SDK Inst   | Events Cnt | Status    
       ----------------------------------------------------------------------------------
       cyprus-n9k      | x.x.x.x:50002             | Yes        |          1 | Connected 
       sinai           | y.y.y.y:50002             | Yes        |          1 | Connected 
       
       ! Added 10 adjacencies in Sinai switch
       sinai(config)# sh ip adjacency 
       IP Adjacency Table for VRF default
       Total number of entries: 10
       Address         MAC Address     Pref Source     Interface       Flags
       20.1.1.2        0000.0000.1891  50   arp        Vlan10              
       20.1.1.3        0000.0000.1892  50   arp        Vlan10              
       20.1.1.4        0000.0000.1893  50   arp        Vlan10              
       20.1.1.5        0000.0000.1894  50   arp        Vlan10              
       20.1.1.6        0000.0000.1895  50   arp        Vlan10              
       20.1.1.7        0000.0000.1896  50   arp        Vlan10              
       20.1.1.8        0000.0000.1897  50   arp        Vlan10              
       20.1.1.9        0000.0000.1898  50   arp        Vlan10              
       20.1.1.10       0000.0000.1899  50   arp        Vlan10              
       20.1.1.11       0000.0000.189a  50   arp        Vlan10              

       ! In NX-SDK Remote container where dupHostMonitor is running.
       Time: 2019-07-02 22:35:08.706976

       Switch          | Addr:Port                 | SDK Inst   | Events Cnt | Status    
       ----------------------------------------------------------------------------------
       cyprus-n9k      | x.x.x.x:50002             | Yes        |          1 | Connected 
       sinai           | y.y.y.y:50002             | Yes        |         11 | Connected 

       ! Added 1 Duplicate adjacency in cyprus-n9k switch
       cyprus(config)# sh ip adjacency 
       IP Adjacency Table for VRF default
       Total number of entries: 1
       Address         MAC Address     Pref Source     Interface       Flags
       20.1.1.2        0000.0000.1891  50   arp        Vlan10              

       Time: 2019-07-02 22:38:56.945580

       Switch          | Addr:Port                 | SDK Inst   | Events Cnt | Status    
       ----------------------------------------------------------------------------------
       cyprus-n9k      | x.x.x.x:50002             | Yes        |          2 | Connected 
       sinai           | y.y.y.y:50002             | Yes        |         11 | Connected 
      
       Dup Host detected [default] 20.1.1.2 Mac: 0000.0000.1891 L3 Intf: Vlan10 Phy Intf: Ethernet1/2 in switches  
       [x.x.x.x, y.y.y.y]
    ```
    



   
