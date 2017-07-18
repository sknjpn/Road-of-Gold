#include"Group.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"GlobalVariables.h"

Vehicle::Vehicle(int _nowUrbanID)
	: nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
{}
bool	Vehicle::inRoute() const{return routeID != -1;}
Urban&	Vehicle::getNowUrban() const{return urbans[nowUrbanID];}
Route&	Vehicle::getRoute() const{return routes[routeID];}
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
	, money(0)
{}
void Vehicle::update()
{
	if (inRoute())
	{
		routeProgress += timeSpeed;
		if (routeProgress >= getRoute().totalLength)
		{
			nowUrbanID = getRoute().destinationUrbanID;
			routeID = -1;
		}
	}
	else
	{
		routeProgress += timeSpeed;
		if (routeProgress >= 0.0)
		{
			routeProgress = 0.0;
			const auto rs = getNowUrban().getRoutes();
			if (!rs.isEmpty()) routeID = rs[Random(int(rs.size() - 1))]->id;
			else routeProgress = -100.0;
		}
	}
}

void Group::update()
{
	for (auto& v : vehicles)
		v.update();
}