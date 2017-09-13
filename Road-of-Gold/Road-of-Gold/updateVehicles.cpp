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
			for (;;)
			{
				if (v.isError) break;
				if (v.route != nullptr)
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
				else if (v.sleepTimer > 0)
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
				else if (v.stopFlag)	//Ž–‹Æ’âŽ~
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
						if (v.reader + 1 == int(v.chain.size()))
						{
							v.isError = true;
						}
						else v.reader++;
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
							LOG_ERROR(L"Chain‚ÌŽw‚·“sŽs‚ªˆÙí‚Å‚·B");
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
						for (auto& b : v.nowUrban->buyers)
						{
							if (b.walletID == v.walletID)
							{
								b.progress = 0;
								v.cargo = b.casket;
								b.casket.numItem = 0;
								b.target = v.maxVolume;
								b.period = v.period;
								flag = true;
								break;
							}
						}
						if (!flag)
						{
							v.nowUrban->buyers.emplace_back(v.walletID, c.second, int(v.period), v.maxVolume);
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
					case Code::MVol:
						v.maxVolume = c.second;
						v.reader++;
						break;
					}
				}
			}
		}
	}

	//SelectVehicle
	if (MouseL.down())
	{
		if (!Rect(480, Window::Size().y).mouseOver() || ui.selectedVehicleID == -1)
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
							ui.newChain = vehicles[j].chain;
						}
					}
				}
			}
		}
	}
}