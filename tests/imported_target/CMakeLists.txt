cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../")

# set(ICU_DEBUG TRUE)
find_package(ICU COMPONENTS i18n io)

add_executable(uregex ${PROJECT_SOURCE_DIR}/../uregex.c)
target_link_libraries(uregex ICU::ICU)
