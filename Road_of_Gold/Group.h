#pragma once
#include"Pos.h"


enum struct Command {
	MOVE,	//“ss‚ÉˆÚ“®
	JUMP,	//ÀsˆÊ’u‚Ì•ÏX
	WAIT,	//Às‹x~
	BUY,	//w“ü
	SELL,	//”Ì”„
};

struct Route;
struct Urban;

struct Stock
{
	Stock() 
		: itemType(0)
		, num(0) 
	{}
	int		itemType;
	int		num;
};

struct Vehicle
{
	int		id;
	int		vehicleType;
	int		joinedGroupID;
	Stock	stock;
	int		nowUrbanID;
	int		routeID;
	double	routeProgress;
	double	sleepTimer;
	int32	progress;
	Array<std::pair<int16, int32>> chain;

	Vehicle(int _nowUrbanID);
	Vec2	getMPos() const;
	void	update();
	void	draw() const;
};
extern Array<Vehicle> vehicles;

struct Group
{
	double	timer;
	Color	color;
	int		id;
	int		money;
	String	name;
	String	description;
	int		moneyLog;	
	Array<int>	ownVehicleIDs;

	Group();
	void	update();
};
extern Array<Group> groups;