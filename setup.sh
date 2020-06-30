#!/bin/bash

update_git=0
while getopts :uh flag
do
    case "${flag}" in
       u) update_git=1;;
       h)
          echo "Usage:"
          echo "    setup.sh -h    Display this help message."
          echo "    setup.sh -u    Does a git clean & stash and does a fresh git pull."
          exit 1
       ;;
       \?)
         echo "Invalid option: -$OPTARG. Use ./setup.sh -h for Usage" >&2
         exit 1
    esac
done

NXSDK_ROOT=/NX-SDK
if [ "$update_git" = "1" ]; 
then
   echo "----- Updating repo ------"
   cd ${NXSDK_ROOT}
   echo "=> git stash" 
   git clean  -d  -f .
   git stash
   echo "=> git pull"
   git pull
fi

if [ "${ENXOS_SDK_ROOT}" = "" ]; then 
   if [	-d "/enxos-sdk" ]; then
      ENXOS_SDK_ROOT=/enxos-sdk  	
      export ENXOS_SDK_ROOT=${ENXOS_SDK_ROOT}
   fi    
fi

if [ "${ENXOS_SDK_ROOT}" = "" ]; then
   echo "------ Setting up env for NX-SDK app to run remotely ------"
   echo "------ setting up python env for NX-SDK --------"
   echo "=> copying python bindings"
   cp python/_nx_sdk_py.so /usr/lib/python2.7/lib-dynload/_nx_sdk_py.so
   cp python/nx_sdk_py.py /usr/local/lib/python2.7/dist-packages/nx_sdk_py.py
else
   echo "------ Setting up env for NX-SDK app to run inside NX-OS ------"
   echo "=> export ENXOS_SDK_ROOT=${ENXOS_SDK_ROOT}"
   echo "=> source ${ENXOS_SDK_ROOT}/environment-setup-x86-wrsmllib32-linux" 
   source ${ENXOS_SDK_ROOT}/environment-setup-x86-wrsmllib32-linux    
fi
echo "------ Done ------"
