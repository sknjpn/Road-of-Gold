#include"Group.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"GlobalVariables.h"
#include"JSON.h"

Array<Vehicle> vehicles;

Vehicle::Vehicle(int _nowUrbanID)
	: id(int(vehicles.size()))
	, nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
	, progress(0)
	, sleepTimer(0)
	, stock()
	, joinedGroupID(-1)
{}
Vec2	Vehicle::getMPos() const
{
	if (routeID != -1)
	{
		double length = routeProgress;

		for (int i = 0; i < routes[routeID].pathIDs.size(); ++i)
		{
			auto& p = paths[routes[routeID].pathIDs[i]];
			const auto line = p->getLine();

			if (length > p->cost) length -= p->cost;
			else return line.begin.lerp(line.end, length / p->cost);
		}
	}
	return nodes[urbans[nowUrbanID].joinedNodeID].pos.mPos;
}
void	Vehicle::draw() const
{
	auto& g = groups[joinedGroupID];
	const Circle shape(0.01);


	if (routeID != -1)
	{
		auto& r = routes[routeID];
		double length = routeProgress;

		for (int i = 0; i < r.pathIDs.size(); ++i)
		{
			auto& p = paths[r.pathIDs[i]];
			const auto line = p->getLine();

			if (length > p->cost)
			{
				line.draw(0.005, Color(g.color, 64));
				length -= p->cost;
			}
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->cost);

				Line(line.begin, pos).draw(0.005, Color(g.color, 64));
				shape.movedBy(pos).draw(stock.num == 0 ? Color(0, 0) : iData[stock.itemType].color).drawFrame(0.005, Palette::Black);
				break;
			}
		}
	}
	else Triangle(-0.02, -0.03, 0.02, -0.03, 0.0, 0.0).movedBy(getMPos()).draw(g.color);
}

Array<Group> groups;
Group::Group()
	: id(int(groups.size()))
	, name(Format(L"hoge商会"))
	, money(0)
	, color(RandomColor())
	, timer(0)
	, moneyLog(0)
{}
void	Vehicle::update()
{
	auto& g = groups[joinedGroupID];
	double actionTime = timeSpeed;
	if (routeID == -1 && sleepTimer==0 && chain.isEmpty())
	{
		progress = 0;
		auto& u1 = urbans[nowUrbanID];
		auto& u2 = urbans.choice();

		if (&u1 != &u2)
		{
			g.description = Format(u1.name, L"-", u2.name, L"間");
			for (auto& r : u1.getRoutesToUrban(u2.id))
				chain.push_back({ int16(Command::MOVE), r->destinationUrbanID });

			chain.push_back({ int16(Command::SELL), int32(1000) });
			chain.push_back({ int16(Command::BUY), iData.choice().id });

			for (auto& r : u2.getRoutesToUrban(u1.id))
				chain.push_back({ int16(Command::MOVE), r->destinationUrbanID });

			chain.push_back({ int16(Command::SELL), int32(1000) });
			chain.push_back({ int16(Command::BUY), iData.choice().id });

			chain.push_back({ int16(Command::JUMP), int32(0) });
		}
	}

	for (;;)
	{
		if (routeID != -1)
		{
			if (actionTime >= routes[routeID].totalCost - routeProgress)
			{
				actionTime -= routes[routeID].totalCost - routeProgress;
				nowUrbanID = routes[routeID].destinationUrbanID;
				routeProgress = 0.0;
				routeID = -1;
				sleepTimer = 1 / 24.0;	//1hour
			}
			else
			{
				routeProgress += actionTime;
				break;
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
				break;
			}
		}
		else if (progress < int(chain.size()) && progress >= 0)
		{
			//スクリプトの実行
			const int command = chain[progress].first;
			const int data = chain[progress].second;

			switch (Command(command))
			{
			case Command::MOVE:	//都市へ移動
				for (auto& rID : urbans[nowUrbanID].routeIDs)
				{
					if (routes[rID].destinationUrbanID == data)
					{
						routeProgress = 0.0;
						routeID = routes[rID].id;
						break;
					}
				}
				break;

			case Command::JUMP:	//アドレスジャンプ命令
				progress = data;
				break;

			case Command::WAIT: //ウェイト命令
				sleepTimer = 1;
				break;

			case Command::BUY: //購買命令
				if (stock.num == 0)
				{
					Basket& b = urbans[nowUrbanID].baskets[data];
					const int numBuy = Min(10, b.getNumItem());
					if (numBuy > 0)
					{
						g.money -= b.getCost(numBuy);
						b.buyItem(numBuy);
						stock.num = numBuy;
						stock.itemType = data;
					}
					sleepTimer = 1 / 24.0;	//1hour
				}
				else ++progress;
				break;

			case Command::SELL:	//販売命令
				if (stock.num > 0)
				{
					Basket& b = urbans[nowUrbanID].baskets[stock.itemType];
					b.addRing(data, stock.num, &g);
					stock.num = 0;
					sleepTimer = 1 / 24.0;	//1hour
				}
				else ++progress;
				break;

			default:	//存在しない命令
				++progress;
				break;
			}
		}
		else break;	//存在しない地点の参照
	}
}

void Group::update()
{
	timer += timeSpeed;
	if (timer > 100)
	{
		timer = 0;
		if (money - moneyLog < 1000)
		{
			for (auto& v : vehicles)
			{
				if(v.joinedGroupID == id) v.chain.clear();
			}
		}
		moneyLog = money;
	}
}