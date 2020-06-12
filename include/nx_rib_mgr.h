/** @file nx_rib_mgr.h
 *  @brief Abstract SDK interface to Nexus(NXOS) Layer 3 RIB
 *
 *  Provides necessary Abstraction/Plugin for NXOS RIB functionalities.
 *  It provides the Ability to register for Route and VRF updates and more.
 *
 *  Steps to register for route event from RIB:
 *   1) Instantiate NxSDK Rib Manager Object using getRibMgr().
 *   2) To watch for VRF updates, use watchVrf() (or) watchAllVrf()
 *      for all VRF updates.
 *   3) To watch for L3 route updates, use watchL3Route()
 *   4) Register for callback handler with NxRibMgr to get the regsitered
 *      RIB updates.
 *   5) Overload NxRibMgrHandler class and its member functions like
 *      (postVrfCb()     - To get watched VRF updates,
 *       postL3RouteCb() - To get watched L3 route update,
 *       etc) with your own implementation to handle various events.
 *   6) Adding/Deleting Routes in Rib is also supported.
 *
 *   @note By default, ribMgr CLI is auto-generated for the NxSDK Apps.
 *         Try "show $appname nxsdk ribmgr" in the box to access them.
 *
 *  @since NXSDK V1.5
 *
 *  Refer to some example Applications.
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 */

#ifndef __NX_RIB_MGR_H__
#define __NX_RIB_MGR_H__

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>

#include "nx_common.h"

namespace nxos
{

/**
 * @brief Abstract VRF Interface used to get details about a VRF.
 **/
class NxVrf
{
public:
    /**
     * Destructor
     **/
    virtual ~NxVrf() {}

    /**
     * To get the Name of the VRF from the given VRF object.
     *
     * @returns VRF Name(string)
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       NxRibMgr *ribMgr;
     *       NxVrf    *vrf;
     *
     *       if (sdk) {
     *           ribMgr = sdk->getRibMgr();
     *           if (rigMgr) {
     *               vrf = ribMgr->getVrf("default");
     *               if (vrf) {
     *                   cout << "Vrf Name: " << vrf->getName();
     *               }
     *           }
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       if sdk:
     *          ribMgr = sdk.getRibMgr()
     *          if ribMgr:
     *             vrf = ribMgr.getVrf("default")
     *             if vrf:
     *                print "Vrf Name: %s" % vrf.getName()
     *  @endcode
     *  NOTE: For rest of the APIs usage, object exists check will not
     *        be done. Please check for object existence before referring
     *        the object.
     **/
    virtual std::string getName() = 0;

    /**
     * To get the ID of the VRF from the given VRF object.
     * Returns 0 if the VRF is not yet created.
     *
     * @retruns VRF Id(unsigned long)
     *
     *  @code
     *  C++:
     *       NxVrf *vrf;
     *       vrf = ribMgr->getVrf("default");
     *       cout << "Vrf Id: " << vrf->getId();
     *
     *  Python:
     *       vrf = ribMgr.getVrf("default")
     *       print "Vrf Id %d" % vrf.getId()
     *  @endcode
     **/
    virtual unsigned long getId() = 0;

    /**
     * To get the Table ID of the VRF for the given
     * Address Family (AF) from the given VRF object.
     * Returns 0 if the Table is not yet created.
     *
     * @returns Table(IPv4 or IPv6) Id in a VRF(unsigned long)
     *
     *  @code
     *  C++:
     *       NxVrf *vrf;
     *       vrf = ribMgr->getVrf("default");
     *       cout << "IPv4 Table Id: " << vrf->getTableId(nxos::AF_IPV4);
     *
     *  Python:
     *       vrf = ribMgr.getVrf("default")
     *       print "IPv4 Table Id %d" % vrf.getTableId(nx_sdk_py.AF_IPV4)
     *  @endcode
     *
     *  @throws Invalid Address-Family(AF) passed
     **/
    virtual unsigned long getTableId(nxos::af_e af) = 0;

    /**
     * To get the state(UP or DOWN or etc) of the VRF for the
     * given VRF object.
     *
     * @returns VRF state(state_type_e)
     *
     *  @code
     *  C++:
     *       NxVrf *vrf;
     *       vrf = ribMgr->getVrf("default");
     *       cout << "VRF State: " << vrf->getState();
     *
     *  Python:
     *       vrf = ribMgr.getVrf("default")
     *       print "VRF State: %d" % vrf.getState()
     *  @endcode
     **/
    virtual nxos::state_type_e getState() = 0;

    /**
     * To get the type of Event(Add, Update, Delete etc)
     * when there is a change to a tracked VRF. Should
     * be used only in Rib Handler Callbacks like postVrfCb().
     * If used outside of RIB Handler Callbacks its set to
     * NO_EVENT.
     *
     * @returns Type of VRF Event(event_type_e)
     *
     *  @code
     *  C++:
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postVrfCb(NxVrf *vrf) {
     *                cout << "VRF Event: " << vrf->getEvent();
     *           }
     *     };
     *  Python:
     *     class pyRibHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get VRF events
     *           def postVrfCb(self, nxvrf):
     *               print "VRF Event: %d" % vrf.getEvent()
     *  @endcode
     **/
    virtual nxos::event_type_e getEvent() = 0;

    /**
     *
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *
     *
     *  @endcode
     *
     *  @throws
     *
     **/
    virtual bool operator==(NxVrf const &vrf_obj) const = 0;

    /**
     *
     *  @returns
     *     true if inequality property holds, false otherwise
     *
     *  @code
     *
     *
     *  @endcode
     *
     *  @throws
     *
     **/
    virtual bool operator!=(NxVrf const &vrf_obj) const = 0;

    /**
     *  Test equality of vrf objects.
     *
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *
     *
     *  @endcode
     *
     *  @throws
     *
     **/
    virtual bool equal(NxVrf const &vrf_obj) const = 0;
};

/**
 * @brief Abstract NextHoP Interface for a given L3 Route.
 **/
class NxL3NextHop
{
public:
    /* ***************************************
    * ** public data members
    * ***************************************/
    typedef enum
    {
        /// Directly connected to this system
        ATTACHED,
        /// One of router's IP addresses
        LOCAL,
        /// One of router's subnet prefixes
        DIRECT,
        /// Is recursive NH
        RECURSIVE,
        /// Backup next-hop. Not the Best.
        BACKUP,
        /// Backup next-hop. Not the Best.
        DROP,
        /// Backup next-hop. Not the Best.
        DISCARD,
    } nh_types_e;

    /**
     * Destructor
     **/
    virtual ~NxL3NextHop() {}

    /**
     * To get the Address of the NextHop.
     * NxL3NextHop object is a child of the NxL3Route object.
     *
     * @returns Address of the NextHop(string)
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxSdk        *sdk;
     *       NxRibMgr     *ribMgr;
     *       NxL3Route    *route;
     *       NxL3NextHop  *nh;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       if (sdk) {
     *           ribMgr = sdk->getRibMgr();
     *           if (ribMgr) {
     *               route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *               if (route) {
     *                   nh = route->getL3NextHop();
     *                   if (nh) cout << "NH Address: " << nh->getAddress();
     *               }
     *           }
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       if sdk:
     *          ribMgr = sdk.getRibMgr()
     *          if ribMgr:
     *             route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *             if route:
     *                nh = route.getL3NextHop()
     *                if nh:
     *                   print "NH Address: %s" % nh.getAddress()
     *  @endcode
     **/
    virtual std::string getAddress() = 0;

    /**
     * To get the outgoing interface for the NextHop.
     *
     * @returns Outgoing Interface(string)
     *
     *  @code
     *  C++:
     *       cout << "NH OutInterface: " << nh->getOutInterface();
     *
     *  Python:
     *       print "NH OutInterface: %s" % nh.getOutInterface()
     *  @endcode
     **/
    virtual std::string getOutInterface() = 0;

    /**
     * To get the VrfName for the NextHop of a given Route.
     *
     * @returns VrfName of the NextHop(string)
     *
     *  @code
     *  C++:
     *       cout << "NH VrfName: " << nh->getVrfName();
     *
     *  Python:
     *       print "NH VrfName: %s" % nh.getVrfName()
     *  @endcode
     **/
    virtual std::string getVrfName() = 0;

    /**
     * To get the Owner for the NextHop of a given Route.
     *
     * @returns Owner of NextHop(string)
     *
     *  @code
     *  C++:
     *       cout << "NH Owner: " << nh->getOwner();
     *
     *  Python:
     *       print "NH Owner: %s" % nh.getOwner()
     *  @endcode
     **/
    virtual std::string getOwner() = 0;

    /**
     * To get the Preference for the NextHop of a given Route.
     *
     * @returns Preference of the NextHop(unsigned int)
     *
     *  @code
     *  C++:
     *       cout << "NH Preference: " << nh->getPreference();
     *
     *  Python:
     *       print "NH Preference: %d" % nh.getPreference()
     *  @endcode
     **/
    virtual unsigned int getPreference() = 0;

    /**
     * To get the Metric for the NextHop of a given Route.
     *
     * @returns Metric of the NextHop(unsigned int)
     *
     *  @code
     *  C++:
     *       cout << "NH Metric: " << nh->getMetric();
     *
     *  Python:
     *       print "NH Metric: %d" % nh.getMetric()
     *  @endcode
     **/
    virtual unsigned int getMetric() = 0;

    /**
     * To get the Tag for the NextHop of a given Route.
     *
     * @returns Tag of the NextHop(unsigned int)
     *
     *  @code
     *  C++:
     *       cout << "NH Tag: " << nh->getTag();
     *
     *  Python:
     *       print "NH Tag: %d" % nh.getTag()
     *  @endcode
     **/
    virtual unsigned int getTag() = 0;

    /**
     * To get the SegmentId for the NextHop of a given Route.
     *
     * @returns SegmentId of the NextHop(unsigned int)
     *
     *  @code
     *  C++:
     *       cout << "NH Segment Id: " << nh->getSegmentId();
     *
     *  Python:
     *       print "NH Segment Id: %d" % nh.getSegmentId()
     *  @endcode
     **/
    virtual unsigned int getSegmentId() = 0;

    /**
     * To get the TunnelId for the NextHop of a given Route.
     *
     * @returns TunnelId of the NextHop(unsigned int)
     *
     *  @code
     *  C++:
     *       cout << "NH Tunnel Id: " << nh->getTunnelId();
     *
     *  Python:
     *       print "NH Tunnel Id: %d" % nh.getTunnelId()
     *  @endcode
     **/
    virtual unsigned int getTunnelId() = 0;

    /**
     * To get the Encap Type for the NextHop of a given Route.
     *
     * @returns Encap type of NextHop(nxos::encap_type_e)
     *
     *  @code
     *  C++:
     *       cout << "NH Encap Type: " << nh->getEncapType();
     *
     *  Python:
     *       print "NH Encap Type: %d" % nh.getEncapType()
     *  @endcode
     **/
    virtual nxos::encap_type_e getEncapType() = 0;

    /**
     * To check the Type of the NextHop of a given Route.
     *
     * @returns bool true - if the type is set
     *                      & vice-versa
     *
     *  @code
     *  C++:
     *       cout << "Is NH Type(DIRECT): " << nh->checkType(nxos::NxL3NextHop::DIRECT);
     *
     *  Python:
     *       print "Is NH Type(DIRECT): %d" % nh.checkType(nx_sdk_py.NxL3NextHop.DIRECT)
     *  @endcode
     **/
    virtual bool checkType(nh_types_e type) = 0;
};

/**
 * @brief Abstract Route Interface to Add/Delete and receive Route updates
 *        from NXOS RIB.
 **/
class NxL3Route
{
public:
    /* ***************************************
    * ** public data members
    * ***************************************/
    /**
     * Destructor
     **/
    virtual ~NxL3Route() {}

    /**
     * To get the VRF Name of the L3 Route.
     *
     * @returns VRF Name(string)
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *       if (route) {
     *           cout << "Route VRfName: " << route->getVrfName();
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *       if route:
     *          print "Route VrfName: %s" % route.getVrfName()
     *  @endcode
     **/
    virtual std::string getVrfName() = 0;

    /**
     * To get the Address of the L3 Route.
     *
     * @returns Address of the Route(string)
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *       if (route) {
     *           cout << "Route Address: " << route->getAddress();
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *       if route:
     *          print "Route Address: %s" % route.getAddress()
     *  @endcode
     **/
    virtual std::string getAddress() = 0;

    /**
     * To get the masklen of the L3 Route.
     *
     * @returns MaskLen of the Route(unsigned int)
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *       if (route) {
     *           cout << "Route MaskLen: " << route->getMaskLen();
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *       if route:
     *          print "Route MaskLen: %d" % route.getMaskLen()
     *  @endcode
     **/
    virtual unsigned int getMaskLen() = 0;

    /**
     * To get the count of NextHops for this L3 Route.
     * Returns 0 if there are no nexthops for the route
     * which happens in the case of route Delete.
     *
     * @returns Count of NextHops in a Route(unsigned int)
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *       if (route) {
     *           cout << "Route NextHop count: " << route->getL3NextHopCount();
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *       if route:
     *          print "Route NextHop Count: %d" % route.getL3NextHopCount()
     *  @endcode
     **/
    virtual unsigned int getL3NextHopCount() = 0;

    /**
     * To get the NextHop for this L3 Route if it exists
     * by the specified address.
     *
     * @returns a pointer to NxL3NextHop if
     *          it exists. NULL if not.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *       NxL3NextHop  *nh;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *       if (route) {
     *           nh = route->getL3NextHop("20.1.1.1");
     *           if (nh) {
     *               cout << "NextHop " << nh->getAddress() << " exists for route "
     *                    << route->getAddress();
     *           } else {
     *               cout << "NextHop " << nh->getAddress() << " does not exist "
     *                    "for route " << route->getAddress();
     *           }
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *       if route:
     *          nh = route.getL3NextHop("20.1.1.1")
     *          if nh:
     *             print "NextHop %s exists for route %s" % \
     *                   (nh.getAddress(), route.getAddress())
     *          else:
     *             print "NextHop %s does not exist for route %s" % \
     *                   (nh.getAddress(), route.getAddress())
     *  @endcode
     **/
    virtual NxL3NextHop *getL3NextHop(const std::string &address,
                                      const std::string &intfName="") = 0;
    /**
     * To get the first NextHop for this L3 Route. To get all the
     * NextHops call this API in a while loop.
     * @param[in] fromFirst At anytime, to get or start from first
     *                      NextHop, set fromFrist to True and then
     *                      call it in a while loop with fromFirst
     *                      to False to loop through the list if there
     *                      are more than one Nexthop. Refer to usage.
     * @returns a pointer to NxL3NextHop if it exists
     *          NULL if not.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *       NxL3NextHop  *nh;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *       if (route) {
     *           nh = route->getL3NextHop(true);
     *           while (nh) {
     *               cout << "NextHop " << nh->getAddress();
     *               nh = route->getL3NextHop();
     *           }
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *       if route:
     *          nh = route.getL3NextHop(True)
     *          while nh:
     *              print "NextHop %s" % nh.getAddress()
     *              nh = route.getL3NextHop()
     *  @endcode
     **/
    virtual NxL3NextHop *getL3NextHop(bool fromFirst = false) = 0;

    /**
     * To get the type of Event(Add, Update, Delete etc)
     * when there is a change to a tracked L3 Route. Should
     * be used only in Rib Handler Callbacks like postL3RouteCb().
     * If used outside of RIB Handler Callbacks its set to
     * NO_EVENT.
     *
     * @returns Route Event Type(event_type_e)
     *
     *  @code
     *  C++:
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postL3RouteCb(NxL3Route *route) {
     *                cout << "Route Event: " << route->getEvent();
     *           }
     *     };
     *  Python:
     *     class pyRibHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postL3RouteCb(self, nxroute):
     *               print "Route Event: %d" % nxroute.getEvent()
     *  @endcode
     **/
    virtual nxos::event_type_e getEvent() = 0;

    /**
     *
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *
     *
     *  @endcode
     *
     *  @throws
     *
     **/
    virtual bool operator==(NxL3Route const &route_obj) const = 0;

    /**
     *
     *  @returns
     *     true if inequality property holds, false otherwise
     *
     *  @code
     *
     *
     *  @endcode
     *
     *  @throws
     *
     **/
    virtual bool operator!=(NxL3Route const &route_obj) const = 0;

    /**
     *  Test equality of route objects.
     *
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *
     *
     *  @endcode
     *
     *  @throws
     *
     **/
    virtual bool equal(NxL3Route const &route_obj) const = 0;

    /**
     * To add a direct next-hop to a route. Changes will take effect when 
     * sendMyL3RoutesToRib is called. 
     * postMyL3RouteCb is called after sendMyL3RoutesToRib called to signal 
     * that route changes are done.
     *
     * @param[in] nextHopAddr Address of the next-hop as a string.
     * @param[in] intfName  Egress interface name as a string.
     * @param[in] preference Preference value score is inverted where lower is
     *                       more preferred.
     * @returns
     *     an object of type NxL3NextHop to use to further configure the next-hop
     *     if needed.
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->addL3Route("10.1.1.1", 24, "default");
     *       NxL3NextHop *nh = route->addL3DirectNextHop("10.1.1.3", 
     *                                                   "Eth1/3" 12);
     *       bool result = route->sendMyL3RoutesToRib(AF_IPV4);
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.addL3Route("10.1.1.1", 24, "default")
     *       next_hop = route.addL3DirectNextHop("10.1.1.3", "Eth1/3", 12)
     *       result = ribMgr.sendMyL3RoutesToRib(nx_sdk_py.AF_IPV4)
     *  @endcode
     *
     *  @throws
     */
    virtual NxL3NextHop *addL3DirectNextHop(const std::string &nextHopAddr,
                                            const std::string &intfName,
                                            uint8_t           preference)=0;
    /**
     * To add a recursive next-hop to a route. Changes will take effect after 
     * sendMyL3RoutesToRib is called. 
     * postMyL3RouteCb is called after sendMyL3RoutesToRib is called to signal 
     * that route changes are done. 
     * postL3RecursiveNextHopCb is called after sendMyL3RoutesToRib to signal
     * changes to the next hop. 
     * 
     * @param[in] nextHopAddr Address of the next-hop as a string.
     * @param[in] preference Preference value score is inverted where lower is
     *                       more preferred.
     * @returns
     *     an object of type NxL3NextHop to use to further configure the next-hop
     *     if needed.
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->addL3Route("10.1.1.1", 24, "default");
     *       NxL3NextHop *nh = route->addL3RecursiveNextHop("10.1.1.3", 12);
     *       bool result = route->sendMyL3RoutesToRib(AF_IPV4);
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.addL3Route("10.1.1.1", 24, "default")
     *       next_hop = route.addL3RecursiveNextHop("10.1.1.3", 12)
     *       result = ribMgr.sendMyL3RoutesToRib(nx_sdk_py.AF_IPV4)
     *  @endcode
     *
     *  @throws
     */
    virtual NxL3NextHop *addL3RecursiveNextHop(const std::string &nextHopAddr,
                                               uint8_t           preference)=0;
    /**
     * To delete a Layer3 route next-hop. Changes will take effect after 
     * sendMyL3RoutesToRib is called. 
     * postMyL3RouteCb is called after sendMyL3RoutesToRib is called to signal 
     * that route changes are complete. 
     *
     * @param[in] nextHopAddress Address of the next-hop as a string.
     *
     * @returns
     *     an object of type NxL3NextHop to use to further configure the next-hop
     *     if needed.
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3RouteDetail("10.1.1.1", 24, "default");
     *       NxL3NextHop *nh = route->delL3NextHop("10.1.1.3");
     *       bool result = route->sendMyL3RoutesToRib(AF_IPV4);
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.addL3Route("10.1.1.1", 24, "default")
     *       next_hop = route.delL3NextHop("10.1.1.3")
     *       result = ribMgr.sendMyL3RoutesToRib(nx_sdk_py.AF_IPV4)
     *  @endcode
     *
     *  @throws
     */
    virtual bool delL3NextHop(const std::string &nexHopAddress, 
                              const std::string &intfName="")=0;

    /**
     * To get the IP prefix string of the L3 Route. 
     *
     * @returns IP prefix string of the L3 Route
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *       if (route) {
     *           cout << "Route Prefix: " << route->getL3Prefix();
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *       if route:
     *          print "Route Prefix: %d" % route.getL3Prefix()
     *  @endcode
     **/
    virtual std::string getL3Prefix() const=0;
};

/*
 * @brief Abstract RibMgr callback Interface to receive VRF,
 *        Route updates etc from NXOS RIB.
 **/
class NxRibMgrHandler
{
public:
    /**
     * Destructor.
     **/
    virtual ~NxRibMgrHandler(){};

    /**
     * User to overload the postL3RouteCb callback method
     * to receive any registered route updates based on the
     * set filters.
     * @param[in]  NxL3Route object.
     *
     * @returns True : if the action was successful.
     *          False: if the action was not successful.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_rib_mgr.h>
     *
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postL3RouteCb(NxL3Route *route) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxRibMgr *ribMgr = sdk->getRibMgr();
     *          NxRibMgrHandler *myribcb = new myRibMgrHandler();
     *          ribMgr->setRibMgrHandler(myribcb);
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myRibMgrHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postL3RouteCb(self, nxroute):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         ribMgr = sdk.getRibMgr()
     *         myribcb = myRibMgrHandler()
     *         ribMgr.setRibMgrHandler(myribcb)
     *  @endcode
     **/
    virtual bool postL3RouteCb(NxL3Route *route) { return (true); }

    /**
     * User to overload the postVrfCb callback method
     * to receive any registered VRF updates based on
     * the set filters.
     * @param[in]  NxVrf object.
     *
     * @returns True : if the action was successful.
     *          False: if the action was not successful.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_rib_mgr.h>
     *
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postVrfCb(NxVrf *vrf) {
     *                // Do your action
     *           }
     *     };
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myRibMgrHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postVrfCb(self, nxvrf):
     *               # Do your action here
     *  @endcode
     **/
    virtual bool postVrfCb(NxVrf *vrf) { return (true); }

    /**
     * User to overload the postL3RecursiveNextHopCb callback method
     * to receive updates after a recursive next hop is added by the application.
     * 
     * @param[in]  NxL3Route object representing the route for the added 
     *             recursive next-hop
     * @param[in]  isResolved A boolean value representing whether the RNH is
     *             resolved.
     *
     * @returns True : if the action was successful.
     *          False: if the action was not successful.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_rib_mgr.h>
     *
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postL3RecursiveNextHopCb(NxL3Route *route) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxRibMgr *ribMgr = sdk->getRibMgr();
     *          NxRibMgrHandler *myribcb = new myRibMgrHandler();
     *          ribMgr->setRibMgrHandler(myribcb);
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myRibMgrHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postL3RecursiveNextHopCb(self, nxroute):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         ribMgr = sdk.getRibMgr()
     *         myribcb = myRibMgrHandler()
     *         ribMgr.setRibMgrHandler(myribcb)
     *  @endcode
     **/
    virtual bool postL3RecursiveNextHopCb(NxL3Route *rnhRoute, 
                                          bool      isResolved) { return (true);}

    /**
     * User to overload the postMyL3RouteCb callback method
     * to receive any registered route updates for routes added or modified by 
     * the application.
     * @param[in]  NxL3Route object.
     *
     * @returns True : if the action was successful.
     *          False: if the action was not successful.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_rib_mgr.h>
     *
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postMyL3RouteCb(NxL3Route *route) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxRibMgr *ribMgr = sdk->getRibMgr();
     *          NxRibMgrHandler *myribcb = new myRibMgrHandler();
     *          ribMgr->setRibMgrHandler(myribcb);
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myRibMgrHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postMyL3RouteCb(self, nxroute):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         ribMgr = sdk.getRibMgr()
     *         myribcb = myRibMgrHandler()
     *         ribMgr.setRibMgrHandler(myribcb)
     *  @endcode
     **/

    virtual bool postMyL3RouteCb(NxL3Route *route) { return (true); }
    /**
     * User to overload the postL3RouteRepopulateCb callback method
     * to receive a notification when the application needs to re-add
     * all it's routes.
     * @param[in]  vrfName A string representing the VRF name
     * @param[in]  routeAddress: string representation of the address
     *             if address is all zeros, that means repopulate all routes
     * @param[in]  maskLen: route mask. If value is zero along with 
     *             zero routeAddress, this means repopulate all routes for this 
     *             vrf
     *
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_rib_mgr.h>
     *
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postL3RouteRepopulateCb(const std::string vrfName,
     *                                        const std::string routePrefix,
     *                                        unsigned char     maskLen) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxRibMgr *ribMgr = sdk->getRibMgr();
     *          NxRibMgrHandler *myribcb = new myRibMgrHandler();
     *          ribMgr->setRibMgrHandler(myribcb);
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myRibMgrHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postL3RouteRepopulateCb(self, vrfName, routeAddr, maskLen):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         ribMgr = sdk.getRibMgr()
     *         myribcb = myRibMgrHandler()
     *         ribMgr.setRibMgrHandler(myribcb)
     *  @endcode
     **/

    virtual void postL3RouteRepopulateCb(const std::string vrfName,
                                         const std::string routeAddress,
                                         unsigned char     maskLen=0) {}
};

/**
 *  @brief Abstract Rib Manager interface to gain access to NXOS RIB.
 */
class NxRibMgr
{
public:
    /**
     * Destructor
     *
     * To Delete the RIB manager instance and its associated
     * objects gracefully. Its cleaned on delete sdk too.
     **/
    virtual ~NxRibMgr() {}

    /**
     * To get the VRF object from a given name.
     * Returns NULL if "all" (or) empty string "" is passed.
     * @param[in]  vrfName name of the VRF in string.
     *             vrfName cannot be "all" or empty string ""
     * @param[in]  watch [Optional] set to True if this VRF
     *                   needs to be tracked. By default,
     *                   its set to False.
     * @returns NxVrf Object if it exists.
     *          NULL if it does not exist.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxVrf *vrf;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       vrf = ribMgr->getVrf("default");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       vrf = ribMgr.getVrf("default")
     *  @endcode
     *  @throws vrfName is more than 32 characters.
     **/
    virtual NxVrf *getVrf(std::string vrfName = "default",
                          bool        watch   = false) = 0;

    /**
     * To receive/subscribe for any updates to this VRF.
     * On any update to this VRF, users overloaded postVrfCb
     * handler will be called. To subscribe for all VRFs, use
     * vrfName as "all".
     * NOTE: Forward referencing is allowed for a non created
     *       VRF i.e API can be called even if the VRF is not
     *       yet created. vrfName cannot be empty.
     * @param[in] vrfName [Optional] name of the vrf in string.
     *                    If not specified, "all" is used to
     *                    watch on all VRFs. VrfName cannot be
     *                    empty string.
     * @returns bool True -  if successful.
     *               False - if not successful(Empty VRfName).
     *
     * @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       class myRibMgrMgrHandler : public NxRibMgrHandler {
     *          public:
     *             bool postVrfCb(NxVrf *vrf) {
     *                  cout << "Vpn1 VRF changed";
     *             }
     *       };
     *
     *       int  main (int argc, char **argv)
     *       {
     *            NxSdk    *sdk;
     *            NxRibMgr *ribMgr;
     *            NxVrf    *vrf;
     *
     *            NxRibMgrHandler *myribcb = new myRibMgrHandler();
     *            sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *            ribMgr = sdk->getRibMgr();
     *            ribMgr->setRibMgrHandler(myribcb);
     *
     *            ribMgr->watchVrf("vpn1");
     *       }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myRibMgrHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postVrfCb(self, nxvrf):
     *               print "vpn1 has changed"
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         ribMgr = sdk.getRibMgr()
     *         myribcb = myRibMgrHandler()
     *         ribMgr.setRibMgrHandler(myribcb)
     *
     *         ribMgr.watchVrf("vpn1")
     * NOTE: Use the pregenerated "show <appname> nxsdk ribMgr" command
     *       to verify if the API has succeeded.
     * @endcode
     *
     * @throws vrfName is more than 32 characters.
     **/
    virtual bool watchVrf(std::string vrfName = "all") = 0;

    /**
     * To unsubscribe for any updates to this VRF. To unsubscribe
     * for all VRF updates, use vrfName as "all".
     * @param[in] VrfName [Optional] name of the vrf in string.
     *                    If not specified, "all" is used to
     *                    unwatch all VRFs. VrfName cannot be
     *                    empty string.
     *
     * @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxVrf *vrf;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       ribMgr->unwatchVrf("vpn1");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       ribMgr.unwatchVrf("vpn1")
     * @endcode
     *
     * @throws vrfName is more than 32 characters.
     *
     * NOTE: Use the pregenerated "show <appname> nxsdk ribMgr" command
     *       to verify if the API has succeeded.
     **/
    virtual void unwatchVrf(std::string vrfName = "all") = 0;

    /**
     * Users to overload RIB Manager Handler class
     * to handle regsiterd events like VRF, Route
     * updates etc and register the handler object
     * with RIB manager to handle rib events.
     * @param[in] NxRibMgrHandler object with user overloaded
     *             functions to handle rib events.
     *
     * @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_rib_mgr.h>
     *
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postL3RouteCb(NxL3Route *route) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxRibMgr *ribMgr = sdk->getRibMgr();
     *          NxRibMgrHandler *myribcb = new myRibMgrHandler();
     *          ribMgr->setRibMgrHandler(myribcb);
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myRibMgrHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postL3RouteCb(self, nxroute):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         ribMgr = sdk.getRibMgr()
     *         myribcb = myRibMgrHandler()
     *         ribMgr.setRibMgrHandler(myribcb)
     * @endcode
     **/
    virtual void setRibMgrHandler(NxRibMgrHandler *handler) = 0;

    /**
     *  To get the Handler object registered with RIB Mgr.
     **/
    virtual NxRibMgrHandler *getRibMgrHandler() = 0;

    /**
     * To get the L3 Route with it's bext next-hop based on the passed
     * input parameters. Routes returned by this function cannot be used 
     * to addL3DirectNextHop() or addL3RecursiveNextHop()
     * @param[in] routeAddr Address of the route as a string.
     * @param[in] maskLen [Optional] MaskLength if its a prefix.
     *                     If not set it will pick /32 for IP
     *                     address (or) /128 for IPv6 address.
     * @param[in] vrfName [Optional] VrfName in which the route is
     *                    present. If not set "default" will be used.
     *                    vrfName cannot be "all" or empty string "".
     * @param[in] clearBuffer [Optional] If the route is present,
     *                        getL3Route() creates a new NxL3Route
     *                        object. [Recommended] App owners must
     *                        free/destory the L3 Route object once
     *                        done. If they fail to free it, NXSDK
     *                        will store the object until its freed.
     *                        NOTE: NXSDK allows a max of 10 getL3Route
     *                        Objects per VRF without having the Apps
     *                        to free it explicitly. If user failed to
     *                        cleanup after 10 getL3Route objects, it
     *                        will raise an exception. If the App owners,
     *                        failed/forgot to free the route object then
     *                        set clearBuffer flag to True which will
     *                        free all the previously created route objects.
     * @returns NxL3Route object if it exists.
     *          NULL if not (VrfName empty (or) "all", Vrf does
     *          not exist, down or invalid, no route etc).
     *
     * @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     * @endcode
     *
     * @throws invalid routeAddr
     * @throws invalid maskLen
     * @throws vrfName is more than 32 characters.
     **/
    virtual NxL3Route *getL3Route(std::string   routeAddr,
                                  unsigned char maskLen     = 0,
                                  std::string   vrfName     = "default", 
                                  bool          clearBuffer = false) = 0;

    /**
     * To receive/subscribe for Route updates based on
     * protocol owner of the route, vrf, Address Family
     * etc. To get the route updates, user overloaded
     * postL3RouteCb handler will be called.
     * @param[in] protocol owner of the routes in NX RIB.
     * @param[in] tag [Optional] instance id of the owner
     *            for ex) for BGP  - tag is the asn
     *                    for isis - instance id
     * @param[in] vrfName [Optional] Register for route
     *                    events only from this Vrf. If
     *                    not specified, it means for all Vrfs.
     *                    vrfName cannot be empty string.
     * @param[in] af [Optional] Register for route events only
     *               from the specified address-family (af).
     *               For ex) To get only IPv4 route events,
     *               use AF_IPV4 etc. IF not speficied, it
     *               means from all AFs.
     * @returns bool True -  if successful.
     *               False - if not successful(Empty VRfName).
     *
     * @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_rib_mgr.h>
     *
     *     class myRibMgrMgrHandler : public NxRibMgrHandler {
     *        public:
     *           bool postL3RouteCb(NxL3Route *route) {
     *                cout << "Received bgp-100 updates for IPV4 routes on vpn1";
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxRibMgr *ribMgr;
     *          NxRibMgrHandler *myribcb = new myRibMgrMgrHandler();
     *
     *          ribMgr = sdk->getRibMgr();
     *          if (ribMgr) {
     *              ribMgr->setRibMgrHandler(myribcb);
     *              ribMgr->watchL3Route("bgp", "100", "vpn1", nxos::AF_IPV4);
     *          }
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myRibMgrHandler(nx_sdk_py.NxRibMgrHandler):
     *     ### Overloaded Callback to get Route events
     *           def postL3RouteCb(self, nxroute):
     *               print "Received bgp-100 updates for IPV4 routes on vpn1"
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         myribcb = myRibMgrHandler()
     *         if sdk:
     *            ribMgr = sdk.getRibMgr()
     *            if ribMgr:
     *               ribMgr.setRibMgrHandler(myribcb)
     *               ribMgr.watchL3Route("bgp", "100", "vpn1", nxos::AF_IPV4)
     * @endcode
     *
     * @throws vrfName is more than 32 characters.
     * @throws reached maximum watch filter count: 15 for this Address-Family(AF)
     *         in this VRF.
     *
     * NOTE: Use the pregenerated "show <appname> nxsdk ribMgr" command
     *       to verify if the API has succeeded.
     **/
    virtual bool watchL3Route(std::string protocol,
                              std::string tag     = "",
                              std::string vrfName = "all",
                              nxos::af_e  af      = nxos::MAX_AF) = 0;

    /**
     * To unsubscribe for Route updates based on
     * protocol owner of the route, vrf, Address Family
     * etc. To get the route updates, user overloaded
     * @param[in] protocol Owner of the routes in NX RIB.
     * @param[in] tag [Optional] instance id of the owner
     *            for ex) for BGP  - tag is the asn
     *                    for isis - instance id
     * @param[in] vrfName [Optional] Register for route
     *                    events only from this Vrf. If
     *                    not specified, it means for all Vrfs.
     *                    vrfName cannot be empty string.
     * @param[in] af [Optional] Register for route events only
     *               from the specified address-family (af).
     *               For ex) To get only IPv4 route events,
     *               use AF_IPV4 etc. IF not speficied, it
     *               means from all AFs.
     *
     * @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_rib_mgr.h>
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxRibMgr *ribMgr = sdk->getRibMgr();
     *          ribMgr->unwatchL3Route("bgp", "100", "vpn1");
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         ribMgr = sdk.getRibMgr()
     *         ribMgr.unwatchL3Route("bgp", "100", "vpn1")
     * @endcode
     *
     * @throws vrfName is more than 32 characters.
     *
     * NOTE: Use the pregenerated "show <appname> nxsdk ribMgr" command
     *       to verify if the API has succeeded.
     **/
    virtual void unwatchL3Route(std::string protocol,
                                std::string tag     = "",
                                std::string vrfName = "all",
                                nxos::af_e  af      = nxos::MAX_AF) = 0;
    /**
     * To get the L3 Route with all its next-hops based on the passed
     *                     input parameters.
     * @param[in] routeAddr Address of the route as a string.
     * @param[in] maskLen [Optional] MaskLength if its a prefix.
     *                     If not set it will pick /32 for IP
     *                     address (or) /128 for IPv6 address.
     * @param[in] vrfName [Optional] VrfName in which the route is
     *                    present. If not set "default" will be used.
     *                    vrfName cannot be "all" or empty string "".
     * @returns NxL3Route object if it exists.
     *          NULL if not (VrfName empty (or) "all", Vrf does
     *          not exist, down or invalid, no route etc).
     *
     * @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     * @endcode
     *
     * @throws invalid routeAddr
     * @throws invalid maskLen
     * @throws vrfName is more than 32 characters.
     **/
    virtual NxL3Route *getL3RouteDetail(const std::string &routeAddr,
                                        unsigned int      maskLen     = 0,
                                        const std::string &vrfName    ="default")=0;
    /**
     * To add a L3 Route object. This API will return a route object 
     * to add/update/remove next-hops. All operations take place when 
     * sendMyL3RoutesToRib is called. 
     * postMyL3RouteCb is called after sendMyL3RoutesToRib is called to signal 
     * that route changes are done. 
     * postL3RecursiveNextHopCb is called after sendMyL3RoutesToRib to signal changes to 
     * the next hop. 
     * @param[in] routeAddr Address of the route as a string.
     * @param[in] maskLen [Optional] MaskLength if its a prefix.
     *                     If not set it will pick /32 for IP
     *                     address (or) /128 for IPv6 address.
     * @param[in] vrfName [Optional] VrfName in which the route is
     *                    present. If not set "default" will be used.
     *                    vrfName cannot be "all" or empty string "".
     *                    
     * @returns NxL3Route object to be used to further configure the route.
     *          NULL if not (VrfName empty (or) "all", Vrf does
     *          not exist, down or invalid, no route etc).
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->addL3Route("10.1.1.1", 24, "default");
     *       NxL3NextHop *nh = route->addL3DirectNextHop("10.1.1.3", 
     *                                                   "Eth1/3" 12);
     *       bool result = route->sendMyL3RoutesToRib(AF_IPV4);
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.addL3Route("10.1.1.1", 24, "default")
     *       next_hop = route.addL3DirectNextHop("10.1.1.3", "Eth1/3", 12)
     *       result = ribMgr.sendMyL3RoutesToRib(nx_sdk_py.AF_IPV4)
     *  @endcode
     *
     * @throws invalid routeAddr
     * @throws invalid maskLen
     * @throws vrfName is more than 32 characters.
     **/
    virtual NxL3Route *addL3Route(const std::string &routeAddr,
                                  unsigned char     maskLen =0,
                                  const std::string &vrfName="default")=0;
    /**
     * To delete a L3 Route and all it's next-hops owned by the application. 
     * 
     * postMyL3RouteCb is called after sendMyL3RoutesToRib is called to signal 
     * that route changes are done. 
     * 
     * @param[in] routeAddr Address of the route as a string.
     * @param[in] maskLen [Optional] MaskLength if its a prefix.
     *                     If not set it will pick /32 for IP
     *                     address (or) /128 for IPv6 address.
     * @param[in] vrfName [Optional] VrfName in which the route is
     *                    present. If not set "default" will be used.
     *                    vrfName cannot be "all" or empty string "".
     * @returns true or false
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     * @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->getL3Route("10.1.1.1", 24, "default");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.getL3Route("10.1.1.1", 24, "default")
     *  @endcode
     *
     *  @throws invalid routeAddr
     *  @throws invalid maskLen
     *  @throws vrfName is more than 32 characters.
     **/
    virtual bool delL3Route(const std::string &routePrefix,
                            unsigned char     maskLen  =0,
                            const std::string &vrfName ="default")=0;
    /**
     * Send all pending route operations to RIB. This call is asynchronous. 
     * To check if route was added properly, override postMyL3RouteCb to be 
     * notified when your route is added or deleted. 
     * 
     * @param[in] af [Optional] Register for route events only
     *               from the specified address-family (af).
     *               For ex) To get only IPv4 route events,
     *               use AF_IPV4 etc. IF not speficied, it
     *               means from all AFs.
     *                    
     * @returns true if sendMyL3RoutesToRib operation succeeded. 
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *       NxL3Route    *route;
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       route = ribMgr->addL3Route("10.1.1.1", 24, "default");
     *       NxL3NextHop *nh = route->addL3DirectNextHop("10.1.1.3", 
     *                                                   "Eth1/3", 12);
     *       bool result = route->sendMyL3RoutesToRib(AF_IPV4);
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       route = ribMgr.addL3Route("10.1.1.1", 24, "default")
     *       next_hop = route.addL3DirectNextHop("10.1.1.3", "Eth1/3", 12)
     *       result = ribMgr.sendMyL3RoutesToRib(nx_sdk_py.AF_IPV4)
     *  @endcode
     *
     *  @throws invalid Address family
     **/
    virtual bool sendMyL3RoutesToRib(nxos::af_e af=nxos::AF_IPV4) = 0;

    /**
     * Tell Route information base that all routes belonging to the application
     * were programmed. This is needed after system switch over and the 
     * application has added all its routes.
     * @param[in] af Address Family
     * @param[in] vrfName Name of the VRF
     *                    
     * @returns true if sendMyL3RoutesToRib operation succeeded. 
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_rib_mgr.h>
     *
     *
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       ribMgr = sdk->getRibMgr();
     *
     *       bool result = ribMgr->converged();
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       ribMgr = sdk.getRibMgr()
     *
     *       result = ribMgr.converged()
     *  @endcode
     *
     **/
    virtual bool converged(nxos::af_e  af=nxos::AF_IPV4, 
                           const std::string &vrfName="all") = 0;
};

} // namespace nxos

#endif //__NX_RIB_MGR_H__
