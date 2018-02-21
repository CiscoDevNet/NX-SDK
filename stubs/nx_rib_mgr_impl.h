#ifndef __NX_RIB_MGR_IMPL_H__
#define __NX_RIB_MGR_IMPL_H__

#include <iostream>
#include <string>

#include "nx_rib_mgr.h"

using namespace std;

namespace nxos {

class NxVrfImpl : public NxVrf {
public:

    ~NxVrfImpl() {}
    std::string         getName();
    unsigned long       getId();
    unsigned long       getTableId(nxos::af_e af);
    nxos::state_type_e  getState();
    nxos::event_type_e  getEvent();
};

class NxL3NextHopImpl : public NxL3NextHop {
public:
    ~NxL3NextHopImpl() {}
    std::string getAddress();
    std::string getOutInterface();
    std::string getVrfName();
    std::string getOwner();
    unsigned int getPreference();
    unsigned int getMetric();
    unsigned int getTag();
    unsigned int getSegmentId();
    unsigned int getTunnelId();
    nxos::encap_type_e getEncapType();
    bool checkType(nh_types_e type);
};

class NxL3RouteImpl : public NxL3Route {
public:
    ~NxL3RouteImpl() {}
    std::string getVrfName();
    std::string getAddress();
    unsigned int getMaskLen();
    unsigned int getL3NextHopCount();
    NxL3NextHop *getL3NextHop(std::string address);
    NxL3NextHop *getL3NextHop(bool fromFirst=false);
    nxos::event_type_e getEvent();
};

class NxRibMgrImpl : public NxRibMgr {
public:
    ~NxRibMgrImpl() {}
    NxVrf*   getVrf(std::string vrfName="default", 
                    bool watch=false);
    bool watchVrf(std::string vrfName="all");
    void unwatchVrf(std::string vrfName="all");
    void setRibMgrHandler(NxRibMgrHandler *handler);
    NxRibMgrHandler  *getRibMgrHandler();
    NxL3Route* getL3Route(std::string routeAddr, 
                          unsigned char maskLen=0,
                          std::string vrfName="default", 
                          bool clearBuffer=false);
    bool  watchL3Route(std::string protocol,
                       std::string tag="",
                       std::string vrfName="all", 
                       nxos::af_e af=nxos::MAX_AF);
    void unwatchL3Route(std::string protocol,
                        std::string tag="",
                        std::string vrfName="all",
                        nxos::af_e af=nxos::MAX_AF);
};

}

#endif //__NX_RIB_MGR_IMPL_H__
