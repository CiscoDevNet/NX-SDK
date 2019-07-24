/** @file nx_mac_mgr.h
 *  @brief Abstract SDK interface to Nexus(NXOS) MAC Management
 *
 *  Provides necessary Abstraction/Plugin for NXOS MAC functionalities.
 *  It provides the Ability to register for MAC updates and more.
 *
 *  Steps to register for MAC event:
 *   1) Instantiate NxSDK Mac Manager Object using getMacMgr().
 *   2) To watch for MAC updates, use watchMac() (or) watchAllMac()
 *      for all MAC updates.
 *   3) Register for callback handler with NxMacMgr to get the regsitered
 *      MAC updates.
 *   4) Overload NxMacMgrHandler class and its member functions like
 *      (postMacCb() - To get watched MAC updates)
 *      with your own implementation to handle various events.
 *
 *  @since NXSDK V1.7.5
 *
 *  Refer to some example Applications.
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 *
 * Copyright (c) 2018 by cisco Systems, Inc.
 * All rights reserved.

 */

#ifndef __NX_MAC_MGR_H__
#define __NX_MAC_MGR_H__

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include "types/nx_common.h"

namespace nxos
{

/**
 * @brief Abstract MAC Interface used to get details about MAC entry.
 **/
class NxMac
{
public:
    /**
     * Destructor, called when application releases object reference.
     * Removes the object from the SDK map.
     **/
    virtual ~NxMac() {}

    /**
     * To get the MAC address from the given MAC object.
     *
     * @returns MAC address (string)
     *
     *  @code
     *  C++:
     *      NxMacMgr *macMgr = sdk->getMacMgr();
     *      if (macMgr) {
     *          NxMac *mac = macMgr->getMac("1.1.1", 2);
     *          if (mac) {
     *              std::string macAddress = mac->getMacAddress();
     *          }
     *
     *  Python:
     *      macMgr = sdk.getMacMgr()
     *      if macMgr:
     *          mac = macMgr.getMac("1.1.1", 2)
     *          if mac:
     *             print "Mac Address: %s" % mac.getMacAddress()
     *  @endcode
     **/
    virtual std::string getMacAddress() = 0;

    /**
     * To get MAC Type (Static or Dynamic) from the given MAC object.
     *
     * @returns MAC type (mac_type_e)
     *
     *  @code
     *  C++:
     *       nxos::mac_type_e type = mac->getType();
     *
     *  Python:
     *       print "Mac type: %s" % getTypeStr(mac.getType())
     *  @endcode
     **/
    virtual nxos::type_e getType() = 0;

    /**
     * To get Vlan from the given MAC object.
     *
     * @returns vlan (unsigned int)
     *
     *  @code
     *  C++:
     *       unsigned int vlan = mac->getVlan();
     *
     *  Python:
     *       print "Vlan: %d" % int(mac.getVlan())
     *  @endcode
     **/
    virtual unsigned int getVlan() = 0;

    /**
     * To get outgoing Interface from the given MAC object.
     *
     * @returns interface name (string)
     *
     *  @code
     *  C++:
     *       std::string interface = mac->getIntfName();
     *
     *  Python:
     *       print "Interface: %s" % mac.getIntfName()
     *  @endcode
     **/
    virtual std::string getIntfName() = 0;

    /**
     * To get the type of Event(Add, Update, Delete etc).
     * Should be used only in MAC Handler Callbacks like
     * postMacCb(). If used outside of MAC Handler
     * Callbacks its set to NO_EVENT.
     *
     * @returns Type of MAC Event (event_type_e)
     *
     *  @code
     *  C++:
     *     class myMacMgrHandler : public NxMacMgrHandler {
     *        public:
     *           bool postMacCb(NxMac *mac) {
     *                cout << "MAC Event: " << mac->getEvent();
     *           }
     *     }
     *
     *  Python:
     *     class pyMacHandler(nx_sdk_py.NxMacMgrHandler):
     *     ### Overloaded Callback to get MAC events
     *           def postMacCb(self, nxmac):
     *               print "MAC Event: %d" % nxmac.getEvent()
     *  @endcode
     **/
    virtual nxos::event_type_e getEvent() = 0;

    /**
     *  Test equality of MAC objects. 
     *  NOTE: To be used only in C++ & Python. 
     *        For GO apps, use equal API. 
     *
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *      C++:
     *          NxMacMgr *macMgr = sdk->getMacMgr();
     *          if (macMgr) {
     *              NxMac *mac1 = macMgr->getMac("1.1.1", 2);
     *              NxMac *mac2 = macMgr->getMac("1.1.1", 2);;
     *              if (*mac1 == *mac2)
     *                  cout << "Equal" << endl;
     *          }
     *      Python:
     *          macMgr = sdk.getMacMgr()
     *          if macMgr:
     *             mac1 = macMgr.getMac("1.1.1", 2)
     *             mac2 = macMgr.getMac("1.1.1", 2)
     *             if mac1 == mac2:
     *                print "Equal"
     *  @endcode
     *
     **/
    virtual bool operator==(NxMac const &mac_obj) const = 0;

    /**
     *  Test inequality of MAC objects.
     *  NOTE: To be used only in C++ & Python. 
     *        For GO apps, use equal API. 
     *
     *  @returns
     *     true if inequality property holds, false otherwise
     *
     *  @code
     *      C++:
     *          NxMacMgr *macMgr = sdk->getMacMgr();
     *          if (macMgr) {
     *              NxMac *mac1 = macMgr->getMac("1.1.1", 2);
     *              NxMac *mac2 = macMgr->getMac("1.1.1", 5);
     *              if (*mac1 != *mac2)
     *                  cout << "Not Equal" << endl;
     *          }
     *      Python:
     *          macMgr = sdk.getMacMgr()
     *          if macMgr:
     *             mac1 = macMgr.getMac("1.1.1", 2)
     *             mac2 = macMgr.getMac("1.1.1", 5)
     *             if mac1 != mac2:
     *                print "Not Equal"
     *  @endcode
     *
     **/
    virtual bool operator!=(NxMac const &mac_obj) const = 0;

    /**
     *  Test equality of MAC objects.
     *  NOTE: Generic API to check for equality of MAC objects
     *        across all languages (esp for langiages that
     *        dont support operator overloading)
     *
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *      C++:
     *          NxMacMgr *macMgr = sdk->getMacMgr();
     *          if (macMgr) {
     *              NxMac *mac1 = macMgr->getMac("1.1.1", 2);
     *              NxMac *mac2 = macMgr->getMac("1.1.1", 2);;
     *              if (*mac1 == *mac2)
     *                  cout << "Equal" << endl;
     *          }
     *      Python:
     *          macMgr = sdk.getMacMgr()
     *          if macMgr:
     *             mac1 = macMgr.getMac("1.1.1", 2)
     *             mac2 = macMgr.getMac("1.1.1", 2)
     *             if mac1 == mac2:
     *                print "Equal"
     *  @endcode
     *
     **/
    virtual bool equal(NxMac const &mac_obj) const = 0;
};

/**
 * @brief Abstract MacMgr callback Interface to receive 
 *        MAC updates from NX-OS.
 **/
class NxMacMgrHandler
{
public:
    /**
     * Destructor.
     **/
    virtual ~NxMacMgrHandler(){};

    /**
     * User to overload the postMacCb callback method
     * to receive any registered MAC updates based on
     * the set watch filters.
     * @param[in]  NxMac object.
     *
     * @returns True : if the action was successful.
     *          False: if the action was not successful.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_mac_mgr.h>
     *
     *     class myMacMgrHandler : public NxMacMgrHandler {
     *        public:
     *           bool postMacCb(NxMac *mac) {
     *                // Do your action
     *           }
     *     };
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myMacMgrHandler(nx_sdk_py.NxMacMgrHandler):
     *     ### Overloaded Callback to get MAC events
     *           def postMacCb(self, nxmac):
     *               # Do your action here
     *  @endcode
     **/
    virtual bool postMacHandlerCb(NxMac *mac_obj) { return (true); }

    /**
     * Callback used to mark the end of a requested object
     * download operation.  This will be called after the last
     * event is sent to the application.
     *
     * @param[in] id value returned to app that matches the watch download. For
     *            watch "all", the value is zero; for download of all values
     *            in a vlan, the id is the vlan id.  For a specific mac watch,
     *            the macaddr will be set.  The app can use this to
     *            correlate the download cb instance to the download
     *            transaction to tell when it is complete.
     * @param[in] macaddr  Mac address returned in case a specific mac is downloaded
     *
     * @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_mac_mgr.h>
     *
     *     class myMacMgrHandler : public NxMacMgrHandler {
     *        public:
     *           bool postMacHandlerDownloadCb(unsigned int id, 
     *                                         const std::string &macaddr) {
     *                // Do your action
     *                if (id == 0) {
     *                    cout << "Download complete for all existing MACs";
     *                } else {
     *                    if (macaddr.empty()) {
     *                        cout << "Download complete for all existing MACs in Vlan " << id;
     *                    } else {
     *                         cout << "Download complete for MAC " << macaddr << "in Vlan " << id;
     *                    }
     *                }
     *           }
     *     };
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myMacMgrHandler(nx_sdk_py.NxMacMgrHandler):
     *     ### Overloaded Callback to get MAC events
     *           def postMacHandlerDownloadCb(self, id, macaddr):
     *               # Do your action here
     *               if id == 0:
     *                  print "Download complete for all existing MACs"
     *               else:
     *                  if not macaddr:
     *                     print ("Download complete for all existing MACs in Vlan %d" % id) 
     *                  else:
     *                     print ("Download complete for MAC %s in Vlan %d" % (macaddr, id))
     * @endcode
     **/
    virtual void postMacHandlerDownloadCb(unsigned int      id,
                                          const std::string &macaddr)
    {
        return;
    }
};

/**
 *  @brief Abstract NX-SDK MAC Manager interface to gain access 
 *  to NXOS MAC address table.
 **/
class NxMacMgr
{
public:
    /**
     * Destructor
     *
     * To delete the MAC manager instance and its associated
     * objects gracefully. Its cleaned on delete sdk too.
     **/
    virtual ~NxMacMgr() {}

    /**
     * To add static mac
     * 
     * @param[in] macAddr: Mac address to add
     * @param[in] vlan   : vlan value
     * @param[in] intf_name: Interface name that connects to this mac. 
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     * @returns True  - if successful.
     *          False - if it not successful.     
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_mac_mgr.h>
     *
     *       NxMac *mac;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       macMgr = sdk->getMacMgr();
     *       if (macMgr) {
     *           bool ret = macMgr->addStaticMac("1.1.1", 2, "eth1/1");
     *           if (ret) {  
     *               cout << "Static MAC added\n";
     *           } else {
     *               cout << "Failed to add static mac\n";
     *           }
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       macMgr = sdk.getMacMgr()
     *       if macMgr:
     *           ret = macMgr.addStaticMac("1.1.1", 2, "eth1/1")
     *           if ret:
     *              print "static mac added\n"
     *           else:
     *              print "Failed to add static mac\n"
     *  @endcode
     *
     *  @throws ERR_INVALID macAddr or vlan or intf_name is invalid.
     */
    virtual bool addStaticMac(const std::string  &macAddr,
                              const unsigned int vlan,
                              const std::string  &intf_name) = 0;
    /**
     * To delete static mac
     * @param[in] macAddr: Mac address to remove
     * @param[in] vlan   : vlan value
     * @returns bool True  - if successful.
     *               False - if not successful.
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_mac_mgr.h>
     *
     *       NxMac *mac;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       macMgr = sdk->getMacMgr();
     *       if (macMgr) {
     *           bool ret = macMgr->delStaticMac("1.1.1", 2, "eth1/1");
     *           if (ret) {  
     *               cout << "Static MAC deleted\n";
     *           } else {
     *               cout << "Failed to delete static mac\n";
     *           }
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       macMgr = sdk.getMacMgr()
     *       if macMgr:
     *           ret = macMgr.addStaticMac("1.1.1", 2, "eth1/1")
     *           if ret:
     *              print "static mac deleted\n"
     *           else:
     *              print "Failed to delete static mac\n"
     *  @endcode
     *
     *  @throws ERR_INVALID macAddr or vlan is invalid.
     *
     **/
    virtual bool delStaticMac(const std::string  &macAddr,
                              const unsigned int vlan) = 0;

    /**
     * To get the MAC object from a given MAC address and vlan.
     * @param[in]  macAddr MAC address in string.
     * @param[in]  vlan vlan in unsigned int
     * @param[in]  clearBuffer [Optional] If the MAC is present,
     *                         getMac() creates a new MAC object.
     *                         [Recommended] App owners must free/destroy
     *                         the MAC object once done. If they fail to
     *                         free it, NXSDK will store the object until
     *                         it is freed. When clearBuffer flag is set
     *                         to True it will free all the previously 
     *                         created MAC objects.
     * @returns NxMac Object if it exists.
     *          NULL if it does not exist.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_mac_mgr.h>
     *
     *       NxMac *mac;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       macMgr = sdk->getMacMgr();
     *       if (macMgr) {
     *           mac = macMgr->getMac("1.1.1", 2);
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       macMgr = sdk.getMacMgr()
     *       if macMgr:
     *           mac = macMgr.getMac("1.1.1", 2)
     *  @endcode
     *
     *  @throws ERR_INVALID macAddr or vlan is invalid.
     **/
    virtual NxMac *getMac(const std::string  &macAddr,
                          const unsigned int vlan,
                          bool               clearBuffer = false) = 0;

    /**
     * To receive/subscribe for all MAC updates.
     * On any update to any MAC, user overloaded postMacCb
     * handler will be called.
     * 
     * @param[in] download: [Optional] If set to true, all existing macs in the system
     *                     will be notified to the application via the 
     *                     registered callback.
     *
     * @returns bool True -  if successful.
     *               False - if not successful. 
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_mac_mgr.h>
     *
     *       class myMacMgrHandler : public NxMacMgrHandler {
     *          public:
     *             bool postMacCb(NxMac *mac) {
     *                  cout << "MAC changed";
     *             }
     *       };
     *
     *       int  main (int argc, char **argv)
     *       {
     *            NxSdk    *sdk;
     *            NxMacMgr *macMgr;
     *            NxMac    *mac;
     *
     *            NxMacMgrHandler *mymaccb = new myMacMgrHandler();
     *            sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *            macMgr = sdk->getMacMgr();
     *            macMgr->setMacMgrHandler(mymaccb);
     *
     *            macMgr->watchAllMac();
     *       }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myMacMgrHandler(nx_sdk_py.NxMacMgrHandler):
     *     ### Overloaded Callback to get MAC events
     *           def postMacCb(self, nxmac):
     *               print "MAC has changed"
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         macMgr = sdk.getMacMgr()
     *         mymaccb = myMacMgrHandler()
     *         macMgr.setMacMgrHandler(mymaccb)
     *
     *         macMgr.watchAllMac()
     *  @endcode
     **/
    virtual bool watchAllMac(bool download = true) = 0;

    /**
     * To unsubscribe for all MAC updates.
     *
     * @returns bool True -  if successful.
     *               False - if not successful.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_mac_mgr.h>
     *
     *       NxMac *mac;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       macMgr = sdk->getMacMgr();
     *
     *       macMgr->unwatchAllMac();
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       macMgr = sdk.getMacMgr()
     *
     *       macMgr.unwatchAllMac()
     *  @endcode
     **/
    virtual bool unwatchAllMac() = 0;

    /**
     * To receive/subscribe for any updates to this vlan and
     * mac address. On any update to this MAC, users
     * overloaded postMacCb handler will be called. To
     * subscribe all MAC address for specific vlan, use
     * macAddr as "".
     * NOTE: Forward referencing is allowed for a non created
     *       MAC i.e API can be called even if the MAC is not
     *       yet created. Subscribing all vlans for specific
     *       MAC address is not supported.
     * @param[in] vlan vlan in unsigned int.
     * @param[in] macAddr [Optional] MAC address in string.
     *                    If not specified, "" is used to
     *                    watch on all MAC address.
     * @param[in] download  [Optional] Specify to get sync download of existing MAC
     *                      objects or not (default True)
     * @returns bool True -  if successful.
     *               False - if not successful.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_mac_mgr.h>
     *
     *       class myMacMgrHandler : public NxMacMgrHandler {
     *          public:
     *             bool postMacCb(NxMac *mac) {
     *                  cout << "MAC changed";
     *             }
     *       };
     *
     *       int  main (int argc, char **argv)
     *       {
     *            NxSdk    *sdk;
     *            NxMacMgr *macMgr;
     *            NxMac    *mac;
     *
     *            NxMacMgrHandler *mymaccb = new myMacMgrHandler();
     *            sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *            macMgr = sdk->getMacMgr();
     *            macMgr->setMacMgrHandler(mymaccb);
     *
     *            macMgr->watchMac(2, "1.1.1");
     *       }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myMacMgrHandler(nx_sdk_py.NxMacMgrHandler):
     *     ### Overloaded Callback to get MAC events
     *           def postMacCb(self, nxmac):
     *               print "MAC has changed"
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         macMgr = sdk.getMacMgr()
     *         mymaccb = myMacMgrHandler()
     *         macMgr.setMacMgrHandler(mymaccb)
     *
     *         macMgr.watchMac(2, "1.1.1")
     *  @endcode
     *
     *  @throws ERR_INVALID macAddr or vlan is invalid.
     **/
    virtual bool watchMac(const unsigned int vlan,
                          const std::string  &macAddr = "",
                          bool               download = true) = 0;

    /**
     * To un-subscribe for any updates to this vlan and mac address.
     * To un-subscribe all MAC address for specific vlan, use
     * macAddr as "".
     * @param[in] vlan vlan in unsigned int.
     * @param[in] macAddr [Optional] MAC address in string.
     *                    If not specified, "" is used to
     *                    watch on all MAC address.
     * @returns bool True -  if successful.
     *               False - if not successful.
     *
     *  @code
     *  C++:
     *       #include <nx_sdk.h>
     *       #include <nx_mac_mgr.h>
     *
     *       NxMac *mac;
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       macMgr = sdk->getMacMgr();
     *
     *       macMgr->unwatchMac(2, "1.1.1");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       macMgr = sdk.getMacMgr()
     *
     *       macMgr.unwatchMac(2, "1.1.1")
     *  @endcode
     *
     *  @throws ERR_INVALID macAddr or vlan is invalid.
     **/
    virtual bool unwatchMac(const unsigned int vlan,
                            const std::string  &macAddr = "") = 0;

    /**
     * Users to overload MAC Manager Handler class to handle
     * registered events like MAC updates and register the
     * handler object with MAC manager to handle mac events.
     * @param[in]  NxMacMgrHandler object with user overloaded
     *             functions to handle mac events.
     *
     *  @code
     *  C++:
     *     #include <nx_sdk.h>
     *     #include <nx_mac_mgr.h>
     *
     *     class myMacMgrHandler : public NxMacMgrHandler {
     *        public:
     *           bool postMacCb(NxMac *mac) {
     *                // Do your action
     *           }
     *     };
     *
     *     int  main (int argc, char **argv)
     *     {
     *          NxSdk    *sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *          NxMacMgr *macMgr = sdk->getMacMgr();
     *          NxMacMgrHandler *mymaccb = new myMacMgrHandler();
     *          macMgr->setMacMgrHandler(mymaccb);
     *     }
     *
     *  Python:
     *     import nx_sdk_py
     *
     *     class myMacMgrHandler(nx_sdk_py.NxMacMgrHandler):
     *     ### Overloaded Callback to get MAC events
     *           def postMacCb(self, nxmac):
     *               # Do your action here
     *
     *     # Do SDK related inits in one thread.
     *     def sdkThread(name,val):
     *         sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *         macMgr = sdk.getMacMgr()
     *         mymaccb = myMacMgrHandler()
     *         macMgr.setMacMgrHandler(mymaccb)
     *  @endcode
     **/
    virtual void setMacMgrHandler(NxMacMgrHandler *handler) = 0;

    /**
     * To get the Handler object registered with MAC Manager.
     *
     * @returns user defined NxMacMgrHandler Obj if registered using
     *          setMacMgrHandler (or) NULL.
     **/
    virtual NxMacMgrHandler *getMacMgrHandler() = 0;
};

} // namespace nxos

#endif //__NX_MAC_MGR_H__
