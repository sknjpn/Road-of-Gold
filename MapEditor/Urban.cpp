#include"Urban.h"
#include"EnergyData.h"
#include"Node.h"
Urban::Urban(int _joinedNodeID)
	: name(L"hogeTown")
	, joinedNodeID(_joinedNodeID)
	, numCitizens(1)
{
	energies.resize(energyData.size());
}
Urban::Urban()
	: name(L"hogeTown")
	, joinedNodeID(-1)
	, numCitizens(1)
{
	energies.resize(energyData.size());
}
Pos&	Urban::getPos() const { return nodes[joinedNodeID].pos; }
int		Urban::id() const
{
	return int(this - &urbans.front());
}