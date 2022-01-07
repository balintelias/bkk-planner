#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>

struct _city;
struct _adjacency;

typedef enum _print
{
    TIME,
    PRICE,
    NONE
} print; //az útvonalat kiíró függvény argumentuma

typedef struct time
{
    int hour, min, sec;
} time; //időpont és időtartam tárolására alkalmas struktúra

typedef struct _city
{
    char name[4];
    int reached;
    int distance;
    struct _city *settingCity;

    struct _adjacency *head;

    struct _city *next;

} city; /*az adatszerkezet külső listája, gerince.
fésűs lista, városokat tárol, a fogak pedig a szomszédos városokat szomszédossági listaként.
*/

typedef struct _departure
{
    time depart;

    struct _departure *next;
} departure; /*az adatszerkezet belső listája.
Egy járat indulási időpontjait tárolja.*/

typedef struct _adjacency
{
    char id[5];
    city *destination;
    time length;
    int price;

    struct _departure *depart;

    struct _adjacency *next;
} adjacency; /* az adatszerkezet közbülső listája.
A city típusú fésűs lista foga, de maga is fésűs lista.
elemei járatok, fogai a járatok indulási időpontjai listában tárolva.
*/

typedef struct _route
{
    city *node;
    int distance;

    struct _route *next;
} route; /*A megkeresett útvonalakat ilyen típusú listákba építjük fel*/

int time2sec(time x); //átváltás típusok között

time sec2time(int x); //átváltás típusok között

int isInCities(char name[4], city *cities); //a city lista feltöltéséhez szükséges függvény, logikai értéket ad vissza

city *expandAdjacency(adjacency *new, city *cities, char start[], char destination[]); //listabővítő függvény

city *expandCities(city *new, city *cities); //listabővítő függvény

int isFinished(city *cities); //válaszkereső algoritmus futási feltétele, logikai értéket ad vissza

city *readGraph(FILE *input1, city *cities);

city *readDepartures(FILE *input2, city *cities);

void printCities(city *cities);

city *dijkstraByPrice(city *cities);

city *dijkstraByTime(city *cities);

city *resetGraph(city *cities, char start[]);

route *flip(route *head);

route *buildRoute(city *cities, char destination[]);

int cmpRoutes(route *first, route *second);

void printRoute(route *head, print type, time userInput);

void freeRoute(route *head);

void freeGraph(city *cities);
#endif