#ifndef HEADER_H
#define HEADER_H

enum node_type
{
    STOP,
    PLATFORM,
    STATION,
    ENTRANCE,
    GENERIC,
    BOARDING
};

typedef struct node
{
    char node_id[7];
    char node_name[100];
    double node_lat, node_lon;
    char node_code[20];
    enum node_type type;
    //location_sub_type;
    int parent_station_index;
    int wheelchair_boarding; //enum?
    int stop_direction;
} node;

typedef struct stop
{
    char stop_id[7];
    char stop_name[100];
    double stop_lat, stop_lon;
    int location_type;
    int location_sub_type;
    char parent_station[7];  //ID referencing stops.stop_id
    int wheelchair_boarding; //enum?
    int stop_direction;
} stop;

typedef struct route
{
    char agency_id[10];
    int route_id;
    char route_short_name[10];
    char route_long_name[50];
    char route_desc[200];
    char route_color[7];
    char route_text_color[7];
    int route_sort_order;
    char route_icon_display_text[10];
} route;

typedef struct pathway
{
    char pathway_id[20];
    int pathway_mode;
    int is_bidirectional;
    char from_stop_id[10];
    char to_stop_id[10];
    int traversal_time;
} pathway;

typedef struct trip
{
    int route_id; //maybe pointer
    char trip_id[20];
    char service_id[30];    //maybe pointer
    char trip_headsign[50]; //UTF-8
    int direction_id;
    char block_id[30];
    char shape_id[10]; //maybe pointer
    int wheelchair_accessible;
    int bikes_allowed;
    int boarding_door;
} trip;

typedef struct stop_time
{

} stop_time;

typedef struct time
{
    int hour, min, sec;
} time; //struct for time and timespan

int getSize(char *filename);

int importStops(node *stops, int linesCount);

#endif