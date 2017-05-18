/*********************************************************************
 *
 * File:   testCli.cc
 *
 * Description:
 *
 *
 * Copyright (c) 2016 by cisco Systems, Inc.
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

class myCmdHandler : public NxCmdHandler {
public:
    myCmdHandler () {}
    void postCliCb(NxCliCmd *cmd) {
         string cmd_name = cmd->getCmdName();
         string cli_str = cmd->getCmdLineStr();
         cout << "cmdName: " << cmd_name << "..Entered cmd is \"" << cli_str << "\"" << endl;
         if (cmd_name.find("super_bgp_cmd") != std::string::npos) {
             cmd->printConsole("\n#####################################################");
             cmd->printConsole("\nI added this super bgp command");
             cmd->printConsole("\n#####################################################");
         } else if (cmd_name.find("conf1_cmd") != std::string::npos) {
             cmd->printConsole("\n#####################################################");
             cmd->printConsole("\nI added this conf1 cmd");
             cmd->printConsole("\n#####################################################");
         } 
    }
};

int  main (int argc, char **argv)
{
   NxCmdHandler *cmdHandler = NULL;
   NxSdk *sdk = nxos::NxSdk::getSdkInst(argc, argv);
   if (!sdk) {
       cout << "SDK Is NULL" << endl;
       return -1;
   }
  
   /*
    * Create the Base CLI Object.
    * This will be a singleton Object.
    */
    NxCliParser *cliP = sdk->getCliParser();
    if (!cliP) {
        cout << "cli parser is null";
        return -1;
    }

    //Raise syslog
    NxTrace *traceP = sdk->getTracer();

    try {

       /*
        * Add Custom show commands 
        */
        cliP->newShowCmd("super_bgp_cmd", "super-bgp <idA>");

        /*
        * Add Custom show commands 
        */
        nxos::NxCliCmd *cmd =  cliP->newConfigCmd("conf1_cmd", 
                                                  "A <id1> {B | {C <id2>}+}");
        cmd->updateKeyword("A", "A is a Keyword");
        cmd->updateParam("<id1>", "As id");
        cmd->updateKeyword("B", "B is a Keyword");
        cmd->updateKeyword("C", "C is a Keyword");
        cmd->updateParam("<id2>", "Cs id", 200, NULL, true, 3);

        //cmd->updateParam()
        /*
         * Add the command callback Handler.
         */
        cmdHandler = new myCmdHandler();
        cliP->setCmdHandler(cmdHandler);

        /*
         * Add to custom configs to NX CLI Parse tree.
         */
        cliP->addToParseTree();
    } catch (string err) {
        cout << "Cli Err: " << err << endl;
        traceP->error("Error: %s", err.c_str());
    }

    traceP->syslog(NxTrace::EMERG, "Hello world. This is APP %s",   sdk->getAppName());

    //Trace events/errors
    traceP->event("Event: %s", "Hello world");
    traceP->error("Error: %s", "Hello world");

    sdk->startEventLoop();

    delete sdk;
    return 0;
}
