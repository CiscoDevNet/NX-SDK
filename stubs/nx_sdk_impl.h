// ********************************************************************
//  *
//  * File:   nx_sdk_impl.h
//  *
//  * Description:
//  *  Stubs
//  *
//  * Copyright (c) 2016 by cisco Systems, Inc.
//  * All rights reserved.
//  *
//  *
//  * $Id: $
//  * $Source: $
//  * $Author: $
//  *
//  *********************************************************************

#ifndef __NX_SDK_IMPL_H__
#define __NX_SDK_IMPL_H__

#include "nx_sdk.h"

namespace nxos {

class NxSdkImpl: public NxSdk {
public:

    /// Destructor
    ~NxSdkImpl();

    /// [Optional] Sdk Desc setter function
    void setAppDesc(std::string desc);

    /// Sdk Name getter function
    const char *getAppName();

    /// Sdk Desc getter function
    const char *getAppDesc();

    /// Sdk Id setter function
    int        getAppId();

    /// Cli Parser object getter function
    NxCliParser *getCliParser();

    /// Tracer syslog object getter function
    NxTrace     *getTracer();

    /// Start Event loop
    void startEventLoop();

    /// Stop Event loop
    void stopEventLoop();

    /// Get Rib Mgr Module
    NxRibMgr *getRibMgr();
    
    /// Set App priority
    void setAppPriority(nxos::prio_e prio);
};

}

#endif //__NX_SDK_IMPL_H__
