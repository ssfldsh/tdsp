#include "road.h"

int main(int argc, char* argv[])
{
	//the first sentence reading file to construct the graph maily vRoad vector 
	RoadNetwork* r = new RoadNetwork(argv);
	// vector<road>::iterator ivRoad;
	// //ivRoad xunHuai zhongDe Road struct duixiang
	// for(ivRoad = r->g.vRoad.begin(); ivRoad != r->g.vRoad.end(); ivRoad++)
	// {
	// 	cout << (*ivRoad).roadID << " Cost Fucntion:" << endl;
	// 	(*ivRoad).costFunction.display();
	// }

	return 0;
}
