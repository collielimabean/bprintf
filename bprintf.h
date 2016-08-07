#ifndef _BPRINTF_H
#define _BPRINTF_H

enum Endianness
{
    Big, Little
};

int bprintf(
    char *buffer,
    int size, 
    const char *fmt, 
    Endianness endianness,
    ...
);

#endif