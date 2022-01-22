#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>
#include "header.h"

int time2sec(time x) //conversion between types
{
    int sum = 0;
    sum = 3600 * x.hour + 60 * x.min + x.sec;
    return sum;
}

time sec2time(int x) //conversion between types
{
    time length;
    length.hour = x / 3600;
    x = x % 3600;
    length.min = x / 60;
    x = x % 60;
    length.sec = x;

    return length;
}