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
    //schematic for reading files:
    node *stops = (node *)malloc(sizeof(node));
    char filename[100] = "/home/elijah/PROGprojects/bkk/ascii/stops-ascii.csv";
    int size = getSize(filename);
    stops = (node *)malloc(size * sizeof(node));
    int n = importStops(stops, size);

    for (int i = 0; i < size; i++)
    {
        printf("%s \n", stops[i].node_id);
        printf("%s ", stops[i].node_name);
        printf("%lf ", stops[i].node_lat);
        printf("%lf\n", stops[i].node_lon);
    }

    //unnecessary comment
    //another one

    return 0;
}