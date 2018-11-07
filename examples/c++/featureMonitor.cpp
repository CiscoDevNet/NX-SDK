/*********************************************************************
 *
 * File: featureMonitor.cpp
 *
 * Description:  Example C++ application that uses the DME event
 *               feature of NxSDK to monitor the configuration object
 *               store for features.  If a feature is enabled or 
 *               disabled, a DME event is triggered by the switch 
 *               and captured by this application, which prints a 
 *               message about the event.
 *
 *  Supported since NX-SDK v1.7.5
 *
 * Copyright (c) 2018 by cisco Systems, Inc.
 * All rights reserved.
 *
 *
 * $Id: $
 * $Source: $
 * $Author: $
 *
 *********************************************************************
 */

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <nx_sdk.h>
#include <nx_trace.h>
#include <nx_dme.h>
#include <stdexcept>
#include <nx_common.h>
#include <pthread.h>
#include <vector>


using namespace std;
using namespace nxos;



NxIntfMgr *intfMgr;
NxSdk    *sdk;
NxTrace  *traceP;
NxDmeMgr       *dmeP;
NxDmeMgrHandler *dmeHandler;



static const char *
event_str ( event_type_e  event)
{
    switch (event) {
    case ADD: return "Add";
    case DELETE: return "Delete";
    case UPDATE: return "Update";
    default: return "Unknown";
    }
}


class myNxDmeHandler : public NxDmeMgrHandler {
public:

    void postDmeHandlerCb (NxDme *objp)
    {
        std::string value;
        std::string feature;
        std::string status;
        std::string data;
        int  err;

        traceP->event("Feature monitor callback: dn=%s event=%s",
                      objp->getDn().c_str(), event_str(objp->getEvent()));

        traceP->syslog(NxTrace::NOTICE, "Feature monitor callback: dn=%s event=%s",
                      objp->getDn().c_str(), event_str(objp->getEvent()));
                       
        data = objp->getDataJson();
        traceP->syslog(NxTrace::NOTICE, "MO json data = %s", data.c_str());
                       
        if (data.length()) {
            
            feature = objp->getProperty("rn", &err);
            status = objp->getProperty("adminSt", &err);
            traceP->syslog(NxTrace::NOTICE, "Feature \"%s\"  status is %s", 
                           feature.c_str(), status.c_str());
        }
    }
};



int main (int argc, char *argv[], char *env[])
{
  
    int i = 0;


    sdk = nxos::NxSdk::getSdkInst(argc, argv, true);

    if (!sdk) {
        cout << "Could not get access to Nx Infra SDK" << endl;
        exit(-1);
    }
    
    
    traceP = sdk->getTracer();
    if (!traceP) {
        cout << "Could not get access to Tracer" << endl;
        exit(-1);
    }
    traceP->syslog(NxTrace::EMERG, "Appname: %s", sdk->getAppName());
 
    dmeP = sdk->getDmeMgr();
    if (!dmeP) {
        traceP->syslog(NxTrace::EMERG,"Could not get access to DME");
        exit(-1);
    }
    
    // regiser for the dme events on feature manager "sys/fm"
    dmeP->watch("sys/fm");
    dmeHandler = new myNxDmeHandler();
    dmeP->setDmeHandler(dmeHandler);
    
    traceP->syslog(NxTrace::NOTICE, "Starting event procesing...");
    sdk->startEventLoop();

    
    delete sdk;
}
