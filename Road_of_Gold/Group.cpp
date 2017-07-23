#include"Group.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"GlobalVariables.h"

Vehicle::Vehicle(int _nowUrbanID)
	: nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
	, progress(0)
{
	chain = {
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(1), int32(0)}
	};
}
bool	Vehicle::inRoute() const { return routeID != -1; }
Urban&	Vehicle::getNowUrban() const { return urbans[nowUrbanID]; }
Route&	Vehicle::getRoute() const { return routes[routeID]; }
void	Vehicle::draw() const
{
	const Circle shape(0.01);
	if (inRoute())
	{
		auto& r = getRoute();
		double length = routeProgress;
		for (int i = 0; i < r.pathIDs.size(); i++)
		{
			auto& p = paths[r.pathIDs[i]];
			const auto line = p->getLine();

			if (length > p->length)
			{
				line.draw(0.005, Color(color, 64));
				length -= p->length;
			}
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->length);
				Line(line.begin, pos).draw(0.005, Color(color, 64));
				shape.movedBy(pos).draw(color).drawFrame(0.005, Palette::Black);
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
		//スクリプトの実行
		for (;;)
		{
			if (progress >= int(chain.size())) break;
			switch (chain[progress].first)
			{
			case 0:	//都市へ移動
			{
				Urban& targetUrban = urbans[chain[progress].second];
				for (auto& r : getNowUrban().getRoutes())
				{
					if (r->destinationUrbanID == targetUrban.id)
					{
						routeProgress = 0.0;
						routeID = r->id;
						break;
					}
				}
				++progress;

				if (inRoute()) break;	//ルートが決まった場合
			}
			break;

			case 1:	//アドレスジャンプ命令
			{
				progress = chain[progress].second;
			}
			break;

			default://存在しない命令
			{
				++progress;
			}
			break;

			}
			break;
		}
		/*
		routeProgress += timeSpeed;
		if (routeProgress >= 0.0)
		{
			routeProgress = 0.0;
			const auto rs = getNowUrban().getRoutes();
			if (!rs.isEmpty()) routeID = rs[Random(int(rs.size() - 1))]->id;
			else routeProgress = -100.0;
		}*/
	}
}

void Group::update()
{
	for (auto& v : vehicles)
		v.update();
}