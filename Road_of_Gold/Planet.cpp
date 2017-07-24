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
	const int numGroups = 50;

	for (int j = 0; j < numGroups; j++)
	{
		groups.emplace_back();
		auto& g = groups.back();
		g.name = GroupName.choice();
		auto& u = urbans[Random(int(urbans.size() - 1))];
		for (int i = 0; i < 20; i++)
		{
			vehicles.emplace_back(u.id);
			auto& v = vehicles.back();
			v.joinedGroupID = g.id;
		}
	}
}
