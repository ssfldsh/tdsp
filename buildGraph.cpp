#include "road.h"

RoadNetwork::RoadNetwork(char* argv[])
{
	conf = new Configuration();
	buildMapGraph();
	//readSpeedProfile();
}

int RoadNetwork::buildMapGraph()
{
	cout << "Building Background Graph" << endl;
	readMap();
	if(conf->bBuild)//noexe
	{
//		loadIndex();
	}
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
		node startNode;
		node endNode;
		int speedLimit;//no use
		inMapFile >> r.roadID >> r.ID1>> r.ID2  >> r.direction >> r.length >>speedLimit >> startNode.x >>startNode.y >>endNode.x>>endNode.y;
		startNode.nodeID = r.ID1;
		endNode.nodeID = r.ID2;
		neighbor nei;
		nei.neighborNodeID = r.ID2;
		nei.neighborRoadID = r.roadID;
		//neiborNode and neiborRoad
		startNode.vNeighbor.push_back(nei);
		r.isolated = false;
		startNode.bSpecial=false;
		startNode.isolated=false;
		startNode.type=0;
		endNode.bSpecial=false;
		endNode.isolated=false;
		endNode.type=0;
		g.vRoad[r.roadID]=r;
		g.vNode[r.ID1]=startNode;
		g.vNode[r.ID2]=endNode;
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