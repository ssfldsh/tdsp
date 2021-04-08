#ifndef CONF_H
#define CONF_H

#include "tools.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class Configuration
{
public:
	Configuration();
	
	int		readConfiguration();
	bool	findNewGraphFile();

	string	city;
	string	dataFolder;			//base data folder
	string  mapFile;
	string	nodeFile;
	string	roadFile;
	string  isoNodeFile;
	string  isoRoadFile;
	string	nodeMapFile;
	bool	bNewGraphFile;
	
	bool	bSpecialNode;
	bool	bBuild;
	string	specialNodeType;	
	string	speicalName;
	string  orgSpecialNodeFile;
	string  specialNodeFile;	//special node file location
	string  specialRoadFile;	//special edge file location

	string	graphType;

	vector<string>	vTestAlgorithm;
	string	testInputFile;
	string	testInputFileName;
	bool	bTest;

	/* Section stores the index information
	 * Currently only support Quad Tree
	 */
	bool	bIndex;
	string	sIndex;
	bool	bQuadTree;	//Has QuadTree File
	string  quadTreeFile;
	int		quadTreeMaxLevel;
	string  sPUNCHDir;
	string  sPUNCHName;
	bool	bPathIndex;
	string  sPathIndex;
	string	CHFile;
	string	NodeOrderFile;
	
	bool	bTemporal;
	bool	hasNewSP;
	string  orgCostFile;
	string	costFile;
	bool	bSPNew;		//bool of costFileNew
	string	costFileNew;//use lpf directly
	string  spDate;
	void	findSPFile();
	bool	bPrune;
	string  pruneType;
	
	bool	bGrid;
	bool	bRawGridFile;
	bool	bGridFile;
	bool	bT2HopGridInner;
	bool	bT2HopGridBoundary;
	bool	bT2HopGridBoundaryAllPair;
	bool	bT2HopGridBoundaryOrder; 
	string  sGridFolder;
	string  sT2HopGridBoundaryAllPairFolder;//store the all pair info
	string  sT2HopGridBoundaryAllPairCompressFolder;
	string  sT2HopGridBoundaryAllPairFilename;
	string  gridType;
	string	sRawGridPath;
	string	sGridPath;
	string	sT2HopGridInnerPath; 
	string	sT2HopGridInner;
	string	sT2HopGridPathBoundary;//store the pruned Boundary T2Hop
	string	sT2HopGridPathBoundaryCompress;
	string	sT2HopGridPathBoundaryCompressCombine;
	string	sT2HopGridPathInnerCompress;
	string	sT2HopGridPathBoundaryNodeOrder;
	string  sT2HopGridPathBoundaryHopFolder;
	string  sT2HopGridInnerTestFolder;
	string  GTCHID;
	void	findGridFile();
	
	bool	bCompress;
	string  sT2HopGridBoundaryCompressFilenameBase;

	vector<string> vsTestArgs;

	bool	bAlpha;
	double	alpha;
	string	sAlpha;
//	string  sT2HopGridBoundaryCompressFilenameBase;

private: 
	void confInit();
};


#endif
