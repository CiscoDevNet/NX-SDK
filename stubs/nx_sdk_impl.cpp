// ********************************************************************
//  *
//  * File:   nx_sdk_impl.cpp
//  *
//  * Description:
//  *   stub implementation for nx_sdk.h
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

#include "nx_sdk_impl.h"


using namespace nxos;
using namespace std;

NxSdk* nxos::NxSdk::getSdkInst(int argc, char **argv)
{
   return NULL;
}

/// Destructor
NxSdkImpl::~NxSdkImpl() 
{
}

/// [Optional] Sdk Desc setter function
void NxSdkImpl::setAppDesc(std::string desc)
{

}

/// Sdk Name getter function
const char* NxSdkImpl::getAppName()
{
   return("NULL");
}

/// Sdk Id getter function
int NxSdkImpl::getAppId()
{
   return(0);
}

/// Sdk Desc getter function
const char* NxSdkImpl::getAppDesc()
{
   return("NULL");
}

/// Cli Parser object getter function
NxCliParser* NxSdkImpl::getCliParser()
{
   return(NULL);
}

///Tracer syslog object getter function
NxTrace* NxSdkImpl::getTracer()
{
    return(NULL);   
}

/// Start Event loop
void NxSdkImpl::startEventLoop()
{
   return;
}

/// Stop Event loop
void NxSdkImpl::stopEventLoop()
{
   return;
}

extern "C" NxSdk* create() {
    return NULL;
}

extern "C" void destroy(NxSdk* p) {
    delete p;
}
