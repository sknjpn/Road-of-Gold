#include"Group.h"
#include"Urban.h"
#include"Route.h"
#include"Wallet.h"
#include"Planet.h"
#include"Vehicle.h"
#include"ItemData.h"
#include"VehicleData.h"

double tmr = 0.0;

void	updateGroups()
{
	tmr += planet.timeSpeed;

	if (tmr > 0.0)
	{
		tmr -= Random(50.0, 100.0);
		//下位1/4の事業見直し
		for (int i = 0; i < int(vehicles.size() / 10); i++)
		{
			int min = 0;
			Vehicle* target = nullptr;
			for (auto& v : vehicles)
			{
				if (!v.stopFlag && (target == nullptr || min > v.wallet().money))
				{
					target = &v;
					min = v.wallet().money;
				}
			}
			if (target != nullptr && !target->planFixed) target->stopFlag = true;
		}
		int avg = 0;
		for (auto& v : vehicles) avg += v.wallet().money;

		for (auto& v : vehicles)
		{
			if (v.wallet().money <= 0 && !v.planFixed) v.stopFlag = true;
			v.wallet().money = 0;
			v.wallet().expenses = 0;
			v.wallet().income = 0;
		}
	}

	for (auto& v : vehicles)
	{
		if ((v.chain.isEmpty() || v.reader >= int(v.chain.size()) || v.isError) && !v.planFixed)
		{
			v.nowUrban = &urbans.choice();
			v.vehicleType = vehicleData.choice().id();

			v.isError = false;
			auto* u2 = v.nowUrban;
			auto rs = u2->ownRoutes.filter([&v](const Route* r) { return (r->isSeaRoute == v.data().isShip) && (r->movingCost <= v.data().range); });

			//ルートがない場合、再生成
			if (rs.isEmpty())
			{
				v.nowUrban = &urbans.choice();

				//乗り物種類の変更
				v.vehicleType = vehicleData.choice().id();
				continue;
			}

			auto* r = rs.choice();
			auto* u1 = r->toUrban;
			int itemType = -1;

			for (int j = 0; j < int(itemData.size()); j++)
			{
				auto i = itemData.choice().id();

				//if (u2->isSoldOut(i) || u2->baskets.at(i).tradeLog.numProduction.front() == 0) continue;
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
				v.nowUrban = &urbans.choice();

				//乗り物種類の変更
				v.vehicleType = vehicleData.choice().id();
				continue;
			}

			v.route = nullptr;
			v.reader = 0;
			v.routeProgress = 0;
			v.sleepTimer = 0;
			v.chain.clear();
			v.period = (2.0 * r->movingCost / v.data().speed + 1.0);	//周回に要する時間
			v.chain = {
				{ Code::Move, u1->id() },
				{ Code::Sell, 0 },
				{ Code::Move, u2->id() },
				{ Code::Buy,  itemType },	//Buyerに指示&商品受け取り
				{ Code::Jump, 0 }
			};
			v.chain.resize(10, { Code::None, 0 });
			bool flag = true;
			v.exportLog = Export(u2, u1, itemType, 50 / v.period);
			for (auto& e : exports)
			{
				if (e.from == u2 && e.to == u1&& e.itemType == itemType)
				{
					e.numItemPerDay += 50 / v.period;
					flag = false;
					break;
				}
			}
			if (flag) exports.emplace_back(v.exportLog);

			v.timer = 0;

			//販売価格の設定
			v.wallet().price = u2->cost(itemType);
		}
	}
}