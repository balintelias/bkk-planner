/*

2021. 12. 06.
BMEVIHIAA01 - A programozás alapjai 1
Házi feladat

Bálint Éliás, G5RXH0, 3. IMSc tankör
Útvonaltervező program

A program két bemeneti fájlból épít fel egy buszhálózatot menetrenddel együtt,
majd a felhasználói kérésből kiszámítja a legolcsóbb útvonalat,
illetve azt, amelyben az utak időbeli hossza a legrövidebb.
Amennyiben ezek megegyeznek, kiírja az útvonalat, amennyiben a kettő különbözik,
kiírja a legolcsóbbat az árával együtt, az időben rövidebbet pedig a járatokon töltött idő összegével,
illetve ekkor a program kiírja az első busz indulásáig hátralévő időt is.

*/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
//#include "debugmalloc.h"

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

int time2sec(time x) //átváltás típusok között
{
    int sum = 0;
    sum = 3600 * x.hour + 60 * x.min + x.sec;
    return sum;
}

time sec2time(int x) //átváltás típusok között
{
    time length;
    length.hour = x / 3600;
    x = x % 3600;
    length.min = x / 60;
    x = x % 60;
    length.sec = x;

    return length;
}

int isInCities(char name[4], city *cities) //a city lista feltöltéséhez szükséges függvény, logikai értéket ad vissza
{
    city *p = cities; //futóindex a lista bejárásához
    if (cities == NULL)
    {
        return 0;
    }
    while (p != NULL)
    {
        if (!strcmp(p->name, name))
            return 1;
        p = p->next;
    }
    return 0;
}

city *expandAdjacency(adjacency *new, city *cities, char start[], char destination[]) //listabővítő függvény
{
    city *p = cities; //futóindex a bejáráshoz
    adjacency *q;     //futóindex a bejáráshoz

    while (p != NULL) //new->destination pointer beállítása
    {
        if (!strcmp(destination, p->name))
        {
            new->destination = p;
        }
        p = p->next;
    }

    p = cities;

    while (p != NULL) //new-t a szomszédossági listába láncolás
    {
        if (!strcmp(start, p->name))
        {
            if (p->head == NULL) //üres szomszédossági lista lekezelése
            {
                p->head = new;
                return cities;
            }
            q = p->head;

            while (q->next != NULL) //new-t a szomszédossági lista végére láncolás
            {
                q = q->next;
            }
            q->next = new;
            return cities;
        }
        p = p->next;
    }
}

city *expandCities(city *new, city *cities) //listabővítő függvény
{
    if (cities == NULL) //üres lista lekezelése
    {
        cities = new;
        return cities;
    }
    city *p = cities;
    while (p->next != NULL) //new-t a lista végére láncoljuk
    {
        p = p->next;
    }
    p->next = new;
    return cities;
}

int isFinished(city *cities) //válaszkereső algoritmus futási feltétele, logikai értéket ad vissza
{
    city *p = cities;
    while (p != NULL)
    {
        if (p->reached == 0)
        {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

city *readGraph(FILE *input1, city *cities) //első fájlt beolvasó függvény
{
    char start[4], destination[4], id[5], x;
    int h, m, s, price, citiesSize = 0, n = 0;
    fscanf(input1, "%d\n", &n);
    for (int i = 0; i < n; i++)
    {
        fscanf(input1, "%s %s %s %d %d %d %d\n", start, destination, id, &h, &m, &s, &price);
        /*
    
        A fájl első sora az utána lévő sorok száma
        Ezt követően így néz ki a fájl:
        kiindulópont cél    járatazonosító óra perc másodperc ár
        string       string string         int int  int       int
    
        */
        if (!isInCities(start, cities)) //ha nincs még ilyen city a listában, bővítem azt
        {
            city *newCity;
            newCity = (city *)malloc(sizeof(city));
            if (newCity == NULL)
            {
                printf("A malloc() fuggveny nem tudott megfelelo meretu memoriateruletet foglalni\n");
                return NULL;
            }
            //newCity megfelelő kitöltése, majd listába láncolás
            strcpy(newCity->name, start);
            newCity->reached = 0;
            newCity->distance = INT_MAX - 1;
            newCity->settingCity = NULL;
            newCity->head = NULL;
            newCity->next = NULL;
            cities = expandCities(newCity, cities);
        }
        if (!isInCities(destination, cities)) //ha nincs még ilyen city a listában, bővítem azt
        {
            city *newCity;
            newCity = (city *)malloc(sizeof(city));
            if (newCity == NULL)
            {
                printf("A malloc() függvény nem tudott megfelelő méretű memóriaterületet foglalni\n");
                return NULL;
            }
            //newCity megfelelő kitöltése, majd listába láncolás
            strcpy(newCity->name, destination);
            newCity->reached = 0;
            newCity->distance = INT_MAX - 1;
            newCity->settingCity = NULL;
            newCity->head = NULL;
            newCity->next = NULL;
            cities = expandCities(newCity, cities);
        }

        /* új járat létrehozása a beolvasott adatokból
            = szomszédossági lista bővítése */
        adjacency *newAdjacency;
        time length;
        length.hour = h;
        length.min = m;
        length.sec = s;
        newAdjacency = (adjacency *)malloc(sizeof(adjacency));
        if (newAdjacency == NULL)
        {
            printf("A malloc() függvény nem tudott megfelelő méretű memóriaterületet foglalni\n");
            return NULL;
        }
        //newAdjacency megfelelő kitöltése, majd listába láncolás
        strcpy(newAdjacency->id, id);
        newAdjacency->length = length;
        newAdjacency->price = price;
        newAdjacency->depart = NULL;
        newAdjacency->next = NULL;
        cities = expandAdjacency(newAdjacency, cities, start, destination);
    }

    return cities;
}

city *readDepartures(FILE *input2, city *cities) //második fájlt beolvasó függvény
{
    char id[5], x;
    int n;
    time depart;
    city *p;      //futóindex a tárolt gráf bejárásához
    adjacency *q; //ez is
    fscanf(input2, "%d\n", &n);
    for (int i = 0; i < n; i++)
    {
        fscanf(input2, "%s %d %d %d", id, &depart.hour, &depart.min, &depart.sec);
        /*
    
        A fájl első sora az utána lévő sorok száma
        Ezt követően így néz ki a fájl:
        járatazonosító óra perc másodperc
        string         int int  int      
    
        */
        int condition = 1; //futási feltétel, a megfelelő adjacency-nél 0-ra állítjuk, a while()-ok megszakadnak

        p = cities;
        while (condition && p != NULL)
        {
            q = p->head;
            while (condition && q != NULL)
            {
                if (!strcmp(id, q->id))
                {
                    //traverse to the end of departure, and link depart
                    departure *new, *r = q->depart;
                    new = (departure *)malloc(sizeof(departure));
                    new->depart = depart;
                    new->next = NULL;
                    if (r == NULL) //üres lista lekezelése
                    {
                        q->depart = new;
                    }
                    else //new-t a lista végére láncoljuk
                    {
                        while (r->next != NULL)
                        {
                            r = r->next;
                        }
                        r->next = new;
                    }

                    condition = 0;
                }
                if (condition) //csak akkor kell léptetni, ha nem láncoltunk elemet
                    q = q->next;
            }
            if (condition) //csak akkor kell léptetni, ha nem láncoltunk elemet
                p = p->next;
        }
    }
    return cities;
}

void printCities(city *cities) //kiírja a lista minden elemének nevét a standard output-ra
{
    city *p = cities;
    while (p != NULL)
    {
        printf("%s\n", p->name);
        p = p->next;
    }
}

city *dijkstraByPrice(city *cities) //válaszkereső algoritmus
{
    city *p, *current;
    adjacency *q;
    /*
    
    Aktuális csúcs, a legkisebb távolsággal rendelkező csúcs, amely még nincs elérve.
    Minden iterációban keresünk egy aktuális csúcsot, a szomszédai távolságát élek mentén javítjuk, majd a csúcsot elértté tesszük.
    Erre azért van lehetőség, mert az ő távolsága már nem csökkenhet.
    
    */
    while (isFinished(cities)) //Aktuális csúcs megkeresése
    {
        p = cities;
        int minimum = INT_MAX;
        while (p != NULL)
        {
            if (p->distance < minimum && !(p->reached))
            {
                minimum = p->distance;
                current = p;
            }
            p = p->next;
        }

        //szomszédossági lista bejárása az aktuális csúcsból
        q = current->head;

        while (q != NULL)
        {
            int calculated;
            calculated = current->distance + q->price;
            if (calculated < (q->destination)->distance)
            {
                /*
                Amennyiben a számított távolság kisebb, mint az eddig ismert távolság,
                élmenti javításokkal felülírjuk a távolságot, majd az aktuális csúcsot adjuk meg beállító csúcsként.
                */
                (q->destination)->distance = calculated;
                (q->destination)->settingCity = current;
            }
            q = q->next;
        }

        current->reached = 1;
    }
    return cities;
}

city *dijkstraByTime(city *cities) //válaszkereső algoritmus
{
    /*ugyanazon az elven működik, mint az előző függvény,
    csak ár helyett idő alapján számítja a csúcsok távolságát*/
    city *p, *current;
    adjacency *q;

    //aktuális csúcs megkeresése
    while (isFinished(cities))
    {
        p = cities;
        int minimum = INT_MAX;
        while (p != NULL)
        {
            if (p->distance < minimum && !(p->reached))
            {
                minimum = p->distance;
                current = p;
            }
            if (p != NULL)
                p = p->next;
            else
                break;
        }

        //szomszédossági lista bejárása az aktuális csúcsból
        q = current->head;

        while (q != NULL)
        {
            int calculated;
            calculated = current->distance + time2sec(q->length);
            if (calculated < (q->destination)->distance)
            {
                //élmenti javítás
                (q->destination)->distance = calculated;
                (q->destination)->settingCity = current;
            }
            q = q->next;
        }

        current->reached = 1;
    }
    return cities;
}

city *resetGraph(city *cities, char start[]) //a gráfot előkészíti a válaszkereső algoritmusnak
{
    city *p = cities;
    while (p != NULL) //távolság és elértség visszaállítása
    {
        p->distance = INT_MAX - 1;
        p->reached = 0;
        p->settingCity = NULL;
        p = p->next;
    }
    p = cities;
    while (1) //kezdőcsúcs távolsága legyen 0
    {
        if (!strcmp(start, p->name))
        {
            p->distance = 0;
            break;
        }
        p = p->next;
    }

    return cities;
}

route *flip(route *head) //újraláncol egy listát fordított sorrendben
{
    route *p = NULL;
    route *q = head;
    while (q != NULL)
    {
        route *r = q->next;
        q->next = p;
        p = q;
        q = r;
    }
    return p;
}

route *buildRoute(city *cities, char destination[]) //a beállító csúcsok alapján készíti el egy útvonal listáját
{
    city *p = cities;
    while (p != NULL) //p
    {
        if (!strcmp(destination, p->name))
        {
            break;
        }
        p = p->next;
    }

    route *head, *q = NULL, *new;          //q a listaépítéséhez használt pointer
    head = (route *)malloc(sizeof(route)); //az útvonal elejét cikluson kívül állítjuk be
    if(head == NULL)
    {
        return NULL;
    }
    head->node = p;
    head->distance = p->distance;
    head->next = NULL;
    p = p->settingCity;
    q = head;

    while (p != NULL) //minden beállító csúcsot listába láncol
    {
        new = (route *)malloc(sizeof(route));
        if(new == NULL)
        {
            return NULL;
        }
        new->node = p;
        new->distance = p->distance;
        new->next = NULL;
        p = p->settingCity;
        q->next = new;
        q = q->next;
    }

    head = flip(head); //a listánk a célból a kiindulópontba mutatja az útvonalat, meg kell fordítani

    return head;
}

int cmpRoutes(route *first, route *second) //útvonalakat hasonlít össze, logikai értéket ad vissza
{
    route *a = first, *b = second;
    while (a != NULL && b != NULL)
    {
        if (strcmp(a->node->name, b->node->name))
            return 0;
        if (a->next == NULL && b->next != NULL)
            return 0;
        if (a->next != NULL && b->next == NULL)
            return 0;
        if (a->next == NULL && b->next == NULL)
            return 1;
        a = a->next;
        b = b->next;
    }
}

void printRoute(route *head, print type, time userInput) //a választ kiíró függvény
{
    /*
    1. rész:
    Az útvonal állomásainak (városainak) kiírása.
    */
    route *p = head, *q;
    while (p != NULL)
    {
        printf("%s ", p->node->name);
        q = p;
        p = p->next;
    }
    /*
    2. rész:
    A hívó által megadott módon írja ki az útvonal távolságát.
    q->distance az utolsó elem távolsága lehet ár, vagy az útvonalak összideje másodpercben mérve.
    */
    if (type == PRICE) //kiírás ár alapján
    {
        printf("Ara: %d HUF\n", q->distance);
    }
    else if (type == TIME) //kiírás idő alapján. Ekkor a program megadja az első járat indulásáig hátralévő időt is.
    {
        time length = sec2time(q->distance);
        printf("Buszutak osszhossza: %d:%d:%d\n", length.hour, length.min, length.sec);
        time timeUntil;
        p = head;
        departure *r, *s; //futóindexek
        //r megkereséséhez
        adjacency *a = p->node->head;
        while (strcmp(a->destination->name, p->next->node->name))
        {
            a = a->next;
        }
        r = a->depart;
        if(r == NULL)
        {
            printf("A jarat nem kozlekedik, a bemeneti fajl nem volt megfeleloen kitoltve.\n");
            return;
        }
        while (time2sec(r->depart) < time2sec(userInput))
        {
            r = r->next;
            if(r == NULL)
            {
                printf("Az elso jarat mar nem indul ma.\n");
                return;
            }
        }
        timeUntil = sec2time(time2sec(r->depart) - time2sec(userInput));
        printf("Ido indulasig: %d:%d:%d\n", timeUntil.hour, timeUntil.sec, timeUntil.sec);
    }
}

void freeRoute(route *head) //felszabadít egy útvonal listát
{
    route *p;

    while (head != NULL)
    {
        p = head;
        head = head->next;
        free(p);
    }
}

void freeGraph(city *cities) //felszabadítja a gráfot tároló duplán fésűs listát
{
    city *p;
    adjacency *q, *r;
    departure *s, *t;

    while (cities != NULL) //külső listát felszabadító ciklus
    {
        q = cities->head;
        while (q != NULL) //közbülső listát felszabadító ciklus
        {
            s = q->depart;
            while (s != NULL) //legbelső listát felszabadító ciklus
            {
                t = s;
                s = s->next;
                free(t);
            }

            r = q;
            q = q->next;
            free(r);
        }
        p = cities;
        cities = cities->next;
        free(p);
    }
}

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
