#include"Group.h"
#include"Vehicle.h"
#include"Urban.h"
#include"VehicleData.h"
#include"Fleet.h"
#include"Route.h"

void	initGroups()
{
	if (urbans.isEmpty())
	{
		Output << L"“sŽs‚ª‘¶Ý‚µ‚È‚¢‚½‚ß‚ÉŒðˆÕƒ†ƒjƒbƒg“WŠJ‚ÉŽ¸”s‚µ‚Ü‚µ‚½";
		return;
	}

	vehicles.reserve(65536);

	INIReader iniReader(L"assets/config.ini");
	int numVehicles = iniReader.getOr<int>(L"Groups", L"NumVehicles", 0);

	for (int j = 0; j < numVehicles; j++)
	{
		for (;;)
		{
			auto* u = &urbans.choice();
			int i = vehicleData.choice().id();
			bool flag = false;
			for (auto* r : u->ownRoutes)
			{
				if (r->movingCost < vehicleData[i].range || r->isSeaRoute == vehicleData[i].isShip)
				{
					flag = true;
					fleets.emplace_back(i, u);
					break;
				}
			}
			if (flag) break;
		}
	}
}