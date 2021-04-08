#include "road.h"

RoadNetwork::RoadNetwork(char* argv[])
{
	conf = new Configuration();
	if(conf->bSpecialNode)
	{
		cout << "Has special node" << endl;
		if(conf->bBuild)
		{
			cout << "Build Special node" << endl;
			buildBackgroundGraph();
			createSpecialNode();
		}
		else
		{
			readSpecialNode();
			readSpecialRoad();
			if(conf->bTemporal)
				readSpeedProfile();
	
	///		if(conf->bIndex)
	//			loadIndex();
		}	
	}
	else//exe
		//modified
		//buildBackgroundGraph();
		buildMapGraph();

	if(conf->bTest)
		bTest = true;
	else
		bTest = false;

//	analysis();

	if(conf->bPathIndex)
	{
/*		if(conf->sPathIndex == "SCH")
			loadSCH();
		else if(conf->sPathIndex == "TCH")
			loadTCH();
		else if(conf->sPathIndex == "DTCH") 
			loadDTCH();
		else if(conf->sPathIndex == "GTCH") 
		{
			grid.init(&g, conf);
			grid.createGrid();
			stringstream ss;
			int gID;
			ss << conf->GTCHID;
			ss >> gID;
			loadGTCH(gID);
		}
		else if(conf->sPathIndex == "T2HOP")
		{
			if(conf->bGrid)
			{
				grid.init(&g, conf);
				grid.createGrid();
				grid.loadT2HopGrid(argv[1]);
			}
			else
				loadT2Hop();
		}
*/	}

}

void RoadNetwork::analysis()
{
	vector<node>::iterator ivNode;
	vector<road>::iterator ivRoad;
	int i;
	int count = 0;
	int scount = 0;
	int scount2 = 0;
	for(ivNode = g.vNode.begin(); ivNode != g.vNode.end(); ivNode++)
	{
		int nodeID = (*ivNode).nodeID;
		for(i = 0; i < (int)(*ivNode).vNeighborNode.size(); i++) 
		{
			int neighborRoadID = (*ivNode).vNeighborRoad[i];
			int neighborNodeID = (*ivNode).vNeighborNode[i];
			int ID1 = g.vRoad[neighborRoadID].ID1;
			int ID2 = g.vRoad[neighborRoadID].ID2; 
			if((nodeID == ID1 && neighborNodeID == ID2) || (nodeID == ID2 && neighborNodeID == ID1))
			{
				if(g.vNode[ID1].bSpecial || g.vNode[ID2].bSpecial)
					scount2++;
			}
			else
			{
				count++;
				
				cout << "nodeID:" << nodeID << "\t neighborNodeID:" << neighborNodeID << "\t roadID:" << neighborRoadID  << "\t ID1:" << ID1 << "\tID2:" << ID2 << endl;

				if(ID1 != nodeID && ID1 != neighborNodeID)
				{
					cout << "ID1!!!" << endl;
					cout << "ID1 " << ID1 << " is not correct" << endl;
					set<int>::iterator isNeighborNode;
					if(ID2 == nodeID)
					{
						bool b = false;
						bool b2 = false;
						for(isNeighborNode = g.vNode[ID2].sNeighborNode.begin(); isNeighborNode != g.vNode[ID2].sNeighborNode.end(); isNeighborNode++) 
						{
							if(neighborNodeID == *isNeighborNode)
							{
								b = true;
								break;
							}
						}
						for(int j = 0; j < (int)g.vNode[ID1].vNeighborNode.size(); j++)
						{
							if(ID2  == g.vNode[ID1].vNeighborNode[j])
							{
								b2 = true;
								cout << "ID2 is in ID1's neighborhoood\tcooresponding roadID:" << g.vNode[ID1].vNeighborRoad[j] << endl;  
								break;
							}
						}
						for(int j = 0; j < (int)g.vNode[ID1].vRNeighborNode.size(); j++)
						{
							if(ID2  == g.vNode[ID1].vRNeighborNode[j])
							{
								b2 = true;
								cout << "ID2 is in ID1's neighborhoood\tcooresponding roadID:" << g.vNode[ID1].vRNeighborRoad[j] << endl;  
								break;
							}
						}

						if(b)
							cout << "neighborNode:" << neighborNodeID << " is in ID2's neighborhood" << endl; 
						else
							cout << "neighborNode:" << neighborNodeID << " is not in ID2's neighborhood" << endl; 

						if(!b2)
							cout << "ID2 is NOT in ID1's neighborhoood" << endl; 
						cout << endl;
					}
					else if(ID2 == neighborNodeID)
					{
						bool b = false; 
						bool b2 = false;
						for(isNeighborNode = g.vNode[ID2].sNeighborNode.begin(); isNeighborNode != g.vNode[ID2].sNeighborNode.end(); isNeighborNode++) 
						{
							if(nodeID == *isNeighborNode)
							{
								b = true;
								break;
							}
						}
						
						for(int j = 0; j < (int)g.vNode[ID1].vNeighborNode.size(); j++)
						{
							if(ID2  == g.vNode[ID1].vNeighborNode[j])
							{
								b2 = true;
								cout << "ID2 is in ID1's neighborhoood\tcooresponding roadID:" << g.vNode[ID1].vNeighborRoad[j] << endl;  
								break;
							}
						}
						for(int j = 0; j < (int)g.vNode[ID1].vRNeighborNode.size(); j++)
						{
							if(ID2  == g.vNode[ID1].vRNeighborNode[j])
							{
								b2 = true;
								cout << "ID2 is in ID1's neighborhoood\tcooresponding roadID:" << g.vNode[ID1].vRNeighborRoad[j] << endl;  
								break;
							}
						}
						
						if(b)
							cout << "nodeID:" << nodeID << " is in ID2's neighborhood" << endl; 
						else
							cout << "nodeID:" << nodeID << " is not in ID2's neighborhood" << endl; 

						if(!b2)
							cout << "ID2 is NOT in ID1's neighborhoood" << endl; 
						cout << endl;
					}
				}
				else if(ID2 != nodeID && ID2 != neighborNodeID)
				{
					cout << "ID2!!!" << endl;
					cout << "ID2 " << ID2 << " is not correct" << endl; 
					cout << "DIR" << g.vRoad[neighborRoadID].direction <<  endl; 
					if(neighborRoadID > 387587)
						cout << "NEW ROAD" << endl; 
					else
						cout << "OLD ROAD" << endl; 
					set<int>::iterator isNeighborNode;
					if(ID1 == nodeID)
					{
						bool b = false;
						bool b2 = false;
						for(isNeighborNode = g.vNode[ID1].sNeighborNode.begin(); isNeighborNode != g.vNode[ID1].sNeighborNode.end(); isNeighborNode++) 
						{
							if(neighborNodeID == *isNeighborNode)
							{
								b = true;
								break;
							}
						}
						
						for(int j = 0; j < (int)g.vNode[ID2].vNeighborNode.size(); j++)
						{
							if(ID1  == g.vNode[ID1].vNeighborNode[j])
							{
								b2 = true;
								cout << "ID1 is in ID2's neighborhoood\tcooresponding roadID:" << g.vNode[ID2].vNeighborRoad[j] << endl;  
								break;
							}
						}
						for(int j = 0; j < (int)g.vNode[ID2].vRNeighborNode.size(); j++)
						{
							if(ID1  == g.vNode[ID2].vRNeighborNode[j])
							{
								b2 = true;
								cout << "ID1 is in ID2's neighborhoood\tcooresponding roadID:" << g.vNode[ID2].vRNeighborRoad[j] << endl;  
								break;
							}
						}

						if(b)
							cout << "neighborNode:" << neighborNodeID << " is in ID1's neighborhood" << endl;  
						else
							cout << "neighborNode:" << neighborNodeID << " is not in ID1's neighborhood" << endl; 

						if(!b2)
							cout << "ID1 is in ID2's neighborhoood" << endl; 
						cout << endl;
					}
					else if(ID1 == neighborNodeID)
					{
						bool b = false;
						bool b2 = false;
						for(isNeighborNode = g.vNode[ID1].sNeighborNode.begin(); isNeighborNode != g.vNode[ID1].sNeighborNode.end(); isNeighborNode++) 
						{
							if(nodeID == *isNeighborNode)
							{
								b = true;
								break;
							}
						}
						
						for(int j = 0; j < (int)g.vNode[ID2].vNeighborNode.size(); j++)
						{
							if(ID1  == g.vNode[ID1].vNeighborNode[j])
							{
								b2 = true;
								cout << "ID1 is in ID2's neighborhoood\tcooresponding roadID:" << g.vNode[ID2].vNeighborRoad[j] << endl;  
								break;
							}
						}
						for(int j = 0; j < (int)g.vNode[ID2].vRNeighborNode.size(); j++)
						{
							if(ID1  == g.vNode[ID2].vRNeighborNode[j])
							{
								b2 = true;
								cout << "ID1 is in ID2's neighborhoood\tcooresponding roadID:" << g.vNode[ID2].vRNeighborRoad[j] << endl;  
								break;
							}
						}
						
						if(b)
							cout << "nodeID:" << nodeID << " is in ID1's neighborhood" << endl; 
						else
							cout << "nodeID:" << nodeID << " is not in ID1's neighborhood" << endl; 

						if(!b2)
							cout << "ID1 is NOT in ID2's neighborhoood" << endl; 
						cout << endl;
					}

				}

				if(g.vNode[ID1].bSpecial || g.vNode[ID2].bSpecial)
					scount++;
			}
		}
	}
	cout << "Issue road number:" << count << endl; 
	cout << "There are " << scount << " speical node inside" << endl;
	cout << "There are " << scount2 << " speical node outside" << endl;
}

int RoadNetwork::buildBackgroundGraph()
{
	cout << "Building Background Graph" << endl;
	if(!conf->bNewGraphFile)
	{
		createNewGraphFile();
		writeNewGraphFile();
		return 0;
	}
	else//exe
	{
		readNode();
		readRoad();
		if(conf->bBuild)//noexe
		{
//			loadIndex();
		}
		if(conf->bTemporal)//exe
			readSpeedProfile();
	}

	return 0;
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
		//readSpeedProfile();
	return 0;
}

int RoadNetwork::createNewGraphFile()
{
		cout << "Creating new Graph from Original Data" << endl;
		readOrgNodeMap();
		cout << "mNodeMap size:" << mNodeMap.size() << endl;
		readOrgRoad();
		readOrgNode();
		return 0;
}

int	RoadNetwork::readOrgNodeMap()
{
	ifstream inNM(conf->nodeMapFile.c_str());
	if(!inNM)
	{
		cout << "Cannot open node Map file:" << conf->nodeMapFile << endl;
		return -1;
	}
	cout << "Reading nodeMapFile " << conf->nodeMapFile << endl;

	double d1, d2;
	while(inNM >> d1)
	{
		inNM >> d2;
		mNodeMap[d1] = d2;
		mNodeMapFull[d1] = true;
		mNodeMapFull[d2] = true;
	}
	inNM.close();

	return 0;
}

int RoadNetwork::readOrgRoad()
{
	ifstream inRoadFile(conf->roadFile.c_str());
	if(!inRoadFile)
	{
		cout << "Cannot open Original road file" << conf->roadFile << endl;
		return -1;
	}
	cout << "Reading Original Road File: " << conf->roadFile << endl;

	int k = 0;
	string sTmp;
	double idTmp;
	vector<string> vs, vscoor;
	while(getline(inRoadFile, sTmp))
	{
		vs = split(sTmp, "\t");
		road ri;
		ri.roadID = k;
		ri.orgRoadID = parseFloat(vs[0]);
		mRoadIDTrans[ri.orgRoadID] = k;
		mRRoadIDTrans[k] = ri.orgRoadID;

		ri.direction = parseInt(vs[1]);
		ri.length = parseInt(vs[2]);
		
		idTmp = parseFloat(vs[3]);
		if(mNodeMap.find(idTmp) != mNodeMap.end())
		{
			idTmp = mNodeMap[idTmp];
		}
		ri.orgID1 = idTmp;

		idTmp = parseFloat(vs[4]);
		if(mNodeMap.find(idTmp) != mNodeMap.end())
		{
			idTmp = mNodeMap[idTmp];
		}
		ri.orgID2 = idTmp;

		ri.isolated = false;

		for(int i = 5; i < (int)vs.size(); i++)
		{
			vscoor = split(vs[i],",");
			ri.vpRoadDetail.push_back(make_pair(parseFloat(vscoor[0]), parseFloat(vscoor[1])));
			vscoor.clear();
		}

		g.vRoad.push_back(ri);
		k++;
		vs.clear();
	}
	inRoadFile.close();

	ifstream inIso(conf->isoRoadFile.c_str());
	if(inIso)
	{
		cout << "Reading Isolated Road File " << conf->isoRoadFile << endl;
		int		roadID;
		while(inIso >> roadID)
			g.vRoad[mRoadIDTrans[roadID]].isolated = true;
		inIso.close();
	}
	else
		cout << "Cannot open isolated road file:" << conf->isoRoadFile << endl;

	return 0;
}

int RoadNetwork::readOrgNode()
{
	ifstream inNodeFile(conf->nodeFile.c_str());
	if(!inNodeFile)
	{
		cout << "Cannot open node file" << conf->nodeFile << endl;
		return -1;
	}
	cout << "Reading Original Node File: " << conf->nodeFile << endl;

	string sTmp;

	int nodeNum, i, j, num, k, p;
	double dtmp;
	double nodeID;
	int newNodeID;
	map<double, int>::iterator imSNR1, imSNR2;
	vector<int> vRoadTmp;
	vector<int>::iterator ivRoad1, ivRoad2;
	vector<road>::iterator ivRoad;
	inNodeFile >> nodeNum;
	cout << "Node number: " << nodeNum << endl;
	k = 0;
	p = 0;
	maxX = 0;
	maxY = 0;
	minX = 90;
	minY = 180;
	for(i = 0; i < nodeNum; i++)
	{
		inNodeFile >> nodeID;
		if(mNodeMap.find(nodeID) != mNodeMap.end())
			nodeID = mNodeMap[nodeID];

		//New node
		if(mNodeIDTrans.find(nodeID) == mNodeIDTrans.end())
		{
			node n;
			n.orgNodeID = nodeID;
			n.nodeID = k;
			newNodeID = k;
			mNodeIDTrans[n.orgNodeID] = k;	
			mRNodeIDTrans[k] = n.orgNodeID;	
			k++;
			inNodeFile >> n.type;
			inNodeFile >> num;
			for(j = 0; j < num; j++)	//mainNeighbor Road
			{
				inNodeFile >> dtmp;
			}
			inNodeFile >> n.orgMainID;
	
			inNodeFile >> num;
			for(j = 0; j < num; j++)	//subID
			{
				inNodeFile >> dtmp;
				n.vOrgSubID.push_back(dtmp);
			}
			inNodeFile >> n.orgNeighborNode;
			n.isolated = false;
			inNodeFile >> num;
			for(j = 0; j < num; j++)	//subNeighbor Road
			{
				inNodeFile >> dtmp;
				int roadID = mRoadIDTrans[dtmp];
				n.sNeighborRoad.insert(roadID);
			}
			inNodeFile >> n.x;
			inNodeFile >> n.y;
			updateMMXY(n.x, n.y);	//update the min/max XY
			g.vNode.push_back(n);
		}
		//Existing Node with different ID
		else
		{
			p++;
			newNodeID = mNodeIDTrans[nodeID];
			if(newNodeID == 0)
				cout << nodeID << "\t" << newNodeID << endl;
			for(j = 0; j < 5; j++)
				inNodeFile >> dtmp;
			inNodeFile >> num;
			for(j = 0; j < num; j++)
			{
				inNodeFile >> dtmp;
				int roadID = mRoadIDTrans[dtmp];
				g.vNode[newNodeID].sNeighborRoad.insert(roadID);
			}
			inNodeFile >> dtmp;
			inNodeFile >> dtmp;
		}
	}
	inNodeFile.close();

	int id1, id2;
	for(ivRoad = g.vRoad.begin(); ivRoad != g.vRoad.end(); ivRoad++)
	{
		if(mNodeMap.find((*ivRoad).orgID1) != mNodeMap.end())
			id1 = mNodeIDTrans[mNodeMap[(*ivRoad).orgID1]];
		else
			id1 = mNodeIDTrans[(*ivRoad).orgID1];

		if(mNodeMap.find((*ivRoad).orgID2) != mNodeMap.end())
			id2 = mNodeIDTrans[mNodeMap[(*ivRoad).orgID2]];
		else
			id2 = mNodeIDTrans[(*ivRoad).orgID2];

		if(id1==0 || id2 == 0)
			cout << setprecision(15) << id1 << "\t" << id2 << "\t" << (*ivRoad).roadID << "\t" << (*ivRoad).orgID1 << "\t" << (*ivRoad).orgID2 << endl;


		if((*ivRoad).direction == 0 || (*ivRoad).direction == 1)
		{
			g.vNode[id1].mNeighborLength[id2] = (*ivRoad).length;
			g.vNode[id1].mSubNeighborRoad[id2] = (*ivRoad).roadID;
			g.vNode[id1].mRSubNeighborRoad[id2] = (*ivRoad).roadID;
			g.vNode[id2].mNeighborLength[id1] = (*ivRoad).length;
			g.vNode[id2].mSubNeighborRoad[id1] = (*ivRoad).roadID;
			g.vNode[id2].mRSubNeighborRoad[id1] = (*ivRoad).roadID;
		}
		else if((*ivRoad).direction == 2)
		{
			g.vNode[id1].mNeighborLength[id2] = (*ivRoad).length;
			g.vNode[id1].mSubNeighborRoad[id2] = (*ivRoad).roadID;
			g.vNode[id2].mRSubNeighborRoad[id1] = (*ivRoad).roadID;
		}
		else if((*ivRoad).direction == 3)
		{
			g.vNode[id2].mNeighborLength[id1] = (*ivRoad).length;
			g.vNode[id2].mSubNeighborRoad[id1] = (*ivRoad).roadID;
			g.vNode[id1].mRSubNeighborRoad[id2] = (*ivRoad).roadID;
		}
	}

	vector<node>::iterator	ivNode;
	map<double, int>::iterator	imSNR;
	set<int> sNR;
	set<int>::iterator isNR;
	for(ivNode = g.vNode.begin(); ivNode != g.vNode.end(); ivNode++)
	{
		sNR.clear();
		for(isNR = (*ivNode).sNeighborRoad.begin(); isNR != (*ivNode).sNeighborRoad.end(); isNR++)
		{
			sNR.insert(*isNR);
		}
		for(isNR = (*ivNode).sNeighborRoad.begin(); isNR != (*ivNode).sNeighborRoad.end(); isNR++)
		{
			g.vRoad[*isNR].sNeighborRoad.insert(sNR.begin(), sNR.end());
			g.vRoad[*isNR].sNeighborRoad.erase(*isNR);
		}
	}

	ifstream ifIso(conf->isoNodeFile.c_str());
	if(ifIso)
	{
		int	nodeID;
		while(ifIso >> nodeID)
		{
			if(mNodeMap.find(nodeID) != mNodeMap.end())
			{
				nodeID = mNodeMap[nodeID];
			}
			g.vNode[mNodeIDTrans[nodeID]].isolated = true;
		}
		ifIso.close();
	}
	else
		cout << "Cannot open isolated node file" << endl;
	
	return 0;
}
	
int RoadNetwork::writeNewGraphFile()
{
	writeNewNode();
	writeNewRoad();
	return 0;
}

int	RoadNetwork::writeNewNode()
{
	cout << "Writing New Node File" << endl;
	vector<node>::iterator ivNode;
	map<int, int>::iterator imSNR, imRSNR;
	map<int, float>::iterator imNL;
	
	ofstream ofNode((conf->nodeFile+"New").c_str());
	ofNode << g.vNode.size() << "\t" << minX << "\t" << maxX << "\t" << minY << "\t" << maxY << endl;
	for(ivNode = g.vNode.begin(); ivNode != g.vNode.end(); ivNode++)
	{
		ofNode << setprecision(15) << (*ivNode).nodeID << "\t" << (*ivNode).type << "\t" << (*ivNode).x << "\t" << (*ivNode).y << "\t";
		if((*ivNode).isolated)
			ofNode << 1 << "\t";
		else
			ofNode << 0 << "\t";

		ofNode << (*ivNode).mNeighborLength.size() << "\t";
		for(imNL = (*ivNode).mNeighborLength.begin(); imNL != (*ivNode).mNeighborLength.end(); imNL++)
			ofNode << (*imNL).first << "\t" << (*imNL).second << "\t";

		ofNode << (*ivNode).mSubNeighborRoad.size() << "\t";
		for(imSNR = (*ivNode).mSubNeighborRoad.begin(); imSNR != (*ivNode).mSubNeighborRoad.end(); imSNR++)
			ofNode << (*imSNR).first << "\t" << (*imSNR).second << "\t";

		ofNode << (*ivNode).mRSubNeighborRoad.size() << "\t";
		for(imRSNR = (*ivNode).mRSubNeighborRoad.begin(); imRSNR != (*ivNode).mRSubNeighborRoad.end(); imRSNR++)
			ofNode << (*imRSNR).first << "\t" << (*imRSNR).second << "\t";
		ofNode << endl;
	}

	ofNode.close();

	ofstream ofNodeMap((conf->nodeFile+"NewMap").c_str());
	map<double,int>::iterator imN;
	for(imN = mNodeIDTrans.begin(); imN != mNodeIDTrans.end(); imN++)
		ofNodeMap << setprecision(15) << (*imN).first << "\t" << (*imN).second << endl;
	ofNodeMap.close();

	return 0;
}
	
int	RoadNetwork::writeNewRoad()
{
	cout << "Writing New Road File" << endl;
	vector<road>::iterator ivRoad;
	set<int>::iterator isNR;

	ofstream ofRoad((conf->roadFile+"New").c_str());
	ofRoad << g.vRoad.size() << endl;
	for(ivRoad = g.vRoad.begin(); ivRoad != g.vRoad.end(); ivRoad++)
	{
		ofRoad << (*ivRoad).roadID << "\t" << (*ivRoad).direction << "\t" << (*ivRoad).length << "\t" << mNodeIDTrans[(*ivRoad).orgID1] << "\t" << mNodeIDTrans[(*ivRoad).orgID2] << "\t";
		if((*ivRoad).isolated)
			ofRoad << 1 << "\t";
		else
			ofRoad << 0 << "\t";

		ofRoad << (*ivRoad).sNeighborRoad.size() << "\t";
		for(isNR = (*ivRoad).sNeighborRoad.begin(); isNR != (*ivRoad).sNeighborRoad.end(); isNR++)
			ofRoad << *isNR << "\t";

		ofRoad << endl;
	}
	ofRoad.close();

	ofstream ofRoadMap((conf->roadFile+"NewMap").c_str());
	map<double, int>::iterator imRT;
	for(imRT = mRoadIDTrans.begin(); imRT != mRoadIDTrans.end(); imRT++)
	{
		ofRoadMap << setprecision(15) << (*imRT).first << "\t" << (*imRT).second << endl;
	}
	ofRoadMap.close();

	return 0;
}
	
int RoadNetwork::readNode()
{
	ifstream inNodeFile(conf->nodeFile.c_str());
	if(!inNodeFile)
	{
		cout << "Cannot open new node file" << conf->nodeFile << endl;
		return -1;
	}
	cout << "Reading new Node file: " << conf->nodeFile << endl;

	int nodeNum;
	inNodeFile >> nodeNum >> minX >> maxX >> minY >> maxY;
	cout << "Node number: " << nodeNum << endl;

	int i,j,k,i1,i2;
	float f;
	for(i = 0; i < nodeNum; i++)
	{
		node n;
		inNodeFile >> n.nodeID >> n.type >> n.x >> n.y >> j;
		if(j)// no exe
			n.isolated = true;
		else//no exe
			n.isolated = false;
		
		n.bSpecial = false;

		inNodeFile >> j;
		for(k = 0; k < j; k++)
		{
			inNodeFile >> i1 >> f;
			n.mNeighborLength[i1] = f;//ignore
			n.vNeighborNode.push_back(i1);
			n.vNeighborLength.push_back(f);//ignore
			n.sNeighborNode.insert(i1);
		}
		
		inNodeFile >> j;
		for(k = 0; k < j; k++)//node based road //and some varible that will be infected in road struct.
		{
			inNodeFile >> i1 >> i2;
			n.mSubNeighborRoad[i1] = i2;//part road from i1->i2
			n.vNeighborRoad.push_back(i2);
			n.sNeighborRoad.insert(i2);
		}
		
		inNodeFile >> j;
		for(k = 0; k < j; k++)
		{
			inNodeFile >> i1 >> i2;
			n.mRSubNeighborRoad[i1] = i2;//part road from i1->i2
			n.sNeighborNode.insert(i1);//all node 
			n.sNeighborRoad.insert(i2);//all road
			n.vRNeighborNode.push_back(i1);//part
			n.vRNeighborRoad.push_back(i2);//part

			//add 2 hang
			//n.vNeighborNode.push_back(i1);
			//n.vNeighborRoad.push_back(i2);
		}
		g.vNode.push_back(n);
	}
	inNodeFile.close();
	
	return 0;
}

int RoadNetwork::readRoad()
{
	ifstream inRoadFile(conf->roadFile.c_str());
	if(!inRoadFile)
	{
		cout << "Cannot open new road file" << conf->roadFile << endl;
		return -1;
	}
	cout << "Reading new Road File: " << conf->roadFile << endl;//exe

	int roadNum;
	inRoadFile >> roadNum;
	cout << "Road Number:" << roadNum << endl;
	int i,j,k,p;
	for(i = 0; i < roadNum; i++)
	{
		road r;
		inRoadFile >> r.roadID >> r.direction >> r.length >> r.ID1 >> r.ID2 >> j;
		if(j)
			r.isolated = true;
		else
			r.isolated = false;

		inRoadFile >> j;
		for(k = 0; k < j; k++)
		{
			inRoadFile >> p;
			r.sNeighborRoad.insert(p);
		}
		g.vRoad.push_back(r);
	}
	inRoadFile.close();
	

	ifstream ifRoadMap((conf->roadFile+"Map").c_str());
	if(!ifRoadMap)
	{
		cout << "Cannot open road map file" << conf->roadFile+"Map" << endl;
	}
	else
	{
		cout << "Reading Road Map File" << endl;
		double oldID;
		int newID;
		while(ifRoadMap >> oldID >> newID)
		{
			mRoadIDTrans[oldID] = newID;
			mRRoadIDTrans[newID] = oldID;
		}
		ifRoadMap.close();
	}

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
		neibor nei;
		nei.neiborNodeID = r.ID2;
		nei.neiborRoadID = r.roadID;
		//neiborNode and neiborRoad
		startNode.vNeibor.push_back(nei);
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

void RoadNetwork::createSpecialNode()
{
	readOrgSpecialNode();
	writeSpecialNode();
	writeSpecialRoad();
}

int RoadNetwork::readOrgSpecialNode()
{
	ifstream inSpecialNode(conf->orgSpecialNodeFile.c_str());
	if(!inSpecialNode)
	{
		cout << "Cannot Open Special Node File:" << conf->orgSpecialNodeFile << endl;
		return -1;
	}

	cout << "Reading " << conf->specialNodeType << " Special Node File" << conf->orgSpecialNodeFile << endl;
	double x,y, nx, ny;
//	vector<simpleNode> vSimpleNode;
//	vector<simpleRoad> vSimpleRoad;
//	vector<simpleRoad>::iterator ivSR;
/*	int minRoadID, i;
	double minDistance, currentDistance, lineDistance, newRoadLength;
	set<int>::iterator isNR, isNR2;
	int count = 0;
	bool bAttached = false;
	while(inSpecialNode >> y >> x)
	{
		count++;
		minDistance = 999999;
		vSimpleRoad.clear();
	//	vSimpleRoad = quadTree->GetRoadAt(x,y);
		bAttached = false;
		for(ivSR = vSimpleRoad.begin(); ivSR != vSimpleRoad.end(); ivSR++)
		{
			GetFootOfPerpendicular(x, y, (*ivSR).x1, (*ivSR).y1, (*ivSR).x2, (*ivSR).y2, nx, ny);
			if(!nodeOnRoad(nx, ny, (*ivSR).x1, (*ivSR).y1, (*ivSR).x2, (*ivSR).y2))
				continue;
			bAttached = true;
			currentDistance = nodeRoadDistance(x, y, (*ivSR).x1, (*ivSR).y1, (*ivSR).x2, (*ivSR).y2);
			lineDistance = sqrt(pow((*ivSR).y2 - (*ivSR).y1, 2) + pow((*ivSR).x2 - (*ivSR).x1, 2));
			newRoadLength = g.vRoad[(*ivSR).roadID].length / lineDistance * currentDistance;
			if(newRoadLength < minDistance)
			{
				minDistance = newRoadLength;
				minRoadID = (*ivSR).roadID;
			}
		}

		if(!bAttached)
			continue;
		
		node n;
		n.nodeID = g.vNode.size();
		GetFootOfPerpendicular(x, y, g.vNode[g.vRoad[minRoadID].ID1].x, g.vNode[g.vRoad[minRoadID].ID1].y, g.vNode[g.vRoad[minRoadID].ID2].x, g.vNode[g.vRoad[minRoadID].ID2].y, nx, ny);
		n.x = nx;
		n.y = ny;
		n.bSpecial = true;
		
		road r;
		r.roadID = g.vRoad.size();
		r.length = ceil((sqrt(pow(nx-g.vNode[g.vRoad[minRoadID].ID2].x,2)+pow(ny-g.vNode[g.vRoad[minRoadID].ID2].y,2))) / (sqrt(pow(g.vNode[g.vRoad[minRoadID].ID2].x - g.vNode[g.vRoad[minRoadID].ID1].x,2)+pow(g.vNode[g.vRoad[minRoadID].ID2].y-g.vNode[g.vRoad[minRoadID].ID1].y,2))) * g.vRoad[minRoadID].length);
//		r.length = ceil(g.vRoad[minRoadID].length * (nx-g.vNode[g.vRoad[minRoadID].ID2].x)/(g.vNode[g.vRoad[minRoadID].ID1].x - g.vNode[g.vRoad[minRoadID].ID2].x));
		r.isolated = false;
		r.ID1 = n.nodeID;
		r.ID2 = g.vRoad[minRoadID].ID2;
		if(r.length > g.vRoad[minRoadID].length)
			continue;
		if(r.length == g.vRoad[minRoadID].length)
		{
			g.vNode[g.vRoad[minRoadID].ID1].bSpecial = true;
			continue;
		}
		else if(r.length == 0)
		{
			g.vNode[g.vRoad[minRoadID].ID2].bSpecial = true;
			continue;
		}

		//Update sNR info of ID2's connected Road
		vector<int> vTmp;
		vector<int>::iterator ivTmp;
		for(isNR = g.vRoad[minRoadID].sNeighborRoad.begin(); isNR != g.vRoad[minRoadID].sNeighborRoad.end(); isNR++)
			vTmp.push_back(*isNR);
		
		for(ivTmp = vTmp.begin(); ivTmp != vTmp.end(); ivTmp++)
		{
			if(g.vRoad[*ivTmp].ID1 == g.vRoad[minRoadID].ID2 || g.vRoad[*ivTmp].ID2 == g.vRoad[minRoadID].ID2)
			{
				g.vRoad[*ivTmp].sNeighborRoad.erase(minRoadID);
				g.vRoad[*ivTmp].sNeighborRoad.insert(r.roadID);
				g.vRoad[minRoadID].sNeighborRoad.erase(*ivTmp);
				g.vRoad[minRoadID].sNeighborRoad.insert(r.roadID);
				r.sNeighborRoad.insert(*ivTmp);
			}
		}
		r.sNeighborRoad.insert(minRoadID);
		g.vRoad[minRoadID].sNeighborRoad.insert(r.roadID);
		
		int orgLength = g.vRoad[minRoadID].length;
		g.vRoad[minRoadID].length -= r.length;
		if(g.vRoad[minRoadID].length < 0)
		{
			cout << minRoadID << " length is 0!\torgID:"<< mRRoadIDTrans[minRoadID] << "\t" << orgLength << "\t" << r.length << endl << g.vNode[g.vRoad[minRoadID].ID1].x << "\t" << g.vNode[g.vRoad[minRoadID].ID1].y << "\t" << g.vNode[g.vRoad[minRoadID].ID2].x << "\t" << g.vNode[g.vRoad[minRoadID].ID2].y << "\t" << nx << "\t" << ny << endl; 
			continue;
		}

		if(g.vRoad[minRoadID].direction == 0 || g.vRoad[minRoadID].direction == 1)
		{
			r.direction = g.vRoad[minRoadID].direction; 
			g.vNode[n.nodeID].vNeighborNode.push_back(g.vRoad[minRoadID].ID2);
			g.vNode[n.nodeID].vNeighborRoad.push_back(r.roadID);
			g.vNode[n.nodeID].vNeighborLength.push_back(r.length);
			
			g.vNode[n.nodeID].vNeighborNode.push_back(g.vRoad[minRoadID].ID1);
			g.vNode[n.nodeID].vNeighborRoad.push_back(minRoadID);
			g.vNode[n.nodeID].vNeighborLength.push_back(g.vRoad[minRoadID].length);
			
			g.vNode[n.nodeID].vRNeighborNode.push_back(g.vRoad[minRoadID].ID2);
			g.vNode[n.nodeID].vRNeighborRoad.push_back(r.roadID);
			g.vNode[n.nodeID].vRNeighborLength.push_back(r.length);
			
			g.vNode[n.nodeID].vRNeighborNode.push_back(g.vRoad[minRoadID].ID1);
			g.vNode[n.nodeID].vRNeighborRoad.push_back(minRoadID);
			g.vNode[n.nodeID].vRNeighborLength.push_back(g.vRoad[minRoadID].length);
			
			//Update ID2's Info
//			if(g.vNode[g.vRoad[minRoadID].ID2].mSubNeighborRoad.find(g.vRoad[minRoadID].ID1) != g.vNode[g.vRoad[minRoadID].ID2].mSubNeighborRoad.end())
//			{
				g.vNode[g.vRoad[minRoadID].ID2].sNeighborNode.erase(g.vRoad[minRoadID].ID1);
				g.vNode[g.vRoad[minRoadID].ID2].sNeighborRoad.erase(minRoadID);
				g.vNode[g.vRoad[minRoadID].ID2].sNeighborNode.insert(n.nodeID);
				g.vNode[g.vRoad[minRoadID].ID2].sNeighborRoad.insert(r.roadID);
				g.vNode[g.vRoad[minRoadID].ID2].mNeighborLength.erase(g.vRoad[minRoadID].ID1);
				g.vNode[g.vRoad[minRoadID].ID2].mSubNeighborRoad.erase(g.vRoad[minRoadID].ID1);
				g.vNode[g.vRoad[minRoadID].ID2].mRSubNeighborRoad.erase(g.vRoad[minRoadID].ID1);
				g.vNode[g.vRoad[minRoadID].ID2].mNeighborLength[n.nodeID] = r.length;
				g.vNode[g.vRoad[minRoadID].ID2].mSubNeighborRoad[n.nodeID] = r.roadID;
				g.vNode[g.vRoad[minRoadID].ID2].mRSubNeighborRoad[n.nodeID] = r.roadID;
//			}

*/
/*			for(i = 0; i < (int)g.vNode[g.vRoad[minRoadID].ID2].vNeighborNode.size(); i++)
			{
				if(g.vNode[g.vRoad[minRoadID].ID2].vNeighborNode[i] == g.vRoad[minRoadID].ID1)
				{
					g.vNode[g.vRoad[minRoadID].ID2].vNeighborNode[i] = n.nodeID;
					g.vNode[g.vRoad[minRoadID].ID2].vNeighborRoad[i] = r.roadID;
					g.vNode[g.vRoad[minRoadID].ID2].vNeighborLength[i] = r.length;
					g.vNode[g.vRoad[minRoadID].ID2].sNeighborNode.erase(g.vRoad[minRoadID].ID1);
					g.vNode[g.vRoad[minRoadID].ID2].sNeighborRoad.erase(minRoadID);
					g.vNode[g.vRoad[minRoadID].ID2].sNeighborNode.insert(n.nodeID);
					g.vNode[g.vRoad[minRoadID].ID2].sNeighborRoad.insert(r.roadID);
					g.vNode[g.vRoad[minRoadID].ID2].mNeighborLength.erase(g.vRoad[minRoadID].ID1);
					g.vNode[g.vRoad[minRoadID].ID2].mSubNeighborRoad.erase(g.vRoad[minRoadID].ID1);
					g.vNode[g.vRoad[minRoadID].ID2].mRSubNeighborRoad.erase(g.vRoad[minRoadID].ID1);
					g.vNode[g.vRoad[minRoadID].ID2].mNeighborLength[n.nodeID] = r.length;
					g.vNode[g.vRoad[minRoadID].ID2].mSubNeighborRoad[n.nodeID] = r.roadID;
					g.vNode[g.vRoad[minRoadID].ID2].mRSubNeighborRoad[n.nodeID] = r.roadID;
				}
			}
*/
			//Update ID1's Info
//			for(i = 0; i < (int)g.vNode[g.vRoad[minRoadID].ID1].vNeighborNode.size(); i++)
//			{
//				if(g.vNode[g.vRoad[minRoadID].ID1].vNeighborNode[i] == g.vRoad[minRoadID].ID2)
//				{
//					g.vNode[g.vRoad[minRoadID].ID1].vNeighborNode[i] = n.nodeID;
//					g.vNode[g.vRoad[minRoadID].ID1].vNeighborLength[i] = g.vRoad[minRoadID].length;

/*					g.vNode[g.vRoad[minRoadID].ID1].sNeighborNode.erase(g.vRoad[minRoadID].ID2);
					g.vNode[g.vRoad[minRoadID].ID1].sNeighborNode.insert(n.nodeID);
					g.vNode[g.vRoad[minRoadID].ID1].mNeighborLength.erase(g.vRoad[minRoadID].ID2);
					g.vNode[g.vRoad[minRoadID].ID1].mSubNeighborRoad.erase(g.vRoad[minRoadID].ID2);
					g.vNode[g.vRoad[minRoadID].ID1].mRSubNeighborRoad.erase(g.vRoad[minRoadID].ID2);
					g.vNode[g.vRoad[minRoadID].ID1].mNeighborLength[n.nodeID] = g.vRoad[minRoadID].length;
					g.vNode[g.vRoad[minRoadID].ID1].mSubNeighborRoad[n.nodeID] = g.vRoad[minRoadID].roadID;
					g.vNode[g.vRoad[minRoadID].ID1].mRSubNeighborRoad[n.nodeID] = g.vRoad[minRoadID].roadID;
//				}
//			}
			n.mSubNeighborRoad[g.vRoad[minRoadID].ID1] = g.vRoad[minRoadID].roadID;
			n.mNeighborLength[g.vRoad[minRoadID].ID1] = g.vRoad[minRoadID].length;
			n.mRSubNeighborRoad[g.vRoad[minRoadID].ID1] = g.vRoad[minRoadID].roadID;

			n.mSubNeighborRoad[g.vRoad[minRoadID].ID2] = r.roadID;;
			n.mNeighborLength[g.vRoad[minRoadID].ID2] = r.length;
			n.mRSubNeighborRoad[g.vRoad[minRoadID].ID2] = r.roadID;;

		}
		else if(g.vRoad[minRoadID].direction == 2)
		{
			r.direction = g.vRoad[minRoadID].direction;
			g.vNode[n.nodeID].vNeighborNode.push_back(g.vRoad[minRoadID].ID2);
			g.vNode[n.nodeID].vNeighborRoad.push_back(r.roadID);
			g.vNode[n.nodeID].vNeighborLength.push_back(r.length);
			
			g.vNode[n.nodeID].vRNeighborNode.push_back(g.vRoad[minRoadID].ID1);
			g.vNode[n.nodeID].vRNeighborRoad.push_back(minRoadID);
			g.vNode[n.nodeID].vRNeighborLength.push_back(g.vRoad[minRoadID].length);
			
			//Update ID2's Info
			g.vNode[g.vRoad[minRoadID].ID2].sNeighborNode.erase(g.vRoad[minRoadID].ID1);
			g.vNode[g.vRoad[minRoadID].ID2].sNeighborRoad.erase(minRoadID);
			g.vNode[g.vRoad[minRoadID].ID2].sNeighborNode.insert(n.nodeID);
			g.vNode[g.vRoad[minRoadID].ID2].sNeighborRoad.insert(r.roadID);
			g.vNode[g.vRoad[minRoadID].ID2].mRSubNeighborRoad.erase(g.vRoad[minRoadID].ID1);
			g.vNode[g.vRoad[minRoadID].ID2].mRSubNeighborRoad[n.nodeID] = r.roadID;
	
			//Update ID1's Info
//			for(i = 0; i < (int)g.vNode[g.vRoad[minRoadID].ID1].vNeighborNode.size(); i++)
//			{
//				if(g.vNode[g.vRoad[minRoadID].ID1].vNeighborNode[i] == g.vRoad[minRoadID].ID2)
//				{
		//			g.vNode[g.vRoad[minRoadID].ID1].vNeighborNode[i] = n.nodeID;
		//			g.vNode[g.vRoad[minRoadID].ID1].vNeighborLength[i] = g.vRoad[minRoadID].length;
					g.vNode[g.vRoad[minRoadID].ID1].sNeighborNode.erase(g.vRoad[minRoadID].ID2);
					g.vNode[g.vRoad[minRoadID].ID1].sNeighborNode.insert(n.nodeID);
					g.vNode[g.vRoad[minRoadID].ID1].mNeighborLength.erase(g.vRoad[minRoadID].ID2);
					g.vNode[g.vRoad[minRoadID].ID1].mSubNeighborRoad.erase(g.vRoad[minRoadID].ID2);
					g.vNode[g.vRoad[minRoadID].ID1].mNeighborLength[n.nodeID] = g.vRoad[minRoadID].length;
					g.vNode[g.vRoad[minRoadID].ID1].mSubNeighborRoad[n.nodeID] = minRoadID;
//				}
//			}
			n.mSubNeighborRoad[g.vRoad[minRoadID].ID2] = r.roadID;;
			n.mNeighborLength[g.vRoad[minRoadID].ID2] = r.length;
			n.mRSubNeighborRoad[g.vRoad[minRoadID].ID1] = g.vRoad[minRoadID].roadID;
		}
		else if(g.vRoad[minRoadID].direction == 3)
		{
			r.direction = g.vRoad[minRoadID].direction;
			
			g.vNode[n.nodeID].vNeighborNode.push_back(g.vRoad[minRoadID].ID1);
			g.vNode[n.nodeID].vNeighborRoad.push_back(minRoadID);
			g.vNode[n.nodeID].vNeighborLength.push_back(g.vRoad[minRoadID].length);
			
			g.vNode[n.nodeID].vRNeighborNode.push_back(g.vRoad[minRoadID].ID2);
			g.vNode[n.nodeID].vRNeighborRoad.push_back(r.roadID);
			g.vNode[n.nodeID].vRNeighborLength.push_back(r.length);
			
			//Update ID2's Info
//			for(i = 0; i < (int)g.vNode[g.vRoad[minRoadID].ID2].vNeighborNode.size(); i++)
//			{
//				if(g.vNode[g.vRoad[minRoadID].ID2].vNeighborNode[i] == g.vRoad[minRoadID].ID1)
//				{
//					g.vNode[g.vRoad[minRoadID].ID2].vNeighborNode[i] = n.nodeID;
//					g.vNode[g.vRoad[minRoadID].ID2].vNeighborRoad[i] = r.roadID;
//					g.vNode[g.vRoad[minRoadID].ID2].vNeighborLength[i] = r.length;
					g.vNode[g.vRoad[minRoadID].ID2].sNeighborNode.erase(g.vRoad[minRoadID].ID1);
					g.vNode[g.vRoad[minRoadID].ID2].sNeighborRoad.erase(minRoadID);
					g.vNode[g.vRoad[minRoadID].ID2].sNeighborNode.insert(n.nodeID);
					g.vNode[g.vRoad[minRoadID].ID2].sNeighborRoad.insert(r.roadID);
					g.vNode[g.vRoad[minRoadID].ID2].mNeighborLength.erase(g.vRoad[minRoadID].ID1);
					g.vNode[g.vRoad[minRoadID].ID2].mSubNeighborRoad.erase(g.vRoad[minRoadID].ID1);
					g.vNode[g.vRoad[minRoadID].ID2].mNeighborLength[n.nodeID] = r.length;
					g.vNode[g.vRoad[minRoadID].ID2].mSubNeighborRoad[n.nodeID] = r.roadID;
//				}
//			}
			//Update ID1's Info
//			g.vNode[g.vRoad[minRoadID].ID1].vNeighborNode[i] = n.nodeID;
//			g.vNode[g.vRoad[minRoadID].ID1].vNeighborLength[i] = g.vRoad[minRoadID].length;
//			g.vNode[g.vRoad[minRoadID].ID1].sNeighborNode.erase(g.vRoad[minRoadID].ID2);
//			g.vNode[g.vRoad[minRoadID].ID1].sNeighborNode.insert(n.nodeID);
			g.vNode[g.vRoad[minRoadID].ID1].mRSubNeighborRoad.erase(g.vRoad[minRoadID].ID2);
			g.vNode[g.vRoad[minRoadID].ID1].mRSubNeighborRoad[n.nodeID] = r.roadID;
			
			n.mSubNeighborRoad[g.vRoad[minRoadID].ID1] = g.vRoad[minRoadID].roadID;
			n.mNeighborLength[g.vRoad[minRoadID].ID1] = g.vRoad[minRoadID].length;
			n.mRSubNeighborRoad[g.vRoad[minRoadID].ID2] = r.roadID;;
		}
		r.sNeighborRoad.erase(r.roadID);
//		r.ID1 = n.nodeID;
//		r.ID2 = g.vRoad[minRoadID].ID2;
		g.vRoad.push_back(r);
		g.vNode.push_back(n);
		
		g.vRoad[minRoadID].ID2 = n.nodeID;

	}

	//Refine
	vector<node>::iterator ivNode;
	vector<int>::iterator ivNR;
	map<int, int>::iterator	imRSN, imSN;
/*	cout << "Refining" << endl;
	for(ivNode = g.vNode.begin(); ivNode != g.vNode.end(); ivNode++)
	{
		for(imRSN = (*ivNode).mRSubNeighborRoad.begin(); imRSN != (*ivNode).mRSubNeighborRoad.end(); imRSN++)
		{
			if(g.vNode[(*imRSN).first].mSubNeighborRoad.find((*ivNode).nodeID) == g.vNode[(*imRSN).first].mSubNeighborRoad.end())
			{
				g.vNode[(*imRSN).first].vNeighborNode.push_back((*ivNode).nodeID);
				g.vNode[(*imRSN).first].vNeighborRoad.push_back((*imRSN).second);
				g.vNode[(*imRSN).first].vNeighborLength.push_back(g.vRoad[(*imRSN).second].length);
				g.vNode[(*imRSN).first].mSubNeighborRoad.insert(make_pair((*ivNode).nodeID,(*imRSN).second));
				g.vNode[(*imRSN).first].mNeighborLength.insert(make_pair((*ivNode).nodeID,g.vRoad[(*imRSN).second].length));
			}
		}
	}

	for(ivNode = g.vNode.begin(); ivNode != g.vNode.end(); ivNode++)
	{
		for(imSN = (*ivNode).mSubNeighborRoad.begin(); imSN != (*ivNode).mSubNeighborRoad.end(); imSN++)
		{
			if(g.vNode[(*imSN).first].mRSubNeighborRoad.find((*ivNode).nodeID) == g.vNode[(*imSN).first].mRSubNeighborRoad.end())
			{
				g.vNode[(*imSN).first].mRSubNeighborRoad.insert(make_pair((*ivNode).nodeID,(*imSN).second));
			}
		}
	}
*/

	return 0;
}

int RoadNetwork::writeSpecialNode()
{
	cout << "Writing Speical Node File:" << conf->specialNodeFile << endl;
	vector<node>::iterator ivNode;
	map<int, int>::iterator imSNR, imRSNR;
	map<int, float>::iterator imNL;
	
	ofstream ofNode(conf->specialNodeFile.c_str());
	ofNode << g.vNode.size() << "\t" << minX << "\t" << maxX << "\t" << minY << "\t" << maxY << endl;
	for(ivNode = g.vNode.begin(); ivNode != g.vNode.end(); ivNode++)
	{
		ofNode << setprecision(15) << (*ivNode).nodeID << "\t" << (*ivNode).type << "\t" << (*ivNode).x << "\t" << (*ivNode).y << "\t";
		if((*ivNode).isolated)
			ofNode << 1 << "\t";
		else
			ofNode << 0 << "\t";
		
		if((*ivNode).bSpecial)
			ofNode << 1 << "\t";
		else
			ofNode << 0 << "\t";

		ofNode << (*ivNode).mNeighborLength.size() << "\t";
		for(imNL = (*ivNode).mNeighborLength.begin(); imNL != (*ivNode).mNeighborLength.end(); imNL++)
			ofNode << (*imNL).first << "\t" << (*imNL).second << "\t";

		ofNode << (*ivNode).mSubNeighborRoad.size() << "\t";
		for(imSNR = (*ivNode).mSubNeighborRoad.begin(); imSNR != (*ivNode).mSubNeighborRoad.end(); imSNR++)
			ofNode << (*imSNR).first << "\t" << (*imSNR).second << "\t";

		ofNode << (*ivNode).mRSubNeighborRoad.size() << "\t";
		for(imRSNR = (*ivNode).mRSubNeighborRoad.begin(); imRSNR != (*ivNode).mRSubNeighborRoad.end(); imRSNR++)
			ofNode << (*imRSNR).first << "\t" << (*imRSNR).second << "\t";
		ofNode << endl;
	}

	ofNode.close();
	return 0;
}

int RoadNetwork::writeSpecialRoad()
{
	cout << "Writing Special Road File:" << conf->specialRoadFile << endl;
	vector<road>::iterator ivRoad;
	set<int>::iterator isNR;

	ofstream ofRoad((conf->specialRoadFile).c_str());
	ofRoad << g.vRoad.size() << endl;
	for(ivRoad = g.vRoad.begin(); ivRoad != g.vRoad.end(); ivRoad++)
	{
		ofRoad << (*ivRoad).roadID << "\t" << (*ivRoad).direction << "\t" << (*ivRoad).length << "\t" << (*ivRoad).ID1 << "\t" << (*ivRoad).ID2 << "\t";
		if((*ivRoad).isolated)
			ofRoad << 1 << "\t";
		else
			ofRoad << 0 << "\t";

		ofRoad << (*ivRoad).sNeighborRoad.size() << "\t";
		for(isNR = (*ivRoad).sNeighborRoad.begin(); isNR != (*ivRoad).sNeighborRoad.end(); isNR++)
			ofRoad << *isNR << "\t";

		ofRoad << endl;
	}
	ofRoad.close();

	ofstream ofRoadSM((conf->specialRoadFile+"Map").c_str());
	map<double, int>::iterator imRoadIDSpecial;
	for(imRoadIDSpecial = mRoadIDSpecial.begin(); imRoadIDSpecial != mRoadIDSpecial.end(); imRoadIDSpecial++)
	{
		ofRoadSM << setprecision(15) << (*imRoadIDSpecial).first << "\t" << (*imRoadIDSpecial).second << endl;
	}
	ofRoadSM.close();

	return 0;
}

int RoadNetwork::readSpecialNode()
{
	ifstream inNodeFile(conf->specialNodeFile.c_str());
	if(!inNodeFile)
	{
		cout << "Cannot open speical node file" << conf->specialNodeFile << endl;
		return -1;
	}
	cout << "Reading Speical Node file: " << conf->specialNodeFile << endl;

	int nodeNum;
	inNodeFile >> nodeNum >> minX >> maxX >> minY >> maxY;
	cout << "Node number: " << nodeNum << endl;
	node ni;
	g.vNode.assign(nodeNum, ni);

	int i,j,k,i1,i2;
	float f;
	for(i = 0; i < nodeNum; i++)
	{
		node n;
		inNodeFile >> n.nodeID >> n.type >> n.x >> n.y >> j;
//		if(j)
//			n.isolated = true;
//		else
			n.isolated = false;
		
		inNodeFile >> j;
		if(j)
			n.bSpecial = true;
		else
			n.bSpecial = false;

		inNodeFile >> j;
		for(k = 0; k < j; k++)
		{
			inNodeFile >> i1 >> f;
			n.mNeighborLength[i1] = f;
			n.vNeighborNode.push_back(i1);
			n.vNeighborLength.push_back(f);
			n.sNeighborNode.insert(i1);
		}
		
		inNodeFile >> j;
		for(k = 0; k < j; k++)
		{
			inNodeFile >> i1 >> i2;
			n.mSubNeighborRoad[i1] = i2;
			n.vNeighborRoad.push_back(i2);
			n.sNeighborRoad.insert(i2);
		}
		
		inNodeFile >> j;
		for(k = 0; k < j; k++)
		{
			inNodeFile >> i1 >> i2;
			n.mRSubNeighborRoad[i1] = i2;
//			n.sNeighborNode.insert(i1);
//			n.sNeighborRoad.insert(i2);
//			n.vRNeighborNode.push_back(i1);
//			n.vRNeighborRoad.push_back(i2);
		}
		g.vNode[i] = n;
	}
	inNodeFile.close();

	cout << "Organizing Reverse"  << endl;
	for(int i = 0; i < nodeNum; i++)
	{
		for(int j = 0; j < (int)g.vNode[i].vNeighborNode.size(); j++) 
		{
			int neighborNodeID = g.vNode[i].vNeighborNode[j];
			int neighborRoadID = g.vNode[i].vNeighborRoad[j];
			int neighborLength = g.vNode[i].vNeighborLength[j];
			g.vNode[neighborNodeID].vRNeighborNode.push_back(i);
			g.vNode[neighborNodeID].vRNeighborRoad.push_back(neighborRoadID);
			g.vNode[neighborNodeID].vRNeighborLength.push_back(neighborLength); 
		}
	}
/*	ifstream inISO(conf->isoNodeFile.c_str());
	cout << "Reading iso node file:" << conf->isoNodeFile << endl;
	int id;
	while(inISO >> id)
	{
		g.vNode[id].isolated = true;
	}
	inISO.close();
*/	
	return 0;
}

int RoadNetwork::readSpecialRoad()
{
	ifstream inRoadFile(conf->specialRoadFile.c_str());
	if(!inRoadFile)
	{
		cout << "Cannot open special road file" << conf->specialRoadFile << endl;
		return -1;
	}
	cout << "Reading Speical Road File: " << conf->specialRoadFile << endl;

	int roadNum;
	inRoadFile >> roadNum;
	cout << "Road Number:" << roadNum << endl;
	road ri;
	g.vRoad.assign(roadNum, ri);

	int i,j,k,p;
	for(i = 0; i < roadNum; i++)
	{
		road r;
		inRoadFile >> r.roadID >> r.direction >> r.length >> r.ID1 >> r.ID2 >> j;
		if(j)
			r.isolated = true;
		else
			r.isolated = false;

		inRoadFile >> j;
		for(k = 0; k < j; k++)
		{
			inRoadFile >> p;
			r.sNeighborRoad.insert(p);
		}
		g.vRoad[i] = r;
	}
	inRoadFile.close();

	ifstream ifRoadMap((conf->roadFile+"Map").c_str());
	if(!ifRoadMap)
	{
		cout << "Cannot open road map file" << conf->roadFile+"Map" << endl;
		return -1;
	}
	cout << "Reading Road Map File " << conf->roadFile+"Map" << endl;
	double oldID;
	int newID;
	int count = 0;
	while(ifRoadMap >> oldID >> newID)
	{
		mRoadIDTrans[oldID] = newID;
		mRRoadIDTrans[newID] = oldID;
		count++;
	}
	cout << count <<endl;
	ifRoadMap.close();

	ifstream ifRoadSpecialMap("../data/beijing/special/beijingParkingRoadOriginalMap");
	cout << "Reading Speical Road Map File " << conf->roadFile+"Map" << endl;
	while(ifRoadMap >> oldID >> newID)
	{
		mRoadIDTrans[oldID] = newID;
		mRRoadIDTrans[newID] = oldID;
		count++;
	}
	cout << count <<endl;
	ifRoadMap.close();

	vector<node>::iterator ivNode;
	vector<int>::iterator ivRNR;
	for(ivNode = g.vNode.begin(); ivNode != g.vNode.end(); ivNode++)
		for(ivRNR = (*ivNode).vRNeighborRoad.begin(); ivRNR != (*ivNode).vRNeighborRoad.end(); ivRNR++)
			(*ivNode).vRNeighborLength.push_back(g.vRoad[(*ivRNR)].length);

	return 0;
}
