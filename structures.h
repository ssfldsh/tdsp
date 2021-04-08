#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include <map>
#include <set> 
#include "linearPiecewiseFunction.h"
#include "Semaphore.h"
//#include "punch.h"
//#include "grid.h"

typedef struct ROAD
{
	long int		roadID;
	long int		ID1, ID2;		
	int		direction;	//A6
	float	length;		//A13
	bool	isolated;
	set<long int>	sNeighborRoad;	//connected road,include main 
	vector<pair<double, double> > vpRoadDetail;	//Road line detail
	
	double	orgRoadID;		//A2
	double	orgID1, orgID2;	//org End IDs
//	vector<pair<int, double> > vCost;

	LPFunction costFunction;

	//Old structure, discarded
	vector<int>	vCostT;
	vector<int>	vCostC;
}road;
typedef struct NEIGHBOR
{
	long int neighborNodeID;
	long int neighborRoadID;
}neighbor;
typedef struct NODE
{
	long int	nodeID;
	int		type;	//A5,0123
	bool	bSpecial;
	double	x;
	double	y;
	bool	isolated;
	long int 	newNeighborNode;
	vector<neighbor> vNeighbor;			//out neibor node id and out neibor road id
	
	set<int>	sNeighborNode;		//in-out neighbor node
	set<int>	sNeighborRoad;		//in-out neighbor road
	vector<int>		vNeighborNode;	//Out-neighbor
	vector<int>		vNeighborRoad;	//Corresponding Out-road
	vector<int>		vNeighborLength;//Corresponding length

	map<int, float>	mNeighborLength;	//Node,Length, out
//	vector<pair<int, float> > vNeighborLength;
	map<int, int>	mSubNeighborRoad;	//A13,nodeID,roadID
	map<int, int>	mRSubNeighborRoad;	//reverse neighbor roads, N,R
	vector<int>		vRNeighborNode;
	vector<int>		vRNeighborRoad;
	vector<int>		vRNeighborLength;
	
	double	orgNodeID;			//A2
	double	orgMainID;			//A8,type=1,2,3
	vector<double>	vOrgSubID;	//A9,A10,type=3
	double	orgNeighborNode;	//special case, A12
}node;

typedef struct GRAPH
{
	//using hash map is more okay but should change the lpf function(which is dependent on vector road)
	//vector<node>	vNode;
	//change
	map<long int, node> vNode;
	//vector<road>	vRoad;
	//add
	map<long int, road> vRoad;
	//vRoad.resize(100000000);
}Graph;


#endif
