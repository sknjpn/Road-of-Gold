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
	int		joinedGroupID;
	Stock	stock;
	int		nowUrbanID;
	int		routeID;
	double	routeProgress;
	double	sleepTimer;
	int32	progress;
	Array<std::pair<int16, int32>> chain;

	Vehicle(int _nowUrbanID);
	Urban&	getNowUrban() const;
	Route&	getRoute() const;
	Vec2	getMPos() const;
	void	update();
	void	draw() const;
	bool	inRoute() const;
};

struct Group
{
	Color	color;
	int		id;
	int		money;
	String	name;
	Array<Vehicle> vehicles;

	Group();
	void	update();
};
extern Array<Group> groups;