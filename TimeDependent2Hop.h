#ifndef T2HOP_H
#define	T2HOP_H

#include <vector>
#include <unordered_map>
#include <boost/heap/fibonacci_heap.hpp>
#include <fstream>
#include "structures.h"

#define INF 999999999

class T2Hop
{
private:
	//vvpHop[nodeID] to accees each node's hops
	//The hops are stored in vector in pair
	vector<vector<pair<int, LPFunction> > > vvpOutHop;
	vector<vector<pair<int, LPFunction> > > vvpInHop; 

	//Store the position of neighbors in vvpOut/InHops
	vector<unordered_map<int, int> > vmOutHop;
	vector<unordered_map<int, int> > vmInHop;

public:
	T2Hop();

	//Initialized from road network's speed profile
	//Only import the functions on edges
	T2Hop(vector<node>& vNode);

	bool empty();
	void clear();
	void init(vector<node>& vNode);
	void writeT2Hop(string filename);
	void writeT2HopSeparateF(string T2HopDir, string T2HopFilename, int nodeID);
	void writeT2HopSeparateB(string T2HopDir, string T2HopFilename, int nodeID);
	void readT2Hop(string filename);
	void display();

	void forwardSearch(int sourceID, Graph& g);
	void backwardSearch(int sourceID, Graph& g);

	bool fullQuery(int ID1, int ID2, LPFunction& lpf, int& hop);
	bool fullQuery2(int ID1, int ID2, LPFunction &lpf, vector<int>& vHop); 

	//Pruned search
	void pForwardSearch(int sourceID, Graph& g);
	void pBackwardSearch(int sourceID, Graph& g);
};

#endif
