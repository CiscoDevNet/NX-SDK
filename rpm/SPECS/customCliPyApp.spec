 
### To build an RPM package for a custom App using NX-SDK 
### Just update the following information.

### Name of the binary built for the Application
%define APP_NAME      customCliPyApp

### Description of the Applicaiton
%define APP_DESC      "Create Custom/Sample template Cli Python Application RPM package"

### Path where nxsdk is git cloned to. (path/NX-SDK). Default location is /Nx-SDK
%define NXSDK_ROOT    /NX-SDK

### Source of the Application
%define APP_SOURCE    %{NXSDK_ROOT}/python/examples

### Set the path where Applciation target will be built or present.
%define APP_TARGET    %{APP_SOURCE}/%{APP_NAME}

### Update the RELEASE_VER to the release of nxos you are using.
### Current release is to Freeport (7.0.3.I6.1)
%define RELEASE_VER   7.0.3.I6.1

#####################################################################
########### Do not update beyond this point.#########################
#####################################################################
%define TARGET_DIR    /isan/bin
%define CURR_DIR      %(pwd)

Summary: Custom Application
Name: %{APP_NAME}
Version: 1.0
Release: %{RELEASE_VER}
Group: Development/Tools
License: Propreitary
URL: None

##Source: %{APP_SOURCE}
BuildRoot: %{NXSDK_ROOT}
 
%description
%{APP_DESC}
 
%prep
 
%build
 
%install
rm -rf "$RPM_BUILD_ROOT/%{TARGET_DIR}"
mkdir -p "$RPM_BUILD_ROOT/%{TARGET_DIR}"
cp -R %{APP_TARGET} "$RPM_BUILD_ROOT/%{TARGET_DIR}"
 
%clean
rm -rf "$RPM_BUILD_ROOT/%{TARGET_DIR}"
 
%files
%{TARGET_DIR}
 
%changelog
