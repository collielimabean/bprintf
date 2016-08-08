#include "bprintf.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


void dump_buf(const char *buf, int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d: 0x%x\n", i, buf[i]);
}

int simple_test(void)
{
    char buf[15];
    int bytes_written;
    int i;
    long longval;

    unsigned char golden_big[] = { 
        0xAB, 0xAB, 0xCD,
        0xDE, 0xAD, 0xBE,
        0xEF, 0x01, 0x23,
        0x45, 0x67, 0x89,
        0xAB, 0xCD, 0xEF
    };

    unsigned char golden_little[] = { 
        0xAB, 0xCD, 0xAB,
        0xEF, 0xBE, 0xAD,
        0xDE, 0xEF, 0xCD,
        0xAB, 0x89, 0x67,
        0x45, 0x23, 0x01
    };

    longval = 0x0123456789ABCDEF;

    bytes_written = bprintf(buf, 15, "csil", Big, 0xAB, 0xABCD, 0xDEADBEEF, longval);
    if (bytes_written <= 0)
        assert(0);

    for (i = 0; i < bytes_written; i++)
        if ((buf[i] & 0xFF) != golden_big[i])
            assert(0);

    bytes_written = bprintf(buf, 15, "csil", Little, 0xAB, 0xABCD, 0xDEADBEEF, longval);
    if (bytes_written <= 0)
        assert(0);

    for (i = 0; i < bytes_written; i++)
        if ((buf[i] & 0xFF) != golden_little[i])
            assert(0);

    return 1;
}

int bad_param_test(void)
{
    char buf[10];

    if (bprintf(NULL, 15, "", Big) != 0)
        return 0;
    if (bprintf(buf, -1, "sii", Big, 0xABCD, 0xDEADBEEF, 0xBEEFDEAD) != 0)
        return 0;
    if (bprintf(buf, 0, "sii", Big, 0xABCD, 0xDEADBEEF, 0xBEEFDEAD) != 0)
        return 0;
    if (bprintf(buf, 9, "sii", Big, 0xABCD, 0xDEADBEEF, 0xBEEFDEAD) != 0)
        return 0;
    if (bprintf(buf, 10, NULL, Big, 0xABCD, 0xDEADBEEF, 0xBEEFDEAD) != 0)
        return 0;

    /*TODO: verify if invalid format specifiers are passed in */

    return 1;
}


int main(void)
{
    printf("%s: %s!\n", "simple_test", (simple_test()) ? "Pass" : "Fail");
    printf("%s: %s!\n", "bad_param_test", (bad_param_test()) ? "Pass" : "Fail");
    return 0;
}
