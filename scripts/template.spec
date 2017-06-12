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
### If needed you can make the application here as well.
#cd $NXSDK_ROOT
#%make clean
#%make all
#cd $CURR_DIR
 
%install
rm -rf "$RPM_BUILD_ROOT/%{TARGET_DIR}"
mkdir -p "$RPM_BUILD_ROOT/%{TARGET_DIR}"
cp -R %{APP_TARGET} "$RPM_BUILD_ROOT/%{TARGET_DIR}"
 
%clean
rm -rf "$RPM_BUILD_ROOT/%{TARGET_DIR}"
 
%files
%{TARGET_DIR}
 
%changelog
