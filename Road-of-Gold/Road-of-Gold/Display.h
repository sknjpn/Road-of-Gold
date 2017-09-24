#pragma once
#include"Vehicle.h"
#include"Chain.h"

struct Route;
struct Fleet;
struct Urban;

enum struct UIState
{
	None,				//何も表示しない
	UrbanInfo,			//単一のUrbanの情報を表示
	AllUrbansInfo,		//すべてのUrbanの比較情報を表示
	FleetInfo,			//一つの船団に対する編集
	FleetsInfo,			//複数の船団に対する編集
	ExportRelationsInfo,//全都市間の貿易関係
};

struct DisplayUrban
{
	enum struct UrbanInfoState
	{
		Worker,
		Shelves,
	} urbanInfoState;
	Stopwatch	openElapsedTime;	//開いてからの経過時間
	Stopwatch	closeElapsedTime;	//閉じてからの経過時間
	Urban*	selectedUrban;

	void	update();
	DisplayUrban()
		: openElapsedTime(MillisecondsF(10000.0), true)
		, closeElapsedTime(true)
	{}
};


struct Display
{

	Stopwatch	elapsedTime;	//別のUI開いた際の経過時間
	bool	useRouteMenu;
	bool	useUrbanMenu;
	int		urbanDrawState;
	Urban*	selectedUrban;
	int		selectedFleetID;
	int		selectedItemType;
	bool	drawExportLineEnabled;
	bool	drawExportImportPowerEnabled;
	Route*	selectedRoute;
	Chain	newChain;
	TextBox	fleetNameTextBox;
	bool	keyControlBlocked;
	RectF	selectedRegion;
	int		destinationUrbanID;
	int		transportItemType;
	UIState	uiState;
	Array<Fleet*> selectedFleets;
	Array<Font*>	fonts;

	Display();
	void	updateSelectItem();
	void	updateAllUrbansInfo();
	void	updateFleetInfo();
	void	updateFleetsInfo();
	void	updateExportRelationsInfo();
};

extern Display		display;
extern DisplayUrban	displayUrban;

void	drawUI();
void	updateTimeSpeed();