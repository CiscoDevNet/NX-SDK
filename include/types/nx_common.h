/** @file nx_common.h
 *  @brief Provides necessary common types for  using NXSDK
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 *
 *
 * Copyright (c) 2018 by cisco Systems, Inc.
 * All rights reserved.
 *

 */

#ifndef __NX_COMMON_TYPES_H__
#define __NX_COMMON_TYPES_H__

namespace nxos {

// Supported Record types
typedef enum {
   /// Record Type is TEXT
   R_TEXT = 0,
 
   /// Record Type is JSON   
   R_JSON,

   /// Record Type is XML
   R_XML,

   /// Record Type is MAX_TYPE
   R_MAX_TYPE
} record_type_e;

// Supported Action types
typedef enum {
   /// Not an Event
   NO_EVENT = 0,

   /// Event is ADD
   ADD = 1,

   /// Event is DELETE
   DELETE,

   /// Event is UPDATE
   UPDATE,

   /// Event is DOWNLOAD
   DOWNLOAD, 

   /// Event is DOWNLOAD_DONE
   DOWNLOAD_DONE,

   /// MAX Action Types
   E_MAX_TYPE
} event_type_e;

// Supported State types
typedef enum {
    UNKNOWN = -1,
   /// State is DOWN
   DOWN = 0,

   /// State is UP   
   UP,

   /// Record Type is MAX_TYPE
   S_MAX_TYPE
} state_type_e;

// Supported Encap types
typedef enum {
   /// State is DOWN
   NONE = 0,

   /// State is UP   
   VXLAN,

   /// Record Type is MAX_TYPE
   ENCAP_MAX_TYPE
} encap_type_e;

typedef enum {
   AF_START = 0,

   /// Address Family type - IPV4
   AF_IPV4 = 0,

   /// Address Family type - IPV6
   AF_IPV6 = 1,

   /// Max types supported/
   MAX_AF
} af_e;

/**
 *  Supported Priority Types.
 *  Usage:
 *    setAppPriority() - To limit the use of CPU per application.
 *                       So that the Apps dont run away with the CPU. 
 **/
typedef enum {
   /// Low Priority.
   LOW_PRIO = 1,

   /// Medium Priority Application.
   MED_PRIO,

   /// High Priority Application.
   HIGH_PRIO,

   /// No Priority.
   NO_PRIO
} prio_e;


typedef enum {
    NX_TYPE_UNKNOWN,
    NX_TYPE_ETH,
    NX_TYPE_SVI,
    NX_TYPE_ETH_PC,
    NX_TYPE_LOOPBACK,
    NX_TYPE_SUBINTF,
    NX_TYPE_TUNNEL,
    NX_TYPE_MGMT,
    NX_TYPE_MAX
} intf_type_e;


// Supported Mac types
typedef enum {
   /// Type is not set
   TYPE_NO_TYPE = 0,

   /// Type is static
   TYPE_STATIC,

   /// Type is dynamic
   TYPE_DYNAMIC,

   /// MAX Action Types
   TYPE_MAX_TYPE
} type_e;

// Running Environment
typedef enum {
    BASH, 
    VSH,
    REMOTE
} running_env_e;

}

#endif //__NX_COMMON_TYPES__

