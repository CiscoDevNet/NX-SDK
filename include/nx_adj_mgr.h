/** @file nx_adj_mgr.h
 *  @brief Abstract SDK interface to Nexus(NXOS) Adjacency Mgr
 *
 *  Provides necessary Abstraction/Plug-in for NXOS Adjacencies like ARP, ND etc.
 *  It provides the Ability to register for adjacency updates and more.
 *
 *  @since NXSDK V1.7.5
 *
 *  Refer to some example Applications.
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 */

#ifndef __NX_ADJ_MGR_H__
#define __NX_ADJ_MGR_H__

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include "types/nx_common.h"

namespace nxos
{

/**
 * @brief Abstract Adjacency Interface used to get details about an adjacency.
 **/
class NxAdj
{
public:
    /**
     * Destructor, called when application releases object reference.
     * Removes the object from the SDK map.
     **/
    virtual ~NxAdj() {}

    /**
     * Get IP (or) Ipv6 address of the adjacency.
     *
     * @returns IP (or) Ipv6 address (string)
     *
     *  @code
     *  C++:
     *       std::string ipAddr = adj->getIpAddr();
     *
     *  Python:
     *       print "IP Address: %s" % adj.getIpAddr()
     *  @endcode
     **/
    virtual std::string getIpAddr() = 0;

    /**
     * Get MAC address of the adjacency.
     *
     * @returns MAC address (string)
     *
     *  @code
     *  C++:
     *       std::string macAddress = adj->getMacAddr();
     *
     *  Python:
     *       print "Mac Address: %s" % adj.getMacAddr()
     *  @endcode
     **/
    virtual std::string getMacAddr() = 0;

    /**
     * Get Vrf of the adjacency.
     *
     * @returns VRF (string)
     *
     *  @code
     *  C++:
     *       std::string vrf = adj->getVrf();
     *
     *  Python:
     *       print "VRF: %s" % adj.getVrf()
     *  @endcode
     **/
    virtual std::string getVrf() = 0;

    /**
     * Get Layer3 Interface of the adjacency.
     *
     * @returns interface name (string)
     *
     *  @code
     *  C++:
     *       std::string interface = adj->getIntfName();
     *
     *  Python:
     *       print "Interface: %s" % adj.getIntfName()
     *  @endcode
     **/
    virtual std::string getIntfName() = 0;

    /**
     * Get Layer2 physical Interface of the adjacency.
     *
     * @returns physical interface name (string)
     *
     *  @code
     *  C++:
     *       std::string phyIntf = adj->getPhyIntfName();
     *
     *  Python:
     *       print "Physical Interface: %s" % adj.getPhyIntfName()
     *  @endcode
     **/
    virtual std::string getPhyIntfName() = 0;

    /**
     * Get Preference of the adjacency.
     *
     * @returns preference (unsigned int)
     *
     *  @code
     *  C++:
     *       unsigned int preference = adj->getPreference();
     *
     *  Python:
     *       print "Preference: %d" % int(adj.getPreference())
     *  @endcode
     **/
    virtual unsigned int getPreference() = 0;

    /**
     * Get Source (Arp, ND etc) of the adjacency.
     *
     * @returns Source (string)
     *
     *  @code
     *  C++:
     *       std::string source = adj->getSource();
     *
     *  Python:
     *       print "Source: %s" % adj.getSource()
     *  @endcode
     **/
    virtual std::string getSource() = 0;

    /**
     * To get the type of Event(Add, Update, Delete etc).
     * Should be used only in Adjacency Handler Callbacks like
     * postAdjCb(). If used outside of Adjacency Handler
     * Callbacks its set to NO_EVENT.
     *
     * @returns Type of Adjacency Event (event_type_e)
     *
     *  @code
     *  C++:
     *     class myAdjMgrHandler : public NxAdjMgrHandler {
     *        public:
     *           void postAdjCb(NxAdj *adj) {
     *                cout << "Adjacency Event: " << adj->getEvent();
     *           }
     *     }
     *
     *  Python:
     *     class pyAdjHandler(nx_sdk_py.NxAdjMgrHandler):
     *     ### Overloaded Callback to get Adjacency events
     *           def postAdjCb(self, nxadj):
     *               print "Adjacency Event: %d" % nxadj.getEvent()
     *  @endcode
     **/
    virtual nxos::event_type_e getEvent() = 0;

    /**
     * To get the type of address family (AF_IPV4 or AF_IPV6).
     *
     * @returns Type of address family (af_e)
     *
     *  @code
     *  C++:
     *     class myAdjMgrHandler : public NxAdjMgrHandler {
     *        public:
     *           void postAdjCb(NxAdj *adj) {
     *                cout << "Adjacency AF: " << adj->getAF();
     *           }
     *     }
     *
     *  Python:
     *     class pyAdjHandler(nx_sdk_py.NxAdjMgrHandler):
     *     ### Overloaded Callback to get Adjacency events
     *           def postAdjCb(self, nxadj):
     *               print "Adjacency AF: %d" % nxadj.getAF()
     *  @endcode
     **/
    virtual nxos::af_e getAF() = 0;

    /**
     *  Test equality of adjacency objects.
     *
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *      C++:
     *          NxAdj *adj1 = adjMgr->getAdj("Vlan100", "1.1.1.2");
     *          NxAdj *adj2 = adjMgr->getAdj("Vlan100", "1.1.1.3");
     *          if (*adj1 == *adj2)
     *              cout << "Equal" << endl;
     *      Python:
     *          adj1 = adjMgr.getAdj("Vlan100", "1.1.1.2")
     *          adj2 = adjMgr.getAdj("Vlan100", "1.1.1.2")
     *          if adj1 == adj2:
     *               print "Equal"
     *  @endcode
     *
     **/
    virtual bool operator==(NxAdj const &adj_obj) const = 0;

    /**
     *  Test inequality of adjacency objects.
     *
     *  @returns
     *     true if inequality property holds, false otherwise
     *
     *  @code
     *      C++:
     *          NxAdj *adj1 = adjMgr->getAdj("Vlan100", "1.1.1.2");
     *          NxAdj *adj2 = adjMgr->getAdj("Vlan100", "1.1.1.3");
     *          if (*adj1 != *adj2)
     *              cout << "Not Equal" << endl;
     *      Python:
     *          adj1 = adjMgr.getAdj("Vlan100", "1.1.1.2")
     *          adj2 = adjMgr.getAdj("Vlan100", "1.1.1.3")
     *          if adj1 != adj2:
     *               print "Not Equal"
     *  @endcode
     *
     **/
    virtual bool operator!=(NxAdj const &adj_obj) const = 0;

    /**
     *  Test equality of adjacency objects.
     *
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *      C++:
     *          NxAdj *adj1 = adjMgr->getAdj("Vlan100", "1.1.1.2");
     *          NxAdj *adj2 = adjMgr->getAdj("Vlan100", "1.1.1.2");
     *          if (adj1->equal(*adj2))
     *              cout << "Equal" << endl;
     *      Python:
     *          adj1 = adjMgr.getAdj("Vlan100", "1.1.1.2")
     *          adj2 = adjMgr.getAdj("Vlan100", "1.1.1.2")
     *          if adj1.equal(adj2):
     *               print "Equal"
     *  @endcode
     *
     **/
    virtual bool equal(NxAdj const &adj_obj) const = 0;
};

/**
 * @brief Abstract AdjMgr callback Interface to receive 
 *        adjacency updates from NX-OS.
 **/
class NxAdjMgrHandler
{
public:
    virtual ~NxAdjMgrHandler(){};

    /**
     * User to overload the postAdjCb callback method
     * to receive any registered Adjacency updates based
     * on the set watch adjacency filters.
     *
     * @param[in]  NxAdj object. Not valid outside of postAdjCb.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_adj_mgr.h>
     *
     *     class myAdjMgrHandler : public NxAdjMgrHandler {
     *        public:
     *           void postAdjCb(NxAdj *adj) {
     *                // Do your action
     *           }
     *     };
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myAdjMgrHandler(nx_sdk_py.NxAdjMgrHandler):
     *     ### Overloaded Callback to get Adjacency events
     *           def postAdjCb(self, nxadj):
     *               # Do your action here
     *  @endcode
     **/
    virtual void postAdjCb(NxAdj *adj_obj) {}

    /**
     * User to overload the postAdjIPv4DownloadDone callback method
     * to be notified when watchAllAdjs (or) watchAdj with download
     * = true filter is set to signal the completion of download of 
     * existing IPV4 adjacencies. 
     * @param[in]  intfName: interface name. set to "" for download all
     *                       adjacencies across all interfaces.
     * @param[in]  ipv4_addr: IPV4 Address. set to "" for download all
     *                        adjacencies across all interfaces or
     *                        all adjacencies for a single interface.
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_adj_mgr.h>
     *
     *     class myAdjMgrHandler : public NxAdjMgrHandler {
     *        public:
     *           bool postAdjIPv4DownloadDone(const std::string &intfName,
     *                                        const std::string &ipv4_addr) {
     *                // Do your action
     *                if (intfName.empty()) {
     *                    cout << "Download complete for all existing ipv4 adjs\n";
     *                } else {
     *                    if (ipv4_addr.empty()) {
     *                        cout << "Download complete for all exising ipv4 adjs in " << intfName;
     *                    } else {
     *                        cout << "Download complete for ipv4 adj " << ipv4_addr << "in " << intfName;    
     *                    }
     *                }
     *           }
     *     };
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myAdjMgrHandler(nx_sdk_py.NxAdjMgrHandler):
     *     ### Overloaded Callback to get Adjacency events
     *           def postAdjIPv4DownloadDone(self, intf_name, ipv4_addr):
     *               # Do your action here
     *               if not intfName:
     *                    print "Download complete for all existing ipv4 adjs\n"
     *               else:
     *                  if not ipv4_addr:
     *                     print "Download complete for all exising ipv4 adjs in %s" % (intfName)
     *                  else:
     *                     print "Download complete for ipv4 adj %s in %s" % (ipv4_addr, intfName) 
     *  @endcode
     **/
    virtual void postAdjIPv4DownloadDone(const std::string &intfName,
                                         const std::string &ipv4_addr) {}
    /**
     * User to overload the postAdjIPv6DownloadDone callback method
     * to be notified when watchAllAdjs (or) watchAdj with download
     * = true filter is set to signal the completion of download of 
     * existing IPV6 adjacencies. 
     * @param[in]  intfName: interface name. set to "" for download all
     *                       adjacencies across all interfaces.
     * @param[in]  ipv6_addr: IPV6 Address. set to "" for download all
     *                        adjacencies across all interfaces or
     *                        all adjacencies for a single interface.
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_adj_mgr.h>
     *
     *     class myAdjMgrHandler : public NxAdjMgrHandler {
     *        public:
     *           bool postAdjIPv6DownloadDone(const std::string &intfName,
     *                                        const std::string &ipv6_addr) {
     *                // Do your action
     *                if (intfName.empty()) {
     *                    cout << "Download complete for all existing ipv6 adjs\n";
     *                } else {
     *                    if (ipv6_add.empty()) {
     *                        cout << "Download complete for all exising ipv6 adjs in " << intfName;
     *                    } else {
     *                        cout << "Download complete for ipv6 adj " << ipv6_addr << "in " << intfName;    
     *                    }
     *                }
     *           }
     *     };
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myAdjMgrHandler(nx_sdk_py.NxAdjMgrHandler):
     *     ### Overloaded Callback to get Adjacency events
     *           def postAdjIPv6DownloadDone(self, intf_name, ipv6_addr):
     *               # Do your action here
     *               if not intfName:
     *                    print "Download complete for all existing ipv6 adjs\n"
     *               else:
     *                  if not ipv6_addr:
     *                     print "Download complete for all exising ipv6 adjs in %s" % (intfName)
     *                  else:
     *                     print "Download complete for ipv6 adj %s in %s" % (ipv6_addr, intfName)
     *  @endcode
     **/
    virtual void postAdjIPv6DownloadDone(const std::string &intfName,
                                         const std::string &ipv6_addr) {}
};

/**
 *  @brief Abstract NX-SDK Adj Manager interface to gain access 
 *  to NXOS adjacency Manager.
 **/
class NxAdjMgr
{
public:
    /**
     * Destructor
     *
     * Delete the Adjacency manager instance and its associated
     * objects gracefully. Its cleaned on delete sdk too.
     **/
    virtual ~NxAdjMgr() {}

    /**
     * Lookup adjacency based on Interface & Address.
     * @param[in]  intfName: interface name.
     * @param[in]  ipAddr: IPV4 or IPV6 Address.
     *
     * @returns NxAdj Object if it exists.
     *          NULL if it does not exist.
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_adj_mgr.h>
     *
     *       NxAdj *adj;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       adjMgr = sdk->getAdjMgr();
     *
     *       adj = adjMgr->getAdj("Vlan100", "10.1.1.2");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       adjMgr = sdk.getAdjMgr()
     *
     *       adj = adjMgr.getAdj("Vlan100", "10.1.1.2")
     *
     *  @endcode
     *  
     *  @throws ERR_INVALID Invalid intfName & ipAddr
     **/
    virtual NxAdj *getAdj(const std::string &intfName,
                          const std::string &ipAddr) = 0;

    /**
     * Discover an adjacency by probing the interface and IP address for
     * an adjacency. If one exists, user will be notified via postAdjCb.
     * User must have postAdjCb registered and have a watch on the host
     * via watchAllAdjs or watchAdj API.
     *
     * @param[in]  intfName: interface name.
     * @param[in]  ipAddr: IPV4 or IPV6 Address.
     *
     * @returns true if discoverAdj operation is successful
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_adj_mgr.h>
     *
     *       NxAdj *adj;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       adjMgr = sdk->getAdjMgr();
     *       adjMgr->watchAllAdjs();
     *       adj = adjMgr->discoverAdj("Vlan100", "10.1.1.2");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       adjMgr = sdk.getAdjMgr()
     *       adjMgr.watchAllAdj()
     *       adj = adjMgr.discoverAdj("Vlan100", "10.1.1.2")
     *
     *  @endcode
     *  
     *  @throws ERR_INVALID Invalid intfName & ipAddr
     **/
    virtual bool discoverAdj(const std::string &intfName,
                             const std::string &ipAddr) = 0;

    /**
     * Watch All Adjacencies and get notified when an adjacency is created or 
     * deleted. User must have postAdjCb registered to receive events. 
     * 
     * @param[in]  addr_family: [Optional] nxos::AF_IPV4 or nxos::AF_IPV6
     * @param[in]  download: [Optional] true or false. If true, all adjacencies already 
     *                       created in the system will be notified via 
     *                       postAdjCb and the end of download is signalled
     *                       by overloading the respective postAdjIPv4DownloadDone 
     *                      (or) postAdjIPv6DownloadDone callbacks. 
     *                        
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_adj_mgr.h>
     *
     *       NxAdj *adj;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       adjMgr = sdk->getAdjMgr();
     *
     *       adjMgr->watchAllAdjs();
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       adjMgr = sdk.getAdjMgr()
     *
     *       adjMgr.watchAllAdjs()
     *  @endcode
     *  
     **/
    virtual void watchAllAdjs(nxos::af_e addr_family = AF_IPV4,
                              bool       download = true) = 0;

    /**
     * UnWatch All adjacencies. Stop receiving adjacency events. 
     * 
     * @param[in]  addr_family: [Optional] nxos::AF_IPV4 or nxos::AF_IPV6
     * 
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_adj_mgr.h>
     *
     *       NxAdj *adj;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       adjMgr = sdk->getAdjMgr();
     *
     *       adjMgr->unwatchAllAdjs();
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       adjMgr = sdk.getAdjMgr()
     *
     *       adjMgr.unwatchAllAdjs()
     *  @endcode
     **/
    virtual void unwatchAllAdjs(nxos::af_e addr_family = AF_IPV4) = 0;

    /**
     * Watch an adjacency
     * @param[in] intfName interface name in string.
     * @param[in] addr_family: [Optional] nxos::AF_IPV4 or nxos::AF_IPV6
     * @param[in] download: [Optional] true or false. If true, if the adjacency is
     *                      already created in the system will be notified via 
     *                      postAdjCb and the end of download is signalled
     *                      by overloading the respective postAdjIPv4DownloadDone 
     *                      (or) postAdjIPv6DownloadDone callbacks. 
     * @param[in] ipAddr [Optional] IP address in string.
     *                    If not specified, "" is used to
     *                    watch on all IP address.
     * @returns bool True -  if successful.
     *               False - if not successful.
     *
     *  @throws 
     *  ERR_INVALID if intfName or addr_family or ipAddr is invalid.
     *  
     **/
    virtual bool watchAdj(const std::string &intfName,
                          nxos::af_e        addr_family = AF_IPV4,
                          bool              download = true,
                          const std::string &ipAddr = "") = 0;

    /**
     * unwatch adjacency. Register for receiving all adjacencies on a specified 
     * interface, or a specific adjacency if you specify the IP. 
     * 
     * @param[in]  intfName interface name in string.
     * @param[in]  addr_family: [Optional] nxos::AF_IPV4 or nxos::AF_IPV6
     * @param[in] ipAddr [Optional] IP address in string.
     *                    If not specified, "" is used to
     *                    unwatch on all IP address.
     * @returns bool True -  if successful.
     *               False - if not successful.
     *
     *  @throws 
     *  ERR_INVALID if intfName or addr_family or ipAddr is invalid.
     **/
    virtual bool unwatchAdj(const std::string &intfName,
                            nxos::af_e        addr_family = AF_IPV4,
                            const std::string &ipAddr = "") = 0;

    /**
     * Register your handler object with Adjacency manager to
     * handle adjacency events.
     * @param[in]  NxAdjMgrHandler object with user overloaded
     *             functions to handle adjacency events.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_adj_mgr.h>
     *
     *     class myAdjMgrHandler : public NxAdjMgrHandler {
     *        public:
     *           void postAdjCb(NxAdj *adj) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxAdjMgr *adjMgr = sdk->getAdjMgr();
     *          NxAdjMgrHandler *myadjcb = new myAdjMgrHandler();
     *          adjMgr->setAdjMgrHandler(myadjcb);
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myAdjMgrHandler(nx_sdk_py.NxAdjMgrHandler):
     *     ### Overloaded Callback to get Adjacency events
     *           def postAdjCb(self, nxadj):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         adjMgr = sdk.getAdjMgr()
     *         myadjcb = myAdjMgrHandler()
     *         adjMgr.setAdjMgrHandler(myadjcb)
     *  @endcode
     **/
    virtual void setAdjMgrHandler(NxAdjMgrHandler *handler) = 0;

    /**
     * Unset Adjacency Manager Handler
     **/
    virtual void unsetAdjMgrHandler() = 0;

    /**
     * Get the Handler object registered with Adjacency Manager.
     *
     * @returns user defined NxAdjMgrHandler Obj if registered using
     *          setAdjMgrHandler (or) NULL.
     **/
    virtual NxAdjMgrHandler *getAdjMgrHandler() = 0;
};
} // namespace nxos

#endif
