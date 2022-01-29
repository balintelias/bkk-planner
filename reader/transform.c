#include <stdio.h>
#include <stdlib.h>
#include "transform.h"

#define LENGTH 1000

int importStops(stop *stops)
{
    char line[LENGTH];
    FILE *input = fopen("/home/elijah/PROGprojects/bkk/ascii/stops-ascii.csv", "r");
    if (input == NULL)
    {
        printf("couldn't open stops-ascii.csv.");
        return -1;
    }
    while (fgets(line, LENGTH, input) != NULL)
    {
        /* code */
    }
    
    
    
    //salalala ez a remote file
    
}

int importAgency();

int importCalendarDates();

int importFeedInfo();

int import();
