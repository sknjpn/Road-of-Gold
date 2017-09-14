#include"BiomeData.h"
#include"EnergyData.h"
#include"CitizenData.h"
#include"VehicleData.h"
#include"ItemData.h"
#include"Data.h"

int		BiomeData::id() const { return int(this - &biomeData.front()); }
int		CitizenData::id() const { return int(this - &citizenData.front()); }
int		EnergyData::id() const { return int(this - &energyData.front()); }
int		ItemData::id() const { return int(this - &itemData.front()); }
int		VehicleData::id() const { return int(this - &vehicleData.front()); }

void	loadData()
{
	JSONReader json;

	json.open(L"assets/data/itemData.json");
	for (auto j : json.arrayView()) itemData.emplace_back(j);
	Log(L"ItemDataÇÃì«Ç›çûÇ›äÆóπ size = ", itemData.size());

	json.open(L"assets/data/biomeData.json");
	for (auto j : json.arrayView()) biomeData.emplace_back(j);
	Log(L"BiomeDataÇÃì«Ç›çûÇ›äÆóπ size = ", biomeData.size());

	json.open(L"assets/data/energyData.json");
	for (auto j : json.arrayView()) energyData.emplace_back(j);
	Log(L"EnergyDataÇÃì«Ç›çûÇ›äÆóπ size = ", energyData.size());

	json.open(L"assets/data/citizenData.json");
	for (auto j : json.arrayView()) citizenData.emplace_back(j);
	Log(L"CitizenDataÇÃì«Ç›çûÇ›äÆóπ size = ", citizenData.size());

	json.open(L"assets/data/vehicleData.json");
	for (auto j : json.arrayView()) vehicleData.emplace_back(j);
	Log(L"VehicleDataÇÃì«Ç›çûÇ›äÆóπ size = ", vehicleData.size());
}

CitizenData::CitizenData(const JSONValue& _json)
	: name(_json[L"Name"].getOr<String>(L""))
	, wage(_json[L"Wage"].getOr<int>(0))
	, product(_json[L"Product"])
	, needEnergyType(getEnergyType(_json[L"NeedEnergy"].getString()))
{}
BiomeData::BiomeData(const JSONValue& _json)
	: name(_json[L"Name"].getOr<String>(L""))
	, color(_json[L"Color"].getOr<String>(L"#FFFFFF"))
	, movingCost(_json[L"MovingCost"].getOr<double>(1.0))
	, isSea(_json[L"IsSea"].getOr<bool>(false))
{}
EnergyData::EnergyData(const JSONValue& _json)
	: name(_json[L"Name"].getOr<String>(L""))
{}
ItemData::ItemData(const JSONValue& _json)
	: name(_json[L"Name"].getOr<String>(L""))
	, value(_json[L"Value"].getOr<int>(0))
	, color(_json[L"Color"].getOr<String>(L"#FFFFFF"))
	, icon(_json[L"Icon"].getOr<String>(L""))
{}
VehicleData::VehicleData(const JSONValue& _json)
	: name(_json[L"Name"].getOr<String>(L""))
	, speed(_json[L"Speed"].getOr<double>(1.0))
	, volume(_json[L"Volume"].getOr<int>(100))
	, range(_json[L"Range"].getOr<double>(1.0))
	, isShip(_json[L"IsShip"].getOr<bool>(false))
	, icon(_json[L"Icon"].getOr<String>(L""))
{}
EnergyData*	getEnergyData(const String& _name)
{
	for (auto& e : energyData)
	{
		if (e.name == _name) return &e;
	}
	return nullptr;
}
CitizenData*	getCitizenData(const String& _name)
{
	for (auto& e : citizenData)
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
int	getCitizenType(const String& _name)
{
	for (auto& e : citizenData)
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