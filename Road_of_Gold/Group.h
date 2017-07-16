#pragma once

struct Route;
struct Pos;
struct Urban;

struct Vehicle
{
	Vehicle(int _nowUrbanID);


	Urban&	getNowUrban() const;
	Route&	getRoute() const;
	void	update();
	bool	inRoute() const;
	void	draw() const;

	int		nowUrbanID;
	int		routeID;
	double	routeProgress;
};

struct Group
{
	Group();
	void	update();

	int		money;
	int		id;
	String	name;
	Color	color;
	Array<Vehicle> vehicles;
};
extern Array<Group> groups;
void makeGroupsRandom();