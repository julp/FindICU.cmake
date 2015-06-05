cmake_minimum_required(VERSION 2.8.3) # inherit FindICU.cmake

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../")

set(CMAKE_SYSTEM_PREFIX_PATH )
set(CMAKE_SYSTEM_INCLUDE_PATH )

# set(ICU_DEBUG TRUE)
find_package(ICU QUIET)

if(ICU_FOUND)
    message(FATAL_ERROR "expected ICU to be not found")
endif(ICU_FOUND)
