#include"Urban.h"
#include"EnergyData.h"
#include"Node.h"
Urban::Urban(int _joinedNodeID)
	: name(U"hogeTown")
	, joinedNodeID(_joinedNodeID)
	, numCitizens(1)
	, productivity(1.0)
{
	energies.resize(energyData.size(), 0);
}
Urban::Urban()
	: name(U"hogeTown")
	, joinedNodeID(-1)
	, numCitizens(1)
	, productivity(1.0)
{
	energies.resize(energyData.size(), 0);
}
Urban::Urban(const JSONValue _json)
	: name(_json[U"Name"].getString())
	, joinedNodeID(_json[U"JoinedNodeID"].getOr<int>(-1))
	, productivity(_json[U"Productivity"].getOr<double>(1.0))
	, numCitizens(_json[U"NumCitizens"].getOr<int>(1))
{
	//Ž‘Œ¹”z’u
	energies.resize(energyData.size(), 0);
	for (auto i : _json[U"Energies"].objectView())
	{
		if (getEnergyType(i.name) == -1) Logger << U"‘¶Ý‚µ‚È‚¢Energy‚ª“Ç‚Ýž‚Ü‚ê‚Ü‚µ‚½";
		else energies.at(getEnergyType(i.name)) = i.value.getOr<int>(0);
	}
}
Pos&	Urban::getPos() const { return nodes[joinedNodeID].pos; }
int		Urban::id() const
{
	return int(this - &urbans.front());
}