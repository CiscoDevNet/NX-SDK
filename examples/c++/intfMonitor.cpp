/*********************************************************************
 *
 * File: intfMonitor.cpp
 *
 *  Supported since NX-SDK v1.7.5
 *
 * Description:  Example C++ application that uses NxSDK interface
 *               manager to monitor all interfaces for events.  If
 *               an event occurs, the application callback is called
 *               and a message is displayed.
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
#include <nx_cli.h>
#include <nx_intf_mgr.h>
#include <stdexcept>
#include <nx_common.h>
#include <pthread.h>
#include <vector>


using namespace std;
using namespace nxos;

void
query_interface (NxIntf* objp);


NxIntfMgr *intfMgr;
NxSdk    *sdk;
NxTrace  *traceP;


static const char *intf_str[] = {
    "NX_TYPE_UNKNOWN",
    "NX_TYPE_ETH",
    "NX_TYPE_SVI",
    "NX_TYPE_ETH_PORT_CHANNEL",
    "NX_TYPE_LOOPBACK",
    "NX_TYPE_SUBINTF",
    "NX_TYPE_TUNNEL",
    "NX_TYPE_MGMT"
};

static const char *speed_str (uint32_t speed)
{
    switch (speed) {
    case INTF_SPEED_AUTO:
        return "auto";
    case INTF_SPEED_AUTO_100:
        return "auto 100M";
    case INTF_SPEED_AUTO_100_1000:
        return "auto 100M 1G";
    case INTF_SPEED_100M:
        return "100M";
    case INTF_SPEED_1G:
        return "1G";
    case INTF_SPEED_10G:
        return "10G";
    case INTF_SPEED_100G:
        return "100G";
    case INTF_SPEED_25G:
        return "25G";
    case INTF_SPEED_40G:
        return "40G";
    default:
        return "Unknown"; 
    }
}




class myIntfHandler : public NxIntfMgrHandler {
public:
    bool postIntfAddDelCb (NxIntf *intf);
    bool postIntfStateCb   (NxIntf *intf);
    bool postIntfLayerCb  (NxIntf *intf);
    bool postIntfIpv4AddrCb (NxIntf *intf);
    bool postIntfIpv6AddrCb (NxIntf *intf);
    bool postIntfPortMemberCb (NxIntf *intf);
    bool postIntfVrfCb  (NxIntf *intf);
    bool postIntfVlanCb  (NxIntf *intf);
};

bool 
myIntfHandler::postIntfLayerCb  (NxIntf *intf)
{
    traceP->syslog(NxTrace::NOTICE,"Application: %s", __FUNCTION__);
    traceP->syslog(NxTrace::NOTICE,"New layer: %s", intf->getLayer().c_str());

}


bool
myIntfHandler::postIntfVrfCb (NxIntf *intf)
{
    traceP->syslog(NxTrace::NOTICE,"Application: %s", __FUNCTION__);
    traceP->syslog(NxTrace::NOTICE,"interface %s  moved to vrf %s", intf->getName().c_str(),
                   intf->getVrf().c_str());
}

bool
myIntfHandler::postIntfVlanCb (NxIntf *intf)
{
    traceP->syslog(NxTrace::NOTICE,"Application: %s", __FUNCTION__);
    traceP->syslog(NxTrace::NOTICE,"interface %s new vlan is %s", intf->getName().c_str(),
                   intf->getVlan().c_str());
}

bool
myIntfHandler::postIntfPortMemberCb (NxIntf *intf)
{
    event_type_e  event;
    std::string mbr;

    traceP->syslog(NxTrace::NOTICE,"Application: %s", __FUNCTION__);
    
    event = intf->getEvent();
    traceP->syslog(NxTrace::NOTICE,"App got port member %s event for %s", 
                   (event == ADD ? "ADD":"DELETE"), intf->getName().c_str());

    if (intf->getMemberCount() > 0) {
        traceP->syslog(NxTrace::NOTICE,"\t\tMember count: %d", intf->getMemberCount());

        mbr = intf->getMembers(true);
        while (!mbr.empty()) {
            traceP->syslog(NxTrace::NOTICE,"\t\t *<%s>",mbr.c_str());
            mbr = intf->getMembers();
        }
    }
}


bool
myIntfHandler::postIntfAddDelCb (NxIntf *intf)
{
    traceP->syslog(NxTrace::NOTICE,"Application: %s", __FUNCTION__);
    
    if (intf->getEvent() == ADD ) {
        traceP->syslog(NxTrace::NOTICE,"App got ADD notification for interface %s", 
                       intf->getName().c_str());
    } else if (intf->getEvent() == DELETE ) {
        traceP->syslog(NxTrace::NOTICE,"App got DELETE notification for interface %s",
                       intf->getName().c_str());
    }
}

bool
myIntfHandler::postIntfStateCb (NxIntf *intf)
{

    traceP->syslog(NxTrace::NOTICE,"Application: %s", __FUNCTION__);

    if (intf) {
        traceP->syslog(NxTrace::NOTICE,"State change for interface: %s", intf->getName().c_str());
        traceP->syslog(NxTrace::NOTICE,"\t new state is: %d", 
                       intf->getOperState());
    }
}

bool
myIntfHandler::postIntfIpv4AddrCb (NxIntf *intf)
{
    traceP->syslog(NxTrace::NOTICE,"Application: %s", __FUNCTION__);
    
    if (intf->getEvent() == ADD ) {
        traceP->syslog(NxTrace::NOTICE,"App got ADD notification, primary address added for interface %s", intf->getName().c_str());
    } else if (intf->getEvent() == DELETE ) {
        traceP->syslog(NxTrace::NOTICE,"App got DELETE notification, primary address removed for interface %s", intf->getName().c_str()); 
    } else if (intf->getEvent() == UPDATE) {
        traceP->syslog(NxTrace::NOTICE,"Ap got UPDATE notification, primary address change for interface %s",  intf->getName().c_str());
    }
}

bool
myIntfHandler::postIntfIpv6AddrCb (NxIntf *intf)
{
    traceP->syslog(NxTrace::NOTICE,"Application: %s", __FUNCTION__);

    if (intf->getEvent() == ADD ) {
        traceP->syslog(NxTrace::NOTICE,"App got ADD notification, IPv6 address added for interface %s", intf->getName().c_str());
    } else if (intf->getEvent() == DELETE ) {
        traceP->syslog(NxTrace::NOTICE,"App got DELETE notification, IPv6 address removed for interface %s", intf->getName().c_str()); 
    } else if (intf->getEvent() == UPDATE) {
        traceP->syslog(NxTrace::NOTICE,"App got UPDATE notification, IPv6 address change for interface %s", intf->getName().c_str());
    }
}


void
query_interface (NxIntf* objp)
{
    af_e  af = AF_IPV4;
    std::string intf_name;
    std::string l3_addrv4;
    std::string l3_addrv6;
    std::string l3_sec_addr;
    std::string l2_addr;
    std::string l2_addrHW;
    std::string adminSt;
    std::string operSt;
    std::string layer;
    nxos::intf_type_e  type;
    std::string vrf;
    std::string vlan;
    std::string descr;
    uint32_t  mtu;
    uint32_t  speed;
    std::string  lastmod;

    if (!objp)  return;
    
    intf_name = objp->getName();
    type      = objp->getType();
    adminSt   = objp->getAdminState() == UP ? "up" : "down";
    operSt    = objp->getOperState() == UP ? "up" : "down";  
    layer    =  objp->getLayer();
    l3_addrv4  =  objp->getL3PrAddr(AF_IPV4);
    l3_addrv6  =  objp->getL3PrAddr(AF_IPV6);
    l2_addr  =  objp->getL2Address();
    l2_addrHW  =  objp->getL2AddressHW();
    vrf      =  objp->getVrf();
    vlan     =  objp->getVlan();
    descr    =  objp->getDescription();
    mtu      =  objp->getMtu();
    speed    =  objp->getSpeed();
    lastmod  =  objp->getLastMod();

    traceP->syslog(NxTrace::NOTICE,"======================");
    traceP->syslog(NxTrace::NOTICE,"Interface : %s", intf_name.c_str());
    traceP->syslog(NxTrace::NOTICE,"\ttype     : %s", intf_str[type]);
    traceP->syslog(NxTrace::NOTICE,"\tdescr    : %s", descr.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tmtu      : %d", mtu);
    traceP->syslog(NxTrace::NOTICE,"\tspeed    : %s", speed_str(speed));
    traceP->syslog(NxTrace::NOTICE,"\tadminSt  : %s", adminSt.c_str());
    traceP->syslog(NxTrace::NOTICE,"\toperSt   : %s", operSt.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tlayer    : %s", layer.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tL3addrv4 : %s", l3_addrv4.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tL3sec    : ");
    l3_sec_addr = objp->getL3ScAddr(AF_IPV4, true);
    while (!l3_sec_addr.empty()) {
        traceP->syslog(NxTrace::NOTICE,"%s ", l3_sec_addr.c_str());
        l3_sec_addr = objp->getL3ScAddr(AF_IPV4);
    }

    traceP->syslog(NxTrace::NOTICE,"\tL3addrv6 : %s", l3_addrv6.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tL3sec    : ");
    l3_sec_addr = objp->getL3ScAddr(AF_IPV6, true);
    while (!l3_sec_addr.empty()) {
        traceP->syslog(NxTrace::NOTICE,"%s ", l3_sec_addr.c_str());
        l3_sec_addr = objp->getL3ScAddr(AF_IPV6);
    }

    traceP->syslog(NxTrace::NOTICE,"\tL2addr   : %s", l2_addr.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tL2addrHW : %s", l2_addrHW.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tvrf      : %s", vrf.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tvlan     : %s", vlan.c_str());
    traceP->syslog(NxTrace::NOTICE,"\tlastmod  : %s", lastmod.c_str());
    
    if (type == NX_TYPE_ETH_PC)  {
        std::string mbr;
        
        traceP->syslog(NxTrace::NOTICE,"\t\tMember count: %d", objp->getMemberCount());

        mbr = objp->getMembers(true);
        while (!mbr.empty()) {
            traceP->syslog(NxTrace::NOTICE,"\t\t *<%s>", mbr.c_str());
            mbr = objp->getMembers();
        }
    }
}


int main (int argc, char *argv[], char *env[])
{
    NxIntf* objp = NULL;
    int i = 0;
    std::vector<NxIntf*> intf_vec;
    std::string arg;
    

    sdk = nxos::NxSdk::getSdkInst(argc, argv, true);

    if (!sdk) {
        cout << "Could not get access to Nx Infra SDK" << endl;
        exit(-1);
    }
    

    traceP = sdk->getTracer();
    traceP->syslog(NxTrace::NOTICE, "Appname: %s", sdk->getAppName());
    
    intfMgr = sdk->getIntfMgr();
    
    if (!intfMgr) {
        traceP->syslog(NxTrace::EMERG, "%s failed to open IntfMgr, quitting.",
                       sdk->getAppName());
        exit(-1);
    }
    
    if ((sdk->getRunningEnv() == BASH) && argc > 1) {
        for (i=1, arg = string(argv[1]);
             i < argc;
             i++) {
            NxIntf* objp = NULL;
            
            if (argv[i])
                arg = string(argv[i]);
            else
                break;

            if (!arg.compare("all")) {
                intfMgr->getIntfAll();
                objp = intfMgr->iterateIntf(true);
                while (objp) {
                    traceP->syslog(NxTrace::NOTICE, "Opened %s", objp->getName().c_str());
                    intf_vec.push_back(objp);
                    objp = intfMgr->iterateIntf();
                }
                continue;
            }
        
            objp = intfMgr->getIntf(arg);
            if (objp) {
                traceP->syslog(NxTrace::NOTICE, "Opened %s", objp->getName().c_str());
                query_interface(objp);
                intf_vec.push_back(objp);
            }
            
        }
    }
    // set up an event handler and enable watching of interfaces
    NxIntfMgrHandler *intfHandler = new myIntfHandler();
    intfMgr->setIntfHandler(intfHandler);
    
    // watch all interfaces 
    intfMgr->watchIntf("all");
    
    traceP->syslog(NxTrace::NOTICE, "Starting event loop...");
    // go into event processing loop
    sdk->startEventLoop();

    delete sdk;
}


