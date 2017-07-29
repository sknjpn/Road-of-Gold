#include"Planet.h"
#include"Group.h"
#include"Urban.h"
#include"GlobalVariables.h"
#include"JSON.h"

Planet::Planet()
	: mapTexture()
{}
void	Planet::makeGroupsRandom()
{
	const int numGroups = 40;

	for (int j = 0; j < numGroups; j++)
	{
		groups.emplace_back();
		auto& g = groups.back();
		g.name = GroupName.choice();
		auto& u = urbans[Random(int(urbans.size() - 1))];
		for (int i = 0; i < 1; i++)
		{
			vehicles.emplace_back(u.id);
			g.ownVehicleIDs.emplace_back(vehicles.back().id);
			auto& v = vehicles.back();
			v.joinedGroupID = g.id;
		}
	}
}
