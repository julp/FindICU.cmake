#include <stdlib.h>

#include <unicode/putil.h>
#include <unicode/ures.h>
#include <unicode/ustdio.h>
#include <unicode/uclean.h>
#ifndef URES_COMMON
# include <unicode/udata.h>
#endif /* !URES_COMMON */

#ifndef _WIN32
# define RED(str) "\33[1;31m" str "\33[0m"
# define GREEN(str) "\33[1;32m" str "\33[0m"
# define YELLOW(str) "\33[1;33m" str "\33[0m"
#else
# define RED(str) str
# define GREEN(str) str
# define YELLOW(str) str
#endif /* !_WIN32 */

#define STRINGIFY(x) #x
#define STRINGIFY_EXPANDED(x) STRINGIFY(x)
# define CONCAT(a, b) a ## b
# define CONCAT_EXPANDED(a, b) CONCAT(a, b)

#define URES_STR STRINGIFY_EXPANDED(URESNAME)
#ifndef URES_COMMON
# define URES_SYM CONCAT_EXPANDED(URESNAME, _dat)
U_CFUNC const void
# ifdef URES_SHARED
U_IMPORT
# endif /* URES_SHARED */
*URES_SYM;
#endif /* !URES_COMMON */

#define debug(format, ...) \
    fprintf(stderr, format, ## __VA_ARGS__);

int main(int argc, char **argv)
{
    int ret;
    UFILE *ustdout;
    int32_t msg_len;
    const UChar *msg;
    UErrorCode status;
    UResourceBundle *ures;
    const char *locale = NULL;//"fr_FR";

    ures = NULL;
    ret = EXIT_FAILURE;
    status = U_ZERO_ERROR;
    ustdout = u_finit(stdout, NULL, NULL);

    if (2 == argc) {
        locale = argv[1];
    }

#ifdef URES_COMMON
    u_setDataDirectory(".");
#else
    udata_setAppData(URES_STR, &URES_SYM, &status);
    if (U_FAILURE(status)) {
        debug("udata_setAppData failed: %s\n", u_errorName(status));
        goto end;
    }
#endif /* URES_COMMON */

    debug("loading locale: %s\n", NULL == locale ? "-" : locale);
    ures = ures_open(URES_STR, locale, &status);
    if (U_FAILURE(status)) {
        debug("translation " RED("disabled") ": %s\n", u_errorName(status));
        goto end;
    } else {
        if (U_USING_DEFAULT_WARNING == status) {
            debug(YELLOW("default") " translation enabled\n");
        } else {
            debug("translation " GREEN("enabled") ": %s\n", ures_getLocaleByType(ures, ULOC_ACTUAL_LOCALE, &status));
        }
    }
    /* */
    msg = ures_getStringByKey(ures, "whoami", &msg_len, &status);
    if (U_FAILURE(status)) { /* U_MISSING_RESOURCE_ERROR */
        // TODO: "better"
        goto end;
    } else if (U_USING_DEFAULT_WARNING == status) {
        debug("[M] default translation used\n");
    } else if (U_USING_FALLBACK_WARNING == status) {
        /* on working tests you see that because resource bundle source has 'fr' as locale not 'fr_FR' */
        debug("[M] translation fallback\n");
    }
    u_fprintf(ustdout, "%S\n", msg);
    /* */
    ret = EXIT_SUCCESS;

end:
    if (NULL != ures) {
        ures_close(ures);
    }
    u_fclose(ustdout);
    u_cleanup();

    return ret;
}
