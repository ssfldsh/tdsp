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
	cout<<r->g.vNode[545509].nodeID<<" "<<r->g.vNode[545509].x<<" "<<r->g.vNode[545509].y<<endl;
	cout<<r->g.vNode[74993605].nodeID<<" "<<r->g.vNode[74993605].x<<" "<<r->g.vNode[74993605].y<<endl;
	cout<<r->g.vRoad[88740116].roadID<<" "<<r->g.vRoad[88740116].ID1<<" "<<r->g.vRoad[88740116].ID2<<endl;
	cout<<r->g.vNode.size()<<endl;
	cout<<r->g.vRoad.size()<<endl;
	//test speed
	cout<<r->g.vRoad[614567].length<<endl;
	cout<<r->g.vRoad[-615858].length<<endl;
	//test every road start end length neighbor
	r->shortestPathDijkstraStatic(288965,20000670988);
	r->shortestPathDijkstraStatic(288965,20000671007);
	r->shortestPathDijkstraStatic(309504,20000671007);
	r->shortestPathDijkstraStatic(302230,530223);
	return 0;
}
