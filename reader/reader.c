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

    /*wint_t buffer;
    wchar_t c;
    buffer = __WINT_MAX__;
    printf("%d\n", buffer);
    printf("%04X\n", buffer);
    printf("%ld\n", sizeof(wint_t));*/
    wint_t c;
    while ((c = getwc(input)) != WEOF)
    {
        printf("%lc", c);
    }
    fclose(input);
    return 0;
}