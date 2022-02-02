#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#define LENGTH 1000

int importStops(node *stops)
{
    FILE *input = fopen("/home/elijah/PROGprojects/bkk/ascii/stops-test.csv", "r");
    if (input == NULL)
    {
        printf("couldn't open stops-ascii.csv.");
        return -1;
    }
    char ch;
    int linesCount = 0;
    while ((ch = fgetc(input)) != EOF)
    {
        if (ch == '\n')
            linesCount++;
    }
    fclose(input);

    char line[LENGTH];
    int i;
    stops = (node*)malloc(linesCount*sizeof(node));
    input = fopen("/home/elijah/PROGprojects/bkk/ascii/stops-test.csv", "r");
    fgets(line, LENGTH, input); //delete first line
    memset(line, '\0', LENGTH);
    for (i = 0; i < linesCount; i++)
    {
        fgets(line, LENGTH, input);
        strncpy(stops[i].node_id, line, 6);
        memmove(line, line + 7, strlen(line));
        printf("%s\n", stops[i].node_id);
        printf("%s", line);
        memset(line, '\0', LENGTH);
    }
    return i;
}

int importAgency();

int importCalendarDates();

int importFeedInfo();

int import();
