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

void simple_bprintf_big_endian_test(void)
{
    char buf[15];
    int bytes_written;
    int i;
    long longval;

    unsigned char golden[] = { 
        0xAB, 0xAB, 0xCD,
        0xDE, 0xAD, 0xBE,
        0xEF, 0x01, 0x23,
        0x45, 0x67, 0x89,
        0xAB, 0xCD, 0xEF
    };

    longval = 0x0123456789ABCDEF;

    bytes_written = bprintf(buf, 15, "csil", Big, 0xAB, 0xABCD, 0xDEADBEEF, longval);
    assert(bytes_written > 0);

    for (i = 0; i < bytes_written; i++)
        assert((buf[i] & 0xFF) == golden[i]);
}

void simple_bprintf_little_endian_test(void)
{
    char buf[15];
    int bytes_written;
    int i;
    long longval;

    unsigned char golden[] = { 
        0xAB, 0xCD, 0xAB,
        0xEF, 0xBE, 0xAD,
        0xDE, 0xEF, 0xCD,
        0xAB, 0x89, 0x67,
        0x45, 0x23, 0x01
    };

    longval = 0x0123456789ABCDEF;

    bytes_written = bprintf(buf, 15, "csil", Little, 0xAB, 0xABCD, 0xDEADBEEF, longval);
    assert(bytes_written > 0);

    for (i = 0; i < bytes_written; i++)
        assert((buf[i] & 0xFF) == golden[i]);
}

void simple_bscanf_big_endian_test(void)
{
    unsigned char cval;
    unsigned short sval;
    unsigned int ival;
    unsigned long lval;

    unsigned char buf[] = { 
        0xAB, 0xAB, 0xCD,
        0xDE, 0xAD, 0xBE,
        0xEF, 0x01, 0x23,
        0x45, 0x67, 0x89,
        0xAB, 0xCD, 0xEF
    };

    assert(bscanf((const char *)buf, 15, "csil", Big, &cval, &sval, &ival, &lval) > 0);
    assert(cval == 0xAB);
    assert(sval == 0xABCD);
    assert(ival == 0xDEADBEEF);
    assert(lval == 0x0123456789ABCDEFL);
}

void bad_param_test(void)
{
    char buf[10];

    assert(bprintf(NULL, 15, "", Big) <= 0);
    assert(bprintf(buf, -1, "sii", Big, 0xABCD, 0xDEADBEEF, 0xBEEFDEAD) <= 0);
    assert(bprintf(buf, 0, "sii", Big, 0xABCD, 0xDEADBEEF, 0xBEEFDEAD) <= 0);
    assert(bprintf(buf, 9, "sii", Big, 0xABCD, 0xDEADBEEF, 0xBEEFDEAD) <= 0);
    assert(bprintf(buf, 10, NULL, Big, 0xABCD, 0xDEADBEEF, 0xBEEFDEAD) <= 0);

    /* TODO: verify if invalid format specifiers are passed in */
}


int main(void)
{
    simple_bprintf_big_endian_test();
    simple_bprintf_little_endian_test();
    simple_bscanf_big_endian_test();
    bad_param_test();
    printf("All tests passed!\n");
    return 0;
}
