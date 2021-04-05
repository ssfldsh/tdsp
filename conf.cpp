#include "conf.h"

using namespace std;

//Configuration Class Read Conf File in Lines
Configuration::Configuration()
{
	ifstream inCity("City");
	if(!inCity)
	{
		cout << "Cannot Find City File" << endl;
	}
	inCity >> city;
	readConfiguration();
}

int Configuration::readConfiguration()
{
	string fileName = "configure_" + city;
	ifstream inConf(fileName.c_str());
	if(!inConf)
	{
		cout << "Cannot Find File \"" + fileName + "\"" << endl;
		return -1;
	}

	confInit();

	string s;
	vector<string> vs;
	while(getline(inConf, s))
	{
		//skip all the comments and disabled configurations
		if(s[0] == '#' || s[0] == '\n')
			continue;
		else
		{
			cout << s << endl;
			vs = split(s, "\t");
			if("dataPath" == vs[0])
			{
				dataFolder = vs[1] + city + "/";
				findNewGraphFile();
				if(!bNewGraphFile)//no execute
				{
					nodeFile = dataFolder + city + "Node";
					roadFile = dataFolder + city + "Road";
					nodeMapFile = dataFolder + city + "NodeMap";
					isoNodeFile = dataFolder + city + "IsoNode";
					isoRoadFile = dataFolder + city + "IsoRoad";
				}
				else
				{
					nodeFile = dataFolder + city + "NodeNew";
					roadFile = dataFolder + city + "RoadNew";
					isoNodeFile = dataFolder + city + "IsoNodeNew";
					isoRoadFile = dataFolder + city + "IsoRoadNew";
				}
			}
			else if("specialNode" == vs[0])
			{

				if(vs[1] != "Fuel" && vs[1] != "Parking" && vs[1] != "off")
				{
					cout <<	"speicalNode Type " << vs[1] << "cannot be found.\nspecialNode Type can only be Fuel/Parking/off" << endl;
					return -1;
				}
				else if(vs[1] == "off")
				{
					bSpecialNode = false;
				}
				else
				{
					bSpecialNode = true;
					specialNodeType = vs[1];
					speicalName = vs[3];
					if("Build" == vs[2])
					{
						bBuild = true;
						orgSpecialNodeFile = dataFolder + "special/" + city + specialNodeType + speicalName;
						specialNodeFile = dataFolder + "special/" + city + specialNodeType + "Node" + speicalName;
						specialRoadFile = dataFolder + "special/" + city + specialNodeType + "Road" + speicalName;
					}
					else if("Use" == vs[2])
					{
						specialNodeFile = dataFolder + "special/" + city + specialNodeType + "Node" + speicalName;
						specialRoadFile = dataFolder + "special/" + city + specialNodeType + "Road" + speicalName;
					}
					else
					{
						cout << "The third parameter of specialNode should be Build or Use" << endl;
						return -1;
					}
				}
			}
			else if("graphType" == vs[0])
			{
				if("Static" == vs[1])
					graphType = vs[1];
				else if("Temporal" == vs[1])//exe
				{
					if(bSpecialNode)
					{
						cout << "Temporal " << speicalName << endl;
						graphType = vs[1];
						bTemporal = true;
						spDate = vs[2];
						orgCostFile = dataFolder+city+ "SpeedProfile" + spDate;
						costFile = dataFolder+ "special/" + city + specialNodeType + "SpeedProfile" + speicalName + spDate;
						costFileNew = dataFolder+ "special/" + city + specialNodeType + "SpeedProfile" + speicalName + spDate + "New";
						findSPFile();
					}
					else//exe
					{
						cout << "Temporal" << endl;
						graphType = vs[1];
						bTemporal = true;
						spDate = vs[2];
						orgCostFile = dataFolder+city+ "SpeedProfile" + spDate;
						costFile = dataFolder+city+ "SpeedProfile" + spDate;
						findSPFile();
					}
				}
				else
				{	
					cout << "No Graph Type of " << vs[1] << endl;
					return -1;
				}
			}
			else if("Index" == vs[0])	
			{
				bIndex = true;
				if("QuadTree" == vs[1])
				{
					sIndex = vs[1];
					quadTreeMaxLevel = parseInt(vs[2]);
				}
				else if("PUNCH" == vs[1])
				{
					sIndex = vs[1];
					if(bSpecialNode)
					{
						sPUNCHDir = dataFolder + "special/index/";
						sPUNCHName = city + specialNodeType + vs[1];
					}
					else	
					{
						sPUNCHDir = dataFolder + "index/";
						sPUNCHName = city + specialNodeType + vs[1];
					}
				}
				else
				{
					cout << "Index type " << vs[1] << "is not supported" << endl;
					return -1;
				}
			}
			else if("PathIndex" == vs[0])//exe
			{
				if("SCH" == vs[1] || "TCH" == vs[1] || "DTCH" == vs[1])
				{
					sPathIndex = vs[1];
					bPathIndex = true;
					if(bSpecialNode)
					{
//						CHFile = dataFolder + "special/index/" + city + specialNodeType + vs[1] + "New_bak";
						CHFile = dataFolder + "special/index/CH/" + city + specialNodeType + vs[1];
						NodeOrderFile = dataFolder + "special/index/CH/" + city + specialNodeType + "NodeOrder";
					}
					else
					{
						CHFile = dataFolder + city + vs[1];		
						NodeOrderFile = dataFolder + city + "NodeOrder";
					}
				}
				else if("GTCH" == vs[1])
				{
					sPathIndex = vs[1];
					bPathIndex = true;
					GTCHID = vs[3];
					CHFile = dataFolder + "special/index/CH/" + city + specialNodeType + vs[1] + "." + GTCHID;
			//		NodeOrderFile = dataFolder + "special/index/CH/" + city + specialNodeType + "NodeOrder";
					bGrid = true;
					gridType = vs[2];
					sGridFolder = dataFolder + "special/index/T2HopGrid/" + gridType + "/";

					sRawGridPath = sGridFolder + city + specialNodeType + speicalName + "RawGrid" + gridType;
					sGridPath = sGridFolder + city + specialNodeType + speicalName + "Grid" + gridType;
					findGridFile();
				}
				else if("T2HOP" == vs[1])//exe
				{
					sPathIndex = vs[1];
					bPathIndex = true;
					if(vs.size() > 2)
					{
						if("Grid" == vs[2])
						{
							bGrid = true;
							gridType = vs[3];
							sGridFolder = dataFolder + "special/index/T2HopGrid/" + gridType + "/";

							sRawGridPath = sGridFolder + city + specialNodeType + speicalName + "RawGrid" + gridType;
							sGridPath = sGridFolder + city + specialNodeType + speicalName + "Grid" + gridType;
							sT2HopGridInnerPath = sGridFolder + specialNodeType + speicalName + "T2HopGridInner" + gridType+"/";
							sT2HopGridInner = sT2HopGridInnerPath; 
							sT2HopGridBoundaryAllPairFolder = sGridFolder + "BoundaryAllPair" + gridType + "/";
							sT2HopGridPathBoundary = sT2HopGridBoundaryAllPairFolder + city + specialNodeType + speicalName + "T2HopGridBoundary" + gridType;
							if(vs.size() == 5)
							{
								bCompress = true;
								sT2HopGridPathBoundaryCompress = sGridFolder + "BoundaryT2Hop" + vs[4] + "/";
								sT2HopGridPathBoundaryCompressCombine = sGridFolder + "BoundaryT2HopGrid" + vs[4] + "/";
								sT2HopGridBoundaryCompressFilenameBase = city + specialNodeType + speicalName + "T2HopGridBoundary" + gridType + vs[4];
								sT2HopGridBoundaryAllPairCompressFolder = sGridFolder + "BoundaryAllPair" + gridType + "Compress/"; 
								sT2HopGridPathInnerCompress = sGridFolder + specialNodeType + speicalName + "T2HopGridInner" + gridType+"Compress/";
							}
							
							sT2HopGridBoundaryAllPairFilename = city + specialNodeType + speicalName + "T2HopGridBoundary" + gridType ; 
							sT2HopGridPathBoundaryNodeOrder = sT2HopGridBoundaryAllPairFolder + "boundaryNodeOrder" + gridType;
						
							//Folder of T2Hop Boundary separate files
							sT2HopGridPathBoundaryHopFolder = sGridFolder + "BoundaryT2Hop/";
							sT2HopGridInnerTestFolder= sGridFolder + "testFile/inner/";
							findGridFile();
						}
						else if("GridBU" == vs[2])
						{
							bGrid = true;
							gridType = vs[3];
							string sError = vs[4];
							sGridFolder = dataFolder + "special/index/T2HopGrid/" + gridType + "/";

							sRawGridPath = sGridFolder + city + specialNodeType + speicalName + "RawGrid" + gridType;
							sGridPath = sGridFolder + city + specialNodeType + speicalName + "Grid" + gridType;
							
							sT2HopGridInnerPath = sGridFolder + specialNodeType + speicalName + "T2HopGridInner" + gridType+ "BU" + sError + "/";
							sT2HopGridInner = sT2HopGridInnerPath; 
							sT2HopGridPathBoundary = sGridFolder + city + specialNodeType + speicalName + "T2HopGridBoundary";
							sT2HopGridBoundaryAllPairFolder = sGridFolder + "BoundaryAllPair" + gridType + "BU" + sError + "/";
							sT2HopGridBoundaryAllPairFilename = city + specialNodeType + speicalName + "T2HopGridBoundary" + gridType ; 
							
							if(vs.size() == 6)
							{
								bCompress = true;
								sT2HopGridPathBoundaryCompressCombine = sGridFolder + "BoundaryT2HopBU" + sError + "Grid" + vs[5] + "/";
								sT2HopGridPathBoundaryCompress = sGridFolder + "BoundaryT2HopBU" + sError + vs[5] + "/";
								sT2HopGridBoundaryCompressFilenameBase = city + specialNodeType + speicalName + "T2HopGridBoundary" + gridType + vs[5];
								sT2HopGridPathInnerCompress = sGridFolder + specialNodeType + speicalName + "T2HopGridInner" + gridType + "BU" + sError + "Compress/";
							}

							sT2HopGridPathBoundaryNodeOrder = sT2HopGridBoundaryAllPairFolder + "boundaryNodeOrder" + gridType;
							//Folder of T2Hop Boundary separate files
							sT2HopGridPathBoundaryHopFolder = sGridFolder + "BoundaryT2HopBU" + sError + "/";
							sT2HopGridInnerTestFolder= sGridFolder + "testFile/inner/";
							findGridFile();
						}
						else if("GridAlpha" == vs[2])
						{
							bGrid = true;
							bAlpha = true;
							gridType = vs[3];
							string sAlpha = vs[4];
							stringstream ss;
							ss << sAlpha;
							ss >> alpha;
//							alpha /= 100;
							alpha = 0.8;
							sGridFolder = dataFolder + "special/index/T2HopGrid/" + gridType + "/";

							sRawGridPath = sGridFolder + city + specialNodeType + speicalName + "RawGrid" + gridType;
							sGridPath = sGridFolder + city + specialNodeType + speicalName + "Grid" + gridType;
							
							sT2HopGridInnerPath = sGridFolder + specialNodeType + speicalName + "T2HopGridInner" + gridType+ "Alpha" + sAlpha + "/";
							sT2HopGridInner = sT2HopGridInnerPath; 
//							sT2HopGridPathBoundary = sGridFolder + city + specialNodeType + speicalName + "T2HopGridBoundary";
							sT2HopGridBoundaryAllPairFolder = sGridFolder + "BoundaryAllPair" + gridType + "Alpha" + sAlpha + "/";
							sT2HopGridBoundaryAllPairFilename = city + specialNodeType + speicalName + "T2HopGridBoundary" + gridType ; 
							
							if(vs.size() == 6)
							{
								bCompress = true;
								sT2HopGridPathBoundaryCompressCombine = sGridFolder + "BoundaryT2HopAlpha" + sAlpha + "Grid" + vs[5] + "/";
								sT2HopGridPathBoundaryCompress = sGridFolder + "BoundaryT2HopAlpha" + sAlpha + vs[5] + "/";
								sT2HopGridBoundaryCompressFilenameBase = city + specialNodeType + speicalName + "T2HopGridBoundary" + gridType + vs[5];
								sT2HopGridPathInnerCompress = sGridFolder + specialNodeType + speicalName + "T2HopGridInner" + gridType + "Alpha" + sAlpha + "Compress/";
							}

							sT2HopGridPathBoundaryNodeOrder = sT2HopGridBoundaryAllPairFolder + "boundaryNodeOrder" + gridType;
							//Folder of T2Hop Boundary separate files
							sT2HopGridPathBoundaryHopFolder = sGridFolder + "BoundaryT2HopAlpha" + sAlpha + "/";
							sT2HopGridInnerTestFolder= sGridFolder + "testFile/inner/";
							findGridFile();

						}
					}
				}
				else
				{
					cout << "PathIndex type " << vs[1] << " is not supported" << endl;
					return -1;
				}
			}
			else if("Test" == vs[0])
			{
				bTest = true;
				vsTestArgs.clear();
				for(int i = 1; i < (int)vs.size(); i++)
					vsTestArgs.push_back(vs[i]);

				vTestAlgorithm = split(vs[1], "|");
				testInputFile = dataFolder + "testFile/" + vs[2];
				testInputFileName = vs[2];
			}
			else if("prune" == vs[0])
			{
				bPrune = true;
				cout << vs[0] << endl;
				cout << vs[1] << endl;
				pruneType = vs[1];
			}
		}
	}
	inConf.close();

	return 0;
}

/*
 * Initialize all the default values
 */
void Configuration::confInit()
{
	bSpecialNode = true;
	bBuild		= false;
	graphType	= "Static";
	bIndex		= false;
	bPathIndex	= false;
	bQuadTree	= false;
	bNewGraphFile = false;
	bTemporal	= false;
	hasNewSP	= false;
	bSPNew		= false;
	bTest		= false;
	bGrid		= false;
	bRawGridFile =false;
	bGridFile	= false;
	bT2HopGridInner		= false;
	bT2HopGridBoundary  = false;
	bCompress	= false;
}

bool Configuration::findNewGraphFile()
{
	vector<string> vFiles;
	vector<string>::iterator ivFiles;
	getdir (dataFolder, vFiles);
				
	string nodeFile = city + "NodeNew";
	string roadFile = city + "RoadNew";
	string quadTreeFile = city + "QuadTree";
	
	int count = 0;
	for(ivFiles = vFiles.begin(); ivFiles != vFiles.end(); ivFiles++)
	{
		if(*ivFiles == nodeFile || *ivFiles == roadFile)
			count++;
		if(count == 2)
			bNewGraphFile = true;

		if(*ivFiles == quadTreeFile)
		{
			bQuadTree = true;
			quadTreeFile = dataFolder + city + "QuadTree";
		}
	}

	return false;
}

void Configuration::findSPFile()
{
	vector<string> vFiles;
	vector<string>::iterator ivFiles;
	string spFile, spFileNew;
	if(bSpecialNode)
	{
		getdir((dataFolder+"special").c_str(), vFiles);
		spFile = city + specialNodeType + "SpeedProfile" + speicalName + spDate;
		spFileNew = city + specialNodeType + "SpeedProfile" + speicalName + spDate + "New";
	}
	else
	{
		getdir((dataFolder).c_str(), vFiles);
		spFile = city + "SpeedProfile" + spDate;
	}
	for(ivFiles = vFiles.begin(); ivFiles != vFiles.end(); ivFiles++)
	{
		if(*ivFiles == spFile)
			hasNewSP = true;
		else if(*ivFiles == spFileNew)
			bSPNew = true;
	}
}

void Configuration::findGridFile()
{
	string rawGridFile = city + specialNodeType + speicalName + "RawGrid" + gridType;
	string GridFile = city + specialNodeType + speicalName + "Grid" + gridType;
	string T2HopGridFileInner = city + specialNodeType + speicalName + "T2HopGridInner" + gridType;
	string T2HopGridFileBoundary = city + specialNodeType + speicalName + "T2HopGridBoundary" + gridType;
	
	vector<string> vFiles;
	vector<string>::iterator ivFiles;

	int i = 0;
	getdir(sGridFolder.c_str(), vFiles);
	cout << rawGridFile << endl << GridFile << endl;
	for(ivFiles = vFiles.begin(); ivFiles != vFiles.end(); ivFiles++)
	{
		if(*ivFiles == rawGridFile)
		{
			bRawGridFile = true;
			i++;
		}
		else if(*ivFiles == GridFile)
		{
			bGridFile = true;
			i++;
		}
		else if(*ivFiles == T2HopGridFileInner)
		{
			bT2HopGridInner = true;
			i++;
		}
		else if(*ivFiles == T2HopGridFileBoundary)
		{
			bT2HopGridBoundary = true;
			i++;
		}

		if(i == 4)
			break;
	}
}
