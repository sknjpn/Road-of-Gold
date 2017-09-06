#include"Group.h"
#include"Vehicle.h"
#include"Urban.h"
#include"VehicleData.h"

void	initGroups()
{
	INIReader iniReader(L"config.ini");
	int numVehicles = iniReader.getOr<int>(L"Groups", L"NumVehicles", 0);

	vehicles.reserve(numVehicles);
	for (int i = 0; i < numVehicles; i++)
		vehicles.emplace_back(vehicleData.choice().id(), &urbans.choice());
}