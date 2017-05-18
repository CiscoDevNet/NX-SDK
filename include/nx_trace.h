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

namespace nxos {

/**
 * @brief Tracer interface to generate custom Syslogs, Events & Errors history.
 **/
class NxTrace {
public:

    /**
     * Priority level for the custom syslog.
     **/
    typedef enum { 
                    EMERG, 
                    ALERT,
                    CTRI,
                    ERR,
                    WARNING,
                    NOTICE,
                    INFO,
                    DEBUG,
                 } Priority;

    /// Destructor
    virtual ~NxTrace() {}
    
    /// Generate Custom Syslog with Priority 
    virtual void syslog(Priority prio, const char *fmt, ...)=0;

    /**
     * Generate custom App Event in its event history.
     * Use "show <AppName> internal event-history events".
     **/
    virtual void event(const char *fmt, ...)=0;

    /**
     * Generate custom App Error in its event history.
     * Use "show <AppName> internal event-history errors".
     **/
    virtual void error(const char *fmt, ...)=0;
};
}

#endif //__NX_TRACE_H__
