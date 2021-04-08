#ifndef ROAD_H
#define ROAD_H

#include "conf.h"
#include "structures.h"
#include "time.h"
#include <algorithm>
#include <iomanip>
#include <pthread.h>
#include <signal.h> 
#include <list>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <python2.7/Python.h>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/multi_array.hpp> 
#include <boost/array.hpp>
#include <queue>
#include "tools.h"
#include "TimeDependent2Hop.h"
#include "heap.h"

class RoadNetwork
{
public:
	Graph g;
	Configuration* conf;
	
public:
//Implemented in buildGraph.cpp
	RoadNetwork(char* argv[]);

	int		buildBackgroundGraph();	//Build the graph from the original node and road
	int 	buildMapGraph();//build from map data
	int		createNewGraphFile();
	bool	hasNewGraph();
	int		readOrgNodeMap();
	int		readOrgRoad();
	int		readOrgNode();
	int		writeNewGraphFile();
	int		writeNewNode();
	int		writeNewRoad();
	int		readRoad();
	int		readNode();
	int 	readMap();

	void	analysis();//fix the neighbor problem

	void	createSpecialNode();
	int		readOrgSpecialNode();
	int		writeSpecialNode();
	int		writeSpecialRoad();
	int		readSpecialNode();
	int		readSpecialRoad();
	int		readSpecialRoadMap();

	int		loadIndex();
	int		buildQuadTree();
	int		readQuadTree();
	double	minX, minY, maxX, maxY;
	void	updateMMXY(double x, double y);	//update the min/max XY

//Implemented in CH.cpp	
	vector<int>		vNodeOrder;	//i:Order item:nodeID
	vector<int>		vRNodeOrder;//i:nodeID item:Order
	void	readNodeOrder();
	bool	NodeOrderUpdate(int nodeID, int index, map<int, int>& mPosition, vector<int>& vnContractedNeighborsOld);
	bool	hasCH();
	int		loadSCH();
	int		readSCH();

//Implemented in SCH.cpp
	int		createSCH();
	int		writeSCH();
	int		buildCHDijkstra(int ID1, int ID2, int dThreshold, vector<bool>& vbVisited, int dUV, map<int, vector<pair<int, int> > > mvpX, vector<vector<pair<int, int> > > &vvOutNode, vector<vector<pair<int, int> > > &vvInNode, vector<pair<int, int> >& vW, int& vEdge, map<int, vector<pair<int, int> > > &mIn, map<int, vector<pair<int, int> > >& mOut);

//Implemented in TCH.cpp
	Semaphore *smTCH = new Semaphore(40);            
	vector<vector<pair<int, int> > > vvpShortCut;//out-shortcut
	vector<vector<pair<int, int> > > vvpRShortCut;//in-shortcut
	//overlay graph, neighbor order are higher than current node
	//Also contains shortcuts
	vector<unordered_map<int, LPFunction> > vmOutNeighbor;
	vector<unordered_map<int, LPFunction> > vmInNeighbor;
	vector<vector<int> > vvOutNeighbor;
	vector<vector<int> > vvInNeighbor;
	
	vector<unordered_map<int, vector<bool> > > vmOutNeighborARC;//TSHARC
	vector<unordered_map<int, vector<bool> > > vmInNeighborARC;
	void	TSHARCSpace();
/*	void	loadTSHARC();
	bool	hasTSHARC();
	void	readTSHARC();
	void	writeTSHARC();
	void	createTSHARC();
	void	TSHARCBoundaryForwardSearch(int sourceID, int gridID, int order);
	void	TSHARCBoundaryBackwardSearch(int sourceID, int gridID, int order);
*/	
	//G' structure for faster creation
	//only store neighbors higher than the key vertex
	//<id, length>
	//Init to the original graph info
	vector<vector<pair<int, int> > > vvOutNode, vvInNode;
	vector<bool>	vbContracted;
	vector<int>		vnContractedNeighbors;
	int		createTCH(); 
	int		createTCH2();
	void	writeTCH();
	void	buildTCHLPF(int ID1, unordered_map<int, LPFunction>& mID2, int v, map<pair<int, int>, LPFunction>& mpLPFR, map<pair<int, int>, bool>& mpBool, unordered_map<int, bool>& mContracted);
//	void	fastestPathTCH(int ID1, int ID2, LPFunction& lpfR);
	void	fastestPathTCH(int ID1, int ID2, LPFunction& lpfR, vector<double>& vRunTime, int index);
	int		loadTCH();
	int		readTCH();
	void	buildTCHLPFMinSearch(int ID1, unordered_map<int, LPFunction>& mID2, int v, unordered_map<int, bool>& mContracted, unordered_map<int, unordered_map<int, bool> > mmSearchedEdge);

	//smaller TCH on Grid
	int		loadGTCH(int gridID); 
	int		readGTCH(int gridID);
	int		createGTCH(int gridID);
	void	writeGTCH();
	void	buildGTCHLPF(int ID1, unordered_map<int, LPFunction>& mID2, int v, map<pair<int, int>, LPFunction>& mpLPFR, map<pair<int, int>, bool>& mpBool, unordered_map<int, bool>& mContracted);  
	vector<unordered_map<int, LPFunction> > vmOutNeighborG; 
	vector<unordered_map<int, LPFunction> > vmInNeighborG;
	vector<vector<int> > vvOutNeighborG;
	vector<vector<int> > vvInNeighborG;
	unordered_map<int, int> mNodeOrder;
	unordered_map<int, int> mRNodeOrder; 
	vector<int> vNodeOrderGrid;
	void	buildTCHLPFInit(pair<int, int> pID1, LPFunction& lpfID1, vector<int>& vID2, vector<LPFunction>& vLPFF2, boost::multi_array<LPFunction, 2>& maLPFOld, boost::multi_array<bool, 2>& maBool, vector<pair<int, int> >& vpMM);
	void	buildTCHLPFBound(pair<pair<int, int>, pair<int, pair<int, int> > > pID, vector<int>& vID2, boost::multi_array<LPFunction, 2>& maLPFOld, boost::multi_array<LPFunction, 2>& maLPFR, boost::multi_array<int, 2>& maIndicator, unordered_map<int, bool>& mContracted, vector<vector<int> >& vvPos2);
	void	buildTCHLPFMinSearch(int ID1, vector<int>& vID2, int v, unordered_map<int, bool>& mContracted, unordered_map<int, vector<int> >& mvNodes, unordered_map<int, int>& mMin, int oldMin);
	void	buildTCHLPFMaxSearch(int ID1, vector<int>& vID2, int v, unordered_map<int, bool>& mContracted, unordered_map<int, vector<int> >& mvNodes, unordered_map<int, int>& mMax, int oldMax);        

	int		highestCHCheapest(int ID1, int ID2, int ID3, vector<int>& vX, vector<int>& vY);
	vector<vector<pair<int, vector<int> > > > vvpShortCutX;
	vector<vector<pair<int, vector<int> > > > vvpShortCutY;
	vector<vector<pair<int, vector<int> > > > vvpRShortCutX;
	vector<vector<pair<int, vector<int> > > > vvpRShortCutY;
	
	int		loadDTCH();
	int		readDTCH();
	int		createDTCH();
	int		writeDTCH();
	int		DTCHFastestPath(int u, vector<pair<int, int> >& vpCostT, vector<pair<int, int> >& vpCostC, int v);
	int 	Chaining(vector<pair<int, int> > vX1, vector<pair<int, int> > vY1, vector<pair<int, int> > vX2, vector<pair<int, int> > vY2, vector<pair<int, int> >& vX, vector<pair<int, int> >& vY);
	int 	addDTCHShortCut(int u, int v, vector<pair<int, vector<pair<int, int> > > >& vpvpGX, vector<pair<int, vector<pair<int, int> > > >& vpvpGY, vector<vector<int> >& vvX, vector<vector<int> >& vvY);
	bool	DTCHMinCostUpdate(int nodeID, int neighborNodeID, int neighborRoadID, int& minCost, int currentTime, int& neighborCurrentTime, vector<int>& vEA, vector<int>& vLD, vector<vector<int> >& vvX, vector<vector<int> >& vvY);
	vector<vector<pair<int, vector<pair<int, int> > > > > vDTCHX;//nodeID, <neighborID, <(x1s,x1d)(x2s,x2d)>>
	vector<vector<pair<int, vector<pair<int, int> > > > > vDTCHY;//nodeID, <neighborID, <(y1s,y1d)(y2s,y2d)>>
	vector<vector<pair<int, vector<pair<int, int> > > > > vRDTCHX;
	vector<vector<pair<int, vector<pair<int, int> > > > > vRDTCHY;

//Implemented in CHQuery.cpp
	int		shortestPathDijkstraCH(int ID1, int ID2, vector<int>& vRoadList);
	int		shortestPathDijkstraCHPure(int ID1, int ID2, vector<int>& vRoadList);
	void	cheapeatPathCH(int ID1, int ID2, int ts, int td, int& t, int &EA, int& LD);
	void	markEdge(int ID2, vector<vector<pair<int, bool> > >& vvpSCMarked, vector<bool>& vbMarkedEdge);
	int		shortestPathDijkstraCHVP(int ID1, int ID2, vector<int>& vRoadList); 
	void	fastestPathGTCH(int ID1, int ID2, LPFunction& lpfR); 

//Implemented in temporal.cpp
	void	readSpeedProfile();
	int		readOriginalSpeedProfile();
	void	fillVoidCost();
	void	writeNewSpeedProfile();
	int		readNewSpeedProfile();

//Map of the original IDs and new IDs in vector
	map<double, double> mNodeMap;	//Same Node with different IDs
	map<double, bool> mNodeMapFull; //Store all the same node IDs
	map<double, int> mRoadIDTrans;	//Original,Order
	map<double, int> mRRoadIDTrans;	//Order,Original
	map<double, int> mRoadIDSpecial;//Original,Special Node
	map<double, int> mNodeIDTrans;	//Original,Order
	map<double, int> mRNodeIDTrans;	//Order,Original

//Implemented in test.cpp	
	void	test();
	int		readTestFile();
	bool	bTest;
	vector<vector<int> > vvTestInput;
	vector<vector<string> > vvTestFileInput;
	void	testShortestPathDijkstra();
	void	testCheapestPathOriginal();
	void	testIntervalFastestPathEDBT();
	void	testCheapestAlgorithms();
	void	testCheapestPathLPFBasic();
	void	testCheapestPathLPFInc();
	void	testCheapestAlgorithmsLPF();  
	static void* testCheapestAlgorithmsThread(void *s);
	void	testT2HopGrid();
	string	parkingOption;
	void	testSCH();
	void	testGTCH();
	void	testT2HopBoundary();
	void	testT2HopBoundaryTotal();
	void	testT2HopBoundaryQuery();//query
	void	testT2HopGridBoundaryCompress();
	void	testT2HopGridInner();	
	void	testT2HopGridRoadNetwork();
	void	testTCH(); 
	void	testT2HopDij();
	void	testT2HopGridDirectQuery();
	void	testT2HopGridInnerSSFP();
	void	testT2HopGridBoundaryQueryCompressSSFP1Inner1();
	void	testT2HopGridInnerISFP();
	void	testT2HopGridInterISFP();

//Implemented in road.cpp
	void	shortestPathDijkstraStatic(int ID1, int ID2, int startTime);
	void 	shortestPathDijkstraTimeDependent(int ID1, int ID2, int startTime);
	void    shortestPathDijkstraHeap(int ID1, int ID2);
	void	shortestPathDijkstra(int ID1, int ID2, vector<int>& vRoadList, int& distance);
	void	shortestPathDijkstraHeap(int ID1, int ID2, benchmark::heap<2,int, int>& queue, vector<int>& vRoadList, int& distance);

//Implemented in pitu.cpp
	void	pituShortestWalk(int ID1, int ID2, int dThreshold, vector<int>& vRoadList, vector<int>& vSpecialNodeList, int& distance);

//Implemented in cheapestPath.cpp
	void	cheapestPathOriginal(int ID1, int ID2, int ts, int td, int& t, int& EA, int& LD);
	void	activeTimeInterval(int ID1, int ID2, int ts, int td, vector<int> &vEA, vector<int> &vLD, vector<vector<int> >& vvX, vector<vector<int> >& vvY);
	void	earliestArrivalTime(int ID1, int ts, vector<int> &vEA);
	void	latestDepartureTime(int ID2, int td, vector<int> &vLD);
	bool	minCostUpdate(int nodeID, int neighborNodeID, int neighborRoadID, int& minCost, int currentTime, int& newCurrentTime, vector<int>& vEA, vector<int>& vLD, vector<vector<int> >& vvX, vector<vector<int> >& vvY);
	//Algorithms using LPF Class
	void	cheapestPathLPFBasic(int ID1, int ID2, int ts, int td, int& t, int& EA, int& LDa, int& nTurnPoint, string parkingOption);
	void	cheapestPathLPFBasicPrune(int ID1, int ID2, int ts, int td, int& t, int& EA, int& LD, int nTurnPoint, string parkingOption, double alpha, bool bRandom);
	void	cheapestPathLPFInc(int ID1, int ID2, int ts, int td, int& t, int& EA, int& LD);
	void	EALPF(int ID1, int ID2, int ts, vector<int> &vEA, int& nodeNum);
	void	LDLPF(int ID1, int ID2, int td, vector<int> &vLD, int& nodeNum);
	void	ATILPF(int ID1, int ID2, int ts, int td, vector<int> &vEA, vector<int> &vLD, int& nodeNum);
	void	ATILPFPrune(int ID1, int ID2, int ts, int td, vector<int> &vEA, vector<int> &vLD, int& nodeNum, int& tpUBound);	

//Implemented in fastestPath.cpp
	int		SSFP(int ID1, int ID2, int t);
	void	intervalFastestPathEDBT(int ID1, int ID2, int ts1, int ts2, int& t);
	int		findMaxTao(int c, vector<int>& vX, vector<int>& vY);

//Implemented in roadT2Hop.cpp
	T2Hop   t2hop;
	string  T2HopFileName;
	string  T2HopDir;
	void	setT2Hop();
	void	loadT2Hop();
	bool	hasT2Hop();
	void	readT2Hop();
	void	createT2Hop();
};

#endif
