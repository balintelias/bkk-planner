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
    char filename[100] = "/home/elijah/PROGprojects/bkk/ascii/stops-test.csv";
    int size = getSize(filename);
    stops = (node *)malloc(size * sizeof(node));
    int n = importStops(stops, size);
    
    return 0;
}