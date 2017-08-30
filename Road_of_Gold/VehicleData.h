#pragma once

struct VehicleData
{
	String	name;
	double	speed;
	int		volume;
	double	range;
	bool	isShip;

	VehicleData(const JSONValue& _json)
		: name(_json[L"Name"].getOr<String>(L""))
		, speed(_json[L"Speed"].getOr<double>(1.0))
		, volume(_json[L"Volume"].getOr<int>(100))
		, range(_json[L"Range"].getOr<double>(1.0))
		, isShip(_json[L"IsShip"].getOr<bool>(false))
	{}
	int		id() const;
};

extern Array<VehicleData>	vehicleData;