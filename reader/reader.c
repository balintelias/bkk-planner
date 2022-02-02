/*

The purpose of this next program is to transform a GTFS-formatted set of input files into a few text files.
One might include the network as a graph and another the departure times of certain trips.
For example the output may be .xml, .json, or some other markup language.

*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "header.h"

int main(void)
{
    node *stops;
    int n = importStops(stops);
    //for (int i = 0; i < n; i++)
    {
        printf("%s\n", stops[0].node_id);
    }

    return 0;
}