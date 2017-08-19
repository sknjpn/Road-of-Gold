#include"BiomeData.h"
#include"EnergyData.h"
#include"CitizenData.h"
#include"VehicleData.h"
#include"ItemData.h"

int		BiomeData::id() const { return int(this - &biomeData.front()); }
int		CitizenData::id() const { return int(this - &citizenData.front()); }
int		EnergyData::id() const { return int(this - &energyData.front()); }
int		ItemData::id() const { return int(this - &itemData.front()); }
int		VehicleData::id() const { return int(this - &vehicleData.front()); }
void	loadBiomeData()
{
	JSONReader biomeDataJson(L"Assets/BiomeData.json");
	for (auto json : biomeDataJson[L"BiomeData"].arrayView())
		biomeData.emplace_back(json);
	Log(L"BiomeDataÇÃì«Ç›çûÇ›äÆóπ size = ", biomeData.size());
}
void	loadEnergyData()
{
	JSONReader energyDataJson(L"Assets/EnergyData.json");
	for (auto json : energyDataJson[L"EnergyData"].arrayView())
		energyData.emplace_back(json);
	Log(L"EnergyDataÇÃì«Ç›çûÇ›äÆóπ size = ", energyData.size());
}
void	loadCitizenData()
{
	JSONReader citizenDataJson(L"Assets/CitizenData.json");
	for (auto json : citizenDataJson[L"CitizenData"].arrayView())
		citizenData.emplace_back(json);
	Log(L"CitizenDataÇÃì«Ç›çûÇ›äÆóπ size = ", citizenData.size());
}
void	loadVehicleData()
{
	JSONReader vehicleDataJson(L"Assets/VehicleData.json");
	for (auto json : vehicleDataJson[L"VehicleData"].arrayView())
		vehicleData.emplace_back(json);
	Log(L"VehicleDataÇÃì«Ç›çûÇ›äÆóπ size = ", vehicleData.size());
}
void	loadItemData()
{
	JSONReader itemDataJson(L"Assets/ItemData.json");
	for (auto json : itemDataJson[L"ItemData"].arrayView())
		itemData.emplace_back(json);
	Log(L"ItemDataÇÃì«Ç›çûÇ›äÆóπ size = ", itemData.size());
}

CitizenData::CitizenData(const JSONValue& _json)
	: name(_json[L"Name"].getOr<String>(L""))
	, wage(_json[L"Wage"].getOr<int>(0))
	, cost(_json[L"Cost"].getOr<int>(0))
	, product(_json[L"Product"])
	, needEnergyType(-1)
{
	//needEnergyTypeÇÃéQè∆
	for (auto i : step(int(energyData.size())))
	{
		if (energyData[i].name == _json[L"NeedEnergy"].getOr<String>(L""))
		{
			needEnergyType = i;
			break;
		}
	}

}