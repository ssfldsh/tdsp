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
	
	}
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
		cout << "Reading Speed Profile:" << conf->costFile << endl;
	
		double cost;
		int num, i, t;
		int roadID;
		//read each line
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
			//LPFunction is vX[], vY[], minY,maxY, upperbound=last read De t, input file Li each line's max t
			LPFunction f = LPFunction(g.vRoad[roadID].ID1, g.vRoad[roadID].ID2, t, vX, vY);  
			g.vRoad[roadID].costFunction = f;
		}
		ifSP.close();
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


