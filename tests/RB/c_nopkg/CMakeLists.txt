#
# File: c_nopkg/CMakeLists.txt
# Test: standalone (= no package) resource bundles
#

cmake_minimum_required(VERSION 2.8.3) # inherit FindICU.cmake

# set(CMAKE_VERBOSE_MAKEFILE TRUE)
# set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../..")

find_package(ICU COMPONENTS i18n io REQUIRED)

add_definitions(-DURESNAME=. -DURES_COMMON)
include_directories(${ICU_INCLUDE_DIRS})
add_executable(rb ../rb.c)
install(TARGETS rb DESTINATION bin)
target_link_libraries(rb ${ICU_LIBRARIES})

icu_generate_resource_bundle(
    NAME abc
    # No PACKAGE argument here!!!
    DESTINATION bin # share the same directory with the binary
    FILES ../root.txt ../fr/fr.txt ../es/es.txt ../es/es_ES.txt
)
