/**
 * @file  nx_dme.h
 * @brief Abstract SDK interface to Nexus(NXOS) DME 
 *
 * Provides abstractions for NXOS DME functionalites such as getting
 * dme objects, reading/setting properties, and dme event notification.
 *
 *
 *  Copyright (c) 2018 by cisco Systems, Inc.
 *  All rights reserved.
 *
 *  @since  NXSDK V1.7.5
 *  @author nxos-sdk@cisco.com
 */
#ifndef __NX_DME_H__
#define __NX_DME_H__

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include "types/nx_common.h"
#include "nx_exception.h"

using namespace std;

namespace nxos
{

/**
 *  @brief  Public interface for NXSDK DME objects.  These objects are
 *          used by applications to read or update the DME object store
 *          in NXOS, and also for DME event processing. 
 */
class NxDme
{

public:
    virtual ~NxDme() {}

    /**
     *  Get the DN string for the object
     *  @returns
     *         DN string
     *  @code
     *   C++:
     *        cout << "NxDme obj = " << objp->getDn() << endl;
     *   Python:
     *        print "%s" % obj.getDn()
     *  @endcode
     *
     *  @throws
     *        None.
     **/
    virtual std::string getDn() = 0;

    /**
     *  Set an object's property to specified value.  The change
     *  becomes active on this object's next NxDme::commit().
     *  @param[in] sInPropName  Name of the property to set
     *  @param[in] sInValue    Value of the property
     *  @param[out] err    Error return value
     *  @returns
     *      void, err_type_e set in argument
     *  @code
     *   C++:
     *      int err;
     *      objp->setProperty("property1", "value", &err);
     *   Python:
     *       errp = nx_sdk_py.new_intp();
     *       obj.setProperty(cmds[1], cmds[2], errp)
     *       err = nx_sdk_py.intp_value(errp);
     *       nx_sdk_py.delete_intp(errp);
     *
     *       if err == nx_sdk_py.ERR_ESUCCESS:
     *
     *  @endcode
     *
     *  @throws
     *      ERR_INVALID_USAGE
     **/
    virtual void setProperty(const std::string &sInPropName,
                             const std::string &sInValue,
                             int               *err) = 0;

    /**
     *  Read an object property.  By default reads from the
     *  object cache.
     *
     *  @param[in]  sInPropName  Name of property to get.
     *  @param[out] err    Error return
     *  @returns
     *     property string,  err_type_e set in argument
     *  @code
     *   C++:
     *     string  mode;
     *     int     err;
     *     mode =  _dmeobj->getProperty("mode", &err);
     *   Python:
     *      errp = nx_sdk_py.new_intp();
     *      value = obj.getProperty(cmds[1], errp)
     *      err = nx_sdk_py.intp_value(errp);
     *      nx_sdk_py.delete_intp(errp);
     *
     *      if err == nx_sdk_py.ERR_ESUCCESS:
     *
     *  @endcode
     *
     *  @throws
     *     ERR_INVALID_USAGE
     **/
    virtual std::string getProperty(const std::string &sInPropName,
                                    int               *err) = 0;

    /**
     *  Commit the pending changes that were made to this object.  Only
     *  the changes to this object are committed.  The completion of the
     *  commit does not imply that any child objects that may be created
     *  as a result of config change are also created.
     *  @param[out]   Error value
     *
     *  @returns
     *      error string,  err_type_e set in argument
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *   C++:
     *      int  err;
     *      std::string  err_str;
     *
     *      objp->setProperty("property1", "value1", &err);
     *      objp->setProperty("property2", "value2", &err);
     *      err_str =  objp->commit(&err);
     *      cout << "commit status: " << err << " status: " << err_str;
     *   Python:
     *      errp = nx_sdk_py.new_intp();
     *      err_str = obj.commit(errp)
     *      err = nx_sdk_py.intp_value(errp);
     *      nx_sdk_py.delete_intp(errp);
     *
     *      if err == nx_sdk_py.ERR_ESUCCESS:
     *  @endcode
     *
     *  @throws
     *      ERR_INVALID_USAGE
     **/
    virtual std::string commit(int *err) = 0;

    /**
     *  Get the object data as a JSON string. 
     *
     *  @returns
     *      string containing json string
     *  @code
     *   C++:
     *      NxDme *dme = dmeP->getDmeObj("some/dn/path");
     *      string  jdata = dme->getDataJson(); //read from obj
     *              json_parse(jdata); //etc.
     *   Python:
     *       data = obj.getDataJson()
     *  @endcode
     *
     *  @throws
     *      None.
     **/
    virtual std::string getDataJson() = 0;

    /**
     *  Iteratively return all of the property names for the object.
     *  @param[in]  fromFirst: [Optional]  Start iteration from beginning or not.
     *  @returns
     *      string representing the next property name
     *  @code
     *   C++:
     *      string  name;
     *      NxDme   *dme = dmeMgr->getDmeObj("dn/mo/path");
     *      name = dme->iterateProperties(true);
     *          while (!name.empty()) {
     *              cout << name << endl;
     *              name = dme->iterateProperties();
     *          }
     *   Python:
     *      data = obj.iterateProperties(True)
     *      while len(data):
     *          errp = nx_sdk_py.new_intp();
     *          value = obj.getProperty(data, errp)
     *          err = nx_sdk_py.intp_value(errp);
     *          nx_sdk_py.delete_intp(errp);
     *
     *          print "%s : %s" % (data,value)
     *          data = obj.iterateProperties()
     *  @endcode
     *
     *  @throws
     *      None.
     **/
    virtual std::string iterateProperties(bool fromFirst = false) = 0;

    /**
     *  Get the event code for this object, which was set if this
     *  object is coming from an event handler.
     *
     *  @returns
     *      nxos::event_type_e value indicating the source of the event
     *  @code
     *   C++:
     *      void postdmeHandlerCb (NxDme *objp)
     *      {
     *          string value;
     *
     *          cout << "application callback called" << endl;
     *          cout << "NxDme event = " << event_str(objp->getEvent()) << endl;
     *      }
     *   Python:
     *      class pyDmeHandler(nx_sdk_py.NxDmeHandler):
     *
     *          def postDmeHandlerCb(self, obj):
     *              print "pyDmeHandler: application callback"
     *              print "dn = %s" % obj.getDn()
     *              print "event = %d" % obj.getEvent()
     *
     *  @endcode
     *
     *  @throws
     *      None.
     **/
    virtual nxos::event_type_e getEvent() = 0;

    /**
     *  Check if the specified property is in the list of properties
     *  that changed in the current event object.  This api is only
     *  valid for objects returned to the event callback with an
     *  event type of UPDATE.
     *
     *  @param[in]  property  Name of property to test for change.
     *  @returns
     *      true if property was modified in the event
     *  @code
     *   C++:
     *       void postDmeHandlerCb (NxDme *objp)
     *       {
     *          if (objp->getEvent() == UPDATE) {
     *            if (objp->getEventIsPropertyChanged("adminSt")) {
     *               cout << " Property changed: " << "adminSt" << endl;
     *               cout << " new value is: " <<
     *                 objp->getProperty("adminSt", &err) << endl;;
     *           }
     *
     *        }
     *   Python:
     *       class pyDmeHandler(nx_sdk_py.NxDmeMgrHandler):
     *
     *           def postDmeHandlerCb(self, obj):
     *              if obj.getEvent() == nx_sdy_py.UPDATE:
     *                  if obj.getEventIsPropertyChanged("adminSt"):
     *                      print "adminSt changed"
     *  @endcode
     *
     *  @throws
     *       ERR_INVALID_USAGE, ERR_INVALID
     **/
    virtual bool getEventIsPropertyChanged(const std::string &sInPropName) = 0;

    /**
     *  Iterate through the list of properties that changed in the
     *  current event object.  This is only applicable to UPDATE
     *  events.  The property name can then be used in getProperty()
     *  to retrieve the new property.
     *
     *  @param[in]  fromFirst: [Optional] Start iteration from beginning or not.
     *  @returns
     *     The property names which were modified in the event.
     *  @code
     *   C++:
     *        void postDmeHandlerCb (NxDme *objp)
     *        {
     *           std::string value;
     *
     *           if (objp->getEvent() == UPDATE) {
     *             value = objp->iterateEventUpdatedProperties(true);
     *             while (value.length()) {
     *               cout << "iterateEvent prop: " << value << endl;
     *               value = objp->iterateEventUpdatedProperties();
     *             }
     *           }
     *   Python:
     *         class pyDmeHandler(nx_sdk_py.NxDmeMgrHandler):
     *
     *           def postDmeHandlerCb(self, obj):
     *              if obj.getEvent() == nx_sdy_py.UPDATE:
     *                  value = obj.iterateEventUpdatedProperties(True)
     *                  while len(value) :
     *                     print "Property changed: %s" % value
     *                     value = obj.iterateEventUpdatedProperties()
     *  @endcode
     *
     *  @throws
     *      ERR_INVALID_USAGE, ERR_INVALID
     **/
    virtual std::string iterateEventUpdatedProperties(bool fromFirst = false) = 0;

    /**
     *  Test equality of dme objects.  Two objects are equal if they
     *  refer to the same DN path and have the same property values.
     *  @param[in] dme_obj   Object to compare against.
     *  @returns
     *     true if equivalence relationship evaluates to true.
     *
     *  @code
     *      C++:
     *          NxDme *dme1 = dme_v.at(idx1);
     *          NxDme *dme2 = dme_v.at(idx2);
     *          if (*dme1 == *dme2)
     *              cout << "Equal" << endl;
     *      Python:
     *          if obj1 == obj2:
     *               print "Equal"
     *  @endcode
     *
     *  @throws
     *      None.
     **/
    virtual bool operator==(NxDme const &dme_obj) const = 0;

    /**
     *  Test inequality of dme objects.  Two objects are not equal if they
     *  refer to different dn paths or have different property values.
     *  @param[in] dme_obj   Object to compare against.
     *  @returns
     *     true if inequality relationship evaluates to true.
     *
     *  @code
     *      C++:
     *          NxDme *dme1 = dme_v.at(idx1);
     *          NxDme *dme2 = dme_v.at(idx2);
     *          if (*dme1 != *dme2)
     *              cout << "Not Equal" << endl;
     *      Python:
     *          if obj1 != obj2:
     *              print "Not Equal"
     *
     *  @endcode
     *
     *  @throws
     *      None.
     **/
    virtual bool operator!=(NxDme const &dme_obj) const = 0;

    /**
     *  Test equality of dme objects.  Can be used in languages
     *  which do not support overloading "==".
     *  @param[in] dme_obj   Object to compare against.
     *  @returns
     *     true if equivalence relationship evaluates to true.
     *
     *  @code
     *     C++:
     *          NxDme *dme1 = dme_v.at(idx1);
     *          NxDme *dme2 = dme_v.at(idx2);
     *          if (dme1->equal(*dme2))
     *              cout << "Equal" << endl;
     *      Python:
     *          if obj1.equal(obj2):
     *              print "Equal"
     *
     *  @endcode
     *
     *  @throws
     *      None.
     **/
    virtual bool equal(NxDme const &dme_obj) const = 0;


    /**
     *  Retrieve the number of direct children of this object.  This api
     *  will return valid count only if this dme object was opened with
     *  'getChildren = true' in call to getDmeObj().
     *  
     *  
     *  @returns
     *    The number of children immediately under this object.
     *
     *  @code
     *    C++:
     *       cout << "Number of children: " << dme->getChildrenDmeObjCount() << endl;
     *    Python:
     *       print  "Number of children: %d" % dme.getChildrenDmeObjCount()
     *  @endcode
     *
     *  @throws
     *     ERR_ENOMEM
     **/
    virtual int  getChildrenDmeObjCount () = 0;

    /**
     *  Iterate through the children of this dme object.  The initial
     *  call should set fromFirst=true, then subsequent calls use
     *  the default value of false.  To reset the iteration to the
     *  beginning, call again with fromFirst=true.  When there are no
     *  more DNs to retrieve, the function returns empty string "".
     *  This api will return valid data only if this dme object was opened with
     *  'getChildren = true' in call to getDmeObj().
     *
     *  @param[in]  Start at the beginning or not.
     *  @returns
     *     DN string of next child or ""
     *
     *  @code
     *    C++:
     *      std::string   child_dn;
     *      int  err;
     *                
     *      cout << "Number of children: " << 
     *               obj->getChildrenDmeObjCount() << endl;
     *
     *      child_dn = obj->iterateChildrenDmeDn(true);
     *      while (child_dn.length()) {
     *         cout << "child DN: " << child_dn << endl;
     *         child_dn = obj->iterateChildrenDmeDn(false);
     *      }
     *    Python:
     *      child_dn = obj.iterateChildrenDmeDn(True)
     *      while child_dn:
     *          print "child DN: %s"  % child_dn
     *          child_dn = obj.iterateChildrenDmeDn()
     *  @endcode
     *
     *  @throws
     *     ERR_INVALID_USAGE, ERR_FAILURE
     **/
    virtual std::string iterateChildrenDmeDn (bool fromFirst = false) = 0;
    /**
     *  Test if this object has been modified 
     *
     *  @returns
     *     true if this object has been modified, false if not
     *
     *  @code
     *    C++:
     *       if (dme_obj->isModified()) 
     *          cout << "Local object has been modified" << endl;
     *
     *    Python:
     *          if dme_obj.isModified():
     *             print "Local object has been modified"
     *  @endcode
     *
     *  @throws   None.
     *      
     **/
    virtual bool  isModified () = 0;

    /**
     *  Retrieve the value of a changed property of an object
     *  passed to event callback.  This is only valid for an
     *  event object.
     *  @param[in]  The property name
     *  @param[out]  Error value, ERR_ESUCCESS or ERR_NOT_FOUND
     *  @returns
     *     The value of the property, if present.
     *
     *  @code
     *     C++:
     *        prop_name = objp->iterateEventUpdatedProperties(true);
     *        while (prop_name.length()) {
     *           prop_value = objp->getEventPropertyValue(prop_name, &error);
     *           cout << "iterateEvent prop: " << prop_name << endl;
     *           cout << "             value: " << prop_value << endl;
     *           prop_name = objp->iterateEventUpdatedProperties();
     *        }
     *     Python:
     *        class pyDmeHandler(nx_sdk_py.NxDmeMgrHandler):
     *
     *           def postDmeHandlerCb(self, obj):
     *              if obj.getEvent() == nx_sdy_py.UPDATE:
     *                  prop = obj.iterateEventUpdatedProperties(True)
     *                  while len(prop) :
     *                     errp = nx_sdk_py.new_intp()
     *                     value = obj.getEventPropertyValue(prop, errp);
     *                     print "Property changed: %s:%s" % (prop,value)
     *                     prop = obj.iterateEventUpdatedProperties()
     *                     nx_sdk_py.delete_intp(errp)
     *  @endcode
     *
     *  @throws  ERR_INVALID_USAGE
     *
     **/
    virtual std::string getEventPropertyValue(const std::string& sInPropName,
                                              int *err) = 0;
};


/**
 *  @brief  Public interface for NXSDK DME event notification.  
 *
 *  Applications will override this class and provide a specific 
 *  implementation of the callback methods defined here.  
 *  An object will be passed to it which
 *  is the object that generated the event.  This object is owned by
 *  the SDK and should not be stored nor deleted by the application.
 *  DME events generated from NXOS are delivered to application
 *  callbacks through NXSDK dme objects
 * Steps to register for dme events:
 *  1)  Open NXSDK dme mgr object using getDmeMgr()
 *  2)  Define application callbacks by re-defining NxDmeMgrHandler class
 *  3)  Install application handlers using setDmeHandler()
 *  4)  call NxSdk::startEventLoop()
 */
class NxDmeMgrHandler
{
public:
    virtual ~NxDmeMgrHandler() {}
    /**
     *  Application callback for receiving DME events.  The object
     *  passed contains the object data which the event occurred.
     *
     *  @param[in] objp   The object which generated the event.
     *  @returns
     *     void
     *  @code
     *     C++:
     *       class myNxDmeHandler : public NxDmeHandler {
     *       public:
     *
     *         void postDmeHandlerCb (NxDme *objp)
     *         {
     *           std::string value;
     *
     *           cout << __FUNCTION__ << "(): application callback called" << endl;
     *           cout << "NxDme obj.dn <" << objp->getDn() << ">" << endl;
     *           cout << "NxDme event = " << objp->getEvent() << endl;
     *
     *         }
     *       };
     *
     *
     *     Python:
     *       class pyDmeHandler(nx_sdk_py.NxDmeHandler):
     *
     *          def postDmeHandlerCb(self, obj):
     *              print "pyDmeHandler: application callback"
     *              print "dn = %s" % obj.getDn()
     *              print "event = %d" % obj.getEvent()
     *
     *  @endcode
     *
     *  @throws
     *     Applications should not throw exceptions.
     **/
    virtual void postDmeHandlerCb(NxDme *objp)
    {
        return;
    }
    /**
     *  Callback for notifying that the end of a bulk object
     *  download has completed.  This is only called if the
     *  application called a watch api with download enabled.
     *  The dn of the top-level object being downloaded is passed
     *  as an argument.
     *  @param[in]  dn   The dn for which the download has completed.
     *  @returns
     *    void
     *  @code
     *     C++
     *       class myNxDmeHandler : public NxDmeHandler {
     *       public:
     *
     *         void postDmeHandlerDownloadDoneCb (std::string dn)
     *         {
     *
     *           cout << __FUNCTION__ << "(): application callback called"
     *              "  for object: " << dn  << endl;
     *
     *         }
     *       };
     *
     *     Python:
     *       class pyDmeHandler(nx_sdk_py.NxDmeHandler):
     *
     *          def postDmeHandlerCb(self, dn):
     *              print "pyDmeHandler: application callback"
     *              print "dn = %s" % dn
     *
     *  @endcode
     *
     *  @throws
     *      Applications should not throw exceptions.
     **/
    virtual void postDmeHandlerDownloadDoneCb(const std::string &dn)
    {
        return;
    }
};

/**
 *  @brief  Public DME Manager interface to gain access to NXOS DME 
 *          functionality such as reading DME objects, getting/setting
 *          object properties, and capturing DME events.
 */
class NxDmeMgr
{
public:
    virtual ~NxDmeMgr() {}

    /**
     *  Opens DME object path and return an object for it, which the
     *  caller owns and must delete.
     *  @param[in]  Dn  The DN string which specifies the DME object.
     *  @param[in]  getChildren  Read child information for this object.
     *                           Required if using any children related apis.
     *           
     *  @returns
     *     Pointer to object or NULL.
     *  @code
     *  C++:
     *       NxDmeMgr       *dmeP =  sdk->getDmeMgr();
     *       NxDme *dme = dmeP->getDmeObj("dn/path/mo");
     *  Python:
     *       dmeobj = dmeP.getDmeObj("dn/path/mo")
     *  Go:
     *       package main
     *       import (
     *          "nx_sdk_go"
     *          "os"
     *       )
     *
     *       var dmeMgr  nx_sdk_go.NxDmeMgr
     *       var dmeObj  nx_sdk_go.NxDme
     *
     *       sdk_obj := nx_sdk_go.NxSdkGetSdkInst(len(os.Args), os.Args, true)
     *       if sdk_obj.Swigcptr() == 0 {
     *          os.Exit(1)
     *       }
     *       dmeMgr = sdk_obj.GetDmeMgr()
     *       if dmeMgr.Swigcptr() == 0 {
     *          os.Exit(1)
     *       }
     *       dmeobj = dmeMgr.GetDmeObj("sys/ch")
     *  @endcode
     *
     *  @throws
     *      ERR_INVALID_USAGE
     *      ERR_FAILURE
     *      ERR_INVALID
     **/
    virtual NxDme *getDmeObj(const std::string &Dn,
	                     bool getChildren=false) = 0;

    /**
     *  Create a new DME object that will be added to the running
     *  configuration.
     *  Reference :
     *  https://developer.cisco.com/site/nxapi-dme-model-reference-api/?version=9.2(1)
     *  @param[in]  sInDn    The DN path for the object to be added.
     *  @param[out] error    The error value.
     *
     *  @returns
     *     pointer to created NxDme object
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *
     *  @code
     *  C++:
     *      int   err;
     *      NxDme *obj;
     *      NxDmeMgr       *dmeP =  sdk->getDmeMgr();
     *      
     *      obj = dmeP->addDmeObj("path/to/object", &err);
     *  Python:
     *      obj = dmeP.addDmeObj("sys/intf/lb-[lo1]", err)
     *      if err == nx_sdk_py.ERR_ESUCCESS:
     *
     *  @endcode
     *
     *  @throws
     *      ERR_INVALID_USAGE
     **/
    virtual NxDme*  addDmeObj(const std::string &sInDn,
                              int     *error) = 0;


    /**
     *  Remove an objet from the running DME configuration.
     *  @param[in]  sInDn  The DN path of the object to delete.
     *
     *  @returns
     *    err_type_e  value
     *
     * @details
     *     Write API - NX-SDK Applications can use this API only if security profile with permit (or) throttle is 
     *                 enabled for that application. Using this API without the appropriate security profiles will
     *                 throw an exception. Refer to readmes/security_profiles.md for more info in NX-SDK Git repo.
     *                 
     *  @code
     *  C++:
     *      int err;
     *      err = dmeP->delDmeObj("path/to/object");
     *      if (err == ERR_ESUCCESS) ...
     *  Python:
     *      err = dmeP.delDmeObj("path/to/object")
     *      if err == nx_sdk_py.ERR_ESUCCESS:
     *           ...
     *  @endcode
     *
     *  @throws
     *        ERR_INVALID_USAGE, ERR_FAILURE
     **/
    virtual int delDmeObj(const std::string &sInDn) = 0;

    /**
     *  Retrieve the object data as a json string for the
     *  object at the specified dn path.
     *  @param[in] sInDn   The DN path of the object.
     *  @param[out] error  The resulting error value.
     *  @returns
     *     Json string, error code set in argument
     *
     *  @code
     *  C++:
     *        int err;
     *        string data_string = NULL;
     *
     *        data_string = dmeP->getMOJson("dn/path", &err);
     *        cout << "Object : " << data_string;
     *  Python:
     *       errp = nx_sdk_py.new_intp();
     *       data = dmeP.getMoJson("dn/path", errp)
     *       status = nx_sdk_py.intp_value(errp);
     *       nx_sdk_py.delete_intp(errp);
     *
     *       if status == nx_sdk_py.ERR_ESUCCESS:
     *            # use 'data'
     *  @endcode
     *
     *
     *  @throws
     *     ERR_INVALID_USAGE
     **/
    virtual std::string getMoJson(const std::string &sInDn,
                                  int               *error) = 0;

    /**
     *  Retrieve the object data in JSON format for the object at the
     *  specified dn path along with its immediate children
     *  @param[in] sInDn  The DN path of the parent dme object.
     *  @param[out] error  The resulting error value.
     *  @returns
     *     Json string, error code set in argument
     *  @code
     *  C++:
     *     int err;
     *     string data_string = NULL;
     *
     *     data_string = dmeP->getMOJson("dn/path", &err);
     *     cout << "Object + Children: " << data_string;
     *  Python:
     *     errp = nx_sdk_py.new_intp();
     *     data = dmeP.getChildrenMoJson("dn/path", errp)
     *     status = nx_sdk_py.intp_value(errp);
     *     nx_sdk_py.delete_intp(errp);
     *
     *     if status == nx_sdk_py.ERR_ESUCCESS:
     *          # use 'data'
     *  @endcode
     *
     *
     *  @throws
     *      ERR_INVALID_USAGE
     **/
    virtual std::string getChildrenMoJson(const std::string &sInDn,
                                          int               *error) = 0;

    /**
     *  Test if the specified dn path exists in the current config.
     *  @param[in] sInDn  The DN path of the object to test.
     *  @returns
     *     true if object exists, false otherwise
     *  @code
     *    C++:
     *      if (dmeP->existsMO("sys/fm/nxapi")) {
     *          dmeP->setProperty("sys/fm/nxapi", "adminSt", "enabled");
     *          dmeP->commit();
     *      }
     *    Python:
     *      if not dmeP.existsMo("some/dn/path"):
     *          print "Object does not exist"
     *
     *  @endcode
     *
     *
     *  @throws
     *     ERR_INVALID_USAGE
     **/
    virtual bool existsDmeObj(const std::string &sInDn) = 0;

    /**
     *  Watch the given DME object dn for events, filtered by pattern. The
     *  pattern is a regular expression conforming to POSIX ERE syntax.
     *  The default pattern is empty, which means to match everything
     *  generated for this dn.  Calling with an empty pattern after
     *  previously installing non-empty patterns results in clear all
     *  other patterns (resetting to default).  Setting a watch pattern
     *  if the current (only) pattern is empty results in effectively
     *  removing the default empty pattern and matching the installed
     *  pattern(s).  Installing multiple non-empty patterns is possible,
     *  and each pattern is matched in the order installed.  If any
     *  pattern matches, the event is delivered to callback.  It is not
     *  required that all patterns match.  The 'download' flag controls
     *  if all objects in the subtree are returned via the application
     *  callback.  These objects are also filtered by the patterns.
     *
     *  @param[in]  sInDn  The DN path of the root object to watch
     *  @param[in]  pattern [Optional] Regular expression string used for filtering.
     *  @param[in]  download [Optional] Specify to do object download.
     *  @returns
     *    true if operation succeeded
     *  @code
     *  C++:
     *       string ere_pattern = "^vni-*";
     *       dmeP->watch("sys/eps/epId-1/nws", ere_pattern);
     *  Python:
     *      dmeP.watch("sys/eps/spId-1/nws", "^vni-*")
     *  @endcode
     *
     *
     *  @throws
     *     ERR_INVALID_USAGE
     *     ERR_FAILURE
     **/
    virtual bool watch(const std::string &sInDn,
                       const std::string &pattern = "",
                       bool              download = true) = 0;

    /**
     *  Stop watching a DME object or remove an existing pattern.  If
     *  called with non-empty pattern, remove the pattern.  Otherwise,
     *  remove the watch for this dn completely.  If unwatch is called
     *  for the only existing pattern, this results in the default pattern
     *  being installed and used.  Otherwise, any existing patterns remain
     *  in effect.  If pattern is specified as "" or default value is
     *  used, this clears all patterns and removes the watch completely.
     *  @param[in] sInDn  The DN string of root object to unwatch.
     *  @param[in] pattern [Optional] Regular expression pattern to remove.
     *  @returns
     *     true if operation successful
     *  @code
     *    C++:
     *         string ere_pattern = "^vni-*";
     *           // remove ere_pattern from this watched dn
     *         dmeP->unwatch("sys/eps/epId-1/nws", ere_pattern);
     *           // remove watch on this dn completely
     *         dmeP->unwatch("sys/eps/epId-1/nws");
     *    Python:
     *          #clear single pattern from watched object
     *         dmeP.unwatch("sys/eps/epId-1/nws", "^vni-*")
     *          #clear watch completely
     *         dmeP.unwatch("sys/eps/epId-1/nws")
     *  @endcode
     *
     *
     *  @throws
     *      ERR_INVALID_USAGE
     **/
    virtual bool unwatch(const std::string &sInDn,
                         const std::string &pattern = "") = 0;

    /**
     *  Install a user defined event callback handler that will be
     *  invoked if a watched dn has activity.  If a filter was also
     *  given for the dn, only events that match the filter patterns
     *  will result in calling the callback method.
     *  @param[in] handler  The DME event handler to be used.
     *  @returns
     *    true if operation successful
     *  @code
     *    C++:
     *
     *      class myNxDmeHandler : public NxDmeHandler {
     *      public:
     *
     *        void postDmeHandlerCb (NxDme *objp)
     *        {
     *          string value;
     *
     *          cout << __FUNCTION__ << "(): application callback called"
     *                       << endl;
     *          cout << "NxDme obj.dn <" << objp->getDn() << ">" << endl;
     *          cout << "NxDme event = " << event_str(objp->getEvent())
     *                      << endl;
     *        }
     *
     *      };
     *
     *      NxDmeHandler *dmeHandler = new myNxDmeHandler();
     *      dmeP->setDmeHandler(dmeHandler);
     *
     *  Python:
     *
     *     class pyDmeHandler(nx_sdk_py.NxDmeHandler):
     *
     *         def postDmeHandlerCb(self, obj):
     *            print "pyDmeHandler: application callback"
     *            print "dn = %s" % obj.getDn()
     *            print "event = %s" % getEventStr(obj.getEvent())
     *            dme_object_mode(obj, False)
     *
     *      myDmeHandler = pyDmeHandler()
     *      dmeP.setDmeHandler(myDmeHandler)
     *  @endcode
     *
     *
     *  @throws
     *      ERR_INVALID
     *      ERR_FAILURE
     **/
    virtual bool setDmeHandler(NxDmeMgrHandler *handler) = 0;

    /**
     *  Return the currently installed handler, if any.
     *  @returns
     *    pointer to currently installed event handler
     *  @code
     *    C++:
     *       NxDmeHandler *dmeHandler = dmeP->getDmeHandler();
     *    Python:
     *       handler = dmeP.getDmeHandler()
     *  @endcode
     *
     *
     *  @throws
     *      None.
     **/
    virtual NxDmeMgrHandler *getDmeHandler() = 0;
};

} // namespace nxos

#endif //__NX_DME_H__
