/*
 * Contains GO NX-SDK Callback Wrappers. It is located at ${NXSDK_ROOT}/go/nx_sdk_go_cb.go
 * 
 * GO NX-SDK callback wrappers are simplified for GO application developers hence only the
 * respective PostCb functions need to be extended by them. Please copy nx_sdk_go_cb.go to 
 * the directory that has your go code extending NX-SDK PostCb functions. 
 *
 * If you are doing "make" to build your applications then Makefile recipes takes care of symlinking to 
 * nx_sdk_go_cb.go in your project automatically. Hence nothing to be done when doing "make".
 *
 * Refer to Makefile on how the sample GO applications ($NXSDK_ROOT/examples/go/src) are built.
 */
 
package main

import (
     "nx_sdk_go"
)

// NOTE: This code to handle class inheritance was modeled on the SWIG code
// in Section 23.4.7 (Go Director Classes) in the SWIG 3.0 document at
// http://www.swig.org/Doc3.0/SWIGDocumentation.pdf

// NxDmeMgrHandler Callback interface
type NxDmeMgrHandlerGo interface {
    nx_sdk_go.NxDmeMgrHandler
    deleteNxDmeMgrHandler()
    IsNxDmeMgrHandlerGo()
}

type nxNxDmeMgrHandlerGo struct {
    nx_sdk_go.NxDmeMgrHandler
}

func (nchgs *nxNxDmeMgrHandlerGo) deleteNxDmeMgrHandler() {
    nx_sdk_go.DeleteDirectorNxDmeMgrHandler(nchgs.NxDmeMgrHandler)
}

func (nchgs *nxNxDmeMgrHandlerGo) IsNxDmeMgrHandlerGo() {}

type NxDmeMgrHandlerGoFunc struct {
    nch nx_sdk_go.NxDmeMgrHandler
}

func NewNxDmeMgrHandlerGo() NxDmeMgrHandlerGo {
    om := &NxDmeMgrHandlerGoFunc{}
    nch := nx_sdk_go.NewDirectorNxDmeMgrHandler(om)
    om.nch = nch
    nchgs := &nxNxDmeMgrHandlerGo{ NxDmeMgrHandler: nch }
    return nchgs
}

func DeleteNxDmeMgrHandlerGo(nchg NxDmeMgrHandlerGo) {
    nchg.deleteNxDmeMgrHandler()
}

// NxCmdHandler Callback interface
type NxCmdHandlerGo interface {
    nx_sdk_go.NxCmdHandler
    deleteNxCmdHandler()
    IsNxCmdHandlerGo()
}

type nxNxCmdHandlerGo struct {
    nx_sdk_go.NxCmdHandler
}

func (nchgs *nxNxCmdHandlerGo) deleteNxCmdHandler() {
    nx_sdk_go.DeleteDirectorNxCmdHandler(nchgs.NxCmdHandler)
}

func (nchgs *nxNxCmdHandlerGo) IsNxCmdHandlerGo() {}

type NxCmdHandlerGoFunc struct {
    nch nx_sdk_go.NxCmdHandler
}

func NewNxCmdHandlerGo() NxCmdHandlerGo {
    om := &NxCmdHandlerGoFunc{}
    nch := nx_sdk_go.NewDirectorNxCmdHandler(om)
    om.nch = nch
    nchgs := &nxNxCmdHandlerGo{ NxCmdHandler: nch }
    return nchgs
}

func DeleteNxCmdHandlerGo(nchg NxCmdHandlerGo) {
    nchg.deleteNxCmdHandler()
}

// NxIntfMgrHandler Callback interface
type NxIntfMgrHandlerGo interface {
    nx_sdk_go.NxIntfMgrHandler
    deleteNxIntfMgrHandler()
    IsNxIntfMgrHandlerGo()
}

type nxNxIntfMgrHandlerGo struct {
    nx_sdk_go.NxIntfMgrHandler
}

func (nchgs *nxNxIntfMgrHandlerGo) deleteNxIntfMgrHandler() {
    nx_sdk_go.DeleteDirectorNxIntfMgrHandler(nchgs.NxIntfMgrHandler)
}

func (nchgs *nxNxIntfMgrHandlerGo) IsNxIntfMgrHandlerGo() {}

type NxIntfMgrHandlerGoFunc struct {
    nch nx_sdk_go.NxIntfMgrHandler
}

func NewNxIntfMgrHandlerGo() NxIntfMgrHandlerGo {
    om := &NxIntfMgrHandlerGoFunc{}
    nch := nx_sdk_go.NewDirectorNxIntfMgrHandler(om)
    om.nch = nch
    nchgs := &nxNxIntfMgrHandlerGo{ NxIntfMgrHandler: nch }
    return nchgs
}

func DeleteNxIntfMgrHandlerGo(nchg NxIntfMgrHandlerGo) {
    nchg.deleteNxIntfMgrHandler()
}

// NxRibMgrHandler Callback interface
type NxRibMgrHandlerGo interface {
    nx_sdk_go.NxRibMgrHandler
    deleteNxRibMgrHandler()
    IsNxRibMgrHandlerGo()
}

type nxNxRibMgrHandlerGo struct {
    nx_sdk_go.NxRibMgrHandler
}

func (nchgs *nxNxRibMgrHandlerGo) deleteNxRibMgrHandler() {
    nx_sdk_go.DeleteDirectorNxRibMgrHandler(nchgs.NxRibMgrHandler)
}

func (nchgs *nxNxRibMgrHandlerGo) IsNxRibMgrHandlerGo() {}

type NxRibMgrHandlerGoFunc struct {
    nch nx_sdk_go.NxRibMgrHandler
}

func NewNxRibMgrHandlerGo() NxRibMgrHandlerGo {
    om := &NxRibMgrHandlerGoFunc{}
    nch := nx_sdk_go.NewDirectorNxRibMgrHandler(om)
    om.nch = nch
    nchgs := &nxNxRibMgrHandlerGo{ NxRibMgrHandler: nch }
    return nchgs
}

func DeleteNxRibMgrHandlerGo(nchg NxRibMgrHandlerGo) {
    nchg.deleteNxRibMgrHandler()
}

// NxAdjMgrHandler Callback interface
type NxAdjMgrHandlerGo interface {
    nx_sdk_go.NxAdjMgrHandler
    deleteNxAdjMgrHandler()
    IsNxAdjMgrHandlerGo()
}

type nxNxAdjMgrHandlerGo struct {
    nx_sdk_go.NxAdjMgrHandler
}

func (nchgs *nxNxAdjMgrHandlerGo) deleteNxAdjMgrHandler() {
    nx_sdk_go.DeleteDirectorNxAdjMgrHandler(nchgs.NxAdjMgrHandler)
}

func (nchgs *nxNxAdjMgrHandlerGo) IsNxAdjMgrHandlerGo() {}

type NxAdjMgrHandlerGoFunc struct {
    nch nx_sdk_go.NxAdjMgrHandler
}

func NewNxAdjMgrHandlerGo() NxAdjMgrHandlerGo {
    om := &NxAdjMgrHandlerGoFunc{}
    nch := nx_sdk_go.NewDirectorNxAdjMgrHandler(om)
    om.nch = nch
    nchgs := &nxNxAdjMgrHandlerGo{ NxAdjMgrHandler: nch }
    return nchgs
}

func DeleteNxAdjMgrHandlerGo(nchg NxAdjMgrHandlerGo) {
    nchg.deleteNxAdjMgrHandler()
}

// NxMacMgrHandler Callback interface
type NxMacMgrHandlerGo interface {
    nx_sdk_go.NxMacMgrHandler
    deleteNxMacMgrHandler()
    IsNxMacMgrHandlerGo()
}

type nxNxMacMgrHandlerGo struct {
    nx_sdk_go.NxMacMgrHandler
}

func (nchgs *nxNxMacMgrHandlerGo) deleteNxMacMgrHandler() {
    nx_sdk_go.DeleteDirectorNxMacMgrHandler(nchgs.NxMacMgrHandler)
}

func (nchgs *nxNxMacMgrHandlerGo) IsNxMacMgrHandlerGo() {}

type NxMacMgrHandlerGoFunc struct {
    nch nx_sdk_go.NxMacMgrHandler
}

func NewNxMacMgrHandlerGo() NxMacMgrHandlerGo {
    om := &NxMacMgrHandlerGoFunc{}
    nch := nx_sdk_go.NewDirectorNxMacMgrHandler(om)
    om.nch = nch
    nchgs := &nxNxMacMgrHandlerGo{ NxMacMgrHandler: nch }
    return nchgs
}

func DeleteNxMacMgrHandlerGo(nchg NxMacMgrHandlerGo) {
    nchg.deleteNxMacMgrHandler()
}