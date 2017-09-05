#include"Group.h"
#include"Vehicle.h"
#include"Urban.h"
#include"VehicleData.h"

void	initGroups()
{
	JSONReader json(L"‹N“®İ’è.json");
	int numVehicles = json[L"NumVehicles"].getOr<int>(100);
	vehicles.reserve(numVehicles);

	for (int i = 0; i < numVehicles; i++)
		vehicles.emplace_back(vehicleData.choice().id(), &urbans.choice());
}