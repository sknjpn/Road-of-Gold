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

struct RouteMaker
{
	struct WayPoint
	{
		Urban* urban;
		Optional<int> buyItemType;

		WayPoint(Urban* _urban)
			: urban(_urban)
			, buyItemType(none)
		{}
	};

	Fleet*	targetFleet;
	Array<WayPoint>	wayPoints;


	RouteMaker()
		: targetFleet(nullptr)
	{}
	void	set(Fleet* _targetFleet);
	void	update();
};
extern RouteMaker routeMaker;

struct Selecter
{
	Urban*	selectedUrban;
	Array<Fleet*> selectedFleets;
	Optional<Vec2>	selectedPos;

	Selecter()
		: selectedUrban(nullptr)
		, selectedPos(none)
	{}
	void	update();
	void	draw() const;
	RectF	selectedRegion() const;
};
extern Selecter	selecter;

struct DisplayUrban
{
	enum struct UrbanInfoState
	{
		Worker,
		Shelves,
		Docks
	} urbanInfoState;
	Stopwatch	openElapsedTime;	//開いてからの経過時間
	Stopwatch	closeElapsedTime;	//閉じてからの経過時間

	DisplayUrban()
		: openElapsedTime(MillisecondsF(10000.0), true)
		, closeElapsedTime(MillisecondsF(10000.0), true)
	{}
	void	update();
};
extern DisplayUrban	displayUrban;

struct DisplayFleets
{
	Stopwatch	openElapsedTime;	//開いてからの経過時間
	Stopwatch	closeElapsedTime;	//閉じてからの経過時間

	DisplayFleets()
		: openElapsedTime(MillisecondsF(10000.0), true)
		, closeElapsedTime(MillisecondsF(10000.0), true)
	{}
	void	update();
};
extern DisplayFleets	displayFleets;

extern Array<Font*>	globalFonts;

void	updateDisplay();
void	updateTimeSpeed();