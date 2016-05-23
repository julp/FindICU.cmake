@echo off

set INSTALL_DIR=M:\
set BUILD_DIR=M:\build
set ICU_ROOT_DIR=C:\prog\icu

rem set it to "64" to switch from x86 to x64
set ARCH=

set LIB=%ICU_ROOT_DIR%\bin%ARCH%;%ICU_ROOT_DIR%\lib%ARCH%;%LIB%
set PATH=%ICU_ROOT_DIR%\bin%ARCH%;%ICU_ROOT_DIR%\lib%ARCH%;%INSTALL_DIR%\bin;%PATH%
set INCLUDE=%ICU_ROOT_DIR%\include;%INCLUDE%

cmake -H%~dp0 -B%BUILD_DIR% -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% -DCMAKE_BUILD_TYPE="Release"
cd /D %BUILD_DIR%
nmake
nmake install

rem to see it M:\bin\rb is linked to debug libraries, you can use: dumpbin /IMPORTS M:\bin\rb
