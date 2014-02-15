# FindICU.cmake

A CMake module to find International Components for Unicode (ICU) Library

## Information

### Prerequisites

Only one: CMake >= 2.8.3 (for new version of find_package_handle_standard_args)

### License

License: BSD

## Usage

### Variables set for your project

* ICU_FOUND: were all of your specified components found (include dependencies)?
* ICU_INCLUDE_DIRS: ICU include directory
* ICU_LIBRARIES: ICU libraries
* ICU_VERSION: complete version of ICU (x.y.z)
* ICU_MAJOR_VERSION: major version of ICU
* ICU_MINOR_VERSION: minor version of ICU
* ICU_PATCH_VERSION: patch version of ICU
* ICU_\<COMPONENT\>_FOUND: was \<COMPONENT\> found? (FALSE for non specified component if it is not a dependency)
* ICU_C_FLAGS: C compiler flags
* ICU_CXX_FLAGS: C++ compiler flags
* ICU_CPP_FLAGS: C Preprocessor flags
* ICU_C_SHARED_FLAGS: C flags for building shared libraries
* ICU_CXX_SHARED_FLAGS: C++ flags for building shared libraries
* ICU_CPP_SHARED_FLAGS: C Preprocessor flags for building shared libraries

Note: all ICU_*_FLAGS variables will be defined as empty strings if icu-config does not exist or cannot be found

### How to use

#### For developpers

- You just need to copy the file FindICU.cmake at top of your project directory (you don't need sources for tests)
- Tell to CMake to search this non-standard module in your project directory by adding near top to your CMakeLists.txt: `set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})`
- Just after, call find_package() once: `find_package(ICU)`

Notes:
* if ICU is required by your project, append the keyword *REQUIRED* (`find_package(ICU REQUIRED)`). On the opposite, if ICU is optional, the keyword *QUIET* can be used to silently try to find ICU.
* for a project which depends on a minimum version of ICU, add it just after 'ICU' (eg mandatory ICU >= 4.4: `find_package(ICU 4.4 REQUIRED)`)

#### For end users

If ICU is located in a non-regular place, add a `-DICU_ROOT=<PATH>` option to your cmake command line.

### Sample

For a binary which implies ICU >= 4.4:

```cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package(ICU 4.4 REQUIRED)

include_directories(
    ${ICU_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(tables tables.c)
target_link_libraries(tables ${ICU_LIBRARIES})
```
