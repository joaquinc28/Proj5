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
	std::string ResultsPath = "results/";

	for(int i = 1; i < argc; i++) {
		std::string str(argv[i]);
		std::vector<std::string> arg = StringUtils::Split(str, "/");
		for (int j = 0; j < arg.size(); i++) {
			if (arg[j] == "--results=") {
				for (int k = 0; k < arg.size(); k++) {
					ResultsPath = ResultsPath + arg[k];
				}
			}
		}

	}

    //parse cmd line args
    //input file stream for data

    //load in maps and routes
    CMapRouter MapRouter;

    std::string DataPath = "data/";

    std::ifstream osm(DataPath + "davis.osm");
    std::ifstream stops(DataPath + "stops.csv");
    std::ifstream routes(DataPath + "routes.csv");

    MapRouter.LoadMapAndRoutes(osm, stops, routes);

    std::cout << ">";

	printf("Line %d\n", __LINE__);

	std::string input2;

	printf("Line %d\n", __LINE__);

	std::vector<CMapRouter::TPathStep> FastestPath;
	std::vector<CMapRouter::TNodeID> ShortestPath;

	printf("Line %d\n", __LINE__);

	int PathType = 0;
	CMapRouter::TNodeID node1;
	CMapRouter::TNodeID node2;
	double DistTime;

	printf("Line %d\n", __LINE__);

	std::getline(std::cin, input2);
	std::cout << "Line "<<__LINE__<<" " << input2 << std::endl;
	std::vector<std::string> input;
	size_t start;
	size_t end = 0;

	while ((start = input2.find_first_not_of(" ", end)) != std::string::npos) {
		end = input2.find(" ", start);
		printf("Line %d\n", __LINE__);
		input.push_back(input2.substr(start, end - start));
		std::cout << "Line "<<__LINE__<<" " << input[0] << std::endl;
		printf("Line %d\n", __LINE__);
	}
	printf("Line %d\n", __LINE__);


    while (input[0] != "exit") {
		printf("Line %d\n", __LINE__);

		//std::cout << input[0];

		//input = StringUtils::Split(input2);

        if (input[0] == "help"){

			input.clear();
			for (int i = 0; i < input.size(); i++) {
				std::cout << "Line "<<__LINE__<<" " << input[i] << std::endl;
			}
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
            std::cout << MapRouter.NodeCount() << std::endl;
        }

        else if (input[0] == "node") {
            std::stringstream i(input[1]);
            int index = 0;
            i >> index;
			CMapRouter::TNodeID nodeID = MapRouter.GetSortedNodeIDByIndex(index);
			CMapRouter::TLocation Location = MapRouter.GetSortedNodeLocationByIndex(index);

            std::cout << "Node " << index << ": " << "id = " << nodeID << " is at "
            << Location.first << ", " << Location.second << std::endl;

        }
        else if (input[0] == "fastest"){
            std::stringstream s(input[1]);
            CMapRouter::TNodeID src = 0;
            s >> src;

            std::stringstream d(input[2]);
            CMapRouter::TNodeID dest = 0;
            d >> dest;

            double fastest = 0;
            fastest = MapRouter.FindFastestPath(src, dest, FastestPath);
			DistTime = fastest;
			PathType = 0;
            std::cout << "Fastest path takes " << fastest << std::endl;

        }
        else if (input[0] == "shortest") {
            std::stringstream s(input[1]);
            CMapRouter::TNodeID src = 0;
            s >> src;
			node1 = src;

            std::stringstream d(input[2]);
            CMapRouter::TNodeID dest = 0;
            d >> dest;
			node2 = src;

            double shortest = 0;
            shortest = MapRouter.FindShortestPath(src, dest, ShortestPath);
			DistTime = shortest;
			PathType = 1;
			std::cout << "Shortest path is " << shortest << " miles" << std::endl;



        }
        else if (input[0] == "save") {

			//output should look like: "/home/juaco28/Proj5/62264034_95712808_0.833378hr.csv".
			std::vector< std::string > desc;
			if (PathType == 0) {
            	MapRouter.GetPathDescription(FastestPath, desc);
			}
			else {
				MapRouter.GetPathShortest(ShortestPath, desc);
			}
			std::string filename;
            filename = ResultsPath + "/" + std::to_string(node1) + "_" + std::to_string(node2) + "_" + std::to_string(DistTime) + ".csv";

			std::ofstream myfile;
			CCSVWriter writer(myfile);
			std::vector<std::string> headers;
			headers.push_back("mode");
			headers.push_back("node_id");
			writer.WriteRow(headers);

			//for (int i = 0; i < desc.size(); i++) {
				writer.WriteRow(desc);

			//}



        }
        else if (input[0] == "print") {
            std::vector< std::string > desc;
			if (PathType == 0) {
            	MapRouter.GetPathDescription(FastestPath, desc);
				for (int i = 0; i < desc.size(); i++) {
					std::cout << desc[i] << std::endl;
				}
			}
			else {
				MapRouter.GetPathShortest(ShortestPath, desc);
				for (int i = 0; i < desc.size(); i++) {
					std::cout << desc[i] << std::endl;
				}

			}

        }
		std::cout << ">";
		std::getline(std::cin, input2);
		std::cout << "Line "<<__LINE__<<" " << input2 << std::endl;

		input.clear();

		//StringUtils::Split(input2);
		printf("Line %d\n", __LINE__);

		while ((start = input2.find_first_not_of(" ", end)) != std::string::npos) {
			end = input2.find(" ", start);
			input.push_back(input2.substr(start, end - start));
			printf("Line %d\n", __LINE__);
			std::cout << "Line "<<__LINE__<<" " << input2.substr(start, end - start) << std::endl;
			printf("Line %d\n", __LINE__);
			std::cout << "Line "<<__LINE__<<" " << input[0] << std::endl;
			printf("Line %d\n", __LINE__);
		}
		printf("Line %d\n", __LINE__);
    }
	return 0;
}
