#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unicode/uregex.h>
#include <unicode/ustdio.h>
#include <unicode/uclean.h>

#define ARRAY_SIZE(array)     (sizeof(array) / sizeof((array)[0]))
#define USTRING_SIZE(array)   ARRAY_SIZE(array)
#define USTRING_LENGTH(array) (ARRAY_SIZE(array) - 1)

#define icu_error(status, function) \
    fprintf(stderr, "ICU Error \"%s\" from " function "()\n", u_errorName(status))

int main(void)
{
    int ret;
    int32_t l, u;
    UErrorCode status;
    UFILE *ustdout, *ustderr;
    URegularExpression *uregex;
    UBool case_insensitive = FALSE;
    UParseError pe = {-1, -1, {0}, {0}};
    UChar pattern[] = {
        0x0028, // 28, (
        0x005C, // 5C, backslash
        0x0070, // 70, p
        0x007B, // 7B, {
        0x004C, // 4C, L
        0x007D, // 7D, }
        0x0029, // 29, )
        0x0028, // 28, (
        0x005C, // 5C, backslash
        0x0070, // 70, p
        0x007B, // 7B, {
        0x004E, // 4E, N
        0x0064, // 64, d
        0x007D, // 7D, }
        0x0029, // 29, )
        0
    };
    UChar string[] = {
        0xD835, 0xDE3C, // A
        0xD835, 0xDE3C, // A
        0xD835, 0xDFE2, // 0
        0xD835, 0xDE3D, // B
        0xD835, 0xDE3D, // B
        0xD835, 0xDFE3, // 1
        0xD835, 0xDE3E, // C
        0xD835, 0xDE3E, // C
        0xD835, 0xDFE4, // 2
        0
    };

    uregex = NULL;
    ret = EXIT_SUCCESS;
    status = U_ZERO_ERROR;
    ustdout = u_finit(stdout, NULL, NULL);
    ustderr = u_finit(stderr, NULL, NULL);

    uregex = uregex_open(pattern, -1, case_insensitive ? UREGEX_CASE_INSENSITIVE : 0, &pe, &status);
    if (U_FAILURE(status)) {
        if (U_REGEX_RULE_SYNTAX == status) {
            u_fprintf(ustderr, "Invalid pattern: error at offset %d\n\t%S\n\t%*c\n", pe.offset, pattern, pe.offset, '^');
        }
        goto end;
    }
    uregex_setText(uregex, string, -1, &status);
    if (U_FAILURE(status)) {
        goto end;
    }
    while (uregex_findNext(uregex, &status)) {
        l = uregex_start(uregex, 0, &status);
        if (U_FAILURE(status)) {
            goto end;
        }
        u = uregex_end(uregex, 0, &status);
        if (U_FAILURE(status)) {
            goto end;
        }
        // $0
        u_fprintf(ustdout, "Match found at %d position (to %d): %.*S\n", l, u, u - l, string + l);
        // $1 à $2 (on pourrait aussi utiliser uregex_group avec un groupNum à valeur 0 pour récupérer $0)
        {
            UChar buffer[1024];
            int32_t i, l, g;

            l = uregex_groupCount(uregex, &status);
            if (U_FAILURE(status)) {
                icu_error(status, "uregex_groupCount");
            }
            for (i = 1; i <= l; i++) {
                g = uregex_group(uregex, i, buffer, USTRING_SIZE(buffer), &status);
                if (U_FAILURE(status)) {
                    icu_error(status, "uregex_group");
                }
                u_fprintf(ustdout, "$%d : %S\n", i, buffer);
            }
        }
    }
    if (U_FAILURE(status)) {
        goto end;
    }

    if (FALSE) {
end:
        ret = EXIT_FAILURE;
    }
    if (NULL != ustderr) {
        u_fclose(ustderr);
    }
    if (NULL != ustdout) {
        u_fclose(ustdout);
    }
    if (NULL != uregex) {
        uregex_close(uregex);
    }
    u_cleanup();

    return ret;
}
