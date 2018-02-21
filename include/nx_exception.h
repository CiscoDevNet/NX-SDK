/** @file nx_exception.h
 *  @brief Abstract Exception Interface for all thrown NX errors.
 *
 *  NX-SDK v1.0.0 supports <br>
 *     - Throws Simple exception of the type in <br>
 *         - C++    - std::logic_err    <br>
 *         - Python - String Object.    <br><br>
 *
 *  NX-SDK v1.5.0 supports    <br>
 *     - Throws Simple exception of the type as version v1.0.0, <br>
 *         - C++    - std::logic_err   <br>
 *         - Python - String Object.   <br><br>
 *     
 *     - Also Throws Advanced exeception of the type NxException across all <br>
 *       languages with more information.  <br>
 *         - C++    - NxException Object   <br>
 *         - Python - NxException Object   <br>
 *
 *  @note By default, simple exception is enabled for an App. 
 *        If the App supports Advanced Exception handling then 
 *        to enable Advanced exception, set advException flag to 
 *        TRUE in getSdkInst Api.
 *
 *  @since NX-SDK v1.5.0

 *  Refer to some example Applications. 
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 */

#ifndef __NX_EXCEPTION_H__
#define __NX_EXCEPTION_H__

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>

namespace nxos {

/**
 *  @brief Abstract Exception Interface for all NXOS thrown errors.
 *         Each API throws an exception of the type NxException 
 *         when advException is enabled for an App to handle the 
 *         failures gracefully. Refer to nxsdk applications for the 
 *         usage of NxException.
 *
 *  @since NX-SDK v1.5.0
 *
 *  1) To Enable advanced exception, 
 *  @code
 *  Usage:
 *
 *  C++:
 *     sdk = nxos::NxSdk::getSdkInst(argc, argv, true);
 *
 *  Python:
 *     import nx_sdk_py
 *     sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv, True)
 *          
 *  @endcode 
 * 
 *  2) To catch the exceptions,
 *  @code
 *  Usage:
 *
 *  C++:
 *     try {
 *         ribMgr->watchL3Route(protocol, tag, vrf_name, af);
 *     } catch (std::logic_error const& err) {
 *         clicmd.printConsole("Failed with Err (%s)", err.what());    
 *     } catch (const NxException *e) {
 *         clicmd.printConsole("Module: %s, API: %s, Severity: %s, 
 *                             ErrCode %d(%s)", e->getErrModule(), 
 *                             e->getErrApi(), 
 *                             NxTrace::getPrioStr(e->getErrSeverity()), 
 *                             e->getErrCode(), e->what()));
 *     }
 *
 *  Python:
 *     try:           
 *        ribMgr.watchL3Route(protocol, tag, vrf_name, af)       
 *     except RuntimeError,e:                     
 *        # If Simple Exception is set (since v1.0.0
 *        clicmd.printConsole("Error %s" % str(e))
 *     except nx_sdk_py.NxException as e:           
 *        #If Advanced Exception is set(since v1.5.0))
 *        clicmd.printConsole("Module: %s, API: %s, Severity: %s, \
 *                            ErrCode %d(%s)" % (e.getErrModule(), \
 *                            e.getErrApi(), \
 *                            nx_sdk_py.NxTrace.getPrioStr(e.getErrSeverity()), \
 *                            e.getErrCode(), e.what()))  
 *  @endcode   
 **/
class NxException {
public:

    /**
     * [Optional] Destructor
     *
     * To Delete the Exception instance gracefully.
     * Automatically called when the App Exits. 
     **/
    virtual ~NxException() {}

    /**
     * To get the Error Reason string for the thrown exception. 
     *
     * @returns Error reason as string
     **/
    virtual const char * what()=0;

    /**
     * To get the Error code ID for the thrown exception.
     *
     * @returns Error Code of type err_type_e 
     **/
    virtual unsigned long getErrCode()=0;

    /**
     * To get the Module/Class Name that threw the Exception.
     *
     * @returns Error Module Name as string 
     **/
    virtual const char * getErrModule()=0;

    /**
     * To get the Severity of the Exception.
     * 
     * @returns Error Severity of type NxTrace::Priority
     **/
    virtual unsigned short getErrSeverity()=0;

    /**
     * To get the API name in the Module/Class that threw 
     * the Exception.
     *
     * @returns Error Api Name as string 
     **/
    virtual const char * getErrApi()=0;
};

/// Supported Error types
typedef enum {
   /// Not used Right.
   ERR_INVALID_USAGE = 1,
   
   /// Not a valid scenario.
   ERR_INVALID       = 2,

   /// NULL Pointer Parameter passed
   ERR_NULL_PTR      = 3,

   /// Does not exist
   ERR_NOT_FOUND     = 4,

   /// Empty case
   ERR_EMPTY         = 5,

   /// Reached Max Limit
   ERR_MAX_LIMIT     = 6,

   /// Failure case
   ERR_FAILURE       = 7,
} err_type_e;

}

#endif //__NX_EXCEPTION_H__
