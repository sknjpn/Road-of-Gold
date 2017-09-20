#include"Fleet.h"
#include"Planet.h"
#include"Route.h"
#include"Urban.h"

void	updateFleets()
{
	for (auto& f : fleets)
	{
		double actionTime = planet.timeSpeed;

		for (;;)
		{
			if (f.route != nullptr)	//ŽÀs’†‚Ìƒ‹[ƒg‚ª‚ ‚é‚È‚ç‚Î
			{
				if (f.route->movingCost / f.data.speed <= f.routeProgress + actionTime)
				{
					actionTime -= f.route->movingCost / f.data.speed - f.routeProgress;
					f.timer += f.route->movingCost / f.data.speed - f.routeProgress;
					f.routeProgress = 0;
					f.nowUrban = f.route->toUrban;
					f.route->pullVehicle();
					f.route = nullptr;
					f.chain.readerPos++;
				}
				else
				{
					f.routeProgress += actionTime;
					f.timer += actionTime;
					actionTime = 0;
					break;
				}
			}
			else if (f.sleepTimer > 0)	//‹xŒe’†‚È‚ç‚Î
			{
				if (f.sleepTimer <= actionTime)
				{
					f.sleepTimer = 0.0;
					actionTime -= f.sleepTimer;
					f.timer += f.sleepTimer;
					f.chain.readerPos++;
				}
				else
				{
					f.sleepTimer -= actionTime;
					f.timer += actionTime;
					actionTime = 0;
					break;
				}
			}
			else if (f.stopFlag)	//Ž–‹Æ’âŽ~
			{
				f.stopFlag = false;
				for (auto& e : exports)
				{
					if (e.from == f.exportLog.from &&
						e.to == f.exportLog.to &&
						e.itemType == f.exportLog.itemType)
					{
						e.numItemPerDay -= f.exportLog.numItemPerDay;
						break;
					}
				}
				exports.remove_if([](const Export& e) { return e.numItemPerDay == 0; });
				f.chain.clear();
				for (auto& u : urbans) u.buyers.remove_if([&f](const Buyer& b) { return b.walletID == f.walletID; });

				break;
			}
			else if (f.chain.update(&f)) break;	//error‚Ìê‡’Eo
		}
	}
}