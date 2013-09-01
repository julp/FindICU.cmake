Only significant changes to FindICU.cmake figure here:

* 2013-09-01: icu-config was never found (find_program arguments inverted)
* 2013-06-11: revert previous translation of new ICU versions (ICU >= 49) into old format, keep it as is (done by 2012-05-31)
* 2012-12-10: support for ICU_*_FLAGS (empty if icu-config is not found)
* 2012-09-22: version header not found was not fatal
* 2012-07-29:
  + distinguish release and debug ICU builds (win32)
  + io component library was named ustdio in version 2.4
* 2012-06-25:
  + consider amd64 architectures (bin64 and lib64 as PATH_SUFFIXES) (not tested)
  + detection improved for versions older than 4
  + i18n component mapped to the wrong library (on uc - due to cache ?)
* 2012-05-31: workaround to handle new release numbering since 4.9.1 (still version 4)
* 2012-04-28: support for ICU_ROOT: allow user to set the root installation of ICU
* 2011-11-04: fix mix of ICU_INCLUDE_DIR and ICU_INCLUDE_DIRS to (only) ICU_INCLUDE_DIRS
* 2011-06-24: add i18n library name on windows (msvc)
