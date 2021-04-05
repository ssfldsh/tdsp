#include "road.h"

int main(int argc, char* argv[])
{
	//the first sentence reading file to construct the graph maily vRoad vector 
	RoadNetwork* r = new RoadNetwork(argv);
	vector<road>::iterator ivRoad;
	//ivRoad xunHuai zhongDe Road struct duixiang
	for(ivRoad = r->g.vRoad.begin(); ivRoad != r->g.vRoad.end(); ivRoad++)
	{
		cout << (*ivRoad).roadID << " Cost Fucntion:" << endl;
		(*ivRoad).costFunction.display();
	}


	/*t2hopForward
	T2Hop t2hop = T2Hop(r->g.vNode);//no use data of vNode, only size used
	t2hop.forwardSearch(1, r->g);
	t2hop.backwardSearch(1, r->g);
	t2hop.forwardSearch(2, r->g);
	t2hop.backwardSearch(2, r->g);
	t2hop.forwardSearch(3, r->g);
	t2hop.backwardSearch(3, r->g);
	t2hop.forwardSearch(4, r->g);
	t2hop.backwardSearch(4, r->g);
	t2hop.forwardSearch(5, r->g);
	t2hop.backwardSearch(5, r->g);
	t2hop.forwardSearch(6, r->g);
	t2hop.backwardSearch(6, r->g);
	t2hop.forwardSearch(7, r->g);
	t2hop.backwardSearch(7, r->g);
	t2hop.forwardSearch(8, r->g);
	t2hop.backwardSearch(8, r->g);
	*/	
	//one day time 300 part 
	//input a roadid a start time , output a cost

	//static road lpfY use t=0
	//cout<<r->g.vNode.size();

	clock_t start, stop;

	int ID1=0;
	int ID2=0;
	int startTime=0;

	/*cout<<"static dijistra at time="<<startTime<<endl;
	start=clock();
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			r->shortestPathDijkstraStatic(i,j,startTime);
		}
	}
	stop=clock();
	cout<<"running time of static dijkstra is"<<1000*((float)(stop-start)/CLOCKS_PER_SEC)<<"ms"<<endl<<endl;
	*/

	cout<<"time dependent dijistra at time="<<startTime<<endl;
	start=clock();
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			r->shortestPathDijkstraTimeDependent(i,j,startTime);
		}
	}
	stop=clock();
	cout<<"running time of time dependent dijkstra is"<<1000*((float)(stop-start)/CLOCKS_PER_SEC)<<"ms"<<endl<<endl;

	
	/*
	T2Hop t2hop = T2Hop(r->g.vNode);
	t2hop.forwardSearch(1, r->g);
	t2hop.backwardSearch(1, r->g);
	t2hop.forwardSearch(2, r->g);
	t2hop.backwardSearch(2, r->g);
	t2hop.forwardSearch(3, r->g);
	t2hop.backwardSearch(3, r->g);
	t2hop.forwardSearch(4, r->g);
	t2hop.backwardSearch(4, r->g);
	t2hop.forwardSearch(5, r->g);
	t2hop.backwardSearch(5, r->g);
	t2hop.forwardSearch(6, r->g);
	t2hop.backwardSearch(6, r->g);
	t2hop.forwardSearch(7, r->g);
	t2hop.backwardSearch(7, r->g);
	t2hop.forwardSearch(8, r->g);
	t2hop.backwardSearch(8, r->g);
	*/

	return 0;
}
