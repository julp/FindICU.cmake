1) Define build and installation directories:

    (ba|k)sh:

        export BUILD_DIR=/tmp/build/
        export INSTALL_DIR=/tmp/

    (t)csh:

        set BUILD_DIR=/tmp/build/
        set INSTALL_DIR=/tmp/

2) Configure it: cmake -H/path/to/FindICU.cmake/sources/tests/RB/c_pkg_shared -B$BUILD_DIR -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR

3) Build it: make -C $BUILD_DIR

4) Install it: make -C $BUILD_DIR install

5) Run it:

    (ba|k)sh:

        #export LD_LIBRARY_PATH=$INSTALL_DIR/bin:$LD_LIBRARY_PATH
        LD_LIBRARY_PATH=$INSTALL_DIR/bin:$LD_LIBRARY_PATH LANG="" $INSTALL_DIR/bin/rb

    (t)csh:

        #setenv LD_LIBRARY_PATH $INSTALL_DIR/bin:$LD_LIBRARY_PATH
        env LD_LIBRARY_PATH=$INSTALL_DIR/bin:$LD_LIBRARY_PATH LANG="" $INSTALL_DIR/bin/rb

Change value of LANG to "fr", "es", ... and/or add a locale as argument to rb. Default locale is fallback on LANG environment variable not "root" except if you force it (LANG="" ./rb it).


If it works as expected, you should read as output:
   translation enabled: fr
   ...
   fr.txt

(the most important line is the last one)
