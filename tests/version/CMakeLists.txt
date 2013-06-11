cmake_minimum_required(VERSION 2.8.3)

set(ICU_PUBLIC_VAR_NS "ICU")                    # Prefix for all ICU relative public variables
set(ICU_PRIVATE_VAR_NS "_${ICU_PUBLIC_VAR_NS}") # Prefix for all ICU relative internal variables

# Keep this in sync with FindICU.cmake

# Changes between this CMakeLists.txt and FindICU.cmake:
# - suffix _${_DIR} added to ${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS (no cache)
# - unicode/uversion.h and unicode/uvernum.h added to find_path to minimize the number of headers file (unicode/utypes.h and utypes.h is sufficient and common to all versions)
# - added option NO_DEFAULT_PATH to find_path to forbid cmake to search locations other than our fake include directory

function(icu_version_test _DIR)
    find_path(
        ${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR} # UT specific: add ${_DIR} as suffix to not fetch first cached value
        NAMES unicode/utypes.h utypes.h unicode/uversion.h unicode/uvernum.h # UT specific: search directly the header version file as all headers are not present
        HINTS ${_DIR} # UT specific: we have to add our own location
        DOC "Include directories for ICU"
        NO_DEFAULT_PATH # UT specific: ignore standard locations, use only our own location
    )

    if(${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR})
        ########## <part to keep synced with FindICU.cmake> ##########
        # TODO: define a list?
        if(EXISTS "${${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR}}/unicode/uvernum.h") # ICU >= 4
            file(READ "${${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR}}/unicode/uvernum.h" ${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS)
        elseif(EXISTS "${${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR}}/unicode/uversion.h") # ICU [2;4[
            file(READ "${${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR}}/unicode/uversion.h" ${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS)
        elseif(EXISTS "${${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR}}/unicode/utypes.h") # ICU [1.4;2[
            file(READ "${${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR}}/unicode/utypes.h" ${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS)
        elseif(EXISTS "${${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR}}/utypes.h") # ICU 1.3
            file(READ "${${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR}}/utypes.h" ${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS)
        else()
            message("ICU version header not found") # UT specific: no FATAL_ERROR
        endif()

        if(${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS MATCHES ".*# *define *ICU_VERSION *\"([0-9]+)\".*") # ICU 1.3
            # [1.3;1.4[ as #define ICU_VERSION "3" (no patch version, ie all 1.3.X versions will be detected as 1.3.0)
            set(${ICU_PUBLIC_VAR_NS}_MAJOR_VERSION "1")
            #string(REGEX REPLACE ".*# *define *ICU_VERSION *\"([0-9]+)\".*" "\\1" ${ICU_PUBLIC_VAR_NS}_MINOR_VERSION "${${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS}")
            set(${ICU_PUBLIC_VAR_NS}_MINOR_VERSION "${CMAKE_MATCH_1}")
            set(${ICU_PUBLIC_VAR_NS}_PATCH_VERSION "0")
        elseif(${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS MATCHES ".*# *define *U_ICU_VERSION_MAJOR_NUM *([0-9]+).*")
            #
            # Since version 4.9.1, ICU release version numbering was totaly changed, see:
            # - http://site.icu-project.org/download/49
            # - http://userguide.icu-project.org/design#TOC-Version-Numbers-in-ICU
            #
            #string(REGEX REPLACE ".*# *define *U_ICU_VERSION_MAJOR_NUM *([0-9]+).*" "\\1" ${ICU_PUBLIC_VAR_NS}_MAJOR_VERSION "${${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS}")
            set(${ICU_PUBLIC_VAR_NS}_MAJOR_VERSION "${CMAKE_MATCH_1}")
            string(REGEX REPLACE ".*# *define *U_ICU_VERSION_MINOR_NUM *([0-9]+).*" "\\1" ${ICU_PUBLIC_VAR_NS}_MINOR_VERSION "${${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS}")
            string(REGEX REPLACE ".*# *define *U_ICU_VERSION_PATCHLEVEL_NUM *([0-9]+).*" "\\1" ${ICU_PUBLIC_VAR_NS}_PATCH_VERSION "${${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS}")
        elseif(${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS MATCHES ".*# *define *U_ICU_VERSION *\"(([0-9]+)(\\.[0-9]+)*)\".*") # ICU [1.4;1.8[
            # [1.4;1.8[ as #define U_ICU_VERSION "1.4.1.2" but it seems that some 1.4.1(?:\.\d)? have releasing error and appears as 1.4.0
            #string(REGEX REPLACE ".*# *define *U_ICU_VERSION *\"([0-9]+)(\\.[0-9]+)*\".*" "\\1" ${ICU_PRIVATE_VAR_NS}_FULL_VERSION "${${ICU_PRIVATE_VAR_NS}_VERSION_HEADER_CONTENTS}")
            set(${ICU_PRIVATE_VAR_NS}_FULL_VERSION "${CMAKE_MATCH_1}") # copy CMAKE_MATCH_1, no longer valid on the following if
            if(${ICU_PRIVATE_VAR_NS}_FULL_VERSION MATCHES "^([0-9]+)\\.([0-9]+)$")
                set(${ICU_PUBLIC_VAR_NS}_MAJOR_VERSION "${CMAKE_MATCH_1}")
                set(${ICU_PUBLIC_VAR_NS}_MINOR_VERSION "${CMAKE_MATCH_2}")
                set(${ICU_PUBLIC_VAR_NS}_PATCH_VERSION "0")
            elseif(${ICU_PRIVATE_VAR_NS}_FULL_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
                set(${ICU_PUBLIC_VAR_NS}_MAJOR_VERSION "${CMAKE_MATCH_1}")
                set(${ICU_PUBLIC_VAR_NS}_MINOR_VERSION "${CMAKE_MATCH_2}")
                set(${ICU_PUBLIC_VAR_NS}_PATCH_VERSION "${CMAKE_MATCH_3}")
            endif()
        else()
            message("failed to detect ICU version") # UT specific: no FATAL_ERROR
        endif()
        set(${ICU_PUBLIC_VAR_NS}_VERSION "${${ICU_PUBLIC_VAR_NS}_MAJOR_VERSION}.${${ICU_PUBLIC_VAR_NS}_MINOR_VERSION}.${${ICU_PUBLIC_VAR_NS}_PATCH_VERSION}")
        ########## </part to keep synced with FindICU.cmake> ##########
    endif(${ICU_PUBLIC_VAR_NS}_INCLUDE_DIRS_${_DIR})

    if(
        NOT DEFINED ${ICU_PUBLIC_VAR_NS}_MAJOR_VERSION
        OR NOT DEFINED ${ICU_PUBLIC_VAR_NS}_MINOR_VERSION
        OR NOT DEFINED ${ICU_PUBLIC_VAR_NS}_PATCH_VERSION
    )
        message("${_DIR} => unable to determine ICU version")
    else()
        if(EXISTS "${_DIR}/EXPECTED.txt")
            file(STRINGS "${_DIR}/EXPECTED.txt" expected LIMIT_COUNT 1) # allow additionnal comment for documentation at end of file
            if(${ICU_PUBLIC_VAR_NS}_VERSION VERSION_EQUAL expected)
                message("${_DIR} => ${expected} (pass)")
            else(${ICU_PUBLIC_VAR_NS}_VERSION VERSION_EQUAL expected)
                message(FATAL_ERROR "test failed for ${_DIR}: get ${${ICU_PUBLIC_VAR_NS}_VERSION}, expect ${expected}")
            endif(${ICU_PUBLIC_VAR_NS}_VERSION VERSION_EQUAL expected)
        else(EXISTS "${_DIR}/EXPECTED.txt")
            message("${_DIR} => ${${ICU_PUBLIC_VAR_NS}_VERSION} (default: please fill ${_DIR}/EXPECTED.txt)")
        endif(EXISTS "${_DIR}/EXPECTED.txt")
    endif()
endfunction(icu_version_test)

file(GLOB directories RELATIVE ${PROJECT_SOURCE_DIR} "[0-9]*")
list(SORT directories)
foreach(d ${directories})
    icu_version_test("${d}")
endforeach(d)
