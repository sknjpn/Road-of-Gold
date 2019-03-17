#pragma once
#include <HamFramework.hpp>

Array<String>	itemList =
{
	U"itemData.json",
	U"citizenData.json",
	U"energyData.json",
	U"vehicleData.json"
};

struct GameData
{
	int selectedSceneID = 0;
	int	selectedItemType = 0;
	int	selectedVehicleType = 0;
	int	selectedEnergyType = 0;
	int	selectedCitizenType = 0;
};
using MyApp = SceneManager<String, GameData>;

bool	itemIsExist(const String& _itemName);
bool	energyIsExist(const String& _energyName);