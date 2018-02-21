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

#ifndef __NX_CLI_IMPL_H__
#define __NX_CLI_IMPL_H__

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "nx_cli.h"

using namespace std;

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
class NxCliCmdImpl: public NxCliCmd {
public:

	/* ***************************************
	 * ** public data members
	 * ***************************************/

	/**
	 * Destructor
	 **/
	~NxCliCmdImpl();

	/**
	 * [Optional] Add more semantics to keywords in the CLI.
	 * Like HelpString, Alias etc. By default Keywords are
	 * constructed from the syntax without any HelpString or
	 * Aliases.
	 *  Ex) For config,
	 *      app1 vlan 10
	 *      keyword: app1
	 **/
	void updateKeyword(const char *keyword_name,
		            const char *help_str);

    void updateParam (const char *param_name,
                      const char *help_str, 
                      nxos::param_type_e param_type=nxos::P_STRING,
                      void *param_type_attributes=NULL,                    
                      int param_type_attr_len=0,
                      bool is_key=false, 
                      bool additive=false,
                      uint8_t repeat_count=0);
    void addTimeout(uint32_t timeout);

    /**
     * [Optional] Specify the mode in which the cmd belongs.
     *  Ex) For config, Syntax: hello [ A | B ]
     *      mode: CONF.
     **/
	void addMode(modes_t mode);

	/**
	 * Getter functions
	 **/
	cmdtype_t        getCmdType();
	modes_t          getCmdMode();
	std::string      getCmdName();
	std::string      getCmdSyntax();
	std::string      getCmdLineStr();
    std::string      getCmdErr();
    bool             isKeywordSet(const char *keyword_name);
    nxos::param_type_e getParamType(const char *param_name);
    void             *getParamValue(const char *param_name);
    int              getParamCount(const char *param_name);

	/**
	 * Print on the console
	 **/
	void printConsole(const char *fmt, ...);

    void updateKeyword(const char *keyword_name,
                       const char *help_str,
                       bool make_key);
};

class NxCliParserImpl: public NxCliParser {
public:

	/* ***************************************
	 * ** public methods
	 * ***************************************/

    /**
     * Destructor
     **/
    ~NxCliParserImpl();

    /// Create a new Cli command object 
    NxCliCmd* newCliCmd(NxCliCmd::cmdtype_t ctype,
                        const char *cmd_name,
                        const char *syntax);

    NxCliCmd *newShowCmd(const char *cmd_name, const char *syntax);
    NxCliCmd *newConfigCmd(const char *cmd_name, const char *syntax);

	/**
	 * Register all custom commands in CLI Parser List
     * to NX CLI Parser Tree.
	 **/
	void addToParseTree();

	/**
	 * Delete all your custom commands from NX CLI 
     * Parser List.
	 **/
	void delFromParseTree();

    /**
     * Setter function
     **/
	void setCmdHandler(NxCmdHandler *handler);

    /** 
     * Getter function
     **/
    std::string  getParserStatus();
	NxCmdHandler *getCmdHandler();

    char* execShowCmd(std::string show_syntax,
                      nxos::record_type_e type=nxos::R_TEXT,
                      bool *oper_result=NULL);
};

}

#endif //__NX_CLI_IMPL_H__
