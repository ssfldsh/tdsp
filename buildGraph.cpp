#include "road.h"

RoadNetwork::RoadNetwork(char* argv[])
{
	conf = new Configuration();
	buildMapGraph();
}

int RoadNetwork::buildMapGraph()
{
	cout << "Building Background Graph" << endl;
	readMap();
	if(conf->bTemporal)//exe
		readSpeedProfile();
	return 0;
}


int RoadNetwork::readMap()
{
	//readRoad
	ifstream inMapFile(conf->mapFile.c_str());
	if(!inMapFile)
	{
		cout << "Cannot open new map file" << conf->mapFile << endl;
		return -1;
	}
	cout << "Reading new Map File: " << conf->mapFile << endl;//exe

	long int roadNum = 0;
	inMapFile >> roadNum;
	cout << "Road Number:" << roadNum << endl;
	int i;
	for(i = 0; i < roadNum; i++)
	{
		road r;
		// node startNode;
		// node endNode;
		int speedLimit;//no use
		double x1,y1,x2,y2;
		inMapFile >> r.roadID >> r.ID1>> r.ID2  >> r.direction >> r.length >>speedLimit >> x1 >> y1 >> x2 >> y2;
		r.isolated = false;
		g.vRoad[r.roadID]=r;
		//road has been added
		//now modify node
		neighbor nei;
		nei.neighborNodeID = r.ID2;
		nei.neighborRoadID = r.roadID;
		g.vNode[r.ID1].nodeID=r.ID1;
		g.vNode[r.ID2].nodeID=r.ID2;
		g.vNode[r.ID1].x = x1;
		g.vNode[r.ID1].y = y1;
		g.vNode[r.ID2].x = x2;
		g.vNode[r.ID2].y = y2;
		g.vNode[r.ID1].vNeighbor.push_back(nei);
		g.vNode[r.ID1].bSpecial=false;
		g.vNode[r.ID1].isolated=false;
		g.vNode[r.ID1].type=0;
		g.vNode[r.ID2].bSpecial=false;
		g.vNode[r.ID2].isolated=false;
		g.vNode[r.ID2].type=0;
	}
	inMapFile.close();
	return 0;
}

void RoadNetwork::updateMMXY(double x, double y)
{
	if(x > maxX)
		maxX = x;
	if(x < minX)
		minX = x;
	if(y > maxY)
		maxY = y;
	if(y < minY)
		minY = y;
}