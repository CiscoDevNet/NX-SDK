# Security Profiles for NX-SDK Applications


# Table of Contents

   - [Description](#description)
   - [Versions](#versions)
   - [Configuring Security Profiles](#configuring-security-profiles)
   - [Limitations and Restrictions](#limitations-and-restrictions)
   - [Troubleshooting](#troubleshooting)

# Description

A security profile is an optional object that is attached to the NX-SDK (on box or remote) services running in the switch.
Security profiles control an NX-SDK (on box or remote) application's ability to interact/access NX-OS. 

# Versions
  - [NX-SDK 2.0.0](##from-nx-sdk-v2.0.0) 
    
# Configuring Security Profiles

## From NX-SDK V2.0.0

  - Security profiles have been added to control an NX-SDK (on box or remote) application's ability to use/access NX-SDK write 
    APIs (Apis that writes to the switch). Refer to API documentation for more details on the NX-SDK APIs classified as write 
    APIs. 
  - <b>By default, NX-SDK (on box or remote) application access/use of write APIs are disallowed</b> (If used without the 
    appropriate profile then an exception is thrown). 
  - For on box (or) remote NX-SDK applications to access the NX-SDK write apis, Network admin (or one with right priviledges) 
    must create appropriate security profiles to be attached to the NX-SDK on box (or) remote service thereby allowing only 
    the right applications to access the write APIs.
  - <b>NOTE:</b> Refer to API documentation for more details on which APIs are write APIs. 
  - Security profiles are not needed if your NX-SDK application is not using any of the write APIs.
  - More security profiles will be added in the future NX-SDK versions.

    Cisco's NX-SDK offers the following security profiles in NX-SDK V2.0.0
    
    | Profile | Description | Values |
    |---------|-------------|--------|
    | <b>Deny</b>    |  Prevents any write API calls from writing to the switch except for adding CLIs. ie Allows only GET requests & register for events | This is the default profile. |
    | <b>Throttle</b> | Allows write APIs that modify the switch, but only up to a specified number of calls. This security profile applies throttling to control the number of API calls.<br> The application is allowed to write up to the limit, but when the limit is exceeded, writing stops, and the reply sends an error message.| The throttle is 50 API calls, and the throttle resets after five seconds. Its fixed and not user configurable. |
    | <b>Permit</b> | APIs that modify the switch are allowed without restriction | |

### Configuring a Security Profile to Allow Write Access
   - You can enable write access for NX-SDK API calls without any restrictions. 
   - Through this security profile, an NX-SDK (on box or remote) application can modify the switch through NX-SDK
     supported write APIs, and without any control over the number of NX-SDK write API calls.
   
     ! Before you begin, Enable NX-SDK before applying a security profile. Use "feature nxsdk" to enable NX-SDK.

     | Steps | Command or Action | Purpose |
     |-------|--------------------|--------|
     | Step 1 |  nxsdk profile <b><i>\<profile-name\></i></b> <br><br> Example: <br><i>switch(config)# nxsdk profile permit-writes</i> | Create an application profile|
     | Step 2 | <b>system modifications permit</b> <br><br> Example:<br> <i>switch(config-nxsdk-profile)# system modifications permit </i>| For the profile, enable writes through the permit option.|
     | Step 3a | nxsdk service-name \<app-name\> <b><i>profile \<profile-name\></b></i> <br><br> Example:<br> <i>switch(config)#  nxsdk service-name MyApp profile permit-write </i>| Attach the security profile to an on box service.|
     | Step 3b | Example:<br> <i>switch(config)#  nxsdk remote port 50002 namespace default<br>switch(config-nxsdk-remote)#      service myRemoteApp <b>profile permit-write</b></i>| Attach the security profile to a remote service.|
     | Step 4  | To verify the profiles use <b> show nxsdk internal profile</b> <br><br> Example: <br> switch# show nxsdk internal profile<br> Service Profile:   permit-writes<br>----------------<br>NxSDK Manager      Write API Policy<br>--------------- --------------------------<br>NxCliParser     Allowed<br>NxRibMgr        Allowed<br>NxAdjMgr        Allowed<br>NxMacMgr        Allowed<br>NxIntfMgr       Allowed<br>NxDmeMgr        Allowed | To verify the created profiles. |
   
      
### Configuring a Security Profile for Throttling
   - You can also throttle the number of NX-SDK API write calls to the switch inorder to prevent flooding of requests.
   - In throttling mode by default it allows only 50 write API calls in 5 secs. When the maximum number of calls occurs, 
     any subsequent API requests are disallowed by sending an error message in the reply. After a cooldown period of 5  
     seconds, an application can make write API calls again.
   - In V2.0.0, throttling rate is fixed (50 write API Calls in 5 seconds) and is not user configurable (planned for 
     future versions).
   
     | Steps | Command or Action | Purpose |
     |-------|-------------------|---------|
     | Step 1 |  nxsdk profile <b><i>\<profile-name\></i></b> <br><br> Example: <br><i>switch(config)# nxsdk profile permit-writes-throttle</i> | Create an application profile. |
     | Step 2 | <b>system modifications throttle</b> <br><br> Example:<br> <i>switch(config-nxsdk-profile)# system modifications throttle </i>| Enable throttling of API writes that modify the switch. |
     | Step 3a | nxsdk service-name \<app-name\> <b><i>profile \<profile-name\></b></i> <br><br> Example:<br> <i>switch(config)#  nxsdk service-name MyApp profile permit-write-throttle </i> | Attach the security profile to an on box service. |
     | Step 3b | Example:<br> <i>switch(config)#  nxsdk remote port 50002 namespace default<br>switch(config-nxsdk-remote)#      service myRemoteApp <b>profile permit-write-throttle</b></i> | Attach the security profile to a remote service. |
     | Step 4  | To verify the profiles use <b> show nxsdk internal profile</b> <br><br> Example: <br> switch# show nxsdk internal profile<br> Service Profile:   permit-writes-throttle<br>----------------<br>NxSDK Manager      Write API Policy<br>--------------- --------------------------<br>NxCliParser     50 calls every 5 seconds<br>NxRibMgr        50 calls every 5 seconds<br>NxAdjMgr        50 calls every 5 seconds<br>NxMacMgr        50 calls every 5 seconds<br>NxIntfMgr       50 calls every 5 seconds<br>NxDmeMgr        50 calls every 5 seconds | To verify the created profiles. |
    
# Limitations and Restrictions
 - Security Profiles once applied to an application <b>cannot be modified</b>. To modify an already applied profile, 
   user must remove all the services attached to the profile to make any changes.  
 - Add (or) update a security profile to an already running service is prohibited. 

# Troubleshooting
  - Use "show nxsdk internal profile" to get the state of the profile.
  

