#include "bprintf.h"
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <errno.h>


#define GET_BYTE(val, index) ((char)((val) >> (8 * (index)) & 0xFF))


static int validate_fmt_str(const char *fmt);
static int get_fmt_chr_size(char fmt_char);

/*
 * Supported format specifiers:
 * byte/char (1) = 'c'
 * short (2) = 's'
 * int (4) = 'i'
 * long (8) = 'l'
 */

int bprintf(char *buffer, int size, const char *fmt, Endianness endianness,...)
{
    int required_size;
    int num_args;
    int arg_index;
    int buffer_index;
    const char *itr;
    va_list args;

    if (!buffer || !fmt || size <= 0)
        return -EINVAL;

    /*
     * since fmt specifier is one char
     * # of varargs is simply length of fmt
     */
    num_args = strlen(fmt);
    required_size = validate_fmt_str(fmt);
    if (required_size == 0 || required_size > size)
        return -EINVAL;

    va_start(args, endianness);
    itr = fmt;
    buffer_index = 0;
    arg_index = 0;

    for (itr = fmt; *itr; itr++)
    {
        int fmt_chr_size;
        int i;
        uint64_t val;
        val = (uint64_t) va_arg(args, uint64_t);

        fmt_chr_size = get_fmt_chr_size(*itr);
        if (fmt_chr_size == 0)
            return -EINVAL;

        for (i = 0; i < fmt_chr_size; i++)
        {
            if (endianness == Little)
                buffer[buffer_index + i] = GET_BYTE(val, i);
            else
                buffer[buffer_index + i] = GET_BYTE(val, fmt_chr_size - i - 1);
        }

        buffer_index += fmt_chr_size;
        arg_index++;
    }

    /* not all arguments consumed! */
    if (arg_index != num_args)
        return -E2BIG;

    va_end(args);
    return required_size;
}

static int validate_fmt_str(const char *fmt)
{
    int bufsize;
    const char *itr;

    if (!fmt || strlen(fmt) == 0)
        return 0;

    bufsize = 0;
    itr = fmt;

    for (itr = fmt; *itr; itr++)
    {
        int fmt_chr_size;
        fmt_chr_size = get_fmt_chr_size(*itr);

        if (fmt_chr_size == 0)
            return 0;

        bufsize += fmt_chr_size;
    }

    return bufsize;
}

static int get_fmt_chr_size(char fmt_char)
{
    switch (fmt_char)
    {
        case 'c':
            return sizeof(char);
        case 's':
            return sizeof(uint16_t);
        case 'i':
            return sizeof(uint32_t);
        case 'l':
            return sizeof(uint64_t);
        default:
            return 0;
    }
}
