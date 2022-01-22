/*

The purpose of this next program is to transform a GTFS-formatted set of input files into a single text file.
For example the output may be .xml, .json, or some other markup language.

*/

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <limits.h>
//#include "transform.h"

int main(void)
{
    FILE *input = fopen("readerinput.txt", "r, ccs=UTF-8");
    if (!input)
        return 1;

    wchar_t c;
    c = WCHAR_MAX;
    printf("%04X\n", c);
    printf("%ld\n", sizeof(wchar_t));
    while (c = fgetwc(input), c != WEOF)
        printf("%04X\n", c);

    fclose(input);
    return 0;
}