#include"Group.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"GlobalVariables.h"
#include"JSON.h"

Array<Vehicle> vehicles;

Vehicle::Vehicle(int _nowUrbanID)
	: nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
	, progress(0)
	, sleepTimer(0)
	, stock()
	, joinedGroupID(-1)
{}
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

			if (length > p->cost) length -= p->cost;
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->cost);
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

			if (length > p->cost)
			{
				line.draw(0.005, Color(groups[joinedGroupID].color, 64));
				length -= p->cost;
			}
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->cost);
				Line(line.begin, pos).draw(0.005, Color(groups[joinedGroupID].color, 64));
				shape.movedBy(pos).draw(groups[joinedGroupID].color).drawFrame(0.005, Palette::Black);
				break;
			}
		}
	}
	else
	{
		Triangle(-0.02, -0.03, 0.02, -0.03, 0.0, 0.0).movedBy(getMPos()).draw(groups[joinedGroupID].color);
	}
}

Array<Group> groups;
Group::Group()
	: id(int(groups.size()))
	, name(Format(L"第", id, L"名無し商会"))
	, money(0)
	, color(RandomColor())
{}
void Vehicle::update()
{
	if (chain.isEmpty())
	{
		auto& u1 = getNowUrban();
		auto& u2 = urbans.choice();
		if (&u1 != &u2)
		{
			for (auto& r : u1.getRoutesToUrban(u2.id))
				chain.push_back({ int16(Command::MOVE), r->destinationUrbanID });

			chain.push_back({ int16(Command::SELL), int32(1000) });
			chain.push_back({ int16(Command::BUY), iData.choice().id });
			chain.push_back({ int16(Command::WAIT), int32(0) });

			for (auto& r : u2.getRoutesToUrban(u1.id))
				chain.push_back({ int16(Command::MOVE), r->destinationUrbanID });

			chain.push_back({ int16(Command::SELL), int32(1000) });
			chain.push_back({ int16(Command::BUY), iData.choice().id });
			chain.push_back({ int16(Command::WAIT), int32(0) });

			chain.push_back({ int16(Command::JUMP), int32(0) });
		}
	}
	double actionTime = timeSpeed;

	for (;;)
	{
		if (actionTime == 0) break;
		else if (inRoute())
		{
			if (actionTime >= getRoute().totalLength - routeProgress)
			{
				actionTime -= getRoute().totalLength - routeProgress;
				nowUrbanID = getRoute().destinationUrbanID;
				routeProgress = 0.0;
				routeID = -1;
				++progress;
			}
			else
			{
				routeProgress += actionTime;
				actionTime = 0;
			}
		}
		else if (sleepTimer > 0)
		{
			if (actionTime > sleepTimer)
			{
				actionTime -= sleepTimer;
				sleepTimer = 0;
				++progress;
			}
			else
			{
				sleepTimer -= actionTime;
				actionTime = 0;
			}
		}
		else
		{
			//スクリプトの実行
			if (progress >= int(chain.size())) break;
			for (;;)
			{
				if (inRoute() || sleepTimer > 0) break;
				switch (Command(chain[progress].first))
				{
				case Command::MOVE:	//都市へ移動
				{
					Urban& targetUrban = urbans[chain[progress].second];
					for (auto& rID : getNowUrban().routeIDs)
					{
						if (routes[rID].destinationUrbanID == targetUrban.id)
						{
							routeProgress = 0.0;
							routeID = routes[rID].id;
							break;
						}
					}
				}
				break;

				case Command::JUMP:	//アドレスジャンプ命令
				{
					progress = chain[progress].second;
				}
				break;

				case Command::WAIT: //ウェイト命令
				{
					sleepTimer += 1;
				}
				break;

				case Command::BUY: //購買命令
				{
					if (stock.num == 0)
					{
						const int itemType = chain[progress].second;
						const int numBuy = Min(10, getNowUrban().baskets[chain[progress].second].getNumItem());
						if (numBuy > 0)
						{
							groups[joinedGroupID].money -= getNowUrban().baskets[chain[progress].second].getCost(numBuy);
							getNowUrban().baskets[itemType].buyItem(numBuy);
							stock.num = numBuy;
							stock.itemType = itemType;
						}
					}
					sleepTimer = 0.1;
					//++progress;
				}
				break;

				case Command::SELL:	//販売命令
				{
					if (stock.num > 0)
					{
						getNowUrban().baskets[stock.itemType].addRing(chain[progress].second, stock.num, &groups[joinedGroupID]);
						stock.num = 0;
					}
					sleepTimer = 0.1;
					//++progress;
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
}