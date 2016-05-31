1) Move into test's directory (cd /.../FindICU.cmake/tests/RB/c_nopkg)
2) Configure it: cmake . -DCMAKE_INSTALL_PREFIX=/tmp
3) Build it: make
4) Install it: make install
5) Move to installation directory: cd /tmp/bin
6) Run it: ./rb

If it works as expected, you should read as output:
   translation enabled: fr
   ...
   fr.txt

(the most important line is the last one)
