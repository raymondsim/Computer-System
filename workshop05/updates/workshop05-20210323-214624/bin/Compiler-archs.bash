#!/bin/bash

# on MacOS check which architectures are supported, x86 or x86 and arm?

# what kind of system - it must be Darwin
System=`test -f /usr/bin/uname && /usr/bin/uname -s`
if [ ${#System} -eq 0 ] ; then exit 0 ; fi
if [ "${System}" != "Darwin" ] ; then exit 0 ; fi

# we need an SDK Path
SDKPath=`test -x /usr/bin/xcrun && /usr/bin/xcrun --sdk macosx --show-sdk-path`
if [ ${#SDKPath} -eq 0 ] ; then exit 0 ; fi

# check we have a JSON setting file
json="${SDKPath}/SDKSettings.json"
if [ ! -f "${json}" ] ; then exit 0 ; fi

# check we have the architecture checker
if [ ! -x lib/macos/bin/xcode_archs ] ; then exit 0 ; fi

lib/macos/bin/xcode_archs < "${json}"
