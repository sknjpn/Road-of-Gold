#include"Group.h"
#include"Vehicle.h"
#include"Urban.h"
#include"VehicleData.h"

void	initGroups()
{
	if (urbans.isEmpty())
	{
		Output << L"“sŽs‚ª‘¶Ý‚µ‚È‚¢‚½‚ß‚ÉŒðˆÕƒ†ƒjƒbƒg“WŠJ‚ÉŽ¸”s‚µ‚Ü‚µ‚½";
		return;
	}

	INIReader iniReader(L"assets/config.ini");
	int numVehicles = iniReader.getOr<int>(L"Groups", L"NumVehicles", 0);

	vehicles.reserve(numVehicles);
	for (int i = 0; i < numVehicles; i++) vehicles.emplace_back(vehicleData.choice().id(), &urbans.choice());
}