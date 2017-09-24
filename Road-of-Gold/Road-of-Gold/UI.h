#pragma once
#include"Vehicle.h"
#include"Chain.h"

struct Route;
struct Fleet;

struct UI
{
	bool	useRouteMenu;
	bool	useUrbanMenu;
	int		urbanDrawState;
	int		selectedUrbanID;
	int		selectedFleetID;
	int		selectedItemType;
	Array<Font*>	fonts;
	bool	drawExportLineEnabled;
	bool	drawExportImportPowerEnabled;
	Route*	selectedRoute;
	Chain	newChain;
	TextBox	fleetNameTextBox;
	bool	keyControlBlocked;
	RectF	selectedRegion;
	Array<Fleet*> selectedFleets;
	//autoCode
	int		destinationUrbanID;
	int		transportItemType;

	UI()
		: keyControlBlocked(false)
		, transportItemType(0)
		, destinationUrbanID(0)
		, urbanDrawState(0)
		, useRouteMenu(false)
		, useUrbanMenu(false)
		, selectedUrbanID(-1)
		, selectedFleetID(-1)
		, selectedItemType(-1)
		, drawExportLineEnabled(false)
		, drawExportImportPowerEnabled(false)
		, selectedRoute(nullptr)
		, selectedRegion()
	{}
};

extern UI	ui;

void	drawUI();
void	selectItem();
void	updateTimeSpeed();