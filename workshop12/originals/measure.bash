#!/bin/bash

# run the loop programs in different configurations
# output should look like a CSV file

# Banner
echo "Program,Iterations,Size,Direction,Time,Million Reads / Second,Hostname,CPU,Cores/Threads,L1i Cache,L1d Cache,L2 Cache,L3 Cache,Memory"

# what kind of system are we running on?
# MacOS is reported as Darwin
System=`test -f /usr/bin/uname && /usr/bin/uname -s`
if [ "x${System}" == "xDarwin" ]
then
    export CS_ARCH=macos
    export MACOSX_DEPLOYMENT_TARGET=10.12
    export MACOS_ARCHS=$(bash bin/Compiler-archs.bash)
else
    THIS_OS=$(test -f /etc/os-release && (grep "^ID=" /etc/os-release | sed 's/ID="\([^"]*\).*/\1/'))
    THIS_OSV=$(test -f /etc/os-release && (grep "^VERSION_ID=" /etc/os-release | sed 's/VERSION_ID="\([^\."]*\).*/\1/'))

    # CATS is currently RHEL7.7
    if [ "xrhel7" == "x${THIS_OS}${THIS_OSV}" -o "xcentos7" == "x${THIS_OS}${THIS_OSV}" ]
    then
        export CS_ARCH=cats
    else
        export CS_ARCH=centos8
    fi
fi

# mem size
mem_size()
{
    memory_size=
    if [ ${#@} -eq 0 ] ; then return ; fi

    # extract any fractions?
    size="${1%%.*}"
    fraction=0
    divider=1

    # if we have a fraction, remove by scaling
    if [ "x${size}" != "x${1}" ]
    then
        fraction="${1#*.}"
        fdigits=${#fraction}
        divider=1
        if [ ${fdigits} -gt 0 ]
        then
            while ((fdigits-->0)) ; do
                ((divider*=10))
                ((size*=10))
            done
            ((size+=fraction))
        fi
    fi

    # convert to bytes, MiB scale tries to avoid overflow
    case "${2}" in
    kB | KiB)
        ((size=size*1024))
        ((size=size/divider))
        ;;
    MiB)
        ((size=size*1024))
        ((size=size/divider))
        ((size=size*1024))
        ;;
    *)
        ;;
    esac

    if [ ${#size} -eq 0 -o "${size}" -eq 0 ] ; then return ; fi

    ((KB=size/1024))
    ((MB=KB/1024))
    ((Mremainder=KB-MB*1024))
    ((GB=MB/1024))
    ((Gremainder=MB-GB*1024))

    if [ "${GB}" -gt 0 ]
    then
        if [ "${Gremainder}" -eq 0 ]
        then
            memory_size="${GB}GB"
        else
            memory_size="${MB}MB"
        fi

    elif [ "${MB}" -gt 0 -a "${Mremainder}" -eq 0 ]
    then
        memory_size="${MB}MB"
    else
        memory_size="${KB}KB"
    fi
}

# lookup CPU info
host=$(hostname)
if [ -f /proc/cpuinfo ] ; then
    cpu=$(grep -i "model name" /proc/cpuinfo 2> /dev/null | sed 's/.*:  *//'| sort -u)
    cpus=$(lscpu 2> /dev/null |grep -i "socket(s):" | sed 's/.*:  *//')
    cpuc=$(lscpu 2> /dev/null |grep -i " per socket:" | sed 's/.*:  *//')
    cput=$(lscpu 2> /dev/null |grep -i " per core:" | sed 's/.*:  *//')
    L1i=$(lscpu 2> /dev/null |grep -i "l1i cache" | sed 's/.*:  *//')
    L1d=$(lscpu 2> /dev/null |grep -i "l1d cache" | sed 's/.*:  *//')
    L2=$(lscpu 2> /dev/null | grep -i "l2 cache" | sed 's/.*:  *//')
    L3=$(lscpu 2> /dev/null |grep -i "l3 cache" | sed 's/.*:  *//')
    mem=$(grep -i memtotal /proc/meminfo 2> /dev/null  | sed 's/.*:  *//')

    # cores is sockets x cores / socket
    ((cpuc=cpus*cpuc))

    # threads is cores x threads / core
    ((cput=cpuc*cput))
else
    cpu=$(sysctl -n machdep.cpu.brand_string 2> /dev/null)
    cpus=1
    cpuc=$(sysctl -n machdep.cpu.core_count 2> /dev/null)
    cput=$(sysctl -n machdep.cpu.thread_count 2> /dev/null)
    L1i=$(sysctl -n hw.l1icachesize 2> /dev/null)
    L1d=$(sysctl -n hw.l1dcachesize 2> /dev/null)
    L2=$(sysctl -n hw.l2cachesize 2> /dev/null)
    L3=$(sysctl -n hw.l3cachesize 2> /dev/null)
    mem=$(sysctl -n hw.memsize 2> /dev/null)
fi

# make memory sizes readable
mem_size ${L1i} ; L1i=${memory_size}
mem_size ${L1d} ; L1d=${memory_size}
mem_size ${L2} ; L2=${memory_size}
mem_size ${L3} ; L3=${memory_size}
mem_size ${mem} ; mem=${memory_size}

# measure program over size:iterations pairs
# dispay results with system info too
measure_program()
{
    # arg 1 is the program to run
    program="${1}"
    shift 1

    # for each pair of size:iterations run all 3 directions
    while [ ${#@} -gt 0 ] ; do
        size="${1%:*}"
        iterations="${1#*:}"
        shift 1

        # try all 3 directions
        for direction in asc dsc random ; do
            ${program} ${size} ${iterations} ${direction}
            echo "${host},${cpu},${cpuc}c/${cput}t,${L1i},${L1d},${L2},${L3},${mem}"
        done
    done
}

# PHP - probably very slow and big, 64-bit numbers (8 Bytes) so with overheads multiply data size by 64 Bytes
# trying 1K = 64KB RAM, 2K = 128KB RAM, 4K = 256KB RAM, 1M = 64MB RAM, 16M = 1GB RAM
measure_program "php loop.php" 1024:40000 2048:20000 4096:10000 1048576:30 16777216:2

# check we have compiled loop programs
if [ ! -x lib/${CS_ARCH}/loop-O0 ] ; then make ; fi

# C - 32-bit numbers (4 Bytes) so multiply data size by 8 Bytes
# trying 1K = 8KB, 4K = 32KB, 16K = 128KB, 1M = 8MB, 16M = 128MB, 256M = 2GB, 512M = 4GB
measure_program lib/${CS_ARCH}/loop-O0 1024:500000 4096:150000 16384:30000 1048576:500 16777216:30 268435456:2 536870912:1
measure_program lib/${CS_ARCH}/loop-O1 1024:3000000 4096:800000 16384:200000 1048576:3000 16777216:250 268435456:15 536870912:3
measure_program lib/${CS_ARCH}/loop-O3 1024:6000000 4096:1500000 16384:400000 1048576:5000 16777216:300 268435456:20 536870912:8
