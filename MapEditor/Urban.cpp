#include"Urban.h"
#include"EnergyData.h"
#include"Node.h"
Urban::Urban(int _joinedNodeID)
	: name(L"hogeTown")
	, joinedNodeID(_joinedNodeID)
	, numCitizens(1)
	, productivity(1.0)
{
	energies.resize(energyData.size(), 0);
}
Urban::Urban()
	: name(L"hogeTown")
	, joinedNodeID(-1)
	, numCitizens(1)
	, productivity(1.0)
{
	energies.resize(energyData.size(), 0);
}
Urban::Urban(const JSONValue _json)
	: name(_json[L"Name"].getString())
	, joinedNodeID(_json[L"JoinedNodeID"].getOr<int>(-1))
	, productivity(_json[L"Productivity"].getOr<double>(1.0))
	, numCitizens(_json[L"NumCitizens"].getOr<int>(1))
{
	//éëåπîzíu
	energies.resize(energyData.size(), 0);
	for (auto i : _json[L"Energies"].objectView())
	{
		if (getEnergyType(i.name) == -1) LOG_ERROR(L"ë∂ç›ÇµÇ»Ç¢EnergyÇ™ì«Ç›çûÇ‹ÇÍÇ‹ÇµÇΩ");
		else energies.at(getEnergyType(i.name)) = i.value.getOr<int>(0);
	}
}
Pos&	Urban::getPos() const { return nodes[joinedNodeID].pos; }
int		Urban::id() const
{
	return int(this - &urbans.front());
}