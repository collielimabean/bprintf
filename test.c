#include "bprintf.h"
#include <stdio.h>

char buf[20];

int main(void)
{
    int bytes_written;
    int i;
    
    bytes_written = bprintf(buf, 20, "csi", Big, 0xAB, 0xBAAD, 0xDEADBEEF);
    if (bytes_written <= 0)
    {
        printf("Failed to parse?\n");
        return 1;
    }
    
    for (i = 0; i < bytes_written; i++)
        printf("%d: 0x%x\n", i, buf[i]);
    
    return 0;
}
