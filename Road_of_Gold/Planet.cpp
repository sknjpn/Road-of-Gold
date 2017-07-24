#include"Planet.h"
#include"Group.h"
#include"Urban.h"
#include"GlobalVariables.h"


Planet::Planet()
	: mapTexture()
{}
void	Planet::makeGroupsRandom()
{
	const int numGroups = 20;

	for (int j = 0; j < numGroups; j++)
	{
		groups.emplace_back();
		auto& g = groups.back();
		auto& u = urbans[Random(int(urbans.size() - 1))];
		for (int i = 0; i < 30; i++) g.vehicles.emplace_back(u.id);
		for (auto& v : g.vehicles) v.color = g.color;
	}
}
