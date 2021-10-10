#!/bin/bash

# stage 0 default installer for assignments and workshops
# builds generic assignment / workshop startup files


# The Plan:
# . installer.bash runs updater.bash from newest updates/YYMMDD-HHIISS-name.zip
#   . all updates/*zip must use the same assign or workshop name
#   . all output is logged in updates/log
# . updater.bash creates default directory structure and updates startup files
#   . the top level should be:
#     Makefile, bin, includes, lib, tests, toedit, updates
#   . top level also includes a copy of everything a student changes in toedit 
#     If missing from top level, copy from toedit
#     If present at top level, do nothing ...
# . updater.bash ensures all executables are executable, this may involve removing quarantine flags
# . a student svn directory must contain:
#   . at the top level
#     Makefile, updates, and any files students are asked to edit
#   . in the updates directory
#     log, installer.bash and *-*-*.zip files

# remove all access for others
chmod -R go= *

# setup colours for interative output
# if using an xterm - use headings and colour coded test results
if [ "${TERM:0:5}" == "xterm" -o "${TERM:0:2}" == "vt" ] ; then
    colorboldblack="\x1b[1;7;30m"
    colorboldgreen="\x1b[1;7;32m"
    colorboldred="\x1b[1;7;31m"
    colorboldmagenta="\x1b[1;7;35m"
    undocolor="\x1b[0m"
else
    colorboldblack=
    colorboldgreen=
    colorboldred=
    colorboldmagenta=
    undocolor=
fi

# what kind of system?
System=`test -x /usr/bin/uname && /usr/bin/uname -s`
if [ "${#System}" -eq 0 ] ; then System=Linux ; fi

# record start of this run - set timezone, students run this in other countries
now=`(export TZ=Australia/Adelaide ; date +%y%m%d-%H%M%S)`
now_s=`date +%s`
echo "${now},make,${@}" >> updates/log
chmod 600 updates/log

# reminder every time
reminders_exam()
{
    echo -e "${colorboldred}                                                                                ${undocolor}"
    echo -e "${colorboldred}         Make a web submission after completing each part of the exam!          ${undocolor}"
    echo -e "${colorboldred}                                                                                ${undocolor}"
    echo "${now_s}" > .reminders-svn
}

# reminder every hour - svn udpate / svn commit
reminders_svn()
{
    if [ -f .reminders-svn ]
    then
        last_reminder=`cat .reminders-svn`
        ((since=now_s-last_reminder))
        if [ "${since}" -lt 3600 ] ; then return ; fi
    fi

    echo -e "${colorboldgreen}                                                                                ${undocolor}"
    echo -e "${colorboldgreen}                   Every time you start work, run svn update                    ${undocolor}"
    echo -e "${colorboldgreen}                   Every time you stop work,  run svn commit                    ${undocolor}"
    echo -e "${colorboldgreen}                                                                                ${undocolor}"
    echo "${now_s}" > .reminders-svn
}

# reminder every 20 minutes - no logbook no marks
reminders_logbook()
{
    if [ -f .reminders-logbook ]
    then
        last_reminder=`cat .reminders-logbook`
        ((since=now_s-last_reminder))
        if [ "${since}" -lt 1200 ] ; then return ; fi
    fi

    echo -e "${colorboldred}                                                                                ${undocolor}"
    echo -e "${colorboldred}                       Remember to write in your LOGBOOK!                       ${undocolor}"
    echo -e "${colorboldred}                              No logbook, No marks!                             ${undocolor}"
    echo -e "${colorboldred}                                                                                ${undocolor}"
    echo "${now_s}" > .reminders-logbook
}

# reminder every 30 minutes - we don't fix startups everytime if nfs mounted
reminders_update()
{
    if [ -f .reminders-update ]
    then
        last_reminder=`cat .reminders-update`
        ((since=now_s-last_reminder))
        if [ "${since}" -lt 1800 ] ; then doupdate="no" ; return ; fi
    fi

    echo "${now_s}" > .reminders-update
    doupdate="yes"
}

# recommendation every 60 minutes - practise using logbook
recommendations_logbook()
{
    if [ -f .reminders-logbook ]
    then
        last_reminder=`cat .reminders-logbook`
        ((since=now_s-last_reminder))
        if [ "${since}" -lt 3600 ] ; then return ; fi
    fi

    echo -e "${colorboldgreen}                          Practise using your LOGBOOK!                          ${undocolor}"
    echo -e "${colorboldgreen}                                                                                ${undocolor}"
    echo "${now_s}" > .reminders-logbook
}

# we do not check on reminder_update here
reminders()
{
    if [ "${repo_assign}" != "${repo_assign##exam}" ]
    then
        reminders_exam
    elif [ "${repo_assign}" != "${repo_assign##assignment}" ]
    then
        reminders_svn
        reminders_logbook
    elif [ "${repo_assign}" != "${repo_assign##workshop}" -o "${repo_assign}" != "${repo_assign##project}" ]
    then
        reminders_svn
        recommendations_logbook
    fi
}

# nullglob is set so patterns can be empty
shopt -s nullglob

# create an updates/Makefile if we haven't got one
default_makefile()
{
    if [ ! -f updates/Makefile ]
    then
        echo ".PHONY: _ \$(MAKECMDGOALS)" > updates/Makefile
        echo "\$(eval \$(sort \$(MAKECMDGOALS)):;@:)" >> updates/Makefile
        echo "_:;@:" >> updates/Makefile
        chmod 600 updates/Makefile
    fi
}

# exit with error message - ensure we have a Makefile first ...
exit_with_error()
{
    echo "${now},error,\"${@}\"" >> updates/log
    echo "${@}"
    default_makefile
    rm -f .reminders-update
    exit -1
}

# tools we check - which, zip, unzip, diff, svn, cksum, java
# check tool, add to list if we cannot find it
declare -a bad_tools
check_tool()
{
    tool="${1}"
    which_tool=`/usr/bin/which "${tool}" 2> /dev/null`

    # giveup if which failed, result is "" or it is not executable
    if [ "$?" -ne 0 -o ${#which_tool} -eq 0 -o ! -x "${which_tool}" ]
    then
        bad_tools[${#bad_tools[@]}]="${tool}"
    fi
}

# check list of tools
check_tools()
{
    # check /usr/bin/which first, its a bit tricky without it
    if [ ! -x '/usr/bin/which' ]
    then
        exit_with_error "Cannot find /usr/bin/which - please install and try again"
    fi

    while [ "${#@}" -gt 0 ]
    do
        check_tool "${1}"
        shift 1
    done
}

# if any tools not found, write out list of bad tools and exit
report_bad_tools()
{
    if ((${#bad_tools[@]} > 0)) ; then
        echo "*** Error! The following command(s) cannot be found:"
        for ((i = 0 ; i < ${#bad_tools[@]} ; i++ )) ; do
            echo " .  ${bad_tools[$i]}"
        done
        exit_with_error "Please install them and try again."
    fi
}

# function to check C++ compiler and record compiler found
# sets compiler_found if given compiler is acceptable
compiler_found=
check_compiler()
{
    # give up if a compiler already found
    if ((${#compiler_found} > 0)) ; then return ; fi

    compiler="${1}"
    compiler=`/usr/bin/which "${compiler}" 2> /dev/null`

    # giveup if which failed, result is "" or it is not executable
    if [ "$?" -ne 0 -o ${#compiler} -eq 0 -o ! -x "${compiler}" ] ; then return ; fi

    # extract major version number
    version=`"${compiler}" --version | head -1 | cut -f "${version_field}" -d ' '| cut -f 1,1 -d '-'| cut -f 1 -d '.'`

    # if specified, check against minimum acceptable version
    min_version="${2}"
    if [ "${#version}" -gt 0 -a "${#min_version}" -gt 0 ]
    then
        if [ "${version}" -lt "${min_version}" ] ; then return ; fi
    fi

    # if still going, we found an acceptable compiler
    compiler_found="${compiler}"
}

# look for C++ compiler in preferred order ... currently:
# clang++ 9*, 11*, 10*, 8*, 7*, 6*
# g++ 8*, 10*, 9*, 7*
check_compilers()
{
    if [ "${System}" == "Darwin" ]
    then
        version_field=4
        # On a Mac just check clang++ min version is 10.0.0, prefer 11.0.3
        # 11.0.3 => clang 9.0.0
        # 11.0.0 => clang 8.0.0
        # 10.0.1 => clang 7.0.0
        # 10.0.0 => clang 6.0.0
        check_compiler `/usr/bin/which clang++ 2> /dev/null` 10
    else
        version_field=3

        # On our RHEL7 systems clang++-9 is in /opt/rh/llvm-toolset-9.0/root/usr/bin/clang++-9
        check_compiler clang++-9  9
        if [ "${#compiler_found}" -eq 0 -a -x "/opt/rh/llvm-toolset-9.0/root/usr/bin/clang++-9" ]
        then
            # PATH not set correctly to use our preferred compiler
            echo "*** Error! PATH does not include /opt/rh/llvm-toolset-9.0/root/usr/bin/clang++-9"
            echo "To use this C++ compiler please run this before trying again: source /opt/rh/llvm-toolset-9.0/enable"
        fi
        check_compiler clang++  9
        check_compiler clang++-11  11
        check_compiler clang++-10  10
        check_compiler clang++-8  8
        check_compiler clang++  8
        check_compiler clang++-7  7
        check_compiler clang++  7
        check_compiler clang++-6  6
        check_compiler clang++  6

        # On our RHEL7 systems g++-8 is in /opt/rh/devtoolset-8/root/usr/bin/g++-8
        check_compiler g++-8  8
        if [ "${#compiler_found}" -eq 0 -a -x "/opt/rh/devtoolset-8/root/usr/bin/g++-8" ]
        then
            if [ ! -x "/opt/rh/llvm-toolset-9.0/root/usr/bin/clang++-9" ]
            then
                # PATH not set correctly to use our preferred backup compiler
                echo "*** Error! PATH does not include /opt/rh/devtoolset-8/root/usr/bin/g++-8"
                echo "To use this C++ compiler please run this before trying again: source /opt/rh/devtoolset-8/enable"
            fi
        fi
        check_compiler g++  8
        check_compiler g++-10  10
        check_compiler g++-9  9
        check_compiler g++-7  7
        check_compiler g++  7
    fi

    if [ "${#compiler_found}" -eq 0 ]
    then
        bad_tools[${#bad_tools[@]}]="clang++-9, or clang++ version 6 or later, or g++ version 7 or later"
    fi
}

# check which svn repository directory this working copy belongs too
# give up if this is not a working copy!
### alternate for 2021? check ., .., ../.. and ../../.. before giving up
repouser=
repo_assign=
check_svn()
{
    repo=`svn info . 2> /dev/null | grep "^URL:"`
    if [ "${#repo}" -eq 0 ]
    then
        repo=`svn info .. 2> /dev/null | grep "^URL:"`
        if [ "${#repo}" -eq 0 ]
        then
            repo=`svn info ../.. 2> /dev/null | grep "^URL:"`
            if [ "${#repo}" -eq 0 ]
            then
                repo=`svn info ../../.. 2> /dev/null | grep "^URL:"`
                if [ "${#repo}" -eq 0 ]
                then
                    exit_with_error "`pwd` is not a working copy!"
                fi
            fi
        fi
    fi

    # remove server from URL, leaving user and path
    repo_path="${repo##URL: https://version-control.adelaide.edu.au/svn/}"
    if [ "${repo}" == "${repo_path}" ]
    then
        exit_with_error "Repository URL must start with: https://version-control.adelaide.edu.au/svn/"
    fi

    # extract user - everything up to first /
    repo_user="${repo_path%%/*}"
    repo_path="${repo_path#*/}"

    # skip a level if courseware
    if [ "x${repo_user}" == "xcourseware" ] ; then repo_path="${repo_path#*/}" ; fi

    # extract year - everything after the first /
    repo_year="${repo_path%%/*}"
    repo_path="${repo_path#*/}"

    # extract sem - everything after the first /
    repo_sem="${repo_path%%/*}"
    repo_path="${repo_path#*/}"

    # extract course - everything after the first /
    repo_course="${repo_path%%/*}"
    repo_path="${repo_path#*/}"

    # if course is Adelaide, change to cs
    if [ "x${repo_course}" == "xAdelaide" ] ; then repo_course="cs" ; fi

    # extract assign - everything after the first /
    repo_assign="${repo_path%%/*}"

    # if assign is zipped, skip one level to cs
    if [ "x${repo_assign}" == "xzipped" ] ; then repo_path="${repo_path#*/}" ; repo_assign="${repo_path%%/*}" ; fi

    repo_path="${repo_year}/${repo_sem}/${repo_course}/${repo_assign}"

    echo "${now},svn,\"$repo_user\",\"$repo_path\"" >> updates/log
}

add_to_svn()
{
    svn add --depth=empty "${1}" 2> /dev/null
}

declare -a svn_status
declare -a svn_not_found
declare -a svn_not_added
declare -a svn_not_committed
warn_svn_status()
{
    while [ ${#@} -gt 0 ]
    do
        ofile="${1}"
        file="${ofile##./originals/}"
        shift 1

        if [ "${file}" == "./originals" ] ; then continue ; fi

        if [ -f "${file}" -a "${ofile}" != "${file}" ]
        then
            svn_originals[${#svn_originals[@]}]="${file}"
        fi

        if [ ! -f "${file}" -a ! -d "${file}" ]
        then
            svn_not_found[${#svn_not_found[@]}]="${file}"
            continue
        fi

        stat=`svn status --depth=empty -v "${file}" 2> /dev/null`
        if [ ${#stat} -eq 0 ]
        then
            svn_not_added[${#svn_not_added[@]}]="${file}"
            continue
        fi

        # check status of
        case "${stat:0:1}" in
        " ")
            # nothing to report
            ;;
        "M" | "R")
            # nothing to report
            ;;
        "A")
            svn_not_committed[${#svn_not_committed[@]}]="${file}"
            ;;
        "I")
            svn_not_added[${#svn_not_added[@]}]="${file}"
            ;;
        "?")
            svn_not_added[${#svn_not_added[@]}]="${file}"
            ;;
        "!")
            svn_status[${#svn_status[@]}]="${file} is missing"
            ;;
        "D")
            svn_not_added[${#svn_not_added[@]}]="${file}"
            ;;
        "C")
            svn_status[${#svn_status[@]}]="${file} has conflicts to be resolved"
            ;;
        "X")
            svn_status[${#svn_status[@]}]="${file} is an external reference to another svn repository"
            ;;
        "~")
            svn_status[${#svn_status[@]}]="${file} has changed type"
            ;;
        *)
            # no idea - just ignore it ...
            ;;
        esac
    done
}

check_svn_originals()
{
    # Makefile, updates/installer.bash, updates/updater.bash and updates/log must all be in svn
    warn_svn_status . Makefile updates updates/installer.bash updates/updater.bash updates/log

    # the latest startups .zip should be in svn
    warn_svn_status "${update_zip}"

    # if originals extracted - check if they are in svn
    if [ -d ./originals ] ; then
        warn_svn_status `find ./originals -type d`
        warn_svn_status `find ./originals -type f`
    fi

    # warnings ...
    padding="                                                                                "
    displayed="no"
    blank="${colorboldmagenta}${padding}${undocolor}"
    banner="${colorboldmagenta}                                    WARNING                                     ${undocolor}"

    # files that do not exist
    if ((${#svn_not_found[@]} > 0)) ; then
        displayed="yes"
        echo -e "${banner}"

        echo -e "${colorboldmagenta}  The following files / directories could not be found:                         ${undocolor}"
        for ((i = 0 ; i < ${#svn_not_found[@]} ; i++ )) ; do
            display_line="    . ${svn_not_found[$i]}"
            echo -e "${colorboldmagenta}${display_line}${padding:${#display_line}}${undocolor}"
        done
        echo -e "${blank}"
        exit_with_error "Cannot find some of the required files / directories"
    fi

    # files not added to svn
    if ((${#svn_not_added[@]} > 0)) ; then
        echo -e "${colorboldmagenta}  The following files / directories have not been added to svn:                 ${undocolor}"
        for ((i = 0 ; i < ${#svn_not_added[@]} ; i++ )) ; do
            svn_not_committed[${#svn_not_committed[@]}]="${svn_not_added[$i]}"
            display_line="    . ${svn_not_added[$i]}"
            echo -e "${colorboldmagenta}${display_line}${padding:${#display_line}}${undocolor}"
        done

        echo -e "${colorboldmagenta} Running svn add                                                                ${undocolor}"
        for ((i = 0 ; i < ${#svn_not_added[@]} ; i++ )) ; do
            add_to_svn "${svn_not_added[$i]}"
        done
        echo -e "${blank}"
    fi

    # files not committed to svn
    if ((${#svn_not_committed[@]} > 0)) ; then
        if [ "x${displayed}" == "xno" ]
        then
            displayed="yes"
            echo -e "${blank}"
            echo -e "${banner}"
            echo -e "${blank}"
        fi

        echo -e "${colorboldmagenta}  The following files / directories have not been committed to svn:             ${undocolor}"
        for ((i = 0 ; i < ${#svn_not_committed[@]} ; i++ )) ; do
            display_line="    . ${svn_not_committed[$i]}"
            echo -e "${colorboldmagenta}${display_line}${padding:${#display_line}}${undocolor}"
        done
        echo -e "${blank}"
        echo -e "${colorboldmagenta} Running svn commit, you may be prompted for your password!                     ${undocolor}"
        svn commit -m "${repo_assign} startup files"
        echo -e "${blank}"
    fi

    # other stuff ...
    if ((${#svn_status[@]} > 0)) ; then
        if [ "x${displayed}" == "xno" ]
        then
            displayed="yes"
            echo -e "${blank}"
            echo -e "${banner}"
            echo -e "${blank}"
        fi

        echo -e "${colorboldmagenta}  The following svn issues need to be addressed:                                ${undocolor}"
        for ((i = 0 ; i < ${#svn_status[@]} ; i++ )) ; do
            display_line="    . ${svn_status[$i]}"
            echo -e "${colorboldmagenta}${display_line}${padding:${#display_line}}${undocolor}"
        done
        echo -e "${blank}"
    fi

    # now record svn diffs - when originals created above commit should mean empty diffs
    if ((${#svn_originals[@]} >0)) ; then
        for ((i = 0 ; i < ${#svn_originals[@]} ; i++ )) ; do
            svn cl originals "${svn_originals[$i]}" > /dev/null 2> /dev/null
        done
        echo -n "${now},svndiff," >> updates/log
        if [ "${System}" == "Darwin" ] ; then
            svn diff --cl originals -x "-w --ignore-eol-style" | zip - - 2> /dev/null | base64 >> updates/log
        else
            svn diff --cl originals -x "-w --ignore-eol-style" | zip - - 2> /dev/null | base64 --wrap=0 >> updates/log
            echo >> updates/log
        fi
        for ((i = 0 ; i < ${#svn_originals[@]} ; i++ )) ; do
            svn cl --remove "${svn_originals[$i]}" > /dev/null 2> /dev/null
        done
    fi
}

check_tools df zip unzip svn cksum diff grep make java find shasum base64
check_compilers
report_bad_tools
check_svn

#echo "Found C++ compiler: $compiler_found"

# do we have any update files?
update_zips=(updates/*-*-*.zip)
nzips=${#update_zips[@]}
((nzips--))

# give up if no update files
if [ "${#update_zips[@]}" -eq 0 ]
then
    exit_with_error "Cannot find any zip files in updates/"
fi

# name of newest update zip file
update_zip="${update_zips[${nzips}]}"
update_dir="${update_zip%%.zip}"
echo "${now},startups,${update_zip#updates/}" >> updates/log

# remove quarantine flags and all other macos extended attributes if on macos
if [ -x /usr/bin/xattr ]
then
    chmod -R u+w .
    xattr -rc .
fi

# if updates already installed, only update if local FS or less than 30 mins since last update
doupdate="yes"
if [ -f "${update_dir}-installed" ]
then
    # already installed - check file system, df -l returns nothing for NFS mounts
    rm -f .fstype ; df -l . 2> /dev/null > .fstype
    if [ ! -s .fstype ]
    then
        reminders_update
    fi
    rm -f .fstype
fi

# if we are running updates ...
if [ "${doupdate}" == "yes" ]
then
    echo -e "${colorboldmagenta}  Checking startup files are up to date ...                                     ${undocolor}"

    # unzip the newest file
    unzip -qqud updates "${update_zip}"
    chmod -R go= "${update_zip}"

    # check we have an update directory
    if [ ! -d ${update_dir} ]
    then
        exit_with_error "Cannot find directory ${update_dir}"
    fi
    update_basename=`basename ${update_dir}`

    # run update's updater.bash if present, otherwise the generic one
    if [ -f "${update_dir}/updater.bash" ]
    then
        bash "${update_dir}/updater.bash" "${update_basename}" "${now}" | tee -a updates/log | sed "s/${now},updater,//"
    else
        bash "updates/updater.bash" "${update_basename}" "${now}" | tee -a updates/log | sed "s/${now},updater,//"
    fi
else
    echo -e "${colorboldmagenta}  Skipping checking of startup files ...                                        ${undocolor}"
fi

# make sure we have a minimal Makefile
default_makefile

# check svn contains startup files
echo -e "${colorboldmagenta}                                                                                ${undocolor}"
echo -e "${colorboldmagenta}  Checking startup files and originals are in svn ...                           ${undocolor}"
check_svn_originals
echo -e "${colorboldmagenta}                                                                                ${undocolor}"

# finally remind students about svn update/svn commit and their logbooks
reminders
