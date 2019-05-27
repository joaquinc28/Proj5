# Proj5

Navigation is a critical to modern society. Once source and destination coordinates are translated to vertices, the shortestor fastest path can be calculated to route the user to their destination.

The goal of your project is to write a program that will be able to parse an OpenStreetMap (OSM) file for an area and then to find shortest/fastest routes as fast as possible. Additionally,the goal is to include bus routes in your search for fastest routes. 

You will be building a program that can find paths, print and save them. OSM files are XML format(more information about OSM can be found at https://wiki.openstreetmap.org/wiki/OSM_XML), so they are human readable and are easily parsed. The bus stop and routes files are CSV and each have two columns. The stops translate stop numbers to node ID numbers in the OSM file. The bus route file lists the name of the bus route followed by the stop number. The stops are listed in order in the route file. 

It is highly advised that you use your CCSVReader, CCSVWriter, CXMLReader, and CXMLWriterfrom project 4.A working example canbe found on the CSIF in /home/cjnitta/ecs34/findroute.  Your program is expected to have the same interface as the working example.The full interface can be listed by typing help after launching the program. A program that can convert saved paths into KML files has also been provided /home/cjnitta/ecs34/kmlout. You can run the KML converter without arguments and it will create a KML file for each bus route. 

Directions of how to view a KML file in google maps can be found here https://www.youtube.com/watch?v=1HqQuHeGa38.The class you will be developing is the CMapRouter. You must keep the existing public interface to the CMapRouter. You may add any private data membersor functions that you choose.

// Constructor of the CMapRouter  
CMapRouter();

// Destructor of the CMapRouter  
~CMapRouter();

// Provided function to calculate the distance in miles  
static double HaversineDistance(double lat1, double lon1, double lat2, double lon2);

// Provided function to calculate the bearing in degrees  
static double CalculateBearing(double lat1, double lon1,double lat2, double lon2);

// Loads the map, stops, and routes given the input streams  
bool LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes);

// Returns the number of nodes read in by the osm file  
size_t NodeCount() const;

// Returns the node ID of the node specified by the index of the nodes in sorted order  
TNodeID GetSortedNodeIDByIndex(size_t index) const;

// Returns the location of the node that is returned by GetSortedNodeIDByIndex when passed index, returns  
// std::make_pair(180.0, 360.0)on errorTLocation   
GetSortedNodeLocationByIndex(size_t index) const;

// Returns the location of the node specified by nodeid, returns std::make_pair(180.0, 360.0) on errorTLocation     GetNodeLocationByID(TNodeID nodeid) const;

// Returns the node ID of the stop ID specifiedTNodeID  
GetNodeIDByStopID(TStopID stopid) const;

// Returns the number of bus routessize_t   
RouteCount() const;

// Returns the name of the bus route specified by the index of the bus routes in sorted order  
std::string GetSortedRouteNameByIndex(size_t index) const;

// Fills the stops vector with the stops of the bus route specified by route  
bool GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops);

// Finds the shortest path from the src node to dest node. The list of nodes visited will be filled in path. The return  
// value is the distance in miles, std::numeric_limits<double>::max() is returned if no path exists.  
double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path);

// Finds the fastest path from the src node to dest node. The list of nodes and mode of transit will be filled in path.   
// The return value is the time in hours, std::numeric_limits<double>::max() is returned if no path exists.  
// When a bus can be taken it should be taken for as long as possible. If more than one bus can be taken for the same length,   
// the one with the earliest route name in the sorted route names.  
double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path);  

// Returns a simplified set of directions given the input path  
bool GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const;

Important assumptions for the project:
  - Bus travels at the speed limit of the street. If a speed limit is not specified for the way, it is assumed that it is 25mph.
  - People walk at 3mph when not on the bus.
  - Each segment of the bus route (between each bus stop) takes additional 30s, assume that the bus stops at each bus stop for 30s.
  
A rough outline for approaching this project might be:  
  1. Design how you want to hold the vertex/edge information. Once you have this done, you can start on reading in the OSM file.You may       want to have some translation from the node IDs provided in the OSM file to your internal node IDs (or indices), this will help to       simplify your code.
  
  2. The Haversine formula has been provided to calculate the distance between two points on the globe. This will be useful for               determining the edge length in miles.You will probably use this during the loading of the ways from the OSM.
  
  3. Once you have read in the OSM file, you should read in the bus stop data and the bus route data. Consider how you willintegrate the     bus stop data into the vertex/edge information.  You will also need to find the fastest bus path for the bus route.You will probably     work on that after you have worked on the next step.
  
  4. Once you can load the data you will want to start on the shortest path search (the fastest path is identical, except your edge           weights will be time instead of distance). There are a lot of shortest path algorithms to choose from, it is up to you to implement     one that correctly finds the shortest path. You maywant to also consider that since the algorithm is identical for shortest or           fastest path, you will probably want to parameterize your code so that you don’t have multiple functions to maintain when you are       optimizing your code.
  
  5. Once you can construct a path, the last step is to be able to convert the list of node IDs and transit method back into steps. Make     sure not to list taking the same bus multiple times in a row.Test the working findrouteprogram using printcommand to see how the         steps should be converted into strings.
  
Files provided:
  - davis.osm –The Open Street Map file of Davis (actually a simplified version from original)
  - stops.csv –The bus stop file, has conversions from stop ID to node ID
  - routes.csv –The bust stop routes, has the name of the route and the stop ID associated with the route (route stops are in order by      rows)
  - MapRouter.h –The header for the CMapRouter class
  - MapRouter.cpp –The cpp for the CMapRouter class
  - testrouter.cpp –A google test that tests the functionality of the CMapRouter
