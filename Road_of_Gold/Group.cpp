#include"Group.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"GlobalVariables.h"
#include"JSON.h"

Vehicle::Vehicle(int _nowUrbanID)
	: nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
	, progress(0)
	, sleepTimer(0)
	, stock()
{
	chain = {
		{ int16(2), int32(0)},
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(3), iData.choice().id },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(3), iData.choice().id },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(3), iData.choice().id },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(3), iData.choice().id },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(3), iData.choice().id },
		{ int16(1), int32(0)}
	};
}
bool	Vehicle::inRoute() const { return routeID != -1; }
Urban&	Vehicle::getNowUrban() const { return urbans[nowUrbanID]; }
Route&	Vehicle::getRoute() const { return routes[routeID]; }
Vec2		Vehicle::getMPos() const
{
	if (inRoute())
	{
		auto& r = getRoute();
		double length = routeProgress;
		for (int i = 0; i < r.pathIDs.size(); i++)
		{
			auto& p = paths[r.pathIDs[i]];
			const auto line = p->getLine();

			if (length > p->length) length -= p->length;
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->length);
				return pos;
			}
		}
	}
	return getNowUrban().getPos().mPos;
}
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
	else
	{
		Triangle(-0.02, -0.03, 0.02, -0.03, 0.0, 0.0).movedBy(getMPos()).draw(color);
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
	double actionTime = timeSpeed;

	for (;;)
	{
		if (inRoute())
		{
			if (actionTime > getRoute().totalLength - routeProgress)
			{
				actionTime -= getRoute().totalLength - routeProgress;
				nowUrbanID = getRoute().destinationUrbanID;
				routeProgress = 0.0;
				routeID = -1;
			}
			else
			{
				routeProgress += actionTime;
				actionTime = 0.0;
				return;
			}
		}
		else if (sleepTimer > 0)
		{
			if (actionTime > sleepTimer)
			{
				actionTime -= sleepTimer;
				sleepTimer = 0;
			}
			else
			{
				sleepTimer -= actionTime;
				actionTime = 0;
				break;
			}
		}
		else
		{
			//スクリプトの実行
			for (;;)
			{
				if (progress >= int(chain.size()) || sleepTimer > 0) break;
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

				case 2: //ウェイト命令
				{
					sleepTimer += 1.0;
					++progress;
				}
				break;

				case 3: //購買命令
				{
					if (stock.num == 0)
					{
						const int itemType = chain[progress].second;
						const int numBuy = Min(10, getNowUrban().baskets[chain[progress].second].getNumItem());
						if (numBuy > 0) getNowUrban().baskets[itemType].buyItem(numBuy);
						stock.num = numBuy;
						stock.itemType = itemType;
					}
					++progress;
				}
				break;

				default://存在しない命令
				{
					++progress;
				}
				break;

				}
			}
		}
	}
}

void Group::update()
{
	for (auto& v : vehicles)
		v.update();
}