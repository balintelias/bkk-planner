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
} print; //argument of the route printing function

typedef struct time
{
    int hour, min, sec;
} time; //struct for time and timespan

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

int time2sec(time x); //conversion between types

time sec2time(int x); //conversion between types

int isInCities(char name[4], city *cities); //a city lista feltöltéséhez szükséges függvény, logikai értéket ad vissza

city *expandAdjacency(adjacency *new, city *cities, char start[], char destination[]); //expanding a list

city *expandCities(city *new, city *cities); //expanding a list

int isFinished(city *cities); //válaszkereső algoritmus futási feltétele, logikai értéket ad vissza

city *readGraph(FILE *input1, city *cities); //első fájlt beolvasó függvény

city *readDepartures(FILE *input2, city *cities); //második fájlt beolvasó függvény

void printCities(city *cities); //kiírja a lista minden elemének nevét a standard output-ra

city *dijkstraByPrice(city *cities); //válaszkereső algoritmus

city *dijkstraByTime(city *cities); //válaszkereső algoritmus

city *resetGraph(city *cities, char start[]); //a gráfot előkészíti a válaszkereső algoritmusnak

route *flip(route *head); //újraláncol egy listát fordított sorrendben

route *buildRoute(city *cities, char destination[]); //a beállító csúcsok alapján készíti el egy útvonal listáját

int cmpRoutes(route *first, route *second); //útvonalakat hasonlít össze, logikai értéket ad vissza

void printRoute(route *head, print type, time userInput); //a választ kiíró függvény

void freeRoute(route *head); //felszabadít egy útvonal listát

void freeGraph(city *cities); //felszabadítja a gráfot tároló duplán fésűs listát
#endif