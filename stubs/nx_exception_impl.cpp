// ********************************************************************
//  *
//  * File:   nx_exception_impl.cpp
//  *
//  * Description:
//  *   stub implementation for nxsdk exceptions
//  *
//  * Copyright (c) 2016 by cisco Systems, Inc.
//  * All rights reserved.
//  *
//  *
//  * $Id: $
//  * $Source: $
//  * $Author: $
//  *
//  *********************************************************************

#include "nx_exception_impl.h"

using namespace nxos;
using namespace std;

NxExceptionImpl::~NxExceptionImpl() {}
unsigned long NxExceptionImpl::getErrCode() { return 0; }
const char *NxExceptionImpl::what() { return "NULL"; }
unsigned short NxExceptionImpl::getErrSeverity() { return 0; }
const char *NxExceptionImpl::getErrModule() { return "NULL"; }
const char *NxExceptionImpl::getErrApi() { return "NULL"; }
