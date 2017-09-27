#include"Group.h"
#include"Urban.h"
#include"Route.h"
#include"Wallet.h"
#include"Planet.h"
#include"Vehicle.h"
#include"ItemData.h"
#include"VehicleData.h"
#include"Fleet.h"

double tmr = 0.0;

void	updateGroups()
{
	tmr += planet.timeSpeed;

	if (tmr > 0.0)
	{
		tmr -= 100.0;
		//下位1/4の事業見直し
		for (int i = 0; i < int(vehicles.size() / 10); i++)
		{
			int min = 0;
			Fleet* target = nullptr;
			for (auto& f : fleets)
			{
				if (!f.stopFlag && (target == nullptr || min > f.wallet().money))
				{
					target = &f;
					min = f.wallet().money;
				}
			}
			if (target != nullptr && !target->planFixed) target->stopFlag = true;
		}
		int avg = 0;
		for (auto& f : fleets) avg += f.wallet().money;

		for (auto& f : fleets)
		{
			if (f.wallet().money <= 0 && !f.planFixed) f.stopFlag = true;
			f.wallet().money = 0;
			f.wallet().expenses = 0;
			f.wallet().income = 0;
		}
	}

	for (auto& f : fleets)
	{
		if (f.chain.isError && !f.planFixed)
		{
			f.nowUrban = &urbans.choice();

			f.chain.isError = false;
			auto* u2 = f.nowUrban;
			auto rs = u2->ownRoutes.filter([&f](const Route* r) { return (r->isSeaRoute == f.data.isShip) && (r->movingCost <= f.data.range); });

			//ルートがない場合、再生成
			if (rs.isEmpty())
			{
				f.nowUrban = &urbans.choice();
				continue;
			}

			auto* r = rs.choice();
			auto* u1 = r->toUrban;
			int itemType = -1;

			for (int j = 0; j < int(itemData.size()); j++)
			{
				auto i = itemData.choice().id();

				//if (u2->isSoldOut(i) || u2->shelves.at(i).tradeLog.numProduction.front() == 0) continue;
				//else
				if (!u2->isSoldOut(i))
				{
					if (u1->isSoldOut(i) || u1->cost(i) > u2->cost(i))
					{
						itemType = i;
						break;
					}
				}
			}
			if (itemType == -1)
			{
				f.nowUrban = &urbans.choice();
				continue;
			}

			f.route = nullptr;
			f.routeProgress = 0;
			f.sleepTimer = 0;
			f.chain.clear();
			f.period = (2.0 * r->movingCost / f.data.speed + 1.0);	//周回に要する時間
			f.chain.rings.emplace_back(Code::Move, u1->id());
			f.chain.rings.emplace_back(Code::Sell, 0);
			f.chain.rings.emplace_back(Code::Move, u2->id());
			f.chain.rings.emplace_back(Code::Buy, itemType);
			f.chain.rings.emplace_back(Code::Jump, 0);
			bool flag = true;
			f.exportLog = Export(u2, u1, itemType, 50 / f.period);
			for (auto& e : exports)
			{
				if (e.from == u2 && e.to == u1&& e.itemType == itemType)
				{
					e.numItemPerDay += 50 / f.period;
					flag = false;
					break;
				}
			}
			if (flag) exports.emplace_back(f.exportLog);

			f.timer = 0;

			//販売価格の設定
			f.wallet().price = u2->cost(itemType);
		}
	}
}