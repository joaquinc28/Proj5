#include "CSVReader.h"
#include "CSVWriter.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringUtils.h"
#include "MapRouter.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>


int main(int argc, char* argv[]) {
	std::string ResultsPath;

	for(int i = 1; i < argc; i++) {
		std::string str(argv[i]);
		std::vector<std::string> arg = StringUtils::split(arg, "/");
		for (int j = 0; j < arg.size(); i++) {
			if (arg[j] == "--results=") {
				for (int k = 0; k < arg.size(); k++) {
					ResultsPath = ResultsPath + arg[k];
				}
			}
		}

	}
	std::cout << argv[0] << "yee" << std::endl;
    //parse cmd line args
    //input file stream for data

    //load in maps and routes
    CMapRouter MapRouter();

    std::string DataPath = "data/";

    std::ifstream osm(DataPath + "davis.osm");
    std::ifstream stops(DataPath + "stops.csv");
    std::ifstream routes(DataPath + "routes.csv");

    MapRouter().LoadMapAndRoutes;

    std::cout << ">";
    std::string input;
    //std::cin << input;
    std::vector<CMapRouter::TPathStep> FastestPath;
	std::vector<CMapRouter::TNodeID> ShortestPath;
	int PathType = 0;
	CMapRouter::TNodeID node1;
	CMapRouter::TNodeID node2;
	double DistTime;

	std::getline(std::cin, input);
	std::vector<std::string> input = StringUtils::split(input, " ");

    while (input != "exit") {

        if (input[0] == "help" ){
            std::cout << "findroute [--data=path | --resutls=path] \n"
"------------------------------------------------------------------------\n"
"help     Display this help menu\n"
"exit     Exit the program\n"
"count    Output the number of nodes in the map/\n"
"node     Syntax \"node [0, count)\"\n"
"         Will output node ID and Lat/Lon for node\n"
"fastest  Syntax \"fastest start end\"\n"
"         Calculates the time for fastest path from start to end\n"
"shortest Syntax \"shortest start end\"\n"
"         Calculates the distance for the shortest path from start to end\n"
"save     Saves the last calculated path to file\n"
"print    Prints the steps for the last calculated path\n"
 << std::endl;
        }

        else if (input[0] == "count") {
            std::cout << MapRouter().NodeCount();
        }

        else if (input[0] == "node") {
            std::stringstream i(input[1]);
            int index = 0;
            i >> index;
			CMapRouter::TNodeID nodeID = MapRouter().GetSortedNodeIDByIndex(index);
			CMapRouter::TLocation Location = MapRouter().GetSortedNodeLocationByIndex(index);

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
            fastest = MapRouter().FindFastestPath(src, dest, FastestPath);
			DistTime = fastest;
			PathType = 0;
            std::cout << "Fastest path takes " << fastest << std::endl;

        }
        else if (input[0] == "shortest") {
            stringstream s(input[1]);
            CMapRouter::TNodeID src = 0;
            s >> src;
			node1 = src;

            stringstream d(input[2]);
            CMapRouter::TNodeID dest = 0;
            d >> dest;
			node2 = src;

            double shortest = 0;
            shortest = MapRouter().FindShortestPath(src, dest, ShortestPath);
			DistTime = shortest;
			PathType = 1



        }
        else if (input[0] == "save") {

			//output should look like: "/home/juaco28/Proj5/62264034_95712808_0.833378hr.csv".
			std::vector< std::string > desc;
            desc = MapRouter().GetPathDescription(path, desc);

			std::string filename;
            filename = ResultsPath + "/" + node1 + "_" + node2 + "_" + DistTime ".csv";

			std::ofstream myfile;
			CCSVWriter writer(myfile);
			std::vector<std::string> headers;
			headers.push_back("mode");
			headers.push_back("node_id");
			writer.WriteRow(headers);

			for (int i = 0; i < desc.size(); i++) {
				writer.WriteRow(desc[i]);

			}



        }
        else if (input[0] == "print") {
            std::vector< std::string > desc;
			if (PathType == 0) {
            	MapRouter().GetPathDescription(FastestPath, desc);
				for (int i = 0; i < desc.size(); i++) {
					std::cout << desc[i] << std::endl;
				}
			}
			else if (PathType == 1) {
				MapRouter().GetPathShortest(ShortestPath, desc);
				for (int i = 0; i < desc.size(); i++) {
					std::cout << desc[i] << std::endl;
				}

			}
			else{

			}
        }

    }
	return 0;
}
