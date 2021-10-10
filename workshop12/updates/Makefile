# check that we are on a 64-bit OS
ifneq (64,$(shell getconf LONG_BIT))
    $(error CSTools will only run on a 64-bit Linux operating system)
endif

# look for a suitable compiler to use if CSCOMPILER is not already set
# if CSCOMPILER is set, just trust it
ifeq (x$(CSCOMPILER),x)
    Compiler=$(shell bash bin/Compiler-check.bash)
    ifeq (,$(Compiler))
        $(info Please set environment variable CSCOMPILER to the name of your C++ compiler.) $(error Cannot find a suitable C++ compiler.)
    else
        export CXX=$(Compiler)
    endif
else
    $(info CSCOMPILER is set to $(CXX))
    export CXX=$(CSCOMPILER)
endif

# set CS_ARCH for the OS are we running on
uname:=$(wildcard /usr/bin/uname /bin/uname)
System:=$(shell test -f $(uname) && $(uname) -s)
Machine:=$(shell test -f $(uname) && $(uname) -m)

# MacOS is reported as Darwin
ifeq (Darwin,$(System))
    export CS_ARCH=macos
    export MACOSX_DEPLOYMENT_TARGET=10.12
    export MACOS_ARCHS=$(shell bash bin/Compiler-archs.bash)
else
	THIS_OS:=$(shell test -f /etc/os-release && (grep "^ID=" /etc/os-release | sed 's/ID="\([^"]*\).*/\1/'))
	THIS_OSV:=$(shell test -f /etc/os-release && (grep "^VERSION_ID=" /etc/os-release | sed 's/VERSION_ID="\([^\."]*\).*/\1/'))

    # CATS is currently RHEL7.7
    ifeq (rhel7,$(THIS_OS)$(THIS_OSV))
        export CS_ARCH=cats

    # CentOS7 is close enough RHEL7
    else ifeq (centos7,$(THIS_OS)$(THIS_OSV))
        export CS_ARCH=cats

    # assume everything else is equiv to CentOS 8 for now
    else
        export CS_ARCH=centos8
    endif
endif

# C++ 17 just in case
CXXFLAGS=--std=c++17 -I. -Iincludes -Wall ${MACOS_ARCHS}
CXXSFLAGS=--std=c++17 -I. -Iincludes -Wall
CS_CPU=$(CS_ARCH)-$(Machine)

# capture goal1 then make the remainder PHONY and depend on donothing
goal1:=$(firstword $(MAKECMDGOALS))
unique_goals:=$(filter-out $(goal1),$(sort $(MAKECMDGOALS)))
ifneq ($(words $(unique_goals)),0)
    .PHONY: $(unique_goals)
    $(eval $(unique_goals):donothing ;@true)
endif

# default target is all
all:=loop-O0 loop-O1 loop-O3
all: $(addprefix lib/$(CS_ARCH)/,$(all)) $(addsuffix -$(CS_CPU).s,$(all))

less Less live notest show Show regenerate test working: all
	@true

lib/$(CS_ARCH)/loop-O0: main.cpp loop.cpp
	@echo "Compiling with no optimisation"
	${CXX} ${CXXFLAGS} -O0 -o $@ $^

loop-O0-$(CS_CPU).s: loop.cpp
	@echo "Assembler with no optimisation"
	${CXX} ${CXXSFLAGS} -O0 -S loop.cpp
	mv loop.s loop-O0-$(CS_CPU).s

lib/$(CS_ARCH)/loop-O1: main.cpp loop.cpp
	@echo "Compiling with level-1 optimisations"
	${CXX} ${CXXFLAGS} -O1 -o $@ $^

loop-O1-$(CS_CPU).s: loop.cpp
	@echo "Assembler with level-1 optimisations"
	${CXX} ${CXXSFLAGS} -O1 -S loop.cpp
	mv loop.s loop-O1-$(CS_CPU).s

lib/$(CS_ARCH)/loop-O3: main.cpp loop.cpp
	@echo "Compiling with level-3 optimisations"
	${CXX} ${CXXFLAGS} -O3 -o $@ $^

loop-O3-$(CS_CPU).s: loop.cpp
	@echo "Assembler with level-3 optimisations"
	${CXX} ${CXXSFLAGS} -O3 -S loop.cpp
	mv loop.s loop-O3-$(CS_CPU).s

clean:
	rm -f lib/*/loop-O? loop-O?-*.s

# donothing
donothing:
	@true

