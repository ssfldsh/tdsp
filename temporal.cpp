#include "road.h"
#include "tools.h"

void RoadNetwork::readSpeedProfile()
{
	if(conf->hasNewSP)//exe
	{
		readNewSpeedProfile();
	}
	else//noexe
	{
		readOriginalSpeedProfile();
		fillVoidCost();
		writeNewSpeedProfile();
	}
}

int	RoadNetwork::readOriginalSpeedProfile()
{
	readSpecialRoadMap();
	ifstream ifSP((conf->orgCostFile).c_str());
	if(!ifSP)
	{
		cout << "Cannot Find " << conf->orgCostFile << endl;
		return -1;
	}
	cout << "Reading Original Speed Profile:" << conf->orgCostFile << endl;

	vector<int> vTmp;
	for(auto ivRoad = g.vRoad.begin(); ivRoad != g.vRoad.end(); ivRoad++)
	{
		(*ivRoad).vCostC = vTmp;
		(*ivRoad).vCostT = vTmp;
	}
	
	double orgRoadID, cost;
	int num, i, ts;
	int roadID, sRoadID;
	int count = 0;
	while(ifSP >> orgRoadID)
	{
		if(mRoadIDTrans.find(orgRoadID) != mRoadIDTrans.end())
			roadID = mRoadIDTrans[orgRoadID];
		else
		{
			ifSP >> num;
			count++;
			continue;
		}

		if(mRoadIDSpecial.find(orgRoadID) != mRoadIDSpecial.end())
			sRoadID = mRoadIDSpecial[orgRoadID];
		else
			sRoadID = -1;

		ifSP >> num;
		if(num == 0)
		{
			vector<int> vTmp;
			if(sRoadID == -1)
			{
				g.vRoad[roadID].vCostT = vTmp;
				g.vRoad[roadID].vCostC = vTmp;
			}
			else if(sRoadID != -1)
			{
				g.vRoad[sRoadID].vCostT = vTmp;
				g.vRoad[sRoadID].vCostC = vTmp;
			}
			count++;
			continue;
		}
		for(i = 0; i < num; i++)
		{
			ifSP >> ts >> cost;
			if((int)cost == 0) 
				cout << orgRoadID << endl;
			if(sRoadID == -1)
			{
				g.vRoad[roadID].vCostT.push_back(300 * ts);
				g.vRoad[roadID].vCostC.push_back(cost);
			}
			else if(sRoadID != -1)
			{
				g.vRoad[sRoadID].vCostT.push_back(300 * ts);
				g.vRoad[sRoadID].vCostC.push_back(cost);
			}
		}
	}
	ifSP.close();

	cout << "No SP Road Number:" << count << endl;


	return -1;
}

void RoadNetwork::fillVoidCost()
{
	vector<road>::iterator ivRoad;
	set<int>::iterator	isNR;
	vector<int> vUnfilled;
	vector<int>::iterator ivU;
	for(ivRoad = g.vRoad.begin(); ivRoad != g.vRoad.end(); ivRoad++)
	{
/*		if((*ivRoad).vCostT.size() == 0)
		{
			for(isNR = (*ivRoad).sNeighborRoad.begin(); isNR != (*ivRoad).sNeighborRoad.end(); isNR++)
			{
				if(g.vRoad[*isNR].vCostT.size() != 0)
				{
					(*ivRoad).vCostT = g.vRoad[*isNR].vCostT;
					(*ivRoad).vCostC = g.vRoad[*isNR].vCostC;
					break;
				}
			}
		}*/
		if((*ivRoad).vCostT.size() == 0)
			vUnfilled.push_back((*ivRoad).roadID);
	}

///	for(ivU = vUnfilled.begin(); ivU != vUnfilled.end();ivU++)
//		cout << *ivU <<endl;

	bool bErase = false;
	int lastSize = 0;
	while(!vUnfilled.empty())
	{
		if(lastSize == (int)vUnfilled.size())
		{
			cout << "Brute Filling" << endl;
			for(ivU = vUnfilled.begin(); ivU != vUnfilled.end();)
			{
				int min = INF;

				vector<road>::iterator ivRoad2;
				for(ivRoad = g.vRoad.begin(); ivRoad != g.vRoad.end(); ivRoad++) 
				{
					if((*ivRoad).vCostT.size() == 0)
						continue;
					int tmp = abs((*ivRoad).length - g.vRoad[*ivU].length);
					if(tmp < 2)
					{
						ivRoad2 = ivRoad;
						break;
					}
					else if(tmp < min)
					{
						min = tmp;
						ivRoad2 = ivRoad;
					}

				}
				g.vRoad[*ivU].vCostT = (*ivRoad2).vCostT;
				g.vRoad[*ivU].vCostC = (*ivRoad2).vCostC;
				for(int i = 0; i < (int)g.vRoad[*ivU].vCostC.size(); i++)
					if(g.vRoad[(*ivU)].vCostC[i] < 0)
						cout << "A:" << g.vRoad[(*ivU)].vCostC[i] << endl;
				ivU = vUnfilled.erase(ivU);
		//		bErase = true;
			}
		//	break;
		}
		cout << "vUnfilled Size:" << vUnfilled.size() << endl;
		lastSize = vUnfilled.size();
		for(ivU = vUnfilled.begin(); ivU != vUnfilled.end();ivU++)
			cout << *ivU <<"\t" << g.vRoad[*ivU].vCostT.size() <<endl;
		for(ivU = vUnfilled.begin(); ivU != vUnfilled.end();)
		{
			bErase = false;
			if(*ivU == 2159)
				cout << "Filling 2159 " << g.vRoad[2159].vCostT.size() << endl;
			for(isNR = g.vRoad[*ivU].sNeighborRoad.begin(); isNR != g.vRoad[*ivU].sNeighborRoad.end(); isNR++)
			{
				if(g.vRoad[*isNR].vCostT.size() != 0)
				{
					if(*isNR == 2399)
						cout << g.vRoad[2399].vCostT.size() << endl;

					if(*ivU == 2159)
					{
						cout <<"Neighbor:" << *isNR <<endl;
						for(int i = 0; i < (int)g.vRoad[*isNR].vCostC.size(); i++)
							cout << g.vRoad[*isNR].vCostT[i] << "," << g.vRoad[*isNR].vCostC[i] <<"\t";
					}


					g.vRoad[*ivU].vCostT = g.vRoad[*isNR].vCostT;
//					g.vRoad[*ivU].vCostC = g.vRoad[*isNR].vCostC;
					for(int i = 0; i < (int)g.vRoad[*ivU].vCostT.size(); i++)
					{
						g.vRoad[*ivU].vCostC.push_back(((double)g.vRoad[*ivU].length / (double)g.vRoad[*isNR].length) * g.vRoad[*isNR].vCostC[i]);
						if(*ivU == 2159)
							cout << (double)g.vRoad[*ivU].length << "\t" << (double)g.vRoad[*isNR].length << "\t" << g.vRoad[*isNR].vCostC[i] << "\t" << ((double)g.vRoad[*ivU].length / (double)g.vRoad[*isNR].length) * g.vRoad[*isNR].vCostC[i] << endl;
					}

					bool bError = false;
					for(int i = 0; i < (int)g.vRoad[*ivU].vCostC.size(); i++)
						if(g.vRoad[(*ivU)].vCostC[i] < 0)
						{
							cout << "B:" << *ivU << "\t" << i << "\t" << g.vRoad[*isNR].vCostC[i] << "\t" << g.vRoad[(*ivU)].vCostC[i] << "\t" << *isNR << "\t" << mRRoadIDTrans[*isNR] <<"\t" << mRoadIDTrans[mRRoadIDTrans[*isNR]] << endl;
							bError = true;
							break;
						}
					if(bError)
					{
						for(int i = 0; i < (int)g.vRoad[*isNR].vCostC.size(); i++)
							cout << g.vRoad[*isNR].vCostT[i] << "," << g.vRoad[*isNR].vCostC[i] <<"\t";
						cout << endl;
					}


					ivU = vUnfilled.erase(ivU);
					bErase = true;
					break;
				}
			}
		
			if(!bErase)
				ivU++;
		}
	}
}

int RoadNetwork::readSpecialRoadMap()
{
	ifstream ifSRM((conf->specialRoadFile+"Map").c_str());
	if(!ifSRM)
	{
		cout << "Cannot find " << conf->specialRoadFile+"Map" << endl;
		return -1;
	}
	cout << "Reading " << conf->specialRoadFile+"Map" << endl;
	
	double orgID;
	int newID;
	while(ifSRM >> orgID >> newID)
		mRoadIDSpecial[orgID] = newID;

	ifSRM.close();
	return 0;
}
	
void RoadNetwork::writeNewSpeedProfile()
{
	ofstream ofNSP((conf->costFile).c_str());
	cout << "Writing New Speed Profile:" << conf->costFile << endl;
	vector<road>::iterator ivRoad;
	int i;
	for(ivRoad = g.vRoad.begin(); ivRoad != g.vRoad.end(); ivRoad++)
	{
		ofNSP << (*ivRoad).roadID << "\t" << (*ivRoad).vCostT.size();
		for(i = 0; i < (int)(*ivRoad).vCostT.size(); i++) 
		{
			ofNSP << "\t" << (*ivRoad).vCostT[i] << "\t" << (*ivRoad).vCostC[i]; 
			if((*ivRoad).vCostC[i] < 0)
				cout << (*ivRoad).vCostC[i] << endl;
		}
		ofNSP << endl;
	}
	ofNSP.close();
}

int RoadNetwork::readNewSpeedProfile()
{
	//Read speed profile
	//Prune redundant with lpf
	//Write back New SP
	if(!conf->bSPNew)//exe
	{
		ifstream ifSP((conf->costFile).c_str());
		if(!ifSP)
		{
			cout << "Cannot Find " << conf->costFile << endl;
			return -1;
		}
		cout << "Reading Speed Profile Old:" << conf->costFile << endl;
	
		double cost;
		int num, i, t;
		int roadID;
		while(ifSP >> roadID)
		{
			vector<int> vX, vY;
			ifSP >> num;
			vX.assign(num, 0);
			vY.assign(num, 0);
			for(i = 0; i < num; i++)
			{
				ifSP >> t >> cost;  
				vX[i] = t;    
				vY[i] = cost;

				//For old algorithms, to be removed
//				g.vRoad[roadID].vCostT.push_back(t);
//				g.vRoad[roadID].vCostC.push_back(cost);
			}
			LPFunction f = LPFunction(g.vRoad[roadID].ID1, g.vRoad[roadID].ID2, t, vX, vY);  
			g.vRoad[roadID].costFunction = f;
		}
		ifSP.close();

		cout << "Writing Speed Profile New:" << conf->costFileNew << endl;
		ofstream of(conf->costFileNew.c_str());
		vector<road>::iterator ivRoad;
		vector<int>::iterator ivX, ivY;
		for(ivRoad = g.vRoad.begin(); ivRoad != g.vRoad.end(); ivRoad++)
		{
			of << (*ivRoad).roadID << "\t" << (*ivRoad).costFunction.getSize();
			vector<int> vX = (*ivRoad).costFunction.returnVX();
			vector<int> vY = (*ivRoad).costFunction.returnVY();
			for(ivX = vX.begin(), ivY = vY.begin(); ivX != vX.end(); ivX++, ivY++)
				of << "\t" << *ivX << "\t" << *ivY;
			of << endl;
		}
		of.close();
	}
	else
	{
		ifstream ifSP((conf->costFileNew).c_str());
		if(!ifSP)
		{
			cout << "Cannot Find " << conf->costFile << endl;
			return -1;
		}
		cout << "Reading Speed Profile New:" << conf->costFileNew << endl;
	
		double cost;
		int num, i, t;
		int roadID;
		int minY = INF;
		int maxY = -1;
		while(ifSP >> roadID)
		{
			vector<int> vX, vY;
			ifSP >> num;
			vX.assign(num, 0);
			vY.assign(num, 0);
			for(i = 0; i < num; i++)
			{
				ifSP >> t >> cost;  
				vX[i] = t;    
				vY[i] = cost;
				if(cost < minY)
					minY = cost;
				if(cost > maxY)
					maxY = cost;
			}
			//Difference lies here
//			LPFunction f = LPFunction(vX, vY, g.vRoad[roadID].ID1, g.vRoad[roadID].ID2, t, maxY, minY);  
			LPFunction f = LPFunction(vX, vY, g.vRoad[roadID].ID1, g.vRoad[roadID].ID2, 86400, maxY, minY);  
			g.vRoad[roadID].costFunction = f;
		}
		ifSP.close(); 

		for(int i = 0; i < (int)g.vRoad.size(); i++)
			g.vRoad[i].costFunction.calMM();
	}

	cout << "Speed Profile loading finish" << endl;

	return 0;
}


