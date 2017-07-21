#include"Urban.h"
#include"Node.h"
Urban::Urban(int _joinedNodeID)
	: id(int(urbans.size()))
	, name(L"hogeTown")
	, joinedNodeID(_joinedNodeID)
{
	nodes[_joinedNodeID].ownUrbanID = id;
	//resource.resize(rData.size());
}