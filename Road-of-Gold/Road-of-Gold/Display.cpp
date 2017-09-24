#include"Display.h"


Display::Display()
	: keyControlBlocked(false)
	, transportItemType(0)
	, destinationUrbanID(0)
	, urbanDrawState(0)
	, useRouteMenu(false)
	, useUrbanMenu(false)
	, selectedUrban(nullptr)
	, selectedFleetID(-1)
	, selectedItemType(-1)
	, drawExportLineEnabled(false)
	, drawExportImportPowerEnabled(false)
	, selectedRoute(nullptr)
	, selectedRegion()
	, elapsedTime(true)
	, uiState(UIState::None)
{}