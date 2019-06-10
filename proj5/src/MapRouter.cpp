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
	if(distance[dest] != std::numeric_limits<double>::max())
	break;
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
	if(TempEnt.DType == SXMLEntity::EType::EndElement){
	    if(TempEnt.DNameData == "osm"){
	         break;
	    }
	}	    
        if(TempEnt.DType == SXMLEntity::EType::StartElement){
            if(TempEnt.DNameData == "node"){
                TNodeID  TempId = std::stoul(TempEnt.AttributeValue("id"));
                double TempLat = std::stod(TempEnt.AttributeValue("lat"));
                double TempLon = std::stod(TempEnt.AttributeValue("lon"));
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
                double speed_limit = 25.00;
                std::vector<TnodeIndex> wayorder;
                while(!reader.End()){
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
                            speed_limit = std::stod(speed1);

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
		    tempedge.time = distance / 3.00;
                    nodes[wayorder[i]].edges.push_back(tempedge);
                    if(!oneway) {
                        edge edge2;
                        edge2.ConnectedNode = wayorder[i];
                        edge2.speed = speed_limit;
                        edge2.distance = distance;
			edge2.time = distance / 3.00;
                        nodes[wayorder[i + 1]].edges.push_back(edge2);

                    }


                }

                }

            }
        }
        std::sort(SortedIds.begin(),SortedIds.end());
	 CCSVReader readstops(stops);
    std::vector<std::string>header;
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
            BusRouteNames.push_back(route[0]);

        }

        prev = route[0];

    }

    std::sort(BusRouteNames.begin(),BusRouteNames.end());

    

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
            	for(auto &c:path1){
                	TnodeIndex nodeindex = position.find(c)->second;
                	TempBusedge.path.push_back(nodeindex);
            	}	

            	NodeIDsToBusEdge[busIDS] = TempBusedge;

           }
	    else{
                NodeIDsToBusEdge[busIDS].Routes.push_back(Routename);
            }
	}
    }
	for (const auto &p : NodeIDsToBusEdge){
        	TnodeIndex source = p.first.first;
        	TnodeIndex dest = p.first.second;
        	edge busedge;
        	busedge.distance = p.second.distance;
        	busedge.time = p.second.time + 30.00/3600.00;
        	busedge.busedge = true;
        	busedge.ConnectedNode = dest;
        	nodes[source].edges.push_back(busedge);
        

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

    TLocation emtpy = std::make_pair(180.00,360.00);

    TNodeID id = SortedIds[index];

    auto find = position.find(id);
    
    if(find == position.end()){
        std::cout<<"why"<<std::endl;
        return emtpy;
    }

    TnodeIndex ind = position.find(id)->second;
    return nodes[ind].location;
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
    if(index >= BusRouteNames.size()){
	    return "";
    }
    return BusRouteNames[index];


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
	        auto lookup = position.find(src);
    auto NewSrc = lookup->second;
    auto lookup2 = position.find(dest);
    auto NewDest = lookup2->second;
     std::vector<TnodeIndex > prev(nodes.size());
    std::vector<double>distance(nodes.size(),std::numeric_limits<double>::max());
    std::vector<TNodeID > heap;
    auto compare = [&distance](TnodeIndex idx, TnodeIndex idx2){return distance[idx] < distance[idx2];};

    prev[NewSrc] = NewSrc;
    distance[NewSrc] = 0.0;
    double altdistance = 0.0;
    heap.push_back(NewSrc);
    while(!heap.empty()){
	if(distance[NewDest] != std::numeric_limits<double>::max())
	    break;	
        std::make_heap(heap.begin(),heap.end(),compare);
        std::pop_heap(heap.begin(),heap.end(),compare);
        auto curr  = heap.back();
        heap.pop_back();
        for(auto &edge: nodes[curr].edges){
            altdistance = distance[curr] + edge.time;
            if(altdistance < distance[edge.ConnectedNode]){
                if(distance[edge.ConnectedNode] == std::numeric_limits<double>::max()){
                    heap.push_back(edge.ConnectedNode);
                }
                distance[edge.ConnectedNode] = altdistance;
                prev[edge.ConnectedNode] = curr;
		
            }
        }

    }
    if(distance[NewDest] == std::numeric_limits<double>::max()){
       return false;
}
   auto temp = NewDest;
   std::vector<TnodeIndex>path1;
    while(temp != NewSrc){
        path1.push_back(temp);
        temp = prev[temp];
    }
   path1.push_back(NewSrc);
   std::reverse(path1.begin(),path1.end());
   int i;
   int count;
   for(i = 0;i < path1.size();i++){
	if(i + 1 != path1.size()){   
        	auto edges = busedge(path1[i],path1[i+1]);
        	if(NodeIDsToBusEdge.find(edges) == NodeIDsToBusEdge.end()){
            		TPathStep temppath;
            		temppath.first = "Walk";
            		temppath.second = NodeIdToIndex.find(path1[i])->second;
            		path.push_back(temppath);

        	}
        	else{
	    		TPathStep temp;
            		temp.first = "Walk";
            		temp.second = NodeIdToIndex.find(NodeIDsToBusEdge[edges].path[0])->second;
            		path.push_back(temp);
            		for(int j = 1;j<NodeIDsToBusEdge[edges].path.size();j++){
                		TPathStep temppath;
				std::string route = NodeIDsToBusEdge[edges].Routes[0];
                		temppath.first = "Bus " + NodeIDsToBusEdge[edges].Routes[0];
                		temppath.second = NodeIdToIndex.find(NodeIDsToBusEdge[edges].path[j])->second;
                		path.push_back(temppath);
				int k = i + 1;
				count = 0;
				for(k;k<path1.size()-1;k++){
                    			auto MoreEdges = busedge(path1[k],path1[k+1]);
                    			auto find = NodeIDsToBusEdge.find(MoreEdges);
                    			if(find != NodeIDsToBusEdge.end() and find->second.Routes[0] ==route){
                        			count++;
                        			for(int k2 = 1;k2<NodeIDsToBusEdge[MoreEdges].path.size();k2++) {
			    			TPathStep MorePath;	
                            			MorePath.first = "Bus " + route;
                            			MorePath.second = NodeIdToIndex.find(NodeIDsToBusEdge[MoreEdges].path[k2])->second;
                            			path.push_back(MorePath);
                        			}

                    			}

                		}
            		}
	    	i += count + 1;
        	}
	    }
	else{
	    TPathStep temppath;
            temppath.first = "Walk";
            temppath.second = NodeIdToIndex.find(path1[i])->second;
            path.push_back(temppath);
	}

         }
    return distance[NewDest];
}
std::string find_direction(double bearing){

    std::string direction;
    if(bearing >= -180 and bearing < -135)
        direction = "S";
    else if(bearing >= -135 and bearing < -90)
        direction = "SW";
    else if(bearing >= -90 and bearing < -45)
        direction = "W";
    else if(bearing >= -45 and bearing < 0)
        direction = "NW";
    else if(bearing >= 0 and bearing < 45)
        direction = "N";
    else if(bearing >= 45 and bearing < 90)
        direction = "NE";
    else if(bearing >= 90 and bearing < 135)
        direction = "E";
    else if(bearing >= 135 and bearing <= 180)
        direction = "SE";

    return direction;
}

std::string helper(double lat, double lon,int type,const std::string dir){
    auto latmin = (lat - floor(lat)) * 60;
    auto latsec = (latmin - floor(latmin)) * 60;
    std::string str;
	int latsec2 = int(latsec);
	str = std::to_string(latsec2);
    int latmin2 = int(floor(latmin));
    auto latdeg = int(abs((lat)));

    double longmin = (lon - floor(lon)) * 60;
    double longsec = (longmin - floor(longmin)) * 60;
    std::string str2;
	int longsec2 = int(longsec);
        str2 = std::to_string(longsec2);
    int longmin2 = int(floor(longmin));
    auto longdeg = int(abs((lon)));

    char latdir = (lat >= 0) ? 'N' : 'S';
    char longdir = (lon >= 0) ? 'E' : 'W';
    std::string ret;
    if(type == 0) {

        ret = "Start at " + std::to_string(latdeg) + "d " + std::to_string(latmin2)
              + "' " + str + "\" " + latdir + ", " + std::to_string(longdeg) +
              "d " + std::to_string(longmin2) + "' " + str2 + "\" " + longdir;
    }
    if(type == 1){
        ret = "Walk " + dir + " to " + std::to_string(latdeg) + "d " + std::to_string(latmin2)
                + "' " + str + "\" " + latdir + ", " + std::to_string(longdeg) +
              "d " + std::to_string(longmin2) + "' " + str2 + "\" " + longdir;
    }
    if(type == 2){
        ret = "End at " + std::to_string(latdeg) + "d " + std::to_string(latmin2)
                + "' " + str + "\" " + latdir + ", " + std::to_string(longdeg) +
              "d " + std::to_string(longmin2) + "' " + str2 + "\" " + longdir;
    }
    return ret;
}


bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
    // Your code HERE
    if(path.empty())
	    return false;
    for(int i = 0;i < path.size() - 1;i++){
        if (i == 0){
            auto loca = position.find(path[0].second)->second;
            TLocation locate = nodes[loca].location;

            double latmin = (locate.first - floor(locate.first)) * 60;
            double latsec = (latmin - floor(latmin)) * 60;
	    int latsec2 = int(latsec);
            int latmin2 = int(floor(latmin));
            auto latdeg = int(abs((locate.first)));

            double longmin = (locate.second - floor(locate.second)) * 60;
            double longsec = (longmin - floor(longmin)) * 60;
	    int longsec2 = int(longsec);
            int longmin2 = int(floor(longmin));
            auto longdeg = int(abs((locate.first)));

            char latdir = (locate.first >= 0) ? 'N' : 'S';
            char longdir = (locate.second >= 0) ? 'E' : 'W';

            std::string start = "Start at " + std::to_string(latdeg) + "d " + std::to_string(latmin2)
                    + "' " + std::to_string(latsec2) + "\" " + latdir + ", " + std::to_string(longdeg) +
                                "d " + std::to_string(longmin2) + "' " + std::to_string(longsec2) + "\" " + longdir;
            desc.push_back(start);
            if(path[1].first == "Walk"){
                auto find = position.find(path[1].second)->second;
                TLocation coord = nodes[find].location;
                double bear = CalculateBearing(locate.first,locate.second,coord.first,coord.second);
                std::string bear1 = find_direction(bear);
                desc.push_back(helper(coord.first,coord.second,1,bear1));
            }

        }
        else if (path[i].first != "Walk"){
            if(path[i + 1].first == "Walk"){

                TnodeIndex node = position.find(path[i].second)->second;
                TStopID stop = NodeIdToStopID.find(node)->second;
                std::string str;
                str = "Take " + path[i].first + " and get off at stop " + std::to_string(stop);
                desc.push_back(str);

                auto firs = position.find(path[i].second)->second;
                auto secon = position.find(path[i + 1].second)->second;
                TLocation first = nodes[firs].location;
                TLocation second = nodes[secon].location;
                double bearing = CalculateBearing(first.first,first.second,second.first,second.second);
                std::string direction = find_direction(bearing);
                desc.push_back(helper(second.first,second.second,1,direction));

            }

        }
        else if(path[i].first == "Walk"){
            if(path[i + 1].first == "Walk"){
                TnodeIndex pos = position.find(path[i].second)->second;
                TLocation first = nodes[pos].location;
                TnodeIndex pos2 = position.find(path[i + 1].second)->second;
                TLocation sec = nodes[pos2].location;

                double bearing = CalculateBearing(first.first,first.second,sec.first,sec.second);
                std::string direct = find_direction(bearing);

                desc.push_back(helper(sec.first,sec.second,1,direct));
            }
        }
    }
    TnodeIndex pos = position.find(path[path.size()-1].second)->second;
    TLocation last = nodes[pos].location;
    std::string directi;
    desc.push_back(helper(last.first,last.second,2,directi));
    for(int j = 0;j<desc.size();j++){
        std::cout<<desc[j]<<std::endl;
    }
    return true;
}
bool CMapRouter::GetPathShortest(const std::vector<CMapRouter::TNodeID> &path, std::vector<std::string> &desc) const {
    if(path.empty()){
        return false;
    }

    for(int i = 0;i<path.size() - 1;i++){
        if (i == 0){
            TnodeIndex index = position.find(path[0])->second;
            TLocation location = nodes[index].location;
            std::string empty;
            desc.push_back(helper(location.first,location.second,0,empty));

            TnodeIndex ind2 = position.find(path[1])->second;
            TLocation loc = nodes[ind2].location;
            double bear = CalculateBearing(location.first,location.second,loc.first,loc.second);
            std::string dir = find_direction(bear);
            desc.push_back(helper(loc.first,loc.second,1,dir));
        }
        else{
            TnodeIndex first = position.find(path[i])->second;
            TLocation loc = nodes[first].location;

            TnodeIndex sec = position.find(path[i + 1])->second;
            TLocation loc2 = nodes[sec].location;

            double bear = CalculateBearing(loc.first,loc.second,loc2.first,loc2.second);
            std::string dir = find_direction(bear);
            desc.push_back(helper(loc2.first,loc2.second,1,dir));
        }
    }

    TnodeIndex last = position.find(path[path.size() - 1])->second;
    TLocation place = nodes[last].location;
    std::string empty;
    desc.push_back(helper(place.first,place.second,2,empty));
    
    return true;
}

