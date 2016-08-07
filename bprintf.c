#include "bprintf.h"
#include <stdint.h>
#include <string.h>


#define GET_BYTE(val, index) ((char)(val >> (8 * index) & 0xFF))


static int validate_fmt_str(const char *fmt);

static 

/*
 * Supported format specifiers:
 * byte/char (1) = 'c'
 * short (2) = 's'
 * int (4) = 'i'
 * long (8) = 'l'
 */

// write big endian only
int bprintf(char *buffer, int size, const char *fmt, Endianness endianness,...)
{
    int required_size;
    int num_args;
    int arg_index;
    int buffer_index;
    const char *itr;
    va_list args;

    if (!buffer || size <= 0)
        return 0;

    // since fmt specifier is one char
    // # of varargs is simply length of fmt
    num_args = strlen(fmt);
    required_size = validate_fmt_str(fmt);
    if (required_size == 0 || required_size > size)
        return 0;

    va_start(args, endianness);
    itr = fmt;
    buffer_index = 0;
    arg_index = 0;

    for (itr = fmt; *itr; *itr++)
    {
        switch (*itr)
        {
            case 'c':
                buffer[buffer_index] = va_arg(args, uint8_t);
                buffer_index += sizeof(uint8_t);
                break;

            case 's':
                uint16_t val = va_arg(args, uint16_t);
                if (endianness == BIG)
                {
                    buffer[buffer_index] = GET_BYTE(val, 1);
                    buffer[buffer_index + 1] = GET_BYTE(val, 0);
                }
                else
                {
                    buffer[buffer_index] = GET_BYTE(val, 0);
                    buffer[buffer_index + 1] = GET_BYTE(val, 1);
                }

                buffer_index += sizeof(uint16_t);
                break;

            case 'i':
                uint32_t val = va_arg(args, uint32_t);
                if (endianness == BIG)
                {
                    buffer[buffer_index] = GET_BYTE(val, 3);
                    buffer[buffer_index + 1] = GET_BYTE(val, 2);
                    buffer[buffer_index + 2] = GET_BYTE(val, 1);
                    buffer[buffer_index + 3] = GET_BYTE(val, 0);
                }
                else
                {
                    buffer[buffer_index] = GET_BYTE(val, 2);
                    buffer[buffer_index + 1] = GET_BYTE(val, 3);
                    buffer[buffer_index + 2] = GET_BYTE(val, 0);
                    buffer[buffer_index + 3] = GET_BYTE(val, 1);                    
                }
                break;

            case 'l':
                if (endianness == BIG)
                {
                    buffer[buffer_index] = GET_BYTE(val, 3);
                    buffer[buffer_index + 1] = GET_BYTE(val, 2);
                    buffer[buffer_index + 2] = GET_BYTE(val, 1);
                    buffer[buffer_index + 3] = GET_BYTE(val, 0);
                }
                else
                {
                    
                }
                break;
            default:
                // string corrupted?
                return 0;
        }
        arg_index++;
    }

    // not all arguments consumed!
    if (arg_index != num_args - 1)
        return 0;

    va_end(args);
    return required_size;
}

int validate_fmt_str(const char *fmt)
{
    int bufsize;
    const char *itr;

    if (!fmt)
        return 0;

    bufsize = 0;
    itr = fmt;

    for (itr = fmt, *itr; *itr++)
    {    
        switch (*itr)
        {
            case 'c':
                bufsize += sizeof(uint8_t);
                break;
            case 's':
                bufsize += sizeof(uint16_t);
                break;
            case 'i':
                bufsize += sizeof(uint32_t);
                break;
            case 'l':
                bufsize += sizeof(uint64_t);
                break;
            default:
                return 0;
        }
    }

    return bufsize;
}