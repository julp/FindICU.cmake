#!/usr/bin/env bash

shopt -s extglob

declare -r __DIR__=$(dirname $(readlink -f "${BASH_SOURCE}"))
declare -r ICUC_SVN_URL='http://source.icu-project.org/repos/icu/icu'

SVN_CMD=`whereis -bq svn`
if [ -z "${SVN_CMD}" ]; then
    SVN_CMD=`whereis -bq svnlite`
fi
if [ -z "${SVN_CMD}" ]; then
    echo "no suitable svn command found, abort"
fi

for tag in `${SVN_CMD} ls "${ICUC_SVN_URL}/tags/"`; do
# ${SVN_CMD} ls "${ICUC_SVN_URL}/tags/" > "${__DIR__}/tags.txt"
# while read tag; do
    case "${tag}" in
    *-c0-*|*update*|*-alpha/|release-1-[456]-0/|*d+([0123456789])/)
        continue
        ;;
    milestone*)
        milestone=1
        ;;
    release*)
        milestone=0
        ;;
    *)
        continue
        ;;
    esac
    version=`echo "$tag" | sed -Ee 's/^(release|milestone)-//' -e 's#/$##' | tr '-' '.' | tr '[[:lower:]]' '[[:upper:]]'`
    parts=( ${version//./ } )
    if [ "${milestone}" -eq 1 ]; then
        if [ "${parts[0]}" -ge 49 ]; then
            version="${parts[0]}M${parts[2]}"
        fi
    fi
    version=`echo "${version}" | sed -E 's/.(RC)/\1/'`
    # ICU >= 4.4
    if [ "${parts[0]}" -gt 4 ]; then
        file="unicode/uvernum.h"
    elif [ "${parts[0]}" -eq 4 -a "${parts[1]}" -ge 4 ]; then
        file="unicode/uvernum.h"
    # ICU [2;4.4]
    elif [ "${parts[0]}" -ge 2 ]; then
        file="unicode/uversion.h"
    # ICU [1.4;2[
    elif [ "${parts[1]}" -ge 4 ]; then
        file="unicode/utypes.h"
    # ICU < 1.4
    else
        file="utypes.h"
    fi
    if [ ! -f "${__DIR__}/${version}/${file}" ]; then
#         echo "${tag} => ${version}"
        mkdir -p "${__DIR__}/${version}/`dirname \"${file}\"`"
#         echo "mkdir -p \"${__DIR__}/${version}/`dirname '${file}'`/.svn/tmp\""
        ${SVN_CMD} export "${ICUC_SVN_URL}/tags/${tag}source/common/${file}" "${__DIR__}/${version}/${file}"
#         echo "${SVN_CMD} export \"${ICUC_SVN_URL}/tags/${tag}source/common/${file}\" \"${__DIR__}/${version}/${file}\""
    fi
done
# done < tags.txt
