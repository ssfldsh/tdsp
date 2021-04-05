CXX=g++ -std=c++11 -fPIC
OPT=-O0

RoadNetwork: linearPiecewiseFunction.o buildGraph.o TimeDependent2Hop.o temporal.o road.o conf.o tools.o test2hop.o -lpthread -lboost_system -lboost_thread 
	$(CXX) -g -o test2hop test2hop.o buildGraph.o linearPiecewiseFunction.o temporal.o TimeDependent2Hop.o road.o conf.o tools.o -lpthread -lboost_thread -lboost_system
	

conf.o:conf.cpp
	$(CXX) -g -c $(OPT) conf.cpp
road.o:road.cpp 
	$(CXX) -g -c $(OPT) road.cpp 
buildGraph.o:buildGraph.cpp
	$(CXX) -g -c $(OPT) buildGraph.cpp
tools.o:tools.cpp
	$(CXX) -g -c $(OPT) tools.cpp
linearPiecewiseFunction.o:linearPiecewiseFunction.cpp
	$(CXX) -g -c $(OPT) linearPiecewiseFunction.cpp
TimeDependent2Hop.o:TimeDependent2Hop.cpp
	$(CXX) -g -c TimeDependent2Hop.cpp
test2hop.o:test2hop.cpp
	$(CXX) -g -c test2hop.cpp
temporal.o:temporal.cpp
	$(CXX) -g -c temporal.cpp



clean:
	rm *.o
	rm test2hop
