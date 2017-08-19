#include"Group.h"
#include"Vehicle.h"
#include"Urban.h"
#include"VehicleData.h"

void	initGroups()
{
	for (int i = 0; i < 100; i++)
		vehicles.emplace_back(vehicleData.choice().id(), &urbans[Random(int(urbans.size() - 1))]);
}