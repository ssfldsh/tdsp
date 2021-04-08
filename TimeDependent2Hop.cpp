#include "TimeDependent2Hop.h"

typedef struct COMPARENODE
{
	pair<int, int> pif;//ID, minCost
	bool operator() (const struct COMPARENODE& a, const struct COMPARENODE& b) const  
	{  
		return a.pif.second > b.pif.second; 
	} 
}compareNode;

typedef struct COMPAREHOP
{
	pair<int, int> pif;//m1+m2, hop
	bool operator < (const struct COMPAREHOP &h) const  
	{  
		return pif.second > h.pif.second;  
	} 
}compareHop;

T2Hop::T2Hop()
{
}

T2Hop::T2Hop(vector<node>& vNode)
{
	init(vNode);
}
	
bool T2Hop::empty()
{
	if(vvpOutHop.empty())
		return true;
	return false;
}

void T2Hop::clear()
{
	vvpOutHop.clear();
	vvpInHop.clear();
	vmOutHop.clear();
	vmInHop.clear();
}

void T2Hop::init(vector<node>& vNode)
{
	clear();
	
	for(int i = 0; i < (int)vNode.size(); i++)
	{
		vector<pair<int, LPFunction> > vpOut, vpIn;
		unordered_map<int, int> mOut, mIn; 
		vvpOutHop.push_back(vpOut);
		vvpInHop.push_back(vpIn);
		vmOutHop.push_back(mOut);
		vmInHop.push_back(mIn);
	}
}
	
void T2Hop::writeT2Hop(string filename)
{
	vector<vector<pair<int, LPFunction> > >::iterator ivvpOutHop, ivvpInHop;
	vector<pair<int, LPFunction> >::iterator ivp;
	int i;

	ofstream of(filename);
	of << vvpOutHop.size() << endl;
	for(ivvpOutHop = vvpOutHop.begin(), i = 0; ivvpOutHop != vvpOutHop.end(); ivvpOutHop++, i++)
	{
		of << i << "\t" << (*ivvpOutHop).size() << endl;
		for(ivp = (*ivvpOutHop).begin(); ivp != (*ivvpOutHop).end(); ivp++)
			of << (*ivp).first << "\t" << (*ivp).second.toString();
	}
	
	of << vvpInHop.size() << endl;
	for(ivvpInHop = vvpInHop.begin(), i = 0; ivvpInHop != vvpInHop.end(); ivvpInHop++, i++)
	{
		of << i << "\t" << (*ivvpInHop).size() << endl;
		for(ivp = (*ivvpInHop).begin(); ivp != (*ivvpInHop).end(); ivp++)
			of << (*ivp).first << "\t" << (*ivp).second.toString();
	}

	of.close();
}

void T2Hop::writeT2HopSeparateF(string T2HopDir, string T2HopFilename, int nodeID)
{
	vector<pair<int, LPFunction> >::iterator ivp;
	ofstream of(T2HopDir+"/"+T2HopFilename+"In", std::ofstream::app);
	of << nodeID << "\t" << vvpInHop[nodeID].size() << endl;
	for(ivp = vvpInHop[nodeID].begin(); ivp != vvpInHop[nodeID].end(); ivp++)
		of << (*ivp).first << "\t" << (*ivp).second.toString();
	
	of.close();
}
	
void T2Hop::writeT2HopSeparateB(string T2HopDir, string T2HopFilename, int nodeID)
{
	vector<pair<int, LPFunction> >::iterator ivp;
	ofstream of(T2HopDir+"/"+T2HopFilename+"Out", std::ofstream::app);
	of << nodeID << "\t" << vvpOutHop[nodeID].size() << endl;
	for(ivp = vvpOutHop[nodeID].begin(); ivp != vvpOutHop[nodeID].end(); ivp++)
		of << (*ivp).first << "\t" << (*ivp).second.toString();
	
	of.close();
}

	
void T2Hop::readT2Hop(string filename)
{
	clear();

	ifstream infile(filename);
	int num, maxY;
	int nodeID, ID1, ID2, upperBound, minY, size, lsize, x, y;
	
	infile >> num;
	for(int i = 0; i < num; i++)
	{
		infile >> nodeID >> size;
		vector<pair<int, LPFunction> > vpOutHop;
		unordered_map<int, int> mOutHop;
		for(int j = 0; j < size; j++)
		{
			infile >> ID2 >> ID1 >> ID2 >> upperBound >> minY >> lsize;
			vector<int> vX, vY;
			maxY = -1;
			for(int k = 0; k < lsize; k++)
			{
				infile >> x >> y;
				vX.push_back(x);
				vY.push_back(y);
				if(y > maxY)
					maxY = y;
			}
			LPFunction lpf;
			lpf.import(ID1, ID2, upperBound, minY, maxY, vX, vY);
			vpOutHop.push_back(make_pair(ID2, lpf));
			mOutHop[ID2] = j;
		}
		vvpOutHop.push_back(vpOutHop);
		vmOutHop.push_back(mOutHop);
	}
	
	infile >> num;
	for(int i = 0; i < num; i++)
	{
		infile >> nodeID >> size;
		vector<pair<int, LPFunction> > vpInHop;
		unordered_map<int, int> mInHop;
		for(int j = 0; j < size; j++)
		{
			infile >> ID2 >> ID1 >> ID2 >> upperBound >> minY >> lsize;
			vector<int> vX, vY;
			maxY = -1;
			for(int k = 0; k < lsize; k++)
			{
				infile >> x >> y;
				vX.push_back(x);
				vY.push_back(y);
				if(y > maxY)
					maxY = y;
			}
			LPFunction lpf;
			lpf.import(ID1, ID2, upperBound, minY, maxY, vX, vY);
			vpInHop.push_back(make_pair(ID1, lpf));
			mInHop[ID1] = j;
		}
		vvpInHop.push_back(vpInHop);
		vmInHop.push_back(mInHop);
	}
}

void T2Hop::display()
{
	vector<vector<pair<int, LPFunction> > >::iterator ivvpOutHop, ivvpInHop;
	vector<pair<int, LPFunction> >::iterator ivp;
	int i;

	cout << "Out Label size:" << vvpOutHop.size() << endl;
	for(ivvpOutHop = vvpOutHop.begin(), i = 0; ivvpOutHop != vvpOutHop.end(); ivvpOutHop++, i++)
	{
		cout << "NodeID:" << i << "\t" << (*ivvpOutHop).size() << endl;
		for(ivp = (*ivvpOutHop).begin(); ivp != (*ivvpOutHop).end(); ivp++)
			cout << "HopNode:" << (*ivp).first << "\tFunction:" << (*ivp).second.toString();
	}
	
	cout << endl << "In Label Size:" << vvpInHop.size() << endl;
	for(ivvpInHop = vvpInHop.begin(), i = 0; ivvpInHop != vvpInHop.end(); ivvpInHop++, i++)
	{
		cout << "NodeID:" << i << "\t" << (*ivvpInHop).size() << endl;
		for(ivp = (*ivvpInHop).begin(); ivp != (*ivvpInHop).end(); ivp++)
			cout << "HopNode:" << (*ivp).first << "\tFunction:" << (*ivp).second.toString();
	}

}
	
//Need to create 2hop only with functions
//Init 2hop to empty
void T2Hop::forwardSearch(int sourceID, Graph& g)
{
	cout << "Forward Search " << sourceID << endl;
	boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> > fHeap;
	vector<boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> >::handle_type> vHandler(g.vNode.size());
	vector<int> vTime(g.vNode.size(), INF);  
	vector<bool> vbHeap(g.vNode.size(), false);//Store if in Heap
	vector<bool> bVisited(g.vNode.size(), false);

	vector<int>::iterator ivNN, ivNR;
	int topNodeID, neighborNodeID, neighborRoadID;
	int i, minCost;
	int posST;	//position:source to topNode
	int posSN;	//position:source to neighborNode
	bool bUpdated;
	
	vTime[sourceID] = 0;
	compareNode cn;
	cn.pif = make_pair(sourceID, 0);
	vHandler[sourceID] = fHeap.push(cn);
	vbHeap[sourceID] = true;
	bVisited[sourceID] = true;

	compareNode cnTop;

	while(!fHeap.empty())
	{
		cnTop = fHeap.top();
		fHeap.pop();
		topNodeID = cnTop.pif.first;
		vbHeap[topNodeID] = false;
		bVisited[topNodeID] = true;
		posST = vmInHop[topNodeID][sourceID];
	
//		cout << "Visiting "  << topNodeID << endl;
		for(i = 0; i < (int)g.vNode[topNodeID].vNeighborNode.size(); i++)
		{
			bUpdated = false;
			neighborNodeID = g.vNode[topNodeID].vNeighborNode[i];
			neighborRoadID = g.vNode[topNodeID].vNeighborRoad[i];
//			cout << "Neighbor:" << neighborNodeID << endl;
			if(neighborNodeID == sourceID)
				continue;
			minCost = vTime[neighborNodeID];
			LPFunction lpf;
			if(topNodeID == sourceID)
				lpf = g.vRoad[neighborRoadID].costFunction;
			else
				lpf =vvpInHop[topNodeID][posST].second.LPFCat(g.vRoad[neighborRoadID].costFunction);   
		
			if(vmInHop[neighborNodeID].find(sourceID) == vmInHop[neighborNodeID].end())
			{
				vvpInHop[neighborNodeID].push_back(make_pair(sourceID, lpf)); 
				vmInHop[neighborNodeID][sourceID] = vvpInHop[neighborNodeID].size()-1; 
				posSN = vvpInHop[neighborNodeID].size()-1;
				bUpdated = true;
			}
			else
			{
				posSN = vmInHop[neighborNodeID][sourceID];
				if(!vvpInHop[neighborNodeID][posSN].second.equal(lpf)) 
				{
					lpf = vvpInHop[neighborNodeID][posSN].second.LPFMin(lpf); 
					vvpInHop[neighborNodeID][posSN].second = lpf;
					bUpdated = true;
				}
			}
			
			minCost = vvpInHop[neighborNodeID][posSN].second.getMin(); 
//			if(bUpdated)
//				cout << "Update!" << endl;

			//Updated and not in Heap
			if((!vbHeap[neighborNodeID] && bUpdated) || !bVisited[neighborNodeID] )
			{
				vTime[neighborNodeID] = minCost;
				compareNode cn;
				cn.pif = make_pair(neighborNodeID, minCost);
				vHandler[neighborNodeID] = fHeap.push(cn);
				vbHeap[neighborNodeID] = true;
				bVisited[neighborNodeID] = true;
			}
			//Updated and in Heap, key changed
			else if(bUpdated && vbHeap[neighborNodeID])
			{
				vTime[neighborNodeID] = minCost;
				(*vHandler[neighborNodeID]).pif.second = vTime[neighborNodeID];
				fHeap.decrease(vHandler[neighborNodeID]);
			}
		}
	}
}

void T2Hop::backwardSearch(int sourceID, Graph& g)
{
	cout << "Backward Search " << sourceID << endl;
	boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> > fHeap;
	vector<boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> >::handle_type> vHandler(g.vNode.size());
	vector<int> vTime(g.vNode.size(), INF);  
	vector<bool> vbHeap(g.vNode.size(), false);//Store if in Heap
	vector<bool> bVisited(g.vNode.size(), false);

	vector<int>::iterator ivNN, ivNR;
	int topNodeID, neighborNodeID, neighborRoadID;
	int i, minCost;
	int posTS;	//position:topNode to source
	int posNS;	//position:neighborNode to source
	bool bUpdated;
	
	vTime[sourceID] = 0;
	compareNode cn;
	cn.pif = make_pair(sourceID, 0);
	vHandler[sourceID] = fHeap.push(cn);
	vbHeap[sourceID] = true;
	bVisited[sourceID] = true;

	
	compareNode cnTop;

	while(!fHeap.empty())
	{
		cnTop = fHeap.top();
		fHeap.pop();
		topNodeID = cnTop.pif.first;
		vbHeap[topNodeID] = false;
		bVisited[topNodeID] = true;
		posTS = vmOutHop[topNodeID][sourceID];
//		cout << "Visiting "  << topNodeID << endl;
		
		for(i = 0; i < (int)g.vNode[topNodeID].vRNeighborNode.size(); i++)
		{
			bUpdated = false;
			neighborNodeID = g.vNode[topNodeID].vRNeighborNode[i];
			neighborRoadID = g.vNode[topNodeID].vRNeighborRoad[i];
//			cout << "Neighbor:" << neighborNodeID << endl;
			if(neighborNodeID == sourceID)
				continue;
			minCost = vTime[neighborNodeID];
//			posNT = vmOutHop[neighborNodeID][topNodeID];
			LPFunction lpf;
			if(topNodeID == sourceID)
				lpf = g.vRoad[neighborRoadID].costFunction;
			else
			{
//				cout << "Cat:" << "f1:";
//				g.vRoad[neighborRoadID].costFunction.display();
//				cout << "f2:";
//				vvpOutHop[topNodeID][posTS].second.display();
				lpf =g.vRoad[neighborRoadID].costFunction.LPFCat(vvpOutHop[topNodeID][posTS].second);  
			}
		
			if(vmOutHop[neighborNodeID].find(sourceID) == vmOutHop[neighborNodeID].end())
			{
				vvpOutHop[neighborNodeID].push_back(make_pair(sourceID, lpf)); 
				vmOutHop[neighborNodeID][sourceID] = vvpOutHop[neighborNodeID].size()-1; 
				posNS = vvpOutHop[neighborNodeID].size()-1;
				bUpdated = true;
			}
			else
			{
				posNS = vmOutHop[neighborNodeID][sourceID];
				if(!vvpOutHop[neighborNodeID][posNS].second.equal(lpf)) 
				{
					lpf = vvpOutHop[neighborNodeID][posNS].second.LPFMin(lpf); 
					vvpOutHop[neighborNodeID][posNS].second = lpf;
					bUpdated = true;
				}
			}
			
			minCost = vvpOutHop[neighborNodeID][posNS].second.getMin(); 

			//Updated and not in Heap
			if((!vbHeap[neighborNodeID] && bUpdated) || !bVisited[neighborNodeID] )
			{
				vTime[neighborNodeID] = minCost;
				compareNode cn;
				cn.pif = make_pair(neighborNodeID, minCost);
				vHandler[neighborNodeID] = fHeap.push(cn);
				vbHeap[neighborNodeID] = true;
				bVisited[neighborNodeID] = true;

			}
			//Updated and in Heap, key changed
			else if(bUpdated && vbHeap[neighborNodeID])
			{
				vTime[neighborNodeID] = minCost;
				(*vHandler[neighborNodeID]).pif.second = vTime[neighborNodeID];
				fHeap.decrease(vHandler[neighborNodeID]);
			}
		}
	}
}

//TO FIX:
//The result should be the combination mix of the functions
//Not only one of them!
bool T2Hop::fullQuery(int ID1, int ID2, LPFunction &lpf, int& hop)
{
	int s1 = vvpOutHop[ID1].size();
	int s2 = vvpInHop[ID2].size();  

	vector<pair<int, LPFunction> >::iterator ivp;
	int hopTmp;
	hop = -1;
	bool hit= false;
	if(vmOutHop[ID1].find(ID2) != vmOutHop[ID1].end())
	{
		int pos = vmOutHop[ID1][ID2];
		lpf = vvpOutHop[ID1][pos].second;
		hop = ID2;
		hit = true;
	}
	
	if(vmInHop[ID2].find(ID1) != vmInHop[ID2].end())
	{
		int pos = vmInHop[ID2][ID1];
		if(vvpInHop[ID2][pos].second.dominate(lpf))
		{
			hop = ID1;
			lpf = vvpInHop[ID2][pos].second;
			hit = true;
		}
	}

	if(hit)
		return true;

	if((s1 == 0 || s2 == 0) && !hit)
		return false;
	else if ((s1 ==0 || s2 == 0) && hit)
		return true;

	vector<int> v1 = {0};
	vector<int> v2 = {vvpOutHop[ID1][0].second.getUBound()};  
	lpf = LPFunction(ID1, ID2, vvpOutHop[ID1][0].second.getUBound(), v1, v2); 
	LPFunction lpftmp;

	if(s1 <= s2)
	{
		for(ivp = vvpOutHop[ID1].begin(); ivp != vvpOutHop[ID1].end(); ivp++)
		{
			hopTmp = (*ivp).first; 
			if(vmInHop[ID2].find(hopTmp) != vmInHop[ID2].end())
			{
				int pos = vmInHop[ID2][hopTmp];
				lpftmp = (*ivp).second.LPFCat(vvpInHop[ID2][pos].second);   
				if(lpftmp.dominate(lpf))
				{
					lpf = lpftmp;
					hop = hopTmp;
					hit = true;
				}
			}
		}
	}
	else
	{
		for(ivp = vvpInHop[ID2].begin(); ivp != vvpInHop[ID2].end(); ivp++)
		{
			hopTmp = (*ivp).first;
			if(vmOutHop[ID1].find(hopTmp) != vmOutHop[ID1].end())
			{
				int pos = vmOutHop[ID1][hopTmp];
				lpftmp = vvpOutHop[ID1][pos].second.LPFCat((*ivp).second);  
				if(lpftmp.dominate(lpf))
				{
					lpf = lpftmp;
					hop = hopTmp;
					hit = true;
				}
			}
		}
	}

	if(hit)
		return true;
	else 
		return false;
}
	
void T2Hop::pForwardSearch(int sourceID, Graph& g) 
{
	cout << "Pruned Forward Search " << sourceID << endl;
	boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> > fHeap;
	vector<boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> >::handle_type> vHandler(g.vNode.size());
	vector<int> vTime(g.vNode.size(), INF);  
	vector<bool> vbHeap(g.vNode.size(), false);//Store if in Heap
	vector<bool> bVisited(g.vNode.size(), false);

	vector<int>::iterator ivNN, ivNR;
	int topNodeID, neighborNodeID, neighborRoadID;
	int i, minCost;
	bool bUpdated;
	
	vTime[sourceID] = 0;
	compareNode cn;
	cn.pif = make_pair(sourceID, 0);
	vHandler[sourceID] = fHeap.push(cn);
	vbHeap[sourceID] = true;
	bVisited[sourceID] = true;
	bool bQ;

	vector<pair<int, LPFunction> > vpIn;
	vector<bool> vExist(g.vNode.size(), false);
	for(i = 0; i < (int)g.vNode.size(); i++)
	{
		LPFunction lpf;
		vpIn.push_back(make_pair(sourceID, lpf)); 
	}

	vector<bool> bSkipped(g.vNode.size(), false);

	compareNode cnTop;
	int hop;

	vector<bool> vb(g.vNode.size(), false);
	int remain = g.vNode.size();
	while(!fHeap.empty())
	{
		cnTop = fHeap.top();
		fHeap.pop();
		topNodeID = cnTop.pif.first;
		vbHeap[topNodeID] = false;
		if(!vb[topNodeID])
			remain--;
		vb[topNodeID] = true;
		bVisited[topNodeID] = true;
		if(bSkipped[topNodeID])
			continue;
		cout << sourceID << " Forward Visiting " << topNodeID << endl;
		cout << "Remaining:" << remain << endl; 
		for(i = 0; i < (int)g.vNode[topNodeID].vNeighborNode.size(); i++)
		{
			bUpdated = false;
			bQ = false;
			neighborNodeID = g.vNode[topNodeID].vNeighborNode[i];
			neighborRoadID = g.vNode[topNodeID].vNeighborRoad[i];
			minCost = vTime[neighborNodeID];
			LPFunction lpf;
			
			if(neighborNodeID == sourceID)
				continue;
	
			if(topNodeID == sourceID)
			{
				lpf = g.vRoad[neighborRoadID].costFunction;
				if( sourceID == g.vRoad[neighborRoadID].ID2)    
				{
					lpf.setID1(g.vRoad[neighborRoadID].ID2);
					lpf.setID2(g.vRoad[neighborRoadID].ID1);
				}
				lpf.setArrival(false);
			}
			else
			{
				LPFunction lpftmp = g.vRoad[neighborRoadID].costFunction;
				if(topNodeID == g.vRoad[neighborRoadID].ID2)
				{
				lpftmp.setID1(g.vRoad[neighborRoadID].ID2);
					lpftmp.setID2(g.vRoad[neighborRoadID].ID1);
				}
				lpf.setArrival(false);
				lpf = vpIn[topNodeID].second.LPFCat(lpftmp);   
			}

			if(lpf.getSize() == 1)
				continue;

			LPFunction lpfQ;
			vector<int> vHop;
//			bQ = fullQuery(sourceID, neighborNodeID, lpfQ, hop);
			bQ = fullQuery2(sourceID, neighborNodeID, lpfQ, vHop); 
			
			if(bQ && lpfQ.dominate(lpf))
			{
				cout << "Skip! " << neighborNodeID << endl;  
				bSkipped[neighborNodeID] = true;
				continue;
			}
			
			if(!vExist[neighborNodeID])
			{
				vpIn[neighborNodeID].second = lpf;
				vExist[neighborNodeID] = true;
		
//				vvpInHop[neighborNodeID].push_back(make_pair(sourceID, lpf)); 
//				vmInHop[neighborNodeID][sourceID] = vvpInHop[neighborNodeID].size()-1; 

				bUpdated = true;
			}
			else
			{
				LPFunction lpfmin;
				lpfmin = vpIn[neighborNodeID].second.LPFMinNew3(lpf);  
			
				vpIn[neighborNodeID].second = lpfmin;
//				int pos = vmInHop[neighborNodeID][sourceID];
//				vvpInHop[neighborNodeID][pos].second = lpfmin;

				bUpdated = true;
			}
			
			minCost = vpIn[neighborNodeID].second.getMin(); 

			//Updated and not in Heap
			if((!vbHeap[neighborNodeID] && bUpdated) || !bVisited[neighborNodeID] )
			{
				vTime[neighborNodeID] = minCost;
				compareNode cn;
				cn.pif = make_pair(neighborNodeID, minCost);
				vHandler[neighborNodeID] = fHeap.push(cn);
				vbHeap[neighborNodeID] = true;
				bVisited[neighborNodeID] = true;
			}
			//Updated and in Heap, key changed
			else if(bUpdated && vbHeap[neighborNodeID])
			{
				vTime[neighborNodeID] = minCost;
				(*vHandler[neighborNodeID]).pif.second = vTime[neighborNodeID];
				fHeap.decrease(vHandler[neighborNodeID]);
			}
		}
	}
	cout << endl;
	vector<pair<int, LPFunction> >::iterator ivpIn;
	vector<bool>::iterator ivExist; 
	for(ivExist = vExist.begin(), ivpIn = vpIn.begin(); ivExist != vExist.end(); ivExist++, ivpIn++)
	{
		if(*ivExist)
		{
			int id = ivpIn-vpIn.begin();
			vvpInHop[id].push_back(*ivpIn);
			vmInHop[id][sourceID] = vvpInHop[id].size()-1;
		}
	}
}


void T2Hop::pBackwardSearch(int sourceID, Graph& g) 
{
	cout << "Pruned Backward Search " << sourceID << endl;
	boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> > fHeap;
	vector<boost::heap::fibonacci_heap<compareNode, boost::heap::compare<compareNode> >::handle_type> vHandler(g.vNode.size());
	vector<int> vTime(g.vNode.size(), INF);  
	vector<bool> vbHeap(g.vNode.size(), false);//Store if in Heap
	vector<bool> bVisited(g.vNode.size(), false);

	vector<int>::iterator ivNN, ivNR;
	int topNodeID, neighborNodeID, neighborRoadID;
	int i, minCost;
	bool bUpdated;
	
	vTime[sourceID] = 0;
	compareNode cn;
	cn.pif = make_pair(sourceID, 0);
	vHandler[sourceID] = fHeap.push(cn);
	vbHeap[sourceID] = true;
	bVisited[sourceID] = true;
	bool bQ;
	vector<bool> bSkipped(g.vNode.size(), false);
	
	vector<pair<int, LPFunction> > vpOut;
	vector<bool> vExist(g.vNode.size(), false);
	for(i = 0; i < (int)g.vNode.size(); i++)
	{
		LPFunction lpf;
		vpOut.push_back(make_pair(sourceID, lpf)); 
	}

	compareNode cnTop;
	int hop;

	vector<bool> vb(g.vNode.size(), false);
	int remain = g.vNode.size(); 
	while(!fHeap.empty())
	{
		cnTop = fHeap.top();
		fHeap.pop();
		topNodeID = cnTop.pif.first;
		vbHeap[topNodeID] = false;
		bVisited[topNodeID] = true;
//		posTS = vmOutHop[topNodeID][sourceID];
		if(bSkipped[topNodeID])
			continue;
		
		if(!vb[topNodeID])
			remain--;
		vb[topNodeID] = true;
//		cout << "Visiting " << topNodeID << endl;
		
		cout << sourceID << " Backward Visiting " << topNodeID << endl;
		cout << "Remaining:" << remain << endl << endl;
		
		if(vpOut[topNodeID].second.getSize() == 1)
			continue;

		for(i = 0; i < (int)g.vNode[topNodeID].vRNeighborNode.size(); i++)
		{
			bUpdated = false;
			bQ = false;
			neighborNodeID = g.vNode[topNodeID].vRNeighborNode[i];
			neighborRoadID = g.vNode[topNodeID].vRNeighborRoad[i]; 

			if(bSkipped[neighborNodeID])

			if(neighborNodeID == sourceID)
				continue;
			minCost = vTime[neighborNodeID];
			
			LPFunction lpf;
			if(topNodeID == sourceID)
			{
				lpf = g.vRoad[neighborRoadID].costFunction;
				if( sourceID == g.vRoad[neighborRoadID].ID1)     
				{
					lpf.setID1(g.vRoad[neighborRoadID].ID2);
					lpf.setID2(g.vRoad[neighborRoadID].ID1);
				}
				lpf.setArrival(false);
			}
			else
			{
				LPFunction lpftmp = g.vRoad[neighborRoadID].costFunction;
				if(topNodeID == g.vRoad[neighborRoadID].ID1)
				{
					lpftmp.setID1(g.vRoad[neighborRoadID].ID2);
					lpftmp.setID2(g.vRoad[neighborRoadID].ID1);
				}
				
				lpf = lpftmp.LPFCat(vpOut[topNodeID].second);    
				lpf.setArrival(false);
//				lpf = (vpOut[topNodeID].second.LPFCat(lpftmp));
			}
			
			if(lpf.getSize() == 1)
			{
//				cout << "Cannot Reach " << neighborNodeID << endl;
				continue;
			}

			LPFunction lpfQ;
			vector<int> vHop;
//			bQ = fullQuery(neighborNodeID, sourceID, lpfQ, hop);
			bQ = fullQuery2(neighborNodeID, sourceID, lpfQ, vHop); 
			if(bQ && lpfQ.dominate(lpf))
			{
				cout << "Skip! " << neighborNodeID << endl;  
				bSkipped[neighborNodeID] = true;
				continue;
			}
			
//			lpf.display();
//			cout << "neighbor:" << neighborNodeID << endl;
		
			if(!vExist[neighborNodeID])
			{
				vpOut[neighborNodeID].second = lpf;
				vExist[neighborNodeID] = true;
		
	//			vvpOutHop[neighborNodeID].push_back(make_pair(sourceID, lpf)); 
	//			vmOutHop[neighborNodeID][sourceID] = vvpOutHop[neighborNodeID].size()-1; 
				bUpdated = true;
			}
			else
			{
				LPFunction lpfmin;
				lpfmin = vpOut[neighborNodeID].second.LPFMinNew3(lpf); 
					
				vpOut[neighborNodeID].second = lpfmin;
	//			int pos = vmOutHop[neighborNodeID][sourceID];
	//			vvpOutHop[neighborNodeID][pos].second = lpfmin;
				
				bUpdated = true;
			}
			
			minCost = vpOut[neighborNodeID].second.getMin(); 

			//Updated and not in Heap
			if((!vbHeap[neighborNodeID] && bUpdated) || !bVisited[neighborNodeID] )
			{
				vTime[neighborNodeID] = minCost;
				compareNode cn;
				cn.pif = make_pair(neighborNodeID, minCost);
				vHandler[neighborNodeID] = fHeap.push(cn);
				vbHeap[neighborNodeID] = true;
				bVisited[neighborNodeID] = true;

			}
			//Updated and in Heap, key changed
			else if(bUpdated && vbHeap[neighborNodeID])
			{
				vTime[neighborNodeID] = minCost;
				(*vHandler[neighborNodeID]).pif.second = vTime[neighborNodeID];
				fHeap.decrease(vHandler[neighborNodeID]);
			}
		}
	}
//	cout << endl;
	vector<pair<int, LPFunction> >::iterator ivpOut; 
	vector<bool>::iterator ivExist; 
	for(ivExist = vExist.begin(), ivpOut = vpOut.begin(); ivExist != vExist.end(); ivExist++, ivpOut++) 
	{
		if(*ivExist)
		{
			int id = ivpOut-vpOut.begin();
			vvpOutHop[id].push_back(*ivpOut);
			vmOutHop[id][sourceID] = vvpOutHop[id].size()-1;
		}
	}
}

bool T2Hop::fullQuery2(int ID1, int ID2, LPFunction &lpf, vector<int>& vHop)  
{
	int p1 = ID1;
	int p2 = ID2;
	int s1 = vvpOutHop[p1].size();
	int s2 = vvpInHop[p2].size();  

	vector<pair<int, LPFunction> >::iterator ivp;
	int hopTmp;
	int hop = -1;
	bool hit= false;
	if(vmOutHop[p1].find(ID2) != vmOutHop[p1].end())
	{
		int pos = vmOutHop[p1][ID2];
		lpf = vvpOutHop[p1][pos].second;
		hop = ID2;
		vHop.push_back(hop);
		cout << "Hit Out" << endl;
		hit = true;
	}
	
	if(vmInHop[p2].find(ID1) != vmInHop[p2].end())
	{
		int pos = vmInHop[p2][ID1];
		if(vvpInHop[p2][pos].second.dominate2(lpf)) 
		{
			hop = ID1;
//			vHop.push_back(hop);
			lpf = vvpInHop[p2][pos].second;
		}
		cout << "Hit in" << endl; 
		lpf = vvpInHop[p2][pos].second;
		hop = ID1;
		if(vHop.empty())
			vHop.push_back(hop);
		hit = true;
	}

	if(hit)
		return true;

	if((s1 == 0 || s2 == 0) && !hit)
		return false;

	vector<int> v1 = {0};
	vector<int> v2 = {vvpOutHop[p1][0].second.getUBound()};  
	lpf = LPFunction(ID1, ID2, vvpOutHop[p1][0].second.getUBound(), v1, v2); 
	lpf.maxY = INF;
	lpf.setArrival(false);
	LPFunction lpftmp;

	LPFunction lpftmp1, lpftmp2;
	if(s1 <= s2)
	{
		priority_queue<compareHop> qh;
		for(ivp = vvpOutHop[p1].begin(); ivp != vvpOutHop[p1].end(); ivp++)
		{
			int m1 = (*ivp).second.getMin();
			
			hopTmp = (*ivp).first;
			if(vmInHop[p2].find(hopTmp) != vmInHop[p2].end())
			{
				int pos = vmInHop[p2][hopTmp];
				int m2 = vvpInHop[p2][pos].second.getMin();
				compareHop ch;
				ch.pif = make_pair((*ivp).first, m1+m2);
				qh.push(ch);
			}
		}
		while(!qh.empty())
		{
			pair<int, int> p;
			p = qh.top().pif;
			qh.pop();
		
			if(p.second > lpf.getMax())
				break;

			int hopTmp = p.first;
			int pos1 = vmOutHop[p1][hopTmp];
			int pos2 = vmInHop[p2][hopTmp];
	
			lpftmp1 = vvpOutHop[p1][pos1].second;
			lpftmp2 = vvpInHop[p2][pos2].second;

			lpftmp1.setArrival(false);
			lpftmp2.setArrival(false);

			LPFunction lpftmp;
			lpftmp = lpftmp1.LPFCat(lpftmp2);
	
			cout << "Hop " << hopTmp << endl;
			lpftmp.display();
//			if(lpf.dominate5(lpftmp))
			if(lpf.dominate2(lpftmp))  
				continue;
				
			vHop.push_back(hopTmp);
			lpf = lpf.LPFMinNew3(lpftmp);  
//			cout << "AA" << endl;
//			lpf.display(); 
			lpf.setArrival(false); 
			hit = true;
		}
	}
	else
	{
		priority_queue<compareHop> qh;
		for(ivp = vvpInHop[p2].begin(); ivp != vvpInHop[p2].end(); ivp++)
		{
			int m2 = (*ivp).second.getMin();

			hopTmp = (*ivp).first;
			if(vmOutHop[p1].find(hopTmp) != vmOutHop[p1].end())
			{
				int pos = vmOutHop[p1][hopTmp]; 
				int m1 = vvpOutHop[p1][pos].second.getMin();
				compareHop ch;
				ch.pif = make_pair((*ivp).first, m1+m2);
				qh.push(ch);
			}
		}

		while(!qh.empty())
		{
			pair<int, int> p;
			p = qh.top().pif;
			qh.pop();
			int hopTmp = p.first;
		
			if(p.second > lpf.getMax())
				break;

			int pos1 = vmOutHop[p1][hopTmp];
			int pos2 = vmInHop[p2][hopTmp];
	
			lpftmp1 = vvpOutHop[p1][pos1].second;
			lpftmp2 = vvpInHop[p2][pos2].second;

			lpftmp1.setArrival(false);
			lpftmp2.setArrival(false);

			LPFunction lpftmp;
			lpftmp = lpftmp1.LPFCat(lpftmp2); 
	
			cout << "Hop " << hopTmp << endl;
			lpftmp.display();

			if(lpf.dominate(lpftmp)) 
				continue;
				
			vHop.push_back(hopTmp); 
//			cout << "BB" << endl;
			lpf = lpf.LPFMinNew3(lpftmp);
//			lpf.display(); 
			lpf.setArrival(false); 
			hit = true;
		}
	}

	if(hit)
	{
//		cout << "Hit Combine" << endl;
		return true;
	}
	else 
		return false;
}
