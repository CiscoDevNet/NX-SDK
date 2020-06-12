/** @file nx_trace.h
 *  @brief Abstract Tracer interface to generate custom Syslogs, Events & Errors 
 *   history in Nexus Switches.
 *
 *  Steps to construct a custom syslog, Events & Erros history:
 *   1) Instantiate NxSDK Tracer Object using getTracer().
 *   2) To generate custom syslogs, use TracerObj->syslog(..).
 *   3) To generate custom events, use TracerObj->event(...).
 *      To check the events, use "show <AppName> internal event-history events"
 *   4) To generate custom errors, use TracerObj->error(...).
 *      To check the events, use "show <AppName> internal event-history events"(
 *   5) Refer to examples for usage.
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 */

#ifndef __NX_TRACE_H__
#define __NX_TRACE_H__

#include <cstdio>
#include <string>
#include <cerrno>
#include <cstddef>
#include <stdint.h>
#include <iostream>

namespace nxos
{

/**
 * @brief syslog interface that hold syslog data.
 **/
class NxSyslog {
public:
   ///Destructor
   virtual ~NxSyslog() {};

   /**
    * Sample syslog: 2019 Apr  5 14:56:20 n9232c-kg2 %ETHPORT-5-IF_DOWN_ADMIN_DOWN: Interface loopback1000 is down (Administratively down)
    **/
   /// Get Message Id of the Syslog 
   virtual uint32_t    getId() =0;

   /// Get Node Name from Syslog Msg ex) n9232c-kg2
   virtual std::string getNodeName() =0;

   /// Get TimeStamp at which the Syslog is generated ex) 2019 Apr  5 14:56:20
   virtual std::string getTimeStamp() =0;
 
   /// Get the severity of the Syslog ex) 5
   virtual uint32_t    getSeverity() =0;

   /// Get the Syslog Message Name ex) IF_DOWN_ADMIN_DOWN
   virtual std::string getMessageName() =0;

   /// Get the Syslog Category ex) ETHPORT
   virtual std::string getCategory() =0;

   /// Get the Syslog Text ex) Interface loopback1000 is down (Administratively down)
   virtual std::string getText() =0;

   /**
    *  [Optional] Get the Time Zone abbreviations, e.g PST, EST, UTC etc.
    *  By default, timezone is not set in the syslog messages. To enable
    *  timezone in the syslog messages, use the config:
    *    switch(config)# clock format show-timezone syslog
    **/
   virtual std::string getTimeZone() =0;
};

/**
 * @brief Abstract Trace callback Interface to receive 
 *        syslog updates from NX-OS.
 **/
class NxTraceHandler {
public:
    /// Destructor
    virtual ~NxTraceHandler(){};

    /**
     * User to overload the postsyslogCb callback method
     * to receive any registered Syslog updates based
     * on the set watch syslog.
     *
     * @param[in]  NxSyslog object. Not valid outside of postSyslogCb.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_trace.h>
     *
     *     class myTraceHandler : public NxTraceHandler {
     *        public:
     *           void postSyslogCb(NxSyslog *syslog) {
     *                // Do your action
     *           }
     *     };
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class mySyslogHandler(nx_sdk_py.NxSyslogHandler):
     *     ### Overloaded Callback to get syslog events
     *           def postSyslogCb(self, nxsyslog):
     *               # Do your action here
     *  @endcode
     **/
    virtual void postSyslogCb(NxSyslog *syslog) {}

    /**
     * Callback used to mark the end of a requested object
     * download operation.  This will be called after the last
     * event is sent to the application.
     *
     * @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_trace.h>
     *
     *     class myTraceHandler : public NxTraceHandler {
     *        public:
     *           bool postSyslogDownloadDoneCb() { 
     *                cout << "Download complete for all existing syslogs" << endl;
     *           }
     *     };
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myTraceHandler(nx_sdk_py.NxTraceHandler):
     *           def postSyslogDownloadDoneCb():
     *               print "Download complete for all existing Syslogs\n"
     * @endcode
     **/
    virtual void postSyslogDownloadDoneCb()
    {
        return;
    }
};

/**
 * @brief Tracer interface to generate custom Syslogs, Events & Errors history.
 **/
class NxTrace
{
  public:
    /**
     * Priority level for the custom syslog.
     **/
    typedef enum
    {

        /// Emergency - Highest Priority
        EMERG,

        /// Its a severe Alert
        ALERT,

        /// Critical.
        CTRI,

        /// Error Scenario
        ERR,

        /// Warning but not critical
        WARNING,

        /// Notice Alert but not severe.
        NOTICE,

        /// For Information.
        INFO,

        /// For internal Debugging purposes.
        DEBUG,
    } Priority;

    /// Destructor
    virtual ~NxTrace() {}

    /// Generate Custom Syslog with the specified Priority
    virtual void syslog(Priority prio, const char *fmt, ...) = 0;

    /**
     * Generate custom App Event in its event history.
     * Use "show <AppName> internal event-history events".
     **/
    virtual void event(const char *fmt, ...) = 0;

    /**
     * Generate custom App Error in its event history.
     * Use "show <AppName> internal event-history errors".
     **/
    virtual void error(const char *fmt, ...) = 0;

    /**
     * @note Following APIs are supported from NXSDK V1.5 
     **/

    /**
     * Get the specified priority as String
     *
     * @since NXSDK V1.5
     *
     * @returns Priotiry as a string
     **/
    static const char *getPrioStr(Priority prio);

    /**
     * Watch All syslog and get notified when a syslog occurs. 
     * User must have postSyslogCb registered to receive events. 
     * NOTE:
     *    watchSyslog does not work if watchOcPath in NxOcMgr 
     *    for syslog is enabled.
     * 
     * @param[in]  download: [Optional] true or false. If true, all syslogs already 
     *                       created in the system will be notified via 
     *                       postSyslogCb and the end of download is signalled
     *                       by overloading the respective postSyslogDownloadDone 
     *                       callback. 
     *                        
     * @returns bool True if the watch is successful.
     *               False if its unsuccessful. 
     *
     * @since NXSDK V2.5
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_trace.h>
     *
     *       NxTrace *trace;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       trace = sdk->getTracer();
     *
     *       if (trace) trace->watchSyslog();
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       trace = sdk.getTracer()
     *       if trace:
     *          trace.watchSyslog()
     *  @endcode
     *
     **/
    virtual bool watchSyslog(bool download = true) = 0;

    /**
     * UnWatch syslog events. Stop receiving syslog events. 
     *
     * @since NXSDK V2.5
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_trace.h>
     *
     *       NxTrace *trace;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       trace = sdk->getTracer();
     *
     *       trace->unwatchSyslog();
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       tracer = sdk.getTracer()
     *
     *       tracer.unwatchSyslog()
     *  @endcode
     **/
    virtual void unwatchSyslog() = 0;

    /**
     * Register your handler object with Tracer to
     * handle syslog events.
     * @param[in]  NxTraceHandler object with user overloaded
     *             functions to handle syslog events.
     *
     * @since NXSDK V2.5
     * 
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_trace.h>
     *
     *     class myTraceHandler : public NxTraceHandler {
     *        public:
     *           void postSyslogCb(NxSyslog *syslog) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxTrace  *trace = sdk->getTracer();
     *          NxTraceHandler *mytracecb = new myTraceHandler();
     *          trace->setTraceHandler(mytracecb);
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myTraceHandler(nx_sdk_py.NxTraceHandler):
     *           ### Overloaded Callback to get Syslog events
     *           def postSyslogCb(self, nxsyslog):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         trace = sdk.getTracer()
     *         mytracecb = myTraceHandler()
     *         trace.setTraceHandler(mytracecb)
     *  @endcode
     **/
    virtual void setTraceHandler(NxTraceHandler *handler) = 0;

    /**
     * Get the Handler object registered with Tracer.
     *
     * @since NXSDK V2.5
     *
     * @returns user defined NxTraceHandler Obj if registered using
     *          setTraceHandler (or) NULL.
     **/
    virtual NxTraceHandler *getTraceHandler() = 0;
};

} // namespace nxos

#endif //__NX_TRACE_H__
