/*********************************************************************
 *
 **File        :   nx_cli_impl.h
 *
 **Description :
 *   Stub Syslog/Debug Implementation header file
 *
 **Copyright (c) 2016-2017 by cisco Systems, Inc.
 **All rights reserved.
 *
 *
 **$Id         : $
 * $Source     : $
 * $Author     : $
 *
 *********************************************************************
 */

#ifndef __NX_TRACE_IMPL_H__
#define __NX_TRACE_IMPL_H__

#include "nx_trace.h"

namespace nxos {

class NxTraceImpl: public NxTrace {
public:
    ~NxTraceImpl();
    void syslog(Priority, const char *fmt, ...);
    void event(const char *fmt, ...);
    void error(const char *fmt, ...);
};

}

#endif //__NX_TRACE_IMPL_H__
