/** @file nx_cli.h
 *  @brief Abstract CLI interface to generate custom CLI configs in Nexus Switches.
 *
 *  Provides necessary Abstraction/Plugin for NXOS CLI Parser functionalities.
 *  It provides the Ability to construct custom CLIs and callback handlers
 *  when a specific CLI is exectuted.
 *
 *  Steps to construct a custom CLI:
 *   1) Instantiate NxSDK CLI Parser Object using getCliParser().
 *   2) Create a CMD object using newCliCmd().
 *   3) Fill in the appropriate fields for the command like cmdName, type, syntax etc.
 *   4) Update the Keywords, param if needed with custom values using updateKeyword(),
 *      updateParam() etc.
 *   5) Repeat steps 2)-4) for new commands.
 *   6) After creating all the cmds then set the callback handler to be called
 *   7) Add them to NX CLI Parser List using addToParseTree().
 *      If it succeeds then try your commands in NX VSH.
 *   NOTE: By default, some custom CLIs are auto-generated for the NxSDK App.
 *         Try "show $appname ?" in the box to access them.
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 */

#ifndef __NX_CLI_H__
#define __NX_CLI_H__

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "types/nx_cli.h"

using namespace std;

namespace nxos
{

/**
 * @brief Abstract custom CLI command Interface used to construct a custom CLI config.
 *
 * Takes Parameters for custom CLI like cmdName, Syntax, Keywords, Params, etc.
 **/
class NxCliCmd
{
public:
    /* ***************************************
    * ** public data members
    * ***************************************/

    ///Custom Mode for a Custom CLI command.
    typedef enum
    {
        /// Command can be executed in "conf t".
        CONF_MODE = 0,

        /// Command can be executed in any mode.
        EXEC_MODE,

        /// Maximum modes supported
        MAX_MODE
    } modes_t;

    ///Custom CLI command type
    typedef enum
    {
        /// Custom command is a Configuration command.
        CONF_CMD = 0,

        /// Custom command is a show command.
        SHOW_CMD,

        /// Maximum command type supported.
        MAX_CMD
    } cmdtype_t;

    /**
     * Destructor
     **/
    virtual ~NxCliCmd(){};

    /**
     * [Optional] Update more semantics to keywords in the CLI. Like HelpString,
     * Alias etc. By default Keywords are constructed from the syntax with
     * HelpString same as the keyword name and without any Aliases.
     * @param[in] keyword_name Keyword used in the syntax
     * @param[in] help_str HelpString associated with the keyword.
     *
     *  @code
     *  Usage:
     *     If you need to create a CLI with custom keyword (HelpString etc)
     *
     *        CLI     : $appname port-bw threshold <threshold>
     *        Keywords: port-bw, threshold
     *        Default HelpString for: port-bw is "port-bw", threshold is "threshold"
     *
     *  C++:
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       cliP = sdk->getCliParser();
     *       cmd = cliP->newConfigCmd("set_port_bw_threshold_cmd",
     *                                "port-bw threshold <threshold>");
     *       cmd->updateKeyword("port-bw", "Port Bandwidth Information");
     *       cmd->updateKeyword("threshold", "Port BandWidth Threshold Alert");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       cliP = sdk.getCliParser()
     *       cmd = cliP.newConfigCmd("set_port_bw_threshold_cmd",
     *                               "port-bw threshold <threshold>")
     *       cmd.updateKeyword("port-bw", "Port Bandwidth Information");
     *       cmd.updateKeyword("threshold", "Port BandWidth Threshold Alert");
     *
     *   Switch:
     *       After setting Custom HelpString for keywords.
     *       switch(config)# $appname ?
     *                       port-bw    Port Bandwidth Information
     *       switch(config)# $appname port-bw ?
     *                       threshold  Port Bandwidth Threshold
     *  @endcode
     *
     *   @throws Keyword and help_str cannot be empty or NULL.
     *   @throws Keyword does not exist in the cmd Syntax
     *   @throws Cannot edit the keyword if the cmd is already added to NX Parser.
     **/
    virtual void updateKeyword(const char *keyword_name,
                               const char *help_str) = 0;

    /**
     * [Optional] Update more semantics to the input parameter in the CLI.
     *            Input parameters should be enclosed in <>. By default,
     *            Input parameters type is a string. By default param
     *            semantics are generated from the syntax with defaults.
     *            By default, HelpString is same as the parameter name.
     * @param[in] param_name Name of the input parameter
     * @param[in] help_str HelpString for the input parameter
     * @param[in] param_type Type of the input Parameter. Default is String.
     * @param[in] param_type_attributes Optional Additional attributes needed for
     *                                  the input parameter type. Refer to
     *                                  example applications.          <br>
     *            param_type  |  param_type_attributes                 <br>
     *            --------------------------------------------------   <br>
     *            P_INTEGER   | cli_param_type_integer_attr            <br>
     *            P_STRING    | cli_param_type_string_attr             <br>
     *            P_INTERFACE | NULL                                   <br>
     *            P_IP_ADDR   | cli_param_type_ip_addr_attr            <br>
     *            P_MAC_ADDR  | NULL                                   <br>
     *            P_VRF       | NULL                                   <br><br>
     *
     * @note For usage and code refer to respective types in types/nx_cli.h
     * @param[in] param_type_attr_len Length of the void *param_type_attributes
     *                                if passed. With this we can validate if
     *                                right attribute has been passed for the type.
     * @param[in] is_key If set to TRUE then this param value will be added to the
     *                   unique key for this config. Used only for config commands.
     *                   For Ex) Lets say CLI syntax is A <id>,
     *                   - If <id> is not set as key (is_key=false) then
     *                     Lets say we configure "A 10" then we config "A 20"
     *                     then config "A 20" would replace/update "A 10".
     *                     Hence "show run" will have "A 20".
     *                   - If <id> is set as key (is_key=true) then Lets say
     *                     we configure "A 10" then we config "A 20", we will
     *                     have two entries "A 10" and "A 20". Hence "show run"
     *                     will have both "A 10" and "A 20".
     * @param[in] additive Array of inputs for this input parameter
     * @param[in] repeat_count Number of inputs.
     *  <br>
     *  Ex)  Syntax  : (A | B <id>) [C]                              <br>
     *       Default HelpString for: <id> is id.                     <br>
     *       Param   : <id> [if additive = true, repeat_count = 5]   <br>
     *                 <id> can take from 1 to 5 inputs.             <br>
     *
     *  @code
     *  Usage:
     *     If you need to create a CLI with custom HelpString
     *
     *        CLI      : $appname port-bw threshold <threshold>
     *        Parameter: <threshold>
     *
     *  C++:
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       cliP = sdk->getCliParser();
     *       nxos::cli_param_type_integer_attr int_attr = {0};
     *       int_attr.min_val = 1;
     *       int_attr.max_val = 100;
     *       cmd = cliP->newConfigCmd("set_port_bw_threshold_cmd",
     *                                    "port-bw threshold <threshold>");
     *       cmd->updateParam("<threshold>", "Threshold Limit. Default 50%", nxos::P_INTEGER,
     *                        (void *)&int_attr, sizeof(int_attr));
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       cliP = sdk.getCliParser()
     *       int_attr = nx_sdk_py.cli_param_type_integer_attr()
     *       int_attr.min_val = 1;
     *       int_attr.max_val = 100;
     *       cmd = cliP.newConfigCmd("set_port_bw_threshold_cmd",
     *                               "port-bw threshold <threshold>")
     *       cmd.updateParam("<threshold>", "Threshold Limit. Default 50%", nxos::P_INTEGER,
     *                       int_attr, len(int_attr))
     *  @endcode
     *
     *   @throws param and help_str cannot be empty/NULL.
     *   @throws Param does not exist in the cmd Syntax
     *   @throws Cannot edit the keyword if the cmd is already added to NX Parser.
     *   @throws param_name does contains more than one word and doesnt match
     *           regex A-Za-z0-9_-
     *   @throws param_type is a invalid type.
     *   @throws param_type_attr_len doesnt match with the expected sizeof(param_type).
     **/
    virtual void updateParam(const char         *param_name,
                             const char         *help_str,
                             nxos::param_type_e param_type = nxos::P_STRING,
                             void               *param_type_attributes = NULL,
                             int                param_type_attr_len = 0,
                             bool               is_key = false,
                             bool               additive = false,
                             uint8_t            repeat_count = 0) = 0;

    /**
     * [Optional] Timeout to be set for the CLI if its going to take more time
     *            to execute the action. Default is 30secs
     * @param[in] timeout in seconds
     *
     * @throws Cannot edit timeout if the cmd is already added to NX Parser.
     **/
    virtual void addTimeout(uint32_t timeout) = 0;

    /**
     * [Optional] Specify the mode in which the cmd belongs.
     *  Defaults: For SHOW_CMD, mode is EXEC_MODE.
     *            For CONF_CMD, mode is CONF_MODE
     *   @throws mode is invalid.
     *   @throws Cannot edit the mode if the cmd is already added to NX Parser.
     **/
    virtual void addMode(modes_t mode) = 0;

    /// Get the type of the CLI command.
    virtual NxCliCmd::cmdtype_t getCmdType() = 0;

    /// Get the mode of the CLI command
    virtual NxCliCmd::modes_t getCmdMode() = 0;

    /// Get the Name of the CLI command
    virtual std::string getCmdName() = 0;

    /// Get the Syntax of the CLI command
    virtual std::string getCmdSyntax() = 0;

    /// Get the CLI command entered
    virtual std::string getCmdLineStr() = 0;

    /// Get CLI Cmd Err if there is any
    virtual std::string getCmdErr() = 0;

    /**
     * Helper function to Check if the given keyword is set/present
     * in the entered CLI. Should be called only in command handler
     * postCliCb. Or other way is you can use getCmdLineStr() to
     * get the entered CLI and do a string find yourself to find
     * the keywords set.
     *
     * @param[in] keyword_name Name of the keyword.
     *
     * @code
     *  Usage:
     *       switch(config)# $appname port-bw (threshold <val1> | port <val2>)
     *       This CLI can either take threshold (or) port.
     *       In postCliCb func, to check which keyword was entereted
     *
     *  C++:
     *       if (cmd->isKeywordSet("threshold")) {
     *       } else if (cmd->isKeywordSet("port")) {
     *       }
     *
     *  Python:
     *       if (cmd.isKeywordSet("threshold")) :
     *       elif (cmd.isKeywordSet("port")) :
     *  @endcode
     *
     * @returns true  - if keyword is set in the entered CLI
     *          false - if keyword is not set in the entered CLI
     *
     * @throws if the API is called outside of command handler postCliCb.
     * @note Refer to example Apps.
     **/
    virtual bool isKeywordSet(const char *keyword_name) = 0;

    /**
     * Gets the type of parameter in the CLI. Should be called
     * only in command handler postCliCb.
     * @param[in] param_name Name of the input parameter
     * @param[out] returns the type of parameter.
     *
     *  @code
     *  Usage:
     *     If you need to get the type of input parameter.
     *
     *       CLI      : $appname port-bw threshold <threshold>
     *       Parameter: <threshold>
     *
     *       switch(config)# $appname port-bw threshold 10
     *       In postCliCb func, Get Parameter type for Threshold(=P_INTEGER)
     *
     *  C++:
     *       nxos::param_type_e val = cmd->getParamType("<threshold>");
     *
     *  Python:
     *       val = cmd.getParamType("<threshold>");
     *  @endcode
     *
     * @throws param_name is not a single word and does not match
     *         regex A-Za-z0-9-_
     * @throws if the API is called outside of command handler postCliCb.
     * @throws if the param_name does not exist in the entered config.
     * @note Refer to example Apps.
     **/
    virtual nxos::param_type_e getParamType(const char *param_name) = 0;

    /**
     * Gets the values of the given input parameter in <>.
     * Should be called only in command handler postCliCb.
     * @param[in] param_name Name of the input parameter
     * @param[in] fromFirst At anytime, to get or start from first
     *                      input parameter value, set fromFirst to
     *                      True and then call it in a while loop
     *                      with fromFirst to False to loop through
     *                      the list if the input parameter is an
     *                      array. Refer to usage.
     *
     * @returns single value of the input parameter if called once.
     *          To get an array of values associated with a additive
     *          parameter, call the API in a while loop. If cannot be
     *          found it returns NULL. NOTE: Its not thread-safe.
     *           <br>
     *            param_type  |  Return type                         <br>
     *            -------------------------------------------------- <br>
     *            P_INTEGER   |  int *                               <br>
     *            P_STRING    |  char *                              <br>
     *            P_INTERFACE |  char *                              <br>
     *            P_IP_ADDR   |  char *                              <br>
     *            P_MAC_ADDR  |  char *                              <br>
     *            P_VRF       |  char *                              <br><br>
     *
     *  @code
     *  Usage for return type int *:
     *   Ex) Syntax: port-bw threshold <threshold>
     *       CLI: $appname port-bw threshold 10 20
     *   In postCliCb, to get <threshold> parameter value (=10, 20)
     *
     *  C++:
     *       // To get Multiple values
     *       int val = *((int *)cmd->getParamValue("<threshold>"), true);
     *       while (val) {
     *           //print val
     *           val = *((int *)cmd->getParamValue("<threshold>"));
     *       }
     *
     *  Python:
     *       ### Use nx_sdk_py.void_to_int to convert from C void *
     *       ### of type int to python int Object.
     *       int_p = nx_sdk_py.void_to_int(cmd.getParamValue("<threshold>"), True)
     *       while int_p:
     *          threshold = int(nx_sdk_py.intp_value(int_p))
     *          #print threshold
     *          int_p = nx_sdk_py.void_to_int(cmd.getParamValue("<threshold>"), True)
     *
     *  @endcode
     *
     *  @code
     *  Usage for return type char *:
     *    Ex) Syntax: show $appname port-bw <port>
     *        CLI: show $appname port-bw ethernet1/1
     *    In postCliCb, to get <port> parameter value (=ethernet1/1)
     *
     *  C++:
     *       char *port = (char *)cmd->getParamValue("<port>");
     *
     *  Python:
     *       ### Use nx_sdk_py.void_to_string to convert from C void *
     *       ### of type char * to python String Object.
     *       port = nx_sdk_py.void_to_string(cmd.getParamValue("<port>"))
     *       print port
     *  @endcode
     *
     * @throws param_name is not enclosed in <>
     * @throws param_name is not a single word and does not match
     *         regex A-Za-z0-9-_
     * @throws if the API is called outside of command handler postCliCb.
     * @throws if the param_name does not exist in the entered config.
     * @note Refer to example Apps.
     **/
    virtual void *getParamValue(const char *param_name, bool fromFirst = false) = 0;

    /**
     * Given the input parameter, getParamCount returns the number(count)
     * of values associated with the input parameter. Should be called
     * only in command handler postCliCb.
     *
     * @param[in] param_name Name of the input parameter
     *
     * @returns the count of array of values associated with the parameter.
     *          A <param> can contain a single value or array of
     *          values if its set as additive. Should be called only
     *          in command handler postCliCb.
     *
     *  @code
     *  Usage:
     *     To get a count of the number of input param.
     *
     *       CLI      : $appname port-bw threshold <threshold>
     *       Parameter: <threshold> [in updateParam set, additive = true,
     *                               repeat_count = 5]
     *                  <threshold> can take one value (or) an array of
     *                  utpo 5 values
     *
     *       switch(config)# $appname port-bw threshold 10 20 30
     *       In postCliCb func, Get Parameter array count for Threshold (=3)
     *
     *  C++:
     *       int val = cmd->getParamCount("<threshold>");
     *
     *  Python:
     *       val = cmd.getParamCount("<threshold>");
     *  @endcode
     *
     * @throws param_name is not enclosed in <>
     * @throws param_name is not a single word and does not match
     *         regex A-Za-z0-9-_
     * @throws if the API is called outside of command handler postCliCb.
     * @throws if the param_name does not exist in the entered config.
     * @note Refer to example Apps.
     **/
    virtual int getParamCount(const char *param_name) = 0;

    /**
     * Print custom output on the switch console only
     * when called inside command handler postCliCb.
     * Does nothing if called in anyother place.
     *
     * @note Refer to example Apps.
     **/
    virtual void printConsole(const char *fmt, ...) = 0;

    /**
     * @note Following APIs are supported from NXSDK V1.5
     **/

    /**
     *  Same as V1.0 except the addition of new parameter
     *  make_key.
     *
     * @param[in] make_key If set to TRUE then this keyword will be added to the
     *                   unique key for this config. Used only for config commands.
     *                   For Ex) Lets say CLI syntax is "action [A| B]",
     *                   - If A keyword is not set as key (make_key=false) then
     *                     Lets say we configure "action A" then we config
     *                     "action B" then config "action B" would replace/update
     *                     "action A".
     *                     Hence "show run" will have "action B".
     *                   - If A & B keywords are set as key (make_key=true) then Lets say
     *                     we configure action A" then we config "action B", we will
     *                     have two entries "action A" and "action B". Hence "show run"
     *                     will have both "action A" and "action B".
     *
     *  @note Refer to V1.0 API section to get more details
     *        about other parameters.
     *
     * @since V1.5
     **/
    virtual void updateKeyword(const char *keyword_name,
                               const char *help_str,
                               bool       make_key) = 0;
};

/**
 * @brief Custom Cli Command Callback Handler Class
 **/
class NxCmdHandler
{
public:
    virtual ~NxCmdHandler(){};

    /**
     * User to overload the postCliCb callback method
     * to execute an action when their respective custom
     * CLI is executed.
     * @param[in]  assoicated cmd obj for the executed CLI command.
     * @param[out] True : the command was successful
     *             False: command failed.
     **/
    virtual bool postCliCb(NxCliCmd *cmd) { return (true); }
};

/**
 * @brief Abstract Interface to access Nexus CLI Parser tools.
 *
 * Interface to add, delete, modify custom CLI commands into
 * Nexus Parser.
 **/
class NxCliParser
{
public:
    /* ***************************************
    * ** public methods
    * ***************************************/

    /**
     * Destructor
     **/
    virtual ~NxCliParser(){};

    /**
     * Create a new Custom Cli command object
     * @param[in] ctype    Cmd Type (ex CONF_CMD, SHOW_CMD etc)
     * @param[in] cmd_name Cmd Name (NOTE: SDK appends App Name to the
     *                                     passed Cmd Name (AppName_CmdName))
     * @param[in] syntax   Custom Cmd syntax. Syntax comprises of Keywords
     *                     and input parameters. Input parameters are identified
     *                     by enclosing in <> and by default they are of the type "string".
     *                     Refer to updateKeyword & updateParam usage.
     *                     NOTE: All CONF commands are prefixed with AppName. For SHOW
     *                           commands, SDK appends AppName after the show keyword.
     *                           So that the custom configs dont overwrite the existing
     *                           Nexus Configs.
     *
     * @code
     *  Usage:
     *       Ex1) For Config: $appName port-bw threshold <threshold>
     *             API:  newCliCmd(nxos::NxCliCmd::CONF_CMD, "vlan_cmd", "vlan <vlanid>");
     *             $AppName: (Automatically appended by SDK)
     *  C++:
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       cliP = sdk->getCliParser();
     *       cmd = cliP->newConfigCmd("set_port_bw_threshold_cmd",
     *                                    "port-bw threshold <threshold>");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       cliP = sdk.getCliParser()
     *       cmd = cliP.newConfigCmd("set_port_bw_threshold_cmd",
     *                               "port-bw threshold <threshold>")
     * @endcode
     *
     * @code
     *       Ex2) For Config: show $appName (A | B <id>) [C]
     *             API:  newCliCmd(nxos::NxCliCmd::SHOW_CMD, "complex_cmd", "(A | B <id>) [C]");
     *             App Name: show $appName (Automatically appended by SDK)
     *             Syntax  : (A | B <id>) [C]
     *                 Keyword   : A, B, C
     *                 Param     : <id> (Default type: String)
     *                 Explantion: (| - A or B <id>)
     *                             ([] - Optional keyword C)
     *
     *  C++:
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       cliP = sdk->getCliParser();
     *       cmd = cliP->newConfigCmd("set_complex_cmd",
     *                                "(A | B <id>) [C]");
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       cliP = sdk.getCliParser()
     *       cmd = cliP.newConfigCmd("set_complex_cmd", "(A | B <id>) [C]")
     * @endcode
     *
     * NOTE:
     *   @throws ctype is a invalid command type
     *   @throws cmd_name and syntax cannot be empty or NULL.
     *   @throws syntax is not balanced.
     *   @throws Cannot edit the syntax if the cmd is already added to NX Parser.
     *           Can add new custom cmd to Nx Parser but not edit existing cmd
     *           already added to the parser.
     *   @throws syntax contains anything other than the regex "A-Za-z0-9[]{}()<>|_- "
     *   @throws syntax contains free standing _ or - or |.
     *   @throws syntax contains empty open close paranthesis without a word.
     *   @throws syntax <param> dose not contain only one word and doesnt match
     *           regex A-Za-z0-9_-
     *   @throws syntax contains predefined keywords (no, show)
     */
    virtual NxCliCmd *newCliCmd(NxCliCmd::cmdtype_t ctype,
                                const char          *cmd_name,
                                const char          *syntax) = 0;

    /**
     * Create a new custom Show CLI command object.
     * Calls newCliCmd(nxos::NxCliCmd::SHOW_CMD, cmd_name, syntax)
     **/
    virtual NxCliCmd *newShowCmd(const char *cmd_name,
                                 const char *syntax) = 0;

    /**
     * Create a new custom Show CLI command object.
     * Calls newCliCmd(nxos::NxCliCmd::CONF_CMD, cmd_name, syntax)
     **/
    virtual NxCliCmd *newConfigCmd(const char *cmd_name,
                                   const char *syntax) = 0;

    /**
     * Register all created custom commands to NX CLI Parser Tree.
     *  @note Check "show $appname internal state" if the custom
     *        commands are successfully added or not.
     *  @throws if called without creating any custom commands.
     *  @throws if NX CLI Parser rejects the custom commands.
     **/
    virtual void addToParseTree() = 0;

    /**
     * Delete all your custom commands from NX CLI Parser Tree.
     * @note Check "show AppName internal state" if the custom
     *       commands are successfully deleted or not.
     **/
    virtual void delFromParseTree() = 0;

    /**
     * Set the CLI callback handler object when the
     * respective config is executed.
     **/
    virtual void setCmdHandler(NxCmdHandler *handler) = 0;

    /// Get Parser Status/Errors
    virtual std::string getParserStatus() = 0;

    /// Get Cli callback handler object
    virtual NxCmdHandler *getCmdHandler() = 0;

    /**
     * Execute other show commands.
     * @param[in] show_syntax syntax of show command to be executed.
     * @param[in] type Type(TEXT, XML, JSON) of the record to be returned.
     * @param[in] oper_result if show output was successful. Returns true
     *            if show was successful (or) false if its a invalid show command.
     * @param[out] Char* containing show output in the type format specified.
     *             It is not thread safe and any subsequent call to execShowCmd
     *             will overwrite the previous output. Hence make a copy
     *             of the output than the pointer if you need to store the
     *             output.
     **/
    virtual char *execShowCmd(std::string         show_syntax,
                              nxos::record_type_e type = nxos::R_TEXT,
                              bool                *oper_result = NULL) = 0;

    /**
     * Execute other config commands in a file.
     * @param[in] filename File that contains all the config commands to be  executed.
     * @param[out] char* SUCCESS if config commands were executed successfully
     *                   if not contains the reason for failure.
     *
     * @code
     *  C++:
     *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
     *       cliP = sdk->getCliParser();
     *       char *result = cliP->execConfigCmd(filename);
     *       if (strcmp(result, "SUCCESS")== 0) {
     *           // Configs applied successfully.
     *       } else {
     *           // Configs failed. Check the Error.
     *       }
     *
     *  Python:
     *       import nx_sdk_py
     *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
     *       cliP = sdk.getCliParser()
     *       result = cliP.execConfigCmd(filename)
     *       if "SUCCESS" in result:
     *           #Configs applied successfully.
     *       else:
     *           #Configs failed. Check the Error.
     * @endcode
     *
     **/
    virtual char *execConfigCmd(const char *filename) = 0;
};

} // namespace nxos

#endif //__NX_CLI_H__
