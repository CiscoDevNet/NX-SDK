/** **********************************************************************
    * This file is auto-generated by nxsdk_remote_stub_gen.py 
    * at 2019-07-18 23:18:33.475828
    * 
    * Copyright (c) 2017 by cisco Systems, Inc.
    *
    * All rights reserved.
************************************************************************/
#include "nx_cli_impl.h" 
   
using namespace nxos;

   
    
// Implementation for class  
NxCliCmdImpl::NxCliCmdImpl()
{

}

NxCliCmdImpl::~NxCliCmdImpl()
{

}

void 
NxCliCmdImpl::updateKeyword(const char* keyword_name, const char* help_str)
{
   return;
}

void 
NxCliCmdImpl::updateParam(const char* param_name, const char* help_str, nxos::param_type_e param_type, void* param_type_attributes, int param_type_attr_len, bool is_key, bool additive, uint8_t repeat_count)
{
   return;
}

void 
NxCliCmdImpl::addTimeout(uint32_t timeout)
{
   return;
}

void 
NxCliCmdImpl::addMode(modes_t mode)
{
   return;
}

NxCliCmd::cmdtype_t 
NxCliCmdImpl::getCmdType()
{
   return (NxCliCmd::cmdtype_t)0;
}

NxCliCmd::modes_t 
NxCliCmdImpl::getCmdMode()
{
   return (NxCliCmd::modes_t)0;
}

std::string 
NxCliCmdImpl::getCmdName()
{
   return "";
}

std::string 
NxCliCmdImpl::getCmdSyntax()
{
   return "";
}

std::string 
NxCliCmdImpl::getCmdLineStr()
{
   return "";
}

std::string 
NxCliCmdImpl::getCmdErr()
{
   return "";
}

bool 
NxCliCmdImpl::isKeywordSet(const char* keyword_name)
{
   return false;
}

nxos::param_type_e 
NxCliCmdImpl::getParamType(const char* param_name)
{
   return (nxos::param_type_e)0;
}

void* 
NxCliCmdImpl::getParamValue(const char* param_name, bool fromFirst)
{
   return NULL;
}

int 
NxCliCmdImpl::getParamCount(const char* param_name)
{
   return (int)0;
}

void 
NxCliCmdImpl::printConsole(const char* fmt,  ...)
{
   return;
}

void 
NxCliCmdImpl::updateKeyword(const char* keyword_name, const char* help_str, bool make_key)
{
   return;
}

nxos::record_type_e 
NxCliCmdImpl::showCliOutFormat()
{
   return (nxos::record_type_e)0;
}
    
// Implementation for class  
NxCliParserImpl::NxCliParserImpl()
{

}

NxCliParserImpl::~NxCliParserImpl()
{

}

NxCliCmd* 
NxCliParserImpl::newCliCmd(NxCliCmd::cmdtype_t ctype, const char* cmd_name, const char* syntax)
{
   return NULL;
}

NxCliCmd* 
NxCliParserImpl::newShowCmd(const char* cmd_name, const char* syntax)
{
   return NULL;
}

NxCliCmd* 
NxCliParserImpl::newConfigCmd(const char* cmd_name, const char* syntax)
{
   return NULL;
}

void 
NxCliParserImpl::addToParseTree()
{
   return;
}

void 
NxCliParserImpl::delFromParseTree()
{
   return;
}

void 
NxCliParserImpl::setCmdHandler(NxCmdHandler* handler)
{
   return;
}

std::string 
NxCliParserImpl::getParserStatus()
{
   return "";
}

NxCmdHandler* 
NxCliParserImpl::getCmdHandler()
{
   return NULL;
}

char* 
NxCliParserImpl::execShowCmd(std::string show_syntax, nxos::record_type_e type, bool* oper_result)
{
   return NULL;
}

char* 
NxCliParserImpl::execConfigCmd(const char* filename)
{
   return NULL;
}

char* 
NxCliParserImpl::execConfigCmd(const char* filename_cmd, bool isfilename)
{
   return NULL;
}

nxos::err_type_e 
NxCliParserImpl::addTechSupport(string show_cmd_group, bool add_internals)
{
   return (nxos::err_type_e)0;
}