/*********************************************************************
 *
 **File        :   nx_cli_impl.cpp
 *
 **Description :
 *   Stub CLI Implementation for nx_cli.h
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

#include "nx_cli_impl.h"

using namespace std;

using namespace nxos;
using namespace std;


/// Destructor
NxCliCmdImpl::~NxCliCmdImpl()
{
}

void NxCliCmdImpl::updateKeyword (const char *keyword_name,
		                          const char *help_str)
{

}

void NxCliCmdImpl::updateParam (const char *param_name,
                                const char *help_str,
                                nxos::param_type_e param_type/*=nxos::P_STRING*/,
                                void *param_type_attributes/*=NULL*/,
                                int param_type_attr_len/*=0*/,
                                bool is_key/*=false*/,
                                bool additive/*=false*/,
                                uint8_t repeat_count/*=0*/) 
{

}

void NxCliCmdImpl::addTimeout(uint32_t timeout)
{

}

void NxCliCmdImpl::addMode(modes_t mode)
{

}

NxCliCmdImpl::cmdtype_t NxCliCmdImpl::getCmdType()
{
    return((NxCliCmdImpl::cmdtype_t)0);
}

NxCliCmdImpl::modes_t NxCliCmdImpl::getCmdMode() 
{
    return((NxCliCmdImpl::modes_t)0);
}

std::string NxCliCmdImpl::getCmdName()
{
    return("NULL");
}

std::string NxCliCmdImpl::getCmdSyntax()
{
    return("NULL");
}

std::string NxCliCmdImpl::getCmdLineStr()
{
    return("NULL");
}

std::string NxCliCmdImpl::getCmdErr()
{
    return("NULL");
}

bool
NxCliCmdImpl::isKeywordSet(const char *keyword_name)
{
    return(false);
}

nxos::param_type_e 
NxCliCmdImpl::getParamType(const char *param_name) 
{
    return(nxos::P_MAX_TYPE);
}

void *
NxCliCmdImpl::getParamValue(const char *param_name)
{
    return(NULL);    
}

int
NxCliCmdImpl::getParamCount(const char *param_name)
{
    return(0);
}

/**
 * Print on the console
 **/
void NxCliCmdImpl::printConsole(const char *fmt, ...)
{
}

void NxCliCmdImpl::updateKeyword(const char *keyword_name,
                                 const char *help_str,
                                 bool make_key)
{
    return;
}

/**
 * Destructor
 **/
NxCliParserImpl::~NxCliParserImpl()
{

}

/// Create a new Cli command object 
NxCliCmd* NxCliParserImpl::newCliCmd(NxCliCmd::cmdtype_t ctype,
                                     const char *cmd_name,
                                     const char *syntax)
{
    return(NULL);
}

NxCliCmd* NxCliParserImpl::newShowCmd(const char *cmd_name, 
                                      const char *syntax)
{
    return(NULL);
}

NxCliCmd* NxCliParserImpl::newConfigCmd(const char *cmd_name, 
                                        const char *syntax)
{
    return(NULL);
}

/**
 * Register all custom commands in CLI Parser List
 * to NX CLI Parser Tree.
 **/
void NxCliParserImpl::addToParseTree()
{

}

/**
 * Delete all your custom commands from NX CLI 
 * Parser List.
 **/
void NxCliParserImpl::delFromParseTree()
{
}

void NxCliParserImpl::setCmdHandler(NxCmdHandler *handler)
{

}

std::string NxCliParserImpl::getParserStatus()
{
    return("NULL");
}

NxCmdHandler* NxCliParserImpl::getCmdHandler()
{
    return(NULL);
}

char* NxCliParserImpl::execShowCmd (std::string show_syntax,
                                    nxos::record_type_e type,
                                    bool *oper_result)
{
    return(NULL);     
}

