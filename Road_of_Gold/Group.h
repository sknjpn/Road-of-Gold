#pragma once

struct Route;
struct Urban;

struct Vehicle
{
	int		nowUrbanID;
	int		routeID;
	double	routeProgress;

	Vehicle(int _nowUrbanID);
	Urban&	getNowUrban() const;
	Route&	getRoute() const;
	void	update();
	bool	inRoute() const;
	void	draw() const;
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