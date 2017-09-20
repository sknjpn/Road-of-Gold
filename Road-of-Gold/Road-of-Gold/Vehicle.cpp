#include"Vehicle.h"
#include"VehicleData.h"
#include"Wallet.h"
#include"Pos.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"BiomeData.h"

VehicleData&	Vehicle::data() const { return vehicleData.at(vehicleType); }


Vehicle::Vehicle(int _vehicleType)
	: vehicleType(_vehicleType)
{}
int		Vehicle::id() const { return int(this - &vehicles.front()); }