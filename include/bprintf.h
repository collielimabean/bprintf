#ifndef _BPRINTF_H
#define _BPRINTF_H

typedef enum
{
    Big, 
    Little
} Endianness;

/**
 * Serializes the integers into buffer via the given format string and
 * specified endianness. Think of it like sprintf, just without converting
 * the integer to a string. 
 * 
 * Valid format characters are:
 * 'c' - for 1 byte values.
 * 's' - for 2 byte values.
 * 'i' - for 4 byte values.
 * 'l' - for 8 byte values.
 *
 * For example, calling bprintf(buf, sz, "cs", Big, 0xAB, 0xCDEF) would set
 * buf to: { 0xAB, 0xCD, 0xEF }. Calling the same function, except with 
 * Little instead of Big, you get {0xAB, 0xEF, 0xCD }.
 *
 * Returns a negative errno on failure, else the number of bytes
 * written into the buffer.
 */
int bprintf(
    char *buffer,
    int size, 
    const char *fmt, 
    Endianness endianness,
    ...
);

/**
 * Reads the integers from the buffer via the given format string in
 * the specified endianness. Think of it like sscanf, just minus the
 * string to integer conversion. 
 *
 * Valid format characters are:
 * 'c' - for 1 byte values.
 * 's' - for 2 byte values.
 * 'i' - for 4 byte values.
 * 'l' - for 8 byte values.
 *
 * For example, let's have a char *buf = { 0x01, 0x23, 0x34 }. Suppose
 * we call bscanf(buf, sz, "sc", Big, &sval, &cval). sval would be set
 * to 0x0123, and cval to 0x34. Swap out the Big to Little endian, and
 * you get sval = 0x2301, and cval = 0x34.
 *
 * Returns a negative errno on failure, else the number of bytes read.
 */
int bscanf(
    const char *buffer,
    int size,
    const char *fmt,
    Endianness endianness,
    ...
);


#endif
