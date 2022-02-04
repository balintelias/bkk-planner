/*

The purpose of this next program is to transform a GTFS-formatted set of input files into a few text files.
One might include the network as a graph and another the departure times of certain trips.
For example the output may be .xml, .json, or some other markup language.

*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "header.h"

int main(void)
{
    node *stops;
    int size = getSize()
    stops = (node*)malloc(size*sizeof(node));
    printf("arraysize: %ld\n", sizeof(stops));
    int n = importStops(stops);
    printf("\n\nreturn value: %d\n", n); //dbg
    //for (int i = 0; i < n; i++)
    printf("arraysize: %ld\n", sizeof(stops));

    return 0;
}