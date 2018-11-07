/*********************************************************************
 *
 **File        :   nx_cli_impl.h
 *
 **Description :
 *   Stub CLI Implementation header file
 *
 **Copyright (c) 2016-2017 by cisco Systems, Inc.
 **All rights reserved.
 *
 *
 **$Id         : $
 * $Source    : $
 * $Author    : $
 *
 *********************************************************************
 */

#ifndef __NX_EXCEPTION_IMPL_H__
#define __NX_EXCEPTION_IMPL_H__

#include "nx_exception.h"

namespace nxos {


/**
 * Abstract Class used to construct a custom CLI config.
 * Steps to construct a custom CLI:
 *   1) Instantiate NX CLI Parser Object
 *   2) Create a CMD object 
 *   3) Fill in the appropriate fields for the command
 *      like cmd name, type, syntax, keywords, mode etc
 *   4) Add the command to CLI Parser List
 *   5) Repeat steps 1-4 for other commands. 
 *   6) Final step: Register all the commands from CLI Parser List to 
 *      NX Parser tree. If it succeeds then try your commands in 
 *      NX VSH. 
 **/
class NxExceptionImpl: public NxException {
public:
    ~NxExceptionImpl();
    unsigned long getErrCode(); 
    const char  *what();
    unsigned short getErrSeverity();
    const char  *getErrModule();
    const char  *getErrApi();
};
}

#endif
