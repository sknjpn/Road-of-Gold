#pragma once
#include"Vehicle.h"

struct Route;

struct UI
{
	bool	useRouteMenu;
	bool	useUrbanMenu;
	int		urbanDrawState;
	int		selectedUrbanID;
	int		selectedVehicleID;
	int		selectedItemType;
	Array<Font*>	fonts;
	bool	drawExportLineEnabled;
	bool	drawExportImportPowerEnabled;
	Route*	selectedRoute;

	Array<std::pair<Code, int>> newChain;

	UI()
		: urbanDrawState(0)
		, useRouteMenu(false)
		, useUrbanMenu(false)
		, selectedUrbanID(-1)
		, selectedVehicleID(-1)
		, selectedItemType(-1)
		, drawExportLineEnabled(false)
		, drawExportImportPowerEnabled(false)
		, selectedRoute(nullptr)
	{}
};

extern UI	ui;

void	drawUI();
void	selectItem();
void	updateTimeSpeed();