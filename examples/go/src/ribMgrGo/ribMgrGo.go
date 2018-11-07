/***************************************************************************
 * File:   ribMgrGo
 *
 * Supported since NX-SDK v1.7.5
 *
 * Description:
 *    A Sample NX-SDK Go App on how to register with NX-SDK Rib Mgr
 *    for route updates.
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
     "strconv"
)

/*
 * Global Variables.
 */
var cliP      nx_sdk_go.NxCliParser
var trace     nx_sdk_go.NxTrace
var ribMgr    nx_sdk_go.NxRibMgr
var threshold int

/*
 * Handle RibMgr Callback.
 */
func (om *NxRibMgrHandlerGoFunc) PostL3RouteCb(nxRoute nx_sdk_go.NxL3Route) bool {
    trace.Event(fmt.Sprintf("[%s] %s Event: %d, NH count %d", nxRoute.GetVrfName(), nxRoute.GetAddress(), nxRoute.GetEvent(), nxRoute.GetL3NextHopCount()))
    return true
}

/*
 * Handle CLI Callback.
 */
func (om *NxCmdHandlerGoFunc) PostCliCb(cmd nx_sdk_go.NxCliCmd) bool {
    cmdname := cmd.GetCmdName()

    // Get the Vrf_name
    vrf_name := nx_sdk_go.Void_to_string(cmd.GetParamValue("<vrf-name>"))
    if vrf_name == "" {
       vrf_name = "default"
    }

    // Get the interested Address Family to get the route events from.
    af := nx_sdk_go.MAX_AF
    if cmd.IsKeywordSet("ipv4") {
       af = nx_sdk_go.AF_IPV4
    } else if cmd.IsKeywordSet("ipv6") {
       af = nx_sdk_go.AF_IPV6
    }

    if strings.Contains(cmdname, "watch_route_cmd") {
       nocmd := ""
       if cmd.IsKeywordSet("no") {
          nocmd = "No"
       }

       // Get the tag associated with the owner for BGP ex) asn is the tag
       protocol := nx_sdk_go.Void_to_string(cmd.GetParamValue("<protocol>"))
       tag      := nx_sdk_go.Void_to_string(cmd.GetParamValue("<tag>"))

       cmd.PrintConsole(fmt.Sprintf("%s Redistributing %d Routes from %s tag %s in VRF %s",
                                    nocmd, int(af), protocol, tag, vrf_name))
       if nocmd != "" {
          ribMgr.UnwatchL3Route(protocol, tag, vrf_name, af)
       } else {
          ribMgr.WatchL3Route(protocol, tag, vrf_name, af)
       }
    } else if strings.Contains(cmdname, "route_cmd") {
       var route nx_sdk_go.NxL3Route
 
       if strings.Compare(vrf_name, "all") == 0 {
          cmd.PrintConsole("Cannot do getRoute on all Vrfs!!\n")
          return true
       }

       // Get the Ipaddress and mask_len
       pfx := ""
       if af == nx_sdk_go.AF_IPV4 {
          pfx = nx_sdk_go.Void_to_string(cmd.GetParamValue("<ippfx>"))
       } else if af == nx_sdk_go.AF_IPV6 {
          pfx = nx_sdk_go.Void_to_string(cmd.GetParamValue("<ipv6pfx>"))
       }
   
       ip_addr  := strings.Split(pfx, "/")[0]
       mask_len,err := strconv.Atoi(strings.Split(pfx, "/")[1])
       if err != nil {
          cmd.PrintConsole("Err: Invalid Mask Len")
          return true
       }
       cmd.PrintConsole(fmt.Sprintf("Get Route %s/%d in VRF %s\n", ip_addr, mask_len, vrf_name))
       route = ribMgr.GetL3Route(ip_addr, byte(mask_len), vrf_name)
       if route.Swigcptr() != 0 {
          cmd.PrintConsole(fmt.Sprintf("addr: %s\n", route.GetAddress()))
          cnt := route.GetL3NextHopCount()
          cmd.PrintConsole(fmt.Sprintf("Nexthop Count: %d\n", cnt))
          nh := route.GetL3NextHop(true)
          var index uint
          index = 0
          for index < cnt {
              index++
              cmd.PrintConsole(fmt.Sprintf("NH address: %s\n", nh.GetAddress()))
              cmd.PrintConsole(fmt.Sprintf("NH owner: %s\n", nh.GetOwner()))
              cmd.PrintConsole(fmt.Sprintf("NH vrfname: %s\n", nh.GetVrfName()))
              cmd.PrintConsole(fmt.Sprintf("NH OutIntf: %s\n", nh.GetOutInterface()))
              nh = route.GetL3NextHop()
          }
       } else {
          cmd.PrintConsole("Route not found!!!\n")     
       }
    } 

    return true
}

func main() {

    sdk_obj := nx_sdk_go.NxSdkGetSdkInst(len(os.Args), os.Args, true)
    if sdk_obj.Swigcptr() == 0 {
       os.Exit(1) 
    }
    defer nx_sdk_go.DeleteNxSdk(sdk_obj)

    trace = sdk_obj.GetTracer()
    if trace.Swigcptr() == 0 {
       os.Exit(1) 
    }

    desc := "Sample Go application using to generae Custom CLI & Syslog"
    sdk_obj.SetAppDesc(desc)

    cliP = sdk_obj.GetCliParser()
    if cliP.Swigcptr() == 0 {
       os.Exit(1)
    }

    /**
     * Construct  Custom watch route command
     */
    watch_cmd := cliP.NewConfigCmd("watch_route_cmd", "watch owner <protocol> [<tag>] [ipv4 | ipv6] [vrf <vrf-name>]")
    watch_cmd.UpdateKeyword("watch", "Watch on a object")
    watch_cmd.UpdateKeyword("owner", "Owner of the route")
    watch_cmd.UpdateParam("<protocol>", "Owner of the Routes ex bgp, isis, ospf", nx_sdk_go.P_STRING, uintptr(0), 0, true)
    watch_cmd.UpdateParam("<tag>", "Optional tag for a owner. Ex for BGP ASN is the tag, for ISIS the instance number", nx_sdk_go.P_STRING, uintptr(0), 0, true)
    watch_cmd.UpdateKeyword("ipv4", "Redistribute IP Routes from  protocol. Default is Both")
    watch_cmd.UpdateKeyword("ipv6", "Redistribute IPv6 Routes from the protocol. Default is Both")
    watch_cmd.UpdateKeyword("vrf", "VRF Information")
    watch_cmd.UpdateParam("<vrf-name>", "VRF Name. By default vrf name is default", nx_sdk_go.P_VRF)

    /**
     * Construct  Custom show route command
     */
    route_cmd := cliP.NewShowCmd("route_cmd", "route {ipv4 <ippfx> | ipv6 <ipv6pfx>} [vrf <vrf-name>]")
    route_cmd.UpdateKeyword("route", "Routes in Rib")
    ip_attr := nx_sdk_go.Cli_param_type_ip_addr_attr{}
    ip_attr.Is_prefix = true
    route_cmd.UpdateParam("<ippfx>", "Ip Prefix", nx_sdk_go.P_IP_ADDR, &ip_attr, ip_attr.Len())
    ip_attr.Is_ipv6 = true
    route_cmd.UpdateParam("<ipv6pfx>", "Ipv6 Prefix", nx_sdk_go.P_IP_ADDR, &ip_attr, ip_attr.Len())

    /*
     * Set Cmd Handler.
     */
    var cmdHandler = NewNxCmdHandlerGo()
    cliP.SetCmdHandler(cmdHandler)

    // Add the custom commands to NXOS parse tree.
    cliP.AddToParseTree()

    // Registering for rib Events
    ribMgr = sdk_obj.GetRibMgr()
    if ribMgr.Swigcptr() != 0 {
       var myribcb = NewNxRibMgrHandlerGo()
       ribMgr.SetRibMgrHandler(myribcb)
    } else {
       trace.Error("RibMgr Init failed")
    }

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
