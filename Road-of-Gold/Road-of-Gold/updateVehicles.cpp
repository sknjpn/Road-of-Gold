#include"Planet.h"
#include"Vehicle.h"
#include"Urban.h"
#include"Route.h"
#include"VehicleData.h"
#include"ItemData.h"
#include"UI.h"
#include"Sound.h"
#include"Export.h"


void	updateVehicles()
{
	for (auto& v : vehicles)
	{
		double actionTime = planet.timeSpeed;

		if (v.chain.isEmpty() || v.reader >= int(v.chain.size()))
		{
			//乗り物種類の変更
			v.vehicleType = vehicleData.choice().id();

			auto* u2 = v.nowUrban;
			auto rs = u2->ownRoutes.filter([&v](const Route* r) { return r->isSeaRoute == v.data().isShip && r->movingCost <= v.data().range; });

			//ルートがない場合、再生成
			if (rs.isEmpty())
			{
				v.nowUrban = &urbans.choice();
				continue;
			}
			auto* r = rs.choice();
			auto* u1 = r->toUrban;
			 
			int itemType = -1;
			{
				for (int j = 0; j < 10; j++)
				{
					auto i = itemData.choice().id();
					if (u2->isSoldOut(i) || u2->baskets.at(i).tradeLog.numProduction.front() == 0) continue;
					else
					{
						if (u1->isSoldOut(i) || u1->cost(i) > u2->cost(i))
						{
							itemType = i;
							break;
						}
					}
				}
			}
			if (itemType == -1)
			{
				v.nowUrban = &urbans.choice();
				continue;
			}

			v.route = nullptr;
			v.reader = 0;
			v.routeProgress = 0;
			v.sleepTimer = 0;
			v.chain.clear();
			v.period = 2.0*r->movingCost / v.data().speed;	//周回に要する時間
			v.chain = {
				{ Code::Move, u1->id() },
				{ Code::Sell, 0 },
				{ Code::Move, u2->id() },
				{ Code::MVol, 100 },
				{ Code::Buy,  itemType },	//Buyerに指示&商品受け取り
				{ Code::Jump, 0 }
			};
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
			if (flag)
			{
				exports.emplace_back(v.exportLog);
			}
			v.timer = 0;

			//販売価格の設定
			v.wallet().price = u2->cost(itemType);
		}

		if (!v.chain.isEmpty() && v.reader < int(v.chain.size()))
		{
			for (;;)
			{
				if (v.route != nullptr)
				{
					if (v.route->movingCost / v.data().speed <= v.routeProgress + actionTime)
					{
						actionTime -= v.route->movingCost / v.data().speed - v.routeProgress;
						v.timer += v.route->movingCost / v.data().speed - v.routeProgress;
						v.routeProgress = 0;
						v.nowUrban = v.route->toUrban;
						v.route = nullptr;
					}
					else
					{
						v.routeProgress += actionTime;
						v.timer += actionTime;
						actionTime = 0;
						break;
					}
				}
				else if (v.sleepTimer > 0)
				{
					if (v.sleepTimer <= actionTime)
					{
						v.sleepTimer = 0.0;
						actionTime -= v.sleepTimer;
						v.timer += v.sleepTimer;
					}
					else
					{
						v.sleepTimer -= actionTime;
						v.timer += actionTime;
						actionTime = 0;
						break;
					}
				}
				else if (v.stopFlag)	//事業停止
				{
					v.stopFlag = false;
					for (auto& e : exports)
					{
						if (e.from == v.exportLog.from &&
							e.to == v.exportLog.to &&
							e.itemType == v.exportLog.itemType)
						{
							e.numItemPerDay -= v.exportLog.numItemPerDay;
							break;
						}
					}
					exports.remove_if([](const Export& e) { return e.numItemPerDay == 0; });
					v.chain.clear();
					v.reader = 0;

					for (auto& u : urbans)
					{
						u.buyers.remove_if([&v](const Buyer& b) { return b.walletID == v.walletID; });
					}
					break;
				}
				/*if (v.timer > 50.0)
				{

					v.timer = 0;
					if (v.wallet().money <= 0)
					{
						for (auto& e :exports)
						{
							if (e.from == v.exportLog.from &&
								e.to == v.exportLog.to &&
								e.itemType == v.exportLog.itemType)
							{
								e.numItemPerDay -= v.exportLog.numItemPerDay;
								break;
							}
						}
						exports.remove_if([](const Export& e) { return e.numItemPerDay == 0; });
						v.wallet().money = 0;
						v.chain.clear();
						v.reader = 0;
						break;
					}
					else
					{
						v.wallet().money = 0;
					}
				}*/
				else
				{
					auto& c = v.chain[v.reader];
					switch (c.first)
					{
					case Code::Move:
						for (auto& r : v.nowUrban->ownRoutes)
							if (r->toUrban->id() == c.second) v.route = r;
						if (v.route == nullptr) LOG_ERROR(L"Chainの指す都市が異常です。");
						v.reader++;
						break;
					case Code::Wait:
						v.sleepTimer = 1.0;
						v.reader++;
						break;
					case Code::Jump:
						v.reader = 0;
						break;
					case Code::Buy:
					{
						bool flag = false;
						for (auto& b : v.nowUrban->buyers)
						{
							if (b.walletID == v.walletID)
							{
								b.progress = 0;
								v.cargo = b.casket;
								b.casket.numItem = 0;
								b.target = v.maxVolume;
								flag = true;
								break;
							}
						}
						if (!flag)
						{
							v.nowUrban->buyers.emplace_back(v.walletID, c.second, int(v.period), v.maxVolume);
						}
						v.reader++;
						break;
					}
					case Code::Sell:
						if (v.cargo.numItem > 0)
						{
							v.nowUrban->sellers.emplace_back(v.walletID, v.cargo, int(v.period));
						}
						v.cargo.numItem = 0;
						v.reader++;
						break;
					case Code::MVol:
						v.maxVolume = c.second;
						v.reader++;
						break;
					}
				}
			}
		}
	}

	if (MouseL.down())
	{
		ui.selectedVehicleID = -1;
		if (ui.selectedUrbanID == -1)
		{
			for (int i = 0; i < 2; ++i) {
				const auto transformer = tinyCamera.createTransformer(i);

				for (auto j : step(int(vehicles.size())))
				{
					if (vehicles[j].mouseOver())
					{
						ui.selectedVehicleID = j;
						sounds.at(0).play();
					}
				}
			}
		}
	}
}