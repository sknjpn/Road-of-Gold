#include "Group.h"
#include "Node.h"
#include "Urban.h"
#include "Route.h"

Vehicle::Vehicle(const int& _nowUrbanID)
	: nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
{}
bool	Vehicle::inRoute() const
{
	return routeID != -1;
}
Urban&	Vehicle::getNowUrban() const
{
	return urbans[nowUrbanID];
}
Route&	Vehicle::getRoute() const
{
	return routes[routeID];
}
void	Vehicle::draw() const
{
	const Circle shape(0.01);
	if (inRoute())
	{
		auto& r = getRoute();
		double len = routeProgress;
		for (int i = 0; i < r.pathIDs.size(); i++)
		{
			auto& p = paths[r.pathIDs[i]];
			const auto line = p->getLine();

			if (len > p->len)
			{
				line.draw(0.005, Palette::Red);
				len -= p->len;
			}
			else
			{
				const auto pos = line.begin.lerp(line.end, len / p->len);
				Line(line.begin,pos).draw(0.005, Palette::Red);
				shape.movedBy(pos).draw(Palette::Yellow);
				break;
			}
		}
	}
}

Array<Group> groups;
Group::Group()
	: id(int(groups.size()))
	, name(L"hoge")
	, color(RandomHSV())
{}

void makeGroupsRandom()
{
	const int numGroups = 20;

	for (int j = 0; j < numGroups; j++)
	{
		groups.emplace_back();
		auto& g = groups.back();
		auto& u = urbans[Random(int(urbans.size() - 1))];
		for (int i = 0; i < 10; i++) g.vehicles.emplace_back(u.id);
	}
}