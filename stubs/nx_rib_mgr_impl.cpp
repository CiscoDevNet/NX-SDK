#include "nx_rib_mgr_impl.h"

using namespace std;
using namespace nxos;


std::string NxVrfImpl::getName()
{
   return "";
}

unsigned long NxVrfImpl::getId()
{
   return(0);
}

unsigned long NxVrfImpl::getTableId(nxos::af_e af)
{
   return(0);
}

nxos::state_type_e NxVrfImpl::getState()
{
   return((nxos::state_type_e)0);
}

nxos::event_type_e  NxVrfImpl::getEvent()
{
   return((nxos::event_type_e)0);
}

std::string NxL3NextHopImpl::getAddress()
{
   return("");
}

std::string NxL3NextHopImpl::getOutInterface()
{
   return("");
}

std::string NxL3NextHopImpl::getVrfName()
{
   return("");
}

std::string NxL3NextHopImpl::getOwner()
{  
   return("");
}

unsigned int NxL3NextHopImpl::getPreference()
{
   return(0);
}

unsigned int NxL3NextHopImpl::getMetric()
{
   return(0);
}

unsigned int NxL3NextHopImpl::getTag()
{
   return(0);
}

unsigned int NxL3NextHopImpl::getSegmentId()
{
   return(0);
}

unsigned int NxL3NextHopImpl::getTunnelId()
{
   return(0);
}

nxos::encap_type_e NxL3NextHopImpl::getEncapType()
{
   return((nxos::encap_type_e)0);
}

bool NxL3NextHopImpl::checkType(nh_types_e type)
{
   return(false);
}

std::string NxL3RouteImpl::getVrfName()
{
   return("");
}

std::string NxL3RouteImpl::getAddress()
{
   return("");
}

unsigned int NxL3RouteImpl::getMaskLen()
{
   return(0);
}

unsigned int NxL3RouteImpl::getL3NextHopCount()
{
   return(0);
}

NxL3NextHop *NxL3RouteImpl::getL3NextHop(std::string address)
{
   return(NULL);
}

NxL3NextHop *NxL3RouteImpl::getL3NextHop(bool fromFirst)
{
   return(NULL);
}

nxos::event_type_e NxL3RouteImpl::getEvent()
{
   return((nxos::event_type_e)0);
}

NxVrf* NxRibMgrImpl::getVrf(std::string vrfName, 
                            bool watch)
{
   return(NULL);
}

bool NxRibMgrImpl::watchVrf(std::string vrfName)
{
   return(false);
}

void NxRibMgrImpl::unwatchVrf(std::string vrfName)
{
   return;
}

void NxRibMgrImpl::setRibMgrHandler(NxRibMgrHandler *handler)
{
   return;
}

NxRibMgrHandler* NxRibMgrImpl::getRibMgrHandler()
{
    return(NULL);
}

NxL3Route* NxRibMgrImpl::getL3Route(std::string routeAddr, 
                                    unsigned char maskLen,
                                    std::string vrfName, 
                                    bool clearBuffer)
{
    return(NULL);
}

bool  NxRibMgrImpl::watchL3Route(std::string protocol,
                       std::string tag,
                       std::string vrfName, 
                       nxos::af_e af)
{
    return(false);
}

void NxRibMgrImpl::unwatchL3Route(std::string protocol,
                                  std::string tag,
                                  std::string vrfName,
                                  nxos::af_e af)
{
    return;
}

