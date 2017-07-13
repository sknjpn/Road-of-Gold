#pragma once

struct Route;
struct Pos;
struct Urban;

struct Vehicle
{
	Vehicle(const int& _nowUrbanID);


	Urban&	getNowUrban() const;
	Route&	getRoute() const;
	Vec2	getPos() const;
	double	getAngle() const;
	bool	inRoute() const;
	void	draw() const;

	int		nowUrbanID;
	int		routeID;
	double	routeProgress;
};

struct Group
{
	Group();

	int		money;
	int		id;
	String	name;
	Color	color;
	Array<Vehicle> vehicles;
};
extern Array<Group> groups;
void makeGroupsRandom();