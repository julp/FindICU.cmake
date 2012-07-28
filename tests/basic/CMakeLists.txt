cmake_minimum_required(VERSION 2.8.3) # inherit FindICU.cmake

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../..)

# set(ICU_DEBUG TRUE)
find_package(ICU COMPONENTS i18n io REQUIRED)

include_directories(${ICU_INCLUDE_DIRS})
add_executable(uregex uregex.c)
target_link_libraries(uregex ${ICU_LIBRARIES})
