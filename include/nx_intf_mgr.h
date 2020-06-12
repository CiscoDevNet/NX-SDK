/**
 * @file  nx_intf_mgr.h
 * @brief Abstract SDK interface to Nexus(NXOS) Interface Management 
 *
 * Provides abstractions for NXOS interface functionalites such as 
 * reading and setting interface settings, and handling certain
 * interface events.
 *
 *  Copyright (c) 2018 by cisco Systems, Inc.
 *  All rights reserved.
 *
 *  @since  NXSDK V1.7.5
 *  @author nxos-sdk@cisco.com
 */
#ifndef __NX_INTF_MGR_H__
#define __NX_INTF_MGR_H__

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include "types/nx_common.h"

namespace nxos
{

/**
 * @brief  Public interface to NX-SDK interface objects.
 *
 *
 */
class NxIntf
{
  public:
    /**
     *  @brief: Interface speed values.
     *
     **/
    #define INTF_SPEED_AUTO 0
    #define INTF_SPEED_100M 100
    #define INTF_SPEED_1G 1000
    #define INTF_SPEED_10G 10000
    #define INTF_SPEED_100G 100000
    #define INTF_SPEED_25G 25000
    #define INTF_SPEED_40G 40000
    #define INTF_SPEED_AUTO_100 110
    #define INTF_SPEED_AUTO_100_1000 120
    #define INTF_SPEED_UNKNOWN ~((uint32_t)0)
    
    /**
     * Destructor, called when application releases object reference.  
     * Removes the object from the SDK map.
     **/
    virtual ~NxIntf(){};

    /**
     *  Retrieve the interface name from the object.  The name is of
     *  the short form as shown in "show interface brief".
     *
     *  @returns  interface name (string) 
     *
     *  @code
     *
     *  C++:
     *         std::string name = intf->getName();
     *  
     *  Python:
     *         print "Interface: " + intf.getName() 
     *  @endcode
     *
     *  @throws  None.
     *     
     **/
    virtual std::string getName() = 0;

    /**
     *  Get the network layer of this interface
     *
     *  @returns  Layer2 or Layer3 (string)
     *
     *  @code
     *
     *  C++:
     *           std::string layer = intf->getLayer();
     *  Python:
     *           print "\tLayer: " + intf.getLayer()
     *  @endcode
     *
     *  @throws   None.
     **/
    virtual std::string getLayer() = 0;

    /**
     *  Get the interface type
     *
     *  @returns numberic value of type intf_type_e
     *
     *  @code
     *
     *  C++:
     *         nxos::intf_type_e   type = intf->getType();
     *  Python:
     *         print "\tType: " + getTypeStr(intf.getType())
     *  @endcode
     *
     *  @throws   None.
     **/
    virtual nxos::intf_type_e getType() = 0;

    /**
     *  Return the vrf this interface belongs to
     *
     *  @returns  vrf name (string)
     *
     *  @code
     *
     *  C++:
     *         std::string  vrf = intf->getVrf()'
     *  Python:
     *         print "\tVrf: " + intf.getVrf() 
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual std::string getVrf() = 0;

    /**
     *  Return default vlan based on switching mode; if access mode
     *  return access vlan.  If trunking mode return native vlan.
     *  SVI returns vlan it is defined on.
     *
     *  @returns vlan name (string), same as "show vlan"
     *
     *  @code
     *
     *  C++:
     *        std::string vlan = intf->getVlan();
     *  Python:
     *         print "\tVlan: " + intf.getVlan()
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual std::string getVlan() = 0;

    /**
     *  Return the configured description for interface.
     *
     *  @returns description (string)
     *
     *  @code
     *
     *  C++:
     *         std::string desc = intf->getDescription();
     *  Python:
     *         print "\tDescr: " + intf.getDescription()
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual std::string getDescription() = 0;

    /**
     *  Get the interface primary IP address for address family
     *  @param[in]  af   Address family of L3 address.
     *  @returns  IPv4 or IPv6 address (string)
     *
     *  @code
     *
     *  C++:
     *          std::string ip4 = intf->getL3PrAddr(AF_IPV4);
     *          std::string ip6 = intf->getL3PrAddr(AF_IPV6);
     *  Python:
     *          print "\tIPv4: " + intf.getL3PrAddr(nx_sdk_py.AF_IPV4)
     *          print "\tIPv6: " + intf.getL3PrAddr(nx_sdk_py.AF_IPV6)
     *  @endcode
     *
     *  @throws  
     *      ERR_INVALID_USAGE -- invalid address family
     **/
    virtual std::string getL3PrAddr(af_e af) = 0;

    /**
     *  Return interface secondary IP addresses in address family.  Call
     *  iteratively to retrieve all addresses.  Returns empty string ""
     *  when no more addresses are available.
     *  @param[in] af   Address family of L3 address
     *  @param[in] fromFirst [Optional] Start from first address or not.
     *  @returns  IP address (string)
     *
     *  @code
     *
     *  C++:
     *       std::string l3_sec_addr = objp->getL3ScAddr(AF_IPV4, true);
     *       while (!l3_sec_addr.empty()) {
     *           cout << l3_sec_addr << " ";
     *           l3_sec_addr = objp->getL3ScAddr(AF_IPV4);
     *       }
     *  Python:
     *       l3_sec_addr = intf.getL3ScAddr(nx_sdk_py.AF_IPV4, True)
     *       while l3_sec_addr:
     *          print l3_sec_addr + " ",
     *          l3_sec_addr = intf.getL3ScAddr(nx_sdk_py.AF_IPV4)
     *  @endcode
     *
     *  @throws    
     *     ERR_INVALID_USAGE -- invalid address family
    **/
    virtual std::string getL3ScAddr(af_e af, bool fromFirst = false) = 0;

    /**
     *  Retrieve the configured interface MAC address.
     *
     *  @returns  mac address (string), or empty string "" if not
     *            available.
     *
     *  @code
     *
     *  C++:
     *       std::string  mac = intf->getL2Address();
     *  Python:
     *        print "\tL2: " + intf.getL2Address()
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual std::string getL2Address() = 0;

    /**
     *  Returns the backplane hw address for interface.  
     *
     *  @returns  mac address (string), or empty string "" if not
     *            available.
     *
     *  @code
     *
     *  C++:
     *        std::string  machw = intf->getL2AddressHW();
     *  Python:
     *         print "\tL2hw: " + intf.getL2AddressHW()
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual std::string getL2AddressHW() = 0;

    /**
     *  Get the administrative state of the interface.
     *
     *  @returns   nxos::state_type_e   UP, DOWN, UNKNOWN
     *
     *  @code
     *
     *  C++:
     *      nxos::state_type_e  state;
     *      std::string adminSt;
     *      state   = objp->getAdminState();
     *      adminSt = state == UP ? "up" : 
     *                    (state == DOWN ? "down":"unknown");
     *  Python:
     *       print "\tAdminSt: " + getStateStr(intf.getAdminState())
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual nxos::state_type_e getAdminState() = 0;

    /**
     *  Get the operational status of the interface
     *
     *  @returns   nxos::state_type_e  UP, DOWN, UNKNOWN
     *
     *  @code
     *
     *  C++:
     *      nxos::state_type_e  state;
     *      std::string operSt;
     *      state   = objp->getOperState();
     *      operSt = state == UP ? "up" : 
     *         (state == DOWN ? "down":"unknown");
     *  Python:
     *      print "\tOperSt: " + getStateStr(intf.getOperState())      
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual nxos::state_type_e getOperState() = 0;

    /**
     *  Get the configured MTU of the interface if available.
     *
     *  @returns  MTU integer value
     *
     *  @code
     *
     *  C++:
     *        uint mtu =  objp->getMtu();
     *  Python:
     *        print "\tMtu: " + str(intf.getMtu())
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual uint32_t getMtu() = 0;

    /**
     *  Get the configured interface speed  
     *
     *  @returns  Value representing the speed setting.  
     *            
     *
     *  @code
     *
     *  C++:
     *         uint32_t speed    =  objp->getSpeed();
     *  Python:
     *         print "\tSpeed: " + intf.getSpeed()
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual uint32_t getSpeed() = 0;

    /**
     *  Get the number of member ports in a channel group 
     *
     *  @returns  number of members in a port-channel 
     *
     *  @code
     *
     *  C++:
     *         cout << "Member count: " << objp->getMemberCount() << endl;
     *  Python:
     *         print "Member count: " + str(intf.getMemberCount())
     *  @endcode
     *
     *  @throws
     *     ERR_INVALID_USAGE -- wrong interface type
     **/
    virtual unsigned int getMemberCount() = 0;

    /**
     *  Iterate over the members in channel group.  The interface
     *  name is returned for each call until the end of the group
     *  is reached, then the empty string is returned.  The name
     *  returned is the short name format.
     *  @param[in] fromFirst [Optional] Start iterating at beginning or not.
     *  @returns member interface name (string)
     *
     *  @code
     *
     *  C++:
     *        std::string mbr;
     *  
     *        mbr = objp->getMembers(true);
     *        while (!mbr.empty()) {
     *            cout << mbr << endl;
     *            mbr = objp->getMembers();
     *        }
     *  Python
     *        if intf.getType() == nx_sdk_py.NX_TYPE_ETH_PC:
     *           mbr = intf.getMembers(True);
     *           while mbr:
     *              print "\t\t<" + mbr + ">"
     *              mbr = intf.getMembers();
     *
     *  @endcode
     *
     *  @throws
     *      ERR_INVALID_USAGE -- wrong interface type
     **/
    virtual std::string getMembers(bool fromFirst = false) = 0;

    /**
     *  Get the event code from the interface.  If an event occurs for
     *  an interface, this code is set in the NxIntf object before being
     *  delivered to the application handler.
     *
     *  @returns  The event type value of this object.
     *
     *  @code
     *     C++:
     *         bool myIntfHandler::postIntfAddDelCb (NxIntf *intf)
     *         {
     *          
     *         if (intf->getEvent() == ADD ) {
     *            cout << "App got ADD notification for interface  " << 
     *            intf->getName() << endl;
     *         } else if (intf->getEvent() == DELETE ) {
     *            cout << "App got DELETE notification for interface " << 
     *            intf->getName() << endl;
     *         }
     *         }  
     *
     *      Python:
     *         def postIntfAddDelCb(self, intf):
     *            event = intf.getEvent()
     *            print "Application Add/Del cb "
     *            print "intf " + intf.getName()
     *            if event == nx_sdk_py.ADD:
     *               print "ADDED"
     *            elif event == nx_sdk_py.DELETE:
     *               print "DELETED"
     *            return True;   
     *
     *  @endcode
     *
     *
     *  @throws  None.
     **/
    virtual nxos::event_type_e getEvent() = 0;

    /**
     *  Get the last modification time of interface
     *
     *  @returns  time of last modification (string)
     *             ex.  2018-01-19T00:45:08.159+00:00 
     *
     *  @code
     *
     *  C++:
     *      std::string  lastmod  =  objp->getLastMod();
     *  Python:
     *      print "\tLastMod: " + intf.getLastMod()   
     *  @endcode
     *
     *
     *  @throws   None.
     **/
    virtual std::string getLastMod() = 0;

    
    /**
     *  Set the interface switching mode, Layer2 or Layer3
     *  @param[in] layer   Set the interface layer value.
     *  @returns   true if successful
     *          
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.

     *  @code
     *     C++:
     *        ret = intf->setLayer("Layer3");
     *     Python:
     *        intf.setLayer("Layer2")
     *  @endcode
     *
     *
     *  @throws   ERR_INVALID_USAGE 
     *            ERR_FAILURE 
     **/
    virtual bool setLayer(const std::string &layer) = 0;

    /**
     *  Set the interface vrf membership.
     *  @param[in]  vrf   The vrf name to set.
     *  @returns  true if successful
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *     C++:
     *        intf->setVrf("vlan-test2");
     *     Python:
     *        intf.setVrf("vlan-test2")
     *  @endcode
     *
     *
     *  @throws    ERR_INVALID_USAGE -- invalid argument
     *             ERR_FAILURE -- operation failed
     *             ERR_INVALID -- vrf does no exist
     **/
    virtual bool setVrf(const std::string &vrf) = 0;

    /**
     *  Set interface vlan membership.  Input is the vlan id
     *  number.
     *  @param[in] vlan_id  The vlan id value to set.
     *  @returns  true if successful
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *     C++:
     *          intf->setVlan(3);
     *     Python:
     *          intf.setVlan(2)
     *  @endcode
     *
     *
     *  @throws   ERR_INVALID_USAGE -- invalid argument
     *            ERR_FAILURE -- operation failed
     **/
    virtual bool setVlan(unsigned int vlan_id) = 0;

    /**
     *  Set the interface description.
     *  @param[in]  desc  The description to set.
     *  @returns  true if successful
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *     C++:
     *        intf->setDescription("Net-1 interface");
     *     Python:
     *        intf.setDescription("Private segment interface")
     *  @endcode
     *
     *
     *  @throws  ERR_INVALID_USAGE -- invalid input 
     *           ERR_FAILURE -- operation failed
     **/
    virtual bool setDescription(const std::string &desc) = 0;

    /**
     *  Set the IPv4/IPv6 address for an L3 interface, primary or secondary.
     *  IP address string must be a string in form:
     *       A.B.C.D/mm
     *  @param[in] addr  The L3 address in string format.
     *  @param[in] type  The address family of the address.
     *  @param[in] primary  This is the primary address or not.
     *  @returns  true if successful
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *     C++:
     *         intf->setL3Address("192.168.4.1/24", AF_IPV4, true); //primary
     *         intf->setL3Address("192.168.4.100/24", AF_IPV4, false); //sec
     *     Python:
     *         intf.setL3Address("192.168.4.100/24", nx_sdk_py.AF_IPV4, False)
     *  @endcode
     *
     *
     *  @throws  ERR_INVALID_USAGE -- invalid input parameter
     *           ERR_FAILURE -- operation failed
     **/
    virtual bool setL3Address(const std::string &addr, af_e type,
                              bool primary) = 0;

    /**
     *  Configure L2 mac address for routed interface.  L2 address
     *  string must be in form:
     *       AA:BB:CC:DD:EE:FF
     *  @param[in] mac   The mac address in string format.
     *  @returns  true if successful
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *     C++:
     *           intf->setL2Address("52:54:00:C0:CB:C7");
     *     Python:
     *           intf.setL2Address("52:54:00:C0:CB:C7")
     *  @endcode
     *
     *
     *  @throws  ERR_INVALID_USAGE
     *           ERR_FAILURE
     *           ERR_INVALID
     **/
    virtual bool setL2Address(const std::string &mac) = 0;

    /**
     *  Set administrative state for interface
     *  @param[in] state   The state to set interface to.
     *  @returns  true if successful
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *    C++:
     *       intf->setAdminState(nxos::UP);
     *    Python:
     *       intf.setAdminState(nx_sdk_py.UP)
     *  @endcode
     *
     *
     *  @throws ERR_INVALID_USAGE
     *          ERR_FAILURE
     **/
    virtual bool setAdminState(nxos::state_type_e state) = 0;

    /**
     *  Set interface mtu.
     *  @param[in] mtu  The mtu value to set.
     *  @returns true if successful
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *      C++:
     *           intf->setMtu(1440);
     *      Python:
     *           intf.setMtu(1440)
     *  @endcode
     *
     *
     *  @throws  ERR_FAILURE
     *           ERR_INVALID_USAGE
     **/
    virtual bool setMtu(uint32_t mtu) = 0;

    /**
     *  Set interface speed.  Input values are one of INTF_SPEED_* 
     *  values.
     *  @param[in] speed  The speed value to set.
     *  @returns  true if successful
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     * 
     *  @code
     *     C++:
     *         intf->setSpeed(INTF_SPEED_10G);
     *     Python:
     *         intf.setSpeed(INTF_SPEED_10G)
     *  @endcode
     *
     *
     *  @throws  ERR_INVALID_USAGE
     *           ERR_INVALID
     *           ERR_FAILURE
     **/
    virtual bool setSpeed(uint32_t speed) = 0;

    /**
     *  Add interface to this port-channel interface.
     *  @param[in] intf_name  Interface to add to port-channel.
     *  @returns  true if successful.  
     *          
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *     C++:
     *        pc_intf->addMember("eth1/1");
     *     Python:
     *        pc_intf.addMember("eth1/2")
     *  @endcode
     *
     *
     *  @throws    ERR_INVALID_USAGE
     *             ERR_INVALID
     *             ERR_FAILURE
     **/
    virtual bool addMember(const std::string &intf_name) = 0;

    /**
     *  Remove interface from port-channel.
     *  @param[in] intf_name  Interface name to remove from port-channel.
     *  @returns true if successful.  
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *     C++:
     *         pc_intf->delMember("eth1/1");
     *     Python:
     *         pc_intf.delMember("eth1/2")
     *  @endcode
     *
     *
     *  @throws   ERR_INVALID_USAGE
     *            ERR_FAILURE
     **/
    virtual bool delMember(const std::string &intf_name) = 0;

    /**
     *  Test equality of intf objects.  Two objects are equal if they
     *  refer to the same interface name and have the same DME property
     *  values. 
     *  @param[in]  intf_obj  Interface to compare against.
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *      C++:
     *           NxIntf *obj1 = intfMgr->getIntf(arg1);
     *           NxIntf *obj2 = intfMgr->getIntf(arg2);
     *           if (*obj1 == *obj2)
     *               cout << "Equal" << endl;
     *      Python:
     *           obj1 = intfMgr.getIntf(cmds[1])
     *           obj2 = intfMgr.getIntf(cmds[2])
     *           if (obj1 == obj2):
     *               print "Equal"
     *  @endcode
     *
     *  @throws   None.
     *
     **/
    virtual bool operator==(NxIntf const &intf_obj) const = 0;

    /**
     *  Test inequality of interface objects.  Two objects are not equal 
     *  if they refer to different interface names or have different 
     *  DME property values.
     *  @param[in]  intf_obj  Interface to compare against.
     *  @returns
     *     true if inequality property holds, false otherwise
     *
     *  @code
     *      C++:
     *           NxIntf *obj1 = intfMgr->getIntf(arg1);
     *           NxIntf *obj2 = intfMgr->getIntf(arg2);
     *           if (*obj1 != *obj2)
     *               cout << "Not Equal" << endl;
     *      Python:
     *           obj1 = intfMgr.getIntf(cmds[1])
     *           obj2 = intfMgr.getIntf(cmds[2])
     *           if (obj1 != obj2):
     *               print "Not Equal"
     *
     *  @endcode
     *
     *  @throws   None.
     *
     **/
    virtual bool operator!=(NxIntf const &intf_obj) const = 0;

    /**
     *  Test equality of interface objects. This version can be
     *  used in languages that do not support operator overloading
     *  "==", such as Go. 
     *  @param[in]  intf_obj  Interface to compare against.
     *  @returns
     *     true if equality property holds, false otherwise
     *
     *  @code
     *     C++:
     *          NxIntf *obj1 = intfMgr->getIntf(arg1);
     *          NxIntf *obj2 = intfMgr->getIntf(arg2);
     *              if (obj1->equal(*obj2))
     *                  cout << "Equal" << endl;
     *              else
     *                  cout << "Not Equal" << endl;
     *
     *     Python:
     *         obj1 = intfMgr.getIntf(cmds[1])
     *         obj2 = intfMgr.getIntf(cmds[2])
     *         if (obj1.equal(obj2)):
     *             print "Equal"
     *         else:
     *             print "Not Equal"
     *  @endcode
     *
     *  @throws  None.
     *
     **/
    virtual bool equal(NxIntf const &intf_obj) const = 0;
};

/**
 * @brief  Public interface to NX-SDK interface events.  
 *
 * The application should
 * derive its own handler class from this and override the methods.  If
 * a watch is set for an interface, these events will be delivered to the 
 * application if the method is provided in the application handler
 * class.  The interface object passed to the handler is owned by the
 * library and should not be stored or freed by the application.
 * Interface Mgr events generated from NXOS are delivered to application
 * callbacks through NXSDK interface objects
 *  Steps to register for interface events:
 *  1)  Open NXSDK interface mgr object using getIntfMgr()
 *  2)  Define application callbacks by re-defining NxIntfMgrHandler class
 *  3)  Install application handlers using setIntfHandler()
 *  4)  call NxSdk::startEventLoop()
 *
 */
class NxIntfMgrHandler
{
  public:
    virtual ~NxIntfMgrHandler() {}
    
    /**
     *  Interface Add/Delete callback notification handler.
     *  @param[in]  obj  Interface object that received the event.
     *                   Object event is ADD if interface was added, 
     *                   or set to DELETE if interface was removed.
     *  @returns
     *     true if successful, false if failure
     *
     *  @code
     *      C++:
     *         bool
     *         myIntfHandler::postIntfAddDelCb (NxIntf *intf)
     *         {
     *          
     *         if (intf->getEvent() == ADD ) {
     *            cout << "App got ADD notification for interface  " << 
     *            intf->getName() << endl;
     *         } else if (intf->getEvent() == DELETE ) {
     *            cout << "App got DELETE notification for interface " << 
     *            intf->getName() << endl;
     *         }
     *         }  
     *
     *      Python:
     *         def postIntfAddDelCb(self, intf):
     *            event = intf.getEvent()
     *            print "Application Add/Del cb "
     *            print "intf " + intf.getName()
     *            if event == nx_sdk_py.ADD:
     *               print "ADDED"
     *            elif event == nx_sdk_py.DELETE:
     *               print "DELETED"
     *            return True;   
     *  @endcode
     *
     *  @throws
     *      Application exceptions are ignored.
     **/
    virtual bool postIntfAddDelCb(NxIntf *obj) { return true; }
    
    /**
     *  Interface IPv4 address change notification handler.
     *  @param[in]  obj  Interface object that received the event.
     *                   Object event values:
     *                      ADD : Primary address was added
     *                      UPDATE: Primary address was changed
     *                      DELETE: Primary address was removed
     *  @returns
     *     true if successful, false if failure
     *
     *  @code
     *     C++:
     *        bool  myIntfHandler::postIntfIpv4AddrCb (NxIntf *intf)
     *        {
     *          cout << "Application: " << __FUNCTION__ << endl;
     *          if (intf->getEvent() == ADD ) {
     *             cout << "App got ADD notification, new primary address for 
     *                       interface  " <<      intf->getName() << endl;
     *          }
     *          return true;
     *        }
     *
     *     Python:
     *        def postIntfIpv4AddrCb(self, intf):
     *           event = intf.getEvent()
     *           print "Application ipv4 addr cb "
     *           print "intf " + intf.getName()
     *           if event == nx_sdk_py.ADD:
     *              print "App got ADD notification, new primary address"
     *           return True
     *  @endcode
     *
     *  @throws
     *      Application exceptions are ignored.
     **/
    virtual bool postIntfIpv4AddrCb(NxIntf *obj) { return true; }

    /**
     *  Interface IPv6 address change notification handler.
     *  @param[in]  obj  Interface object that received the event.
     *                   Object event values:
     *                      ADD:  Address was added
     *                      UPDATE: Address was changed
     *                      DELETE: Address was removed
     *  @returns
     *     true if successful, false if failure
     *
     *  @code
     *     C++:
     *          bool
     *          myIntfHandler::postIntfIpv6AddrCb (NxIntf *intf)
     *          {
     *             cout << "Application: " << __FUNCTION__ << endl;
     *
     *             if (intf->getEvent() == UPDATE) {
     *               cout << "App got UPDATE notification, primary address change
     *                        for interface " <<  intf->getName() << endl;
     *             }
     *          }
     *
     *     Python:   
     *          def postIntfIpv6AddrCb(self, intf):
     *              event = intf.getEvent()
     *              print "intf " + intf.getName()
     *              if event == nx_sdk_py.ADD:
     *                 print "App got ADD notification, new IPv6 address"
     *              return True
     *  @endcode
     *
     *  @throws
     *      Application exceptions are ignored.
     **/
    virtual bool postIntfIpv6AddrCb(NxIntf *obj) { return true; }

    /**
     *  Interface L2/L3 state change notification handler.
     *  @param[in]  obj  Interface object that received the event.
     *                   Object event values:
     *                      UPDATE: State has been changed
     *
     *  @returns
     *     true if successful, false if failure
     *
     *  @code
     *     C++:
     *         bool
     *         myIntfHandler::postIntfStateCb (NxIntf *intf)
     *         {
     *            if (intf) {
     *               cout << "State change for interface: " << 
     *                        intf->getName() << endl;
     *               cout << "\t new state is: " << (intf->getOperState() == 
     *                           UP) ? "up" : "down" << endl;
     *            }
     *         }
     *
     *     Python:   
     *         def postIntfStateCb(self, intf):
     *             event = intf.getEvent()
     *             if event == nx_sdk_py.UPDATE:
     *                 print "Application State cb "
     *                 print "state changed"
     *             return True
     *
     *  @endcode
     *
     *  @throws
     *      Application exceptions are ignored.
     **/
    virtual bool postIntfStateCb(NxIntf *obj) { return true; }

    /**
     *  Interface layer change notification handler.
     *  @param[in]  obj  Interface object that received the event.
     *                   Object event values:
     *                      UPDATE: Interface layer config changed.
     *  @returns
     *     true if successful, false if failure
     *
     *  @code
     *     C++:
     *        bool 
     *        myIntfHandler::postIntfLayerCb  (NxIntf *intf)
     *        {
     *
     *            cout << "Application layer change: " << 
     *                       intf->getName() << endl;
     *        }
     *
     *     Python:    
     *        def postIntfStateCb(self, intf):
     *            print "Application State cb: " + intf.getName()
     *            return True
     *  @endcode
     *
     *  @throws
     *      Application exceptions are ignored.
     **/
    virtual bool postIntfLayerCb(NxIntf *obj) { return true; }

    /**
     *  Port-channel interface member change notification handler.
     *  @param[in]  obj  Interface object that received the event.
     *                   Object event values:
     *                      ADD:  Address was added
     *                      DELETE: Address was removed
     *  @returns
     *     true if successful, false if failure
     *
     *  @code
     *     C++:
     *        bool myIntfHandler::postIntfPortMemberCb (NxIntf *intf)
     *        {
     *           event_type_e  event;
     *           std::string mbr;
     *           event = intf->getEvent();
     *           cout << "App got port member " << (event == ADD ? 
     *          "ADD":"DELETE") << " event for " << intf->getName() << endl;
     *        }
     *
     *     Python:   
     *        def postIntfPortMemberCb(self, intf):
     *           event = intf.getEvent()
     *           if event == nx_sdk_py.ADD:
     *              state = "ADD"
     *           elif event == nx_sdk_py.DELETE:
     *              state = "DELETE"
     *           print "Got port member " + state + 
     *               " event for interface " + intf.getName()
     *           return True
     *
     *  @endcode
     *
     *  @throws
     *      Application exceptions are ignored.
     **/
    virtual bool postIntfPortMemberCb(NxIntf *obj) { return true; }

    /**
     *  Interface VRF membership change notification handler.
     *  @param[in]  obj  Interface object that received the event.
     *                   Object event values:
     *                      UPDATE: Address was changed
     *  @returns
     *     true if successful, false if failure
     *
     *  @code
     *     C++:
     *         bool  myIntfHandler::postIntfVrfCb (NxIntf *intf)
     *         {
     *             cout << "Application: " << __FUNCTION__ << endl;
     *             cout << "interface: " << intf->getName() << 
     *                  " moved to vrf " <<  intf->getVrf() << endl;
     *         }
     *
     *     Python:   
     *         def postIntfVrfCb(self, intf):
     *             event = intf.getEvent()
     *             print "Application Vrf cb " 
     *             print "intf " + intf.getName() + " moved to vrf " 
     *                     + intf.getVrf();
     *             return True
     *  @endcode
     *
     *  @throws
     *      Application exceptions are ignored.
     **/
    virtual bool postIntfVrfCb(NxIntf *obj) { return true; }

    /**
     *  Interface Vlan membership change notification handler.
     *  @param[in]  obj  Interface object that received the event.
     *                   Object event values:
     *                      UPDATE: Address was changed
     *  @returns
     *     true if successful, false if failure
     *
     *  @code
     *     C++:
     *        bool  myIntfHandler::postIntfVlanCb (NxIntf *intf)
     *        {
     *           cout << "Application: " << __FUNCTION__ << endl;
     *           cout << "interface: " << intf->getName() << 
     *                " new vlan is " << intf->getVlan() << endl;
     *        }
     *
     *     Python:   
     *        def postIntfVlanCb(self, intf):
     *            event = intf.getEvent()
     *            print "Application Vlan cb "
     *            print "intf " + intf.getName() + " new vlan is " + 
     *              intf.getVlan();
     *            return True
     *  @endcode
     *
     *  @throws
     *      Application exceptions are ignored.
     **/
    virtual bool postIntfVlanCb(NxIntf *obj) { return true; }
};

/**
 * @brief  Public interface to NX-SDK interface manager.
 *         
 */

class NxIntfMgr
{
  public:
    virtual ~NxIntfMgr(){};

    /**
     *  Iterate over all open interfaces.  Only interfaces explicitly
     *  opened by the application are returned.  
     *  @param[in]  fromFirst [Optional] Start iteration from beginning or not.
     *  @returns  NxIntf object pointer (NxIntf*)
     *
     *  @code
     *
     *  C++:
     *     intfMgr = sdk->getIntfMgr();
     *     objp = intfMgr->iterateIntf(true);
     *     while (objp) {
     *          do_work(objp);
     *          objp = intfMgr->iterateIntf();
     *      }
     *  Python:
     *     intfMgr = sdk.getIntfMgr()
     *     obj = intfMgr.iterateIntf(True)
     *     while obj is not None:
     *        queryIntf(obj)
     *        obj = intfMgr.iterateIntf()
     *
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual NxIntf *iterateIntf(bool fromFirst = false) = 0;

    /**
     *  Open a system interface based on name.  The interface name is
     *  of the form in "show interface brief", upper/lower case:
     *  e.g.  eth1/1, eth10.1, tunnel1, vlan2, po1, lo1, mgmt0
     *  If already open, returns existing object reference.
     *  Caller is responsible for deleting object when done.  The application
     *  can free all opened objects in one call using closeIntfAll().
     *  @param[in] name   The name of the interface to open.
     *  @returns interface object (NxIntf*) or NULL
     *  @code
     *
     *  C++:
     *     intfMgr = sdk->getIntfMgr();
     *     objp = intfMgr->getIntf("eth1/1");
     *  
     *  Python:
     *     intfMgr = sdk.getIntfMgr()
     *     objp = intfMgr.getIntf("eth1/1")
     *
     *  @endcode
     *
     *  @throws     
     *       ERR_INVALID_USAGE -- 'create' not implemented
     *       ERR_INVALID -- interface type unknown
     **/
    virtual NxIntf *getIntf(std::string name) = 0;

    /**
     *  Open all system interfaces in single call.  Application can
     *  then use iterate api to learn all interfaces.  Caller frees
     *  objects opened.  Existing interface objects are not duplicated.
     *  Currently limited to around 3000 interfaces before memory allocation
     *  begins to fail.
     *
     *  @returns true if successful, false otherwise
     *
     *  @code
     *
     *  C++:
     *     intfMgr = sdk->getIntfMgr();
     *     ret = intfMgr->getIntfAll();
     *     objp = intfMgr->iterateIntf(true);
     *     while (objp) {
     *          do_work(objp);
     *          objp = intfMgr->iterateIntf();
     *      }
     *  Python:
     *     intfMgr = sdk.getIntfMgr()
     *     intfMgr.getIntfAll()  
     *     obj = intfMgr.iterateIntf(True)
     *     while obj is not None:
     *        queryIntf(obj)
     *        obj = intfMgr.iterateIntf()
     *
     *  @endcode
     *
     *  @throws
     *      ERR_INVALID_USAGE
     *      ERR_INVALID
     *      ERR_FAILURE
     **/
    virtual bool getIntfAll() = 0;

    /**
     *  Close and release all interfaces that have been opened by the
     *  application.  This has no effect on interfaces in the system.   
     *
     *  @returns   true if successful, false otherwise.
     *
     *  @code
     *
     *  C++:
     *     intfMgr = sdk->getIntfMgr();
     *     ret = intfMgr->getIntfAll();
     *     objp = intfMgr->iterateIntf(true);
     *     while (objp) {
     *          do_work(objp);
     *          objp = intfMgr->iterateIntf();
     *     }
     *     closeIntfAll();     
     *  
     *  Python:
     *     intfMgr = sdk.getIntfMgr()
     *     intfMgr.getIntfAll()  
     *     obj = intfMgr.iterateIntf(True)
     *     while obj is not None:
     *        queryIntf(obj)
     *        obj = intfMgr.iterateIntf()
     *     intfMgr.closeIntfAll();
     *
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual bool closeIntfAll() = 0;

    /**
     *  Create a system interface based on name.  The interface name is
     *  of the form in "show interface brief", upper/lower case:
     *  e.g.  eth10.1, tunnel1, vlan2, po1, lo1
     *  Caller is responsible for deleting object when done.  The application
     *  can free all opened objects in one call using closeIntfAll().
     *  @param[in]  name  Interface name to add.
     *  @returns interface object (NxIntf*) or NULL
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     * @code
     *
     *  C++:
     *     intfMgr = sdk->getIntfMgr();
     *     objp = intfMgr->addIntf("lo9");
     *  
     *  Python:
     *     intfMgr = sdk.getIntfMgr()
     *     objp = intfMgr.addIntf("lo9")
     *
     *  @endcode
     *
     *  @throws     
     *       ERR_INVALID_USAGE -- invalid interface specified
     *       ERR_INVALID -- interface type unknown
     *       ERR_FAILURE -- internal failure occurred.
     **/
    virtual NxIntf *addIntf(std::string name) = 0;

    /**
     *  Remove a system interface based on name.  The interface name is
     *  of the form in "show interface brief", upper/lower case:
     *  e.g.  eth1/1, eth10.1, tunnel1, vlan2, po1, lo1, mgmt0
     *  
     *  @param[in]  name   The name of the interface to remove.  Only
     *                     logical interfaces can be removed.
     *  @param[in] del_sdk [Optional] Remove the object reference also if open.
     *  @returns  true if successful
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *
     *  C++:
     *     intfMgr = sdk->getIntfMgr();
     *     intfMgr->removeIntf("lo9");
     *  
     *  Python:
     *     intfMgr = sdk.getIntfMgr()
     *     objp = intfMgr.removeIntf("lo9")
     *
     *  @endcode
     *
     *  @throws     
     *       ERR_INVALID_USAGE 
     *       ERR_FAILURE
     *     
     **/
    virtual bool removeIntf(std::string name, bool del_sdk = false) = 0;

    /**
     *  Watch an interface for events.  Input is the interface name or
     *  "all" to watch all interfaces.  The interface does not have to
     *  be open or even exist in the system yet to set a watch. 
     *  @param[in]  name  The name of the interface to watch, or "all"
     *  @returns  true if successful
     *
     *  @code
     *
     *  C++:
     *        intfMgr = sdk->getIntfMgr();
     *        intfMgr->watchIntf("vlan1");  //watch vlan1
     *        intfMgr->watchIntf("all");   //watch all interfaces
     *
     *  Python:
     *        intfMgr = sdk.getIntfMgr()
     *        intfMgr.watchIntf("vlan1") 
     *        intfMgr.watchIntf("all")
     *
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual bool watchIntf(std::string name) = 0;

    /**
     *  Remove interface from watch list by name, or "all" to clear the
     *  watch list.
     *    
     *  @param  name  The interface name, or "all"
     *  @returns true if successful.
     *
     *  @code
     *
     *  C++:
     *        intfMgr = sdk->getIntfMgr();
     *        intfMgr->watchIntf("vlan1");  //watch vlan1
     *        intfMgr->watchIntf("all");   //watch all interfaces
     *        ....
     *        intfMgr->unwatchIntf("vlan1");  //unwatch vlan1
     *        intfMgr->unwatchIntf("all");   //clear watch list
     *  
     *  Python:
     *        intfMgr = sdk.getIntfMgr()
     *        intfMgr.unwatchIntf("vlan1") 
     *        intfMgr.unwatchIntf("all")
     *
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual bool unWatchIntf(std::string name) = 0;

    /**
     *  Install event handlers for interface events.  The application
     *  derives a new handler class from NxIntfMgrEventHandler and 
     *  overrides each event handling method the app is interested in
     *  handling.
     *  
     *  @param[in]  handler  Pointer to handler class that defines
     *                       each application specific handler.
     *  @returns  void
     *
     *  @code
     *
     *  C++:
     *         NxIntfMgrEventHandler *intfHandler = new myIntfHandler();
     *         intfMgr->setIntfHandler(intfHandler);     
     *  Python:
     *         myintfcb = pyIntfHandler()
     *         intfMgr.setIntfHandler(myintfcb)
     *
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual void setIntfHandler(NxIntfMgrHandler *handler) = 0;

    /**
     *  Returns a pointer to the currently installed event handler
     *  object that was previously installed.  
     *
     *  @returns Pointer the the handler object.
     *
     *  @code
     *
     *  C++:
     *       intfHandler = intfMgr->getIntfHandler();
     *  
     *  Python:
     *       myintfcb = intfMgr.getIntfHandler()
     *
     *  @endcode
     *
     *  @throws  None.
     **/
    virtual NxIntfMgrHandler *getIntfHandler() = 0;
};

} // namespace nxos

#endif
