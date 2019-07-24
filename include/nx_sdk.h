/** @file nx_sdk.h
 *  @brief Abstract SDK interface to Nexus(NXOS) tools
 *
 *  NXOS Infra SDK V1.0 provides a flexible and powerful way for third party 
 *  custom Application development to access Nexus infra tools like  
 *      1. generating Custom CLIs 
 *      2. Custom Syslogs, Events and Error history for the App.
 *      3. Ability to start/stop the Application from VSH
 *         thereby providing HA support.
 *  just like any other native Nexus applications. 
 *  Refer to some example Applications. 
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 */

#ifndef __NX_SDK_H__
#define __NX_SDK_H__

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include "types/nx_common.h"
#include "nx_exception.h"

namespace nxos
{

/// Forward Declaration - Refer to nx_cli.h
class NxCliParser;

/// Forward Declaration - Refer to nx_trace.h
class NxTrace;

/// Forward Declaration - Refer to nx_rib_mgr.h
class NxRibMgr;

class NxDmeMgr;

class NxIntfMgr;

/// Forward Declaration - Refer to nx_adj_mgr.h
class NxAdjMgr;

/// Forward Declaration - Refer to nx_mac_mgr.h
class NxMacMgr;

/// Forward Declaration - Refer below.
class NxSdkHandler;

/**
 *  @brief Abstract SDK interface to NXOS infra
 */
class NxSdk
{
  public:
    /**
     * [Required] Destructor
     *
     * To Delete the SDK instance and its associated objects gracefully
     * on App exit. 
     **/
    virtual ~NxSdk() {}

    /**
     * [Required] To get Nx SDK Instance which enables
     *            the third party applications to 
     *            gain access to Nexus functionalities.
     * @note Do all SDK specific initializations in the 
     *       same thread.
     * @param[in] pass in the command line arguments as its
     *            needed when the App is started from VSH.
     *
     * @code
     *  C++:
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     * @endcode
     * @returns  - SDK instance to operate on (registered with NXOS Infra successfully).
     *           - NULL in case of any errors and there will be a Error syslog on the failure.
     * 
     * @note From V1.5, by default an NXSDK App will be started as Low Priority App.
     *       To change the priority, refer to setAppPriority(). 
     **/
    static NxSdk *getSdkInst(int argc, char **argv);

    /**
     * [Optional] Set the description for the Application.
     *            This description will be used as the
     *            helpstring when generating custom CLIs
     *            for the AppName keyword in the config.
     **/
    virtual void setAppDesc(std::string desc) = 0;

    /**
     * Get the App Name which is the name of App binary.
     **/
    virtual const char *getAppName() = 0;

    /**
     *  Get App Description. If not set the default description
     *  is "NxSDK Application"
     **/
    virtual const char *getAppDesc() = 0;

    /**
     * Get App ID. App ID is automatically set by the sysmgr
     * if the service is started from VSH [recommended]. 
     **/
    virtual int getAppId() = 0;

    /**
     * Get Nx Cli Parser object to create custom CLIs.
     * This automatically generates internal show commands
     * for the App. Use "show <AppName> ?" in VSH to 
     * navigate to the pre-generated commands for the App.
     **/
    virtual NxCliParser *getCliParser() = 0;

    /**
     * Get Nx Tracer syslog object to generate custom syslogs,
     * custom App Event history logs. 
     **/
    virtual NxTrace *getTracer() = 0;

    /**
     * [Required] Start Event loop. This keeps the Application running.
     **/
    virtual void startEventLoop() = 0;

    /**
     * Stop Event loop to quit the Application.
     **/
    virtual void stopEventLoop() = 0;

    /**
     * @note Following APIs are supported from NXSDK V1.5 
     **/

    /**
     * [Required] To get Nx SDK Instance which enables the third
     *            party applications to gain access to NXOS Infra 
     *            functionalities.
     * @since NXSDK V1.5
     *
     * @note - Supported from NxSDK V1.5. Similar to getSdkInst 
     *         except the addition of new parameter advException. 
     *       - Do all SDK specific initializations in the same thread.
     *
     * @param[in] pass in the command line arguments as its
     *            needed when the App is started from VSH.
     * @param[in] advException: TRUE -  If the App can handle Advanced
     *                                  Exceptions then Exception will be
     *                                  thrown of the type NxException.
     *                          FALSE - Default Mode. Exception will be
     *                                  thrown of the type 
     *                                  C++ - std::logic_err, Python - String
     *                          Note: Supported from V1.5
     *
     * @code
     *  C++:
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv, true);
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True)
     * @endcode
     *
     * @returns  - SDK instance to operate on (registered with NXOS Infra successfully).
     *           - NULL in case of any errors and there will be a Error syslog on the failure.
     * 
     * @note From V1.5, by default an NXSDK App will be started as Low Priority App.
     *       To change the priority, refer to setAppPriority(). 
     **/
    static NxSdk *getSdkInst(int argc, char **argv, bool advException);

    /**
     * Get Nx Rib Manager object to update and register for route events.
     *
     * @since NXSDK V1.5
     *
     * @returns Pointer of type NxRibMgr.
     **/
    virtual NxRibMgr *getRibMgr() = 0;

    /**
     * [Optional] To Set the Priority of the application to limit the 
     * use of CPU for the application. By default, the App is set as 
     * low priotiy Application(only 25% of CPU).
     *     LOW_PRIO  - 25% of CPU 
     *     MED_PRIO  - 50% of CPU
     *     HIGH_PRIO - 75% of CPU
     *     NO_PRIO   - No limit 
     *
     * @since NXSDK V1.5
     **/
    virtual void setAppPriority(nxos::prio_e prio) = 0;

 /**
     * Get Nx Interface Manager object to get, update and register for interface events.
     *
     * @since NX-SDK V1.7.5
     *
     * @code
     *   C++:
     *          sdk = nxos::NxSdk::getSdkInst(argc, argv, true);
     *          intfMgr = sdk->getIntfMgr();
     *   Python:
     *          sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *          intfMgr = sdk.getIntfMgr()
     *
     * @endcode
     *
     * @returns Pointer of type NxIntfMgr if valid (or) NULL
     **/
    virtual NxIntfMgr *getIntfMgr() = 0;

    /**
     * Get Nx Adjacency Manager object to get and register for adjacency events.
     *
     * @since NX-SDK V1.7.5
     *
     * @returns Pointer of type NxAdjMgr if valid (or) NULL
     **/
    virtual NxAdjMgr *getAdjMgr() = 0;

    /**
     * Get Nx Mac Manager object to get, update and register for mac events.
     *
     * @since NX-SDK V1.7.5
     *
     * @returns Pointer of type NxMacMgr if valid (or) NULL
     **/
    virtual NxMacMgr *getMacMgr() = 0;

    /**
     * Get Nx DME Manager object to get, update and register for DME objectore events.
     *
     * @since NX-SDK V1.7.5
     * @returns Pointer of type NxDme if valid (or) NULL
     **/
    virtual NxDmeMgr *getDmeMgr() = 0;

    /**
     * Returns true if event loop has been started
     *
     * @since NX-SDK V1.7.5
     *
     * @returns true or false
     **/
    virtual bool isInEventLoop() const = 0;

    /**
     * Returns running environment
     *
     * @since NX-SDK V1.7.5
     *
     * @returns running_env_r (BASH or VSH)
     **/
    virtual nxos::running_env_e getRunningEnv() const = 0;

    /**                                                                                       
     * [Required] To get Nx SDK Instance which enables the third                              
     *            party applications to gain access to NXOS Infra                             
     *            functionalities.                                                            
     * @since NXSDK V2.0                                                                      
     *                                                                                        
     * @note - Earlier variants supported from NxSDK V1.5. This version
     *         adds two new parameters for remote ip/port for applications
     *         executing outside the switch.  It is the same in all other
     *         respects as prior variants so refer to those for additional
     *         information.

     *                                                                                        
     * @param[in] pass in the command line arguments as its                                   
     *            needed when the App is started from VSH.                                    
     * @param[in] advException: TRUE -  If the App can handle Advanced                        
     *                                  Exceptions then Exception will be                     
     *                                  thrown of the type NxException.                       
     *                          FALSE - Default Mode. Exception will be                       
     *                                  thrown of the type                                    
     *                                  C++ - std::logic_err, Python - String                 
     *                          Note: Supported from V1.5            
     * @param[in] remoteIp IPv4 (or) IPv6 of the remote switch to connect to.
     *                     Used only if the NX-SDK app is running remotely.Its 
     *                     a no-op if the app is running natively in the switch.                          
     *
     * @param[in] remotePort Port in the remote switch to connect to. To be used only                         
     *                       if the NX-SDK app is running remotely.Its a no-op 
     *                       if the app is running natively in the switch. 
     *                                                        
     * @returns  - SDK instance to operate on (successfully registered with NXOS Infra remotely).
     *             An App can connect to multiple switches by specifying different IP addresses
     *             and port as shown below thereby acquiring SDK instances per switch respectively. 
     *             But an NX-SDK App running natively in the switch can acquire only "ONE" 
     *             SDK instance. 
     *           - NULL in case of any errors and are thrown as exceptions.
     *
     * @throws ERR_INVALID Connection could not be established with the switch.
     *
     *
     * @code                                                                                  
     *  C++:                                                                                  
     *       sdk_switchA  = nxos::NxSdk::getSdkInst(argc, argv, true, "172.22.32.1", 50001);                                 
     *       sdk_switchB  = nxos::NxSdk::getSdkInst(argc, argv, true, "172.22.32.2", 50002); 
     *                                                                                        
     *  Python:                                                                               
     *       import nx_sdk_py                                                                 
     *       sdk_switchA = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True, "172.22.32.1", 50001)                  
     *       sdk_switchB = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True, "172.22.32.2", 50002)   
     * @endcode                                                                               
     *                                                                                        
     **/                                                             
    static NxSdk *getSdkInst(int argc, char **argv, bool advException,
                             const char* remoteIp, uint32_t remotePort);
			     

    /**                                                                                                   
     * To get the destination switch remote IP address associated
     * with this SDK instance.
     *
     * @returns Destination remote IP address if the app is running
     *          remotely. Returns empty string if app is running 
     *          natively in the switch.
     *                                                                                                    
     * @since NX-SDK V2.0                                                                                 
     *                                                                                                    
     *  @code                                                                                             
     *  C++:                                                                                              
     *       cout << "Dest Ip: " << sdk->getRemoteIp() << "\n";            
     *                                                                
     *  Python:                                                       
     *     print "Dest Ip " + sdk.getRemoteIp() 
     *                                                                
     *  @endcode                                                        
     **/     
    virtual std::string getRemoteIp() = 0;

    /**                                                                                                   
     * To get the destination switch remote Port associated                                         
     * with this SDK instance.                                                                            
     *                                                                                                    
     * @returns Destination remote Port if the app is running                                       
     *          remotely. Returns 0 if app is running                                          
     *          natively in the switch.                                                                   
     *                                                                                                    
     * @since NX-SDK V2.0                                                                                 
     *                                                                                                    
     *  @code                                                                                             
     *  C++:                                                                                              
     *       cout << "Dest Port: " << sdk->getRemotePort() << "\n";                                             
     *                                                                                                    
     *  Python:                                                                                           
     *     print "Dest Ip " + sdk.getRemotePort()                                                             
     *                                                                                                    
     *  @endcode                                                                                          
     **/                                                                                         
    virtual uint32_t  getRemotePort() = 0;

    /**                                                                                                   
     * To get the destination switch remote connection status       
     * associated with this SDK instance.                                          
     *                                                                  
     * @returns True  - if the remote connection is Up.
     *          False - if the remote connection is Down.      
     *          For apps running natively it always return True.
     *                                                             
     * @since NX-SDK V2.0                                          
     *                                                             
     *  @code                                                         
     *  C++:                                                       
     *       cout << "Remote Connection status: " << sdk->getRemoteConnStatus() << "\n";                         
     *                                                                                        
     *  Python:                                                 
     *     print "Remote Connection Status:  " + sdk.getRemoteConnStatus()                                     
     *                                                           
     *  @endcode                                               
     **/                                                       
    virtual nxos::state_type_e getRemoteConnStatus() = 0;   

    /**                                                
     * This API will purge all the objects created under that SDK instance.
     * Use it with caution.
     *                                      
     * To be used in cases like,              
     *    On postRemoteConnDownCb, all the objects are stale under  
     *    that SDK instance. This API will purge all the objects.
     *    NOTE: When the remote connection goes down and comes back up,
     *          it is mandatory to call this API before re-initializing
     *          with all the NX-SDK managers. Without this API, app
     *          will not be able to re-init with all the NX-SDK mgrs
     *          on postRemoteConnUpCb.
     *                      
     * NOTE: Refer to sample remote applications on the usage.
     * 
     * @since NX-SDK V2.0                             
     *                                                        
     *  @code                                        
     *  C++:                                                
     *                                                    
     *  Python:                                         
     *                                                            
     *  @endcode                                                        
     **/                                                   
    virtual void purgeAllSdkObjs() = 0;

    /**
     * Users to overload NxSdk Handler class to handle regsiterd 
     * NxSdk events. 
     * @param[in]  NxSdkHandler object with user overloaded
     *             functions to handle Nxsdk events.
     *
     * @since NX-SDK V2.0                                                                         
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *
     *     class mySdkHandler : public NxSdkHandler {
     *        public:
     *           void postRemoteConnUpCb(NxSdk *sdk) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          if (sdk) {
     *              NxSdkHandler *mysdkcb = new mySdkHandler();
     *              sdk->setSdkHandler(mysdkcb);
     *          }
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class mySdkHandler(nx_sdk_py.NxSdkHandler):
     *     ### Overloaded Callback to get Sdk events
     *           def postRemoteConnUpCb(self, nxsdk):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         if sdk:
     *            mysdkcb = mySdkHandler()
     *            sdk.setSdkHandler(mysdkcb)
     *  @endcode
     **/
    virtual void setSdkHandler(NxSdkHandler *handler) = 0;

    /**
     * Unset NxSdk Handler
     *
     * @since NX-SDK V2.0                                                                         
     *
     **/
    virtual void unsetSdkHandler() = 0;

    /**
     * Get the Handler object registered with NxSdk.
     *
     * @returns NxSdkHandler user overloaded obj.
     *
     * @since NX-SDK V2.0                                                                         
     *
     **/
    virtual NxSdkHandler *getSdkHandler() = 0;

    /**                                                                                       
     * [Required] To get Nx SDK Instance which enables the third                              
     *            party applications to gain access to NXOS Infra                             
     *            functionalities.                                                            
     * @since NXSDK V2.0                                                                      
     *                                                                                        
     * @note - Earlier variants supported from NxSDK V1.5. This version
     *         adds two new parameters for remote ip/port for applications
     *         executing outside the switch.  It is the same in all other
     *         respects as prior variants so refer to those for additional
     *         information.

     *                                                                                        
     * @param[in] pass in the command line arguments as its                                   
     *            needed when the App is started from VSH.                                    
     * @param[in] advException: TRUE -  If the App can handle Advanced                        
     *                                  Exceptions then Exception will be                     
     *                                  thrown of the type NxException.                       
     *                          FALSE - Default Mode. Exception will be                       
     *                                  thrown of the type                                    
     *                                  C++ - std::logic_err, Python - String                 
     *                          Note: Supported from V1.5            
     * @param[in] remoteIp IPv4 (or) IPv6 of the remote switch to connect to.
     *                     Used only if the NX-SDK app is running remotely.Its 
     *                     a no-op if the app is running natively in the switch.                          
     *
     * @param[in] remotePort Port in the remote switch to connect to. To be used only                         
     *                       if the NX-SDK app is running remotely.Its a no-op 
     *                       if the app is running natively in the switch. 
     *
      * @param[in] certificate  The path to the certificate file to use for the           
     *                         connection with the switch.  This is not strictly         
     *                         required as there are other ways to provide the           
     *                         certificate.  Using the certificate is required,          
     *                         and below are the ordered methods to specify the          
     *                         certificate location:                                     
     *                              1- The value of this parameter if non-empty       
     *                              2- $NXSDK_SERVER_CERT_A.B.C.D, where A_B_C_D 
     *                                     is the ip address of the switch                                                                 
     *                              3- $NXSDK_SERVER_CERT                                
     *                              4- "nxsdkTmpCert.pem" in app current directory    
     *                                                        
     * @returns  - SDK instance to operate on (successfully registered with NXOS Infra remotely).
     *             An App can connect to multiple switches by specifying different IP addresses
     *             and port as shown below thereby acquiring SDK instances per switch respectively. 
     *             But an NX-SDK App running natively in the switch can acquire only "ONE" 
     *             SDK instance. 
     *           - NULL in case of any errors and are thrown as exceptions.
     *
     * @throws ERR_INVALID Connection could not be established with the switch.
     *
     *
     * @code                                                                                  
     *  C++:                                                                                  
     *       sdk_switchA  = nxos::NxSdk::getSdkInst(argc, argv, true, "172.22.32.1", 50001, "/tmp/switchA_cert.pem");                                 
     *       sdk_switchB  = nxos::NxSdk::getSdkInst(argc, argv, true, "172.22.32.2", 50002, "/tmp/switchB_cert.pem"); 
     *                                                                                        
     *  Python:                                                                               
     *       import nx_sdk_py                                                                 
     *       sdk_switchA = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True, "172.22.32.1", 50001, "/tmp/switchA_cert.pem")                  
     *       sdk_switchB = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True, "172.22.32.2", 50002, "/tmp/switchB_cert.pem")   
     * @endcode                                                                               
     *                                                                                        
     **/                                                             
    static NxSdk *getSdkInst(int argc, char **argv, bool advException,
                             const char* remoteIp, uint32_t remotePort,
			     const char *certificate);

};

/**
 * @brief Abstract NX-SDK callback Interface to receive 
 *        SDK updates.
 **/
class NxSdkHandler
{
public:
    virtual ~NxSdkHandler(){};

    /**                                                                                           
     * Users to overload postRemoteConnUpCb in NxSdk Handler 
     * class to handle remote connection UP event. This is
     * to indicate that the connection went down and has
     * come up again. Apps should re-init with all the mgrs
     * as its a stateless connection(all the inits and objects
     * obtained prior to connection going down are obsolete
     * now and Apps need to re-init to get back to where they
     * left before the connection went down.
     *                                            
     * @param[in]  NxSdk object 
     *                                                                  
     * @note To be used only for apps running remotely. For native
     *       apps its a no-op.
     *
     * @since NX-SDK V2.0                                                                         
     *
     *  @code                                                                           
     *  C++:                                                                                  
     *     #include <nx_sdk.h>                                          
     *                                                                              
     *     class mySdkHandler : public NxSdkHandler {               
     *        public:                                           
     *           void postRemoteConnUpCb(NxSdk *sdk) {         
     *                // Do your action                        
     *           }                                               
     *     };                                                         
     *                                                                
     *  Python:                                                     
     *     import nx_sdk_py                                                 
     *                                                                             
     *     class mySdkHandler(nx_sdk_py.NxSdkHandler):              
     *     ### Overloaded Callback to get Sdk events                        
     *           def postRemoteConnUpCb(self, nxsdk):                       
     *               # Do your action here                              
     *                                                      
     *  @endcode                                                                                  
     **/                
    virtual void postRemoteConnUpCb(NxSdk *sdk) {}

    /**                                                                 
     * Users to overload postRemoteConnDownCb in NxSdk Handler                            
     * class to handle remote connection DOWN event.                                            
     * 
     * @param[in]  NxSdk object                                                    
     *                                                              
     * @note To be used only for apps running remotely. For native                                
     *       apps its a no-op. 
     *
     * @since NX-SDK V2.0                                                                         
     *
     *  @code                                                       
     *  C++:                                                        
     *     #include <nx_sdk.h>                                          
     *                                                                  
     *     class mySdkHandler : public NxSdkHandler {                   
     *        public:                                                   
     *           void postRemoteConnDownCb(NxSdk *sdk) {                  
     *                // Do your action                                  
     *           }                                                      
     *     };                                                            
     *                                                                   
     *  Python:                                                         
     *     import nx_sdk_py                                             
     *                                                                       
     *     class mySdkHandler(nx_sdk_py.NxSdkHandler):                       
     *     ### Overloaded Callback to get Sdk events                    
     *           def postRemoteConnDownCb(self, nxsdk):                       
     *               # Do your action here                                         
     *                                                                  
     *  @endcode                                                            
     **/                                                                    
    virtual void postRemoteConnDownCb(const NxSdk *sdk) {}    
};

} // namespace nxos

#endif //__NX_SDK_H__
