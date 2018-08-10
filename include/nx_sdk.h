/** @file nx_sdk.h
 *  @brief Abstract SDK interface to Nexus(NXOS) tools
 *
 *  NX-SDK v1.0.0 provides a flexible and powerful way for third party
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
     * @note From v1.5.0, by default an NX-SDK App will be started as Low Priority App.
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
     * @note Following APIs are supported from NX-SDK v1.5.0
     **/

    /**
     * [Required] To get Nx SDK Instance which enables the third
     *            party applications to gain access to NXOS Infra
     *            functionalities.
     * @since NX-SDK v1.5.0
     *
     * @note - Supported from NxSDK v1.5.0. Similar to getSdkInst
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
     *                          Note: Supported from v1.5.0
     *
     * @code
     *  C++:
     *       sdk = nxos::NxSdk::getSdkInst(argc, argvi, true);
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True)
     * @endcode
     *
     * @returns  - SDK instance to operate on (registered with NXOS Infra successfully).
     *           - NULL in case of any errors and there will be a Error syslog on the failure.
     *
     * @note From v1.5.0, by default an NX-SDK App will be started as Low Priority App.
     *       To change the priority, refer to setAppPriority().
     **/
    static NxSdk *getSdkInst(int argc, char **argv, bool advException);

    /**
     * Get Nx Rib Manager object to update and register for route events.
     *
     * @since NX-SDK v1.5.0
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
     * @since NX-SDK v1.5.0
     **/
    virtual void setAppPriority(nxos::prio_e prio) = 0;
};

} // namespace nxos

#endif //__NX_SDK_H__
