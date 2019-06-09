#include "CSVReader.h"
#include "CSVWriter.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringUtils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>


int main(int argc, char* argv[]) {
	for(int i = 0; i < argc; i++) {

    //parse cmd line args
    //input file stream for data

    //load in maps and routes
    CMapRouter MapRouter();

    std::string DataPath = "data/";

    std::ifstream osm(DataPath + "davis.osm");
    std::ifstream stops(DataPath + "stops.csv");
    std::ifstream routes(DataPath + "routes.csv");

    MapRouter.LoadMapAndRoutes;

    std::cout << ">";
    std::string input;
    //std::cin << input;
    std::vector<TPathStep> path;

    while (input != "exit") {
        //parse typed commands
        std::getline(std::cin, input);
        std::vector<std::string> input = split(input, " ");

        if (input[0] == "help" ){
            std::cout << "findroute [--data=path | --resutls=path]
------------------------------------------------------------------------
help     Display this help menu
exit     Exit the program
count    Output the number of nodes in the map
node     Syntax \"node [0, count)\"
         Will output node ID and Lat/Lon for node
fastest  Syntax \"fastest start end\"
         Calculates the time for fastest path from start to end
shortest Syntax \"shortest start end\"
         Calculates the distance for the shortest path from start to end
save     Saves the last calculated path to file
print    Prints the steps for the last calculated path
" << std::endl;
        }

        else if (input[0] == "count") {
            std::cout << MapRouter.NodeCount();
        }

        else if (input[0] == "node") {
            stringstream i(input[1]);
            int index = 0;
            i >> index;
            CMapRouter::TnodeIndex node = nodes[index];
            CMapRouter::TNodeID nodeID = node.nodeid;
            CMapRouter::TLocation Location (node.location.first, node.locaton.second);

            std::cout << "Node " << index << ":" << "id = " << nodeID << " is at "
            << Location.first << ", " << Location.second << std::endl;

        }
        else if (input[0] == "fastest"){
            stringstream s(input[1]);
            CMapRouter::TNodeID src = 0;
            s >> src;

            stringstream d(input[2]);
            CMapRouter::TNodeID dest = 0;
            d >> dest;

            double fastest = 0;
            fastest = MapRouter.FindFastestPath(src, dest, &path);
            std::cout << "Fastest path takes " << fastest << std::endl;

        }
        else if (input[0] == "shortest") {
            stringstream s(input[1]);
            CMapRouter::TNodeID src = 0;
            s >> src;

            stringstream d(input[2]);
            CMapRouter::TNodeID dest = 0;
            d >> dest;

			std::vector<TnodeID> shortestpath;

            double shortest = 0;
            shortest = MapRouter.FindShortestPath(src, dest, &shortestpath);

			for (int i = 0; i < shortestpath.size() - 1; i++) {
				MapRouter.
			}

        }
        else if (input[0] == "save") {


        }
        else if (input[0] == "print") {
            std::vector< std::string > desc;
            MapRouter.GetPathDescription(&path, &desc);



        }

    }
}
