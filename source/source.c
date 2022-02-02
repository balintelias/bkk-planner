/*
BKK route planner

The program is based on a homework from Basics of Programming 1 in Electrical Engineering BSc.

The original program built a bus network with departures, and then found the cheapest and the fastest route between two cities, 
where fastest meant that the route with the least amount of time spent travelling (without changes).
In the case of these two routes being the same, it gave back the route itself, if the two were different,
it gave back both with their price/time and for the latter it gave the time until the first departure.

The problems include: the lines travel between two points, and I only have access to the network in GTFS-format.
I need to study the documentation of GTFS before starting to work on the new (and actually useful) version.
The source of GTFS-input: https://bkk.hu/gtfs/budapest_gtfs.zip
*/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
//#include "debugmalloc.h"
#include "header.h"

int main(void)  
{
    FILE *input1, *input2;
    char filename1[100], filename2[100];
    printf("Adja meg az elso bemeneti fajl nevet: ");
    scanf("%s", filename1);
    input1 = fopen(filename1, "r");
    if (input1 == NULL)
    {
        printf("Nem sikerult megnyitni az elso fajlt.\n");
        return 1;
    }
    printf("Adja meg a masodik bemeneti fajl nevet: ");
    scanf("%s", filename2);
    input2 = fopen(filename2, "r");
    if (input2 == NULL)
    {
        printf("Nem sikerult megnyitni a masodik fajlt.\n");
        return 2;
    }

    city *cities = NULL;

    cities = readGraph(input1, cities);
    cities = readDepartures(input2, cities);
    if(cities == NULL)
    {
        printf("Nem sikerult megfeleloen beolvasni a bemeneti fajlok adatait.\n");
        return 6;
    }

    char start[4], destination[4]; //felhasználói bemenet
    time departure;
    printf("Adja meg a kiindulo varost, a celvarost es a tervezett indulas idopontjat szokozzel elvalasztva.\nValaszthato varosok:\n");
    printCities(cities);
    scanf("%s %s %d %d %d", start, destination, &departure.hour, &departure.min, &departure.sec);

    if (!isInCities(start, cities) || !isInCities(destination, cities))
    {
        printf("Ervenytelen bemenet.\n");
        return 3;
    }
    if (!strcmp(start, destination))
    {
        printf("Ervenytelen bemenet.\n");
        return 4;
    }
    
    city *p = cities; //dijkstra előkészítése: kezdőcsúcs távolsága legyen 0
    while (p != NULL)
    {
        if (!strcmp(start, p->name))
        {
            p->distance = 0;
            break;
        }
        p = p->next;
    }

    cities = dijkstraByPrice(cities); //válaszkereső algoritmus: dijkstra jegyár szerint

    route *routeByPrice; //a megtalált legolcsóbb útvonal felépítése listába
    routeByPrice = buildRoute(cities, destination);

    cities = resetGraph(cities, start); //a gráf visszaállítása a kezdőállapotba, majd kezdőcsúcs távolsága legyen 0

    cities = dijkstraByTime(cities); //válaszkereső algoritmus: dijkstra idő szerint

    route *routeByTime; //a megtalált "legrövidebb" útvonal felépítése listába
    routeByTime = buildRoute(cities, destination);

    if (routeByPrice == NULL || routeByTime == NULL)
    {
        printf("Nem sikerult utvonalat talalni.\n");
        return 5;
    }

    if (cmpRoutes(routeByPrice, routeByTime)) //az útvonalak összevetése majd kiírás
    {
        printf("Egy utvonal:\n");
        printRoute(routeByPrice, NONE, departure);
    }
    else
    {
        printf("Ket utvonal:\nA legolcsobb:\n");
        printRoute(routeByPrice, PRICE, departure);
        printf("Legrovidebb ido:\n");
        printRoute(routeByTime, TIME, departure); //majdnem jó, csak írja ki, hogy mennyi idő telik el a első busz indulásáig.
    }
    freeRoute(routeByTime);
    freeRoute(routeByPrice);
    freeGraph(cities);
    return 0;
}
