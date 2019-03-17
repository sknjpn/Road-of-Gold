#include"BiomeData.h"
#include"EnergyData.h"
#include"VehicleData.h"
#include"ItemData.h"
Array<EnergyData>	energyData;
Array<BiomeData>	biomeData;
Array<VehicleData>	vehicleData;
Array<ItemData>		itemData;

int		BiomeData::id() const { return int(this - &biomeData.front()); }
int		EnergyData::id() const { return int(this - &energyData.front()); }
int		VehicleData::id() const { return int(this - &vehicleData.front()); }
int		ItemData::id() const { return int(this - &itemData.front()); }

void	loadData()
{
	JSONReader json;

	json.open(U"assets/data/itemData.json");
	for (auto j : json.arrayView())
		itemData.emplace_back(j);
	Logger << U"ItemData‚Ì“Ç‚Ýž‚ÝŠ®—¹ size = " << itemData.size();

	json.open(U"assets/data/biomeData.json");
	for (auto j : json.arrayView())
		biomeData.emplace_back(j);
	Logger << U"BiomeData‚Ì“Ç‚Ýž‚ÝŠ®—¹ size = " << biomeData.size();

	json.open(U"assets/data/energyData.json");
	for (auto j : json.arrayView())
		energyData.emplace_back(j);
	Logger << U"EnergyData‚Ì“Ç‚Ýž‚ÝŠ®—¹ size = " << energyData.size();

	json.open(U"assets/data/vehicleData.json");
	for (auto j : json.arrayView())
		vehicleData.emplace_back(j);
	Logger << U"VehicleData‚Ì“Ç‚Ýž‚ÝŠ®—¹ size = " << vehicleData.size();
}

BiomeData::BiomeData(const JSONValue& _json)
	: name(_json[U"Name"].getOr<String>(U""))
	, nameJP(_json[U"NameJP"].getOr<String>(U""))
	, color(_json[U"Color"].getOr<String>(U"#FFFFFF"))
	, movingCost(_json[U"MovingCost"].getOr<double>(1.0))
	, isSea(_json[U"IsSea"].getOr<bool>(false))
{}
EnergyData::EnergyData(const JSONValue& _json)
	: name(_json[U"Name"].getOr<String>(U""))
	, icon(_json[U"Icon"].getOr<String>(U""))
{}
ItemData::ItemData(const JSONValue& _json)
	: name(_json[U"Name"].getOr<String>(U""))
	, value(_json[U"Value"].getOr<int>(0))
	, color(_json[U"Color"].getOr<String>(U"#FFFFFF"))
	, icon(_json[U"Icon"].getOr<String>(U""))
{}
VehicleData::VehicleData(const JSONValue& _json)
	: name(_json[U"Name"].getOr<String>(U""))
	, speed(_json[U"Speed"].getOr<double>(1.0))
	, volume(_json[U"Volume"].getOr<int>(100))
	, range(_json[U"Range"].getOr<double>(1.0))
	, isShip(_json[U"IsShip"].getOr<bool>(false))
{}
EnergyData*	getEnergyData(const String& _name)
{
	for (auto& e : energyData)
	{
		if (e.name == _name) return &e;
	}
	return nullptr;
}
BiomeData*	getBiomeData(const String& _name)
{
	for (auto& e : biomeData)
	{
		if (e.name == _name) return &e;
	}
	return nullptr;
}
ItemData*	getItemData(const String& _name)
{
	for (auto& e : itemData)
	{
		if (e.name == _name) return &e;
	}
	return nullptr;
}
VehicleData*	getVehicleData(const String& _name)
{
	for (auto& e : vehicleData)
	{
		if (e.name == _name) return &e;
	}
	return nullptr;
}
int	getEnergyType(const String& _name)
{
	for (auto& e : energyData)
	{
		if (e.name == _name) return e.id();
	}
	return -1;
}
int	getBiomeType(const String& _name)
{
	for (auto& e : biomeData)
	{
		if (e.name == _name) return e.id();
	}
	return -1;
}
int	getItemType(const String& _name)
{
	for (auto& e : itemData)
	{
		if (e.name == _name) return e.id();
	}
	return -1;
}
int	getVehicleType(const String& _name)
{
	for (auto& e : vehicleData)
	{
		if (e.name == _name) return e.id();
	}
	return -1;
}