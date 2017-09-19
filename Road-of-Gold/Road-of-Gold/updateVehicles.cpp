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

		if (!v.chain.isEmpty() && v.reader < int(v.chain.size()) && !v.isError)
		{
			for (int cnt = 0;; cnt++)
			{
				//エラー処理
				if (cnt == 100) v.isError = true;	//ループから抜けられない場合
				if (v.reader >= int(v.chain.size())) v.isError = true;	//読み込み位置がプログラムから外れている場合
				if (v.isError) break;	//エラーがあるならば抜ける

				if (v.route != nullptr)	//実行中のルートがあるならば
				{
					if (v.route->movingCost / v.data().speed <= v.routeProgress + actionTime)
					{
						actionTime -= v.route->movingCost / v.data().speed - v.routeProgress;
						v.timer += v.route->movingCost / v.data().speed - v.routeProgress;
						v.routeProgress = 0;
						v.nowUrban = v.route->toUrban;
						v.route->pullVehicle();
						v.route = nullptr;
						v.reader++;
					}
					else
					{
						v.routeProgress += actionTime;
						v.timer += actionTime;
						actionTime = 0;
						break;
					}
				}
				else if (v.sleepTimer > 0)	//休憩中ならば
				{
					if (v.sleepTimer <= actionTime)
					{
						v.sleepTimer = 0.0;
						actionTime -= v.sleepTimer;
						v.timer += v.sleepTimer;
						v.reader++;
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
				else
				{
					auto& c = v.chain[v.reader];
					switch (c.first)
					{
					case Code::None:
						v.reader++;
						break;
					case Code::Move:
						for (auto& r : v.nowUrban->ownRoutes)
						{
							if (r->isSeaRoute == v.data().isShip && r->toUrban->id() == c.second && r->movingCost < v.data().range)
							{
								v.route = r;
								r->addVehicle();
								break;
							}
						}
						if (v.route == nullptr)
						{
							v.isError = true;
							Output << L"Chainの指す都市が異常です。";
						}
						break;
					case Code::Wait:
						v.sleepTimer = 1.0;
						break;
					case Code::Jump:
						v.reader = c.second;
						break;
					case Code::Buy:
					{
						bool flag = false;
						int volume = int(v.data().volume / itemData[c.second].volume);

						for (auto& b : v.nowUrban->buyers)
						{
							if (b.walletID == v.walletID)
							{
								b.progress = 0;
								v.cargo = b.casket;
								b.casket.numItem = 0;
								b.target = volume;
								b.period = int(v.period);
								flag = true;
								break;
							}
						}
						if (!flag)
						{
							auto* u = v.nowUrban;
							u->buyers.emplace_back(v.walletID, c.second, int(v.period), volume);
						}
						v.sleepTimer = 0.5;
						break;
					}
					case Code::Sell:
						if (v.cargo.numItem > 0)
						{
							v.nowUrban->sellers.emplace_back(v.walletID, v.cargo, int(v.period));
						}
						v.cargo.numItem = 0;
						v.sleepTimer = 0.5;
						break;
					}
				}
			}
		}
	}
}