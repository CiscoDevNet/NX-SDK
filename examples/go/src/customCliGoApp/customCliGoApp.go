/***************************************************************************
 * File:   customCliGoApp
 *
 * Supported since NX-SDK v1.7.5
 *
 * Description:
 *    A Sample NX-SDK Go App to generate custom CLI of users choice
 *    along with respective callbacks showcasing the usage of
 *    CLI infrastructure of NX-OS SDK.
 *
 * Copyright (c) 2018 by cisco Systems, Inc.
 * All rights reserved.
 *
 *
 **************************************************************************/

package main

import (
     "nx_sdk_go"
     "os"
     "fmt"
     "strings"
)

/*
 * Global Variables.
 */
var cliP      nx_sdk_go.NxCliParser
var trace     nx_sdk_go.NxTrace
var threshold int

/*
 * Handle CLI Callback.
 */
func (om *NxCmdHandlerGoFunc) PostCliCb(cmd nx_sdk_go.NxCliCmd) bool {
    cmdname := cmd.GetCmdName()
    if strings.Contains(cmdname, "port_bw_util_cmd") {
       port := nx_sdk_go.Void_to_string(cmd.GetParamValue("<port>"))
       cmd.PrintConsole(fmt.Sprintf("Excecuted cmd <%s>, Port: %s", cmdname, port))
       exec_cmd := "show int " + port
       cmd.PrintConsole(fmt.Sprintf("%s", cliP.ExecShowCmd(exec_cmd, nx_sdk_go.R_JSON)))
    } else if strings.Contains(cmdname, "set_port_bw_threshold_cmd") {
       threshold = nx_sdk_go.Void_to_int(cmd.GetParamValue("<threshold>"))
       cmd.PrintConsole(fmt.Sprintf("Excecuted cmd <%s>, Threshold: %d", cmdname, threshold))
    } else if strings.Contains(cmdname, "get_port_bw_threshold_cmd") {
       cmd.PrintConsole(fmt.Sprintf("Excecuted cmd <%s>, Threshold is: <%d>", cmdname, threshold))
    }

    return true
}

func main() {

    sdk_obj := nx_sdk_go.NxSdkGetSdkInst(len(os.Args), os.Args, true)
    if sdk_obj == nil {
       os.Exit(1) 
    }
    defer nx_sdk_go.DeleteNxSdk(sdk_obj)

    trace = sdk_obj.GetTracer()
    if trace == nil {
       os.Exit(1) 
    }

    desc := "Sample Go application using to generae Custom CLI & Syslog"
    sdk_obj.SetAppDesc(desc)

    cliP = sdk_obj.GetCliParser()
    if cliP == nil {
       os.Exit(1)
    }

    /**
     * Construct  Custom show Port Bandwidth Utilization commands
     */
    cmd1 := cliP.NewShowCmd("port_bw_util_cmd", "port bw utilization [<port>]")
    cmd1.UpdateKeyword("port", "Port Information")
    cmd1.UpdateKeyword("bw", "Port Bandwidth Information")
    cmd1.UpdateKeyword("utilization", "Port BW utilization in (%)")
    cmd1.UpdateParam("<port>", "Optional Filter Port Ex) Ethernet1/1", nx_sdk_go.P_INTERFACE)

    /**
     * Construct custom set Port Bandwidth Threshold config command.
     * NOTE:
     * Since we have already updated the Keyword information for
     * "port" and "bw" we dont have to update for each and every cmd
     * as its information will be automatically picked up for
     * other commands.
     **/
    int_attr := nx_sdk_go.Cli_param_type_integer_attr{}

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
    int_attr.Min_val = 1
    int_attr.Max_val = 100
    cmd2 := cliP.NewConfigCmd("set_port_bw_threshold_cmd", "port bw threshold <threshold>");
    cmd2.UpdateKeyword("threshold", "Set Port BandWidth Threshold Alert");
    cmd2.UpdateParam("<threshold>", "Threshold Limit. Default 50%", nx_sdk_go.P_INTEGER, &int_attr, int_attr.Len())

    /**
     * Construct Custom show Port Bandwidth Utilization threshold commands
     * Since the keywords information are already updated we dont have
     * to do it for each and every keyword and parameters.
     */
    cliP.NewShowCmd("get_port_bw_threshold_cmd", "port bw threshold");

    /*
     * Set Cmd Handler.
     */
    var cmdHandler = NewNxCmdHandlerGo()
    cliP.SetCmdHandler(cmdHandler)

    // Add the custom commands to NXOS parse tree.
    cliP.AddToParseTree()

    /**
     * To log a custom syslog based on some Events.
     **/
    trace.Syslog(nx_sdk_go.NxTraceINFO, fmt.Sprintf("Started App <%s>", sdk_obj.GetAppName()))

    /* 
     * To log Trace events/errors
     */
    trace.Event("Event: Created Custom CLis");

    sdk_obj.StartEventLoop()
}
