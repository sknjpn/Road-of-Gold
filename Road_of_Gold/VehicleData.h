#pragma once

struct VehicleData
{
	String	name;
	double	speed;
	int		volume;
	double	range;
	bool	isShip;

	VehicleData(const JSONValue& _json);
	int		id() const;
};

extern Array<VehicleData>	vehicleData;

VehicleData*	getVehicleData(const String& _name);
int	getVehicleType(const String& _name);