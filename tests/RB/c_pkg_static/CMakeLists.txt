#
# File: c_pkg_static/CMakeLists.txt
# Test: package of resource bundles as static library
#

cmake_minimum_required(VERSION 2.8.3) # inherit FindICU.cmake

# set(CMAKE_VERBOSE_MAKEFILE TRUE)
# set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../..")

find_package(ICU COMPONENTS i18n io REQUIRED)

add_definitions(-DURESNAME=foo -DURES_STATIC)
include_directories(${ICU_INCLUDE_DIRS})
add_executable(rb ../rb.c)
install(TARGETS rb DESTINATION bin)
target_link_libraries(rb ${ICU_LIBRARIES})

icu_generate_resource_bundle(
    NAME foo
    PACKAGE
    TYPE static
    # DESTINATION has no sense
    DEPENDS rb # add_executable
    FILES ../root.txt ../fr/fr.txt ../es/es.txt ../es/es_ES.txt
)
