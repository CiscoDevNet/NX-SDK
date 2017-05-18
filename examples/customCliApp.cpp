/*********************************************************************
 *
 * File:   customCliApp.cpp
 *
 * Description:
 *    A Sample C++ App to generate custom CLI of users choice
 *    along with respective callbacks showcasing the usage of
 *    NX-OS Infra SDK.
 *
 * Copyright (c) 2016-2017 by cisco Systems, Inc.
 * All rights reserved.
 *
 *
 * $Id: $
 * $Source: $
 * $Author: $
 *
 *********************************************************************
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <nx_sdk.h>
#include <nx_cli.h>
#include <nx_trace.h>
#include <stdexcept>

using namespace std;
using namespace nxos;

NxSdk       *sdk = NULL;
NxCliParser *cliP = NULL;
NxTrace     *traceP = NULL;

/**
 * Overload NxCmdHandler's postCliCb method which will
 * be invoked when the custom CLI gets executed.
 **/
class myCmdHandler : public NxCmdHandler {
public:
    myCmdHandler () {
      threshold = 50;
    }
    bool postCliCb(NxCliCmd *cmd);
private:
    int threshold;
};

/**
 * Handler Callback for Custom Cli execution.
 * Returns True if the action was successful.
 * False incase of failure.
 **/
bool myCmdHandler::postCliCb(NxCliCmd *cmd) 
{
    string cmd_name = cmd->getCmdName();
    string cli_str = cmd->getCmdLineStr();
    bool   ret_val = true;

    /// To print the output in VSH console. Should be called only in
    /// postCliCb
    cmd->printConsole("\n#####################################################");

    /// Action to be done when "port_bw_util_cmd" gets executed.
    if (cmd_name.find("port_bw_util_cmd") != std::string::npos) {
        char *port;
        std::string result_buf;

        /// To get input parameter value use getParamValue
        port = (char *)cmd->getParamValue("<port>");
        if (port) {
            cmd->printConsole("\nGet Port BW Utilization percent for %s", port);

            /*
             * To execute and get the result of other show command.
             */
            if (cliP) {
                result_buf = std::string(cliP->execShowCmd(string("show int ") + port, nxos::R_JSON));
            }
         
            /// Perform your action here.
        } else {
            cmd->printConsole("\nGet Port BW Utilization percent for all ports");
        }
    } else if (cmd_name.find("set_port_bw_threshold_cmd") != std::string::npos) {
        /// Action to be done when "set_port_bw_threshold_cmd" gets executed.
        threshold = *((int *)cmd->getParamValue("<threshold>"));
        cmd->printConsole("\nSet Port BW threshold %d", threshold);
    } else if (cmd_name.find("get_port_bw_threshold_cmd") != std::string::npos) {
        /// Action to be done when "get_port_bw_threshold_cmd" gets executed.
        cmd->printConsole("\nGet Port BW threshold value %d", threshold);
    } else if (cmd_name.find("sample_cmd") != std::string::npos) {

        /// Action to be done when "sample_cmd" gets executed.
        char *param_id1;
        int *param_id2;
        int idx = 0;

        try {
            cmd->printConsole("\n<id1> count: %d, <id2> count: %d",
                              cmd->getParamCount("<id1>"),
                              cmd->getParamCount("<id2>"));
            idx = 0;

            /*
             * id1 can take an array of inputs. To get the array of inputs,
             * use getParamValue in a while loop to get the next input values.
             * It returns "NULL" when it reaches the end. 
             */
            while ((param_id1= (char *)cmd->getParamValue("<id1>"))) {
                cmd->printConsole("\n[%d] id1 value is %s", idx++, param_id1);
            }

            /*
             * id2 will be set only if C keyword is set.
             * showcasing the use case if isKeywordSet API.
             */
            if (cmd->isKeywordSet("C")) {
                idx = 0;
                while ((param_id2 = ((int *)cmd->getParamValue("<id2>")))) {
                    cmd->printConsole("\n[%d] id2 value is %d", idx++, *param_id2);
                 }
            } else {
                 cmd->printConsole("\nC keyword is not set");
            }
        } catch (std::logic_error const& err) {
            cmd->printConsole("\nFailed with Err (%s)", err.what());
        }
    }

    cmd->printConsole("\n#####################################################");
    return ret_val;
}

/*
 * App Main
 */
int  main (int argc, char **argv)
{
   NxCmdHandler *cmdHandler = NULL;

   /**
    * getSdkInst is the first step for any custom Application
    * wanting to gain access to NXOS Infra. Without this 
    * NXOS infra cannot be used.
    *
    * NOTE: 
    *   Perform all SDK related initializations and startEventLoop in one
    *   thread. The call to startEventLoop will block the thread until we 
    *   break out of it by calling stopEventLoop.  
    *
    *   Perform other actions in a different thread.   
    **/ 
   sdk = nxos::NxSdk::getSdkInst(argc, argv);
   if (!sdk) {
       cout << "Could not get access to Nx Infra SDK" << endl;
       exit(-1);
   }
  
   /**
    *  To Create & Manage Custom CLI commands one must do
    *  getCliParser() which loads the plugin to NXOS CLI
    *  Infra Functionalities. 
    */
    cliP = sdk->getCliParser();
    if (!cliP) {
        cout << "cli parser is null";
        exit(-1); 
    }

   /**
    *  To Create & Manage Custom syslogs one must do
    *  getTracer() which loads the plugin to NXOS Syslog
    *  Infra Functionalities. 
    **/
    traceP = sdk->getTracer();
    if (!traceP) {
        cout << "Tracer obj is null";
        exit(-1);
    }

    try {

       /**
        * Construct  Custom show Port Bandwidth Utilization commands
        */
       nxos::NxCliCmd *cmd = cliP->newShowCmd("port_bw_util_cmd",  
                                              "port bw utilization [<port>]");
       cmd->updateKeyword("port", "Port Information");
       cmd->updateKeyword("bw", "Port Bandwidth Information");   
       cmd->updateKeyword("utilization", "Port BW utilization in (%)");
       cmd->updateParam("<port>", "Optional Filter Port Ex) Ethernet1/1", nxos::P_INTERFACE); 

       /**
        * Construct custom set Port Bandwidth Threshold config command.
        * NOTE:
        * Since we have already updated the Keyword information for
        * "port" and "bw" we dont have to update for each and every cmd
        * as its information will be automatically picked up for
        * other commands.
        **/
       nxos::cli_param_type_integer_attr int_attr = {0};

       /*
        * Setting additional attributes for input parameter.
        * In this ex) input parameter <threshold> can take only
        * input within the range 1-100. Any input outside the range
        * will be rejected by the NX CLI parser itself thereby
        * simplifying the CLI validation for App developers.
        *
        * NOTE: Refer to include/types/nx_cli.h for additional 
        *       attributes available for other input types.
        */
       int_attr.min_val = 1;
       int_attr.max_val = 100;
       cmd = cliP->newConfigCmd("set_port_bw_threshold_cmd",
                                 "port bw threshold <threshold>");
       cmd->updateKeyword("threshold", "Set Port BandWidth Threshold Alert");
       cmd->updateParam("<threshold>", "Threshold Limit. Default 50%", nxos::P_INTEGER, 
                        (void *)&int_attr, sizeof(int_attr)); 

       /**
        * Construct Custom show Port Bandwidth Utilization threshold commands
        * Since the keywords information are already updated we dont have
        * to do it for each and every keyword and parameters.
        */
       cliP->newShowCmd("get_port_bw_threshold_cmd",  
                        "port bw threshold");

       /*
        * Construct Custom Complex config command example.
        * Following command allows user to configue
        * A some value with either B or C with addtive
        * value. 
        */
        cmd =  cliP->newConfigCmd("sample_cmd", 
                                  "A <id1> {B | C <id2>}");
        cmd->updateKeyword("A", "A is a Keyword");
        cmd->updateParam("<id1>", "As id", nxos::P_STRING, NULL, 0, true);
        cmd->updateKeyword("B", "B is a Keyword");
        cmd->updateKeyword("C", "C is a Keyword");
        cmd->updateParam("<id2>", "Cs id", nxos::P_INTEGER, NULL, 0, false, true, 5);

        /**
         * Add the command callback Handler.
         * When the respective CLI commands gets configured 
         * the overloaded postCliCb callback will be instantiated.
         **/
        cmdHandler = new myCmdHandler();
        cliP->setCmdHandler(cmdHandler);

        /**
         * This is important as it Adds the constructed custom configs 
         * to NXOS CLI Parse tree. If it succeeds then for config command
         * use <app-name> ? and for show commands use show <app-name> ?
         * to check if your commands have been added to NXOS CLI tree
         * successfully. Refer to API documentation for the exceptions
         * thrown. Use "show <appname> nxsdk state".
         **/
        cliP->addToParseTree();
    } catch (std::logic_error const& err) {
        cout << "Cli Err: " << err.what() << endl;

        /* 
         * To log Trace events/errors
         */
        traceP->error("Error: %s", err.what());
    }

    /**
     * To log a custom syslog based on some Events.
     **/
    traceP->syslog(NxTrace::EMERG, "Started App %s",   sdk->getAppName());

    /* 
     * To log Trace events/errors
     */
    traceP->event("Event: %s", "Created Custom CLis");

    /**
     * startEventLoop will block the thread until we break out
     * of it by calling stopEventLoop. This is required to
     * receive any NX-OS specific events. Without this, none of 
     * the NXSDK functionalities will work. 
     **/
    sdk->startEventLoop();

    traceP->event("Event: Killing the App, done graceful cleanup");

    /**
     * [Required] This will do graceful cleanup on application exit.
     **/
    delete sdk;

    exit(0);
}
