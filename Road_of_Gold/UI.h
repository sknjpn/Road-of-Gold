#pragma once

struct UI
{
	int		selectedUrbanID;
	int		selectedVehicleID;
	int		selectedItemType;
	Array<Font*>	fonts;
	bool	drawExportLineEnabled;
	bool	drawExportImportPowerEnabled;

	UI()
		: selectedUrbanID(-1)
		, selectedVehicleID(-1)
		, selectedItemType(-1)
		, drawExportLineEnabled(false)
		, drawExportImportPowerEnabled(false)
	{}
};

extern UI	ui;
extern bool	useMulthThread;

void	drawUI();