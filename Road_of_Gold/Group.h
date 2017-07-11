#pragma once
#include "Item.h"

struct Node;
struct Route;
struct Pos;

struct Vehicle
{
	Vehicle(const int& _nowNodeID);


	Node&	getNowNode() const;
	Route&	getRoute() const;
	Vec2	getPos() const;
	double	getAngle() const;

	Triangle	getShape() const;

	int		nowNodeID;
	int		routeID;
	double	routeProgress;
	Item	item;
};

struct Group
{
	Group(const int& _id);

	int		id;
	String	name;
	Color	color;
	Array<Vehicle> vehicles;
};
extern Array<Group> groups;