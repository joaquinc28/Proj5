#include "MapRouter.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <sstream>

const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;

CMapRouter::CMapRouter(){

}

CMapRouter::~CMapRouter(){

}
double CMapRouter::dkystra(CMapRouter::TnodeIndex src, CMapRouter::TnodeIndex dest,
                           std::vector<CMapRouter::TNodeID> &path, int searchtype) {
    std::vector<TnodeIndex > prev(nodes.size());
    std::vector<double>distance(nodes.size(),std::numeric_limits<double>::max());
    std::vector<TNodeID > heap;
    auto compare = [&distance](TnodeIndex idx, TnodeIndex idx2){return distance[idx] < distance[idx2];};

    prev[src] = src;
    distance[src] = 0.0;
    heap.push_back(src);
    while(!heap.empty()){
        std::make_heap(heap.begin(),heap.end(),compare);
        std::pop_heap(heap.begin(),heap.end(),compare);
        auto curr  = heap.back();
        heap.pop_back();
        for(auto &edge: nodes[curr].edges){
            double edgedist;
            switch(searchtype){
                case 0: edgedist = edge.distance;
                        break;
                case 1: edgedist = edge.time;
                        break;
                case 2: edgedist = edge.distance / edge.speed;//speedlimit
                         break;

            }
            auto altdist = distance[curr] + edgedist;
            if(altdist < distance[edge.ConnectedNode]){
                if(distance[edge.ConnectedNode] == std::numeric_limits<double>::max()){
                    heap.push_back(edge.ConnectedNode);
                }
                distance[edge.ConnectedNode] = altdist;
                prev[edge.ConnectedNode] = curr;
            }

        }

    }

    if(distance[dest] == std::numeric_limits<double>::max()){
       return false;
}
   auto temp = dest;
   std::vector<TnodeIndex>path1;
    while(temp != src){
        path1.push_back(temp);
        temp = prev[temp];
    }
   path1.push_back(src);
   std::reverse(path1.begin(),path1.end());
   for(auto &c:path1){
	auto ID = NodeIdToIndex.find(c)->second;
        path.push_back(ID);

    }
   
   return distance[dest];

}

double CMapRouter::HaversineDistance(double lat1, double lon1, double lat2, double lon2){
    auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
	double LatRad1 = DegreesToRadians(lat1);
	double LatRad2 = DegreesToRadians(lat2);
	double LonRad1 = DegreesToRadians(lon1);
	double LonRad2 = DegreesToRadians(lon2);
	double DeltaLat = LatRad2 - LatRad1;
	double DeltaLon = LonRad2 - LonRad1;
	double DeltaLatSin = sin(DeltaLat/2);
	double DeltaLonSin = sin(DeltaLon/2);	
	double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
	const double EarthRadiusMiles = 3959.88;
	
	return 2 * EarthRadiusMiles * Computation;
}        

double CMapRouter::CalculateBearing(double lat1, double lon1,double lat2, double lon2){
    auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
    auto RadiansToDegrees = [](double rad){return 180.0 * (rad) / M_PI;};
    double LatRad1 = DegreesToRadians(lat1);
	double LatRad2 = DegreesToRadians(lat2);
	double LonRad1 = DegreesToRadians(lon1);
	double LonRad2 = DegreesToRadians(lon2);
    double X = cos(LatRad2)*sin(LonRad2-LonRad1);
    double Y = cos(LatRad1)*sin(LatRad2)-sin(LatRad1)*cos(LatRad2)*cos(LonRad2-LonRad1);
    return RadiansToDegrees(atan2(X,Y));
}

bool CMapRouter::LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes){
    CXMLReader reader(osm);
    SXMLEntity TempEnt;

    reader.ReadEntity(TempEnt);
    if(TempEnt.DType != SXMLEntity::EType::StartElement or TempEnt.DNameData != "osm"){
        return false;
    }
    while(!reader.End()){
	    reader.ReadEntity(TempEnt);
	    printf("Not logical value at line number %d \n",__LINE__);
	if(TempEnt.DType == SXMLEntity::EType::EndElement){
	    if(TempEnt.DNameData == "osm"){
		    printf("Not logical value at line number %d \n",__LINE__);
	         break;
	    }
	}	    
        if(TempEnt.DType == SXMLEntity::EType::StartElement){
		printf("Not logical value at line number %d \n",__LINE__);
            if(TempEnt.DNameData == "node"){
		    printf("Not logical value at line number %d \n",__LINE__);
                TNodeID  TempId = std::stoul(TempEnt.AttributeValue("id"));
                double TempLat = std::stoul(TempEnt.AttributeValue("lat"));
                double TempLon = std::stoul(TempEnt.AttributeValue("lon"));
                node TempNode;
                TempNode.nodeid = TempId;
                TempNode.location = std::make_pair(TempLat, TempLon);
                position.emplace(TempId,nodes.size());
		NodeIdToIndex[nodes.size()] = TempId;

                nodes.push_back(TempNode);
                SortedIds.push_back(TempId);
            }

            else if(TempEnt.DNameData == "way"){
                bool oneway = false;
                int speed_limit = 25;
                std::vector<TnodeIndex> wayorder;
                while(!reader.End()){
			printf("Not logical value at line number %d \n",__LINE__);
                    reader.ReadEntity(TempEnt);
                    if(TempEnt.DType == SXMLEntity::EType::EndElement){
                        if(TempEnt.DNameData == "way"){
                            break;
                        }
                    }
                    if(TempEnt.DType == SXMLEntity::EType::StartElement){
                        if(TempEnt.DNameData == "nd"){
                            auto lookup = position.find(std::stoul(TempEnt.AttributeValue("ref")));
                            if(lookup != position.end()){
                                wayorder.push_back(lookup->second);

                            }
                        }
                        if(TempEnt.DNameData == "tag"){
                            if(TempEnt.AttributeValue("k") == "oneway"){
                                if(TempEnt.AttributeValue("v") == "yes"){
                                    oneway = true;
                                }
                            }
                        }
                        if(TempEnt.AttributeValue("k") == "maxspeed"){
                            std::string speed = TempEnt.AttributeValue("v");
                            int npos = speed.find_first_of(" ");
                            std::string speed1 = speed.substr(0,npos);
                            speed_limit = std::stoul(speed1);

                        }
                    }
                }

                for(int i = 0; i < wayorder.size() - 1 ;i++){
                    edge tempedge;
                    tempedge.ConnectedNode = wayorder[i + 1];
                    tempedge.speed = speed_limit;
                    double distance = HaversineDistance(nodes[wayorder[i]].location.first,
                                                          nodes[wayorder[i]].location.second,
                                                          nodes[wayorder[i + 1]].location.first,
                                                          nodes[wayorder[i + 1]].location.second);
                    tempedge.distance = distance;
		    tempedge.time = distance / 3;
                    nodes[wayorder[i]].edges.push_back(tempedge);
                    if(!oneway) {
                        edge edge2;
                        edge2.ConnectedNode = wayorder[i];
                        edge2.speed = speed_limit;
                        edge2.distance = distance;
			edge2.time = distance / 3;
                        nodes[wayorder[i + 1]].edges.push_back(edge2);

                    }


                }

                }

            }
        }
        std::sort(SortedIds.begin(),SortedIds.end());
       printf ("This line is %d.\n", __LINE__);
	 CCSVReader readstops(stops);
	 printf ("This line is %d.\n", __LINE__);
    std::vector<std::string>header;
    printf ("This line is %d.\n", __LINE__);
    readstops.ReadRow(header);
    size_t StopIndex, NodeIndex;
    for(size_t index = 0; index<header.size();index++){
        if(header[index] == "stop_id"){
            StopIndex = index;
        }
        else if(header[index] == "node_id"){
            NodeIndex = index;
        }
    }
    int i = 0;
    while(!readstops.End()){
        std::vector<std::string>stops;
        readstops.ReadRow(stops);
	auto lookup = position.find(std::stoul((stops[1])));
        TnodeIndex PlaceInVector = lookup->second;
	unsigned long stopid = std::stoul(stops[0]);
        StopIDToNodeIndex[stopid] = PlaceInVector;
        NodeIdToStopID[PlaceInVector] = stopid;
        i++;
    }
    std::vector<std::string>sto;
    readstops.ReadRow(sto);
    std::cout<<i<<std::endl;
for (std::map<int,unsigned long>::iterator it=NodeIdToStopID.begin(); it!=NodeIdToStopID.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

    std::cout<<NodeIdToStopID.size()<<std::endl;

    CCSVReader readroutes(routes);
    std::vector<std::string>Header;
    readroutes.ReadRow(Header);
    std::string prev = Header[0];
    //header row
    while(!readroutes.End()){
        std::vector<std::string>route;
        readroutes.ReadRow(route);
	if(route[0] == prev){
            TnodeIndex stop = StopIDToNodeIndex.find(std::stoul(route[1]))->second;
            Routes.back().stops.push_back(stop);
        }
        else{
            BusRoutes ARoute;
            ARoute.name = route[0];
            TnodeIndex Stopidindex = StopIDToNodeIndex.find(std::stoul(route[1]))->second;
            ARoute.stops.push_back(Stopidindex);
            Routes.push_back(ARoute);
        }

        prev = route[0];

    }
    std::cout<<Routes[0].stops.size()<<std::endl;
    for(auto c:Routes[0].stops){
        std::cout<<c<<std::endl;
    }
    std::cout<<"please"<<std::endl;

     for(int i = 0; i < Routes.size();i++){
        std::string Routename = Routes[i].name;
        for(int j = 0;j < Routes[i].stops.size() - 1;j++){
	    auto busIDS = std::make_pair(Routes[i].stops[j], Routes[i].stops[j+1]);
            if (NodeIDsToBusEdge.find(busIDS) == NodeIDsToBusEdge.end()) {
            	BusEdge TempBusedge;
            	std::vector<TNodeID>path;
		std::vector<TNodeID>path1;
            	TempBusedge.Routes.push_back(Routename);
            	TempBusedge.time = dkystra(Routes[i].stops[j], Routes[i].stops[j+1],path,2);
            	TempBusedge.distance = dkystra(Routes[i].stops[j], Routes[i].stops[j+1],path1,0);
		for(int i = 0;i<path1.size();i++){
                    std::cout<<path1[i]<<std::endl;
                }
            	for(auto &c:path1){
                	TnodeIndex nodeindex = position.find(c)->second;
                	TempBusedge.path.push_back(nodeindex);
            	}	
            	std::cout<<TempBusedge.time<<std::endl;
            	std::cout<<TempBusedge.distance<<std::endl;
                for(int i = 0;i<TempBusedge.path.size();i++){
                    std::cout<<TempBusedge.path[i]<<std::endl;
                }		

            	NodeIDsToBusEdge[busIDS] = TempBusedge;
		std::cout<<NodeIDsToBusEdge[busIDS].Routes[0]<<std::endl;

           }
	    else{
                NodeIDsToBusEdge[busIDS].Routes.push_back(Routename);
            }
	}
    }
     
return true;
}

size_t CMapRouter::NodeCount() const{
    // Your code HERE
    return nodes.size();
}

CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const{
    // Your code HERE
       return SortedIds[index];

}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const{
    // Your code HERE
    return nodes[index].location;
}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const{
    // Your code HERE 
    auto lookup = position.find(nodeid);
    int index = lookup->second;
    TLocation locate;
    locate.first = nodes[index].location.first;
    locate.second = nodes[index].location.second;
    return locate;
}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const{
    // Your code HERE
    TnodeIndex lookup = StopIDToNodeIndex.find(stopid)->second;
    return nodes[lookup].nodeid;
}

size_t CMapRouter::RouteCount() const{
    // Your code HERE
        return Routes.size();

}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const{
    // Your code HERE
}

bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops){
    // Your code HERE
    for(auto c:Routes){
        if(c.name == route){
            for(auto a:c.stops){
                TStopID StopID = NodeIdToStopID.find(a)->second;
                stops.push_back(StopID);

            }
        }
    }
    if(stops.empty()){
        return false;
    }
    return true;
}

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
	auto lookup = position.find(src);
    auto NewSrc = lookup->second;
    auto lookup2 = position.find(dest);
    auto NewDest = lookup2->second;
    std::vector< TnodeIndex > path1;
    return dkystra(NewSrc,NewDest,path,0);
    // Your code HERE
}

double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path){
    // Your code HERE
}

bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
    // Your code HERE
}
