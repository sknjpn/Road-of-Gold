#pragma once
#include"Vehicle.h"

struct Route;

struct UI
{
	int		selectedUrbanID;
	int		selectedVehicleID;
	int		selectedItemType;
	Array<Font*>	fonts;
	bool	drawExportLineEnabled;
	bool	drawExportImportPowerEnabled;
	Route*	selectedRoute;

	Array<std::pair<Code, int>> newChain;

	UI()
		: selectedUrbanID(-1)
		, selectedVehicleID(-1)
		, selectedItemType(-1)
		, drawExportLineEnabled(false)
		, drawExportImportPowerEnabled(false)
		, selectedRoute(nullptr)
	{}
};

extern UI	ui;
extern bool	useMulthThread;

void	drawUI();