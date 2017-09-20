#pragma once

#include"Casket.h"

struct Urban;
struct VehicleData;
struct Fleet;

struct Vehicle
{
	int		vehicleType;

	Vehicle(int _vehicleType);
	VehicleData&	data() const;
	int		id() const;
};

extern Array<Vehicle> vehicles;