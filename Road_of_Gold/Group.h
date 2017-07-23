#pragma once

struct Route;
struct Urban;

struct Vehicle
{
	int		nowUrbanID;
	int		routeID;
	double	routeProgress;

	int		progress;
	Array<std::pair<int16, int32>> chain;

	Vehicle(int _nowUrbanID);
	Urban&	getNowUrban() const;
	Route&	getRoute() const;
	void	update();
	void	draw() const;
	bool	inRoute() const;
};

struct Group
{
	int		id;
	int		money;
	String	name;
	Array<Vehicle> vehicles;

	Group();
	void	update();
};
extern Array<Group> groups;