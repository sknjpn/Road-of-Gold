#include"Urban.h"
#include"JSON.h"
#include"Node.h"
Urban::Urban(int _joinedNodeID)
	: id(int(urbans.size()))
	, name(L"hogeTown")
	, joinedNodeID(_joinedNodeID)
	, numCitizens(1)
{
	nodes[_joinedNodeID].ownUrbanID = id;
	resource.resize(energyData.size());
}
Urban::Urban()
	: id(int(urbans.size()))
	, name(L"hogeTown")
	, joinedNodeID(-1)
	, numCitizens(1)
{
	resource.resize(energyData.size());
}
Pos&	Urban::getPos() const { return nodes[joinedNodeID].pos; }