#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#define LENGTH 1000

//solve it with double indirection
/*int importStops(node *stops)
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
    //return linesCount;

    char line[LENGTH];
    int i;
    stops = (node*)realloc(stops, linesCount*sizeof(node));
    input = fopen("/home/elijah/PROGprojects/bkk/ascii/stops-test.csv", "r");
    fgets(line, LENGTH, input); //delete first line
    printf("first line: %s\n", line);
    memset(line, '\0', LENGTH);
    printf("first line after: %s\n\n", line);
    for (i = 0; i < linesCount; i++)
    {
        fgets(line, LENGTH, input);
        strncpy(stops[i].node_id, line, 6);
        stops[i].node_id[6] = '\0';
        memmove(line, line + 7, strlen(line));
        //printf("%s\n", stops[i].node_id);
        //printf("%s\n", line);
        memset(line, '\0', LENGTH);
    }
    return linesCount;
}*/

//solve it w getSize fv
int getSize(char *filename)
{
    FILE *input = fopen(filename, "r");
    if (input == NULL)
    {
        printf("couldn't open %s.\n", filename);
        return -1;
    }
    printf("file opened\n"); //dbg
    char ch;
    int linesCount = 0;
    while ((ch = fgetc(input)) != EOF)
    {
        if (ch == '\n')
            linesCount++;
    }
    printf("lines: %d\n", linesCount); //dbg
    fclose(input);
    return linesCount;
}

int importStops(node *stops, int linesCount)
{
    FILE *input = fopen("/home/elijah/PROGprojects/bkk/ascii/stops-ascii.csv", "r");
    char line[LENGTH];
    fgets(line, LENGTH, input);

    for (int i = 0; i < linesCount; i++)
    {
        fgets(line, LENGTH, input);

        int toRead = strchr(line, ',') - line + 1;
        strncpy(stops[i].node_id, line, toRead);
        stops[i].node_id[toRead] = '\0';
        memmove(line, line + toRead, strlen(line) - toRead);

        if (line[0] == '"')
        {
            //read until another "
            int index = 1;
            while (line[index] != '"')
            {
                stops[i].node_name[index - 1] = line[index];
                index++;
            }
            stops[i].node_name[index - 1] = '\0';
            memmove(line, line + index, strlen(line) - index);
            memmove(line, line + 2, strlen(line) - 2);
        }
        else
        {
            //read until ,
            int index = 0;
            while (line[index] != ',')
            {
                stops[i].node_name[index] = line[index];
                index++;
            }
            stops[i].node_name[index] = '\0';
            memmove(line, line + index + 1, strlen(line) - index - 1);
        }

        char *bfr;
        stops[i].node_lat = strtod(line, &bfr);
        bfr = bfr + 1;
        strcpy(line, bfr);
        stops[i].node_lon = strtod(line, &bfr);
        bfr = bfr + 1;
        strcpy(line, bfr);

        memset(line, '\0', LENGTH);
    }
    fclose(input);
    return 0;
}

//int importAgency();

//int importCalendarDates();

//int importFeedInfo();

//int import();
