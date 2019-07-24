/** @file nx_exception.h
 *  @brief Abstract Exception Interface for all thrown NX errors.
 *
 *  NXSDK V1.0 supports
 *     - Throws Simple exception of the type in
 *         C++    - std::logic_err
 *         Python - String Object.
 *
 *  NXSDK V1.5 supports
 *     - Throws Simple exception of the type as version V1.0,
 *         C++    - std::logic_err
 *         Python - String Object.
 *     
 *     - Also Throws Advanced exeception of the type NxException across all 
 *       languages with more information.  
 *         C++    - NxException Object
 *         Python - NxException Object
 *
 *  @note By default, simple exception is enabled for an App. 
 *        If the App supports Advanced Exception handling then 
 *        to enable Advanced exception, set advException flag to 
 *        TRUE in getSdkInst Api.
 *
 *  @since NXSDK V1.5
 *
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
 *         when advException is enabled for an App in case of a
 *         failure for the Apps to handle the failures gracefully. 
 *         Refer to sample applications for the usage of NxException.
 */
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
   // No error
   ERR_ESUCCESS       = 0, 
 
   /// Not used Right.
   ERR_INVALID_USAGE,
    
   /// Not a valid scenario.
   ERR_INVALID,
   
   /// NULL Pointer Parameter passed
   ERR_NULL_PTR,
   
   /// Does not exist
   ERR_NOT_FOUND,

   /// Already exists
   ERR_EXISTS,
   
   /// Empty case
   ERR_EMPTY,
   
   /// Reached Max Limit
   ERR_MAX_LIMIT,
   
   /// Failure case
   ERR_FAILURE,
   
   /// No error, but requires further processing
   ERR_ESUCCESS_MORE,
   
   /// Signal interrupt received
   ERR_SIGINT,
   
   /// Not enough memory to complete
   ERR_ENOMEM,
   
   /// Bad fd found
   ERR_BADFD, 
 
   /// Unknown Error  
   ERR_UNKNOWN,

   /// Remote operations performed when the session is Down
   ERR_REMOTE_DOWN,

   /// Using old SDK objects prior to remote session down
   ERR_REMOTE_SDK_OBJS_OBSOLETE

} err_type_e;
 
}

#endif //__NX_EXCEPTION_H__
