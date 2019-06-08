#ifndef MAPROUTER_H
#define MAPROUTER_H

#include <vector>
#include <istream>
#include <unordered_map>
#include <map>
#include "XMLEntity.h"
#include "XMLReader.h"
#include "CSVReader.h"
class CMapRouter{
    public:
        using TNodeID = unsigned long;
        using TStopID = unsigned long;
        using TLocation = std::pair<double, double>;
        using TPathStep = std::pair<std::string, TNodeID>;
        
        static const TNodeID InvalidNodeID;
    private:
	using TnodeIndex = int;
	struct edge{
		TnodeIndex ConnectedNode;
                TLocation location;
                bool oneway;
                int speed;
                double distance;
		double time;
		bool busedge = false;
	};
	struct node{
	       TNodeID  nodeid;
               TLocation location;
               std::vector<edge>edges;
	};
	struct BusRoutes{
        std::string name;
        std::vector<TnodeIndex>stops;
        };
        struct BusEdge{
        std::vector<std::string>Routes;
        double time;
        std::vector<TnodeIndex>path;
	int distance;
        //time
        //vector path
        //vector routenames

        };
         double dkystra(TnodeIndex src,TnodeIndex dest,std::vector<TNodeID> &path,int searchtype);
        std::vector<node>nodes;
	std::unordered_map<TNodeID, int> position;
        std::vector<unsigned long>SortedIds;
        std::vector<BusRoutes>Routes;
        std::map<unsigned long,TnodeIndex>StopIDToNodeIndex;
        std::map<TnodeIndex,unsigned long>NodeIdToStopID;

	    std::unordered_map<TnodeIndex,unsigned long>NodeIdToIndex;

        std::vector<std::string>BusRouteNames;
        struct pair_hash {
		template <class T1, class T2>
                std::size_t operator () (const std::pair<T1,T2> &p) const {
                auto h1 = std::hash<T1>{}(p.first);
        	auto h2 = std::hash<T2>{}(p.second);

        	// Mainly for demonstration purposes, i.e. works but is overly simple
        	// In the real world, use sth. like boost.hash_combine
        	return h1 ^ h2;  
    		}
 	};

        std::map<std::string,BusRoutes>map;
        using busedge = std::pair<TnodeIndex,TnodeIndex>;
	std::unordered_map<busedge,BusEdge,pair_hash>NodeIDsToBusEdge;

    public:
        
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
        // std::make_pair(180.0, 360.0)on error
        TLocation GetSortedNodeLocationByIndex(size_t index) const;
        
        // Returns the location of the node specified by nodeid, returns std::make_pair(180.0, 360.0) on error
        TLocation GetNodeLocationByID(TNodeID nodeid) const;
        
        // Returns the node ID of the stop ID specified
        TNodeID GetNodeIDByStopID(TStopID stopid) const;
        
        // Returns the number of bus routes
        size_t RouteCount() const;
        
        // Returns the name of the bus route specified by the index of the bus routes in sorted order
        std::string GetSortedRouteNameByIndex(size_t index) const;
        
        // Fills the stops vector with the stops of the bus route specified by route
        bool GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops);
        
        
        // Finds the shortest path from the src node to dest node. The list of nodes visited will be filled in path. The return
        // value is the distance in miles, std::numeric_limits::max() is returned if no path exists.
        double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path);
        
        // Finds the fastest path from the src node to dest node. The list of nodes and mode of transit will be filled in path.
        // The return value is the time in hours, std::numeric_limits::max() is returned if no path exists.
        // When a bus can be taken it should be taken for as long as possible. If more than one bus can be taken for the same length,
        // the one with the earliest route name in the sorted route names.
        double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path);
        
        // Returns a simplified set of directions given the input path
        bool GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const;
};

#endif
