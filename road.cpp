#include "road.h"
#include "tools.h"

typedef struct COMPARENODE
{
	pair<long int, double> pif;//ID, distance
	bool operator() (const struct COMPARENODE& a, const struct COMPARENODE& b) const  
	{  
		return a.pif.second > b.pif.second; 
	} 
}compareNode;



//define a function to compute the shortest path with fibonacci_heap, another heap  is completed below.input start pointid
//end pointid roadList, notime ?? no speed?? using Graph g which is defined in conf.h
void RoadNetwork::shortestPathDijkstraStatic(long int ID1, long int ID2)
{
	boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> > fHeap;
	map<long int, double> vDistance;
	map<long int, long int> vPrevious; 
	map<long int, bool> vbVisited;
	map<long int, node>::iterator NodeIterator;
	for(NodeIterator=g.vNode.begin(); NodeIterator!=g.vNode.end(); NodeIterator++){
		vDistance[(*NodeIterator).first]=INF;
	}
	for(NodeIterator=g.vNode.begin(); NodeIterator!=g.vNode.end(); NodeIterator++){
		vPrevious[(*NodeIterator).first]=-1;
	}
	for(NodeIterator=g.vNode.begin(); NodeIterator!=g.vNode.end(); NodeIterator++){
		vbVisited[(*NodeIterator).first]=false;
	}
	//vector<boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> >::handle_type> vHandler(g.vNode.size());
	//vector<int>::iterator ivD, ivP, ivNL;
	//int i;
	long int topNodeID, neighborNodeID, neighborRoadID;

	double neighborLength=0;//初始化需要吗？？

	vDistance[ID1] = 0;
	compareNode cn;
	cn.pif = make_pair(ID1, 0);//初始堆里的元素，推理放的是comparenode对象cn,cn.pif是id和当前distance
	fHeap.push(cn);

	compareNode cnTop;
	while(!fHeap.empty())//every time a topnode is fixed if topid==id2,break.
	{
		cnTop = fHeap.top();
		fHeap.pop();
		//取出堆头元素top头部的意思，最短距离确定的点也是当前要使用的点。

		//add
		if(vbVisited[cnTop.pif.first]) continue;//can also delete fq.remove()

		topNodeID = cnTop.pif.first;
		vbVisited[topNodeID] = true;
		if(topNodeID == ID2) break;
		for(int i = 0; i < (int)g.vNode[topNodeID].vNeighbor.size(); i++)
		{
			neighborNodeID = g.vNode[topNodeID].vNeighbor[i].neighborNodeID;
			//roadLength  self add
			neighborRoadID = g.vNode[topNodeID].vNeighbor[i].neighborRoadID;
			//if has speedprofile vX.size!=0
			neighborLength = g.vRoad[neighborRoadID].length;

			double d = vDistance[topNodeID] + neighborLength;
			if(!vbVisited[neighborNodeID])
			{
				if(vDistance[neighborNodeID] > d)
				{
					vDistance[neighborNodeID] = d;
					compareNode cn;
					//改变的变量
					cn.pif = make_pair(neighborNodeID, d);//优先队列里的元素,id距离对
					fHeap.push(cn);//
					vPrevious[neighborNodeID] = topNodeID;//vPrevious是Path最短路的前继节点。
				}
			}
		}
	}

	cout << "Shortest distance from " << ID1 << " to "  << ID2 << " is " << vDistance[ID2] << endl;
	//path

}
//startTime: start shiKe curTime:daoDaDangQianDianDeShiKe
void RoadNetwork::shortestPathDijkstraTimeDependent(int ID1, int ID2, int startTime)
{
	boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> > fHeap;
	vector<int> vDistance(g.vNode.size(), INF);//distance from sourcenode to others
	vector<int> vPrevious(g.vNode.size(), -1); 
	vector<bool> vbVisited(g.vNode.size(), false);
	//vector<boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> >::handle_type> vHandler(g.vNode.size());
	vector<int>::iterator ivD, ivP, ivNL;
	int i;
	int topNodeID, neighborNodeID, neighborRoadID;

	int neighborLength=0;
	int curTime=startTime;

	vDistance[ID1] = 0;
	compareNode cn;
	cn.pif = make_pair(ID1, 0);//初始堆里的元素，推理放的是comparenode对象cn,cn.pif是id和当前distance
	fHeap.push(cn);

	compareNode cnTop;
	while(!fHeap.empty())//every time a topnode is fixed if topid==id2,break.
	{
		cnTop = fHeap.top();
		fHeap.pop();
		//取出堆头元素top头部的意思，最短距离确定的点也是当前要使用的点。

		//add
		if(vbVisited[cnTop.pif.first]) continue;

		topNodeID = cnTop.pif.first;
		vbVisited[topNodeID] = true;
		if(topNodeID == ID2)	break;
		curTime=startTime+vDistance[topNodeID];
		for(i = 0; i < (int)g.vNode[topNodeID].vNeighbor.size(); i++)
		{
			neighborNodeID = g.vNode[topNodeID].vNeighbor[i].neighborNodeID;
			neighborRoadID = g.vNode[topNodeID].vNeighbor[i].neighborRoadID;
			neighborLength = g.vRoad[neighborRoadID].costFunction.getY(curTime);

			int d = vDistance[topNodeID] + neighborLength;
			if(!vbVisited[neighborNodeID])
			{
				if(vDistance[neighborNodeID] > d)
				{
					vDistance[neighborNodeID] = d;
					compareNode cn;
					//改变的变量
					cn.pif = make_pair(neighborNodeID, d);//优先队列里的元素,id距离对
					fHeap.push(cn);//
					vPrevious[neighborNodeID] = topNodeID;//vPrevious是Path最短路的前继节点。
				}
			}
		}
	}
	
	cout << "Shortest distance from " << ID1 << " to "  << ID2 << " is " << vDistance[ID2] << endl;
}

/*
void RoadNetwork::shortestPathDijkstra(int ID1, int ID2, vector<int>& vRoadList, int& distance)
{
	boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> > fHeap;
	vector<int> vDistance(g.vNode.size(), INF);
	vector<int> vPrevious(g.vNode.size(), -1); 
	vector<bool> vbVisited(g.vNode.size(), false);
	vector<boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> >::handle_type> vHandler(g.vNode.size());
	vector<int>::iterator ivD, ivP, ivNL;
	int i;
	int topNodeID, neighborNodeID, neighborLength;
	
	vDistance[ID1] = 0;
	compareNode cn;
	cn.pif = make_pair(ID1, 0);
	vHandler[ID1] = fHeap.push(cn);
	int nV = 0;

	compareNode cnTop;
	while(!fHeap.empty())
	{
		cnTop = fHeap.top();
		fHeap.pop();
		topNodeID = cnTop.pif.first;
		vbVisited[topNodeID] = true;
		if(topNodeID == ID2)
			break;
		for(i = 0; i < (int)g.vNode[topNodeID].vNeighborLength.size(); i++)
		{
			nV++;
			neighborNodeID = g.vNode[topNodeID].vNeighborNode[i];
			neighborLength = g.vNode[topNodeID].vNeighborLength[i];


			//ask for not visited???? no use visited is ok


			//if this node is not ever watched Initial
			if(vDistance[neighborNodeID] == INF && neighborNodeID != ID1)
			{
				int d = vDistance[topNodeID] + neighborLength;
				vDistance[neighborNodeID] = d;
				compareNode cn;
				cn.pif = make_pair(neighborNodeID, d);
				vHandler[neighborNodeID] = fHeap.push(cn);
				vPrevious[neighborNodeID] = topNodeID;
			}
			//if has been watched and modified Renew
			else if(vDistance[neighborNodeID] > vDistance[topNodeID] + neighborLength)
			{
				vDistance[neighborNodeID] = vDistance[topNodeID] + neighborLength;
				(*vHandler[neighborNodeID]).pif.second = vDistance[neighborNodeID];
				fHeap.decrease(vHandler[neighborNodeID]);
				vPrevious[neighborNodeID] = topNodeID;
			}
		}
	}
	
	cout << "Shortest distance from " << ID1 << " to "  << ID2 << " is " << vDistance[ID2] << endl;
	distance = vDistance[ID2];
	cout << g.vNode[ID1].x << "\t" << g.vNode[ID1].y << "\t" << g.vNode[ID2].x << "\t" << g.vNode[ID2].y << endl; 

/*	cout << "Dijkstra Nodes:" << endl; 
	int n = ID2;
	int oldOrder = -1;
	list<int> lNext;
	list<int>::iterator ilNext;
	while(n != -1)
	{
		lNext.push_front(n);
		n = vPrevious[n];
	}
	for(ilNext = lNext.begin(); ilNext != lNext.end(); ilNext++)
	{
		cout << *ilNext << "\t" << vDistance[*ilNext] << "\t" << vRNodeOrder[*ilNext];
		if(vRNodeOrder[*ilNext] > oldOrder)
			cout << "\tlarger" << endl;  
		else
			cout << endl;
		oldOrder = vRNodeOrder[*ilNext];
	}
**
//	cout << "visit " << nV << " nodes" << endl;
}
*/