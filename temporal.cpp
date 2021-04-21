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
	
	map<long int,road>::iterator it;
	for(it=g.vRoad.begin();it!=g.vRoad.end();it++){
		//map iterator use
		(*it).second.length= 10;
	}
	if(!conf->bSPNew)//exe
	{
		ifstream ifSP((conf->costFile).c_str());
		if(!ifSP)
		{
			cout << "Cannot Find " << conf->costFile << endl;
			return -1;
		}
		cout << "Reading Speed Profile:" << conf->costFile << endl;
	
		float cost;
		int num, t;
		long int roadID;
		//read each line
		while(ifSP >> roadID)
		{
			ifSP >> num;
			ifSP >> t >> cost; 
			//
			g.vRoad[roadID].length = cost;
		}
		ifSP.close();
	}
	cout << "Speed Profile loading finish" << endl;
	return 0;
}


