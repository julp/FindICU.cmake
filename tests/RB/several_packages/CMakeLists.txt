#
# File: several_packages/CMakeLists.txt
# Test: package of multiple resource bundles as dynamic libraries
#

cmake_minimum_required(VERSION 2.8.3) # inherit FindICU.cmake

# set(CMAKE_VERBOSE_MAKEFILE TRUE)
# set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../..")

find_package(ICU COMPONENTS i18n io REQUIRED)

set(TARGET_NAMES )
macro(dummy_binary TARGET_NAME BUNDLE_NAME)
    list(APPEND TARGET_NAMES ${TARGET_NAME})

    add_executable(${TARGET_NAME} ../rb.c)
#     target_compile_definitions(${TARGET_NAME} PRIVATE URESNAME=${BUNDLE_NAME} URES_SHARED) # CMake 3.X
#     set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS URESNAME=${BUNDLE_NAME};URES_SHARED)
    set_property(TARGET ${TARGET_NAME} APPEND PROPERTY COMPILE_DEFINITIONS URESNAME=${BUNDLE_NAME} URES_SHARED)
    target_link_libraries(${TARGET_NAME} ${ICU_LIBRARIES})
endmacro(dummy_binary)

include_directories(${ICU_INCLUDE_DIRS})

dummy_binary(rb1 foo)
dummy_binary(rb2 bar)

install(TARGETS ${TARGET_NAMES} DESTINATION bin)

icu_generate_resource_bundle(
    NAME foo
    PACKAGE
    TYPE library
    DESTINATION bin
    DEPENDS rb1
    FILES ../root.txt ../fr/fr.txt ../es/es.txt ../es/es_ES.txt
)

icu_generate_resource_bundle(
    NAME bar
    PACKAGE
    TYPE library
    DESTINATION bin
    DEPENDS rb2
    FILES root.txt fr/fr.txt
)
