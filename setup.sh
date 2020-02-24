#!/bin/sh

NXSDK_ROOT=/NX-SDK
echo "----- Updating repo ------"
cd ${NXSDK_ROOT}
echo "=> git stash" 
git stash
echo "=> git pull"
git pull

echo "------ setting up python env for NX-SDK --------"
echo "=> copying python bindings"
cp python/_nx_sdk_py.so /usr/lib/python2.7/lib-dynload/_nx_sdk_py.so
cp python/nx_sdk_py.py /usr/local/lib/python2.7/dist-packages/nx_sdk_py.py

echo "------ Done ------"
