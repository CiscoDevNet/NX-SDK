/***************************************************************************
 * File:   featureMonitorGo
 *
 * Supported since NX-SDK v1.7.5
 *
 * Description:
 *     A simple NX-SDK GO application to demonstrate the usage 
 *     of NX-SDK DME manager to monitor for different feature enable 
 *     or disable events.
 *
 *     After starting the app, add or delete a feature from the switch
 *     CLI.  A message is displayed for the event.
 *     ex) switch(config)# feature bgp 
 *         switch(config)# feature ospf
 *         switch(config)# no feature bgp
 *
 * Copyright (c) 2018 by cisco Systems, Inc.
 * All rights reserved.
 *
 *
 **************************************************************************/

package main

import (
     "nx_sdk_go"
     "os"
     "fmt"
)

/*
 * Global Variables.
 */
var trace     nx_sdk_go.NxTrace
var dmeMgr    nx_sdk_go.NxDmeMgr

/*
 * Handle DmeMgr Callback.
 */
func (om *NxDmeMgrHandlerGoFunc) PostDmeHandlerCb(nxDme nx_sdk_go.NxDme) {
    trace.Event(fmt.Sprintf("[%s] ", nxDme.GetDn()))
}


func main() {

    sdk_obj := nx_sdk_go.NxSdkGetSdkInst(len(os.Args), os.Args, true)
    if sdk_obj.Swigcptr() == 0 {
       os.Exit(1) 
    }
    defer nx_sdk_go.DeleteNxSdk(sdk_obj)

    trace = sdk_obj.GetTracer()
    if trace.Swigcptr() == 0 {
       os.Exit(2) 
    }

    desc := "Sample Go application demonstrating DME events for feature mgr "
    sdk_obj.SetAppDesc(desc)

    // Registering for DME Events
    var mydmecb = NewNxDmeMgrHandlerGo()
    dmeMgr = sdk_obj.GetDmeMgr()
    if dmeMgr.Swigcptr() != 0 {
       dmeMgr.Watch("sys/fm")
       if mydmecb == nil {
          trace.Error("Dme callback install failed")
          os.Exit(3) 
       }
       dmeMgr.SetDmeHandler(mydmecb)
       fmt.Println("set the handler")
    } else {
       trace.Error("DmeMgr Init failed")
    }

    /**
     * To log a custom syslog based on some Events.
     **/
    trace.Syslog(nx_sdk_go.NxTraceINFO, fmt.Sprintf("Started App <%s>", sdk_obj.GetAppName()))

    /* 
     * To log Trace events/errors
     */
    trace.Event("Event: starting event loop, waiting for feature actvity");

    sdk_obj.StartEventLoop()
}
