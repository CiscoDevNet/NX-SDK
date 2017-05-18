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

namespace nxos {

/// Forward Declaration - Refer to nx_cli.h
class NxCliParser;

/// Forward Declaration - Refer to nx_trace.h
class NxTrace;

/**
 *  @brief Abstract SDK interface to NXOS infra
 */
class NxSdk {
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
     *            the thrid party applications to 
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
     **/
    static NxSdk *getSdkInst(int argc, char **argv);

    /**
     * [Optional] Set the description for the Application.
     *            This description will be used as the
     *            helpstring when generating custom CLIs
     *            for the AppName keyword in the config.
     **/
    virtual void setAppDesc(std::string desc)=0;

    /**
     * Get the App Name which is the name of App binary.
     **/
    virtual const char *getAppName()=0;

    /**
     *  Get App Description. If not set the default description
     *  is "NxSDK Application"
     **/
    virtual const char *getAppDesc()=0;

    /**
     * Get App ID. App ID is automatically set by the sysmgr
     * if the service is started from VSH [recommended]. 
     **/
    virtual int getAppId()=0;

    /**
     * Get Nx Cli Parser object to create custom CLIs.
     * This automatically generates internal show commands
     * for the App. Use "show <AppName> ?" in VSH to 
     * navigate to the pre-generated commands for the App.
     **/
    virtual NxCliParser *getCliParser()=0;

    /**
     * Get Nx Tracer syslog object to generate custom syslogs,
     * custom App Event history logs. 
     **/
    virtual NxTrace     *getTracer()=0;

    /**
     * [Required] Start Event loop. This keeps the Application running.
     **/
    virtual void startEventLoop()=0;

    /**
     * Stop Event loop to quit the Application.
     **/
    virtual void stopEventLoop()=0;
};

}

#endif //__NX_SDK_H__
