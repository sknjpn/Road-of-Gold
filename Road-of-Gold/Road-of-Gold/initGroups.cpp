#include"Group.h"
#include"Vehicle.h"
#include"Urban.h"
#include"VehicleData.h"

void	initGroups()
{
	if (urbans.isEmpty())
	{
		LOG_ERROR(L"“ss‚ª‘¶İ‚µ‚È‚¢‚½‚ß‚ÉŒğˆÕƒ†ƒjƒbƒg“WŠJ‚É¸”s‚µ‚Ü‚µ‚½");
		return;
	}

	INIReader iniReader(L"assets/config.ini");
	int numVehicles = iniReader.getOr<int>(L"Groups", L"NumVehicles", 0);

	vehicles.reserve(numVehicles);
	for (int i = 0; i < numVehicles; i++)
		vehicles.emplace_back(vehicleData.choice().id(), &urbans.choice());
}