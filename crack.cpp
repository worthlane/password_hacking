#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static const char* ORIGIN_PROG  = "test.com";
static const char* CRACKED_PROG = "crack.com";

static const int   NOP          = 144;

void CodeCracking(FILE* fpin, FILE* fpout);

//-----------------------------------------------------------------------------------------------------


int main()
{
    FILE* fpin = fopen(ORIGIN_PROG, "r");
    if (!fpin)
    {
        fprintf(stderr, "No \"%s\" found\n", ORIGIN_PROG);
        return 1;
    }

    FILE* fpout = fopen(CRACKED_PROG, "w");
    if (!fpout)
    {
        fprintf(stderr, "Can not create \"%s\"\n", CRACKED_PROG);
        return 1;
    }

    CodeCracking(fpin, fpout);

    fclose(fpin);
    fclose(fpout);

    return 0;
}

//-----------------------------------------------------------------------------------------------------

void CodeCracking(FILE* fpin, FILE* fpout)
{
    assert(fpin);
    assert(fpout);

    for (int i = 0; i < 11; i++)            // skip first 11 bytes
    {
        int byte = fgetc(fpin);
        fputc(byte, fpout);
    }

    for (int i = 0; i < 8; i++)             // replace 8 bytes with NOP
    {
        fgetc(fpin);
        fputc(NOP, fpout);
    }

    int byte = 0;

    while ((byte = fgetc(fpin)) != EOF)     // skip all last bytes
    {
        fputc(byte, fpout);
    }

    fputc(byte, fpout);
}
